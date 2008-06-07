#include "StaticChecker.h"
#include "Statement.h"
#include "Function.h"
#include "AllExprs.h"
#include "Type.h"
#include "Scope.h"
#include "TypeLookup.h"
#include "Local.h"

namespace SS {

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

				Expr* returnExpr = CheckRValueWithType(returnStmt->GetExpr(), ctx.func->GetReturnType(), exprCtx);
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
		xformed->SetValueUsed(false);
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

		Expr* cond = CheckRValueWithType(ifStmt->GetCondition(), SS_T_BOOL, exprCtx);
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

		Expr* cond = CheckRValueWithType(whileStmt->GetCondition(), SS_T_BOOL, exprCtx);
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

		Expr* cond = CheckRValueWithType(doStmt->GetCondition(), SS_T_BOOL, exprCtx);
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
				Expr* initExpr = CheckRValueWithType(def->GetInitExpr(), typ, exprCtx);
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

}