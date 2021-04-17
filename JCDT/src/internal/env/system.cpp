// $Id: system.cpp,v 1.64 2004/12/12 04:35:52 elliott-oss Exp $
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 2004 IBM Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#include "JCDT_Lib/internal/env/platform.h"
#include  "JCDT_Lib/internal/semantic/semantic.h"
#include "JCDT_Lib/internal/util/zip/zip.h"
#include "JCDT_Lib/internal/impl/option.h"
#include "JCDT_Lib/internal/util/case.h"
#include <JCDT_Lib/internal/env/LookupEnvironment.h>
#include "JCDT_Lib/internal/env/INameEnvironment.h"
#include <JCDT_Lib/internal/problem/ErrorString.h>

#include <JCDT_Lib/internal/lookup/PathSymbol.h>
#include <JCDT_Lib/internal/lookup/DirectoryEntry.h>
#include <JCDT_Lib/internal/lookup/DirectorySymbol.h>

namespace Jikes { // Open namespace Jikes block





//
// Create the unnamed package and set up global names.
//
void LookupEnvironment::ProcessUnnamedPackage()
{
    unnamed_package = external_table
        .InsertPackageSymbol(nameEnv->FindOrInsertName(US_EMPTY, 0), NULL);

	//unnamed_package->directory.Resize();
	auto _len = nameEnv->unnamed_package_dirs.Length();
	for (size_t i = 0; i < _len; ++i) {
		unnamed_package->directory.Next() = nameEnv->unnamed_package_dirs[i];
	}
    // Create an entry for no_type. no_type is used primarily to signal an
    // error.
    //
    no_type = unnamed_package -> InsertSystemTypeSymbol(nameEnv->question_name_symbol);
    no_type -> SetSignature(nameEnv->dot_name_symbol -> Utf8_literal);
    no_type -> outermost_type = no_type;
    no_type -> SetOwner(unnamed_package);
    no_type -> MarkBad();

    //
    // Create an entry for the null type.
    //
    null_type = unnamed_package -> InsertSystemTypeSymbol(nameEnv->null_name_symbol);
    null_type -> outermost_type = null_type;
    null_type -> SetOwner(unnamed_package);
    null_type -> SetACC_PUBLIC();
}


TypeSymbol* LookupEnvironment::GetPrimitiveType(const char* name, char signature)
{
    NameSymbol* name_symbol = nameEnv->FindOrInsertSystemName(name);
    TypeSymbol* type = unnamed_package -> InsertSystemTypeSymbol(name_symbol);
    char sig[2] = { signature, U_NU };
    type -> SetSignature(nameEnv->Utf8_pool.FindOrInsert(sig, 1));
    type -> outermost_type = type;
    type -> SetOwner(            unnamed_package);
    type -> SetACC_PUBLIC();
    type -> MarkPrimitive();
    return type;
}


void LookupEnvironment::ProcessSystemInformation()
{
    //
    // Add entry for system package
    //
    lang_package = ProcessPackage(US_java_SL_lang);

    //
    // Create an entry for each primitive type. Note that the type void is
    // treated as a primitive. We do not set up any subtyping relationships,
    // as that would violate the assumptions made elsewhere.
    //
    void_type = GetPrimitiveType("void", U_V);
    boolean_type = GetPrimitiveType("boolean", U_Z);
    byte_type = GetPrimitiveType("byte", U_B);
    char_type = GetPrimitiveType("char", U_C);
    short_type = GetPrimitiveType("short", U_S);
    int_type = GetPrimitiveType("int", U_I);
    long_type = GetPrimitiveType("long", U_J);
    float_type = GetPrimitiveType("float", U_F);
    double_type = GetPrimitiveType("double", U_D);
}


//
// Find the given system type.
//
TypeSymbol* LookupEnvironment::ProcessSystemType(PackageSymbol* package,
                                       const char* name)
{
    NameSymbol* name_symbol = nameEnv->FindOrInsertSystemName(name);
    TypeSymbol* type = package -> FindTypeSymbol(name_symbol);

    if (! type)
    {
        FileSymbol* file_symbol = GetFile(option, package, name_symbol);
		
        type = system_semantic -> ReadType(file_symbol, package,
                                           name_symbol, 0);

    }
    else if (type -> SourcePending())
        ProcessFileHeaders(type -> file_symbol);

	if(!type)
	{
		type = no_type;
	}
    return type;
}


//
// Find the given system method.
//
MethodSymbol* LookupEnvironment::ProcessSystemMethod(TypeSymbol* type,
                                           const char* name,
                                           const char* descriptor)
{
    NameSymbol* name_symbol = nameEnv->FindOrInsertSystemName(name);
    MethodSymbol* method = NULL;
    if (! type -> Bad())
    {
        for (method = type -> FindMethodSymbol(name_symbol);
             method; method = method -> next_method)
        {
            if (! strcmp(descriptor, method -> SignatureString()))
                break;
        }
    }
    if (! method)
    {
        if (! type -> Bad())
        {
          /*  system_semantic ->
                ReportSemErrorWithAst(SemanticError::NON_STANDARD_LIBRARY_TYPE,
                               nullptr, nullptr,type -> ContainingPackageName(),
                               type -> ExternalName());*/
            return NULL;
        }
        method = type -> InsertMethodSymbol(name_symbol);
        method -> SetType(no_type);
        method -> SetContainingType(type);
        method -> SetSignature(nameEnv->FindOrInsertSystemName(descriptor) ->
                               Utf8_literal);
    }
    return method;
}


//
// Find the given system field.
//
VariableSymbol* LookupEnvironment::ProcessSystemField(TypeSymbol* type,
                                            const char* name,
                                            const char* descriptor)
{
    NameSymbol* name_symbol = nameEnv->FindOrInsertSystemName(name);
    VariableSymbol* field = NULL;
    if (! type -> Bad())
    {
        field = type -> FindVariableSymbol(name_symbol);
        if (! field -> IsTyped())
            field -> ProcessVariableSignature(system_semantic, BAD_TOKEN);
        field -> MarkInitialized();
    }
    if (! field)
    {
        if (! type -> Bad())
        {
          /*  system_semantic ->
                ReportSemErrorWithAst(SemanticError::NON_STANDARD_LIBRARY_TYPE,
                               nullptr, type -> ContainingPackageName(),
                               type -> ExternalName());*/
            return NULL;
        }
        field = type -> InsertVariableSymbol(name_symbol);
        field -> SetType(no_type);
        field -> SetOwner(type);
        field -> MarkInitialized();
        Utf8LiteralValue* utf8 =
			nameEnv->FindOrInsertSystemName(descriptor) -> Utf8_literal;
        field -> SetSignatureString(utf8 -> value, utf8 -> length);
    }
    return field;
}


DirectorySymbol* LookupEnvironment::GetOutputDirectory(FileSymbol* file_symbol)
{
    DirectorySymbol* directory_symbol;

    // A FileSymbol for a .class file has a NULL semantic.
    if (file_symbol -> semantic == NULL ||
        (file_symbol -> semantic -> lookup_env).option->directory == NULL) {
        directory_symbol = file_symbol -> directory_symbol;
    }
    else
    {
        LookupEnvironment& control = file_symbol -> semantic -> lookup_env;
        char* directory_prefix = control.option->directory;
        int directory_prefix_length = strlen(directory_prefix);
        int utf8_name_length =
            file_symbol -> package -> PackageNameLength() * 3;
        // +1 for slash
        int estimated_length = directory_prefix_length + utf8_name_length + 1;

        char* directory_name = new char[estimated_length + 1];

        strcpy(directory_name, directory_prefix);

        if (file_symbol -> package != control.unnamed_package)
        {
            // If there was a package declaration, then...
            if (directory_prefix[directory_prefix_length - 1] != U_SLASH)
                strcat(directory_name, StringConstant::U8S_SL);
            char* utf8_name = new char[utf8_name_length + 1];
			CharOperation::ConvertUnicodeToUtf8(file_symbol -> package -> PackageName(),
                                 utf8_name);
            strcat(directory_name, utf8_name);
            delete [] utf8_name;

            if (! SystemIsDirectory(directory_name))
            {
                // The directory does not yet exist.
                for (char* ptr = &directory_name[directory_prefix_length + 1];
                     *ptr; ptr++)
                {
                    // all the slashes in a package_name are forward slashes
                    if (*ptr == U_SLASH)
                    {
                        *ptr = U_NULL;
                        if (! SystemIsDirectory(directory_name))
                            SystemMkdir(directory_name);
                        *ptr = U_SLASH; // restore slash
                    }
                }
                SystemMkdir(directory_name);
            }
        }

        // Read the directory and insert the directory symbol. Note that since
        // the original length computation was an estimate, we compute the real
        // length here.
      
  
		auto w_dir = CharOperation::ConvertUtf8ToUnicode(directory_name);
		delete[] directory_name;
        // This is the output directory, so unless it's added to the
        // classpath, it won't matter whether it's a sourcedir or not.
        directory_symbol = control.nameEnv->ProcessSubdirectories(w_dir.c_str(),w_dir.size(), false);
       
    }
    return directory_symbol;
}


FileSymbol* LookupEnvironment::GetJavaFile(PackageSymbol* package,
                                 const NameSymbol* name_symbol)
{
    FileSymbol* file_symbol = NULL;

    //
    //
    //
    int length = name_symbol -> Utf8NameLength() +
        FileSymbol::java_suffix_length;
    char* full_filename = new char[length + 1]; // +1 for \0
    strcpy(full_filename, name_symbol -> Utf8Name());
    strcat(full_filename, FileSymbol::java_suffix);

    DirectoryEntry* entry = NULL;
    DirectorySymbol* directory_symbol = NULL;
    for (unsigned k = 0; k < package -> directory.Length(); k++)
    {
        directory_symbol = package -> directory[k];
        if ((entry = directory_symbol -> FindEntry(full_filename, length)))
            break;
    }

    if (entry)
    {
        PathSymbol* path_symbol = directory_symbol -> PathSym();

        file_symbol = directory_symbol -> FindFileSymbol(name_symbol);
        if (! ((file_symbol && file_symbol -> IsJava()) ||
               path_symbol -> IsZip()))
        {
            file_symbol = directory_symbol -> InsertFileSymbol(name_symbol);

            file_symbol -> directory_symbol = directory_symbol;
            file_symbol -> SetJava();
            file_symbol -> mtime = entry -> Mtime();
        }
    }

    delete [] full_filename;
    return file_symbol;
}

//
//FileSymbol* LookupEnvironment::GetFile(Option* option, PackageSymbol* package,
//                             const NameSymbol* name_symbol)
//{
//	if(old_classpath_search_order)
//    return option->old_classpath_search_order
//        ? GetFileFirst(option, package, name_symbol)
//        : GetFileBoth(option, package, name_symbol);
//}

FileSymbol* LookupEnvironment::GetFileBoth(Option* option, PackageSymbol* package,
                                 const NameSymbol* name_symbol)
{
    FileSymbol* java_file_symbol = NULL;
    FileSymbol* class_file_symbol = NULL;

    // calculate a length that is large enough...
    int class_length = name_symbol -> Utf8NameLength() +
        FileSymbol::class_suffix_length;
    int java_length = name_symbol -> Utf8NameLength() +
        FileSymbol::java_suffix_length;

    char* class_name = new char[class_length + 1]; // +1 for \0
    strcpy(class_name, name_symbol -> Utf8Name());
    strcat(class_name, FileSymbol::class_suffix);

    char* java_name = new char[java_length + 1]; // +1 for \0
    strcpy(java_name, name_symbol -> Utf8Name());
    strcat(java_name, FileSymbol::java_suffix);

    for (unsigned k = 0; k < package -> directory.Length(); k++)
    {
        DirectorySymbol* directory_symbol = package -> directory[k];
        bool foundBothEntries = false;
        FileSymbol* file_symbol =
            directory_symbol -> FindFileSymbol(name_symbol);
        if (! file_symbol)
        {
            PathSymbol* path_symbol = directory_symbol -> PathSym();
            if (! path_symbol -> IsZip())
            {
                DirectoryEntry* java_entry =
                    directory_symbol -> FindEntry(java_name, java_length),
                *class_entry = (((! option->depend ||
                                  java_entry == NULL) &&
                                 (! directory_symbol -> IsSourceDirectory()))
                                ? directory_symbol -> FindEntry(class_name,
                                                                class_length)
                                : (DirectoryEntry*) NULL);

                if (java_entry || class_entry)
                {
                    file_symbol =
                        directory_symbol -> InsertFileSymbol(name_symbol);
                    file_symbol -> directory_symbol = directory_symbol;

                    if (java_entry &&
                        (! class_entry ||
                         class_entry -> Mtime() < java_entry -> Mtime()))
                    {
                        file_symbol -> SetJava();
                        file_symbol -> mtime = java_entry -> Mtime();
                    }
                    else
                    {
                        if (java_entry)
                            file_symbol -> SetClass();
                        else file_symbol -> SetClassOnly();
                        file_symbol -> mtime = class_entry -> Mtime();
                    }
                }

                // Flag case where both .java and class found in same path.
                if (java_entry && class_entry)
                    foundBothEntries = true;
            }
        }

        if (file_symbol)
        {
            // If no .java file seen yet, note this one.
            if (file_symbol -> IsJava() && ! java_file_symbol)
                java_file_symbol = file_symbol;
            // If no .class file seen yet, note this one.
            else if (! class_file_symbol)
                class_file_symbol = file_symbol;


            if (foundBothEntries == true ||
                (java_file_symbol && class_file_symbol))
            {
                // Both .java and .class seen, so no point in continuing the
                // search.
                break;
            }
        }
    }

    delete [] java_name;
    delete [] class_name;

    //
    // If both .java and .class seen, do a mod time check to decide which one
    // to deliver. Otherwise just return whichever kind we found, or NULL.
    //
    if (java_file_symbol &&
        (! class_file_symbol ||
         class_file_symbol -> mtime < java_file_symbol -> mtime))
    {
        return java_file_symbol;
    }
    return class_file_symbol;
}

FileSymbol* LookupEnvironment::GetFileFirst(
	Option* option, 
	PackageSymbol* package,
    const NameSymbol* name_symbol
)
{
    FileSymbol* file_symbol = NULL;

    //
    // calculate a length that is large enough...
    //
    int class_length = name_symbol -> Utf8NameLength() +
        FileSymbol::class_suffix_length;
    int java_length = name_symbol -> Utf8NameLength() +
        FileSymbol::java_suffix_length;

    char* class_name = new char[class_length + 1]; // +1 for \0
    strcpy(class_name, name_symbol -> Utf8Name());
    strcat(class_name, FileSymbol::class_suffix);

    char* java_name = new char[java_length + 1]; // +1 for \0
    strcpy(java_name, name_symbol -> Utf8Name());
    strcat(java_name, FileSymbol::java_suffix);

    for (unsigned k = 0; k < package -> directory.Length(); k++)
    {
        DirectorySymbol* directory_symbol = package -> directory[k];
        file_symbol = directory_symbol -> FindFileSymbol(name_symbol);
        if (file_symbol)
             break;

        PathSymbol* path_symbol = directory_symbol -> PathSym();
        if (! path_symbol -> IsZip())
        {
            DirectoryEntry* java_entry =
                directory_symbol -> FindEntry(java_name, java_length);
            DirectoryEntry* class_entry = ((! option->depend ||
                                            (java_entry == NULL))
                                           ? directory_symbol -> FindEntry(class_name,
                                                                           class_length)
                                           : (DirectoryEntry*) NULL);

            if (java_entry || class_entry)
            {
                file_symbol =
                    directory_symbol -> InsertFileSymbol(name_symbol);
                file_symbol -> directory_symbol = directory_symbol;

                if (java_entry &&
                    (! class_entry ||
                     class_entry -> Mtime() < java_entry -> Mtime()))
                {
                    file_symbol -> SetJava();
                    file_symbol -> mtime = java_entry -> Mtime();
                }
                else
                {
                    if (java_entry)
                         file_symbol -> SetClass();
                    else file_symbol -> SetClassOnly();
                    file_symbol -> mtime = class_entry -> Mtime();
                }
                break;
            }
        }
    }

    delete [] java_name;
    delete [] class_name;
    return file_symbol;
}



} // Close namespace Jikes block


