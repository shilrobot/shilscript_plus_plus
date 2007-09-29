#include "Base.h"
#include "Package.h"
#include "Class.h"
#include "CompileErrors.h"
#include "StaticChecker.h"
#include "Version.h"

#include "grammar/SSParser.hpp"
#include "grammar/SSLexer.hpp"

#include "SSC.h"

// Fill in some more stuff for SSParser
namespace SS {

void SSParser::reportError(const antlr::RecognitionException& ex)
{
	// TODO: Include file info...
	ReportError(ex.toString());
}

void SSParser::reportError(const std::string& s)
{
	ReportError(s);
}

void SSParser::reportWarning(const std::string& s)
{
	ReportWarning(s);
}

}

using namespace SS;

bool TestDriver(Package* pkg)
{
	pkg->SetName("Test");

	std::ifstream infile("scripts/test.ss");
	if(!infile)
	{
		ReportError("Unable to open test script");
		return false;
	}

	try
	{
		SSLexer lexer(infile);
		SSParser parser(lexer);

		parser.translationUnit(pkg);
	}
	catch(antlr::ANTLRException& e)
	{
		ReportError(e.toString());
		return false;
	}

	if(AnyErrors())
		return false;

	/*
	bool result = pkg->Compile();
	if(!result)
		return false;
	*/

	StaticChecker checker(pkg);
	if(!checker.Check())
		return false;

	return true;
}

int main(int argc, char** argv)
{
	/*
	Package* pkg = new Package();
	bool result = TestDriver(pkg);
	ErrorReport();
	delete pkg;
	return result ? 0 : 1;
	*/

	return SSCMain(argc, argv);
}
