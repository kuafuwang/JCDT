// $Id: control.cpp,v 1.80 2004/12/12 04:35:52 elliott-oss Exp $
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 2004 IBM Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//
#include "stdafx.h"
#include "JCDT_Lib/internal/env/LookupEnvironment.h"
#include "JCDT_Lib/internal/lex/scanner.h"
#include "JCDT_Lib/internal/parser/parser.h"
#include  "JCDT_Lib/internal/semantic/semantic.h"
#include "JCDT_Lib/internal/problem/ErrorString.h"
#include  "JCDT_Lib/internal/codegen/bytecode.h"
#include "JCDT_Lib/internal/util/case.h"
#include "JCDT_Lib/internal/impl/option.h"
#include <boost/algorithm/string.hpp>
#include "JCDT_Lib/internal/env/INameEnvironment.h"
#include "JCDT_Lib/internal/impl/ITypeRequestor.h"
#include <JCDT_Lib/internal/lookup/FileSymbol.h>
#include <JCDT_Lib/internal/lookup/PackageSymbol.h>
#include <JCDT_Lib/internal/lookup/TypeSymbol.h>
#include <JCDT_Lib/internal/lookup/DirectoryEntry.h>
#include <JCDT_Lib/internal/lookup/DirectorySymbol.h>
#include <JCDT_Lib/internal/parser/diagnose/diagnose.h>


