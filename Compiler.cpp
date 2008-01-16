#include "Compiler.h"
#include "Node.h"
#include "Package.h"
#include "StaticChecker.h"
#include "grammar/SSLexer.hpp"
#include "grammar/SSParser.hpp"

namespace SS {

static Compiler* g_compiler = 0;

Compiler* GetActiveCompiler()
{
	return g_compiler;
}

template<class T>
String ToString(const T& val)
{
	std::stringstream ss;
	ss << val;
	return ss.str();
}

String CompilerMessage::Format() const
{
	String result;

	if(m_file != 0)
	{
		result += m_file->GetFilename();

		if(m_line > 0)
		{
			result += ":";
			result += ToString(m_line);
		}

		result += ": ";
	}

	if(m_msgType == MSG_WARNING)
		result += "warning: ";

	return result + m_text;
}

Compiler::Compiler() :  m_compiled(false), m_errors(0), m_warnings(0), m_succeeded(false), m_pkg(0), m_listener(0)
{
	m_pkg = new Package();
}

Compiler::~Compiler()
{
	if(m_pkg != 0)
		delete m_pkg;
}

void Compiler::Parse(const String& filename, const String& scriptText)
{
	std::stringstream sstream(scriptText);
	Compiler::Parse(filename, sstream);
}

void Compiler::Parse(const String& filename, std::istream& scriptStream)
{
	SSAssert(!m_compiled);

	g_compiler = this;

	try
	{
		SSLexer lexer(scriptStream);
		SSParser parser(lexer);

		parser.translationUnit(m_pkg);
	}
	catch(antlr::ANTLRException& e)
	{
		ReportError(e.toString());
	}

	g_compiler = 0;
}

Package* Compiler::Compile(const String& pkgName)
{
	SSAssert(!m_compiled);

	Package* ret = 0;

	m_compiled = true;

	g_compiler = this;

	if(AnyErrors())
		goto Failed;

	m_pkg->SetName(pkgName);

	{
	StaticChecker checker(m_pkg);
	if(!checker.Check())
		goto Failed;
	}

	ret = m_pkg;
	m_pkg = 0;
	m_succeeded = true;
	g_compiler = 0;
	return ret;

Failed:
	g_compiler = 0;
	delete m_pkg;
	m_pkg = 0;
	return 0;
}

void Compiler::ReportMessage(const CompilerMessage& msg)
{
	if(msg.GetMessageType() == MSG_ERROR)
		++m_errors;
	else if(msg.GetMessageType() == MSG_WARNING)
		++m_warnings;

	if(m_listener != 0)
		m_listener->OnCompilerMessage(msg);
}

}
