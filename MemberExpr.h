#ifndef SS_MEMBEREXPR_H
#define SS_MEMBEREXPR_H

#include "Expr.h"

namespace SS {

class SS_EXPORT MemberExpr : public Expr
{
public:
	SS_CLASS(MemberExpr, "MemberExpr", Expr);

	MemberExpr(Expr* left, const String& id) : m_left(left), m_id(id) { }

	virtual ~MemberExpr()
	{
		delete m_left;
	}

	SS_GETSET(Expr*, Left, m_left);
	SS_GETSET(const String&, Identifier, m_id);

	// TODO: Other expr. stuff

private:
	Expr* m_left;
	String m_id;
};

}

#endif // SS_MEMBEREXPR_H
