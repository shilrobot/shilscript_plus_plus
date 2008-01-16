#ifndef SS_TRANSFORMEXPRS_H
#define SS_TRANSFORMEXPRS_H

#include "Prereqs.h"
#include "Expr.h"

namespace SS {

class Variable;
class Function;

class SS_EXPORT ErrorExpr : public Expr
{
public:
	SS_CLASS(ErrorExpr, "ErrorExpr", Expr);
};

class SS_EXPORT VariableRefExpr : public Expr
{
public:
	SS_CLASS(VariableRefExpr, "VariableRefExpr", Expr);
	
	VariableRefExpr() : m_var(0)
	{
	}

	SS_GETSET(Variable*, Variable, m_var);
private:
	Variable* m_var;
};

// TODO: LocalExpr
// TODO: ParameterExpr

class SS_EXPORT FunctionRefExpr : public Expr
{
public:
	SS_CLASS(FunctionRefExpr, "FunctionRefExpr", Expr);

	FunctionRefExpr() : m_left(0), m_func(0)
	{
	}

	SS_GETSET(Expr*, Left, m_left);
	SS_GETSET(Function*, Function, m_func);
private:

	Expr* m_left;
	Function* m_func;
};


}

#endif // SS_TRANSFORMEXPRS_H

