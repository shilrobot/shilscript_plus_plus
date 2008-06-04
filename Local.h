#ifndef SS_LOCAL_H
#define SS_LOCAL_H

#include "Node.h"

namespace SS {

// Local variable
class SS_EXPORT Local  : public Node
{
public:
	SS_CLASS(Local, "Local", Node);

	Local() : m_type(0), m_static(false), m_const(false), m_id(-1) {}
	virtual ~Local() {}

	SS_GETSET(Type*, Type, m_type);
	SS_GETSET_BOOL(IsStatic, SetStatic, m_static);
	SS_GETSET_BOOL(IsConst, SetConst, m_const);
	SS_GETSET(int, ID, m_id);

	SS_NAMED_NODE;

	SS_NODE_TYPE_NAME("local variable");

private:
	Type*		m_type;
	bool		m_static;
	bool		m_const;
	int			m_id;
};

}

#endif // SS_LOCAL_H
