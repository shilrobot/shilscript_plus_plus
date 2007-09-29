#ifndef SS_STATICCHECKER_H
#define SS_STATICCHECKER_H

#include "Prereqs.h"

namespace SS {

class Package;
class Class;
class Variable;
class Function;
class Node;

// Performs the static checking passes on the code tree.
class SS_EXPORT StaticChecker
{
public:
	StaticChecker(Package* pkg);
	bool Check();

private:
	// First phase: Checking classes' names/bases
	void ResolveClassBase(Class* cls);

	// Second phase: Process all classes' members (check variables/functions/etc.)
	void ProcessClassMembersRecursive(Class* cls);
	bool ProcessClassMembers(Class* cls);
	bool CheckVariable(Variable* var);
	bool CheckFunction(Function* func);
	void CheckFunctionBody(Function* func);

	// Third phase: Static checking of function contents
	// (TODO)

	bool CheckChild(Node* parent, Node* child);

	Package* m_pkg;
};

}

#endif // SS_STATICCHECKER_H
