#include "Prereqs.h"
#include "Type.h"

namespace SS {


Type* Type::GetBasicType(BasicTypeId id)
{
	static BasicType voidType(BT_VOID, "void");
	static BasicType boolType(BT_BOOL, "bool");
	static BasicType byteType(BT_U1, "byte");
	static BasicType sbyteType(BT_I1, "sbyte");
	static BasicType ushortType(BT_U2, "ushort");
	static BasicType shortType(BT_I2, "short");
	static BasicType uintType(BT_U4, "uint");
	static BasicType intType(BT_I4, "int");
	static BasicType ulongType(BT_U8, "ulong");
	static BasicType longType(BT_I8, "long");
	static BasicType floatType(BT_FLOAT, "float");
	static BasicType doubleType(BT_DOUBLE, "double");
	static BasicType charType(BT_CHAR, "char");
	static BasicType stringType(BT_STRING, "string");
	static BasicType objectType(BT_OBJECT, "object");

	switch(id)
	{
	case BT_VOID: return &voidType;
	case BT_BOOL: return &boolType;
	case BT_U1: return &byteType;
	case BT_I1: return &sbyteType;
	case BT_U2: return &ushortType;
	case BT_I2: return &shortType;
	case BT_U4: return &uintType;
	case BT_I4: return &intType;
	case BT_U8: return &ulongType;
	case BT_I8: return &longType;
	case BT_FLOAT: return &floatType;
	case BT_DOUBLE: return &doubleType;
	case BT_CHAR: return &charType;
	case BT_STRING: return &stringType;
	case BT_OBJECT: return &objectType;
	default:
		SSAssertM(0, "Unknown BasicTypeId");
		return 0;
	}
}

}
