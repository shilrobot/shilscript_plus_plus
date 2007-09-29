#ifndef SS_CLASS_H
#define SS_CLASS_H

#include "Type.h"
#include "Scope.h"

namespace SS {

class Function;
class Variable;
class TypeExpr;
class Package;

class SS_EXPORT Class : public Type
{
public:
	SS_CLASS(Class, "Class", Type)

	Class();
	virtual ~Class();

	SS_GETSET(AccessType, Access, m_access);
	SS_GETSET(TypeExpr*, BaseExpr, m_baseExpr);
	SS_GETSET(Class*, Base, m_base);
	SS_ADD(Variable*, AddVariable, m_vars);
	SS_ADD(Function*, AddFunction, m_funcs);
	SS_GETSET(Package*, Package, m_package);

	SS_GETSET_BOOL(IsProcessed, SetProcessed, m_processed);
	SS_GETSET_BOOL(Failed, SetFailed, m_failed);

	SS_INDEX(Variable*, Variable, m_vars);
	SS_INDEX(Function*, Function, m_funcs);

	SS_NODE_TYPE_NAME("class");

private:
	AccessType	m_access;
	TypeExpr*	m_baseExpr;
	Class*		m_base;
	bool		m_processed;
	bool		m_failed;
	Package*	m_package;

	typedef std::vector<Variable*> VariableVector;
	typedef std::vector<Function*> FunctionVector;

	VariableVector	m_vars;
	FunctionVector	m_funcs;

	// TODO: Other flags: final, native, ...
};

}

#endif // SS_CLASS_H
