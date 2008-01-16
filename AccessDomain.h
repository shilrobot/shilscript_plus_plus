#ifndef SS_ACCESSDOMAIN_H
#define SS_ACCESSDOMAIN_H

#include "Prereqs.h"

namespace SS {

class Node;

// Accessibility domains come in 3 flavors:
// 1) Universal access (this happens if something is public, or a public member of something else that's public, etc.)
// 2) Access lexically inside some node (this happens if something is private, or public inside something else that is private)
// 3) Access lexically inside of some class, and also in all derived classes
//	  (if something is protected, or public inside something protected, etc.)
// This only encodes the domain itself, not the public/private/on what/etc. data.
class SS_EXPORT AccessDomain
{
public:
	AccessDomain(Node* node = 0, bool derivedClasses = false) : m_node(node), m_derivedClasses(derivedClasses) {}

	// Creates a new access domain representing everywhere (a universal domain.)
	static AccessDomain Universal()
	{
		return AccessDomain(0, false);
	}

	// Create a new access domain representing a lexical domain: things physically
	// within a certain block in a file.
	static AccessDomain Lexical(Node* n)
	{
		return AccessDomain(n, false);
	}

	// Creates a new access domain representing the area contained by a
	// class and all its subclasses.
	static AccessDomain ClassAndSubclasses(Node* n)
	{
		return AccessDomain(n, true);
	}

	// Checks if an acces domain contains another domain.
	bool Contains(Node* ctx) const;
private:
	Node*	m_node;
	bool	m_derivedClasses;
};

bool IsAccessible(Node* n, Node* context);

}

#endif // SS_ACCESSDOMAIN_H
