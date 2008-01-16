#ifndef SS_TYPE_H
#define SS_TYPE_H

#include "Node.h"
#include "TypeExpr.h"

namespace SS {

#define SS_T_VOID (SS::Type::GetBasicType(BT_VOID))
#define SS_T_BOOL (SS::Type::GetBasicType(BT_BOOL))
#define SS_T_U1 (SS::Type::GetBasicType(BT_U1))
#define SS_T_S1 (SS::Type::GetBasicType(BT_S1))
#define SS_T_U2 (SS::Type::GetBasicType(BT_U2))
#define SS_T_S2 (SS::Type::GetBasicType(BT_S2))
#define SS_T_U4 (SS::Type::GetBasicType(BT_U4))
#define SS_T_S4 (SS::Type::GetBasicType(BT_S4))
#define SS_T_U8 (SS::Type::GetBasicType(BT_U8))
#define SS_T_S8 (SS::Type::GetBasicType(BT_S8))
#define SS_T_FLOAT (SS::Type::GetBasicType(BT_FLOAT))
#define SS_T_DOUBLE (SS::Type::GetBasicType(BT_DOUBLE))
#define SS_T_CHAR (SS::Type::GetBasicType(BT_CHAR))
#define SS_T_STRING (SS::Type::GetBasicType(BT_STRING))
#define SS_T_OBJECT (SS::Type::GetBasicType(BT_OBJECT))

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
