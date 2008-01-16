#ifndef SS_COMPILER_H
#define SS_COMPILER_H

#include "Prereqs.h"

namespace SS {

class Package;
class FileInfo;

enum CompilerMsgType
{
	MSG_WARNING,
	MSG_ERROR,
	MSG_INFO
};

struct CompilerMessage
{
public:
	CompilerMessage() : m_msgType(MSG_INFO), m_file(0), m_line(0) {}

	String Format() const;

	SS_GETSET(CompilerMsgType, MessageType, m_msgType);
	SS_GETSET(FileInfo*, File, m_file);
	SS_GETSET(int, Line, m_line);
	SS_GETSET(const String&, Text, m_text);

private:
	CompilerMsgType		m_msgType;
	FileInfo*			m_file;
	int					m_line;
	String				m_text;
};

class SS_EXPORT ICompilationListener
{
public:
	virtual ~ICompilationListener() {}

	virtual void OnCompilerMessage(const CompilerMessage& msg)=0;
};

class SS_EXPORT Compiler
{
public:
	Compiler();
	virtual ~Compiler();

	ICompilationListener* GetCompilationListener() const { return m_listener; }
	void SetCompilationListener(ICompilationListener* listener) { m_listener = listener; }

	void Parse(const String& filename, const String& scriptText);
	void Parse(const String& filename, std::istream& scriptStream);
	Package* Compile(const String& pkgName);

	int GetWarningCount() const { return m_warnings; }
	int GetErrorCount() const { return m_errors; }
	bool Succeeded() const { return m_succeeded; }

	void ReportMessage(const CompilerMessage& msg);

private:
	bool					m_compiled;
	int						m_errors;
	int						m_warnings;
	bool					m_succeeded;

	Package*				m_pkg;
	ICompilationListener*	m_listener;
};

SS_EXPORT Compiler* GetActiveCompiler();

}

#endif // SS_COMPILER_H
