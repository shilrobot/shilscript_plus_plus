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

		printf("(A) %s -> %s versus (B) %s -> %s\n",
				src->GetName().c_str(), typeA->GetName().c_str(),
				src->GetName().c_str(), typeB->GetName().c_str());

		ConversionComparison cc = CompareConversions(src, typeA, typeB);

		if(cc == CC_A_BETTER)
			printf("    A better\n");
		else if(cc == CC_B_BETTER)
			printf("    B better\n");
		else
			printf("    neither better\n");

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

static String OverloadToString( const OverloadCandidate* cand )
{
	String buf = "(";
	for(size_t k = 0; k < cand->types.size(); ++k)
	{
		if(k != 0)
			buf += ", ";
		buf += cand->types[k]->GetName();
	}
	buf += ")";
	return buf;
}

// TODO: We'll has to raise specific "cannot convert type" errors if we don't match any!
extern OverloadResolveResult ResolveOverloads(const TypeVector& types,
												const OverloadVector& candidates,
												OverloadVector& result)
{
	OverloadVector matchingSigs;

	std::string buf;
	for(size_t n = 0; n < types.size(); ++n)
	{
		if(n != 0)
			buf += ", ";
		buf += types[n]->GetName();
	}
	//printf("Resolving overload for (%s)\n", buf.c_str());

	//for(size_t n = 0; n < candidates.size(); ++n)
	//	printf("Candidate %d: %s\n", n, OverloadToString(candidates[n]).c_str());
	

	// First, cull out any overloads that can't possibly match the signature of the function.
	// Also, if we run into an exact match, return that!
	for(size_t n = 0; n < candidates.size(); ++n)
	{
		if(OverloadExactlyMatchesTypes(types, candidates[n]))
		{
			//printf("Candidate %d exactly matches\n", n);
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
		//printf("Got just one left, we have a winner\n");
		return OR_SINGLE_MATCH;
	}

	// If none left, we have no matches!
	if(matchingSigs.size() == 0)
	{
		//printf("None left, we fail!\n");
		return OR_NO_MATCH;
	}
	
	//for(size_t n = 0; n < matchingSigs.size(); ++n)
	//	printf("Matching sig %d: %s\n", n, OverloadToString(matchingSigs[n]).c_str());

	OverloadVector betterThanOthers;

	// lol O(n^2)
	for(size_t i = 0; i < matchingSigs.size(); ++i)
	{
		bool betterThanAllOthers = true;

		for(size_t j = 0; j < matchingSigs.size(); ++j)
		{
			if(i == j)
				continue;

			if(OverloadBetter(types, matchingSigs[i], matchingSigs[j]))
			{
				/*
				// TODO: More efficient way
				bool found=false;
				for(size_t k=0; k < betterThanOthers.size(); ++k)
					if(betterThanOthers[k] == matchingSigs[i])
					{
						found = true;
						break;
					}

				if(!found)
					betterThanOthers.push_back(matchingSigs[i]);
				*/
				/*printf("%s > %s\n",
						OverloadToString(matchingSigs[i]).c_str(),
						OverloadToString(matchingSigs[j]).c_str());*/
			}
			else
			{			
				/*printf("%s <= %s\n",
						OverloadToString(matchingSigs[i]).c_str(),
						OverloadToString(matchingSigs[j]).c_str());*/
				betterThanAllOthers = false;
				break;
			}
		}

		if(betterThanAllOthers)
			betterThanOthers.push_back(matchingSigs[i]);
	}

	
	/*for(size_t n = 0; n < betterThanOthers.size(); ++n)
		printf("BetterThanOthers %d: %s\n", n, OverloadToString(betterThanOthers[n]).c_str());
	*/

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
