#include "Type.h"

namespace SS {


Type* Type::GetBasicType(BasicTypeId id)
{
	static BasicType voidType("void", BT_VOID);
	static BasicType byteType("byte", BT_BYTE);
	static BasicType sbyteType("sbyte", BT_SBYTE);
	static BasicType ushortType("ushort", BT_USHORT);
	static BasicType shortType("short", BT_SHORT);
	static BasicType uintType("uint", BT_UINT);
	static BasicType intType("int", BT_INT);
	static BasicType ulongType("ulong", BT_ULONG);
	static BasicType longType("long", BT_LONG);
	static BasicType floatType("float", BT_FLOAT);
	static BasicType doubleType("double", BT_DOUBLE);
	static BasicType stringType("string", BT_STRING);
	static BasicType objectType("object", BT_OBJECT);

	switch(id)
	{
	case BT_VOID: return &voidType;
	case BT_BYTE: return &byteType;
	case BT_SBYTE: return &sbyteType;
	case BT_USHORT: return &ushortType;
	case BT_SHORT: return &shortType;
	case BT_UINT: return &uintType;
	case BT_INT: return &intType;
	case BT_ULONG: return &ulongType;
	case BT_LONG: return &longType;
	case BT_FLOAT: return &floatType;
	case BT_DOUBLE: return &doubleType;
	case BT_STRING: return &stringType;
	case BT_OBJECT: return &objectType;
	default:
		SSAssertM(0, "Unknown BasicTypeId");
		return 0;
	}
}

}
