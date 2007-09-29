#include "Name.h"

namespace SS {

bool IsValidName(const String& name)
{
	// TODO: Maybe check against keyword list also

	if(name.length() == 0)
		return false;

	if(name.length() > 255)
		return false;

	for(int i = 0; i < (int)name.length(); ++i)
	{
		char c = name[i];
		
		if(c >= 'A' && c <= 'Z')
			continue;
		else if(c >= 'a' && c <= 'z')
			continue;
		else if(c == '_')
			continue;
		else if(c >= '0' && c <=' 9' && i != 0)
			continue;
		else
			return false;
	}

	return true;
}

bool NamesEqual(const String& a, const String& b)
{
	if(a.length() != b.length())
		return false;

	for(int i=0; i < (int)a.length(); ++i)
	{
		if(tolower(a[i]) != tolower(b[i]))
			return false;
	}

	return true;
}

void ToLowerInplace(String& inout)
{
	std::transform(inout.begin(), inout.end(), inout.begin(), tolower);
}

String ToLower(const String& in)
{
	String tmp(in);
	ToLowerInplace(tmp);
	return tmp;
}

}
