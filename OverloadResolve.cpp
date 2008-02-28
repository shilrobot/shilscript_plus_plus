#include "OverloadResolve.h"
#include "Type.h"
#include "TypeConversion.h"

namespace SS {

bool OverloadMatchesTypes(const TypeVector& types, const OverloadCandidate* overload)
{
	if(types.size() != overload->types.size())
		return false;
	
	for(size_t n = 0; n < types.size(); ++n)
	{
		const Type* origType = types[n];
		const Type* overloadType = overload->types[n];
		SSAssert(origType != 0);
		SSAssert(overloadType != 0);
		if(origType == overloadType)
			continue;
		else if(CanImplicitlyConvert(origType, overloadType))
			continue;
		else
			return false;
	}

	return true;
}

bool OverloadExactlyMatchesTypes(const TypeVector& types, const OverloadCandidate* overload)
{
	if(types.size() != overload->types.size())
		return false;
	
	for(size_t n = 0; n < types.size(); ++n)
	{
		const Type* origType = types[n];
		const Type* overloadType = overload->types[n];
		SSAssert(origType != 0);
		SSAssert(overloadType != 0);
		if(origType == overloadType)
			continue;
		else
			return false;
	}

	return true;
}

// Check if a is better than b
bool OverloadBetter(const TypeVector& types, const OverloadCandidate* a, const OverloadCandidate* b)
{
	SSAssert(types.size() == a->types.size());
	SSAssert(a->types.size() == b->types.size());

	int numBetter = 0;

	for(size_t n = 0; n < a->types.size(); ++n)
	{
		const Type* src = types[n];
		const Type* typeA = a->types[n];
		const Type* typeB = b->types[n];

		ConversionComparison cc = CompareConversions(src, typeA, typeB);

		if(cc == CC_A_BETTER)
			++numBetter;
		else if(cc == CC_B_BETTER)
			return false;
		else if(cc == CC_NEITHER_BETTER)
			continue;
	}

	if(numBetter >= 1)
		return true;
	else
		return false;
}

// TODO: We'll has to raise specific "cannot convert type" errors if we don't match any!
extern OverloadResolveResult ResolveOverloads(const TypeVector& types,
												const OverloadVector& candidates,
												OverloadVector& result)
{
	OverloadVector matchingSigs;

	// First, cull out any overloads that can't possibly match the signature of the function.
	// Also, if we run into an exact match, return that!
	for(size_t n = 0; n < candidates.size(); ++n)
	{
		if(OverloadExactlyMatchesTypes(types, candidates[n]))
		{
			result.push_back(candidates[n]);
			return OR_SINGLE_MATCH;
		}

		if(OverloadMatchesTypes(types, candidates[n]))
			matchingSigs.push_back(candidates[n]);
	}

	// If there's exactly one left, we have a winner
	if(matchingSigs.size() == 1)
	{
		result.push_back(matchingSigs[0]);
		return OR_SINGLE_MATCH;
	}

	// If none left, we have no matches!
	if(matchingSigs.size() == 0)
		return OR_NO_MATCH;

	OverloadVector betterThanOthers;

	// lol O(n^2)
	for(size_t i = 0; i < matchingSigs.size(); ++i)
	{
		for(size_t j = 0; j < matchingSigs.size(); ++j)
		{
			if(i == j)
				continue;

			if(OverloadBetter(types, candidates[i], candidates[j]))
				betterThanOthers.push_back(candidates[i]);
		}
	}

	// No best overloads?
	if(betterThanOthers.size() == 0)
	{
		result.insert(result.end(), matchingSigs.begin(), matchingSigs.end());
		return OR_AMBIGUOUS;
	}

	// Exactly one best
	if(betterThanOthers.size() == 1)
	{
		result.push_back(betterThanOthers[0]);
		return OR_SINGLE_MATCH;
	}

	// Many "best" (dunno if this can really happen!)
	result.insert(result.end(), betterThanOthers.begin(), betterThanOthers.end());
	return OR_AMBIGUOUS;
}

}
