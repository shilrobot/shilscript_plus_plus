#ifndef SS_TRANSFORMEXPRS_H
#define SS_TRANSFORMEXPRS_H

#include "Prereqs.h"
#include "Expr.h"

namespace SS {

class Variable;
class Function;
class Class;
class Parameter;
class Local;
class Package;

class SS_EXPORT ErrorExpr : public Expr
{
public:
	SS_CLASS(ErrorExpr, "ErrorExpr", Expr);
};

class SS_EXPORT PackageRefExpr : public Expr
{
public:
	SS_CLASS(VariableRefExpr, "PackageRefExpr", Expr);
	
	PackageRefExpr(Package* pkg) : m_pkg(pkg)
	{
		SetRValue(false);
	}

	SS_GETSET(Package*, Package, m_pkg);
private:
	Package* m_pkg;
};

class SS_EXPORT ClassRefExpr : public Expr
{
public:
	SS_CLASS(VariableRefExpr, "ClassRefExpr", Expr);
	
	ClassRefExpr(Class* cls) : m_cls(cls)
	{
		SetRValue(false);
	}

	SS_GETSET(Class*, Class, m_cls);
private:
	Class* m_cls;
};

class SS_EXPORT VariableRefExpr : public Expr
{
public:
	SS_CLASS(VariableRefExpr, "VariableRefExpr", Expr);
	
	VariableRefExpr(Expr* left, Variable* var) : m_left(left), m_var(var)
	{
	}

	SS_GETSET(Expr*, Left, m_left);
	SS_GETSET(Variable*, Variable, m_var);
private:
	Expr* m_left;
	Variable* m_var;
};

class SS_EXPORT ParamRefExpr : public Expr
{
public:
	SS_CLASS(ParamRefExpr, "ParamRefExpr", Expr);
	
	ParamRefExpr(Parameter* param) : m_param(param)
	{
	}

	SS_GETSET(Parameter*, Parameter, m_param);
private:
	Parameter* m_param;
};

class SS_EXPORT LocalRefExpr : public Expr
{
public:
	SS_CLASS(LocalRefExpr, "LocalRefExpr", Expr);
	
	LocalRefExpr(Local* local) : m_local(local)
	{
	}

	SS_GETSET(Local*, Local, m_local);
private:
	Local* m_local;
};

class SS_EXPORT FunctionRefExpr : public Expr
{
public:
	SS_CLASS(FunctionRefExpr, "FunctionRefExpr", Expr);

	FunctionRefExpr(Expr* left, Function* func) : m_left(left), m_func(func)
	{
		SetRValue(false);
	}

	SS_GETSET(Expr*, Left, m_left);
	SS_GETSET(Function*, Function, m_func);
private:

	Expr* m_left;
	Function* m_func;
};

class SS_EXPORT FunctionCallExpr : public Expr
{
public:
	SS_CLASS(CallExpr, "FunctionCallExpr", Expr);

	FunctionCallExpr(Expr* left, Function* func) : m_left(left), m_func(func)
	{
	}

	virtual ~FunctionCallExpr() { delete m_left; }

	SS_GETSET(Expr*, Left, m_left);
	SS_GETSET(Function*, Function, m_func);
	SS_ADD(Expr*, AddParameter, m_params);
	SS_INDEX(Expr*, Parameter, m_params);
private:
	Expr*				m_left;
	Function*			m_func;
	std::vector<Expr*>	m_params;
};



}

#endif // SS_TRANSFORMEXPRS_H

