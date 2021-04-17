#include "stdafx.h"
#include "JavaModelLib/compiler/DefaultProblem.h"
#include <tinyxml.h>
#include <Utillib/common/Util.h>
#include <set>

namespace Jikes { // Open namespace Jikes block


	namespace JavaModel {
		const char* const CLASS = "class"; //$NON-NLS-1$
        const char* const CLASS_FILE = "classfile"; //$NON-NLS-1$
        const char* const CLASSPATH = "classpath"; //$NON-NLS-1$
        const char* const CLASSPATH_FILE = "FILE"; //$NON-NLS-1$
        const char* const CLASSPATH_FOLDER = "FOLDER"; //$NON-NLS-1$
        const char* const CLASSPATH_ID = "id"; //$NON-NLS-1$
        const char* const CLASSPATH_JAR = "JAR"; //$NON-NLS-1$
        const char* const CLASSPATHS = "classpaths"; //$NON-NLS-1$
        const char* const COMMAND_LINE_ARGUMENT = "argument"; //$NON-NLS-1$
        const char* const COMMAND_LINE_ARGUMENTS = "command_line"; //$NON-NLS-1$
        const char* const COMPILER = "compiler"; //$NON-NLS-1$
        const char* const COMPILER_COPYRIGHT = "copyright"; //$NON-NLS-1$
        const char* const COMPILER_NAME = "name"; //$NON-NLS-1$
        const char* const COMPILER_VERSION = "version"; //$NON-NLS-1$

        const char* const STR_ERROR = "ERROR"; //$NON-NLS-1$
        const char* const ERROR_TAG = "error"; //$NON-NLS-1$
        const char* const WARNING_TAG = "warning"; //$NON-NLS-1$
        const char* const EXCEPTION = "exception"; //$NON-NLS-1$
        const char* const EXTRA_PROBLEM_TAG = "extra_problem"; //$NON-NLS-1$
        const char* const EXTRA_PROBLEMS = "extra_problems"; //$NON-NLS-1$
   
        const char* const KEY = "key"; //$NON-NLS-1$
        const char* const MESSAGE = "message"; //$NON-NLS-1$
        const char* const NUMBER_OF_CLASSFILES = "number_of_classfiles"; //$NON-NLS-1$
        const char* const NUMBER_OF_ERRORS = "errors"; //$NON-NLS-1$
        const char* const NUMBER_OF_LINES = "number_of_lines"; //$NON-NLS-1$
        const char* const NUMBER_OF_PROBLEMS = "problems"; //$NON-NLS-1$
        const char* const NUMBER_OF_TASKS = "tasks"; //$NON-NLS-1$
        const char* const NUMBER_OF_WARNINGS = "warnings"; //$NON-NLS-1$
        const char* const NUMBER_OF_INFOS = "infos"; //$NON-NLS-1$
        const char* const OPTION = "option"; //$NON-NLS-1$
        const char* const OPTIONS = "options"; //$NON-NLS-1$
        const char* const OUTPUT = "output"; //$NON-NLS-1$
        const char* const PACKAGE = "package"; //$NON-NLS-1$
        const char* const PATH = "path"; //$NON-NLS-1$
        const char* const PROBLEM_ARGUMENT = "argument"; //$NON-NLS-1$
        const char* const PROBLEM_ARGUMENT_VALUE = "value"; //$NON-NLS-1$
        const char* const PROBLEM_ARGUMENTS = "arguments"; //$NON-NLS-1$
        const char* const PROBLEM_CATEGORY_ID = "categoryID"; //$NON-NLS-1$
        const char* const ID = "id"; //$NON-NLS-1$
        const char* const PROBLEM_ID = "problemID"; //$NON-NLS-1$
        const char* const PROBLEM_LINE = "line"; //$NON-NLS-1$
        const char* const PROBLEM_OPTION_KEY = "optionKey"; //$NON-NLS-1$
        const char* const PROBLEM_MESSAGE = "message"; //$NON-NLS-1$
        const char* const PROBLEM_SEVERITY = "severity"; //$NON-NLS-1$
        const char* const PROBLEM_SOURCE_END = "charEnd"; //$NON-NLS-1$
        const char* const PROBLEM_SOURCE_START = "charStart"; //$NON-NLS-1$
        const char* const PROBLEM_SUMMARY = "problem_summary"; //$NON-NLS-1$
        const char* const PROBLEM_TAG = "problem"; //$NON-NLS-1$
        const char* const PROBLEMS = "problems"; //$NON-NLS-1$
        const char* const SOURCE = "source"; //$NON-NLS-1$
        const char* const SOURCE_CONTEXT = "source_context"; //$NON-NLS-1$
        const char* const SOURCE_END = "sourceEnd"; //$NON-NLS-1$
        const char* const SOURCE_START = "sourceStart"; //$NON-NLS-1$
        const char* const SOURCES = "sources"; //$NON-NLS-1$

        const char* const STATS = "stats"; //$NON-NLS-1$

        const char* const TASK = "task"; //$NON-NLS-1$
        const char* const TASKS = "tasks"; //$NON-NLS-1$
        const char* const TIME = "time"; //$NON-NLS-1$
        const char* const VALUE = "value"; //$NON-NLS-1$
        const char* const WARNING = "WARNING"; //$NON-NLS-1$
        const char* const INFO = "INFO"; //$NON-NLS-1$

