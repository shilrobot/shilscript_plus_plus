#include "Class.h"
#include "Variable.h"
#include "Function.h"
#include "ClassType.h"

namespace SS {

Class::Class() : m_access(ACCESS_PUBLIC),
				m_baseExpr(0),
				m_base(0),
				m_processed(false),
				m_failed(false),
				m_package(0),
				m_type(0)
{
	m_type = new ClassType(this);
}

Class::~Class()
{
	for(size_t i = 0; i < m_vars.size(); ++i)
		delete m_vars[i];

	for(size_t i = 0; i < m_funcs.size(); ++i)
		delete m_funcs[i];

	delete m_type;
}

}
