#include "CompileErrors.h"
#include "Compiler.h"

namespace SS {

#ifdef SS_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable: 4996)
#endif // SS_COMPILER_MSVC

void ReportError(bool warning, Node* obj, const String& str);

#define SS_PRINT_TO_BUF(bufname, size, format) \
	char buf[size]; \
	buf[sizeof(buf)-1] = 0; \
	va_list list; \
	va_start(list, format); \
	vsnprintf(buf, sizeof(buf)-1, format.c_str(), list); \
	va_end(list);

void ReportError(String format, ...)
{
	SS_PRINT_TO_BUF(buf, 1024, format);
	ReportError(false, NULL, buf);
}

void ReportError(Node* obj, String format, ...)
{
	SS_PRINT_TO_BUF(buf, 1024, format);
	ReportError(false, obj, buf);
}

void ReportWarning(String format, ...)
{
	SS_PRINT_TO_BUF(buf, 1024, format);
	ReportError(true, NULL, buf);
}

void ReportWarning(Node* obj, String format, ...)
{
	SS_PRINT_TO_BUF(buf, 1024, format);
	ReportError(true, obj, buf);
}

#ifdef SS_COMPILER_MSVC
#pragma warning(pop)
#endif // SS_COMPILER_MSVC

#undef SS_PRINT_TO_BUF

void ReportError(bool warning, Node* obj, const String& str)
{
	Compiler* compiler = GetActiveCompiler();
	SSAssert(compiler != 0);
	
	CompilerMessage msg;
	msg.SetMessageType(warning ? MSG_WARNING : MSG_ERROR);
	msg.SetFile(obj != 0 ? obj->GetFile() : 0);
	msg.SetLine(obj != 0 ? obj->GetLine() : 0);
	msg.SetText(str);

	compiler->ReportMessage(msg);
}

bool AnyErrors()
{
	Compiler* compiler = GetActiveCompiler();
	SSAssert(compiler != 0);

	return compiler->GetErrorCount() > 0;
}

// In the future this'll also take care of escapes and whatnot
String QuoteString(const String& str)
{
	return "'"+str+"'";
}

}
