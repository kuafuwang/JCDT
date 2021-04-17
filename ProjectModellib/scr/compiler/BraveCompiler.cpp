#include "stdafx.h"
#include <JavaModelLib/compiler/BraveCompiler.h>
#include <JCDT_Lib/internal/env/LookupEnvironment.h>
#include <JCDT_Lib/internal/lookup/FileSymbol.h>
#include <JCDT_Lib/internal/impl/FileLocation.h>
#include <set>
namespace Jikes { // Open namespace Jikes block


	namespace JavaModel {
		BraveCompiler::BraveCompiler(INameEnvironment* i_name_environment, Option* jikes_option)
			: Compiler(i_name_environment, jikes_option)
		{
			
		}

		BraveCompiler::~BraveCompiler(){
			Clear();
		}

		void BraveCompiler::ParseStage()
		{
			lookupEnvironment->RecordIncrementalRecompilationUnit(add_file_set, chang_file_set, remove_file_set, file_has_error_set);
			file_has_error_set.clear();
			auto _size = lookupEnvironment->input_files.size();
			for (auto j = 0; j < _size; j++) {
				lookupEnvironment->ProcessFileHeaders(lookupEnvironment->input_files[j]);
			}
		}

		void BraveCompiler::BindingStage()
		{
			auto _size = lookupEnvironment->input_files.size();
			lookupEnvironment->ProcessFileBodys();
			lookupEnvironment->ReportSymstemErrorMsg();
			for (auto j = 0; j < _size; j++) {
				auto _file_symbol = lookupEnvironment->input_files[j];
				if (_file_symbol->NumErrors()) {
					wstring tempFileName;
					auto file_location = FileLocation(_file_symbol);
					if (file_location.location) {
						tempFileName = file_location.location;
					}
					file_has_error_set.insert(tempFileName);
				}
			}
		}


		void BraveCompiler::Compile()
		{
			lookupEnvironment->RecordcompilationUnit(add_file_set);
			file_has_error_set.clear();
			
			vector<FileSymbol*>& input_files = lookupEnvironment->input_files;
			auto _size = input_files.size();
			for (auto j = 0; j < _size; j++) {
				auto _file_symbol = input_files[j];
				lookupEnvironment->ProcessFileHeaders(_file_symbol);
				if (_file_symbol->NumErrors()) {

				}
			}
			lookupEnvironment->ProcessFileBodys();

			lookupEnvironment->ReportSymstemErrorMsg();
			for (auto j = 0; j < _size; j++) {
				auto _file_symbol = input_files[j];
				if (_file_symbol->NumErrors()) {
					wstring tempFileName;
					auto file_location = FileLocation(_file_symbol);
					if (file_location.location) {
						tempFileName = file_location.location;
					}
					file_has_error_set.insert(tempFileName);
				}
			}
		}


		void BraveCompiler::Clear(){
			add_file_set.clear();
			chang_file_set.clear();
			remove_file_set.clear();
			lookupEnvironment->cleanUp();
		}

		Semantic* BraveCompiler::getSymstemError()
		{
			return lookupEnvironment->system_semantic;
		}
	} // Close namespace JavaModel block

} // Close namespace Jikes block

