#include "JCDT_Lib/internal/env/platform.h"
#include "JCDT_Lib/internal/lookup/FileSymbol.h"
#include "JCDT_Lib/internal/lookup/PathSymbol.h"
#include "JCDT_Lib/internal/lookup/DirectorySymbol.h"
#include <JCDT_Lib/internal/env/LookupEnvironment.h>
#include <JCDT_Lib/internal/env/INameEnvironment.h>
#include <JCDT_Lib/internal/semantic/semantic.h>
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/problem/ParseError.h>
#include  <boost/algorithm/string.hpp>


namespace Jikes { // Open namespace Jikes block


	const	char* FileSymbol::FileName()
	{
		if (!file_name)
			SetFileName();
		return file_name;
	}
	unsigned FileSymbol::FileNameLength()
	{
		if (!file_name)
			SetFileName();
		return file_name_length;
	}

	
	bool FileSymbol::IsZip()
	{
		return PathSym()->IsZip();
	}
	Zip* FileSymbol::Zipfile()
	{
		return PathSym()->zipfile;
	}

	wstring FileSymbol::GetUnicodeFileName()
	{
		if (!file_name)
			SetFileName();
		if (file_name)
			return CharOperation::ConvertUtf8ToUnicode(file_name);
		else
			return{};
	}

	PathSymbol* FileSymbol::PathSym()
	{
		return directory_symbol->PathSym();
	}

	DirectorySymbol* FileSymbol::OutputDirectory()
	{
		return output_directory ? output_directory
			: output_directory = LookupEnvironment::GetOutputDirectory(this);
	}


	FileSymbol::~FileSymbol()
	{
		delete[] file_name;
		delete lex_stream;
		delete parser_error;
		for(auto it : types){
			if(it){
				it->file_symbol = nullptr;
			}
		}
	}


	void FileSymbol::SetFileName()
	{
		PathSymbol* path_symbol = PathSym();
		auto directory_name = directory_symbol->DirectoryName();
		size_t directory_name_length = directory_symbol->DirectoryNameLength();
		bool dot_directory = (strcmp(directory_name, ".") == 0);
		file_name_length = (dot_directory ? 0 : directory_name_length) +
			Utf8NameLength() +
			(path_symbol->IsZip() ? 2 // For zip files, we need "()";
				: (dot_directory || // for regular directory, we need 1 '/'
					directory_name[directory_name_length - 1] == U_SLASH ? 0 : 1)) +
			(kind == JAVA ? java_suffix_length : class_suffix_length);

		file_name = new char[file_name_length + 1]; // +1 for '\0'
		if (dot_directory)
			file_name[0] = U_NULL;
		else
		{
			strcpy(file_name, directory_symbol->DirectoryName());
			if (path_symbol->IsZip())
				strcat(file_name, StringConstant::U8S_LP);
			else if (directory_name[directory_name_length - 1] != U_SLASH)
				strcat(file_name, StringConstant::U8S_SL);
		}
		strcat(file_name, Utf8Name());
		strcat(file_name, kind == JAVA ? FileSymbol::java_suffix
			: FileSymbol::class_suffix);
		if (path_symbol->IsZip())
			strcat(file_name, StringConstant::U8S_RP);

		assert(strlen(file_name) == file_name_length);
	}

	void FileSymbol::SetFileName(const char* _name, unsigned _length)
	{
		delete[] file_name;
	
		file_name = new char[_length + 1]; // +1 for '\0'
		file_name[_length] = 0x00;
		file_name_length = _length;
		memcpy((void*)_name,file_name, _length);
	}

	void FileSymbol::SetFileNameLiteral(LookupEnvironment* control)
	{
		if (!file_name_literal)
		{
			const char* file_name = FileName();

			int i;
			for (i = FileNameLength() - 1; i >= 0; i--)
			{
				if (file_name[i] == U_SLASH)
					break;
			}

			int file_name_start = i + 1;
			int file_name_length = FileNameLength() - file_name_start;
			file_name_literal =
				control->nameEnv->Utf8_pool.FindOrInsert(file_name + file_name_start,
					file_name_length);
		}
	}


	void FileSymbol::CleanUp()
	{

		for(auto it : types){
			it->CleanUpType();
		}

		if (semantic) {
			delete semantic;
			semantic = NULL;
		}

		delete lex_stream;
		lex_stream = NULL;

		if (compilation_unit){
			auto pool = compilation_unit->ast_pool;
			compilation_unit->ast_pool = nullptr;
			delete pool;		
			compilation_unit = NULL;
		}

		if(parser_error){
			delete parser_error;
			parser_error = nullptr;
		}
		package = nullptr;
	}

	void FileSymbol::TransferError(vector<JikesError*>& future_owner)
	{
		if (lex_stream) {
			if (lex_stream->NumBadTokens())
				lex_stream->TransferError(future_owner);
		}

		if (parser_error) {
			if(parser_error->NumErrors())
				parser_error->TransferError(future_owner,lex_stream);
		}
		if (semantic) {
			if(semantic->NumIssues())
				semantic->TransferError(future_owner);
		}
	}
	void FileSymbol::GetImportPackgeNames(vector<wstring>& import_packageNames)
	{
		if( !compilation_unit){
			return;
		}
	
		unsigned num = compilation_unit->NumImportDeclarations();
		for (unsigned i = 0; i < num; i++){
			auto _import_decl =  compilation_unit->ImportDeclaration(i);
			auto compoundName = _import_decl->getImportName();
			import_packageNames.push_back(compoundName);
			
		}
		
	}
	unsigned FileSymbol::NumErrors() const
	{
		unsigned num = 0;
		if(parser_error){
			num += (parser_error->NumErrors());
		}
		if(semantic){
			num += semantic->NumIssues();
		}
		if(lex_stream){
			num += lex_stream->NumBadTokens();
		}
		return num;
	}

	void FileSymbol::RelieveRelathioshipWithType(TypeSymbol* _type)
	{
		auto it = types.begin();
		auto end = types.end();
		for(;it != end;++it)
		{
			if( *it == _type)
			{
				types.erase(it);
				return;
			}
		}
	}

	void FileSymbol::getTypesInfo(set<wstring>& path_set, vector<TypeHeaderInfo*>& type_header_infos)
	{
		for (auto it : types) {
			it->getTypesInfo(path_set, type_header_infos);
		}

	}

	FileSymbol::FileSymbol(const NameSymbol* name_symbol_)
		: output_directory(NULL)
		, file_name(NULL), file_name_length(0)
		  , file_name_literal(NULL)
		, name_symbol(name_symbol_)
		, directory_symbol(NULL)
		, package(NULL)
		, compilation_unit(NULL)
		, semantic(NULL)
	{
		types.reserve(4);
		Symbol::_kind = _FILE;
	}

	void FileSymbol::RemoveClassFileFromOutputDirWhenInJavaFile()
	{
		if(IsJava())
		{
			size_t size = types.size();
			for (size_t i = 0; i < size; ++i) {
				types[i]->RemoveClassFileFromOutputDirWhenInJavaFile();
			}
		}

	}
	


} // Close namespace Jikes block



