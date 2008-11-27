#include "StaticChecker.h"
#include "Statement.h"
#include "Function.h"
#include "AllExprs.h"
#include "Type.h"
#include "Scope.h"
#include "TypeLookup.h"
#include "Local.h"
#include "TypeConversion.h"
#include "NameLookup.h"
#include "Variable.h"
#include "Package.h"
#include "Class.h"

namespace SS {

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
		OVERLOAD_ADD(SS_T_I8, SS_T_U8, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_U8, SS_T_I8, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_F4, SS_T_F4, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_F8, SS_T_F8, SS_T_BOOL)
	OVERLOAD_END

	OVERLOAD_BEGIN(BINOP_LE, "<=")
		OVERLOAD_ADD(SS_T_I4, SS_T_I4, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_U4, SS_T_U4, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_I8, SS_T_I8, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_U8, SS_T_U8, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_I8, SS_T_U8, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_U8, SS_T_I8, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_F4, SS_T_F4, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_F8, SS_T_F8, SS_T_BOOL)
	OVERLOAD_END
	
	OVERLOAD_BEGIN(BINOP_GT, ">")
		OVERLOAD_ADD(SS_T_I4, SS_T_I4, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_U4, SS_T_U4, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_I8, SS_T_I8, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_U8, SS_T_U8, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_I8, SS_T_U8, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_U8, SS_T_I8, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_F4, SS_T_F4, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_F8, SS_T_F8, SS_T_BOOL)
	OVERLOAD_END

	OVERLOAD_BEGIN(BINOP_GE, ">=")
		OVERLOAD_ADD(SS_T_I4, SS_T_I4, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_U4, SS_T_U4, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_I8, SS_T_I8, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_U8, SS_T_U8, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_I8, SS_T_U8, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_U8, SS_T_I8, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_F4, SS_T_F4, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_F8, SS_T_F8, SS_T_BOOL)
	OVERLOAD_END

	OVERLOAD_BEGIN(BINOP_EQ, "==")
		OVERLOAD_ADD(SS_T_BOOL, SS_T_BOOL, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_I4, SS_T_I4, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_U4, SS_T_U4, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_I8, SS_T_I8, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_U8, SS_T_U8, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_I8, SS_T_U8, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_U8, SS_T_I8, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_F4, SS_T_F4, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_F8, SS_T_F8, SS_T_BOOL)
		// TODO: Better job than just adding 'object' here...?
		OVERLOAD_ADD(SS_T_OBJECT, SS_T_OBJECT, SS_T_BOOL)
	OVERLOAD_END

	OVERLOAD_BEGIN(BINOP_NE, "!=")
		OVERLOAD_ADD(SS_T_BOOL, SS_T_BOOL, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_I4, SS_T_I4, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_U4, SS_T_U4, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_I8, SS_T_I8, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_U8, SS_T_U8, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_I8, SS_T_U8, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_U8, SS_T_I8, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_F4, SS_T_F4, SS_T_BOOL)
		OVERLOAD_ADD(SS_T_F8, SS_T_F8, SS_T_BOOL)
		// TODO: Better job than just adding 'object' here...?
		OVERLOAD_ADD(SS_T_OBJECT, SS_T_OBJECT, SS_T_BOOL)
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

Expr* StaticChecker::CheckLValue(Expr* expr, const ExprContext& ctx)
{
	SSAssert(expr != 0);

	Expr* xformed = CheckExpr(expr, ctx);

	SSAssert(xformed != 0);

	if(xformed->IsA<ErrorExpr>())
		return xformed;
	else if(!xformed->IsLValue())
	{
		ReportError(expr, "Expected an lvalue expression, got a non-lvalue");
		return new ErrorExpr();
	}
	else
	{
		SSAssert(xformed->GetResultType() != 0);
		SSAssert(xformed->GetResultType() != SS_T_VOID); // lvalues should never have type 'void'

		return xformed;
	}
}

Expr* StaticChecker::CheckRValue(Expr* expr, const ExprContext& ctx)
{
	SSAssert(expr != 0);

	Expr* xformed = CheckExpr(expr, ctx);

	SSAssert(xformed != 0);

	if(xformed->IsA<ErrorExpr>())
		return xformed;
	else if(!xformed->IsRValue())
	{
		ReportError(expr, "Expected an rvalue expression, got a non-rvalue");
		return new ErrorExpr();
	}
	else
	{
		SSAssert(xformed->GetResultType() != 0);

		if(xformed->GetResultType() == SS_T_VOID)
		{
			ReportError(expr, "Expected an rvalue expression, got void");
			return new ErrorExpr();
		}

		return xformed;
	}
}

Expr* StaticChecker::CheckRValueWithType(Expr* expr, const Type* desiredType, const ExprContext& ctx)
{
	SSAssert(desiredType);
	SSAssert(desiredType != SS_T_VOID);

	Expr* xformed = CheckRValue(expr, ctx);
	SSAssert(xformed);

	if(xformed->IsA<ErrorExpr>())
		return xformed;
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
					return new ErrorExpr();
				}

