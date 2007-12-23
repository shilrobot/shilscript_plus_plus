#ifndef SS_TYPEEXPR_H
#define SS_TYPEEXPR_H

#include "Node.h"
#include "CompileErrors.h"
#include "Name.h"

namespace SS {


class TypeExpr : public Node
{
	SS_CLASS(TypeExpr, "TypeExpr", Node)
};

// Just a name
class NameTypeExpr : public TypeExpr
{
public:
	SS_CLASS(NameTypeExpr, "NameTypeExpr", TypeExpr)

	NameTypeExpr() {}

	// TODO: Maybe this isn't the right way to do this
	SS_NAMED_NODE;
};

// TODO: Char, goddammit!
enum BasicTypeId
{
	BT_VOID,
	BT_BOOL,
	BT_BYTE,
	BT_SBYTE,
	BT_SHORT,
	BT_USHORT,
	BT_INT,
	BT_UINT,
	BT_LONG,
	BT_ULONG,
	BT_FLOAT,
	BT_DOUBLE,
	BT_STRING,
	BT_OBJECT,
};

class BasicTypeExpr : public TypeExpr
{
public:
	SS_CLASS(BasicTypeExpr, "BasicTypeExpr", TypeExpr)

	BasicTypeExpr(BasicTypeId typeId = BT_VOID) : m_typeid(typeId) {}

	SS_GETSET(BasicTypeId, TypeId, m_typeid);	
private:
	BasicTypeId m_typeid;
};

class DottedTypeExpr : public TypeExpr
{
public:
	SS_CLASS(DottedTypeExpr, "DottedTypeExpr", TypeExpr)

	DottedTypeExpr() : m_left(0), m_right(0)
	{
	}
	
	~DottedTypeExpr()
	{
		delete m_left;
		delete m_right;
	}

	SS_GETSET(TypeExpr*, Left, m_left);
	SS_GETSET(TypeExpr*, Right, m_right);
private:
	TypeExpr* m_left;
	TypeExpr* m_right;
};

// TODO: list<T>, dict<T,U>, typename keywords (int, object, ...)

}

#endif // SS_TYPEEXPR_H
