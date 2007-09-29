#ifndef SS_VARIABLE_H
#define SS_VARIABLE_H

#include "Node.h"

namespace SS {

class TypeExpr;
class Type;
class Expr;
class EmitContext;

// Global or member variable
class SS_EXPORT Variable  : public Node
{
public:
	SS_CLASS(Variable, "Variable", Node);

	Variable();
	virtual ~Variable();

	SS_GETSET(AccessType, Access, m_access);
	SS_GETSET(TypeExpr*, TypeExpr, m_typeExpr);
	SS_GETSET(Type*, Type, m_type);
	SS_GETSET(Expr*, InitExpr, m_initExpr);
	SS_GETSET_BOOL(IsStatic, SetStatic, m_static);
	SS_GETSET_BOOL(IsConst, SetConst, m_const);

	SS_NAMED_NODE;

	SS_NODE_TYPE_NAME("variable");

private:
	AccessType	m_access;
	TypeExpr*	m_typeExpr;
	Type*		m_type;
	Expr*		m_initExpr;
	bool		m_static;
	bool		m_const;

	// TODO: Group name/membership(?)
	// TODO: Info about packing/location (for code emission?) -- maybe another layer...
};

}

#endif // SS_VARIABLE_H
