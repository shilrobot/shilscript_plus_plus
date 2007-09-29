#ifndef SS_CASTEXPR_H
#define SS_CASTEXPR_H

#include "Expr.h"
#include "TypeExpr.h"

namespace SS {

class SS_EXPORT CastExpr : public Expr
{
public:
	SS_CLASS(CastExpr, "CastExpr", Expr);

	CastExpr(TypeExpr* type, Expr* right) : m_type(type), m_right(right) {}

	virtual ~CastExpr()
	{
		delete m_type;
		delete m_right;
	}

	SS_GETSET(TypeExpr*, TypeExpr, m_type);
	SS_GETSET(Expr*, Right, m_right);
private:
	TypeExpr*			m_type;
	Expr*				m_right;
};

}

#endif // SS_CASTEXPR_H
