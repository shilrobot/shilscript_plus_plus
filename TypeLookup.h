#ifndef SS_TYPELOOKUP_H
#define SS_TYPELOOKUP_H

#include "Prereqs.h"

namespace SS {

class Node;
class Type;
class TypeExpr;

class TypeLookup
{
public:
	static Type* LookupType(TypeExpr* typeExpr, Node* context);

private:
	static Node* LookupTypeImpl(TypeExpr* typeExpr, Node* context);
};

}


#endif // SS_TYPELOOKUP_H