		const char* const PROBLEM_COLUMN = "column"; //$NON-NLS-1$
		const char* const PROBLEM_RIGHT_LINE = "rline"; //$NON-NLS-1$
		const char* const PROBLEM_RIGHT_COLUMN = "rcolumn"; //$NON-NLS-1$

		
		DefaultProblem::DefaultProblem(): /*JikesError(Problem::ErrorInfo),*/ id(0), startPosition(0), endPosition(0),
		                                  line(0),
		                                  column(0), rline(0), rcolumn(0), severity(Problem::Severity::JIKES_CAUTION)
		{
		}

	
		void ECJSource::TransferError(std::vector<DefaultProblem*>& future_owner)
		{
			if (problems.empty())
				return;
			future_owner.insert(future_owner.end(), problems.begin(), problems.end());
			problems.clear();
		}

		unsigned ECJSource::NumErrors()
		{
			return  problems.size();
		}

		ECJSource::~ECJSource()
		{
			for(auto it :problems)
			{
				delete it;
			}
			problems.clear();
		}

		void ProcessProblems(TiXmlElement* problemsHeaderItem, ECJSource* ej_source)
		{
			TiXmlElement* problemEle = problemsHeaderItem->FirstChildElement();
			while (problemEle)
			{
				DefaultProblem* default_problem = new DefaultProblem();
				ej_source->problems.push_back(default_problem);
				int  sourceEnd = 0;
				int  sourceStart = 0;
				int line = 0;
				wstring message;
				auto str = problemEle->Attribute(PROBLEM_SOURCE_END);
				if (str)
				{
					default_problem->endPosition = Util::StringToInt(str);
				}

				str = problemEle->Attribute(PROBLEM_SOURCE_START);
				if (str)
				{
					default_problem->startPosition = Util::StringToInt(str);
				}
				str = problemEle->Attribute(PROBLEM_LINE);
				if (str)
				{
					default_problem->line = Util::StringToInt(str);
				}
				str = problemEle->Attribute(PROBLEM_COLUMN);
				if (str)
				{
					default_problem->column = Util::StringToInt(str);
				}
				str = problemEle->Attribute(PROBLEM_RIGHT_LINE);
				if (str)
				{
					default_problem->rline = Util::StringToInt(str);
				}
				str = problemEle->Attribute(PROBLEM_RIGHT_COLUMN);
				if (str)
				{
					default_problem->rcolumn = Util::StringToInt(str);
				}
				
				str = problemEle->Attribute(PROBLEM_ID);
				if (str)
				{
					default_problem->id = Util::StringToInt(str);
				}
				
				str = problemEle->Attribute(PROBLEM_SEVERITY);
				Problem::Severity severity = Problem::JIKES_CAUTION;
				if (str)
				{
					string temp = str;
					if(temp == STR_ERROR)
					{
						severity = Problem::JIKES_ERROR;
					}
					else if(temp == INFO)
					{
						severity = Problem::JIKES_CAUTION;
					}
					else if(temp == WARNING)
					{
						severity = Problem::JIKES_WARNING;
					}
					default_problem->severity = severity;
				}
				auto messageItem = problemEle->FirstChildElement(PROBLEM_MESSAGE);
				if(messageItem)
				{
					str = messageItem->Attribute(VALUE);
					if(str)
					{
						default_problem->message = Util::Utf8ToUnicode(str);
					}
				}
				auto source_contextItem = problemEle->FirstChildElement(SOURCE_CONTEXT);
				
				
				if(source_contextItem)
				{
					str = source_contextItem->Attribute(SOURCE_END);
					if(str)
					{
						default_problem->context.sourceEnd = Util::StringToInt(str);
					}
					str =  source_contextItem->Attribute(SOURCE_START);
					if (str)
					{
						default_problem->context.sourceStart = Util::StringToInt(str);
					}

					str = source_contextItem->Attribute(VALUE);
					if (str)
					{
						default_problem->context.value = Util::Utf8ToUnicode(str);
					}
					
				}
				problemEle = problemEle->NextSiblingElement();
			}
			
		}
		void ParserXmlData(const wstring& data, vector<ECJSource*>& result)
		{
			TiXmlDocument xml;
			xml.Parse(Util::UnicodeToUtf8(data).c_str(),nullptr, TIXML_ENCODING_UTF8);
			if(xml.Error())
			{
				return;
			}
			TiXmlElement* rootElement = xml.RootElement();
			TiXmlElement* sourcesItem = rootElement->FirstChildElement(SOURCES);
			if(!sourcesItem)
			{
				return;
			}
			TiXmlElement* sourceEle = sourcesItem->FirstChildElement();
			while (sourceEle)
			{
				ECJSource* ej_source = new ECJSource();
				result.push_back(ej_source);
				auto str = sourceEle->Attribute(PATH);
				if(str)
				{
					std::wstringex temp = Util::Utf8ToUnicode(str);
					temp.replace(L"\\", L"/");
					ej_source->filePath.swap(temp);
				}
				
				TiXmlElement* problemsHeaderItem = sourceEle->FirstChildElement(NUMBER_OF_PROBLEMS);
				if (problemsHeaderItem)
				{
					ProcessProblems(problemsHeaderItem,ej_source);
				}
				TiXmlElement* class_file_item = sourceEle->FirstChildElement(CLASS_FILE);
				set<wstring> class_file_paths;
				while (class_file_item)
				{
					str = class_file_item->Attribute(PATH);
					if(str)
					{
						std::wstringex temp = Util::Utf8ToUnicode(str);
						temp.replace(L"\\", L"/");
						class_file_paths.insert(temp);
					}
					class_file_item = class_file_item->NextSiblingElement();
				}
				ej_source->class_file_paths.swap(class_file_paths);
				
				sourceEle = sourceEle->NextSiblingElement();
			}
			
		}
		
	} // Close namespace JavaModel block

} // Close namespace Jikes block

