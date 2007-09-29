#ifndef SS_UNARYEXPR_H
#define SS_UNARYEXPR_H

#include "Expr.h"

namespace SS {

enum UnaryOp
{
	UNOP_PRE_INC,
	UNOP_PRE_DEC,
	UNOP_POST_INC,
	UNOP_POST_DEC,
	UNOP_LOG_NOT,
	UNOP_COMPLEMENT,
	UNOP_NEGATIVE,
	UNOP_POSITIVE
};

class SS_EXPORT UnaryExpr : public Expr
{
public:
	SS_CLASS(UnaryExpr, "UnaryExpr", Expr);

	UnaryExpr(UnaryOp op, Expr* operand) : m_op(op), m_operand(operand) {}

	virtual ~UnaryExpr()
	{
		delete m_operand;
	}

	SS_GETSET(UnaryOp, Op, m_op);
	SS_GETSET(Expr*, Operand, m_operand);

	// TODO: Other expr. stuff

private:
	UnaryOp m_op;
	Expr* m_operand;
};

}

#endif // SS_UNARYEXPR_H
