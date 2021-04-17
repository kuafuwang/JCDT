// $Id: incrmnt.cpp,v 1.30 2004/03/25 13:32:27 ericb Exp $
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 2004 IBM Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//
#include "stdafx.h"
#include "JCDT_Lib/internal/env/platform.h"
#include "JCDT_Lib/internal/env/LookupEnvironment.h"
#include "JCDT_Lib/internal/lex/scanner.h"
#include "JCDT_Lib/internal/parser/parser.h"
#include  "JCDT_Lib/internal/semantic/semantic.h"
#include "JCDT_Lib/internal/util/case.h"
#include "JCDT_Lib/internal/util/set.h"
#include "JCDT_Lib/internal/env/INameEnvironment.h"
#include "JCDT_Lib/internal/impl/ITypeRequestor.h"
#include "JCDT_Lib/internal/lookup/TypeLookupTable.h"
namespace Jikes { // Open namespace Jikes block


void LookupEnvironment::RemoveTrashedTypes(SymbolSet& type_trash_set)
{
    TypeSymbol* type;

    //
    // For each type T that is going to be trashed, and for each parent P of T
    // that is not itself being trashed, remove T from the set of dependents of
    // P. If T is a subtype of P it is also removed from the subtypes set.
    //
    for (type = (TypeSymbol*) type_trash_set.FirstElement();
         type; type = (TypeSymbol*) type_trash_set.NextElement())
    {

        TypeSymbol* parent;
        for (parent = (TypeSymbol*) type -> static_parents -> FirstElement();
             parent;
             parent = (TypeSymbol*) type -> static_parents -> NextElement())
        {
            if (! type_trash_set.IsElement(parent))
            {
                parent -> dependents -> RemoveElement(type);
                parent -> subtypes -> RemoveElement(type);
            }
        }

        for (parent = (TypeSymbol*) type -> parents -> FirstElement();
             parent;
             parent = (TypeSymbol*) type -> parents -> NextElement())
        {
            if (! type_trash_set.IsElement(parent))
            {
                parent -> dependents -> RemoveElement(type);
                parent -> subtypes -> RemoveElement(type);
            }
        }
    }

    //
    // We can now safely delete the type.
    //
    for (type = (TypeSymbol*) type_trash_set.FirstElement();
         type; type = (TypeSymbol*) type_trash_set.NextElement())
    {
        PackageSymbol* package = type -> ContainingPackage();

        //
        // If a type that is about to be trashed was read in via a class file,
        // remove the class file. Note that invoking RemoveElement for a file
        // that it does not contain has no ill effect.
        //
        FileSymbol* file_symbol = type -> file_symbol;
        if (file_symbol && type -> Identity() == file_symbol -> Identity())
            input_class_file_set.RemoveElement(file_symbol);

        //
        // If a type that is about to be trashed was contained in the
        // unnamed_package, remove it from the set "unnamed_package_types"
        //
        if (package == unnamed_package)
            unnamed_package_types.RemoveElement(type);

        // Remove the type from its containing package.
		auto file = type->file_symbol;
		if(file){
			if(file->IsJava()){
				type->RemoveClassFileFromOutputDirWhenInJavaFile();
			}	
			file->RelieveRelathioshipWithType(type);
		}	
        package -> DeleteTypeSymbol(type);
    }
}



//
// For each file whose associated source (".java") has changed, add it to the
// list to be recompiled...
//
void LookupEnvironment::FindMoreRecentInputFiles(SymbolSet& file_candidates)
{
    FileSymbol* file_symbol;
    for (file_symbol = (FileSymbol*) file_candidates.FirstElement();
         file_symbol;
         file_symbol = (FileSymbol*) file_candidates.NextElement())
    {
        //
        // If the type is not zipped and it is not already contained in the
        // recompilation set, then check it...
        //
        if ((! file_symbol -> IsZip()) &&
            (! recompilation_file_set.IsElement(file_symbol)) &&
            (! expired_file_set.IsElement(file_symbol)))
        {
            // If there is no java source file or its time stamp is not newer
            // than file_symbol then reset file_symbol to NULL. Otherwise,
            // reset file symbol to the newer file.
            DirectoryEntry* java_entry = nameEnv->FindInputFile(file_symbol);
            if (! java_entry)
            {
                // A source file that was compiled in the previous pass no
                // longer exists.
                if (file_symbol -> IsJava())
                    expired_file_set.AddElement(file_symbol);
            }
            else if (java_entry -> Mtime() > file_symbol -> mtime)
            {
                // A newer file was found.
                file_symbol -> mtime = java_entry -> Mtime();
                recompilation_file_set.AddElement(file_symbol);
            }
        }
    }
}




void LookupEnvironment::ComputeRecompilationSet(TypeDependenceChecker& dependence_checker)
{
    SymbolSet type_trash_set;

    // Find out if any source files has been touched since the last
    // compilation and add all such files to recompilation_file_set.
    FindMoreRecentInputFiles(dependence_checker.file_set);

    // Before messing with the files, compute a list of all the types that
    // have just been compiled. We need to do this here as we will be
    // "Resetting" and "reScanning" some files in the loop below, which in
    // effect removes the set of types to which they were associated in the
    // previous compilation.
    int length_estimate = input_java_file_set.Size(); // problem size estimate
    Tuple<TypeSymbol*> input_types(length_estimate * 2);
    FileSymbol* file_symbol;
    for (file_symbol = (FileSymbol*) input_java_file_set.FirstElement();
         file_symbol;
         file_symbol = (FileSymbol*) input_java_file_set.NextElement())
    {
        for (unsigned i = 0; i < file_symbol -> types.size(); i++)
            input_types.Next() = file_symbol -> types[i];
    }

    // Declare the closure set, and initialize it with the Union over the
    // closure of the types in the trash_bin. Essentially, we want to catch
    // all "compiled" types in the compilation that has a dependence on these
    // bad types.
    SymbolSet dependents_closure(length_estimate);
    for (unsigned i = 0; i < type_trash_bin.size(); i++)
    {
        TypeSymbol* type = type_trash_bin[i];
        if (! dependents_closure.IsElement(type))
        {
            if (type -> dependents_closure)
                 dependents_closure.Union(*type -> dependents_closure);
            else dependents_closure.AddElement(type);
        }
    }

    // Compute the set of types from the recompilation set that needs to be
    // recompiled and update the recompilation file set.
    SymbolSet new_set(length_estimate),
              file_seen(length_estimate);
    new_set = recompilation_file_set;
    new_set.Union(expired_file_set);
    file_seen = new_set;

	// How much space do we need for a package declaration? estimate 64 tokens.
	AstStoragePool* ast_pool = new AstStoragePool(64);

    // As long as there is a new_set of files to process,...
    do
    {
        // For each file in new_set, compute the reflexive transitive closure
        // of all types contained in that file. Next, reset and rescan the
        // file. If the scan was successful, iterate over the new list of
        // types to see if any of them had already been introduced in the
        // previous compilation via a class file. If so, add all such types to
        // the dependents closure.
        for (FileSymbol* file_symbol = (FileSymbol*) new_set.FirstElement();
             file_symbol;
             file_symbol = (FileSymbol*) new_set.NextElement())
        {
            for (unsigned i = 0; i < file_symbol -> types.size(); i++)
            {
                TypeSymbol* type = file_symbol -> types[i];
                if (! dependents_closure.IsElement(type))
                {
                    if (type -> dependents_closure)
                        dependents_closure.Union(*type -> dependents_closure);
                    else dependents_closure.AddElement(type);
                }
            }

            if (! expired_file_set.IsElement(file_symbol))
            {
                file_symbol -> Reset();
                file_symbol -> SetJava();

				AstPackageDeclaration* package_declaration = typeRequestor->ParseForPackage(file_symbol, ast_pool);
				
				LexStream* lex_stream = file_symbol->lex_stream;
                if (lex_stream) // did we have a successful scan!
                {
					PackageSymbol* package
						= (package_declaration
							? FindOrInsertPackage(lex_stream,
								package_declaration->name)
							: unnamed_package); 
                }
				ast_pool->Reset();
            }
        }

        //
        // Iterate over the dependents_closure set. For each type T, add it to
        // the trash pile. If the file with which it is associated had not yet
        // been processed, mark it as having been "seen" and add it to the
        // new_set to be considered later. If the file had already been
        // processed but not yet added to the recompilation set, add it to the
        // recompilation set, read it in and if it contains types other than
        // the main one (that had previously been read in via class files) add
        // those new types to the trash pile.
        //
        new_set.SetEmpty();
        TypeSymbol* type;
        for (type = (TypeSymbol*) dependents_closure.FirstElement();
             type;
             type = (TypeSymbol*) dependents_closure.NextElement())
        {
            type_trash_set.AddElement(type);

            FileSymbol* file_symbol = type -> file_symbol;
            if (file_symbol && (! file_seen.IsElement(file_symbol)))
            {
                file_seen.AddElement(file_symbol);
                new_set.AddElement(file_symbol);
                file_symbol -> mtime = 0; // to force a reread of the file.
            }
        }

        //
        // Check that the files in new_set exist, and if so, add them to the
        // recompilation_file_set. Note that if they exist, they will be added
        // because before a file is added to new_set its time stamp is reset
        // to 0. See loop above...
        //
        FindMoreRecentInputFiles(new_set);

        //
        // Empty out the dependents_closure set for the next round.
        //
        dependents_closure.SetEmpty();
    } while (! new_set.IsEmpty());

	delete ast_pool;
    //
    // Clean up the types that were compiled in the previous compilation pass.
    //
    for (unsigned j = 0; j < input_types.Length(); j++)
        input_types[j] -> RemoveCompilationReferences();

    //
    // Reset the closure sets in all the types that were considered in the
    // dependence checker.
    //
    Tuple<TypeSymbol*>& type_list = dependence_checker.TypeList();
    for (unsigned k = 0; k < type_list.Length(); k++)
    {
        TypeSymbol* type = type_list[k];

        type -> index = TypeCycleChecker::OMEGA;
        type -> unit_index = TypeCycleChecker::OMEGA;
        type -> incremental_index = TypeCycleChecker::OMEGA;
        delete type -> dependents_closure;
        type -> dependents_closure = NULL;
    }
    // Remove all dependence edges that are no longer valid.
    RemoveTrashedTypes(type_trash_set);
}

FileSymbol* LookupEnvironment::RecordIncrementalRecompilationUnit(const wstring& deal_file, set<wstring> &error_files)
{
	set<wstring> remove_files;
	set<wstring> change_files;
	std::set<std::wstring> add_files;

	recompilation_file_set.SetEmpty();
	for (FileSymbol* file_symbol = (FileSymbol*)input_java_file_set.FirstElement();
		file_symbol;
		file_symbol = (FileSymbol*)input_java_file_set.NextElement())
	{
		wstring tempFileName;
		auto file_location = FileLocation(file_symbol);
		if (file_location.location) {
			tempFileName = file_location.location;
		}
		if(deal_file == tempFileName){
			change_files.insert(deal_file);
		}
		else
		{
			remove_files.insert(tempFileName);
		}
	}

    if(change_files.empty()){
		add_files.insert(deal_file);
    }

	bool ret =  RecordIncrementalRecompilationUnit(add_files, change_files, remove_files, error_files);
	auto _size = input_files.size();
	for (size_t i = 0; i < _size;  ++i)
	{
		wstring tempFileName;
		auto file_location = FileLocation(input_files[i]);
		if (file_location.location) {
			tempFileName = file_location.location;
		}
		if (deal_file == tempFileName) {
			return input_files[i];
		}
		
	}
	return nullptr;
}
bool LookupEnvironment::RecordcompilationUnit(set<wstring>&add_files)

{
	FileSymbol* file_symbol = nullptr;
	size_t _new_set_size = add_files.size();
	if (_new_set_size) {
		nameEnv->ProcessNewInputFiles(recompilation_file_set, add_files);
	}
	else{
		return false;
	}
	input_files.reserve(recompilation_file_set.Size());
	for (file_symbol = (FileSymbol*)recompilation_file_set.FirstElement();
		file_symbol;
		file_symbol = (FileSymbol*)recompilation_file_set.NextElement()) {
		input_files.push_back(file_symbol);
		if (typeRequestor->HeadParse(file_symbol)) {
			ProcessPackageDeclaration(file_symbol, file_symbol->compilation_unit);

		}
	}
	return true;
}

