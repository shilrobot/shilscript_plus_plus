#ifndef SS_TYPE_H
#define SS_TYPE_H

#include "TypeInfo.h"
#include <sstream>

namespace SS {

class Type
{
public:

	enum TypeFlags
	{
		TF_VALID_RETURN_VALUE		= 0x01,	// Can be used as a retval
		TF_VALID_COMPOUND_MEMBER	= 0x02,	// Can be used as a compound member
		TS_VALID_PARAMETER			= 0x04, // Can be used as a parameter
		TF_REFERENCE_TYPE			= 0x08,	// Is a reference type (opposite is value type)
		TF_CONCRETE					= 0x10,	// Can be instantiated (for classes)
	};

	Type() : m_size(0), m_align(0), m_valueType(true) {}
	virtual ~Type() {}
	bool IsValue() const { return m_valueType; }
	bool IsReference() const { return !m_valueType; }
	size_t GetSize() const { return m_size; }
	size_t GetAlignment() const { return m_align; }
	size_t GetStackSlots() const { return (m_size + 3) >> 2; }
	virtual std::string GetName() const=0;
	virtual bool IsVoid() { return false; }

protected:
	bool	m_valueType;
	size_t m_size;
	size_t m_align;
};

// The type of various atomic types.
class AtomicType : public Type
{
public:
	AtomicType(size_t size, size_t align, const std::string& name) :
	  m_name(name)
	{
		m_size = size;
		m_align = align;
		m_valueType = true;
	}

	std::string GetName() const { return m_name; }

private:
	int				m_typeID;
	std::string		m_name;
};

// The type 'void'
class VoidType : public Type
{
public:
	VoidType()
	{
		m_size = 0;
		m_align = 1;
		m_valueType = true;
	}

	std::string GetName() const { return "void"; }

	bool IsVoid() { return true; }
};


class TypeMgr
{
public:
	static Type* GetVoid() { return &m_void; }
	static Type* GetUInt8() { return &m_uint8; }
	static Type* GetInt8() { return &m_int8; }
	static Type* GetUInt16() { return &m_uint16; }
	static Type* GetInt16() { return &m_int16; }
	static Type* GetUInt32() { return &m_uint32; }
	static Type* GetInt32() { return &m_int32; }
	static Type* GetUInt64() { return &m_uint64; }
	static Type* GetInt64() { return &m_int64; }
	static Type* GetFloat() { return &m_float; }
	static Type* GetDouble() { return &m_double; }
	static Type* GetArray(Type* elementType);

private:
	static VoidType		m_void;
	static AtomicType	m_uint8;
	static AtomicType	m_int8;
	static AtomicType	m_uint16;
	static AtomicType	m_int16;
	static AtomicType	m_uint32;
	static AtomicType	m_int32;
	static AtomicType	m_uint64;
	static AtomicType	m_int64;
	static AtomicType	m_float;
	static AtomicType	m_double;
};

struct FuncTypeParam
{
	Type*	type;
	// More stuff may show up here in the future.
};

typedef std::vector<FuncTypeParam> FuncTypeParamVector;

// The type of function references.
class FuncType : public Type
{
public:
	FuncType() : m_returnType(TypeMgr::GetVoid())
	{
		m_size = sizeof(void*);
		m_align = sizeof(void*);
		m_valueType = true;
	}

	void SetReturnType(Type* type) { m_returnType = type; }
	Type* GetReturnType() const { return m_returnType; }

	void AddParam(Type* type)
	{
		FuncTypeParam param;
		param.type = type;
		m_params.push_back(param);
	}

	size_t GetParamCount() const
	{
		return m_params.size();
	}

	Type* GetParamType(int n) const
	{
		return m_params[n].type;
	}

	std::string GetName() const
	{
		std::stringstream tmp;

		tmp << m_returnType->GetName() << " (";

		for(size_t n=0; n<m_params.size(); ++n)
		{
			if(n != 0)
				tmp << ", ";

			tmp << m_params[n].type->GetName();
		}

		tmp << ")";

		return tmp.str();
	}

private:
	Type*					m_returnType;
	FuncTypeParamVector		m_params;
};

struct CompoundMember
{
	Type* type;
};

typedef std::vector<CompoundMember> CompoundMemberVector;

class CompoundType
{
public:
	
private:
	CompoundMemberVector	m_members;
};

}

#endif // SS_TYPE_H