namespace Jikes { // Open namespace Jikes block

#ifdef JIKES_DEBUG
	int LookupEnvironment::input_files_processed = 0;
	int LookupEnvironment::class_files_read = 0;
	int LookupEnvironment::class_files_written = 0;
	int LookupEnvironment::line_count = 0;
#endif // JIKES_DEBUG

LookupEnvironment::LookupEnvironment(Option* option_, 
	INameEnvironment* nameEnv_ ,
	ITypeRequestor*  _typeRequestor)
    : return_code(0)
	, typeRequestor(_typeRequestor)
    , option(option_)
    , system_semantic(NULL)
    //, semantic(1024)
	//, needs_body_work(1024)
    //, type_trash_bin(1024)
    , input_java_file_set(1021)
    , input_class_file_set(1021)
    , expired_file_set()
    , recompilation_file_set(1021)
    // Type and method cache. These variables are assigned in control.h
    // accessors, but must be NULL at startup.
   // , Annotation_type(NULL)
    //, AssertionError_type(NULL)
  /*  , AssertionError_Init_method(NULL)
    , AssertionError_InitWithChar_method(NULL)
    , AssertionError_InitWithBoolean_method(NULL)
    , AssertionError_InitWithInt_method(NULL)
    , AssertionError_InitWithLong_method(NULL)
    , AssertionError_InitWithFloat_method(NULL)
    , AssertionError_InitWithDouble_method(NULL)
    , AssertionError_InitWithObject_method(NULL)*/
   // , Boolean_type(NULL)
   // , Boolean_TYPE_field(NULL)
   // , Byte_type(NULL)
   // , Byte_TYPE_field(NULL)
   // , Character_type(NULL)
 //   , Character_TYPE_field(NULL)
  //  , Class_type(NULL)
    //, Class_forName_method(NULL)
    //, Class_getComponentType_method(NULL)
    //, Class_desiredAssertionStatus_method(NULL)
   // , ClassNotFoundException_type(NULL)
    //, Cloneable_type(NULL)
    //, Comparable_type(NULL)
  //  , Double_type(NULL)
//    , Double_TYPE_field(NULL)
   // , ElementType_type(NULL)
  /*  , ElementType_TYPE_field(NULL)
	, ElementType_FIELD_field(NULL)
	, ElementType_METHOD_field(NULL)
	, ElementType_PARAMETER_field(NULL)
	, ElementType_CONSTRUCTOR_field(NULL)
	, ElementType_LOCAL_VARIABLE_field(NULL)
	, ElementType_ANNOTATION_TYPE_field(NULL)
	, ElementType_PACKAGE_field(NULL)*/
  //  , Enum_type(NULL)
  /*  , Enum_Init_method(NULL)
    , Enum_ordinal_method(NULL)
    , Enum_valueOf_method(NULL)*/
    //, Error_type(NULL)
   // , Exception_type(NULL)
    //, Float_type(NULL)
  //  , Float_TYPE_field(NULL)
   // , Integer_type(NULL)
   // , Integer_TYPE_field(NULL)
   // , Iterable_type(NULL)
   // , Iterable_iterator_method(NULL)
    //, Iterator_type(NULL)
   // , Iterator_hasNext_method(NULL)
   // , Iterator_next_method(NULL)
   // , Long_type(NULL)
  //  , Long_TYPE_field(NULL)
   // , NoClassDefFoundError_type(NULL)
   /* , NoClassDefFoundError_Init_method(NULL)
    , NoClassDefFoundError_InitString_method(NULL)*/
   // , Object_type(NULL)
  //  , Object_getClass_method(NULL)
    //, Overrides_type(NULL)
   // , Retention_type(NULL)
  //  , RetentionPolicy_type(NULL)
  /*  , RetentionPolicy_SOURCE_field(NULL)
    , RetentionPolicy_CLASS_field(NULL)
    , RetentionPolicy_RUNTIME_field(NULL)*/
  //  , RuntimeException_type(NULL)
   // , Serializable_type(NULL)
   // , Short_type(NULL)
  //  , Short_TYPE_field(NULL)
  //  , String_type(NULL)
  //  , StringBuffer_type(NULL)
   /* , StringBuffer_Init_method(NULL)
    , StringBuffer_InitWithString_method(NULL)
    , StringBuffer_toString_method(NULL)
    , StringBuffer_append_char_method(NULL)
    , StringBuffer_append_boolean_method(NULL)
    , StringBuffer_append_int_method(NULL)
    , StringBuffer_append_long_method(NULL)
    , StringBuffer_append_float_method(NULL)
    , StringBuffer_append_double_method(NULL)
    , StringBuffer_append_string_method(NULL)
    , StringBuffer_append_object_method(NULL)*/
   // , StringBuilder_type(NULL)
   /* , StringBuilder_Init_method(NULL)
    , StringBuilder_InitWithString_method(NULL)
    , StringBuilder_toString_method(NULL)
    , StringBuilder_append_char_method(NULL)
    , StringBuilder_append_boolean_method(NULL)
    , StringBuilder_append_int_method(NULL)
    , StringBuilder_append_long_method(NULL)
    , StringBuilder_append_float_method(NULL)
    , StringBuilder_append_double_method(NULL)
    , StringBuilder_append_string_method(NULL)
    , StringBuilder_append_object_method(NULL)*/
   // , Target_type(NULL)
  //  , Throwable_type(NULL)
  //  , Throwable_getMessage_method(NULL)
  //  , Throwable_initCause_method(NULL)
  //  , Void_type(NULL)
  //  , Void_TYPE_field(NULL)
    // Package cache.  unnamed and lang are initialized in constructor body.
    , annotation_package(NULL)
    , io_package(NULL)
    , util_package(NULL)
	, nameEnv(nameEnv_)
{

	if(option_->old_classpath_search_order == true){
		GetFile = GetFileFirst;
	}
	else{
		GetFile = GetFileBoth;
	}

	bad_value = &(nameEnv->bad_value);
	ProcessUnnamedPackage();
	
	ProcessSystemInformation();

	SemanticError::StaticInitializer();
	// Instantiate a scanner and a parser and initialize the static members
	// for the semantic processors.
	main_file_clone = nameEnv->GetVirtualFile();
	main_file_clone->MarkVirtualFile();


	LexStream* lex = new LexStream((Option*)option, main_file_clone);
	lex->ReadInput();
	main_file_clone->lex_stream = lex;

	main_file_clone->semantic = new Semantic(*this, main_file_clone);
	system_semantic = main_file_clone->semantic;
	system_semantic->MarkSystemSemantic();
	

#ifdef WIN32_FILE_SYSTEM
	if (option->BadMainDisk()){
		system_semantic->ReportSemErrorWithAst(SemanticError::NO_CURRENT_DIRECTORY,nullptr);
	}
#endif // WIN32_FILE_SYSTEM

	

	//
	// Require the existence of java.lang.
	//
	if (lang_package->directory.Length() == 0)
	{
		system_semantic->ReportSemErrorSimple(SemanticError::PACKAGE_NOT_FOUND,
			nullptr,
			StringConstant::US_java_SL_lang);
	}

	//
	// When the -d option is specified, create the relevant
	// directories if they don't already exist.
	//
	if (option->directory)
	{
		if (!SystemIsDirectory(option->directory))
		{
			for (char* ptr = option->directory; *ptr; ptr++)
			{
				char delimiter = *ptr;
				if (delimiter == U_SLASH)
				{
					*ptr = U_NULL;

					if (!SystemIsDirectory(option->directory))
						SystemMkdir(option->directory);

					*ptr = delimiter;
				}
			}

			SystemMkdir(option->directory);

			if (!SystemIsDirectory(option->directory))
			{
				int length = strlen(option->directory);
				wchar_t* name = new wchar_t[length + 1];
				for (int j = 0; j < length; j++)
					name[j] = option->directory[j];
				name[length] = U_NULL;
				system_semantic->ReportSemErrorWithAst(SemanticError::CANNOT_OPEN_DIRECTORY,
					nullptr, name);
				delete[] name;
			}
		}
	}
	unsigned i;
	for (i = 0; i < nameEnv_->bad_input_filenames.size(); i++)
	{
		system_semantic->ReportSemErrorWithAst(SemanticError::BAD_INPUT_FILE,
			nullptr, nameEnv_->bad_input_filenames[i].c_str());
	}

	for (i = 0; i < nameEnv_->unreadable_input_filenames.size(); i++)
	{
		system_semantic->ReportSemErrorWithAst(SemanticError::UNREADABLE_INPUT_FILE,
			nullptr, nameEnv_->unreadable_input_filenames[i].c_str());
	}

#ifdef FOR_CONSOLE
	if (system_semantic->NumErrors() > 0)
	{
		system_semantic->PrintMessages();
		return_code = system_semantic->return_code;
	}
#endif

}
NameSymbol* LookupEnvironment::MakeParameter(int num)
{
	return nameEnv->MakeParameter(num);
}
void LookupEnvironment::cleanUp()
{
    // Clean up all the files that have just been compiled in
    // this new batch.

	size_t size = input_files.size();
	for(size_t i  = 0 ; i < size; ++ i )
	{
		auto file_symbol = input_files[i];
		if(input_java_file_set.IsElement(input_files[i])){
			continue;
		}
		else{
			CleanUp(file_symbol);
		}
	}
	input_files.clear();
	FileSymbol* file_symbol;
    for (file_symbol = (FileSymbol*) input_java_file_set.FirstElement();
         file_symbol;
         file_symbol = (FileSymbol*) input_java_file_set.NextElement())
    {
        CleanUp(file_symbol);
    }

	// The depend flag should only be in effect in the first pass
	if (option->depend)
		option->depend = false;  

    // Are we supposed to generate Makefiles?
    if (option->makefile){
		Makefile();
    } 
}

void LookupEnvironment::ReportSymstemErrorMsg() const
{
	unsigned i = 0;
	unsigned _len = nameEnv->bad_dirnames.Length();
	if (_len) {
		for (i = 0; i < _len; i++)
		{
			system_semantic->ReportSemErrorSimple(SemanticError::CANNOT_OPEN_PATH_DIRECTORY, BAD_TOKEN,
				nameEnv->bad_dirnames[i]);
			delete[] nameEnv->bad_dirnames[i];
		}
		nameEnv->bad_dirnames.Reset();
	}

	_len = nameEnv->bad_zip_filenames.Length();
	if (_len) {
		for (i = 0; i < _len; i++)
		{
			system_semantic->ReportSemErrorSimple(SemanticError::CANNOT_OPEN_ZIP_FILE,
				BAD_TOKEN, nameEnv->bad_zip_filenames[i]);
			delete[] nameEnv->bad_zip_filenames[i];
		}
		nameEnv->bad_zip_filenames.Reset();
	}

	_len = nameEnv->general_io_warnings.Length();
	if (_len) {
		for (i = 0; i < _len; i++)
		{
			system_semantic->ReportSemErrorSimple(SemanticError::IO_WARNING, BAD_TOKEN,
				nameEnv->general_io_warnings[i]);
			delete[] nameEnv->general_io_warnings[i];
		}
		nameEnv->general_io_warnings.Reset();
	}

	_len = nameEnv->general_io_errors.Length();
	if (_len) {
		for (i = 0; i < nameEnv->general_io_errors.Length(); i++)
		{
			system_semantic->ReportSemErrorSimple(SemanticError::IO_ERROR, BAD_TOKEN,
				nameEnv->general_io_errors[i]);
			delete[] nameEnv->general_io_errors[i];
		}
		nameEnv->general_io_errors.Reset();

	}


	_len = nameEnv->bad_input_filenames.size();
	if (_len) {
		for (i = 0; i < _len; i++) {
			system_semantic->ReportSemErrorWithAst(SemanticError::BAD_INPUT_FILE,
				nullptr, nameEnv->bad_input_filenames[i].c_str());
		}
		nameEnv->bad_input_filenames.clear();
	}

	_len = nameEnv->unreadable_input_filenames.size();
	if (_len) {
		for (i = 0; i < nameEnv->unreadable_input_filenames.size(); i++) {
			system_semantic ->
				ReportSemErrorWithAst(SemanticError::UNREADABLE_INPUT_FILE,
					nullptr, nameEnv->unreadable_input_filenames[i].c_str());
		}
		nameEnv->unreadable_input_filenames.clear();
	}
#ifdef FOR_CONSOLE
	// If any system error or warning was detected, print it...
	system_semantic->PrintMessages();
	if (system_semantic->return_code > 0 ||
		bad_input_filenames.size() > 0 ||
		unreadable_input_filenames.size() > 0)
	{
		return_code = 1;
	}
#endif

}
void LookupEnvironment::Makefile()
{
	unsigned i = 0;
	if (option->dependence_report)
	{
		FILE* outfile = SystemFopen(option->dependence_report_name,
			"w");
		if (outfile == NULL)
			Coutput << "*** Cannot open dependence output file "
			<< option->dependence_report_name << endl;
		else
		{
			SymbolSet types_in_new_files;
			FileSymbol* file_symbol;
			for (file_symbol = (FileSymbol*)input_java_file_set.FirstElement();
			file_symbol;
				file_symbol = (FileSymbol*)input_java_file_set.NextElement())
			{
				const char* java_name = file_symbol->FileName();

				for (i = 0; i < file_symbol->types.size(); i++)
				{
					TypeSymbol* type = file_symbol->types[i];
					fprintf(outfile, "%s : %s\n", java_name,
						type->SignatureString());

					TypeSymbol* static_parent;
					for (static_parent = (TypeSymbol*)type->static_parents->FirstElement();
					static_parent;
						static_parent = (TypeSymbol*)type->static_parents->NextElement())
					{
						if (!type->parents ->
							IsElement(static_parent))
						{
							// Only a static ref to static_parent?
							fprintf(outfile, "   !%s\n",
								static_parent->SignatureString());

							//
							// If the type is contained in a type that
							// is not one of the input files, save it
							//
							if (static_parent->file_symbol &&
								static_parent->file_symbol->IsClass())
							{
								types_in_new_files.AddElement(static_parent);
							}
						}
					}

					TypeSymbol* parent;
					for (parent = (TypeSymbol*)type->parents->FirstElement();
					parent;
						parent = (TypeSymbol*)type->parents->NextElement())
					{
						fprintf(outfile, "    %s\n",
							parent->SignatureString());

						//
						// If the type is contained in a type that is
						// not one of the input files, save it
						//
						if (parent->file_symbol &&
							parent->file_symbol->IsClass())
						{
							types_in_new_files.AddElement(parent);
						}
					}
				}
			}

			//
			// Print the list of class files that are referenced.
			//
			TypeSymbol* type;
			for (type = (TypeSymbol*)types_in_new_files.FirstElement();
			type;
				type = (TypeSymbol*)types_in_new_files.NextElement())
			{
				const char* class_name = type->file_symbol->FileName();
				fprintf(outfile, "%s : %s\n", class_name,
					type->SignatureString());
			}

			fclose(outfile);
		}
	}
	else
	{
		SymbolSet* candidates =
			new SymbolSet(input_java_file_set.Size() +
				input_class_file_set.Size());
		*candidates = input_java_file_set;
		candidates->Union(input_class_file_set);

		TypeDependenceChecker* dependence_checker =
			new TypeDependenceChecker(this, *candidates,
				type_trash_bin);
		dependence_checker->PartialOrder();
		dependence_checker->OutputDependences();
		delete dependence_checker;

		delete candidates;
	}
}
LookupEnvironment::~LookupEnvironment()
{
	main_file_clone->CleanUp();
#ifdef JIKES_DEBUG
    if (option->debug_dump_lex || option->debug_dump_ast ||
        option->debug_unparse_ast)
    {
        Coutput << line_count << " source lines read" << endl
                << class_files_read << " \".class\" files read" << endl
                << class_files_written << " \".class\" files written" << endl
                << input_files_processed << " \".java\" files processed"
                << endl;
    }
#endif // JIKES_DEBUG


	delete nameEnv;
	nameEnv = nullptr;

	delete option;
	option = nullptr;
}



	
	/* Cache the binary type since we know it is needed during this compile.
	*
	* Answer the created BinaryTypeBinding or null if the type is already in the cache.
	*/

