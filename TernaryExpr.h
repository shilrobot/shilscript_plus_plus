#ifndef SS_TERNARYEXPR_H
#define SS_TERNARYEXPR_H

#include "Expr.h"

namespace SS {

class SS_EXPORT TernaryExpr : public Expr
{
public:
	SS_CLASS(TernaryExpr, "TernaryExpr", Expr);

	TernaryExpr(Expr* cond, Expr* ifTrue, Expr* ifFalse) : m_cond(cond), m_true(ifTrue), m_false(ifFalse) {}

	virtual ~TernaryExpr()
	{
		delete m_cond;
		delete m_true;
		delete m_false;
	}

	SS_GETSET(Expr*, Condition, m_cond);
	SS_GETSET(Expr*, TrueExpr, m_true);
	SS_GETSET(Expr*, FalseExpr, m_false);

	// TODO: Other expr. stuff

private:
	Expr* m_cond;
	Expr* m_true;
	Expr* m_false;
};

}

#endif // SS_TERNARYEXPR_H
