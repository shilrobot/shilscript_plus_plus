#include "Prereqs.h"
#include "Code.h"
#include "Function.h"
#include "Type.h"
#include "Class.h"
#include "Variable.h"
#include "Local.h"

namespace SS {

void Code::Dump()
{
	printf("; %s\n", m_func->GetDesc().c_str());
	printf("; Instrs: %d\n", m_instructions.size());
	printf("; Args: %d\n", m_func->GetLocalCount());

	for(int i=0; i<m_func->GetParameterCount(); ++i)
	{
		Parameter* param = m_func->GetParameter(i);
		SSAssert(param != 0);
		printf("; %d: %s '%s'\n", param->GetID(), param->GetType()->GetName().c_str(), param->GetName().c_str());
	}

	printf("; Locals: %d\n", m_func->GetLocalCount());
	
	for(int i=0; i<m_func->GetLocalCount(); ++i)
	{
		Local* local = m_func->GetLocal(i);
		SSAssert(local != 0);
		printf("; %d: %s '%s'\n", local->GetID(), local->GetType()->GetName().c_str(), local->GetName().c_str());
	}
	printf("\n");

	for(int i=0; i<(int)m_instructions.size(); ++i)
	{
		printf("%4d: %s", i, GetOpcodeName((Opcode)m_instructions[i]->opcode));

		for(int j=0; j<m_instructions[i]->args.size(); ++j)
		{
			InstrArg& arg = m_instructions[i]->args[j];

			if(j != 0)
				printf(", ");
			else
				printf(" ");

			switch(arg.m_type)
			{
			case IAT_BOOL:
				printf(arg.GetBool() ? "true" : "false");
				break;
			case IAT_U1:
				printf("%d", (int)arg.GetU1());
				break;
			case IAT_I1:
				printf("%d", (int)arg.GetI1());
				break;
			case IAT_U2:
				printf("%d", (int)arg.GetU2());
				break;
			case IAT_I2:
				printf("%d", (int)arg.GetI2());
				break;
			case IAT_U4:
				printf("%d", arg.GetU4());
				break;
			case IAT_I4:
				printf("%d", arg.GetI4());
				break;
			case IAT_U8:
				printf("xxx", arg.GetU8());
				break;
			case IAT_I8:
				printf("xxx", arg.GetI8());
				break;
			case IAT_F4:
				printf("%g", arg.GetF4());
				break;
			case IAT_F8:
				printf("%g", arg.GetF8());
				break;
			case IAT_TYPE:
				printf("<%s>", arg.GetType()->GetName( ).c_str());
				break;
			case IAT_CLASS:
				printf("<%s>", arg.GetClass()->GetDesc( ).c_str());
				break;
			case IAT_FUNC:
				printf("<%s>", arg.GetFunction()->GetDesc( ).c_str());
				break;
			case IAT_VAR:
				printf("<%s>", arg.GetVariable()->GetDesc( ).c_str());
				break;
			case IAT_LABEL:
				printf("%d", arg.GetLabel()->targetIdx);
				break;
			default:
				printf("???");
				break;
			}
		}

		printf("\n");
	}
}

}
