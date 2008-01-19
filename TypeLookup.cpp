#include "TypeLookup.h"
#include "Type.h"
#include "TypeExpr.h"
#include "NameLookup.h"
#include "Class.h"

namespace SS {

Type* TypeLookup::LookupType(TypeExpr* typeExpr, Node* context)
{
	// Handle basic type expressions
	if(typeExpr->IsA<BasicTypeExpr>())
	{
		BasicTypeExpr* btExpr = dynamic_cast<BasicTypeExpr*>(typeExpr);
		return Type::GetBasicType(btExpr->GetTypeId());
	}

	Node* n = LookupTypeImpl(typeExpr, context);

	if(n == 0)
		// Err. message should already be reported
		return 0;
	else if(n->IsA<Class>())
	{
		Class* cls = dynamic_cast<Class*>(n);
		return cls->GetType();
	}
	else
	{
		ReportError(typeExpr, "Expected a type, got %s", n->GetDesc().c_str());
		return 0;
	}
}

Node* TypeLookup::LookupTypeImpl(TypeExpr* typeExpr, Node* context)
{
	if(typeExpr->IsA<NameTypeExpr>())
	{
		NameTypeExpr* ntExpr = dynamic_cast<NameTypeExpr*>(typeExpr);
		return NameLookup::LookupName(ntExpr->GetName(), context, ntExpr);
	}
	else if(typeExpr->IsA<DottedTypeExpr>())
	{
		DottedTypeExpr* dtExpr = dynamic_cast<DottedTypeExpr*>(typeExpr);
		SSAssert(dtExpr->GetRight()->IsA<NameTypeExpr>());
		NameTypeExpr* ntExpr = dynamic_cast<NameTypeExpr*>(dtExpr->GetRight());

		Node* left = LookupTypeImpl(dtExpr->GetLeft(), context);
		if(!left)
			return 0;
		return NameLookup::LookupMember(ntExpr->GetName(), left, context, ntExpr);
	}
	else
	{
		SS_UNREACHABLE;
		return 0;
	}
}

}
