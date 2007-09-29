#ifndef SS_NAMEDICT_H
#define SS_NAMEDICT_H

#include "Prereqs.h"
#include "Node.h"
#include "Name.h"

namespace SS {

class Scope
{
public:
	Scope() {}
	~Scope() {}

	void Add(Node* obj)
	{
		Add(obj->GetName(), obj);
	}

	Node* Find(const String& name) const
	{
		String nameLower(name);
		ToLowerInplace(nameLower);

		BaseDict::const_iterator it = m_dict.find(nameLower);
		if(it == m_dict.end())
			return 0;
		else
			return it->second;
	}

	void Print()
	{
		BaseDict::iterator it;
		for(it = m_dict.begin(); it != m_dict.end(); ++it)
			std::cout << it->first << std::endl;
		std::cout << m_dict.size() << std::endl;
	}

	void Clear()
	{
		m_dict.clear();
	}

private:

	bool Add(const String& name, Node* obj)
	{
		SSAssert(obj != 0);

		String nameLower(name);
		ToLowerInplace(nameLower);
		
		BaseDict::const_iterator it = m_dict.find(nameLower);
		if(it == m_dict.end())
		{
			m_dict[nameLower] = obj;
			return true;
		}
		else
			return false;
	}


	typedef std::map<std::string,Node*> BaseDict;

	BaseDict m_dict;
};

}

#endif // SS_NAMEDICT_H
