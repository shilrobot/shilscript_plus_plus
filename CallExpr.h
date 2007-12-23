#ifndef SS_CALLEXPR_H
#define SS_CALLEXPR_H

#include "Expr.h"

namespace SS {

class SS_EXPORT CallExpr : public Expr
{
public:
	SS_CLASS(CallExpr, "CallExpr", Expr);

	CallExpr(Expr* left) : m_left(left) {}

	virtual ~CallExpr() { delete m_left; }

	SS_GETSET(Expr*, Left, m_left);
	SS_ADD(Expr*, AddParameter, m_params);
	SS_INDEX(Expr*, Parameter, m_params);
private:
	Expr*				m_left;
	std::vector<Expr*>	m_params;
};

}

#endif // SS_CALLEXPR_H
