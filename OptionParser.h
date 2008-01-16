#ifndef SS_OPTIONPARSER_H
#define SS_OPTIONPARSER_H

#include "Prereqs.h"

namespace SS {

enum OptionArgs
{
	ARG_REQUIRED,
	ARG_NONE
};

struct Option
{
public:
	Option(char shortName, const String& longName, OptionArgs argtype, const String& desc, const String& meta="") :
		shortName(shortName), longName(longName), argtype(argtype), desc(desc), meta(meta)
	{
	}

	char shortName;
	String longName;
	OptionArgs argtype;
	String desc;
	String meta;
};

class OptionParser
{
public:
	OptionParser(const String& progname) : m_progname(progname)
	{
	}

	virtual ~OptionParser() {}

	void RegisterOption(Option* opt)
	{
		m_optionDict[opt->longName] = opt;
	}

	void Parse(int argc, char** argv)
	{
		SSAssert(argc >= 0);
		SSAssert(argv != 0);

		// Go through each arg
		for(int i=1; i<argc; ++i)
		{
			String arg = argv[i];

			// Longopt, e.g. --name, --name=XYZ, or --name XYZ
			if(IsLongOpt(arg))
			{
				size_t equalsPos = arg.find('=');
				String optname;
				String optarg;
				bool hasArg = false;
				if(equalsPos == String::npos)
					optname = arg.substr(2);
				else
				{
					optname = arg.substr(2, equalsPos-2);
					optarg = arg.substr(equalsPos+1);
					hasArg = true;
				}

				OptionDict::iterator it = m_optionDict.find(optname);
				if(it == m_optionDict.end())
				{
					OnUnknownOption(arg);
					continue;
				}

				Option* opt = it->second;

				if(opt->argtype == ARG_REQUIRED && !hasArg)
				{
					if(i < (argc-1))
					{
						String next = argv[i+1];
						if(!IsShortOpt(next) && !IsLongOpt(next))
						{
							hasArg = true;
							optarg = next;
							++i;
						}
					}
				}

				DispatchOption(opt, hasArg, optarg);
			}
			// Short opt (-a, -aFoo, -a Foo)
			else if(IsShortOpt(arg))
			{
				char optchar = arg[1];
				String optarg;
				bool hasArg = false;
				if(arg.size() > 2)
				{
					hasArg = true;
					optarg = arg.substr(2);
				}

				Option* opt = 0;
				OptionDict::iterator it;
				for(it = m_optionDict.begin(); it != m_optionDict.end(); ++it)
				{
					if(optchar == it->second->shortName)
					{
						opt = it->second;
						break;
					}
				}

				if(opt == 0)
				{
					OnUnknownOption(arg);
					continue;
				}
				
				if(opt->argtype == ARG_REQUIRED && !hasArg)
				{
					if(i < (argc-1))
					{
						String next = argv[i+1];
						if(!IsShortOpt(next) && !IsLongOpt(next))
						{
							hasArg = true;
							optarg = next;
							++i;
						}
					}
				}

				DispatchOption(opt, hasArg, optarg);
			}
			// Normal arg
			else
			{
				OnArg(arg);
			}
		}
	}


	virtual void OnOption(Option* option, bool hasArg, const String& text)
	{
		/*
		std::cout << "Opt " << option->longName;
		if(hasArg)
			std::cout << " " << text;
		std::cout << std::endl;
		*/
	}

	virtual void OnUnknownOption(const String& option)
	{
		std::cerr << m_progname << ": Unknown option: " << option  << std::endl;
		ShowUsage();
		Exit(1);
	}

	virtual void OnArg(const String& arg)
	{
		/*
		std::cout << "Arg " << arg << std::endl;
		*/
	}

	virtual void Exit(int result)
	{
		exit(result);
	}

	virtual void ShowUsage()
	{
	}

private:
	bool IsLongOpt(const String& arg) const
	{
		return arg.size() > 2 && arg[0] == '-' && arg[1] == '-';
	}

	bool IsShortOpt(const String& arg) const
	{
		return !IsLongOpt(arg) && arg.size() > 1 && arg[0] == '-';
	}

	void DispatchOption(Option* option, bool hasArg, String text)
	{
		if(option->argtype == ARG_NONE && hasArg)
		{
			std::cerr << m_progname << ": warning: Option -" << option->shortName << "/--" << option->longName << " does not take an argument" << std::endl;
			OnOption(option, false, "");
		}
		else if(option->argtype == ARG_REQUIRED && !hasArg)
		{
			std::cerr << m_progname << ": Option -" << option->shortName << "/--" << option->longName << " requires an argument" << std::endl;
			Exit(1);
		}
		else
			OnOption(option, hasArg, text);
	}

	const String& GetProgramName() const { return m_progname; }

private:
	typedef std::map<String, Option*> OptionDict;
	OptionDict	m_optionDict;
	String m_progname;
};

}

#endif // SS_OPTIONPARSER_H
