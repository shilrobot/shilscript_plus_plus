#ifndef SS_TYPECONVERSION_H
#define SS_TYPECONVERSION_H

#include "Prereqs.h"

namespace SS {

class Type;

enum ConversionType
{
	CONV_IDENTICAL,
	CONV_IMPLICIT,
	CONV_EXPLICIT,
	CONV_ILLEGAL
};

enum ConversionComparison
{
	CC_A_BETTER,
	CC_B_BETTER,
	CC_NEITHER_BETTER
};

extern ConversionType GetConversionType(const Type* const from, const Type* const to);
extern bool CanExplicitlyConvert(const Type* const from, const Type* const to);
extern bool CanImplicitlyConvert(const Type* const from, const Type* const to);
extern ConversionComparison CompareConversions(const Type* const src, const Type* const a, const Type* const b);

}

#endif // SS_TYPECONVERSION_H
