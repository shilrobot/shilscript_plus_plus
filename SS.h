#ifndef SS_h
#define SS_h

#include "Prereqs.h"
#include "TypeInfo.h"

namespace SS {

#define SS_CLASS(name, base) \
	virtual SS::TypeInfo* DynamicType() const { return StaticType(); } \
	static SS::TypeInfo* StaticType() { static SS::TypeInfo typeinfo(name, TypeOf<base>()); return &typeinfo; }

class SS_EXPORT Base
{
public:
	Base() {}
	virtual ~Base() {}

	virtual TypeInfo* DynamicType() const { return StaticType(); }

	static TypeInfo* StaticType()
	{
		static TypeInfo typeinfo("Base", 0);
		return &typeinfo;
	}

	bool IsA(TypeInfo* type) const
	{
		if(!type)
			return false;

		TypeInfo* currBase = TypeOf(this);

		// Search all our base classes for this class
		while(currBase)
		{
			if(currBase == type)
				return true;
			else
				currBase = currBase->GetBase();
		}

		return false;
	}

	template<class T>
	bool IsA() const
	{
		return IsA(TypeOf<T>());
	}

	bool IsExactlyA(TypeInfo* type) const
	{
		if(!type)
			return false;

		return TypeOf(this) == type;
	}

	template<class T>
	bool IsExactlyA() const
	{
		return IsExactlyA(TypeOf<T>());
	}
};

#if 0

class SS_EXPORT Base
{
public:

	// Location
	// type info
	// flags(?)
	// read/write?
	// Owner chain? (find first owner of a certain type)

	int GetLine();

	void Error(const char* format, ...);
	void Warn(const char* format, ...);

	//VM* GetVM() const;

	/*virtual TypeInfo* DynamicType() const;
	static TypeInfo* StaticType();*/

	bool IsA(TypeInfo* type) const;

	template<class T>
	bool IsA() const;

	bool IsExactlyA(TypeInfo* type) const;

	template<class T>
	bool IsExactlyA() const;


};

class SS_EXPORT Package : public Base
{
public:

	// Name
	// member list (pending)
	// dependency names
	// dependency packages
	// -- classes, structs, interfaces, functions, variables (constants/statics), typedefs, delegates, enums
	// member list (final - incl. map)
};

class SS_EXPORT Type
{
public:

	// Name
	// Parent
	// Dotted name (full name)
	// Sub-stuff: classes, structs, interfaces, functions, variables, typedefs, delegates, enums
	// (pending and agreed upon)
	// base type (if any)
};

#endif

#if 0
class SS_EXPORT PackageManager
{
public:

	Package* LoadPackage(InStream& strm);
	Package* CompilePackage(/* files, deps, packagename, out error messages */);

private:
	
	PackageManager();
	~PackageManager();
};
#endif

}

#endif // SS_h

