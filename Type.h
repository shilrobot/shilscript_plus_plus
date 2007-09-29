#ifndef SS_TYPE_H
#define SS_TYPE_H

#include "Node.h"
#include "TypeExpr.h"

namespace SS {

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

	BasicType(BasicTypeId typeId = BT_VOID) : m_typeid(typeId) {}

	SS_GETSET(BasicTypeId, TypeId, m_typeid);	
	// TODO: Implement desc
private:
	BasicTypeId m_typeid;
};

}

#endif // SS_TYPE_H
