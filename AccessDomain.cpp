#include "AccessDomain.h"
#include "Node.h"
#include "Class.h"

namespace SS {

bool AccessDomain::Contains(Node* ctx) const
{
	// Universal access
	if(m_node == 0)
	{
		return true;
	}
	// Total lexical scoping
	else if(m_node != 0 && !m_derivedClasses)
	{
		Node* curr = ctx;
		while(curr != 0)
		{
			if(curr == m_node)
				return true;
			curr = curr->GetParent();
		}

		return false;
	}
	// Lexical scoping but with derived classes
	else if(m_node != 0 && m_derivedClasses)
	{
		SSAssert(m_node->IsA<Class>());

		Node* curr = ctx;
		while(curr != 0)
		{
			if(curr == m_node)
				return true;

			// When we hit an enclosing class, check if any of its bases are the 
			// class we're looking for...
			if(curr->IsA<Class>())
			{
				Class* cls = dynamic_cast<Class*>(curr);
				Class* base = cls->GetBase();
				while(base != 0)
				{
					if(base == m_node)
						return true;
					base = base->GetBase();
				}
			}

			curr = curr->GetParent();
		}

		return false;
	}
	else
	{
		SS_UNREACHABLE;
		return false;
	}
}

bool IsAccessible(Node* n, Node* context)
{
	SSAssert(n != 0);
	SSAssert(context != 0);
	return n->GetAccessDomain().Contains(context);
}

}
