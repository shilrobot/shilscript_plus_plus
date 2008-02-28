#ifndef SS_CLASSTYPE_H
#define SS_CLASSTYPE_H

#include "Prereqs.h"
#include "Type.h"

namespace SS {

class Class;

class ClassType : public Type
{
public:
	SS_CLASS(ClassType, "ClassType", Type)

	ClassType(Class* cls) : m_cls(cls)
	{
		SSAssert(cls != 0);
	}

	Class* GetClass() const { return m_cls; }	

	String GetName() const;

public:
	Class* m_cls;
};

}

#endif // SS_CLASSTYPE_H

