#ifndef SS_EXPR_H
#define SS_EXPR_H

#include "Node.h"

class Type;

namespace SS {

class SS_EXPORT Expr : public Node
{
public:
	SS_CLASS(Expr, "Expr", Node);

	Expr() : m_resultType(0), m_lvalue(false)
	{
	}

	SS_GETSET(const Type*, ResultType, m_resultType);
	SS_GETSET_BOOL(IsLValue, SetLValue, m_lvalue);

private:
	const Type* m_resultType;
	bool m_lvalue;
};

}

#endif // SS_EXPR_H
