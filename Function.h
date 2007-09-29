#ifndef SS_FUNCTION_H
#define SS_FUNCTION_H

#include "Node.h"

namespace SS {

class TypeExpr;
class Type;
class Class;
class Expr;
class Statement;

class SS_EXPORT Parameter : public Node
{
public:
	SS_CLASS(Parameter, "Parameter", Node)

	SS_GETSET(TypeExpr*, TypeExpr, m_typeExpr);
	SS_GETSET(Type*, Type, m_type);

	SS_NAMED_NODE

	SS_NODE_TYPE_NAME("parameter");

private:
	TypeExpr*	m_typeExpr;
	Type*		m_type;
	// TODO: Flags
	// TODO: Init expr.
};

class SS_EXPORT Function : public Node
{
public:
	SS_CLASS(Function, "Function", Node)

	Function();
	virtual ~Function();

	bool Check();

	SS_NAMED_NODE
	SS_GETSET(AccessType, Access, m_access);
	SS_GETSET(TypeExpr*, ReturnTypeExpr, m_returnTypeExpr);
	SS_GETSET(Type*, ReturnType, m_returnType);
	SS_GETSET(Statement*, Body, m_body);
	SS_ADD(Parameter*, AddParameter, m_params);
	SS_INDEX(Parameter*, Parameter, m_params);
	SS_GETSET_BOOL(IsStatic, SetStatic, m_static);
	SS_GETSET_BOOL(IsVirtual, SetVirtual, m_virtual);
	SS_GETSET_BOOL(IsSealed, SetSealed, m_sealed);

	SS_NODE_TYPE_NAME("function");

private:
	AccessType	m_access;
	TypeExpr*	m_returnTypeExpr;
	Type*		m_returnType;
	Statement*	m_body;

	typedef std::vector<Parameter*> ParameterVector;

	ParameterVector	m_params;
	bool		m_static;
	bool		m_virtual;
	bool		m_sealed;

	// TODO: Code, locals, ...
	// TODO: more flags?
};

}

#endif // SS_FUNCTION_H