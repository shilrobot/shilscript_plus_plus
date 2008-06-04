//------------------------------------------------------------------
// Prereqs.h
//
// Copyright (C) 2007 Scott Hilbert
//------------------------------------------------------------------
#ifndef SS_PREREQS_H
#define SS_PREREQS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>

#include <cassert>
#include <iostream>
#include <fstream>

#include <list>
#include <vector>
#include <queue>
#include <algorithm>
#include <string>
#include <map>
#include <sstream>

// TODO: OS detect, IA-64 detect maybe...

#if defined(__i386) || defined(_M_IX86)
	#define SS_ARCH_X86
	#define SS_ARCH_NAME "x86"
#elif defined(__ppc__) || defined(__POWERPC) || defined(_M_PPC)
	#define SS_ARCH_PPC
	#define SS_ARCH_NAME "PowerPC"
#else
	#error Unknown architecture
#endif

#if defined(_MSC_VER)
	#define SS_COMPILER_MSVC
	#define SS_COMPILER_NAME "MSVC"
#elif defined(__GNUC__)
	#define SS_COMPILER_GCC
	#define SS_COMPILER_NAME "GCC"
#else
	#error Unknown compiler
#endif

#if defined(SS_ARCH_X86)
	#define SS_LITTLE_ENDIAN
#elif defined(SS_ARCH_PPC)
	#define SS_BIG_ENDIAN
#endif

#if defined(SS_COMPILER_MSVC)
	#define SS_FORCEINLINE __forceinline
#elif defined(SS_COMPILER_GCC)
	#define SS_FORCEINLINE __attribute__((always_inline)) inline
#else
	#define SS_FORCEINLINE inline
#endif

// temp.
#define SS_BUILD

#if defined(SS_COMPILER_MSVC)
	#if defined(SS_BUILD)
		#define SS_EXPORT __declspec(dllexport)
	#else
		#define SS_IMPORT __declspec(dllimport)
	#endif
#else
	#define SS_EXPORT
	#define SS_IMPORT
#endif

#if !defined(SS_DEBUG) && defined(_DEBUG)
	#define SS_DEBUG
#endif

#ifdef SS_COMPILER_MSVC

#pragma warning(disable: 4251) // class XXX needs to have dll-interface to be used by clients of class YYY

#endif

namespace SS {

typedef char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;

#if defined(SS_COMPILER_MSVC)
	typedef __int64 int64;
	typedef unsigned __int64 uint64;
#elif defined(SS_COMPILER_GCC)
	typedef long long int64;
	typedef unsigned long long uint64;
#endif

typedef float float32;
typedef double float64;

typedef int32 ssize_t;

typedef int8 i1;
typedef uint8 u1;
typedef int16 i2;
typedef uint16 u2;
typedef int32 i4;
typedef uint32 u4;
typedef int64 i8;
typedef uint64 u8;
typedef float32 f4;
typedef float64 f8;

typedef std::string String;
typedef std::vector<String> StringVector;

}

/*
#define SSNew(x) (new x)
#define SSDelete(x) delete x
#define SSDeleteArray(x) delete[] x
*/

#define SSAssert(expr) assert(expr)
#define SSAssertM(expr,msg) assert(expr && msg)


#define SS_GET(_type, _name, _var) _type Get##_name() const { return _var; }
#define SS_SET(_type, _name, _var) void Set##_name(_type value) { _var = value; }

#define SS_GETSET(_type, _name, _var) \
			SS_GET(_type,_name,_var) \
			SS_SET(_type,_name,_var)

#define SS_GET_BOOL(_name, _var) bool _name() const { return _var; }
#define SS_SET_BOOL(_name, _var) void _name(bool value) { _var = value; }
#define SS_GETSET_BOOL(_get, _set, _var) \
	SS_GET_BOOL(_get, _var) \
	SS_SET_BOOL(_set, _var)

#define SS_ADD(_type, _name, _var) void _name(_type value) { _var.push_back(value); }

#define SS_INDEX(_type, _name, _var) \
	int Get##_name##Count() const { return (int)_var.size(); } \
	_type Get##_name(int i) const { return _var[i]; }

#define SS_UNREACHABLE { SSAssertM(0, "Unreachable code reached"); }
#define SS_UNIMPLEMENTED { SSAssertM(0, "Feature not yet implemented"); }

#endif // SS_PREREQS_H
