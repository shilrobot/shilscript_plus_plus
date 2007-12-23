#ifndef SS_TYPE_H
#define SS_TYPE_H

#include "Node.h"
#include "TypeExpr.h"

namespace SS {

#define SS_T_VOID (SS::Type::GetBasicType(BT_VOID))
#define SS_T_BOOL (SS::Type::GetBasicType(BT_BOOL))
#define SS_T_BYTE (SS::Type::GetBasicType(BT_BYTE))
#define SS_T_SBYTE (SS::Type::GetBasicType(BT_SBYTE))
#define SS_T_USHORT (SS::Type::GetBasicType(BT_USHORT))
#define SS_T_SHORT (SS::Type::GetBasicType(BT_SHORT))
#define SS_T_UINT (SS::Type::GetBasicType(BT_UINT))
#define SS_T_INT (SS::Type::GetBasicType(BT_INT))
#define SS_T_ULONG (SS::Type::GetBasicType(BT_ULONG))
#define SS_T_LONG (SS::Type::GetBasicType(BT_LONG))
#define SS_T_FLOAT (SS::Type::GetBasicType(BT_FLOAT))
#define SS_T_DOUBLE (SS::Type::GetBasicType(BT_DOUBLE))
#define SS_T_STRING (SS::Type::GetBasicType(BT_STRING))
#define SS_T_OBJECT (SS::Type::GetBasicType(BT_OBJECT))
// TODO: Char

class SS_EXPORT Type : public Node
{
public:
	SS_CLASS(Type, "Type", Node)
	SS_NAMED_NODE
	SS_CONTAINER_NODE
	SS_NODE_TYPE_NAME("type");

	static Type* GetBasicType(BasicTypeId id);
};


// TODO: Break this up when we get more actual types, of course.
class SS_EXPORT BasicType : public Type
{
public:
	SS_CLASS(BasicType, "BasicType", Type)

	BasicType(BasicTypeId typeId = BT_VOID, const String& typeName = "") : m_typeid(typeId), m_typeName(typeName) {}

	SS_GETSET(BasicTypeId, TypeId, m_typeid);
	// TODO: Implement desc

	String GetDesc() const { return m_typeName; }

private:
	BasicTypeId m_typeid;
	String m_typeName;
};

}

#endif // SS_TYPE_H
