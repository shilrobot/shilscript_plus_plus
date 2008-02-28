#ifndef SS_OVERLOADRESOLVE_H
#define SS_OVERLOADRESOLVE_H

#include "Prereqs.h"

namespace SS {

class Type;

typedef std::vector<const Type*> TypeVector;

struct OverloadCandidate
{
	void* userData;
	String name;
	TypeVector types;
};

enum OverloadResolveResult
{
	OR_NO_MATCH,
	OR_AMBIGUOUS,
	OR_SINGLE_MATCH
};

typedef std::vector<const OverloadCandidate*> OverloadVector;

// This function assumes that 'candidates' doesn't have any overloads with *duplicates* of the exact same signature!
extern OverloadResolveResult ResolveOverloads(const TypeVector& types,
												const OverloadVector& candidates,
												OverloadVector& result);

}

#endif // SS_OVERLOADRESOLVE_H
