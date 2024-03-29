#include "Prereqs.h"
#include "CodeGen.h"
#include "Function.h"
#include "Statement.h"
#include "AllExprs.h"
#include "Code.h"
#include "Type.h"
#include "CompileErrors.h"
#include "Local.h"
#include "Variable.h"

namespace SS {

CodeGen::CodeGen()
{
}

void CodeGen::Generate(Function* func)
{
	SSAssert(func != 0);
	m_func = func;
	m_code = new Code(func);	
	CodeGenContext ctx;
	EmitStatement(func->GetBody(), ctx);
	//m_code->Dump();
	m_func->SetCode(m_code);

	m_code = 0;
	m_func = 0;
}

void CodeGen::EmitStatement(const Statement* stmt, const CodeGenContext& ctx)
{
	if(stmt->IsA<EmptyStatement>())
	{
		// Nothing
	}
	else if(stmt->IsA<BreakStatement>())
	{
		SSAssert(ctx.breakLabel);
		m_code->jmp(ctx.breakLabel);
	}
	else if(stmt->IsA<ContinueStatement>())
	{
		SSAssert(ctx.continueLabel);
		m_code->jmp(ctx.continueLabel);
	}
	else if(stmt->IsA<ReturnStatement>())
	{
		const ReturnStatement* returnStmt = static_cast<const ReturnStatement*>(stmt);
		if(returnStmt->GetExpr() != 0)
			EmitExpr(returnStmt->GetExpr(), ctx);
		m_code->ret();
	}
	else if(stmt->IsA<ExprStatement>())
	{
		const ExprStatement* exprStmt = static_cast<const ExprStatement*>(stmt);
		EmitExpr(exprStmt->GetExpr(), ctx);
		if(exprStmt->GetExpr()->IsRValue())
		{
			// TODO: Better way of doing this...
			if(exprStmt->GetExpr()->IsA<AssignExpr>() && !exprStmt->GetExpr()->IsValueUsed())
				;
			else
				m_code->pop();
		}
	}
	else if(stmt->IsA<BlockStatement>())
	{
		const BlockStatement* blockStmt = static_cast<const BlockStatement*>(stmt);
		for(int i=0; i<blockStmt->GetStatementCount(); ++i)
			EmitStatement(blockStmt->GetStatement(i), ctx);
	}
	else if(stmt->IsA<IfStatement>())
	{
		const IfStatement* ifStmt = static_cast<const IfStatement*>(stmt);

		// If it has an else branch
		if(ifStmt->GetIfFalse())
		{
			CodeLabel* elseLabel = m_code->CreateLabel();
			CodeLabel* endLabel = m_code->CreateLabel();

			EmitExpr(ifStmt->GetCondition(), ctx);
			m_code->jmp_false(elseLabel);
			EmitStatement(ifStmt->GetIfTrue(), ctx);
			m_code->jmp(endLabel);
			m_code->PlaceLabel(elseLabel);
			EmitStatement(ifStmt->GetIfFalse(), ctx);
			m_code->PlaceLabel(endLabel);
		}
		else
		{
			CodeLabel* elseLabel = m_code->CreateLabel();

			EmitExpr(ifStmt->GetCondition(), ctx);
			m_code->jmp_false(elseLabel);
			EmitStatement(ifStmt->GetIfTrue(), ctx);
			m_code->PlaceLabel(elseLabel);
		}
	}
	else if(stmt->IsA<WhileStatement>())
	{
		const WhileStatement* whileStmt = static_cast<const WhileStatement*>(stmt);

		CodeLabel* startLabel = m_code->CreateLabel();
		CodeLabel* doneLabel = m_code->CreateLabel();

		m_code->PlaceLabel(startLabel);
		EmitExpr(whileStmt->GetCondition(), ctx);
		m_code->jmp_false(doneLabel);

		CodeGenContext ctxBody(ctx);
		ctxBody.breakLabel = doneLabel;
		ctxBody.continueLabel = startLabel;
		EmitStatement(whileStmt->GetBody(), ctxBody);

		m_code->jmp(startLabel);
		m_code->PlaceLabel(doneLabel);
	}
	else if(stmt->IsA<DoWhileStatement>())
	{
		const DoWhileStatement* doStmt = static_cast<const DoWhileStatement*>(stmt);

		CodeLabel* startLabel = m_code->CreateLabel();
		CodeLabel* continueLabel = m_code->CreateLabel();
		CodeLabel* doneLabel = m_code->CreateLabel();

		m_code->PlaceLabel(startLabel);

		CodeGenContext ctxBody(ctx);
		ctxBody.breakLabel = doneLabel;
		ctxBody.continueLabel = continueLabel;
		EmitStatement(doStmt->GetBody(), ctx);

		m_code->PlaceLabel(continueLabel);
		EmitExpr(doStmt->GetCondition(), ctx);
		m_code->jmp_true(startLabel);
		m_code->PlaceLabel(doneLabel);
	}
	else if(stmt->IsA<LocalDefStatement>())
	{
		const LocalDefStatement* localDefStmt = static_cast<const LocalDefStatement*>(stmt);

		for(int i=0; i<localDefStmt->GetDefCount(); ++i)
		{
			const LocalDef* def = localDefStmt->GetDef(i);
			if(def->GetInitExpr() != 0)
			{
				EmitExpr(def->GetInitExpr(), ctx);
				m_code->store_local(def->GetLocal()->GetID());
			}
		}
	}
	else
	{
		ReportError(stmt, "Unhandled statement type in codegen: %s", stmt->DynamicType()->GetName());
	}
}

void CodeGen::EmitExpr(const Expr* expr, const CodeGenContext& ctx)
{
	if(expr->IsA<AssignExpr>())
	{
		const AssignExpr* assignExpr = static_cast<const AssignExpr*>(expr);

		SSAssert(assignExpr->GetOp() == BINOP_NONE);

		const Expr* left = assignExpr->GetLeft( );
		const Expr* right = assignExpr->GetRight( );

		SSAssert(left != 0);
		SSAssert(right != 0);

		if(left->IsA<VariableRefExpr>())
		{
			const VariableRefExpr* varRefExpr = static_cast<const VariableRefExpr*>(left);

			SSAssert(!varRefExpr->GetVariable()->IsConst());
			if(varRefExpr->GetVariable()->IsStatic())
			{
				SSAssert(varRefExpr->GetLeft() == 0);

				EmitExpr(right, ctx);
				if(assignExpr->IsValueUsed())
					m_code->dup();
				m_code->store_static(varRefExpr->GetVariable());
			}
			else
			{
				SSAssert(varRefExpr->GetLeft() != 0);
				EmitExpr(varRefExpr->GetLeft(), ctx);
				EmitExpr(right, ctx);

				// dup_shift modifies the stack as such:
				// {a,b} -> {b,a,b}
				if(assignExpr->IsValueUsed())
					m_code->dup_shift();

				m_code->store_field(varRefExpr->GetVariable());
			}
		}
		else if(left->IsA<ParamRefExpr>())
		{
			const ParamRefExpr* paramRefExpr = static_cast<const ParamRefExpr*>(left);

			EmitExpr(right, ctx);
			if(assignExpr->IsValueUsed())
				m_code->dup();
			m_code->store_arg(paramRefExpr->GetParameter()->GetID());
		}
		else if(left->IsA<LocalRefExpr>())
		{
			const LocalRefExpr* localRefExpr = static_cast<const LocalRefExpr*>(left);
			
			EmitExpr(right, ctx);
			if(assignExpr->IsValueUsed())
				m_code->dup();
			m_code->store_local(localRefExpr->GetLocal()->GetID());
		}
		else
			SS_UNREACHABLE;
	}
	else if(expr->IsA<BinaryExpr>())
	{
		const BinaryExpr* binExpr = static_cast<const BinaryExpr*>(expr);

		// && and || have short circuiting
		if(binExpr->GetOp() == BINOP_LOG_AND)
		{
			CodeLabel* falseLabel = m_code->CreateLabel();
			CodeLabel* doneLabel = m_code->CreateLabel();

			EmitExpr(binExpr->GetLeft(), ctx);
			m_code->jmp_false(falseLabel);
			EmitExpr(binExpr->GetRight(), ctx);
			m_code->jmp(doneLabel);
			m_code->PlaceLabel(falseLabel);
			m_code->load_false();
			m_code->PlaceLabel(doneLabel);
		}
		else if(binExpr->GetOp() == BINOP_LOG_OR)
		{
			CodeLabel* trueLabel = m_code->CreateLabel();
			CodeLabel* doneLabel = m_code->CreateLabel();

			EmitExpr(binExpr->GetLeft(), ctx);
			m_code->jmp_true(trueLabel);
			EmitExpr(binExpr->GetRight(), ctx);
			m_code->jmp(doneLabel);
			m_code->PlaceLabel(trueLabel);
			m_code->load_true();
			m_code->PlaceLabel(doneLabel);
		}
		else
		{
			EmitExpr(binExpr->GetLeft(), ctx);
			EmitExpr(binExpr->GetRight(), ctx);

			switch(binExpr->GetOp())
			{
			case BINOP_ADD:
				m_code->add();
				break;
			case BINOP_SUB:
				m_code->sub();
				break;
			case BINOP_MUL:
				m_code->mul();
				break;
			case BINOP_DIV:
				m_code->div();
				break;
			case BINOP_MOD:
				m_code->mod();
				break;
			case BINOP_POW:
				m_code->pow();
				break;
			case BINOP_BIT_AND:
				m_code->and();
				break;
			case BINOP_BIT_OR:
				m_code->or();
				break;
			case BINOP_BIT_XOR:
				m_code->xor();
				break;
			case BINOP_SHL:
				m_code->shl();
				break;
			case BINOP_SHR:
				m_code->shr();
				break;
			case BINOP_LT:
				m_code->lt();
				break;
			case BINOP_LE:
				m_code->le();
				break;
			case BINOP_GT:
				m_code->gt();
				break;
			case BINOP_GE:
				m_code->ge();
				break;
			case BINOP_EQ:
				m_code->eq();
				break;
			case BINOP_NE:
				m_code->ne();
				break;
			default:
				SS_UNREACHABLE;
			}
		}
	}
	else if(expr->IsA<FunctionCallExpr>())
	{
		const FunctionCallExpr* funcCallExpr = static_cast<const FunctionCallExpr*>(expr);

		if(funcCallExpr->GetFunction()->IsStatic())
		{
			SSAssert(funcCallExpr->GetLeft() == 0);
			for(int i=0; i<funcCallExpr->GetParameterCount(); ++i)
				EmitExpr(funcCallExpr->GetParameter(i), ctx);
			m_code->call_static(funcCallExpr->GetFunction());
		}
		else
		{
			SSAssert(funcCallExpr->GetLeft() != 0);
			EmitExpr(funcCallExpr->GetLeft(), ctx);
			for(int i=0; i<funcCallExpr->GetParameterCount(); ++i)
				EmitExpr(funcCallExpr->GetParameter(i), ctx);
			m_code->call_method(funcCallExpr->GetFunction());
		}
	}
	else if(expr->IsA<CastExpr>())
	{
		const CastExpr* castExpr = static_cast<const CastExpr*>(expr);

		// TODO: Warn if the types going in & out aren't different (i.e., an extraneous cast)
		//       Problem is it's hard to know if it's done by the user or inserted by the code

		SSAssert(castExpr->GetType() != 0);
		SSAssert(castExpr->GetRight() != 0);

		EmitExpr(castExpr->GetRight(), ctx);
		m_code->convert(castExpr->GetType());
	}
	else if(expr->IsA<TernaryExpr>())
	{
		const TernaryExpr* ternaryExpr = static_cast<const TernaryExpr*>(expr);

		CodeLabel* falseLabel = m_code->CreateLabel();
		CodeLabel* doneLabel = m_code->CreateLabel();

		EmitExpr(ternaryExpr->GetCondition(), ctx);
		m_code->jmp_false(falseLabel);
		EmitExpr(ternaryExpr->GetTrueExpr(), ctx);
		m_code->jmp(doneLabel);
		m_code->PlaceLabel(falseLabel);
		EmitExpr(ternaryExpr->GetFalseExpr(), ctx);
		m_code->PlaceLabel(doneLabel);
	}
	else if(expr->IsA<ParamRefExpr>())
	{
		const ParamRefExpr* paramRefExpr = static_cast<const ParamRefExpr*>(expr);
		m_code->load_arg(paramRefExpr->GetParameter()->GetID());
	}
	else if(expr->IsA<LocalRefExpr>())
	{
		const LocalRefExpr* localRefExpr = static_cast<const LocalRefExpr*>(expr);
		m_code->load_local(localRefExpr->GetLocal()->GetID());
	}
	else if(expr->IsA<VariableRefExpr>())
	{
		const VariableRefExpr* varRefExpr = static_cast<const VariableRefExpr*>(expr);

		if(varRefExpr->GetVariable()->IsConst())
			m_code->load_const(varRefExpr->GetVariable());
		else if(varRefExpr->GetVariable()->IsConst())
			m_code->load_static(varRefExpr->GetVariable());
		else
			m_code->load_field(varRefExpr->GetVariable());
	}
	else if(expr->IsA<BoolLiteralExpr>())
	{
		const BoolLiteralExpr* boolLiteralExpr = static_cast<const BoolLiteralExpr*>(expr);
		if(boolLiteralExpr->GetValue())
			m_code->load_true();
		else
			m_code->load_false();
	}
	else if(expr->IsA<IntLiteralExpr>())
	{
		const IntLiteralExpr* intLiteralExpr = static_cast<const IntLiteralExpr*>(expr);
		m_code->load_imm_i4(intLiteralExpr->GetValue());
	}
	else if(expr->IsA<FloatLiteralExpr>())
	{
		const FloatLiteralExpr* floatLiteralExpr = static_cast<const FloatLiteralExpr*>(expr);
		f4 result = (float)atof(floatLiteralExpr->GetValue().c_str());
		m_code->load_imm_f4(result);
	}
	else if(expr->IsA<NullLiteralExpr>())
	{
		m_code->load_null();
	}
	else if(expr->IsA<ThisExpr>())
	{
		m_code->load_this();
	}
	else if(expr->IsA<UnaryExpr>())
	{
		const UnaryExpr* unaryExpr = static_cast<const UnaryExpr*>(expr);
		
		if(unaryExpr->GetOp() == UNOP_LOG_NOT)
		{
			EmitExpr(unaryExpr->GetOperand(), ctx);
			m_code->log_not();
		}
		else if(unaryExpr->GetOp() == UNOP_COMPLEMENT)
		{
			EmitExpr(unaryExpr->GetOperand(), ctx);
			m_code->inv();
		}
		else if(unaryExpr->GetOp() == UNOP_NEGATIVE)
		{
			EmitExpr(unaryExpr->GetOperand(), ctx);
			m_code->neg();
		}
		else if(unaryExpr->GetOp() == UNOP_POSITIVE)
		{
			EmitExpr(unaryExpr->GetOperand(), ctx);
			// NOTHING! Positive has no effect.
		}
		else
		{
			// TODO
			SS_UNIMPLEMENTED;
		}
	}
	else
	{
		ReportError(expr, "Unhandled expr type in codegen: %s", expr->DynamicType()->GetName());
	}
}

}
