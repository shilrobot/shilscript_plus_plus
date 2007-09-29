#ifndef SS_NAMELOOKUP_H
#define SS_NAMELOOKUP_H

#include "Prereqs.h"
#include "AccessDomain.h"

namespace SS {

class Node;

class ILookupVisitor
{
public:
	virtual void OnMatch(Node* n) = 0;
	virtual bool OnGroupEnd() = 0;
};

class LookupVisitor : public ILookupVisitor
{
public:

	LookupVisitor(const String& name, Node* context, Node* errSource) :
		m_name(name), m_context(context), m_result(0), m_resultCode(RESULT_NOT_FOUND), m_errSource(errSource) {}

	void OnMatch(Node* n)
	{
		m_nodes.push_back(n);
	}

	bool OnGroupEnd();

	Node* Complete();

private:

	enum LookupResult
	{
		RESULT_FOUND,
		RESULT_NOT_FOUND,
		RESULT_AMBIGUOUS,
		RESULT_INACCESSIBLE
	};

	String			m_name;
	LookupResult	m_resultCode;
	Node*			m_result;
	Node*			m_context;
	Node*			m_errSource;
	std::vector<Node*> m_nodes;
	std::vector<Node*> m_inaccessible;
};

// TODO: Name suggestions? (edit distance)
// TODO: For really good errors, we want to tell exactly what nameexpr failed (node position)

class NameLookup
{
public:

	// Context is the node whose "body" the name reference takes place in,
	// e.g. when looking up the type of a member variable, the class is the context.
	static Node* LookupName(const String& name, Node* context, Node* errSource);
	static Node* LookupMember(const String& name, Node* parent, Node* context, Node* errSource);

private:
	static void LookupName(const String& name, Node* currObj, ILookupVisitor* visitor);
	static void LookupMember(const String& name, Node* parent, ILookupVisitor* visitor);

	static bool IsAccessible(Node* n, Node* context);
	static void GetAccessDomain(Node* n, AccessDomain& domain);
};


}

#endif // SS_NAMELOOKUP_H
