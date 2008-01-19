#ifndef SS_LOCATABLE_H
#define SS_LOCATABLE_H

#include "Prereqs.h"
#include "Base.h"
#include "AccessDomain.h"

namespace SS {

enum AccessType
{
	ACCESS_PRIVATE = 0,
	ACCESS_PROTECTED = 1,
	ACCESS_PUBLIC = 2
};

class SS_EXPORT FileInfo
{
public:
	FileInfo(const String& file) : m_filename(file)
	{
	}

	const String& GetFilename() const { return m_filename; }

private:
	String	m_filename;

public:

	static FileInfo NoFile;
};

class TypeExpr;
class Type;
class Scope;

#define SS_NAMED_NODE /*public: bool IsNamed() const { return true; }*/
#define SS_CONTAINER_NODE /*public: bool IsContainer() const { return true; }*/
#define SS_NODE_TYPE_NAME(s) public: String GetTypeName() const { return s; }

class SS_EXPORT Node : public Base
{
public:
	SS_CLASS(Node, "Node", Base)

	Node();
	virtual ~Node();

	SS_GETSET(FileInfo*, File, m_file);
	const String& GetFilename() const;

	SS_GETSET(int, Line, m_line);

	bool HasName() const { return m_name.size() > 0; }

	SS_GETSET(const String&, Name, m_name);

	// Gets the fully qualified name of this object.
	// TODO: Could probably use some testing
	virtual String GetFullName() const
	{
		String s;
		if(m_parent != 0 && m_parent->HasName())
			s += m_parent->GetFullName() + ".";
		s += GetName();
		return s;
	}
	
	// e.g. "package", "class", etc. 
	// Sort of confusing name
	virtual String GetTypeName() const { return "(unknown type)"; }

	virtual String GetDesc() const { return GetTypeName() + " '" + GetFullName() +"'"; }

	SS_GETSET(Node*, Parent, m_parent);
	Scope* GetScope() const;
	SS_GETSET(const AccessDomain&, AccessDomain, m_accessDomain);

private:
	FileInfo*		m_file;				// File object this node is from
	int				m_line;				// Line this node is from
	String			m_name;				// Name of this node (empty if no name)
	Node*			m_parent;			// Parent node (used for various purposes)
										// NOTE: Currently this is NOT necessarily the direct parent node!(?)
	mutable Scope*	m_scope;			// Scope object for this node. May not always be used.
	AccessDomain	m_accessDomain;		// Access domain for this node.
};

}

#endif // SS_LOCATABLE_H
