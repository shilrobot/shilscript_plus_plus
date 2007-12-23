#ifndef SS_EXPR_H
#define SS_EXPR_H

#include "Node.h"

class Type;

namespace SS {

class SS_EXPORT Expr : public Node
{
public:
	SS_CLASS(Expr, "Expr", Node);

	Expr() : m_resultType(0)
	{
	}

	SS_GETSET(Type*, ResultType, m_resultType);
	
private:
	Type* m_resultType;
};

class SS_EXPORT ErrorExpr : public Expr
{
public:
	SS_CLASS(ErrorExpr, "ErrorExpr", Expr);
}

}

#endif // SS_EXPR_H
