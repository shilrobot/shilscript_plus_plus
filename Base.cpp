#include "Prereqs.h"
#include "Base.h"

namespace SS {

Base* Base::s_head = 0;
Base* Base::s_tail = 0;

const int SS_BASE_MAGIC = 0x1234ABCD;

Base::Base() : m_prev(0), m_next(0), m_magic(SS_BASE_MAGIC)
{
	if(s_head == 0)
		s_head = s_tail = this;
	else
	{
		s_tail->m_next = this;
		m_prev = s_tail;
		s_tail = this;
	}
}

Base::~Base()
{
	m_magic = 0;

	if(m_prev != 0)
		m_prev->m_next = m_next;
	else
		s_head = m_next;

	if(m_next != 0)
		m_next->m_prev = m_prev;
	else
		s_tail = m_prev;

	m_prev = m_next = 0;
}

void Base::DumpInstances()
{
	Base* curr = s_head;
	int count = 0;
	printf("-----------------------------------------------\n");
	while(curr != 0)
	{
		printf("0x%08X %s%s\n", (size_t)(void*)curr, curr->DynamicType()->GetName( ), curr->m_magic != SS_BASE_MAGIC ? " MAGIC FAIL":"");
		curr = curr->m_next;
		++count;
	}
	printf("%d total allocated Base instances\n", count);
	printf("-----------------------------------------------\n");
}

}