	TypeSymbol* LookupEnvironment::getType( string compoundTypeName) {
		boost::replace_all(compoundTypeName, L".", L"/");
		boost::replace_all(compoundTypeName, L"$", L"/");

		wstring compoundPackageName;
		auto pos = compoundTypeName.rfind('/');
		string typeName;
		if (pos != string::npos){
			compoundPackageName = CharOperation::ConvertUtf8ToUnicode(compoundTypeName.substr(0, pos));
			typeName = compoundTypeName.substr(pos + 1, compoundTypeName.size());
		}
		else
		{
			compoundPackageName = CharOperation::ConvertUtf8ToUnicode(compoundTypeName);
			typeName = compoundTypeName;
		}
		auto pkg = ProcessPackage(compoundPackageName.c_str());
		if (!pkg)
			return no_type;
		return ProcessSystemType(pkg, typeName.c_str());
	}


PackageSymbol* LookupEnvironment::ProcessPackage(const wchar_t* name)
{
    int name_length = wcslen(name);
    wchar_t* package_name = new wchar_t[name_length];
    int length;
    for (length = 0;
         length < name_length && name[length] != U_SLASH; length++)
    {
         package_name[length] = name[length];
    }
    NameSymbol* name_symbol = nameEnv->FindOrInsertName(package_name, length);

    PackageSymbol* package_symbol =
        external_table.FindPackageSymbol(name_symbol);
    if (! package_symbol)
    {
        package_symbol = external_table.InsertPackageSymbol(name_symbol, NULL);
		nameEnv->FindPathsToDirectory(package_symbol);
    }

    while (length < name_length)
    {
        int start = ++length;
        for (int i = 0;
             length < name_length && name[length] != U_SLASH;
             i++, length++)
        {
             package_name[i] = name[length];
        }
        name_symbol = nameEnv->FindOrInsertName(package_name, length - start);
        PackageSymbol* subpackage_symbol =
            package_symbol -> FindPackageSymbol(name_symbol);
        if (! subpackage_symbol)
        {
            subpackage_symbol =
                package_symbol -> InsertPackageSymbol(name_symbol);
            nameEnv->FindPathsToDirectory(subpackage_symbol);
        }
        package_symbol = subpackage_symbol;
    }

    delete [] package_name;
    return package_symbol;
}




PackageSymbol* LookupEnvironment::FindOrInsertPackage(LexStream* lex_stream,
                                            AstName* name)
{
    PackageSymbol* package;

    if (name -> base_opt)
    {
        package = FindOrInsertPackage(lex_stream, name -> base_opt);
        NameSymbol* name_symbol =
			name->identifier_token->GetNameSymbol();
        PackageSymbol* subpackage = package -> FindPackageSymbol(name_symbol);
        if (! subpackage)
            subpackage = package -> InsertPackageSymbol(name_symbol);
        package = subpackage;
    }
    else
    {
        NameSymbol* name_symbol =
            name -> identifier_token->GetNameSymbol();
        package = external_table.FindPackageSymbol(name_symbol);
        if (! package)
            package = external_table.InsertPackageSymbol(name_symbol, NULL);
    }

	nameEnv->FindPathsToDirectory(package);
    return package;
}

void LookupEnvironment::ProcessFile(FileSymbol* file_symbol)
{
	if (input_java_file_set.IsElement(file_symbol))
		return;

    ProcessFileHeaders(file_symbol);
	ProcessFileBodys();

}
void LookupEnvironment::ProcessFileBodys()
{
	//
	// As long as there are new bodies, ...
	//
	for (unsigned i = 0; i < needs_body_work.size(); i++)
	{
		assert(semantic.size() == 0);

		// These bodies are not necessarily in file_symbol; they
		// might be in another FileSymbol used by file_symbol.
		ProcessBodies(needs_body_work[i]);
	}
	needs_body_work.clear();
}



void LookupEnvironment::ProcessFileHeaders(FileSymbol* file_symbol)
{
	if (input_java_file_set.IsElement(file_symbol))
		return;
    if (file_symbol -> semantic)
        return;
    input_java_file_set.AddElement(file_symbol);

	file_symbol->initial_invocation = (semantic.size() == 0);

    if (option->verbose)
    {
        Coutput << "[read "
                << file_symbol -> FileName()
                << "]" << endl;
    }

  	
    if (typeRequestor->accept(file_symbol))
    {
        assert(! file_symbol -> semantic);

        if (! file_symbol -> package)
            ProcessPackageDeclaration(file_symbol,file_symbol -> compilation_unit);

		file_symbol -> semantic = new Semantic(*this, file_symbol);
        semantic.push_back( file_symbol -> semantic);
        file_symbol -> semantic -> ProcessTypeNames();
    }
    

    if (file_symbol->initial_invocation)
    {
		ProcessMembers(file_symbol);
    }
        
}



