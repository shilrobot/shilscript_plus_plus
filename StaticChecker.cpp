#include "StaticChecker.h"
#include "Package.h"
#include "Class.h"
#include "Variable.h"
#include "Function.h"
#include "NameLookup.h"
#include "TypeLookup.h"
#include "Statement.h"
#include "AllExprs.h"
#include "TypeConversion.h"
#include "ClassType.h"
#include "Local.h"

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
	if(!typ->IsA<ClassType>())
	{
		ReportError(cls, "Base must be another class");
		return;
	}

	// Check that we are not setting up a cycle
	// (e.g., make sure that cls is not in type's bases)
	Class* baseClass = dynamic_cast<ClassType*>(typ)->GetClass();
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

	if(func->GetParameterCount() > SS_MAX_PARAMETERS)
	{
		ReportError(func, "Function has too many parameters. Maximum is %d", SS_MAX_PARAMETERS);
		return false;
	}

	// Now, we have to check params
	bool paramsFailed = false;
	int id = 0;
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

		if(paramType == SS_T_VOID)
		{
			paramsFailed = true;
			ReportError(param, "Parameter %s cannot have type void", param->GetName().c_str());
			continue;
		}

		param->SetType(paramType);
		param->SetID(id);
		++id;

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
				ExprContext exprCtx;
				exprCtx.stmtCtx = &ctx;

				Expr* returnExpr = CheckExprWithType(returnStmt->GetExpr(), ctx.func->GetReturnType(), exprCtx);
				SSAssert(returnExpr);

				if(!returnExpr->IsA<ErrorExpr>())
					returnStmt->SetExpr(returnExpr);
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

		Expr* xformed = CheckExpr(exprStmt->GetExpr(), exprCtx);
		exprStmt->SetExpr(xformed);

		// TODO: Make sure return type of expr isn't a "pseudovalue" like, say, a function
		//		 (which currently isn't in the type system, and thus can't properly
		//		  be 'popped')
	}
	else if(stmt->IsA<IfStatement>())
	{
		IfStatement* ifStmt = dynamic_cast<IfStatement*>(stmt);

		ExprContext exprCtx;
		exprCtx.stmtCtx = &ctx;

		Expr* cond = CheckExprWithType(ifStmt->GetCondition(), SS_T_BOOL, exprCtx);
		SSAssert(cond);

		if(!cond->IsA<ErrorExpr>())
			ifStmt->SetCondition(cond);

		// Check the if-true branch (which is required to exist)
		SSAssert(ifStmt->GetIfTrue());
		StatementContext trueCtx(ctx);
		Scope trueScope;
		trueCtx.scope = &trueScope;
		trueCtx.parent = &ctx;

		CheckStatement(ifStmt->GetIfTrue(), trueCtx);

		if(ifStmt->GetIfFalse() != 0)
		{
			StatementContext falseCtx(ctx);
			Scope falseScope;
			falseCtx.scope = &falseScope;
			falseCtx.parent = &ctx;

			CheckStatement(ifStmt->GetIfFalse(), falseCtx);
		}
	}
	else if(stmt->IsA<WhileStatement>())
	{
		WhileStatement* whileStmt = dynamic_cast<WhileStatement*>(stmt);

		ExprContext exprCtx;
		exprCtx.stmtCtx = &ctx;

		Expr* cond = CheckExprWithType(whileStmt->GetCondition(), SS_T_BOOL, exprCtx);
		SSAssert(cond);

		if(!cond->IsA<ErrorExpr>())
			whileStmt->SetCondition(cond);

		// Check the body of the loop
		StatementContext bodyCtx(ctx);
		Scope scope;
		bodyCtx.scope = &scope;
		bodyCtx.parent = &ctx;
		bodyCtx.canBreak = true;
		bodyCtx.canContinue = true;

		SSAssert(whileStmt->GetBody());
		CheckStatement(whileStmt->GetBody(), bodyCtx);
	}
	else if(stmt->IsA<DoWhileStatement>())
	{
		DoWhileStatement* doStmt = dynamic_cast<DoWhileStatement*>(stmt);

		// Check the body of the loop
		StatementContext bodyCtx(ctx);
		Scope scope;
		bodyCtx.scope = &scope;
		bodyCtx.parent = &ctx;
		bodyCtx.canBreak = true;
		bodyCtx.canContinue = true;

		SSAssert(doStmt->GetBody());
		CheckStatement(doStmt->GetBody(), bodyCtx);
		
		ExprContext exprCtx;
		exprCtx.stmtCtx = &ctx;

		Expr* cond = CheckExprWithType(doStmt->GetCondition(), SS_T_BOOL, exprCtx);
		SSAssert(cond);

		if(!cond->IsA<ErrorExpr>())
			doStmt->SetCondition(cond);
	}
	else if(stmt->IsA<LocalDefStatement>())
	{
		LocalDefStatement* localDefStmt = dynamic_cast<LocalDefStatement*>(stmt);

		Scope* scope = ctx.scope;

		Type* typ = TypeLookup::LookupType( localDefStmt->GetTypeExpr(), ctx.func);

		if(typ == SS_T_VOID)
		{
			ReportError(localDefStmt->GetTypeExpr(), "Variables cannot be void");
			return;
		}

		if(typ == 0)
			return;

		for(int i=0; i<localDefStmt->GetDefCount(); ++i)
		{
			LocalDef* def = localDefStmt->GetDef(i);

			// TODO: Look up with in our *current* scope
			String name = def->GetName();
			bool bValidName = true;

			if(!IsValidName(name))
			{
				// TODO: Can we do better than this? E.g. say what's wrong with the name?
				ReportError(def, "Invalid name %s", SS_QUOTE(name));
				bValidName = false;
			}
			else
			{
				Node* conflict = scope->Find(name);
				if(conflict != 0)
				{
					// TODO: Better error message
					ReportError(def, "Name %s conflicts with %s",
										SS_QUOTE(name),
										conflict->GetDesc().c_str());
					bValidName = false;
				}
			}

			Expr* initExpr = 0;
			bool bInitExprFailed = false;

			if(typ != 0 && def->GetInitExpr())
			{
				ExprContext exprCtx;
				exprCtx.stmtCtx = &ctx;
				Expr* initExpr = CheckExprWithType(def->GetInitExpr(), typ, exprCtx);
				SSAssert(initExpr);
				
				// TODO: memory management
				if(initExpr->IsA<ErrorExpr>())
					bInitExprFailed = true;

				def->SetInitExpr(initExpr);
			}

			if(bValidName && !bInitExprFailed)
			{
				Local* local = new Local();
				local->SetName(name);
				local->SetType(typ);
				//local->SetInitExpr(initExpr);

				local->SetID(ctx.func->GetLocalCount());

				ctx.func->AddLocal(local);
				scope->Add(local);

				def->SetLocal(local);

				if(ctx.func->GetLocalCount() == SS_MAX_LOCALS)
					ReportError(ctx.func, "Function has too many locals. Maximum is %d", SS_MAX_LOCALS);
			}
		}
	}
	// todo: for
	// todo: foreach
	// todo: switch
	// todo: try-catch
	// todo: variable def.
	else
	{
		// Not implemented...
		SSAssert(0 && "Other statements not implemented yet");
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
		OVERLOAD_ADD(SS_T_I4, SS_T_I4, SS_T_I4)
		OVERLOAD_ADD(SS_T_U4, SS_T_U4, SS_T_U4)
		OVERLOAD_ADD(SS_T_I8, SS_T_I8, SS_T_I8)
		OVERLOAD_ADD(SS_T_U8, SS_T_U8, SS_T_U8)
		OVERLOAD_ADD(SS_T_F4, SS_T_F4, SS_T_F4)
		OVERLOAD_ADD(SS_T_F8, SS_T_F8, SS_T_F8)
		OVERLOAD_ADD(SS_T_STRING, SS_T_STRING, SS_T_STRING)
	OVERLOAD_END

	OVERLOAD_BEGIN(BINOP_SUB, "-")
		OVERLOAD_ADD(SS_T_I4, SS_T_I4, SS_T_I4)
		OVERLOAD_ADD(SS_T_U4, SS_T_U4, SS_T_U4)
		OVERLOAD_ADD(SS_T_I8, SS_T_I8, SS_T_I8)
		OVERLOAD_ADD(SS_T_U8, SS_T_U8, SS_T_U8)
		OVERLOAD_ADD(SS_T_F4, SS_T_F4, SS_T_F4)
		OVERLOAD_ADD(SS_T_F8, SS_T_F8, SS_T_F8)
	OVERLOAD_END

	OVERLOAD_BEGIN(BINOP_MUL, "*")
		OVERLOAD_ADD(SS_T_I4, SS_T_I4, SS_T_I4)
		OVERLOAD_ADD(SS_T_U4, SS_T_U4, SS_T_U4)
		OVERLOAD_ADD(SS_T_I8, SS_T_I8, SS_T_I8)
		OVERLOAD_ADD(SS_T_U8, SS_T_U8, SS_T_U8)
		OVERLOAD_ADD(SS_T_F4, SS_T_F4, SS_T_F4)
		OVERLOAD_ADD(SS_T_F8, SS_T_F8, SS_T_F8)
	OVERLOAD_END

	OVERLOAD_BEGIN(BINOP_DIV, "/")
		OVERLOAD_ADD(SS_T_I4, SS_T_I4, SS_T_I4)
		OVERLOAD_ADD(SS_T_U4, SS_T_U4, SS_T_U4)
		OVERLOAD_ADD(SS_T_I8, SS_T_I8, SS_T_I8)
		OVERLOAD_ADD(SS_T_U8, SS_T_U8, SS_T_U8)
		OVERLOAD_ADD(SS_T_F4, SS_T_F4, SS_T_F4)
		OVERLOAD_ADD(SS_T_F8, SS_T_F8, SS_T_F8)
	OVERLOAD_END

	OVERLOAD_BEGIN(BINOP_MOD, "%")
		OVERLOAD_ADD(SS_T_I4, SS_T_I4, SS_T_I4)
		OVERLOAD_ADD(SS_T_U4, SS_T_U4, SS_T_U4)
		OVERLOAD_ADD(SS_T_I8, SS_T_I8, SS_T_I8)
		OVERLOAD_ADD(SS_T_U8, SS_T_U8, SS_T_U8)

		// Works on floats too, a la C#
		OVERLOAD_ADD(SS_T_F4, SS_T_F4, SS_T_F4)
		OVERLOAD_ADD(SS_T_F8, SS_T_F8, SS_T_F8)
	OVERLOAD_END

	OVERLOAD_BEGIN(BINOP_POW, "**")
		// TODO: Should it work on ints?
		OVERLOAD_ADD(SS_T_F4, SS_T_F4, SS_T_F4)
		OVERLOAD_ADD(SS_T_F8, SS_T_F8, SS_T_F8)
	OVERLOAD_END

	OVERLOAD_BEGIN(BINOP_BIT_AND, "&")
		OVERLOAD_ADD(SS_T_I4, SS_T_I4, SS_T_I4)
		OVERLOAD_ADD(SS_T_U4, SS_T_U4, SS_T_U4)
		OVERLOAD_ADD(SS_T_I8, SS_T_I8, SS_T_I8)
		OVERLOAD_ADD(SS_T_U8, SS_T_U8, SS_T_U8)
	OVERLOAD_END

	OVERLOAD_BEGIN(BINOP_BIT_OR, "|")
		OVERLOAD_ADD(SS_T_I4, SS_T_I4, SS_T_I4)
		OVERLOAD_ADD(SS_T_U4, SS_T_U4, SS_T_U4)
		OVERLOAD_ADD(SS_T_I8, SS_T_I8, SS_T_I8)
		OVERLOAD_ADD(SS_T_U8, SS_T_U8, SS_T_U8)
	OVERLOAD_END

	OVERLOAD_BEGIN(BINOP_BIT_XOR, "^")
		OVERLOAD_ADD(SS_T_I4, SS_T_I4, SS_T_I4)
		OVERLOAD_ADD(SS_T_U4, SS_T_U4, SS_T_U4)
		OVERLOAD_ADD(SS_T_I8, SS_T_I8, SS_T_I8)
		OVERLOAD_ADD(SS_T_U8, SS_T_U8, SS_T_U8)
	OVERLOAD_END

	OVERLOAD_BEGIN(BINOP_LOG_AND, "&&")
		OVERLOAD_ADD(SS_T_BOOL, SS_T_BOOL, SS_T_BOOL)
	OVERLOAD_END

	OVERLOAD_BEGIN(BINOP_LOG_OR, "||")
		OVERLOAD_ADD(SS_T_BOOL, SS_T_BOOL, SS_T_BOOL)
	OVERLOAD_END

	OVERLOAD_BEGIN(BINOP_SHL, "<<")
		OVERLOAD_ADD(SS_T_I4, SS_T_I4, SS_T_I4)
		OVERLOAD_ADD(SS_T_U4, SS_T_I4, SS_T_U4)
		OVERLOAD_ADD(SS_T_I8, SS_T_I4, SS_T_I8)
		OVERLOAD_ADD(SS_T_U8, SS_T_I4, SS_T_U8)
	OVERLOAD_END

	OVERLOAD_BEGIN(BINOP_SHR, ">>")
		OVERLOAD_ADD(SS_T_I4, SS_T_I4, SS_T_I4)
		OVERLOAD_ADD(SS_T_U4, SS_T_I4, SS_T_U4)
		OVERLOAD_ADD(SS_T_I8, SS_T_I4, SS_T_I8)
		OVERLOAD_ADD(SS_T_U8, SS_T_I4, SS_T_U8)
	OVERLOAD_END

	OVERLOAD_BEGIN(BINOP_LT, "<")
		OVERLOAD_ADD(SS_T_I4, SS_T_I4, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_U4, SS_T_U4, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_I8, SS_T_I8, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_U8, SS_T_U8, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_F4, SS_T_F4, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_F8, SS_T_F8, SS_T_BOOL)
	OVERLOAD_END

	OVERLOAD_BEGIN(BINOP_LE, "<=")
		OVERLOAD_ADD(SS_T_I4, SS_T_I4, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_U4, SS_T_U4, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_I8, SS_T_I8, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_U8, SS_T_U8, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_F4, SS_T_F4, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_F8, SS_T_F8, SS_T_BOOL)
	OVERLOAD_END
	
	OVERLOAD_BEGIN(BINOP_GT, ">")
		OVERLOAD_ADD(SS_T_I4, SS_T_I4, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_U4, SS_T_U4, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_I8, SS_T_I8, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_U8, SS_T_U8, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_F4, SS_T_F4, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_F8, SS_T_F8, SS_T_BOOL)
	OVERLOAD_END

	OVERLOAD_BEGIN(BINOP_GE, ">=")
		OVERLOAD_ADD(SS_T_I4, SS_T_I4, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_U4, SS_T_U4, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_I8, SS_T_I8, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_U8, SS_T_U8, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_F4, SS_T_F4, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_F8, SS_T_F8, SS_T_BOOL)
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

	OVERLOAD_BEGIN(UNOP_LOG_NOT, "!")
		OVERLOAD_ADD_UN(SS_T_BOOL, SS_T_BOOL)
	OVERLOAD_END

	OVERLOAD_BEGIN(UNOP_COMPLEMENT, "~")
		OVERLOAD_ADD_UN(SS_T_I4, SS_T_I4)
		OVERLOAD_ADD_UN(SS_T_U4, SS_T_U4)
		OVERLOAD_ADD_UN(SS_T_I8, SS_T_I8)
		OVERLOAD_ADD_UN(SS_T_U8, SS_T_U8)
	OVERLOAD_END

	OVERLOAD_BEGIN(UNOP_NEGATIVE, "-")
		OVERLOAD_ADD_UN(SS_T_I4, SS_T_I4)
		OVERLOAD_ADD_UN(SS_T_U4, SS_T_U4)
		OVERLOAD_ADD_UN(SS_T_I8, SS_T_I8)
		OVERLOAD_ADD_UN(SS_T_U8, SS_T_U8)
		OVERLOAD_ADD_UN(SS_T_F4, SS_T_F4)
		OVERLOAD_ADD_UN(SS_T_F8, SS_T_F8)
	OVERLOAD_END

	OVERLOAD_BEGIN(UNOP_POSITIVE, "+")
		OVERLOAD_ADD_UN(SS_T_I4, SS_T_I4)
		OVERLOAD_ADD_UN(SS_T_U4, SS_T_U4)
		OVERLOAD_ADD_UN(SS_T_I8, SS_T_I8)
		OVERLOAD_ADD_UN(SS_T_U8, SS_T_U8)
		OVERLOAD_ADD_UN(SS_T_F4, SS_T_F4)
		OVERLOAD_ADD_UN(SS_T_F8, SS_T_F8)
	OVERLOAD_END

	default:
		{
			SSAssert(0 && "Unknown/unimplemented unary op");
			static OverloadVector dummy;
			return dummy; // quiet, compiler
		}
	}
}


