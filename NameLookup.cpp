#include "NameLookup.h"
#include "Name.h"
#include "Package.h"
#include "Class.h"
#include "CompileErrors.h"
#include "Function.h"

namespace SS {


Node* NameLookup::LookupName(const String& name, Node* context, Node* errSource)
{
	SSAssert(name.size() > 0);
	SSAssert(context != 0);
	SSAssert(errSource != 0);

	LookupVisitor visitor(name, context, errSource);
	LookupName(name, context, &visitor);
	return visitor.Complete();
}

Node* NameLookup::LookupMember(const String& name, Node* parent, Node* context, Node* errSource)
{
	SSAssert(name.size() > 0);
	SSAssert(parent != 0);
	SSAssert(context != 0);
	SSAssert(errSource != 0);

	LookupVisitor visitor(name, context, errSource);
	LookupMember(name, parent, &visitor);
	return visitor.Complete();
}


// NOTE: This "smells" like it should be in the node classes, but it's nice to have
//		 it all right here in one place instead -- so it's easier to read all at once.

void NameLookup::LookupName(const String& name, Node* currObj, ILookupVisitor* visitor)
{
	if(currObj->IsA<Package>())
	{
		Package* pkg = dynamic_cast<Package*>(currObj);

		// Check against this package's name
		if(NamesEqual(name, pkg->GetName()))
			visitor->OnMatch(pkg);

		if(!visitor->OnGroupEnd())
			return;

		// Look up in package-level scope
		Node* n = pkg->GetScope()->Find(name);
		if(n)
			visitor->OnMatch(n);

		//if(!visitor->OnGroupEnd()) return;

		// TODO: Match other package names
		
		// TODO: Look up in package dependencies (ambiguities can arise here)

		return;
	}
	else if(currObj->IsA<Class>())
	{
		Class* cls = dynamic_cast<Class*>(currObj);

		// Look up in bases
		// (but DO NOT look up into the bases' parent contexts!)
		Class* base = cls;
		while(base != 0)
		{
			Node* n = base->GetScope()->Find(name);

			if(n)
				visitor->OnMatch(n);

			if(!visitor->OnGroupEnd())
				return;

			base = base->GetBase();
		}

		// Look up in enclosing object (usually pkg)
		LookupName(name, cls->GetParent(), visitor);
	}
	else if(currObj->IsA<Function>())
	{
		// TODO: Look up in parameter list
		// TODO: Look up in enclosing object (usually class)
		return;
	}
	// TODO: Scope
		// Look up in this scope
		// Look up in parent obj (usually another scope, or function)
	else
		// This should never really happen :(
		return;
}

void NameLookup::LookupMember(const String& name, Node* parent, ILookupVisitor* visitor)
{
	if(parent->IsA<Package>())
	{
		Package* pkg = dynamic_cast<Package*>(parent);

		// Look up in package-level scope
		Node* n = pkg->GetScope()->Find(name);

		if(n)
		{
			visitor->OnMatch(n);

			if(!visitor->OnGroupEnd())
				return;
		}
	}
	else if(parent->IsA<Class>())
	{
		Class* cls = dynamic_cast<Class*>(parent);

		Class* base = cls;
		while(base != 0)
		{
			Node* n = base->GetScope()->Find(name);

			if(n)
				visitor->OnMatch(n);

			if(!visitor->OnGroupEnd())
				return;

			base = cls->GetBase();
		}
	}
	// TODO: Error messages (abrupt failure) for trying to do dots on weird other stuff..?
}

//------------------------------------------------------------------

bool LookupVisitor::OnGroupEnd()
{
	if(m_nodes.size() == 0)
		return true;

	std::vector<Node*> matches;
	
	for(int i=0; i < (int)m_nodes.size(); ++i)
	{
		if(IsAccessible(m_nodes[i], m_context))
			matches.push_back(m_nodes[i]);
		else
			m_inaccessible.push_back(m_nodes[i]);
	}

	m_nodes.clear();

	// TODO: Store info that it was inaccessible for later...!

	if(matches.size() == 0)
	{
		if(m_resultCode == RESULT_NOT_FOUND)
			m_resultCode = RESULT_INACCESSIBLE;
		return true;
	}
	else if(matches.size() == 1)
	{
		m_resultCode = RESULT_FOUND;
		m_result = matches.front();
		return false; // Done
	}
	else
	{
		m_resultCode = RESULT_AMBIGUOUS;
		ReportError(m_errSource, "Ambiguous name %s. Possible matches:", SS_QUOTE(m_name));
		for(int i=0; i < (int)matches.size(); ++i)
		{
			// TODO: We need an "error continuation" mechanism so this only counts as 1 error
			Node* n = matches[i];
			ReportError(m_errSource,  "  %s%s",
										n->GetDesc().c_str(),
										i < (int)(matches.size()-1) ? ", or" : "");
		}
		return false; // Quit immediately
	}
}

Node* LookupVisitor::Complete()
{
	// Finish up if that hasn't happened yet
	if(m_nodes.size() > 0)
		OnGroupEnd();

	// TODO: These errors should be altered a little depending on whether it is a bare name or member lookup!
	//		 e.g. class X has no member named Z, etc.

	switch(m_resultCode)
	{
	case RESULT_FOUND:
		return m_result;
	case RESULT_NOT_FOUND:
		ReportError(m_errSource, "Unknown name: %s", SS_QUOTE(m_name)); // TODO: Uh, actually print the fucking name
		return 0;
	case RESULT_AMBIGUOUS:
		// Error msg. already printed
		return 0;
	case RESULT_INACCESSIBLE:
		{
			SSAssert(m_inaccessible.size() > 0);

			if(m_inaccessible.size() == 1)
			{
				ReportError(m_errSource, "%s is inaccessible from this context", m_inaccessible.front()->GetDesc().c_str());				
			}
			else
			{
				ReportError(m_errSource, "Nothing named %s is accessible from this context.");
				ReportError(m_errSource, "  Inaccessible items with this name:", SS_QUOTE(m_name));
				for(int i=0; i < (int)m_inaccessible.size(); ++i)
				{
					// TODO: We need an "error continuation" mechanism so this only counts as 1 error
					Node* n = m_inaccessible[i];
					ReportError(m_errSource, "  %s%s",
									n->GetDesc().c_str(),
									i < (int)(m_inaccessible.size()-1) ? "" : "");
				}
			}

			return 0;
		}
	default:
		SS_UNREACHABLE;
		return 0;
	}
}

}
