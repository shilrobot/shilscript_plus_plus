#ifndef SS_INDEXEXPR_H
#define SS_INDEXEXPR_H

#include "Expr.h"

namespace SS {

class SS_EXPORT IndexExpr : public Expr
{
public:
	SS_CLASS(IndexExpr, "IndexExpr", Expr);

	IndexExpr(Expr* left, Expr* right) : m_left(left), m_right(right) {}

	virtual ~IndexExpr()
	{
		delete m_left;
		delete m_right;
	}

	SS_GETSET(Expr*, Left, m_left);
	SS_GETSET(Expr*, Right, m_right);

private:
	Expr* m_left;
	Expr* m_right;
};

}

#endif // SS_INDEXEXPR_H
