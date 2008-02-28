#include "StaticChecker.h"
#include "Package.h"
#include "Class.h"
#include "Variable.h"
#include "Function.h"
#include "TypeLookup.h"
#include "Statement.h"
#include "AllExprs.h"
#include "TypeConversion.h"

namespace SS {

StaticChecker::StaticChecker(Package* pkg) : m_pkg(pkg)
{
	SSAssert(pkg != 0);
}

bool StaticChecker::Check()
{
	Package* pkg = m_pkg;

	// Check the package name
	if(!IsValidName(pkg->GetName()))
	{
		ReportError("Invalid package name: %s", SS_QUOTE(pkg->GetName()));
		return false;
	}

	// TODO: Load up other packages. 
	// Watch out for duplicate package names, package names aliasing/referencing current package,
	// infinite recursion (maybe not possible)

	// Check all classes for name conflicts, add them to member list if they pass.
	for(int i = 0; i < pkg->GetClassCount(); ++i)
	{
		Class* cls = pkg->GetClass(i);

		if(!CheckChild(pkg, cls))
			continue;

		if(cls->GetAccess() == ACCESS_PUBLIC)
			cls->SetAccessDomain(AccessDomain::Universal());
		else if(cls->GetAccess() == ACCESS_PRIVATE)
			cls->SetAccessDomain(AccessDomain::Lexical(pkg));
		else
			SS_UNREACHABLE;

		cls->SetParent(m_pkg);
		cls->SetPackage(m_pkg);
		pkg->GetScope()->Add(cls);
	}

	// Quit if we hit any errors.
	if(AnyErrors())
		return false;

	// Resolve all bases for classes.
	for(int i = 0; i < pkg->GetClassCount(); ++i)
		ResolveClassBase(pkg->GetClass(i));

	// Quit if we hit any errors.
	if(AnyErrors())
		return false;

	// At this point all classes have their bases set up properly,
	// and they're known not to have cycles.
	// Now we have to process all of the members of each class, starting with
	// the base classes and moving upwards...
	for(int i = 0; i < pkg->GetClassCount(); ++i)
		ProcessClassMembersRecursive(pkg->GetClass(i));

	if(AnyErrors())
		return false;

	// TODO: Somewhere in here we need to work out overloading, function overriding, etc.

	// Classes and their vars, functions, and inheritance have been worked out;
	// now we need to go through statements & expressions.
	for(int i = 0; i < pkg->GetClassCount(); ++i)
	{
		Class* cls = pkg->GetClass(i);
		for(int j=0; j < cls->GetFunctionCount(); ++j)
			CheckFunctionBody(cls->GetFunction(j));
	}

	if(AnyErrors())
		return false;

	return true;
}


void StaticChecker::ResolveClassBase(Class* cls)
{	
	// TODO: Why's this giving zero???

	if(cls->GetBaseExpr() == 0)
	{
		// TODO: Set object as base class if there's no base expr
		return;
	}

	// This will do error messages itself
	Type* typ = TypeLookup::LookupType(cls->GetBaseExpr(), m_pkg);
	if(!typ)
		return;
		
	// Check that it is a class.
	// TODO: Allow "object" as a base class!
	if(!typ->IsA<Class>())
	{
		ReportError(cls, "Base must be another class");
		return;
	}

	// Check that we are not setting up a cycle
	// (e.g., make sure that cls is not in type's bases)
	Class* baseClass = dynamic_cast<Class*>(typ);
	Class* currBase = baseClass;
	while(currBase != 0)
	{
		if(currBase == cls)
		{
			ReportError(cls, "Inheritance cycle!");
			return;
		}

		currBase = currBase->GetBase();
	}

	cls->SetBase(baseClass);
}

void StaticChecker::ProcessClassMembersRecursive(Class* cls)
{
	SSAssert(cls != 0);
	
	// TODO: Don't want to process classes from a different package!
	SSAssert(cls->GetPackage() != 0);
	if(cls->GetPackage() != m_pkg)
		return;

	// If we're already processed, quit.
	if(cls->IsProcessed())
		return;

	// If we have a base class, make sure its members are processed first!
	if(cls->GetBase() != 0 && cls->GetBase()->GetPackage() == m_pkg)
	{
		ProcessClassMembersRecursive(cls->GetBase());

		// If processing it failed, quit.
		if(cls->GetBase()->Failed())
		{
			cls->SetFailed(true);
			return;
		}
	}

	if(!ProcessClassMembers(cls))
	{
		cls->SetFailed(true);
		cls->SetProcessed(true);
		return;
	}
	else
		cls->SetProcessed(true);
}

bool StaticChecker::ProcessClassMembers(Class* cls)
{
	bool errors = false;

	// Here, all global types are known, but the members of classes are not.
	// We need to go through & make sure they are all semantically OK,
	// as far as names, types, flags, etc.
	// Processing the init exprs. for vars and the bodies of functions is more complicated
	// and will come later.

	// TODO: Shadowing checks for stuff

	for(int i = 0; i < cls->GetVariableCount(); ++i)
	{
		Variable* var = cls->GetVariable(i);

		if(!CheckChild(cls, var))
		{
			errors = true;
			continue;
		}

		if(var->GetAccess() == ACCESS_PUBLIC)
			var->SetAccessDomain(cls->GetAccessDomain());
		else if(var->GetAccess() == ACCESS_PROTECTED)
			var->SetAccessDomain(AccessDomain::ClassAndSubclasses(cls));
		else if(var->GetAccess() == ACCESS_PRIVATE)
			var->SetAccessDomain(AccessDomain::Lexical(cls));
		else
			SS_UNREACHABLE;

		var->SetParent(cls);

		if(!CheckVariable(var))
		{
			errors = true;
			continue;
		}

		// TODO: We have to check against the base class's scope

		// Try to fit it into the current scope
		cls->GetScope()->Add(var);
	}

	for(int i = 0; i < cls->GetFunctionCount(); ++i)
	{
		Function* func = cls->GetFunction(i);

		if(!CheckChild(cls, func))
		{
			errors = true;
			continue;
		}

		if(func->GetAccess() == ACCESS_PUBLIC)
			func->SetAccessDomain(cls->GetAccessDomain());
		else if(func->GetAccess() == ACCESS_PROTECTED)
			func->SetAccessDomain(AccessDomain::ClassAndSubclasses(cls));
		else if(func->GetAccess() == ACCESS_PRIVATE)
			func->SetAccessDomain(AccessDomain::Lexical(cls));
		else
			SS_UNREACHABLE;

		func->SetParent(cls);

		if(!CheckFunction(func))
		{
			errors = true;
			continue;
		}

		// TODO: We have to check against the base class's scope

		cls->GetScope()->Add(func);
	}

	return !errors;
}

bool StaticChecker::CheckVariable(Variable* var)
{
	SSAssert(var->GetParent() != 0);
	SSAssert(var->GetTypeExpr() != 0);

	if(var->IsStatic() && var->IsConst())
	{
		ReportError(var, "Cannot combine const and static flags");
		return false;
	}

	// This prints its own errors
	Type* typ = TypeLookup::LookupType(var->GetTypeExpr(), var->GetParent());
	if(!typ)
		return false;

	
	if(typ == Type::GetBasicType(BT_VOID))
	{
		ReportError(var, "Void cannot be used for variable types");
		return false;
	}

	var->SetType(typ);


	return true;
}

bool StaticChecker::CheckFunction(Function* func)
{
	SSAssert(func->GetParent() != 0);
	SSAssert(func->GetReturnTypeExpr() != 0);

	Type* returnType = TypeLookup::LookupType(func->GetReturnTypeExpr(), func->GetParent());
	if(!returnType)
		return false;
	func->SetReturnType(returnType);

	if(func->IsStatic() && func->IsSealed())
	{
		ReportError(func, "Cannot have sealed static funcs");
		return false;
	}

	if(func->IsStatic() && func->IsVirtual())
	{
		ReportError(func, "Cannot have virtual static funcs");
		return false;
	}

	if(func->IsSealed() && func->IsVirtual())
	{
		ReportError(func, "Cannot have sealed virtual funcs");
		return false;
	}

	// Now, we have to check params
	bool paramsFailed = false;
	for(int i = 0; i < func->GetParameterCount(); ++i)
	{
		Parameter* param = func->GetParameter(i);
		
		if(!CheckChild(func, param))
		{
			paramsFailed = true;
			continue;
		}

		param->SetParent(func);

		Type* paramType = TypeLookup::LookupType(param->GetTypeExpr(), param->GetParent());
		if(!paramType)
		{
			paramsFailed = true;
			continue;
		}
		param->SetType(paramType);

		func->GetScope()->Add(param);
	}

	if(paramsFailed)
		return false;

	return true;
}

bool StaticChecker::CheckChild(Node* parent, Node* child)
{
	SSAssert(parent != 0);
	SSAssert(child != 0);
	SSAssert(parent->HasName());
	SSAssert(child->HasName());

	if(!IsValidName(child->GetName()))
	{
		// TODO: Can we do better than this? E.g. say what's wrong with the name?
		ReportError(child, "Invalid name %s", SS_QUOTE(child->GetName()));
		return false;
	}

	if(NamesEqual(parent->GetName(), child->GetName()))
	{
		ReportError(child, "Name %s conflicts with name of enclosing object %s",
							SS_QUOTE(child->GetName()),
							parent->GetDesc().c_str());
		return false;
	}

	Node* conflict = parent->GetScope()->Find(child->GetName());
	if(conflict != 0)
	{
		// TODO: Better error message
		ReportError(child, "Name %s conflicts with %s",
							SS_QUOTE(child->GetName()),
							conflict->GetDesc().c_str());
		return false;
	}

	return true;
}

void StaticChecker::CheckFunctionBody(Function* func)
{
	StatementContext ctx;
	Scope scope;
	ctx.func = func;
	ctx.scope = &scope;

	// TODO: Don't compile abstract funcs, of course -- etc.
	SSAssert(func->GetBody());
	CheckStatement(func->GetBody(), ctx);
}

void StaticChecker::CheckStatement(Statement* stmt, const StatementContext& ctx)
{
	if(stmt->IsA<EmptyStatement>())
	{
		// Nothing to do :)
	}
	else if(stmt->IsA<BreakStatement>())
	{
		// Check that break makes sense here
		if(!ctx.canBreak)
			ReportError(stmt, "Break statement is not valid here");
	}
	else if(stmt->IsA<ContinueStatement>())
	{
		// Check that continue makes sense here
		if(!ctx.canContinue)
			ReportError(stmt, "Continue statement is not valid here");
	}
	else if(stmt->IsA<ReturnStatement>())
	{
		ReturnStatement* returnStmt = dynamic_cast<ReturnStatement*>(stmt);

		// Function returns void
		if(ctx.func->GetReturnType() == BasicType::GetBasicType(BT_VOID))
		{
			if(returnStmt->GetExpr() != 0)
				ReportError(stmt, "Cannot specify a return value for a void function");
		}
		else
		{
			if(returnStmt->GetExpr() == 0)
				ReportError(stmt, "Must specify a return value");
			else
			{
				// TODO: Check expr is of correct type
				SSAssert(0);
			}
		}
	}
	else if(stmt->IsA<ThrowStatement>())
	{
		// TODO: Check that the expr is valid, and that it produces some kind of object we can throw
		SSAssert(0);
	}
	else if(stmt->IsA<BlockStatement>())
	{
		BlockStatement* blockStmt = dynamic_cast<BlockStatement*>(stmt);

		StatementContext newCtx(ctx);
		Scope scope;
		newCtx.scope = &scope;
		newCtx.parent = &ctx;

		for(int i=0; i < blockStmt->GetStatementCount(); ++i)
			CheckStatement(blockStmt->GetStatement(i), newCtx);
	}
	else if(stmt->IsA<ExprStatement>())
	{
		ExprStatement* exprStmt = dynamic_cast<ExprStatement*>(stmt);

		std::cout << DumpExpr(exprStmt->GetExpr()) << std::endl;

		ExprContext exprCtx;
		exprCtx.stmtCtx = &ctx;
		exprCtx.lvalue = false;

		Expr* xformed = CheckAndTransformExpr(exprStmt->GetExpr(), exprCtx);
		exprStmt->SetExpr(xformed);

		// TODO: Make sure return type of expr isn't a "pseudovalue" like, say, a function
		//		 (which currently isn't in the type system, and thus can't properly
		//		  be 'popped')
	}
	// todo: if
	// todo: while
	// todo: do-while
	// todo: for
	// todo: foreach
	// todo: switch
	// todo: try-catch
	// todo: variable def.
	else
	{
		// Not implemented...
		SSAssert(0);
	}
}

#define OVERLOAD_BEGIN(op_,name_) \
	case op_: { \
		static bool firstTime = true; \
		static OverloadVector candidates; \
		const char* name = name_; \
		if(firstTime) { \
		firstTime = false; 

#define OVERLOAD_ADD(lhs,rhs,retval) \
	{ \
		static OverloadCandidate cand; \
		cand.userData = static_cast<Type*>(retval); \
		cand.name = name; \
		cand.types.push_back(lhs); \
		cand.types.push_back(rhs); \
		candidates.push_back(&cand); }

#define OVERLOAD_ADD_UN(opType,retval) \
	{ \
		static OverloadCandidate cand; \
		cand.userData = static_cast<Type*>(retval); \
		cand.name = name; \
		cand.types.push_back(opType); \
		candidates.push_back(&cand); }

#define OVERLOAD_END  } return candidates; }


const OverloadVector& StaticChecker::GetBinaryOverloads(BinaryOp op) const
{
	switch(op)
	{	

	// TODO: Return types

	OVERLOAD_BEGIN(BINOP_ADD, "+")
		OVERLOAD_ADD(SS_T_S4, SS_T_S4, SS_T_S4)
		OVERLOAD_ADD(SS_T_U4, SS_T_U4, SS_T_U4)
		OVERLOAD_ADD(SS_T_S8, SS_T_S8, SS_T_S8)
		OVERLOAD_ADD(SS_T_U8, SS_T_U8, SS_T_U8)
		OVERLOAD_ADD(SS_T_FLOAT, SS_T_FLOAT, SS_T_FLOAT)
		OVERLOAD_ADD(SS_T_DOUBLE, SS_T_DOUBLE, SS_T_DOUBLE)
		OVERLOAD_ADD(SS_T_STRING, SS_T_STRING, SS_T_STRING)
	OVERLOAD_END

	OVERLOAD_BEGIN(BINOP_SUB, "-")
		OVERLOAD_ADD(SS_T_S4, SS_T_S4, SS_T_S4)
		OVERLOAD_ADD(SS_T_U4, SS_T_U4, SS_T_U4)
		OVERLOAD_ADD(SS_T_S8, SS_T_S8, SS_T_S8)
		OVERLOAD_ADD(SS_T_U8, SS_T_U8, SS_T_U8)
		OVERLOAD_ADD(SS_T_FLOAT, SS_T_FLOAT, SS_T_FLOAT)
		OVERLOAD_ADD(SS_T_DOUBLE, SS_T_DOUBLE, SS_T_DOUBLE)
	OVERLOAD_END

	OVERLOAD_BEGIN(BINOP_MUL, "*")
		OVERLOAD_ADD(SS_T_S4, SS_T_S4, SS_T_S4)
		OVERLOAD_ADD(SS_T_U4, SS_T_U4, SS_T_U4)
		OVERLOAD_ADD(SS_T_S8, SS_T_S8, SS_T_S8)
		OVERLOAD_ADD(SS_T_U8, SS_T_U8, SS_T_U8)
		OVERLOAD_ADD(SS_T_FLOAT, SS_T_FLOAT, SS_T_FLOAT)
		OVERLOAD_ADD(SS_T_DOUBLE, SS_T_DOUBLE, SS_T_DOUBLE)
	OVERLOAD_END

	OVERLOAD_BEGIN(BINOP_DIV, "/")
		OVERLOAD_ADD(SS_T_S4, SS_T_S4, SS_T_S4)
		OVERLOAD_ADD(SS_T_U4, SS_T_U4, SS_T_U4)
		OVERLOAD_ADD(SS_T_S8, SS_T_S8, SS_T_S8)
		OVERLOAD_ADD(SS_T_U8, SS_T_U8, SS_T_U8)
		OVERLOAD_ADD(SS_T_FLOAT, SS_T_FLOAT, SS_T_FLOAT)
		OVERLOAD_ADD(SS_T_DOUBLE, SS_T_DOUBLE, SS_T_DOUBLE)
	OVERLOAD_END

	OVERLOAD_BEGIN(BINOP_MOD, "%")
		OVERLOAD_ADD(SS_T_S4, SS_T_S4, SS_T_S4)
		OVERLOAD_ADD(SS_T_U4, SS_T_U4, SS_T_U4)
		OVERLOAD_ADD(SS_T_S8, SS_T_S8, SS_T_S8)
		OVERLOAD_ADD(SS_T_U8, SS_T_U8, SS_T_U8)

		// Works on floats too, a la C#
		OVERLOAD_ADD(SS_T_FLOAT, SS_T_FLOAT, SS_T_FLOAT)
		OVERLOAD_ADD(SS_T_DOUBLE, SS_T_DOUBLE, SS_T_DOUBLE)
	OVERLOAD_END

	OVERLOAD_BEGIN(BINOP_POW, "**")
		// TODO: Should it work on ints?
		OVERLOAD_ADD(SS_T_FLOAT, SS_T_FLOAT, SS_T_FLOAT)
		OVERLOAD_ADD(SS_T_DOUBLE, SS_T_DOUBLE, SS_T_DOUBLE)
	OVERLOAD_END

	OVERLOAD_BEGIN(BINOP_BIT_AND, "&")
		OVERLOAD_ADD(SS_T_S4, SS_T_S4, SS_T_S4)
		OVERLOAD_ADD(SS_T_U4, SS_T_U4, SS_T_U4)
		OVERLOAD_ADD(SS_T_S8, SS_T_S8, SS_T_S8)
		OVERLOAD_ADD(SS_T_U8, SS_T_U8, SS_T_U8)
	OVERLOAD_END

	OVERLOAD_BEGIN(BINOP_BIT_OR, "|")
		OVERLOAD_ADD(SS_T_S4, SS_T_S4, SS_T_S4)
		OVERLOAD_ADD(SS_T_U4, SS_T_U4, SS_T_U4)
		OVERLOAD_ADD(SS_T_S8, SS_T_S8, SS_T_S8)
		OVERLOAD_ADD(SS_T_U8, SS_T_U8, SS_T_U8)
	OVERLOAD_END

	OVERLOAD_BEGIN(BINOP_BIT_XOR, "^")
		OVERLOAD_ADD(SS_T_S4, SS_T_S4, SS_T_S4)
		OVERLOAD_ADD(SS_T_U4, SS_T_U4, SS_T_U4)
		OVERLOAD_ADD(SS_T_S8, SS_T_S8, SS_T_S8)
		OVERLOAD_ADD(SS_T_U8, SS_T_U8, SS_T_U8)
	OVERLOAD_END

	OVERLOAD_BEGIN(BINOP_LOG_AND, "&&")
		OVERLOAD_ADD(SS_T_BOOL, SS_T_BOOL, SS_T_BOOL)
	OVERLOAD_END

	OVERLOAD_BEGIN(BINOP_LOG_OR, "||")
		OVERLOAD_ADD(SS_T_BOOL, SS_T_BOOL, SS_T_BOOL)
	OVERLOAD_END

	OVERLOAD_BEGIN(BINOP_SHL, "<<")
		OVERLOAD_ADD(SS_T_S4, SS_T_S4, SS_T_S4)
		OVERLOAD_ADD(SS_T_U4, SS_T_S4, SS_T_U4)
		OVERLOAD_ADD(SS_T_S8, SS_T_S4, SS_T_S8)
		OVERLOAD_ADD(SS_T_U8, SS_T_S4, SS_T_U8)
	OVERLOAD_END

	OVERLOAD_BEGIN(BINOP_SHR, ">>")
		OVERLOAD_ADD(SS_T_S4, SS_T_S4, SS_T_S4)
		OVERLOAD_ADD(SS_T_U4, SS_T_S4, SS_T_U4)
		OVERLOAD_ADD(SS_T_S8, SS_T_S4, SS_T_S8)
		OVERLOAD_ADD(SS_T_U8, SS_T_S4, SS_T_U8)
	OVERLOAD_END

	OVERLOAD_BEGIN(BINOP_LT, "<")
		OVERLOAD_ADD(SS_T_S4, SS_T_S4, SS_T_S4)
		OVERLOAD_ADD(SS_T_U4, SS_T_U4, SS_T_U4)
		OVERLOAD_ADD(SS_T_S8, SS_T_S8, SS_T_S8)
		OVERLOAD_ADD(SS_T_U8, SS_T_U8, SS_T_U8)
		OVERLOAD_ADD(SS_T_FLOAT, SS_T_FLOAT, SS_T_FLOAT)
		OVERLOAD_ADD(SS_T_DOUBLE, SS_T_DOUBLE, SS_T_DOUBLE)
	OVERLOAD_END

	OVERLOAD_BEGIN(BINOP_LE, "<=")
		OVERLOAD_ADD(SS_T_S4, SS_T_S4, SS_T_S4)
		OVERLOAD_ADD(SS_T_U4, SS_T_U4, SS_T_U4)
		OVERLOAD_ADD(SS_T_S8, SS_T_S8, SS_T_S8)
		OVERLOAD_ADD(SS_T_U8, SS_T_U8, SS_T_U8)
		OVERLOAD_ADD(SS_T_FLOAT, SS_T_FLOAT, SS_T_FLOAT)
		OVERLOAD_ADD(SS_T_DOUBLE, SS_T_DOUBLE, SS_T_DOUBLE)
	OVERLOAD_END
	
	OVERLOAD_BEGIN(BINOP_GT, ">")
		OVERLOAD_ADD(SS_T_S4, SS_T_S4, SS_T_S4)
		OVERLOAD_ADD(SS_T_U4, SS_T_U4, SS_T_U4)
		OVERLOAD_ADD(SS_T_S8, SS_T_S8, SS_T_S8)
		OVERLOAD_ADD(SS_T_U8, SS_T_U8, SS_T_U8)
		OVERLOAD_ADD(SS_T_FLOAT, SS_T_FLOAT, SS_T_FLOAT)
		OVERLOAD_ADD(SS_T_DOUBLE, SS_T_DOUBLE, SS_T_DOUBLE)
	OVERLOAD_END

	OVERLOAD_BEGIN(BINOP_GE, ">=")
		OVERLOAD_ADD(SS_T_S4, SS_T_S4, SS_T_S4)
		OVERLOAD_ADD(SS_T_U4, SS_T_U4, SS_T_U4)
		OVERLOAD_ADD(SS_T_S8, SS_T_S8, SS_T_S8)
		OVERLOAD_ADD(SS_T_U8, SS_T_U8, SS_T_U8)
		OVERLOAD_ADD(SS_T_FLOAT, SS_T_FLOAT, SS_T_FLOAT)
		OVERLOAD_ADD(SS_T_DOUBLE, SS_T_DOUBLE, SS_T_DOUBLE)
	OVERLOAD_END

	default:
		{
			SSAssert(0 && "Unknown/unimplemented binary op");
			static OverloadVector dummy;
			return dummy; // Quiet, compiler
		}
	}
}


const OverloadVector& StaticChecker::GetUnaryOverloads(UnaryOp op) const
{
	switch(op)
	{	

	// TODO: Return types

	OVERLOAD_BEGIN(UNOP_LOG_NOT, "!")
		OVERLOAD_ADD_UN(SS_T_BOOL, SS_T_BOOL)
	OVERLOAD_END

	OVERLOAD_BEGIN(UNOP_COMPLEMENT, "~")
		OVERLOAD_ADD_UN(SS_T_S4, SS_T_S4)
		OVERLOAD_ADD_UN(SS_T_U4, SS_T_U4)
		OVERLOAD_ADD_UN(SS_T_S8, SS_T_S8)
		OVERLOAD_ADD_UN(SS_T_U8, SS_T_U8)
	OVERLOAD_END

	OVERLOAD_BEGIN(UNOP_NEGATIVE, "-")
		OVERLOAD_ADD_UN(SS_T_S4, SS_T_S4)
		OVERLOAD_ADD_UN(SS_T_U4, SS_T_U4)
		OVERLOAD_ADD_UN(SS_T_S8, SS_T_S8)
		OVERLOAD_ADD_UN(SS_T_U8, SS_T_U8)
		OVERLOAD_ADD_UN(SS_T_FLOAT, SS_T_FLOAT)
		OVERLOAD_ADD_UN(SS_T_DOUBLE, SS_T_DOUBLE)
	OVERLOAD_END

	OVERLOAD_BEGIN(UNOP_POSITIVE, "+")
		OVERLOAD_ADD_UN(SS_T_S4, SS_T_S4)
		OVERLOAD_ADD_UN(SS_T_U4, SS_T_U4)
		OVERLOAD_ADD_UN(SS_T_S8, SS_T_S8)
		OVERLOAD_ADD_UN(SS_T_U8, SS_T_U8)
		OVERLOAD_ADD_UN(SS_T_FLOAT, SS_T_FLOAT)
		OVERLOAD_ADD_UN(SS_T_DOUBLE, SS_T_DOUBLE)
	OVERLOAD_END

	default:
		{
			SSAssert(0 && "Unknown/unimplemented unary op");
			static OverloadVector dummy;
			return dummy; // quiet, compiler
		}
	}
}


Expr* StaticChecker::CheckAndTransformExpr(Expr* expr, const ExprContext& ctx)
{
	Function* func = ctx.stmtCtx->func;
	Scope* scope = ctx.stmtCtx->scope;

	//---------------------------------------------------------------
	// LITERAL EXPRESSIONS
	//---------------------------------------------------------------
	if(expr->IsA<NullLiteralExpr>())
	{
		expr->SetResultType(SS_T_NULLTYPE);
	}
	else if(expr->IsA<BoolLiteralExpr>())
	{
		expr->SetResultType(SS_T_BOOL);
	}
	else if(expr->IsA<IntLiteralExpr>())
	{
		// TODO: Check range, reparse, etc.?
		expr->SetResultType(SS_T_S4);
	}
	else if(expr->IsA<FloatLiteralExpr>())
	{
		// TODO: Check range, reparse, etc.?
		expr->SetResultType(SS_T_FLOAT);
	}
	else if(expr->IsA<CharLiteralExpr>())
	{
		expr->SetResultType(SS_T_CHAR);
	}
	else if(expr->IsA<StringLiteralExpr>())
	{
		expr->SetResultType(SS_T_STRING);
	}
	//---------------------------------------------------------------
	// BINARY EXPRESSION
	//---------------------------------------------------------------
	else if(expr->IsA<BinaryExpr>())
	{
		BinaryExpr* binExpr = dynamic_cast<BinaryExpr*>(expr);

		Expr* left = CheckAndTransformExpr(binExpr->GetLeft(), ctx);
		Expr* right = CheckAndTransformExpr(binExpr->GetRight(), ctx);
		SSAssert(left);
		SSAssert(right);

		// TODO: Maybe there's a better way to do error signaling than this
		//		 Use NULL exprs?
		if(left->IsA<ErrorExpr>() ||
			right->IsA<ErrorExpr>())
		{
			return new ErrorExpr();
		}

		const Type* leftType = left->GetResultType();
		const Type* rightType = right->GetResultType();
		SSAssert(leftType);
		SSAssert(rightType);

		if(binExpr->GetOp() == BINOP_EQ ||
			binExpr->GetOp() == BINOP_NE)
		{
			ReportError(expr, "== and != are temporarily unsupported");
			return new ErrorExpr();
		}
		else
		{
			const OverloadVector& candidates = GetBinaryOverloads(binExpr->GetOp());
			TypeVector types;
			types.push_back(leftType);
			types.push_back(rightType);
			OverloadVector resultOverloads;
			OverloadResolveResult resultCode = ResolveOverloads(types, candidates, resultOverloads);

			if(resultCode == OR_NO_MATCH)
			{
				ReportError(expr, 
							"Operator %s cannot accept operators of types %s, %s",
							binExpr->GetOpName().c_str(),
							leftType->GetName().c_str(),
							rightType->GetName().c_str());
				return new ErrorExpr();
			}
			else if(resultCode == OR_AMBIGUOUS)
			{
				// TODO: Show possibilities
				ReportError(expr, 
							"Operator %s cannot accept operators of types %s, %s due to ambiguity",
							binExpr->GetOpName().c_str(),
							leftType->GetName().c_str(),
							rightType->GetName().c_str());
				return new ErrorExpr();
			}
			else //if(resultCode == OR_SINGLE_MATCH)
			{
				SSAssert(resultOverloads.size() == 1);
				SSAssert(resultOverloads[0] != 0);

				const OverloadCandidate* const match = resultOverloads[0];

				// Stitch up params with casts if necessary
				if(match->types[0] != leftType)
				{
					CastExpr* cast = new CastExpr(match->types[0], left);
					binExpr->SetLeft(cast);
				}

				if(match->types[1] != rightType)
				{
					CastExpr* cast = new CastExpr(match->types[1], right);
					binExpr->SetRight(cast);
				}

				Type* returnType = reinterpret_cast<Type*>(match->userData);
				expr->SetResultType(returnType);
				return expr;
			}
		}
	}
	//---------------------------------------------------------------
	// UNARY EXPRESSION EXPRESSION
	//---------------------------------------------------------------
	else if(expr->IsA<UnaryExpr>())
	{
		UnaryExpr* unExpr = dynamic_cast<UnaryExpr*>(expr);

		Expr* operand = CheckAndTransformExpr(unExpr->GetOperand(), ctx);
		SSAssert(operand);

		if(operand->IsA<ErrorExpr>())
		{
			return new ErrorExpr();
		}

		const Type* operandType = operand->GetResultType();
		SSAssert(operandType != 0);

		if(unExpr->GetOp() == UNOP_LOG_NOT ||
			unExpr->GetOp() == UNOP_NEGATIVE ||
			unExpr->GetOp() == UNOP_POSITIVE)
		{
			const OverloadVector& candidates = GetUnaryOverloads(unExpr->GetOp());
			TypeVector types;
			types.push_back(operandType);
			OverloadVector resultOverloads;
			OverloadResolveResult resultCode = ResolveOverloads(types, candidates, resultOverloads);

			if(resultCode == OR_NO_MATCH)
			{
				ReportError(expr, 
							"Operator %s cannot accept operator of type %s",
							unExpr->GetOpName().c_str(),
							operandType->GetName().c_str());
				return new ErrorExpr();
			}
			else if(resultCode == OR_AMBIGUOUS)
			{
				// TODO: Show possibilities
				ReportError(expr, 
							"Operator %s cannot accept operator of type %s due to ambiguity",
							unExpr->GetOpName().c_str(),
							operandType->GetName().c_str());
				return new ErrorExpr();
			}
			else //if(resultCode == OR_SINGLE_MATCH)
			{
				SSAssert(resultOverloads.size() == 1);
				SSAssert(resultOverloads[0] != 0);

				const OverloadCandidate* const match = resultOverloads[0];

				// Stitch up params with casts if necessary
				if(match->types[0] != operandType)
				{
					CastExpr* cast = new CastExpr(match->types[0], operand);
					unExpr->SetOperand(cast);
				}

				Type* returnType = reinterpret_cast<Type*>(match->userData);
				expr->SetResultType(returnType);
				return expr;
			}
		}
		else
		{
			ReportError(expr, "Temporarily unsupported unary expr type");
			return new ErrorExpr();
		}
	}
	else
	{
		ReportError(expr, "Temporarily unsupported expr type: %s",
					expr->DynamicType()->GetName());
		return new ErrorExpr();
	}
}

// Debug dump of an expr to check that it's processing correctly
// TODO: Put this into Expr::Dump()!
String StaticChecker::DumpExpr(Expr* expr) const
{
	if(expr == 0)
	{
		return "<NULL>";
	}
	else if(expr->IsA<AssignExpr>())
	{
		AssignExpr* assignExpr = dynamic_cast<AssignExpr*>(expr);
		return "(" + DumpExpr(assignExpr->GetLeft()) + " " + assignExpr->GetOpName() + " " +
					DumpExpr(assignExpr->GetRight()) + ")";
	}
	else if(expr->IsA<BinaryExpr>())
	{
		BinaryExpr* binExpr = dynamic_cast<BinaryExpr*>(expr);
		return "(" + DumpExpr(binExpr->GetLeft()) + " " + binExpr->GetOpName() + " " +
					DumpExpr(binExpr->GetRight()) + ")";
	}
	else if(expr->IsA<CallExpr>())
	{
		CallExpr* callExpr = dynamic_cast<CallExpr*>(expr);
		String s = "call(" + DumpExpr(callExpr->GetLeft());
		for(int i=0; i < callExpr->GetParameterCount(); ++i)
		{
			s += ", ";
			s += DumpExpr(callExpr->GetParameter(i));
		}
		s += ")";
		return s;
	}
	else if(expr->IsA<CastExpr>())
	{
		CastExpr* castExpr = dynamic_cast<CastExpr*>(expr);
		return "cast(" + castExpr->GetTypeExpr()->Dump() + ", " + DumpExpr(castExpr->GetRight()) + ")";
	}
	else if(expr->IsA<IndexExpr>())
	{
		IndexExpr* indexExpr = dynamic_cast<IndexExpr*>(expr);
		return "index(" + DumpExpr(indexExpr->GetLeft()) + ", " + DumpExpr(indexExpr->GetRight()) + ")";
	}
	else if(expr->IsA<NullLiteralExpr>())
	{
		return "null";
	}
	else if(expr->IsA<BoolLiteralExpr>())
	{
		BoolLiteralExpr* boolExpr = dynamic_cast<BoolLiteralExpr*>(expr);
		return boolExpr->GetValue() ? "true" : "false";
	}
	else if(expr->IsA<IntLiteralExpr>())
	{
		IntLiteralExpr* castedExpr = dynamic_cast<IntLiteralExpr*>(expr);
		return castedExpr->GetValue();
	}
	else if(expr->IsA<FloatLiteralExpr>())
	{
		FloatLiteralExpr* castedExpr = dynamic_cast<FloatLiteralExpr*>(expr);
		return castedExpr->GetValue();
	}
	else if(expr->IsA<CharLiteralExpr>())
	{
		CharLiteralExpr* castedExpr = dynamic_cast<CharLiteralExpr*>(expr);
		return castedExpr->GetValue();
	}
	else if(expr->IsA<StringLiteralExpr>())
	{
		StringLiteralExpr* castedExpr = dynamic_cast<StringLiteralExpr*>(expr);
		return castedExpr->GetValue();
	}
	else if(expr->IsA<MemberExpr>())
	{
		MemberExpr* memberExpr = dynamic_cast<MemberExpr*>(expr);
		return "(" + DumpExpr(memberExpr->GetLeft()) + "." +
					memberExpr->GetIdentifier() + ")";
	}
	else if(expr->IsA<NameExpr>())
	{
		NameExpr* nameExpr = dynamic_cast<NameExpr*>(expr);
		return nameExpr->GetIdentifier();
	}
	else if(expr->IsA<TernaryExpr>())
	{
		TernaryExpr* ternaryExpr = dynamic_cast<TernaryExpr*>(expr);
		return "ternary(" + DumpExpr(ternaryExpr->GetCondition()) + ", " + 
							DumpExpr(ternaryExpr->GetTrueExpr()) + ", " + 
							DumpExpr(ternaryExpr->GetFalseExpr()) + ")";
	}
	else if(expr->IsA<UnaryExpr>())
	{
		UnaryExpr* unaryExpr = dynamic_cast<UnaryExpr*>(expr);
		return "(" + unaryExpr->GetPrefixOpName() + DumpExpr(unaryExpr->GetOperand()) + unaryExpr->GetPostfixOpName() + ")";
	}
	else
	{
		SS_UNREACHABLE;
		return "";
	}
}

}