	bool LookupEnvironment::RecordcompilationUnit(set<FileSymbol*>& add_files)
	{
		FileSymbol* file_symbol = nullptr;
		size_t _new_set_size = add_files.size();
		if (_new_set_size) {
			nameEnv->ProcessNewInputFiles(recompilation_file_set, add_files);
		}
		else {
			return false;
		}
		input_files.reserve(recompilation_file_set.Size());
		for (file_symbol = (FileSymbol*)recompilation_file_set.FirstElement();
			file_symbol;
			file_symbol = (FileSymbol*)recompilation_file_set.NextElement()) {
			input_files.push_back(file_symbol);
			if(!file_symbol->compilation_unit)
			{
				if (typeRequestor->HeadParse(file_symbol)) {
					ProcessPackageDeclaration(file_symbol, file_symbol->compilation_unit);

				}
			}
			else
			{
				ProcessPackageDeclaration(file_symbol, file_symbol->compilation_unit);
			}
		}
		return true;
	}

	// Check whether or not there are files to be recompiled.
bool LookupEnvironment::RecordIncrementalRecompilationUnit(
	std::set<std::wstring>& add_files,
	std::set<std::wstring>& chang_files,
	std::set<std::wstring>& remove_files,
	std::set<std::wstring>& last_compiled_file_has_error
)
{
    // Empty out the type lookup table so that it does not continue
    // to point to a type that is deleted here.
    type_table.SetEmpty();
    SymbolSet candidates(input_java_file_set.Size() +
                         input_class_file_set.Size() +
                         recompilation_file_set.Size());

	FileSymbol* file_symbol;
	if (!recompilation_file_set.IsEmpty()) {
		candidates = recompilation_file_set;
	}
    else{
		candidates = input_java_file_set;
		candidates.Union(input_class_file_set);
    }

  
    TypeDependenceChecker dependence_checker(this, candidates,
                                                type_trash_bin);
    dependence_checker.PartialOrder();


	auto change_end = chang_files.end();
	auto add_end = add_files.end();
	auto remove_end = remove_files.end();
	auto error_file_end = last_compiled_file_has_error.end();

	if (!recompilation_file_set.IsEmpty()) {
		for (file_symbol = (FileSymbol*)recompilation_file_set.FirstElement();
		file_symbol;
			file_symbol = (FileSymbol*)recompilation_file_set.NextElement())
		{
			wstring tempFileName;
			auto file_location = FileLocation(file_symbol);
			if (file_location.location) {
				tempFileName = file_location.location;
			}
			auto find_it = add_files.find(tempFileName);
			if (find_it != add_end) {
				add_files.erase(find_it);
				continue;
			}
			auto  find_it_it_remove = remove_files.find(tempFileName);
			if (find_it_it_remove != remove_end) {
				remove_files.erase(find_it_it_remove);
				recompilation_file_set.RemoveElement(file_symbol);
				expired_file_set.AddElement(file_symbol);
				continue;
			}

		}
	}
	file_symbol = (FileSymbol*)input_java_file_set.FirstElement();
	for (; file_symbol; file_symbol = (FileSymbol*)input_java_file_set.NextElement())
	{

		wstring tempFileName;
		auto file_location = FileLocation(file_symbol);
		if (file_location.location) {
			tempFileName = file_location.location;
		}
		auto find_it_in_change = chang_files.find(tempFileName);
		if (find_it_in_change != change_end) {
			chang_files.erase(find_it_in_change);
			recompilation_file_set.AddElement(file_symbol);
			continue;
		}
		auto find_it_in_add = add_files.find(tempFileName);
		if (find_it_in_add != add_end) {
			add_files.erase(find_it_in_add);
			recompilation_file_set.AddElement(file_symbol);
			continue;
		}
		auto  find_it_it_remove = remove_files.find(tempFileName);
		if (find_it_it_remove != remove_end) {
			remove_files.erase(find_it_it_remove);
			expired_file_set.AddElement(file_symbol);

			continue;
		}

		auto find_has_errer_file = last_compiled_file_has_error.find(tempFileName);
		if (find_has_errer_file != error_file_end) {
			last_compiled_file_has_error.erase(find_has_errer_file);
			recompilation_file_set.AddElement(file_symbol);
			continue;
		}

	}

	nameEnv->RereadDirectories();

	

	size_t _new_set_size = add_files.size();
	if (_new_set_size) {
		SymbolSet new_javas(_new_set_size);
		nameEnv->ProcessNewInputFiles(new_javas, add_files);
		for (file_symbol = (FileSymbol*)new_javas.FirstElement();
		file_symbol;
			file_symbol = (FileSymbol*)new_javas.NextElement()) {
			recompilation_file_set.AddElement(file_symbol);
		}
	}

    // Compute the initial set of files that need to be recompiled. Place
    // them in recompilation_file_set.
    ComputeRecompilationSet(dependence_checker);	
    

    // Starting with the initial recompilation_file_set, complete the
    // computation of the set of files that need to be recompiled. (Add all
    // new files to recompilation_file_set). Also, complete the computation of
    // type_trash_set, the set of files that should be removed from the
    // database as they will be recompiled.

#ifdef FOR_CONSOLE
    fprintf(stderr, "%s", (recompilation_file_set.IsEmpty() &&
                           expired_file_set.IsEmpty()
                           ? "\nnothing changed...\n" : "\nok...\n"));
    fflush(stderr);
#endif

	input_files.clear();
	
	input_files.reserve(recompilation_file_set.Size());
	for (file_symbol = (FileSymbol*)recompilation_file_set.FirstElement();
		file_symbol;
		file_symbol = (FileSymbol*)recompilation_file_set.NextElement()){
		input_java_file_set.RemoveElement(file_symbol);
		input_files.push_back(file_symbol);
		LexStream* lex_stream = file_symbol->lex_stream;
		if (lex_stream){
			if( typeRequestor ->HeadParse(file_symbol)){
				ProcessPackageDeclaration(file_symbol, file_symbol->compilation_unit);
			}	
		}
	}


	// If a file was erased, remove it from the input file set.
	for (file_symbol = (FileSymbol*)expired_file_set.FirstElement();
	file_symbol;
		file_symbol = (FileSymbol*)expired_file_set.NextElement()) {
		//file_symbol->RemoveClassFileFromOutputDirWhenInJavaFile();
		input_java_file_set.RemoveElement(file_symbol);
		file_symbol->Reset();
	}

	// Reset the global objects before recompiling this new batch.
	expired_file_set.SetEmpty();
	recompilation_file_set.SetEmpty();
	type_trash_bin.clear();

	// Reset the return code as we may compile clean in this pass.
	return_code = 0;
	system_semantic->return_code = 0;
    return true;
}
// Check whether or not there are files to be recompiled.
set<wstring> LookupEnvironment::CalculateRecompilationSet(
	std::set<std::wstring>& chang_files,
	std::set<std::wstring>& remove_files
)
{
	// Empty out the type lookup table so that it does not continue
	// to point to a type that is deleted here.
	cleanUp();
	type_table.SetEmpty();
	SymbolSet candidates(input_java_file_set.Size() +
		input_class_file_set.Size() +
		recompilation_file_set.Size());

	FileSymbol* file_symbol;
	if (!recompilation_file_set.IsEmpty()) {
		candidates = recompilation_file_set;
	}
	else {
		candidates = input_java_file_set;
		candidates.Union(input_class_file_set);
	}


	TypeDependenceChecker dependence_checker(this, candidates,
		type_trash_bin);
	dependence_checker.PartialOrder();


	auto change_end = chang_files.end();
	
	auto remove_end = remove_files.end();

	if (!recompilation_file_set.IsEmpty()) {
		for (file_symbol = (FileSymbol*)recompilation_file_set.FirstElement();
			file_symbol;
			file_symbol = (FileSymbol*)recompilation_file_set.NextElement())
		{
			wstring tempFileName;
			auto file_location = FileLocation(file_symbol);
			if (file_location.location) {
				tempFileName = file_location.location;
			}
		
			auto  find_it_it_remove = remove_files.find(tempFileName);
			if (find_it_it_remove != remove_end) {
				remove_files.erase(find_it_it_remove);
				recompilation_file_set.RemoveElement(file_symbol);
				expired_file_set.AddElement(file_symbol);
				continue;
			}

		}
	}
	file_symbol = (FileSymbol*)input_java_file_set.FirstElement();
	for (; file_symbol; file_symbol = (FileSymbol*)input_java_file_set.NextElement())
	{

		wstring tempFileName;
		auto file_location = FileLocation(file_symbol);
		if (file_location.location) {
			tempFileName = file_location.location;
		}
		auto find_it_in_change = chang_files.find(tempFileName);
		if (find_it_in_change != change_end) {
			chang_files.erase(find_it_in_change);
			recompilation_file_set.AddElement(file_symbol);
			continue;
		}
	
		auto  find_it_it_remove = remove_files.find(tempFileName);
		if (find_it_it_remove != remove_end) {
			remove_files.erase(find_it_it_remove);
			expired_file_set.AddElement(file_symbol);
			continue;
		}

	}

	
	// Compute the initial set of files that need to be recompiled. Place
	// them in recompilation_file_set.
	ComputeRecompilationSet(dependence_checker);


	for (file_symbol = (FileSymbol*)recompilation_file_set.FirstElement();
		file_symbol;
		file_symbol = (FileSymbol*)recompilation_file_set.NextElement()) {
		file_symbol->RemoveClassFileFromOutputDirWhenInJavaFile();
	}

	// If a file was erased, remove it from the input file set.
	for (file_symbol = (FileSymbol*)expired_file_set.FirstElement();
		file_symbol;
		file_symbol = (FileSymbol*)expired_file_set.NextElement()) {
		file_symbol->RemoveClassFileFromOutputDirWhenInJavaFile();
	}



	// If a file was erased, remove it from the input file set.
	for (file_symbol = (FileSymbol*)expired_file_set.FirstElement();
		file_symbol;
		file_symbol = (FileSymbol*)expired_file_set.NextElement()) {
		file_symbol->RemoveClassFileFromOutputDirWhenInJavaFile();
	}


	return {};
}

void LookupEnvironment::HelpToComputeRecompilationSet(TypeDependenceChecker& dependence_checker)
{
	SymbolSet type_trash_set;
	int length_estimate = input_java_file_set.Size(); // problem size estimate
	SymbolSet dependents_closure(length_estimate);
	for (unsigned i = 0; i < type_trash_bin.size(); i++){
		TypeSymbol* type = type_trash_bin[i];
		if (!dependents_closure.IsElement(type)){
			if (type->dependents_closure)
				dependents_closure.Union(*type->dependents_closure);
			else dependents_closure.AddElement(type);
		}
	}
	// Compute the set of types from the recompilation set that needs to be
	// recompiled and update the recompilation file set.
	SymbolSet new_set(length_estimate),
		file_seen(length_estimate);
	new_set = recompilation_file_set;
	new_set.Union(expired_file_set);
	file_seen = new_set;
	// As long as there is a new_set of files to process,...
	do{
		for (FileSymbol* file_symbol = (FileSymbol*)new_set.FirstElement();
			file_symbol;
			file_symbol = (FileSymbol*)new_set.NextElement()){
			for (unsigned i = 0; i < file_symbol->types.size(); i++){
				TypeSymbol* type = file_symbol->types[i];
				if (!dependents_closure.IsElement(type)){
					if (type->dependents_closure)
						dependents_closure.Union(*type->dependents_closure);
					else dependents_closure.AddElement(type);
				}
			}
		}

		// Iterate over the dependents_closure set. For each type T, add it to
		// the trash pile. If the file with which it is associated had not yet
		// been processed, mark it as having been "seen" and add it to the
		// new_set to be considered later. If the file had already been
		// processed but not yet added to the recompilation set, add it to the
		// recompilation set, read it in and if it contains types other than
		// the main one (that had previously been read in via class files) add
		// those new types to the trash pile.
		new_set.SetEmpty();
		TypeSymbol* type;
		for (type = (TypeSymbol*)dependents_closure.FirstElement();
			type;
			type = (TypeSymbol*)dependents_closure.NextElement()){
			type_trash_set.AddElement(type);
			FileSymbol* file_symbol = type->file_symbol;
			if (file_symbol && (!file_seen.IsElement(file_symbol))){
				file_seen.AddElement(file_symbol);
				new_set.AddElement(file_symbol);
				file_symbol->mtime = 0; // to force a reread of the file.
			}
		}

		FindMoreRecentInputFiles(new_set);
		dependents_closure.SetEmpty();
	} while (!new_set.IsEmpty());
}
} // Close namespace Jikes block


