#ifndef SS_TYPEEXPR_H
#define SS_TYPEEXPR_H

#include "Node.h"
#include "CompileErrors.h"
#include "Name.h"

namespace SS {


class TypeExpr : public Node
{
public:
	SS_CLASS(TypeExpr, "TypeExpr", Node)

	virtual String Dump() const=0;
};

// Just a name
class NameTypeExpr : public TypeExpr
{
public:
	SS_CLASS(NameTypeExpr, "NameTypeExpr", TypeExpr)

	NameTypeExpr() {}

	// TODO: Maybe this isn't the right way to do this
	SS_NAMED_NODE;

	String Dump() const { return GetName(); }
};

// TODO: Move elsewhere
enum BasicTypeId
{
	BT_VOID,
	BT_BOOL,
	BT_U1,
	BT_I1,
	BT_U2,
	BT_I2,
	BT_U4,
	BT_I4,
	BT_U8,
	BT_I8,
	BT_FLOAT,
	BT_DOUBLE,
	BT_CHAR,
	BT_STRING,
	BT_OBJECT,
};

class BasicTypeExpr : public TypeExpr
{
public:
	SS_CLASS(BasicTypeExpr, "BasicTypeExpr", TypeExpr)

	BasicTypeExpr(BasicTypeId typeId = BT_VOID) : m_typeid(typeId) {}

	SS_GETSET(BasicTypeId, TypeId, m_typeid);	

	// TODO: Join this with other code that has this same switch()
	String Dump() const
	{
		switch(m_typeid)
		{
			case BT_VOID: return "void";
			case BT_BOOL: return "bool";
			case BT_U1: return "byte";
			case BT_I1: return "sbyte";
			case BT_U2: return "ushort";
			case BT_I2: return "short";
			case BT_U4: return "uint";
			case BT_I4: return "int";
			case BT_U8: return "ulong";
			case BT_I8: return "long";
			case BT_FLOAT: return "float";
			case BT_DOUBLE: return "double";
			case BT_CHAR: return "char";
			case BT_STRING: return "string";
			case BT_OBJECT: return "object";
			default: SS_UNREACHABLE; return "???";
		}
	}
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

	String Dump() const 
	{
		return m_left->Dump() + "." + m_right->Dump();
	}
private:
	TypeExpr* m_left;
	TypeExpr* m_right;
};

// TODO: list<T>, dict<T,U>, typename keywords (int, object, ...)

}

#endif // SS_TYPEEXPR_H
