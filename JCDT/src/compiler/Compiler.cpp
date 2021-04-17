
#include "stdafx.h"
#include "JCDT_Lib/internal/env/platform.h"
#include "JCDT_Lib/internal/env/LookupEnvironment.h"
#include "JCDT_Lib/internal/impl/option.h"
#include "JCDT_Lib/internal/problem/JikesError.h"
#include "JCDT_Lib/internal/impl/JikesOption.h"
#include "JCDT_Lib/internal/util/FileUtil.h"
#include "JCDT_Lib/internal/env/INameEnvironment.h"
#include "JCDT_Lib/internal/parser/parser.h"
#include "JCDT_Lib/internal/lex/scanner.h"
#include "JCDT_Lib/internal/semantic/semantic.h"
#include "JCDT_Lib/compiler/Compiler.h"
#include <JCDT_Lib/internal/parser/ParserUtil.h>
#include <boost/filesystem/operations.hpp>

namespace Jikes {
int init_check(){
	SetNewHandler();
	FloatingPointCheck();
	return 0;
}
int _init_check_ret = init_check();
void Compiler::compiler(set<FileSymbol*>& file_set)
{
	lookupEnvironment->RecordcompilationUnit(file_set);
	for (int j = 0, size = lookupEnvironment->input_files.size(); j < size; j++) {
		lookupEnvironment->ProcessFile(lookupEnvironment->input_files[j]);
	}
}
Compiler::Compiler(INameEnvironment* name_env, Option* option_) 
	: option(option_)                 
{
	lookupEnvironment = new LookupEnvironment(option, name_env,this);
	
	parser = new Parser();

	scanner = new Scanner(option, name_env);
}

	Compiler::Compiler(): lookupEnvironment(nullptr), option(nullptr), parsedOptions(nullptr), parser(nullptr), scanner(nullptr)
	{

	}

	Compiler::~Compiler()
{
   
	delete lookupEnvironment;
	
	delete parser;
	delete scanner;
}



JikesOption* Compiler::getOptions()
{
    return option;
}


bool Compiler::accept(FileSymbol* file_symbol)
{
	auto _info = HeadParse(file_symbol);
	if (!_info)
		return false;
	auto compilation_unit = file_symbol->compilation_unit;
	assert(compilation_unit);
	if (compilation_unit->state == CompilationUnitDeclaration::HEAD_PARSE) {
		for (unsigned k = 0; k < compilation_unit->NumTypeDeclarations(); k++) {
			TypeDeclaration* declaration = compilation_unit->GetTypeDeclaration(k);
			if (!EmptyTypeDeclaration::EmptyDeclarationCast(declaration)) {
				ParserUtil::CheckNestedMembers(file_symbol->lex_stream, declaration->class_body,parser);
			}
		}
		compilation_unit->state = CompilationUnitDeclaration::ALL_PARSE;
	}
	return true;
}

AstPackageDeclaration* Compiler::ParseForPackage(FileSymbol* file_symbol, AstStoragePool* ast_pool)
{
	if(!file_symbol->lex_stream)
		scanner->Scan(file_symbol);

	AstPackageDeclaration* package_declaration = nullptr;
	if (file_symbol->lex_stream) {	
		 package_declaration = parser->PackageHeaderParse(file_symbol->lex_stream, ast_pool);
	}
	return package_declaration;
}

	bool Compiler::HeadParse(FileSymbol* file_symbol)
	{
		if (!file_symbol->lex_stream) {
			scanner->Scan(file_symbol);
		}
	
		if (file_symbol->lex_stream) {
			if (!file_symbol->parser_error) {
				file_symbol->parser_error = new ParseError(option);
			}
			// If we have a compilation unit, analyze it, process its types.
			auto compilation_unit = file_symbol->compilation_unit;

			if (!compilation_unit) {
				file_symbol->parser_error->Clear();
				ParserUtil::DiagnoseAndCure(file_symbol->lex_stream, 
					(file_symbol->parser_error), option->errors, *lookupEnvironment);

				compilation_unit =
					parser->HeaderParse(file_symbol->lex_stream, nullptr);
				/*if(file_symbol->parser_error && file_symbol->parser_error->NumErrors()){
				file_symbol->compilation_unit->MarkBad();
				}
				*/
				if (!compilation_unit) {
					return false;
				}

				file_symbol->compilation_unit = compilation_unit;
			}
		}
		else {
			return false;
		}

		return true;
	}

