#include "ClassType.h"
#include "Class.h"

namespace SS {

String ClassType::GetName() const
{
	return m_cls->GetFullName();
}

}
