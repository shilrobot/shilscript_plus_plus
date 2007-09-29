#include "Type.h"

namespace SS {


Type* Type::GetBasicType(BasicTypeId id)
{
	static BasicType voidType(BT_VOID);
	static BasicType byteType(BT_BYTE);
	static BasicType sbyteType(BT_SBYTE);
	static BasicType ushortType(BT_USHORT);
	static BasicType shortType(BT_SHORT);
	static BasicType uintType(BT_UINT);
	static BasicType intType(BT_INT);
	static BasicType ulongType(BT_ULONG);
	static BasicType longType(BT_LONG);
	static BasicType floatType(BT_FLOAT);
	static BasicType doubleType(BT_DOUBLE);
	static BasicType stringType(BT_STRING);
	static BasicType objectType(BT_OBJECT);

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
