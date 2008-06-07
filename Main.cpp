#include "Base.h"
#include "Package.h"
#include "Class.h"
#include "CompileErrors.h"
#include "StaticChecker.h"
#include "Version.h"
#include "Code.h"

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

int main(int argc, char** argv)
{
	int retval = SS::SSCMain(argc, argv);
	//SS::Base::DumpInstances();
	return retval;
}
