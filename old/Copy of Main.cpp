#include "SS.h"
#include "Type.h"
#include <sstream>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

using namespace SS;

enum
{
#define SS_GEN_SYMBOLS
#include "Generated.h"
#undef SS_GEN_SYMBOLS
};

struct Instr
{
	uint32 op;
};

#define SS_GEN_STRUCTS
#include "Generated.h"
#undef SS_GEN_STRUCTS

class Type
{
};

class Member
{
};

class Compound
{
	// name (inherited)/id/whatever
	// base
	// member list
	// flags
};

class Func
{
public:
	Func() : m_instrs(0)
	{
	}

	void AddInstr(Instr* i, size_t size)
	{
		m_instrOffsets.push_back((uint32)m_code.size());

		uint8* instrData = (uint8*)i;
		for(int n=0; n < (int)size; ++n)
			m_code.push_back(instrData[n]);		
	}

	template<class T>
	void printData(const T& t)
	{
		std::stringstream ss;
		ss << t;
		printf("%s", ss.str().c_str());
	}

#define SS_GEN_ASSEMBLE
#include "Generated.h"
#undef SS_GEN_ASSEMBLE

	void Disassemble()
	{
		for(int i=0; i < (int)m_instrOffsets.size(); ++i)
		{
			Instr* instr = m_instrs[i];

			printf("%04d: ", i);

			switch(instr->op)
			{
#define SS_GEN_DISASSEMBLE
#include "Generated.h"
#undef SS_GEN_DISASSEMBLE

			default:
				printf("???\n");
				return;
			}

			printf("\n");
		}
	}

	void Finalize()
	{
		m_instrs = new Instr*[m_instrOffsets.size()];

		for(int i=0; i < (int)m_instrOffsets.size(); ++i)
		{
			uint8* instrData = &m_code[m_instrOffsets[i]];
			m_instrs[i] = (Instr*)instrData;
		}
	}

public:
	std::vector<uint8>	m_code;
	std::vector<uint32> m_instrOffsets;
	Instr** m_instrs;
	//std::vector<Instr*> m_instrs;
};

class VM
{
public:
	//Struct* CreateStruct(const char* name);
	Func* CreateFunc(const char* name);
	//Global* CreateGlobal(const char* name);
};

//#define BOUNDS_CHECK

#ifdef BOUNDS_CHECK

// TODO: Disable checks during final builds, or use guard pages
#define POP(type,name)						\
	if(sp+sizeof(type) > m_stackUpperBound)	\
		goto StackUnderflow;				\
	name = *(type*)sp;						\
	sp += sizeof(type);

#define PUSH(type,name)						\
	if(sp-sizeof(type) < m_operandStack)	\
		goto StackOverflow;					\
	sp -= sizeof(type);						\
	*(type*)sp = name;

#else

// TODO: Disable checks during final builds, or use guard pages
#define POP(type,name)						\
	name = *(type*)sp;						\
	sp += sizeof(type);

#define PUSH(type,name)						\
	sp -= sizeof(type);						\
	*(type*)sp = name;

#endif

#define SET_PC(val) pc = (val);
#define INC_PC ++pc;

const int OPERAND_STACK_SIZE = 1024*4;

const int TEST_INSTRS = 300000000;

class Context
{
public:
	Context()
	{
		// TODO: Verify this is 4-byte aligned
		// TODO: Do uint64's need to be aligned specially for better speed?
		m_operandStack = new uint8[OPERAND_STACK_SIZE];
		m_stackUpperBound = &m_operandStack[OPERAND_STACK_SIZE];
	}

	~Context()
	{
		delete [] m_operandStack;
	}

	template<class T>
	SS_FORCEINLINE T Pop(uint32** spPtr)
	{
		T* sp = *spPtr;
		T tmp = *sp;
		--sp;
		*spPtr = (uint32*)sp;
		return tmp;
	}

	void Execute(Func* f)
	{
		m_func = f;
		int pc = 0;
		uint8* sp = m_stackUpperBound;
	
		int count = 0;

		for(;;)
		{
			// TODO: Don't use a vector: less decode instrs I think(?)
			Instr* instr = m_func->m_instrs[pc];

			switch(instr->op)
			{
#define SS_GEN_INTERPRETER
#include "Generated.h"
#undef SS_GEN_INTERPRETER
			}

			if(count == TEST_INSTRS)
				return;
			count++;
		}

		return;

	StackUnderflow:
		printf("*** Stack underflow!\n");
		return;
		
	StackOverflow:
		printf("*** Stack overflow!\n");
		return;
	}

private:
	Func* m_func;
	//std::vector<uint32> m_stack;
	//std::vector<uint32> m_operandStack;
	uint8* m_operandStack;
	uint8* m_stackUpperBound;
};

int main(int argc, char** argv)
{
	
	//Instr_add_ui instr;
	//instr.opcode = OP_ADD_UI;

	Func* f = new Func();
	//f->AddInstr((Instr*)&instr, sizeof(instr));
	//f->AddInstr((Instr*)&instr, sizeof(instr));


	f->load_const_i(5);
	f->load_const_i(4);
	f->add_i();
	//f->print_i();
	f->pop_i();
	f->jump(0);
	//f->ret();

	f->Finalize();


	int bytesLeft = f->m_code.size();
	int n=0;
	while(bytesLeft > 0)
	{
		for(int i=0; i<8; ++i)
		{
			if(bytesLeft > 0)
			{
				printf("%02X", (int)f->m_code[n]);
				++n;
				--bytesLeft;
			}
			else
				printf("  ");
		}
		printf("\n");
	}

	f->Disassemble();

	Context* ctx = new Context();

	uint64 start;
	uint64 freq;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	QueryPerformanceCounter((LARGE_INTEGER*)&start);

	ctx->Execute(f);

	uint64 end;
	QueryPerformanceCounter((LARGE_INTEGER*)&end);
	double diff = (double)(end - start)/(double)freq;
	printf("Time: %.2f sec\n", (float)diff);
	printf("Rate: %.2f million instrs/sec\n", (float)((double)TEST_INSTRS/diff) / 1e6f);

	delete ctx;

	delete f;

	return 0;
}