Expr* StaticChecker::CheckExprWithType(Expr* expr, const Type* desiredType, const ExprContext& ctx)
{
	SSAssert(expr);
	SSAssert(desiredType);

	Expr* xformed = CheckExpr(expr, ctx);
	SSAssert(xformed);

	if(xformed->IsA<ErrorExpr>())
	{
		// This has already failed.
		return xformed;
	}
	else
	{
		const Type* xformType = xformed->GetResultType();
		SSAssert(xformType);

		if(xformType == desiredType)
			return xformed;
		else if(CanImplicitlyConvert(xformType, desiredType))
		{
			return new CastExpr(desiredType, xformed);
		}
		else
		{
			ReportError(expr, "Cannot implicitly convert type %s to %s", xformType->GetName().c_str(), desiredType->GetName().c_str());
			return new ErrorExpr();
		}
	}
}

Expr* StaticChecker::CheckExpr(Expr* expr, const ExprContext& ctx)
{
	Function* func = ctx.stmtCtx->func;
	Scope* scope = ctx.stmtCtx->scope;

	//---------------------------------------------------------------
	// NULL LITERAL
	//---------------------------------------------------------------
	if(expr->IsA<NullLiteralExpr>())
	{
		expr->SetResultType(SS_T_NULLTYPE);
		return expr;
	}
	//---------------------------------------------------------------
	// BOOL LITERAL
	//---------------------------------------------------------------
	else if(expr->IsA<BoolLiteralExpr>())
	{
		expr->SetResultType(SS_T_BOOL);
		return expr;
	}
	//---------------------------------------------------------------
	// INT LITERAL
	//---------------------------------------------------------------
	else if(expr->IsA<IntLiteralExpr>())
	{
		// TODO: Check range, reparse, etc.?
		expr->SetResultType(SS_T_I4);
		return expr;
	}
	//---------------------------------------------------------------
	// FLOAT LITERAL
	//---------------------------------------------------------------
	else if(expr->IsA<FloatLiteralExpr>())
	{
		// TODO: Double types
		// TODO: Check range, reparse, etc.?
		expr->SetResultType(SS_T_F4);
		return expr;
	}
	//---------------------------------------------------------------
	// CHAR LITERAL
	//---------------------------------------------------------------
	else if(expr->IsA<CharLiteralExpr>())
	{
		expr->SetResultType(SS_T_CHAR);
		return expr;
	}
	//---------------------------------------------------------------
	// STRING LITERAL
	//---------------------------------------------------------------
	else if(expr->IsA<StringLiteralExpr>())
	{
		expr->SetResultType(SS_T_STRING);
		return expr;
	}
	//---------------------------------------------------------------
	// NAME EXPRESSION
	//---------------------------------------------------------------
	else if(expr->IsA<NameExpr>())
	{
		const String& name = dynamic_cast<NameExpr*>(expr)->GetIdentifier();

		// Look for locals
		const StatementContext* stmtCtx = ctx.stmtCtx;
		SSAssert(stmtCtx);

		while(stmtCtx != 0)
		{
			Node* node = stmtCtx->scope->Find(name);
			if(node != 0)
			{
				SSAssert(node->IsA<Local>());
				Local* local = dynamic_cast<Local*>(node);
				LocalRefExpr* localRefExpr = new LocalRefExpr(local);
				localRefExpr->SetResultType(local->GetType());
				if(!local->IsConst())
					localRefExpr->SetLValue(true);
				delete expr;
				return localRefExpr;
			}

			stmtCtx = stmtCtx->parent;
		}

		// Look for params
		Node* node = func->GetScope()->Find(name);
		if(node != 0)
		{
			SSAssert(node->IsA<Parameter>());
			Parameter* param = dynamic_cast<Parameter*>(node);
			ParamRefExpr* paramRefExpr = new ParamRefExpr(param);
			paramRefExpr->SetResultType(param->GetType());
			paramRefExpr->SetLValue(true);
			delete expr;
			return paramRefExpr;
		}

		// Look for other stuff
		node = NameLookup::LookupName(name, func, expr);
		if(node != 0)
		{
			// TODO: This will change with global vars & funcs
			if(node->IsA<Variable>())
			{
				Variable* var = dynamic_cast<Variable*>(node);	

				if(func->IsStatic() && !var->IsStatic())
				{
					ReportError(expr, "Variable %s cannot be used in a static context", var->GetFullName().c_str());
					delete expr;
					return new ErrorExpr();
				}

				VariableRefExpr* varRefExpr = new VariableRefExpr(new ThisExpr(), var);
				SSAssert(var->GetType());
				varRefExpr->SetResultType(var->GetType());
				if(!var->IsConst())
					varRefExpr->SetLValue(true);
				if(var->IsStatic() || var->IsConst())
					varRefExpr->SetLeft(0);
				else
					varRefExpr->SetLeft(new ThisExpr());
				return varRefExpr;
			}
			else if(node->IsA<Function>())
			{
				Function* func2 = dynamic_cast<Function*>(node);		

				if(func->IsStatic() && !func2->IsStatic())
				{
					ReportError(expr, "Function %s cannot be used in a static context", func2->GetFullName().c_str());
					delete expr;
					return new ErrorExpr();
				}

				delete expr;

				if(func2->IsStatic())
					return new FunctionRefExpr(0, func2);
				else
					return new FunctionRefExpr(new ThisExpr( ), func2);
			}
			else if(node->IsA<Package>())
			{
				Package* pkg = dynamic_cast<Package*>(node);	
				delete expr;
				return new PackageRefExpr(pkg);
			}
			else if(node->IsA<Class>())
			{
				Class* cls = dynamic_cast<Class*>(node);	
				delete expr;
				return new ClassRefExpr(cls);
			}
			else
			{
				// Shouldn't happen. If it does, this needs to be changed.
				SS_UNREACHABLE;
				return 0;
			}
		}
		else
		{
			delete expr;
			// LookupName has already printed an error
			return new ErrorExpr();
		}
	}
	//---------------------------------------------------------------
	// ASSIGN EXPRESSION
	//---------------------------------------------------------------
	else if(expr->IsA<AssignExpr>())
	{
		AssignExpr* assignExpr = dynamic_cast<AssignExpr*>(expr);

		if(assignExpr->GetOp() != BINOP_NONE)
		{
			ReportError(expr, "Assignments besides are = not implemented yet");
			delete expr;
			return new ErrorExpr();
		}

		Expr* left = CheckExpr(assignExpr->GetLeft(), ctx);
		Expr* right = CheckExpr(assignExpr->GetRight(), ctx);

		SSAssert(left);
		SSAssert(right);

		// TODO: Maybe there's a better way to do error signaling than this
		//		 Use NULL exprs?
		if(left->IsA<ErrorExpr>() ||
			right->IsA<ErrorExpr>())
		{
			delete expr;
			return new ErrorExpr();
		}
		
		if(!left->IsLValue())
		{
			ReportError(expr, "Target of assignment must be an lvalue");
			delete expr;
			return new ErrorExpr();
		}

		const Type* leftType = left->GetResultType();
		const Type* rightType = right->GetResultType();
		SSAssert(leftType);
		SSAssert(rightType);

		if(leftType == rightType)
		{
			assignExpr->SetLeft(left);
			assignExpr->SetRight(right);
			assignExpr->SetResultType(leftType);
			return assignExpr;
		}
		else if(CanImplicitlyConvert(rightType, leftType))
		{
			assignExpr->SetLeft(left);
			assignExpr->SetRight(new CastExpr(leftType, right));
			assignExpr->SetResultType(leftType);
			return assignExpr;
		}
		else
		{
			delete expr;
			ReportError(expr, "Cannot implicitly convert type %s to %s", rightType->GetName().c_str(), leftType->GetName().c_str());
			return new ErrorExpr();
		}
	}
	//---------------------------------------------------------------
	// BINARY EXPRESSION
	//---------------------------------------------------------------
	else if(expr->IsA<BinaryExpr>())
	{
		BinaryExpr* binExpr = dynamic_cast<BinaryExpr*>(expr);

		Expr* left = CheckExpr(binExpr->GetLeft(), ctx);
		Expr* right = CheckExpr(binExpr->GetRight(), ctx);
		SSAssert(left);
		SSAssert(right);

		// TODO: Maybe there's a better way to do error signaling than this
		//		 Use NULL exprs?
		if(left->IsA<ErrorExpr>() ||
			right->IsA<ErrorExpr>())
		{
			delete expr;
			return new ErrorExpr();
		}

		const Type* leftType = left->GetResultType();
		const Type* rightType = right->GetResultType();
		SSAssert(leftType);
		SSAssert(rightType);

		if(binExpr->GetOp() == BINOP_EQ ||
			binExpr->GetOp() == BINOP_NE)
		{
			delete expr;
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
							"Operator %s cannot accept operands of types %s, %s",
							binExpr->GetOpName().c_str(),
							leftType->GetName().c_str(),
							rightType->GetName().c_str());
				delete expr;
				return new ErrorExpr();
			}
			else if(resultCode == OR_AMBIGUOUS)
			{
				// TODO: Show possibilities
				ReportError(expr, 
							"Operator %s cannot accept operands of types %s, %s due to ambiguity",
							binExpr->GetOpName().c_str(),
							leftType->GetName().c_str(),
							rightType->GetName().c_str());
				delete expr;
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
				else
					binExpr->SetLeft(left);

				if(match->types[1] != rightType)
				{
					CastExpr* cast = new CastExpr(match->types[1], right);
					binExpr->SetRight(cast);
				}
				else
					binExpr->SetRight(right);

				Type* returnType = reinterpret_cast<Type*>(match->userData);
				expr->SetResultType(returnType);
				return expr;
			}
		}
	}
	//---------------------------------------------------------------
	// CALL EXPRESSION
	//---------------------------------------------------------------
	else if(expr->IsA<CallExpr>())
	{
		// TODO: Overloads

		CallExpr* callExpr = dynamic_cast<CallExpr*>(expr);
		Expr* left = CheckExpr(callExpr->GetLeft( ), ctx);
		SSAssert(left);

		if(left->IsA<ErrorExpr>())
			return new ErrorExpr();

		// TODO: Handle calling classes, for construction
		if(!left->IsA<FunctionRefExpr>())
		{
			ReportError(expr, "Only functions can be called");
			return new ErrorExpr();
		}

		FunctionRefExpr* funcRefExpr = dynamic_cast<FunctionRefExpr*>(left);
		Function* func = funcRefExpr->GetFunction();
		SSAssert(func != 0);

		if(func->GetParameterCount() != callExpr->GetParameterCount())
		{
			ReportError(expr, "Function %s takes %d parameters, %d provided",
							func->GetFullName().c_str(),
							func->GetParameterCount(),
							callExpr->GetParameterCount());
			return new ErrorExpr();
		}

		bool errored = false;

		FunctionCallExpr* funcCallExpr = new FunctionCallExpr(funcRefExpr->GetLeft(), func);

		for(int i=0; i < callExpr->GetParameterCount(); ++i)
		{
			Expr* paramOriginal = callExpr->GetParameter(i);
			SSAssert(paramOriginal != 0);
			Expr* paramXformed = CheckExpr(paramOriginal, ctx);
			SSAssert(paramXformed);
			SSAssert(paramXformed->GetResultType() != 0);
			SSAssert(func->GetParameter(i)->GetType() != 0);

			const Type* paramType = func->GetParameter(i)->GetType();
			const Type* providedValueType = paramXformed->GetResultType();

			if(!CanImplicitlyConvert(providedValueType, paramType))
			{
				ReportError(paramOriginal, "Function %s takes %s for parameter %d, given %s",
							func->GetFullName().c_str(),
							paramType->GetName( ),
							i+1,
							providedValueType->GetName( ));
				errored = true;
				continue;
			}

			Expr* paramFinal = paramXformed;
			if(paramType != providedValueType)
			{
				paramFinal = new CastExpr(paramType, paramFinal);
			}

			funcCallExpr->AddParameter(paramFinal);
		}
		
		if(errored)
			return new ErrorExpr();

		funcCallExpr->SetResultType( func->GetReturnType() );
		return funcCallExpr;
	}
	//---------------------------------------------------------------
	// CAST EXPRESSION
	//---------------------------------------------------------------
	else if(expr->IsA<CastExpr>())
	{
		CastExpr* castExpr = dynamic_cast<CastExpr*>(expr);
		Expr* right = CheckExpr(castExpr->GetRight(), ctx);
		SSAssert(right != 0);

		if(right->IsA<ErrorExpr>())
			return new ErrorExpr();

		if(castExpr->GetType() == 0)
		{
			SSAssert(castExpr->GetTypeExpr() != 0);

			// Look up the type
			Type* typ = TypeLookup::LookupType(castExpr->GetTypeExpr(), func);
			if(!typ)
				return new ErrorExpr();

			castExpr->SetType(typ);
		}


		if(castExpr->GetType() == right->GetResultType())
		{
			return right;
		}
		else if(CanExplicitlyConvert(right->GetResultType(), castExpr->GetType()))
		{
			castExpr->SetResultType(castExpr->GetType());
			castExpr->SetRight(right);
			return castExpr;
		}
		else
		{
			ReportError(castExpr, "Cannot convert from %s to %s", castExpr->GetType()->GetName().c_str(), right->GetResultType()->GetName());
			return new ErrorExpr();
		}
	}
	//---------------------------------------------------------------
	// TERNARY EXPRESSION
	//---------------------------------------------------------------
	else if(expr->IsA<TernaryExpr>())
	{
		TernaryExpr* ternExpr = dynamic_cast<TernaryExpr*>(expr);

		Expr* cond = CheckExpr(ternExpr->GetCondition(), ctx);
		Expr* ifTrue = CheckExpr(ternExpr->GetTrueExpr(), ctx);
		Expr* ifFalse = CheckExpr(ternExpr->GetFalseExpr(), ctx);

		SSAssert(cond);
		SSAssert(ifTrue);
		SSAssert(ifFalse);

		// TODO: Maybe break this up so independent errors can get reported better? I dunno.
		// e.g., ifFalse is wrong, but cond also doesn't convert to bool, etc.
		if(cond->IsA<ErrorExpr>() ||
			ifTrue->IsA<ErrorExpr>() ||
			ifFalse->IsA<ErrorExpr>())
			return new ErrorExpr();

		const Type* condType = cond->GetResultType();
		const Type* ifTrueType = ifTrue->GetResultType();
		const Type* ifFalseType = ifFalse->GetResultType();

		SSAssert(condType);
		SSAssert(ifTrueType);
		SSAssert(ifFalseType);

		if(condType == SS_T_BOOL)
		{
			// Great
		}
		else if(CanImplicitlyConvert(condType, SS_T_BOOL))
		{
			cond = new CastExpr(SS_T_BOOL, cond);
		}
		else
		{
			ReportError(expr, "Cannot convert from type %s to bool", condType->GetName().c_str());
			return new ErrorExpr();
		}

		if(ifTrueType == ifFalseType)
		{
			ternExpr->SetResultType(ifTrueType);
			return ternExpr;
		}
		else
		{
			bool canConvertToTrue = CanImplicitlyConvert(ifFalseType, ifTrueType);
			bool canConvertToFalse = CanImplicitlyConvert(ifTrueType, ifFalseType);

			if(canConvertToTrue && !canConvertToFalse)
			{
				ternExpr->SetResultType(ifTrueType);
				ternExpr->SetTrueExpr(ifTrue);
				ternExpr->SetFalseExpr(new CastExpr(ifTrueType, ifFalse));
				return ternExpr;
			}
			else if(canConvertToFalse && !canConvertToTrue)
			{
				ternExpr->SetResultType(ifFalseType);
				ternExpr->SetTrueExpr(new CastExpr(ifFalseType, ifTrue));
				ternExpr->SetFalseExpr(ifFalse);
				return ternExpr;
			}
			else if(canConvertToTrue && canConvertToFalse)
			{
				ReportError(expr, "Ternary expression with types %s, %s has an ambiguous return value",
									ifTrueType->GetName().c_str(),
									ifFalseType->GetName().c_str());
				return new ErrorExpr();
			} 
			else // Neither work
			{
				ReportError(expr, "Cannot use types %s, %s for true/false branches of ternary expression, neither can implicitly convert to the other",
									ifTrueType->GetName().c_str(),
									ifFalseType->GetName().c_str());
				return new ErrorExpr();
			}
		}
	}
	//---------------------------------------------------------------
	// THIS EXPRESSION
	//---------------------------------------------------------------
	else if(expr->IsA<ThisExpr>())
	{
		if(func->IsStatic( ))
		{
			ReportError(expr, "Cannot use 'this' in a static function context");
			return new ErrorExpr();
		}
		else
		{
			SSAssert(func->GetParent() != 0);
			SSAssert(func->GetParent()->IsA<Class>());
			Class* cls = dynamic_cast<Class*>(func->GetParent());
			Type* type = cls->GetType();
			SSAssert(type != 0);
			expr->SetResultType(type);
			return expr;
		}
	}
	//---------------------------------------------------------------
	// UNARY EXPRESSION
	//---------------------------------------------------------------
	else if(expr->IsA<UnaryExpr>())
	{
		UnaryExpr* unExpr = dynamic_cast<UnaryExpr*>(expr);

		Expr* operand = CheckExpr(unExpr->GetOperand(), ctx);
		SSAssert(operand);

		if(operand->IsA<ErrorExpr>())
		{
			return new ErrorExpr();
		}

		const Type* operandType = operand->GetResultType();
		SSAssert(operandType != 0);

		if(unExpr->GetOp() == UNOP_LOG_NOT ||
			unExpr->GetOp() == UNOP_COMPLEMENT ||
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
							"Operator %s cannot accept operand of type %s",
							unExpr->GetOpName().c_str(),
							operandType->GetName().c_str());
				return new ErrorExpr();
			}
			else if(resultCode == OR_AMBIGUOUS)
			{
				// TODO: Show possibilities
				ReportError(expr, 
							"Operator %s cannot accept operand of type %s due to ambiguity",
							unExpr->GetOpName().c_str(),
							operandType->GetName().c_str());
				return new ErrorExpr();
			}
			else //if(resultCode == OR_SINGLE_MATCH)
			{
				SSAssert(resultOverloads.size() == 1);
				SSAssert(resultOverloads[0] != 0);

				const OverloadCandidate* const match = resultOverloads[0];

				// Stitch up params with casts if necessary (we may need to do something smarter here)
				if(match->types[0] != operandType)
				{
					CastExpr* cast = new CastExpr(match->types[0], operand);
					unExpr->SetOperand(cast);
				}
				else
					unExpr->SetOperand(operand);

				Type* returnType = reinterpret_cast<Type*>(match->userData);
				expr->SetResultType(returnType);
				return expr;
			}
		}
		else if(unExpr->GetOp() == UNOP_PRE_INC ||
				unExpr->GetOp() == UNOP_PRE_DEC ||
				unExpr->GetOp() == UNOP_POST_INC ||
				unExpr->GetOp() == UNOP_POST_DEC)
		{
			if(!operand->IsLValue())
			{
				ReportError(unExpr, "%s operator operand is not an lvalue", unExpr->GetOpName().c_str());
				return new ErrorExpr();
			}

			const Type* operandType = operand->GetResultType();
			SSAssert(operandType);
			if(operandType == SS_T_U1 ||
				operandType == SS_T_I1 ||
				operandType == SS_T_U2 ||
				operandType == SS_T_I2 ||
				operandType == SS_T_U4 ||
				operandType == SS_T_I4 ||
				operandType == SS_T_U8 ||
				operandType == SS_T_I8 ||
				operandType == SS_T_F4 ||
				operandType == SS_T_F8)
			{
				unExpr->SetOperand(operand);
				return unExpr;
			}
			else
			{
				ReportError(unExpr, "Cannot apply %s operator to operand of type %s",
							unExpr->GetOpName().c_str(),
							operandType->GetName().c_str());
				return new ErrorExpr();
			}
		}
		else
		{
			SS_UNREACHABLE;
			return 0;
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
// TODO: handle transform exprs
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
		char buf[32];
		sprintf(buf, "%d", castedExpr->GetValue());
		return buf;
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
		return "'"+nameExpr->GetIdentifier()+"'";
	}
	else if(expr->IsA<TernaryExpr>())
	{
		TernaryExpr* ternaryExpr = dynamic_cast<TernaryExpr*>(expr);
		return "ternary(" + DumpExpr(ternaryExpr->GetCondition()) + ", " + 
							DumpExpr(ternaryExpr->GetTrueExpr()) + ", " + 
							DumpExpr(ternaryExpr->GetFalseExpr()) + ")";
	}
	else if(expr->IsA<ThisExpr>())
	{
		return "this";
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

