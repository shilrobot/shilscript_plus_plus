#ifndef SS_ASSIGNEXPR_H
#define SS_ASSIGNEXPR_H

#include "Expr.h"
#include "BinaryExpr.h"

namespace SS {

class SS_EXPORT AssignExpr : public Expr
{
public:
	SS_CLASS(AssignExpr, "AssignExpr", Expr);

	AssignExpr(BinaryOp op, Expr* left, Expr* right) : m_op(op), m_left(left), m_right(right) {}

	virtual ~AssignExpr()
	{
		delete m_left;
		delete m_right;
	}

	SS_GETSET(BinaryOp, Op, m_op);
	SS_GETSET(Expr*, Left, m_left);
	SS_GETSET(Expr*, Right, m_right);

	// TODO: Other expr. stuff

private:
	BinaryOp m_op;
	Expr* m_left;
	Expr* m_right;
};

}

#endif // SS_ASSIGNEXPR_H
