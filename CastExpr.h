#ifndef SS_CASTEXPR_H
#define SS_CASTEXPR_H

#include "Expr.h"
#include "TypeExpr.h"

namespace SS {

class SS_EXPORT CastExpr : public Expr
{
public:
	SS_CLASS(CastExpr, "CastExpr", Expr);

	CastExpr(TypeExpr* typeExpr, Expr* right) : m_typeExpr(typeExpr), m_type(0), m_right(right) {}
	CastExpr(const Type* type, Expr* right) : m_typeExpr(0), m_type(type), m_right(right) {}

	virtual ~CastExpr()
	{
		delete m_typeExpr;
		delete m_right;
	}

	SS_GETSET(TypeExpr*, TypeExpr, m_typeExpr);
	SS_GETSET(Expr*, Right, m_right);
	SS_GETSET(const Type*, Type, m_type);
private:
	TypeExpr*			m_typeExpr;
	const Type*			m_type;
	Expr*				m_right;
};

}

#endif // SS_CASTEXPR_H
