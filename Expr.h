#ifndef SS_EXPR_H
#define SS_EXPR_H

#include "Node.h"

class Type;

namespace SS {

class SS_EXPORT Expr : public Node
{
public:
	SS_CLASS(Expr, "Expr", Node);

	Expr() : m_resultType(0), m_lvalue(false), m_valueUsed(true), m_rvalue(true)
	{
	}

	SS_GETSET(const Type*, ResultType, m_resultType);
	SS_GETSET_BOOL(IsLValue, SetLValue, m_lvalue);
	SS_GETSET_BOOL(IsRValue, SetRValue, m_rvalue);

	// This is a little hacky
	SS_GETSET_BOOL(IsValueUsed, SetValueUsed, m_valueUsed);

private:
	const Type* m_resultType;
	bool m_lvalue;
	bool m_rvalue;
	bool m_valueUsed;
};

}

#endif // SS_EXPR_H
