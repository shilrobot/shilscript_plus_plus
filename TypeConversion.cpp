#include "TypeConversion.h"
#include "Type.h"
#include "ClassType.h"
#include "Class.h"

namespace SS {


ConversionType GetConversionType(const Type* const from, const Type* const to)
{
	SSAssert(from != 0);
	SSAssert(to != 0);

	// Conversion from a type to itself is always legal, and is even better than an implicit conversion.
	if(from == to)
		return CONV_IDENTICAL;

	// TODO: char stuff

	// Cannot convert anything except void from or to void
	if(from == SS_T_VOID || to == SS_T_VOID)
		return CONV_ILLEGAL;

	// Everything boxes to object
	if(to == SS_T_OBJECT)
		return CONV_IMPLICIT;

	if(from == SS_T_BOOL)
	{
		// bool converts explicitly to all numerical types
		if(to == SS_T_U1 ||
			to == SS_T_I1 ||
			to == SS_T_U2 ||
			to == SS_T_I2 ||
			to == SS_T_U4 ||
			to == SS_T_I4 ||
			to == SS_T_U8 ||
			to == SS_T_I8 ||
			to == SS_T_F4 ||
			to == SS_T_F8)
			return CONV_EXPLICIT;
		else
			return CONV_ILLEGAL;
	}
	else if(from == SS_T_U1)
	{
		if(to == SS_T_U2 ||
			to == SS_T_I2 ||
			to == SS_T_U4 ||
			to == SS_T_I4 ||
			to == SS_T_U8 ||
			to == SS_T_I8 ||
			to == SS_T_F4 ||
			to == SS_T_F8)
			return CONV_IMPLICIT;
		else if(to == SS_T_BOOL ||
			to == SS_T_I1)
			return CONV_EXPLICIT;
		else
			return CONV_ILLEGAL;
	}
	else if(from == SS_T_I1)
	{
		if(to == SS_T_I2 ||
			to == SS_T_I4 ||
			to == SS_T_I8 ||
			to == SS_T_F4 ||
			to == SS_T_F8)
			return CONV_IMPLICIT;
		else if(to == SS_T_BOOL ||
			to == SS_T_U1 ||
			to == SS_T_U2 ||
			to == SS_T_U4 ||
			to == SS_T_U8)
			return CONV_EXPLICIT;
		else
			return CONV_ILLEGAL;
	}
	else if(from == SS_T_U2)
	{
		if(to == SS_T_U4 ||
			to == SS_T_I4 ||
			to == SS_T_U8 ||
			to == SS_T_I8 ||
			to == SS_T_F4 ||
			to == SS_T_F8)
			return CONV_IMPLICIT;
		else if(to == SS_T_BOOL ||
			to == SS_T_U1 ||
			to == SS_T_I1 ||
			to == SS_T_I2)
			return CONV_EXPLICIT;
		else
			return CONV_ILLEGAL;
	}
	else if(from == SS_T_I2)
	{
		if(to == SS_T_I4 ||
			to == SS_T_I8 ||
			to == SS_T_F4 ||
			to == SS_T_F8)
			return CONV_IMPLICIT;
		else if(to == SS_T_BOOL ||
			to == SS_T_U1 ||
			to == SS_T_I1 ||
			to == SS_T_U2 ||
			to == SS_T_U4 ||
			to == SS_T_U8)
			return CONV_EXPLICIT;
		else
			return CONV_ILLEGAL;
	}
	else if(from == SS_T_U4)
	{
		if(to == SS_T_U8 ||
			to == SS_T_I8 ||
			to == SS_T_F4 ||
			to == SS_T_F8)
			return CONV_IMPLICIT;
		else if(to == SS_T_BOOL ||
			to == SS_T_U1 ||
			to == SS_T_I1 ||
			to == SS_T_U2 ||
			to == SS_T_I2 ||
			to == SS_T_I4)
			return CONV_EXPLICIT;
		else
			return CONV_ILLEGAL;
	}
	else if(from == SS_T_I4)
	{
		if(to == SS_T_I8 ||
			to == SS_T_F4 ||
			to == SS_T_F8)
			return CONV_IMPLICIT;
		else if(to == SS_T_BOOL ||
			to == SS_T_U1 ||
			to == SS_T_I1 ||
			to == SS_T_U2 ||
			to == SS_T_I2 ||
			to == SS_T_U4 ||
			to == SS_T_U8)
			return CONV_EXPLICIT;
		else
			return CONV_ILLEGAL;
	}
	else if(from == SS_T_U8)
	{
		if(to == SS_T_F4 ||
			to == SS_T_F8)
			return CONV_IMPLICIT;
		else if(to == SS_T_BOOL ||
			to == SS_T_U1 ||
			to == SS_T_I1 ||
			to == SS_T_U2 ||
			to == SS_T_I2 ||
			to == SS_T_U4 ||
			to == SS_T_I4 ||
			to == SS_T_I8)
			return CONV_EXPLICIT;
		else
			return CONV_ILLEGAL;
	}
	else if(from == SS_T_I8)
	{
		if(to == SS_T_F4 ||
			to == SS_T_F8)
			return CONV_IMPLICIT;
		else if(to == SS_T_BOOL ||
			to == SS_T_U1 ||
			to == SS_T_I1 ||
			to == SS_T_U2 ||
			to == SS_T_I2 ||
			to == SS_T_U4 ||
			to == SS_T_I4 ||
			to == SS_T_U8)
			return CONV_EXPLICIT;
		else
			return CONV_ILLEGAL;
	}
	else if(from == SS_T_F4)
	{
		if(to == SS_T_F8)
			return CONV_IMPLICIT;
		else if(to == SS_T_BOOL ||
			to == SS_T_U1 ||
			to == SS_T_I1 ||
			to == SS_T_U2 ||
			to == SS_T_I2 ||
			to == SS_T_U4 ||
			to == SS_T_I4 ||
			to == SS_T_U8 ||
			to == SS_T_I8)
			return CONV_EXPLICIT;
		else
			return CONV_ILLEGAL;
	}
	else if(from == SS_T_F8)
	{
		if(to == SS_T_BOOL ||
			to == SS_T_U1 ||
			to == SS_T_I1 ||
			to == SS_T_U2 ||
			to == SS_T_I2 ||
			to == SS_T_U4 ||
			to == SS_T_I4 ||
			to == SS_T_U8 ||
			to == SS_T_I8 ||
			to == SS_T_F4)
			return CONV_EXPLICIT;
		else
			return CONV_ILLEGAL;
	}
	else if(from == SS_T_CHAR)
	{
		// TODO
		return CONV_ILLEGAL;
	}
	else if(from == SS_T_STRING)
	{
		return CONV_ILLEGAL;
	}
	else if(from == SS_T_OBJECT)
	{
		// You can pretty much try to cast object to everything.
		// (void is already ruled out above)
		return CONV_EXPLICIT;
	}
	else if(from == SS_T_NULLTYPE)
	{
		if(to->IsA<ClassType>())
			return CONV_IMPLICIT;
		else
			return CONV_ILLEGAL;
	}
	else if(from->IsA<ClassType>())
	{
		if(to->IsA<ClassType>())
		{
			const ClassType* const fromClassType = dynamic_cast<const ClassType*>(from);
			const ClassType* const toClassType = dynamic_cast<const ClassType*>(to);
			const Class* const fromClass = fromClassType->GetClass();
			const Class* const toClass = toClassType->GetClass();

			// If 'to' is a parent of 'from', then conversion is implicit
			Class* curr = fromClass->GetBase();
			while(curr != 0)
			{
				if(curr == toClass)
					return CONV_IMPLICIT;
				else
					curr = curr->GetBase();
			}

			// If 'to' is a child of 'from', then the conversion is explicit
			curr = toClass->GetBase();
			while(curr != 0)
			{
				if(curr == fromClass)
					return CONV_EXPLICIT;
				else
					curr = curr->GetBase();
			}

			// Otherwise, classes are unrelated and cannot possibly be converted
			return CONV_ILLEGAL;
		}
		else
			return CONV_ILLEGAL;
	}

	return CONV_ILLEGAL;
}

extern bool CanExplicitlyConvert(const Type* const from, const Type* const to)
{
	ConversionType convType = GetConversionType(from, to);
	return convType == CONV_IDENTICAL || convType == CONV_IMPLICIT || convType == CONV_EXPLICIT;
}

extern bool CanImplicitlyConvert(const Type* const from, const Type* const to)
{
	ConversionType convType = GetConversionType(from, to);
	return convType == CONV_IDENTICAL || convType == CONV_IMPLICIT;
}

extern ConversionComparison CompareConversions(const Type* const src, const Type* const a, const Type* const b)
{
	SSAssert(a != 0);
	SSAssert(b != 0);

	// This is modeled after C#'s conversion comparisons

	if(a == b)
		return CC_NEITHER_BETTER;

	if(src == a)
		return CC_A_BETTER;

	if(src == b)
		return CC_B_BETTER;

	if(CanImplicitlyConvert(a,b) && !CanImplicitlyConvert(b,a))
		return CC_A_BETTER;

	if(CanImplicitlyConvert(b,a) && !CanImplicitlyConvert(a,b))
		return CC_B_BETTER;

	if(a == SS_T_I1 && (b == SS_T_U1 || b == SS_T_U2 || b == SS_T_U4 || b == SS_T_U8))
		return CC_A_BETTER;

	if(b == SS_T_I1 && (a == SS_T_U1 || a == SS_T_U2 || a == SS_T_U4 || a == SS_T_U8))
		return CC_B_BETTER;

	if(a == SS_T_I2 && (b == SS_T_U2 || b == SS_T_U4 || b == SS_T_U8))
		return CC_A_BETTER;

	if(b == SS_T_I2 && (a == SS_T_U2 || a == SS_T_U4 || a == SS_T_U8))
		return CC_B_BETTER;

	if(a == SS_T_I4 && (b == SS_T_U4 || b == SS_T_U8))
		return CC_A_BETTER;

	if(b == SS_T_I4 && (a == SS_T_U4 || a == SS_T_U8))
		return CC_B_BETTER;

	if(a == SS_T_I8 && b == SS_T_U8)
		return CC_A_BETTER;

	if(b == SS_T_I8 && a == SS_T_U8)
		return CC_B_BETTER;

	return CC_NEITHER_BETTER;
}

}
