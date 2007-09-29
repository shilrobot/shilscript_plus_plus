#include "Node.h"
#include "TypeExpr.h"
#include "CompileErrors.h"
#include "Type.h"
#include "Scope.h"

namespace SS
{

FileInfo FileInfo::NoFile("??");


Node::Node() : m_file(&FileInfo::NoFile),
				m_line(0),
				m_parent(0),
				m_scope(0),
				m_accessDomain(AccessDomain::Universal())
{
}

Node::~Node()
{
	delete m_scope;
}

const String& Node::GetFilename() const
{
	return m_file->GetFilename();
}

Scope* Node::GetScope() const
{
	if(m_scope == 0)
		m_scope = new Scope();
	return m_scope;
}



}