	/**
	 * Compile given list of files.
	 */
int Compiler::compile(
	set<wstring>& add_file_set,
	set<wstring>& chang_file_set ,
	set<wstring>& remove_file_set)
{
    // Cast the JikesOption to an Option instance.
    // Note that the reason we don't use an Option
    // member type in the declaration of Compiler
    // is so that the jikespai.h header does not
    // need to include option.h.
	
	// For each file that should be recompiled, process it if it
	// has not already been dragged in by dependence.
	set<wstring> _file_with_eror_set;
	lookupEnvironment->RecordIncrementalRecompilationUnit(add_file_set, chang_file_set, remove_file_set, _file_with_eror_set);
	for (size_t j = 0; j < lookupEnvironment->input_files.size(); j++) {
		lookupEnvironment->ProcessFile(lookupEnvironment->input_files[j]);
	}
	lookupEnvironment->ReportSymstemErrorMsg();
	lookupEnvironment->cleanUp();

	return lookupEnvironment -> return_code;
    
}

/**
 * This method will be called for each error reported.
 */
void Compiler::reportError(JikesError *error)
{
    Coutput << error -> getErrorReport().c_str() << endl;
}


	void FileHelper::ProcessNewInputFiles(set<wstring>& file_set, char** arguments)
	{
		//
		// Process all file names specified in command line. By this point, only
		// filenames should remain in arguments - constructing the Option should
		// have filtered out all options and expanded @files.
		//
		if (arguments)
		{
			int j = 0;
			while (arguments[j])
			{
				char* file_name = arguments[j++];
				unsigned file_name_length = strlen(file_name);

				wchar_t* name = new wchar_t[file_name_length + 1];
				for (unsigned i = 0; i < file_name_length; i++)
					name[i] = (file_name[i] != U_BACKSLASH ? file_name[i]
						           : (wchar_t)U_SLASH); // Change '\' to '/'.
				name[file_name_length] = U_NULL;

				file_set.insert(wstring(name, name + file_name_length));

			}
		}
	}

	FileHelper::~FileHelper()
	{
		auto _size = assist_list.size();
		for (size_t i = 0; i < _size; ++i) {
			auto assist = assist_list[i];
			if (assist) {
				delete assist;
			}
		}
		SemanticError::DestoryStaticMessages();
	}

	/*
		 * Default file stat/reading and writting:
		 * The following provide the base classes and the
		 * default implamentations to read files from the file systems.
		 */
 FileHelper* FileHelper::getInstance(){
	static  FileHelper instance;
	return &instance;
}


 

 bool FileHelper::RegisterAssist(AssistFileHelper* asssis_helper){
	assist_list.push_back(asssis_helper);
	return true;
}

/**
 *  By default just ask the system for stat information.
 */


	bool FileHelper::LastModifiedTime(const char* fileName, time_t& m_time)
	{
		string temp;
	    if(fileName)
	    {
			temp = fileName;
			LastModifiedTime(temp, m_time);
	    }
		return false;
	}

	bool FileHelper::LastModifiedTime(const string& fileName, time_t& m_time)
	{
		
		auto _w_name = CharOperation::ConvertUtf8ToUnicode(fileName);
		boost::filesystem::path _path(_w_name);
		if (boost::filesystem::exists(_path) && boost::filesystem::is_regular_file(_path))
		{
			boost::system::error_code ec;
			m_time = boost::filesystem::last_write_time(_path , ec);
			return true;
		}
		else
			return false;
	}

	/**
	* By default return an object that reads from the file system.
	*/
FileReader *FileHelper::read(const char *filename)
{
	FileReader  *result;
	auto _size = assist_list.size();
	for (auto i = 0; i < _size; ++i) {
		result = assist_list[i]->read(filename);
		if (!result)
			continue;
		if (!result->getBuffer()) {
			delete result;
			result = NULL;
		}
		else {
			return result;
		}	
	}
	result = new DefaultFileReader(filename);

	// NB even if a file is empty (0 bytes)
	// This will return a pointer to 0 length array
	// and should not be NULL.
	if (result && (result->getBuffer() == NULL)){
		delete result;
		result = NULL;
	}
	return result;
	
}
FileReader* FileHelper::read(const std::wstring& _fileName)
{
	auto _utif_8 = CharOperation::ConvertUnicodeToUtf8(_fileName);
	return read(_utif_8.c_str());
}
FileReader* FileHelper::GetVirutalFile()
{
	return new VirtualFileReader();
}
//
// By Default return an object that reads from the file system.
//
FileWriter *FileHelper::write(const char *fileName, size_t bytes) 
{
	FileWriter *result;
	auto _size = assist_list.size();
	for (auto i = 0; i < _size; ++i) {
		result = assist_list[i]->write(fileName, bytes);
		if (!result)
			continue;

		if (!result->isValid()) {
			delete result;
			result = NULL;
		}
		else{
			return result;
		}
	}

	 result = new DefaultFileWriter(fileName, bytes);

	if (result && (!result->isValid())){
		delete result;
		result = NULL;
	}
	
	return result;
}

} // Close namespace Jikes block


