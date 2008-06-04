#include "Prereqs.h"
#include "ParseLiteral.h"

namespace SS {

i4 ParseDecimal(const String& str)
{
	i4 val = 0;
	for(size_t n=0; n<str.size(); ++n)
	{
		i4 digit = str[n]-'0';
		i4 newVal = val*10 + digit;

		// TODO: Report error
		/*
		if(newVal < val)
		{
			// TO
		}
		*/

		val = newVal;
	}

	return val;
}

i4 ParseHex(const String& str)
{
	return 0;
}

i4 ParseOctal(const String& str)
{
	return 0;
}

}
