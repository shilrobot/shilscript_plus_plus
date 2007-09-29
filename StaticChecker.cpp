#include "StaticChecker.h"
#include "Package.h"
#include "Class.h"
#include "Variable.h"
#include "Function.h"
#include "TypeLookup.h"

namespace SS {

StaticChecker::StaticChecker(Package* pkg) : m_pkg(pkg)
{
	SSAssert(pkg != 0);
}

bool StaticChecker::Check()
{
	Package* pkg = m_pkg;

	// Check the package name
	if(!IsValidName(pkg->GetName()))
	{
		ReportError("Invalid package name: %s", SS_QUOTE(pkg->GetName()));
		return false;
	}

	// TODO: Load up other packages. 
	// Watch out for duplicate package names, package names aliasing/referencing current package,
	// infinite recursion (maybe not possible)

	// Check all classes for name conflicts, add them to member list if they pass.
	for(int i = 0; i < pkg->GetClassCount(); ++i)
	{
		Class* cls = pkg->GetClass(i);

		if(!CheckChild(pkg, cls))
			continue;

		if(cls->GetAccess() == ACCESS_PUBLIC)
			cls->SetAccessDomain(AccessDomain::Universal());
		else if(cls->GetAccess() == ACCESS_PRIVATE)
			cls->SetAccessDomain(AccessDomain::Lexical(pkg));
		else
			SS_UNREACHABLE;

		cls->SetParent(m_pkg);
		cls->SetPackage(m_pkg);
		pkg->GetScope()->Add(cls);
	}

	// Quit if we hit any errors.
	if(AnyErrors())
		return false;

	// Resolve all bases for classes.
	for(int i = 0; i < pkg->GetClassCount(); ++i)
		ResolveClassBase(pkg->GetClass(i));

	// Quit if we hit any errors.
	if(AnyErrors())
		return false;

	// At this point all classes have their bases set up properly,
	// and they're known not to have cycles.
	// Now we have to process all of the members of each class, starting with
	// the base classes and moving upwards...
	for(int i = 0; i < pkg->GetClassCount(); ++i)
		ProcessClassMembersRecursive(pkg->GetClass(i));

	if(AnyErrors())
		return false;

	// TODO: Somewhere in here we need to work out overloading, function overriding, etc.

	// Classes and their vars, functions, and inheritance have been worked out;
	// now we need to go through statements & expressions.
	for(int i = 0; i < pkg->GetClassCount(); ++i)
	{
		Class* cls = pkg->GetClass(i);
		for(int j=0; j < cls->GetFunctionCount(); ++j)
			CheckFunctionBody(cls->GetFunction(j));
	}

	if(AnyErrors())
		return false;

	return true;
}


void StaticChecker::ResolveClassBase(Class* cls)
{	
	// TODO: Why's this giving zero???

	if(cls->GetBaseExpr() == 0)
	{
		// TODO: Set object as base class if there's no base expr
		return;
	}

	// This will do error messages itself
	Type* typ = TypeLookup::LookupType(cls->GetBaseExpr(), m_pkg);
	if(!typ)
		return;
		
	// Check that it is a class.
	// TODO: Allow "object" as a base class!
	if(!typ->IsA<Class>())
	{
		ReportError(cls, "Base must be another class");
		return;
	}

	// Check that we are not setting up a cycle
	// (e.g., make sure that cls is not in type's bases)
	Class* baseClass = dynamic_cast<Class*>(typ);
	Class* currBase = baseClass;
	while(currBase != 0)
	{
		if(currBase == cls)
		{
			ReportError(cls, "Inheritance cycle!");
			return;
		}

		currBase = currBase->GetBase();
	}

	cls->SetBase(baseClass);
}

void StaticChecker::ProcessClassMembersRecursive(Class* cls)
{
	SSAssert(cls != 0);
	
	// TODO: Don't want to process classes from a different package!
	SSAssert(cls->GetPackage() != 0);
	if(cls->GetPackage() != m_pkg)
		return;

	// If we're already processed, quit.
	if(cls->IsProcessed())
		return;

	// If we have a base class, make sure its members are processed first!
	if(cls->GetBase() != 0 && cls->GetBase()->GetPackage() == m_pkg)
	{
		ProcessClassMembersRecursive(cls->GetBase());

		// If processing it failed, quit.
		if(cls->GetBase()->Failed())
		{
			cls->SetFailed(true);
			return;
		}
	}

	if(!ProcessClassMembers(cls))
	{
		cls->SetFailed(true);
		cls->SetProcessed(true);
		return;
	}
	else
		cls->SetProcessed(true);
}

bool StaticChecker::ProcessClassMembers(Class* cls)
{
	bool errors = false;

	// Here, all global types are known, but the members of classes are not.
	// We need to go through & make sure they are all semantically OK,
	// as far as names, types, flags, etc.
	// Processing the init exprs. for vars and the bodies of functions is more complicated
	// and will come later.

	// TODO: Shadowing checks for stuff

	for(int i = 0; i < cls->GetVariableCount(); ++i)
	{
		Variable* var = cls->GetVariable(i);

		if(!CheckChild(cls, var))
		{
			errors = true;
			continue;
		}

		if(var->GetAccess() == ACCESS_PUBLIC)
			var->SetAccessDomain(cls->GetAccessDomain());
		else if(var->GetAccess() == ACCESS_PROTECTED)
			var->SetAccessDomain(AccessDomain::ClassAndSubclasses(cls));
		else if(var->GetAccess() == ACCESS_PRIVATE)
			var->SetAccessDomain(AccessDomain::Lexical(cls));
		else
			SS_UNREACHABLE;

		var->SetParent(cls);

		if(!CheckVariable(var))
		{
			errors = true;
			continue;
		}

		// TODO: We have to check against the base class's scope

		// Try to fit it into the current scope
		cls->GetScope()->Add(var);
	}

	for(int i = 0; i < cls->GetFunctionCount(); ++i)
	{
		Function* func = cls->GetFunction(i);

		if(!CheckChild(cls, func))
		{
			errors = true;
			continue;
		}

		if(func->GetAccess() == ACCESS_PUBLIC)
			func->SetAccessDomain(cls->GetAccessDomain());
		else if(func->GetAccess() == ACCESS_PROTECTED)
			func->SetAccessDomain(AccessDomain::ClassAndSubclasses(cls));
		else if(func->GetAccess() == ACCESS_PRIVATE)
			func->SetAccessDomain(AccessDomain::Lexical(cls));
		else
			SS_UNREACHABLE;

		func->SetParent(cls);

		if(!CheckFunction(func))
		{
			errors = true;
			continue;
		}

		// TODO: We have to check against the base class's scope

		cls->GetScope()->Add(func);
	}

	return !errors;
}

bool StaticChecker::CheckVariable(Variable* var)
{
	SSAssert(var->GetParent() != 0);
	SSAssert(var->GetTypeExpr() != 0);

	if(var->IsStatic() && var->IsConst())
	{
		ReportError(var, "Cannot combine const and static flags");
		return false;
	}

	// This prints its own errors
	Type* typ = TypeLookup::LookupType(var->GetTypeExpr(), var->GetParent());
	if(!typ)
		return false;

	
	if(typ == Type::GetBasicType(BT_VOID))
	{
		ReportError(var, "Void cannot be used for variable types");
		return false;
	}

	var->SetType(typ);


	return true;
}

bool StaticChecker::CheckFunction(Function* func)
{
	SSAssert(func->GetParent() != 0);
	SSAssert(func->GetReturnTypeExpr() != 0);

	Type* returnType = TypeLookup::LookupType(func->GetReturnTypeExpr(), func->GetParent());
	if(!returnType)
		return false;
	func->SetReturnType(returnType);

	if(func->IsStatic() && func->IsSealed())
	{
		ReportError(func, "Cannot have sealed static funcs");
		return false;
	}

	if(func->IsStatic() && func->IsVirtual())
	{
		ReportError(func, "Cannot have virtual static funcs");
		return false;
	}

	if(func->IsSealed() && func->IsVirtual())
	{
		ReportError(func, "Cannot have sealed virtual funcs");
		return false;
	}

	// Now, we have to check params
	bool paramsFailed = false;
	for(int i = 0; i < func->GetParameterCount(); ++i)
	{
		Parameter* param = func->GetParameter(i);
		
		if(!CheckChild(func, param))
		{
			paramsFailed = true;
			continue;
		}

		param->SetParent(func);

		Type* paramType = TypeLookup::LookupType(param->GetTypeExpr(), param->GetParent());
		if(!paramType)
		{
			paramsFailed = true;
			continue;
		}
		param->SetType(paramType);

		func->GetScope()->Add(param);
	}

	if(paramsFailed)
		return false;

	return true;
}

void StaticChecker::CheckFunctionBody(Function* func)
{
	
}

bool StaticChecker::CheckChild(Node* parent, Node* child)
{
	SSAssert(parent != 0);
	SSAssert(child != 0);
	SSAssert(parent->HasName());
	SSAssert(child->HasName());

	if(!IsValidName(child->GetName()))
	{
		// TODO: Can we do better than this? E.g. say what's wrong with the name?
		ReportError(child, "Invalid name %s", SS_QUOTE(child->GetName()));
		return false;
	}

	if(NamesEqual(parent->GetName(), child->GetName()))
	{
		ReportError(child, "Name %s conflicts with name of enclosing object %s",
							SS_QUOTE(child->GetName()),
							parent->GetDesc().c_str());
		return false;
	}

	Node* conflict = parent->GetScope()->Find(child->GetName());
	if(conflict != 0)
	{
		// TODO: Better error message
		ReportError(child, "Name %s conflicts with %s",
							SS_QUOTE(child->GetName()),
							conflict->GetDesc().c_str());
		return false;
	}

	return true;
}

}
