#include "Prereqs.h"
#include "OptionParser.h"
#include "Version.h"
#include "Name.h"
#include "Compiler.h"
#include "Package.h"

namespace SS {

const String SSC_NAME = "ssc";

class SSC : public OptionParser, public ICompilationListener
{
public:

	// TODO: Since we're not printing out options based on the help params, remove them!
	SSC() : OptionParser(SSC_NAME),
			m_optHelp('?', "help", ARG_NONE, "Shows program usage information"),
			m_optVersion('v', "version", ARG_NONE, "Shows program version information"),
			m_optLink('l', "link", ARG_REQUIRED, "Links to another SS package", "FILE"),
			m_optOut('o', "out", ARG_REQUIRED, "Sets the output package filename", "FILE"),
			m_optPkgName('p', "pkgname", ARG_REQUIRED, "Sets the name of this package", "NAME"),
			m_hasPkgName(false),
			m_hasOutName(false)
	{
		RegisterOption(&m_optHelp);
		RegisterOption(&m_optVersion);
		RegisterOption(&m_optLink);
		RegisterOption(&m_optOut);
		RegisterOption(&m_optPkgName);
	}

	void OnOption(Option* opt, bool hasArg, const String& optarg)
	{
		// --help
		if(opt == &m_optHelp)
		{
			ShowUsage();
			Exit(0);
		}
		// --version
		else if(opt == &m_optVersion)
		{
			ShowVersion();
			Exit(0);
		}
		// --pkgname
		else if(opt == &m_optPkgName)
		{
			if(m_hasPkgName)
			{
				std::cerr << SSC_NAME << ": Cannot specify package name multiple times" << std::endl;
				Exit(1);
			}
			if(!IsValidName(optarg))
			{
				std::cerr << SSC_NAME << ": Invalid package name: '" << optarg << "'" << std::endl;
				Exit(1);
			}
			else
			{
				m_hasPkgName = true;
				m_pkgName = optarg;
			}
		}
		// TODO: --out
		// TODO: --link
	}

	void ShowUsage()
	{
		std::cout << "Usage: " << SSC_NAME << " [options] file..." << std::endl
					<< "Compiles SHilScript source files to a bytecode package." << std::endl
					<< std::endl
					<< "Options:" << std::endl
					<< "-?, --help               Display program usage information" << std::endl
					<< "-l FILE, --link FILE     Link to package in FILE" << std::endl
					<< "-o FILE, --out FILE      Output bytecode package to FILE" << std::endl
					<< "-p NAME, --pkgname NAME  Set output package name" << std::endl
					<< "-v, --version            Display program version information" << std::endl
					;
	}

	void ShowVersion()
	{
		std::cout << "SHilScript version " << SS_VERSION_STRING << std::endl;
	}

	void OnArg(const String& arg)
	{
		m_files.push_back(arg);
	}

	int Main(int argc, char** argv)
	{
		Parse(argc, argv);

		if(m_files.size() == 0)
		{
			std::cerr << SSC_NAME << ": No input files" << std::endl;
			ShowUsage();
			Exit(1);
		}

		if(!m_hasPkgName)
		{
			std::cerr << SSC_NAME << ": No package name specified" << std::endl;
			ShowUsage();
			Exit(1);
		}

		Compiler compiler;
		compiler.SetCompilationListener(this);

		for(StringList::iterator it = m_files.begin(); it != m_files.end(); ++it)
		{
			std::ifstream infile(it->c_str());

			if(!infile)
			{
				CompilerMessage msg;
				msg.SetMessageType(MSG_ERROR);
				msg.SetText("Cannot open file: " + *it);
				compiler.ReportMessage(msg);
				continue;
			}

			compiler.Parse(*it, infile);
		}

		Package* pkg = compiler.Compile(m_pkgName);

		int errors = compiler.GetErrorCount(),
			warnings = compiler.GetWarningCount();
		std::cout << compiler.GetErrorCount() << (errors == 1 ? " error, " : " errors, ")
					<< compiler.GetWarningCount() << (warnings == 1 ? " warning" : " warnings") << std::endl;
		
		if(pkg)
		{
			// TODO: Do something with the package...

			delete pkg;

			return 0;
		}
		else
			return 1;
	}

	void OnCompilerMessage(const CompilerMessage& msg)
	{
		std::cerr << msg.Format() << std::endl;
	}

private:
	typedef std::list<String> StringList;
	StringList	m_files;

	bool	m_hasPkgName;
	bool	m_hasOutName;
	String	m_pkgName;

	Option m_optHelp,
			m_optVersion,
			m_optLink,
			m_optOut,
			m_optPkgName;
};

int SSCMain(int argc, char** argv)
{
	SSC ssc;
	return ssc.Main(argc, argv);
}

}