				if(func2->IsStatic())
					return new FunctionRefExpr(0, func2);
				else
					return new FunctionRefExpr(new ThisExpr( ), func2);
			}
			else if(node->IsA<Package>())
			{
				Package* pkg = dynamic_cast<Package*>(node);	
				return new PackageRefExpr(pkg);
			}
			else if(node->IsA<Class>())
			{
				Class* cls = dynamic_cast<Class*>(node);	
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
			return new ErrorExpr();
		}

		Expr* left = CheckLValue(assignExpr->GetLeft(), ctx);
		Expr* right = CheckRValue(assignExpr->GetRight(), ctx);

		// TODO: Maybe there's a better way to do error signaling than this
		//		 Use NULL exprs?
		if(left->IsA<ErrorExpr>() ||
			right->IsA<ErrorExpr>())
		{
			return new ErrorExpr();
		}

		const Type* leftType = left->GetResultType();
		const Type* rightType = right->GetResultType();

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

		Expr* left = CheckRValue(binExpr->GetLeft(), ctx);
		Expr* right = CheckRValue(binExpr->GetRight(), ctx);

		// TODO: Maybe there's a better way to do error signaling than this
		//		 Use NULL exprs?
		if(left->IsA<ErrorExpr>() ||
			right->IsA<ErrorExpr>())
		{
			return new ErrorExpr();
		}

		const Type* leftType = left->GetResultType();
		const Type* rightType = right->GetResultType();

		/*if(binExpr->GetOp() == BINOP_EQ ||
			binExpr->GetOp() == BINOP_NE)
		{
			ReportError(expr, "== and != are temporarily unsupported");
			return new ErrorExpr();
		}
		else*/
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
			Expr* paramXformed = CheckRValue(paramOriginal, ctx);
			SSAssert(func->GetParameter(i)->GetType() != 0);

			if(paramXformed->IsA<ErrorExpr>())
			{
				errored = true;
				continue;
			}

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
		Expr* right = CheckRValue(castExpr->GetRight(), ctx);

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
			ReportError(castExpr, "Cannot convert from %s to %s", castExpr->GetType()->GetName().c_str(), right->GetResultType()->GetName().c_str());
			return new ErrorExpr();
		}
	}
	//---------------------------------------------------------------
	// TERNARY EXPRESSION
	//---------------------------------------------------------------
	else if(expr->IsA<TernaryExpr>())
	{
		TernaryExpr* ternExpr = dynamic_cast<TernaryExpr*>(expr);

		Expr* cond = CheckRValue(ternExpr->GetCondition(), ctx);
		Expr* ifTrue = CheckRValue(ternExpr->GetTrueExpr(), ctx);
		Expr* ifFalse = CheckRValue(ternExpr->GetFalseExpr(), ctx);

		// TODO: Maybe break this up so independent errors can get reported better? I dunno.
		// e.g., ifFalse is wrong, but cond also doesn't convert to bool, etc.
		if(cond->IsA<ErrorExpr>() ||
			ifTrue->IsA<ErrorExpr>() ||
			ifFalse->IsA<ErrorExpr>())
			return new ErrorExpr();

		const Type* condType = cond->GetResultType();
		const Type* ifTrueType = ifTrue->GetResultType();
		const Type* ifFalseType = ifFalse->GetResultType();

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
			ternExpr->SetTrueExpr(ifTrue);
			ternExpr->SetFalseExpr(ifFalse);
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

		// TODO: This has to be rvalue or lvalue depending on what op type...

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

}