	void LookupEnvironment::ProcessMembers(FileSymbol* file_symbol)
{
	if(!file_symbol->initial_invocation)
	{
		assert(false);
		return;
	}
    vector<TypeSymbol*> partially_ordered_types;
    SymbolSet needs_member_work(101);
    TypeCycleChecker cycle_checker(partially_ordered_types);
    TopologicalSort topological_sorter(needs_member_work,
                                       partially_ordered_types);

    unsigned start = 0;
    while (start < semantic.size())
    {
        needs_member_work.SetEmpty();

        do
        {
            //
            // Check whether or not there are cycles in this new batch of
            // types. Create a partial order of the types (cycles are ordered
            // arbitrarily) and place the result in partially_ordered_types.
            //
            cycle_checker.PartialOrder(semantic, start);
            start = semantic.size(); // next starting point

            //
            // Process the extends and implements clauses.
            //
            for (unsigned j = 0; j < partially_ordered_types.size(); j++)
            {
                TypeSymbol* type = partially_ordered_types[j];
                needs_member_work.AddElement(type);
                type -> ProcessTypeHeaders();
                type -> semantic_environment -> sem ->
                    types_to_be_processed.AddElement(type);
            }
        } while (start < semantic.size());

        //
        // Partially order the collection of types in needs_member_work and
        // place the result in partially_ordered_types. This reordering is
        // based on the complete "supertype" information computed in
        // ProcessTypeHeaders.
        //
        topological_sorter.Sort();
        for (unsigned i = 0; i < partially_ordered_types.size(); i++)
        {
            TypeSymbol* type = partially_ordered_types[i];
			//needs_body_work.Next() = type;
			needs_body_work.push_back(type);
            type -> ProcessMembers();
        }
    }

    semantic.clear();
	file_symbol->initial_invocation = false;
}


void LookupEnvironment::CollectTypes(TypeSymbol* type, vector<TypeSymbol*>& types)
{
    types.push_back(type);

    for (unsigned j = 0; j < type -> NumAnonymousTypes(); j++)
        CollectTypes(type -> AnonymousType(j), types);

    if (type -> local)
    {
        for (TypeSymbol* local_type = (TypeSymbol*) type -> local -> FirstElement();
             local_type;
             local_type = (TypeSymbol*) type -> local -> NextElement())
        {
            CollectTypes(local_type, types);
        }
    }

    if (type -> non_local)
    {
        for (TypeSymbol* non_local_type = (TypeSymbol*) type -> non_local -> FirstElement();
             non_local_type;
             non_local_type = (TypeSymbol*) type -> non_local -> NextElement())
        {
            CollectTypes(non_local_type, types);
        }
    }
}


void LookupEnvironment::ProcessBodies(TypeSymbol* type)
{
    Semantic* sem = type -> semantic_environment -> sem;

    if (type -> declaration &&
        ! CompilationUnitDeclaration::BadCompilationUnitCast(sem->compilation_unit))
    {
#ifdef WIN32_FILE_SYSTEM
        if (! type -> file_symbol -> IsZip())
        {
            

            DirectorySymbol* directory =
                type -> file_symbol -> OutputDirectory();
			if(directory)
			{
				int length = type->Utf8NameLength() +
					FileSymbol::class_suffix_length;
				char* classfile_name = new char[length + 1]; // +1 for "\0"
				strcpy(classfile_name, type->Utf8Name());
				strcat(classfile_name, FileSymbol::class_suffix);
				DirectoryEntry* entry =
					directory->FindCaseInsensitiveEntry(classfile_name, length);

				//
				// If an entry is found and it is not identical (in a
				// case-sensitive test) to the name of the type, issue an
				// appropriate message.
				//
				if (entry && strcmp(classfile_name, entry->name) != 0)
				{
					wchar_t* entry_name = new wchar_t[entry->length + 1];
					for (int i = 0; i < length; i++)
						entry_name[i] = entry->name[i];
					entry_name[entry->length] = U_NULL;
					sem->ReportSemError(SemanticError::FILE_FILE_CONFLICT,
						type->declaration->identifier_token,
						type->declaration->identifier_token,
						type->Name(), entry_name,
						directory->Name());
					delete[] entry_name;
				}
				delete[] classfile_name;
			}
         
            
        }
#endif // WIN32_FILE_SYSTEM

        if (!type -> declaration->initializerParseBad){
            type -> CompleteSymbolTable();
            if (!type->declaration->bodyParseBad){
				type->ProcessExecutableBodies();
            }
			else {
				// Mark that syntax errors were detected.
				//sem -> compilation_unit -> MarkBad();
			}
          
        }
		else
		{
			// Mark that syntax errors were detected.
			// sem -> compilation_unit -> MarkBad();  已经标记过了
		}
	
        if (option->bytecode && sem->NumErrors() == 0 &&
			sem->source_file_symbol->parser_error->NumErrors() == 0 &&
            !CompilationUnitDeclaration::BadCompilationUnitCast(sem->compilation_unit))
        {
            vector<TypeSymbol*>* types = new vector<TypeSymbol*>();
			types->reserve(1024);
            CollectTypes(type, *types);

            // If we are supposed to generate code, do so now !!!
            {
                for (unsigned k = 0; k < types -> size(); k++)
                {
                    TypeSymbol* type = (*types)[k];
                    // Make sure the literal is available for bytecode.
                    type -> file_symbol -> SetFileNameLiteral(this);
                    ByteCode* code = new ByteCode(type);
                    code -> GenerateCode(Coutput);
                    delete code;
                }
            }

            //
            // If no error was detected while generating code, then
            // start cleaning up.
            //
         /*   if (! option->nocleanup)
            {
                if (sem -> NumErrors() == 0)
                {
                    for (unsigned k = 0; k < types -> Length(); k++)
                    {
                        TypeSymbol* type = (*types)[k];
                        delete type -> semantic_environment;
                        type -> semantic_environment = NULL;
                        type -> declaration -> semantic_environment = NULL;
                    }
                }
            }*/
			delete types;
        }
    }

    sem -> types_to_be_processed.RemoveElement(type);

    if (sem -> types_to_be_processed.Size() == 0)
    {
        // All types belonging to this compilation unit have been processed.
        CheckForUnusedImports(sem);
       /* if (! option->nocleanup)
        {
            CleanUp(sem -> source_file_symbol);
        }*/
    }
}

void LookupEnvironment::CheckForUnusedImports(Semantic* sem)
{
    if (sem -> NumErrors() != 0 ||
        sem -> lex_stream -> NumErrorTokens() != 0 ||
        CompilationUnitDeclaration::BadCompilationUnitCast(sem->compilation_unit))
    {
        //
        // It's not worth checking for unused imports if compilation
        // wasn't successful; we may well have just not got round to
        // compiling the relevant code, and if there were errors, the
        // user has more important things to worry about than unused
        // imports!
        //
        return;
    }

    for (unsigned i = 0;
         i < sem -> compilation_unit -> NumImportDeclarations(); ++i)
    {
        AstImportDeclaration* import_declaration =
            sem -> compilation_unit -> ImportDeclaration(i);
        Symbol* symbol = import_declaration -> name -> symbol;
        if (import_declaration -> star_token_opt)
        {
            PackageSymbol* package = PackageSymbol::PackageCast(symbol);
            if (package &&
                ! sem -> referenced_package_imports.IsElement(package))
            {
                sem -> ReportSemErrorWithAst(SemanticError::UNUSED_PACKAGE_IMPORT,
                                      import_declaration,
                                      package -> PackageName());
            }
        }
        else
        {
            TypeSymbol* import_type = TypeSymbol::TypeCast(symbol);
            if (import_type &&
                ! sem -> referenced_type_imports.IsElement(import_type))
            {
                sem -> ReportSemErrorWithAst(SemanticError::UNUSED_TYPE_IMPORT,
                                      import_declaration,
                                      import_type -> ContainingPackage() -> PackageName(),
                                      import_type -> ExternalName());
            }
        }
    }
}

//
// Introduce the main package and the current package.
// This procedure is invoked directly only while doing
// an incremental compilation.
//
void LookupEnvironment::ProcessPackageDeclaration(FileSymbol* file_symbol,
	CompilationUnitDeclaration* compilation_unit)
{
	if (!compilation_unit){
		return;
	}
	auto lex_stream =  file_symbol->lex_stream;
	auto package_declaration = compilation_unit->package_declaration_opt;
    file_symbol -> package = (package_declaration
                              ? FindOrInsertPackage(file_symbol -> lex_stream,
								  package_declaration-> name)
                              : unnamed_package);
	auto size = compilation_unit->NumTypeDeclarations();

	for (unsigned k = 0; k < size; k++)
	{
		TypeDeclaration* declaration = compilation_unit->GetTypeDeclaration(k);
		if (!EmptyTypeDeclaration::EmptyDeclarationCast(declaration))
		{
			NameSymbol* name_symbol = declaration->class_body->identifier_token->GetNameSymbol();
			if (!file_symbol->package->FindTypeSymbol(name_symbol))
			{
				TypeSymbol* type = file_symbol->package ->
					InsertOuterTypeSymbol(name_symbol);
				type->file_symbol = file_symbol;
				type->outermost_type = type;
				type->supertypes_closure = new SymbolSet;
				type->SetOwner(file_symbol->package);
				type->SetSignature(*this);
				type->MarkSourcePending();

				//
				// If this type is contained in the unnamed package add it to
				// the set unnamed_package_types if a type of similar name was
				// not already there.
				//
				if (!package_declaration &&
					unnamed_package_types.Image(type->Identity()) == NULL)
				{
					unnamed_package_types.AddElement(type);
				}
			}
		}

	}
	
	


}


void LookupEnvironment::CleanUp(FileSymbol* file_symbol)
{
	file_symbol->CleanUp();
    Semantic* sem = file_symbol -> semantic;
    if (sem)
    {
#ifdef JIKES_DEBUG
        if (option->debug_dump_lex)
        {
            sem -> lex_stream -> Reset(); // rewind input and ...
            sem -> lex_stream -> Dump();  // dump it!
        }
        if (option->debug_dump_ast)
            sem -> compilation_unit -> Print(Coutput,*sem -> lex_stream);
        if (option->debug_unparse_ast)
        {
            if (option->debug_unparse_ast_debug)
              {
                // which of these is correct?
                sem -> compilation_unit -> debug_unparse = true;
                Ast::debug_unparse = true;
              }
           // sem -> compilation_unit -> Unparse(sem -> lex_stream,"unparsed/");
        }
#endif // JIKES_DEBUG

#ifdef FOR_CONSOLE
        sem -> PrintMessages(); 
#endif
		
        if (sem -> return_code > 0)
            return_code = 1;

		
		
    }
}



} // Close namespace Jikes block


