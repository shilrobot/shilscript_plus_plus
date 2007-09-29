#ifndef SS_NAMEEXPR_H
#define SS_NAMEEXPR_H

#include "Expr.h"

namespace SS {

class SS_EXPORT NameExpr : public Expr
{
public:
	SS_CLASS(NameExpr, "NameExpr", Expr);

	NameExpr(const String& id) : m_id(id) { }

	virtual ~NameExpr()
	{
	}

	SS_GETSET(const String&, Identifier, m_id);

private:
	String m_id;
};

}

#endif // SS_NAMEEXPR_H
