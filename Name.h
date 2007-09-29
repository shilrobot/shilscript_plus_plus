#ifndef SS_NAME_H
#define SS_NAME_H

#include "Prereqs.h"

namespace SS {

bool IsValidName(const String& name);
bool NamesEqual(const String& a, const String& b);
void ToLowerInplace(String& inout);
String ToLower(const String& in);

}

#endif // SS_NAME_H
