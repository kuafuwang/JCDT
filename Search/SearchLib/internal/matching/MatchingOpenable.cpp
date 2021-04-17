#include "stdafx.h"
#include "MatchingOpenable.h"
#include <SearchLib/internal/pattern/SearchPattern.h>
#include "MatchSet.h"
#include "MatchLocatorParserVisitor.h"
#include <JCDT_Lib/internal/lookup/FileSymbol.h>
#include <SearchLib/internal/core/IJavaSearchResultCollector.h>
#include <JavaModelLib/internal/core/IType.h>
#include <JavaModelLib/internal/core/ClassFile.h>
#include <JCDT_Lib/internal/classfmt/ClassFile.inl>
#include <JavaModelLib/internal/core/BinaryMethod.h>
#include <JCDT_Lib/internal/classfmt/class.h>
#include <JavaModelLib/internal/core/BinaryField.h>
#include <JCDT_Lib/internal/lookup/MethodSymbol.h>

#include <JavaModelLib/internal/core/JavaModel.h>
#include <JavaModelLib/internal/core/OriginalJavaProject.h>
#include <JavaModelLib/internal/core/CompilationUnit.h>

#include <JavaModelLib/internal/lookup/NameLookup.h>
#include <JCDT_Lib/internal/codegen/Signature.h>
#include "LocatorCompiler.h"
#include <JCDT_Lib/internal/env/LookupEnvironment.h>
#include <JCDT_Lib/internal/lookup/VariableSymbol.h>

using namespace Jikes::JavaModel;

namespace Jikes { // Open namespace Jikes block

	namespace Search {
		MatchingOpenable::~MatchingOpenable()
		{
			if(matchSet)
			{
				delete matchSet;
			}
			
		}

		void MatchingOpenable::CreateMatchSet()
		{
			if (!matchSet)
			{
				matchSet = new MatchSet(this);
			}
		}

		bool MatchingOpenable::ShouldResovle(){
			return matchSet->needsResolve();
		}

		MatchingCompilationOpenable::MatchingCompilationOpenable(FileSymbol* _file_symbol,
			LocatorCompiler& _data_center, JavaModel::CompilationUnit* _compilationUnit, SearchPattern* _patten)
			: MatchingOpenable(_data_center, _patten, _compilationUnit),
			file_symbol(_file_symbol),
			compilationUnit(_compilationUnit),
			ast_unit(nullptr)
		{
			CreateMatchSet();
			ast_unit = _file_symbol->compilation_unit;
		}

		MatchingCompilationOpenable::~MatchingCompilationOpenable(){
			if(file_symbol){
				delete file_symbol;
				if(ast_unit){
					delete ast_unit->ast_pool;
				}
			}
		}

		void MatchingCompilationOpenable::locateMatchesInCompilationUnitBeforResolve(){
			
			if (!file_symbol->compilation_unit)
				return;
			ast_unit = file_symbol->compilation_unit;
			if (!ast_unit)
				return;
			MatchLocatorParserVisitor* visitor = new MatchLocatorParserVisitor();

			visitor->matchSet = matchSet;

			ast_unit->traverse(visitor, nullptr);
			delete  visitor;
			matchSet->reportMatching(ast_unit);

		}
		void MatchingCompilationOpenable::locateMatchesInCompilationUnitAfterResolve(){
			if(ast_unit)
			{
				matchSet->cuHasBeenResolved = true;
				matchSet->reportMatching(ast_unit);
			}
		}

	
		MatchingClassOpenable::MatchingClassOpenable(JavaModel::ClassFile* _openable, 
			class LocatorCompiler& _data_center, SearchPattern* _patten)
			: MatchingOpenable(_data_center, _patten, _openable)
		{
			CreateMatchSet();
		}
		
		void MatchingClassOpenable::locateMatchesInClassFiles()
		{
			Jikes::ClassFile* class_file = openable->getClassFileInJar();
			if (class_file &&  ! class_file->Valid()) {
				return;
			}

			
			std::pair<const ConstantPool*, CPClassInfo*> class_data;
			std::pair<void*, SearchPattern::BinaryInfoType > binaryInfo;
			binaryInfo.first = (void*)(class_file->ThisClass());
			binaryInfo.second = SearchPattern::CLASS_INFO;
		
			class_data.first = &(class_file->Pool());
			class_data.second = class_file->ThisClass();
			if(pattern->matchesBinary(binaryInfo, class_data))
			{
				auto binaryType = openable->getType();
				matchSet->reportBinaryMatch(binaryType, IJavaSearchResultCollector::EXACT_MATCH);
			}

			const ConstantPool& pool = class_file->Pool();
		
			bool compilationAborted = false;
			if (pattern->_needsResolve) {
				CPClassInfo* class_info = class_file->ThisClass();
				if (!class_info)
				{
					assert(false);
					return;
				}
				
				string name_(class_info->TypeName(pool));
				auto lookupEnvironment =  compiler.lookupEnvironment;
				if (!lookupEnvironment)
				{
					assert(false);
					return;
				}

				auto binding = lookupEnvironment->getType(name_);
				if(binding && binding != lookupEnvironment->no_type)
				{
					unsigned  num = binding->NumMethodSymbols();
					for(size_t i = 0 ; i  < num ; ++i)
					{
						auto method = binding->MethodSym(i);
						if (!method)
							continue;

						int level = pattern->matchLevel(method);
						switch (level) 
						{
							case SearchPattern::IMPOSSIBLE_MATCH:
							case SearchPattern::INACCURATE_MATCH:
								break;
							default:
							{
								vector<wstring> parameterTypeNames;
								method->GetParameterTypeQuilifiedNames(parameterTypeNames);
								auto binaryType = openable->getType();
								if (!binaryType)
									continue;
								auto methodHandle = binaryType->getMethod(method->getNameString(),parameterTypeNames);
								matchSet->reportBinaryMatch(
									methodHandle,
									level == SearchPattern::ACCURATE_MATCH ?
									IJavaSearchResultCollector::EXACT_MATCH :
									IJavaSearchResultCollector::POTENTIAL_MATCH);
							}

						}
					}
					num = binding->NumVariableSymbols();
					for (size_t i = 0; i < num; ++i){
						auto field = binding->VariableSym(i);
						if (!field)
							continue;
						int level = pattern->matchLevel(field);
						switch (level) {
						case SearchPattern::IMPOSSIBLE_MATCH:
						case SearchPattern::INACCURATE_MATCH:
							break;
						default:
						{
							auto binaryType = openable->getType();
							if (!binaryType)
								continue;
							auto  fieldHandle = binaryType->getField(field->getNameString());
							matchSet->reportBinaryMatch(
								fieldHandle,
								level == SearchPattern::ACCURATE_MATCH ?
								IJavaSearchResultCollector::EXACT_MATCH :
								IJavaSearchResultCollector::POTENTIAL_MATCH);
						}
						break;
						}
					}
				}
			}

			// if compilation was aborted it is a problem with the class path: 
			// report as a potential match if binary info matches the pattern
			int accuracy = compilationAborted ? IJavaSearchResultCollector::POTENTIAL_MATCH 
				: IJavaSearchResultCollector::EXACT_MATCH;
	
			binaryInfo.second = SearchPattern::METHOD_INFO;
			// Read the methods.

			for (int i = class_file->MethodsCount() - 1; i >= 0; i--)
			{
				const MethodInfo* method = class_file->Method(i);

				if (!method)
					continue;

				binaryInfo.first = (void*)(method);
				if (pattern->matchesBinary(binaryInfo, class_data)) {

					auto binaryType = openable->getType();
					if (!binaryType)
						continue;
					
					const char* str = method->Name(pool);
					wstring name_symbol;
					if(str)
					{
						string temp(str, str + method->NameLength(pool));
						name_symbol = CharOperation::ConvertUtf8ToUnicode(temp);
					}
					
					if (method->ACC_SYNTHETIC()){
						if (name_symbol == L"<clinit>"){
							continue;
						}
						else if (name_symbol == L"<init>"){
							name_symbol = binaryType->getElementName();
						}
					}
					str = method->Signature(pool, nullptr);
					string sig;
					if(str)
					{
						 sig.assign(str,
							str + method->SignatureLength(pool, nullptr));
					}
					
					auto  _ret = Signature::parseSignature(sig);

					auto methodHandle = binaryType->getMethod(name_symbol, _ret.first);
					matchSet->reportBinaryMatch(methodHandle, accuracy);
				}

			}
		
			binaryInfo.second = SearchPattern::FIELD_INFO;
			// Read the fields.
			for (auto i = class_file->FieldsCount() - 1; i >= 0; i--){
				const FieldInfo* field = class_file->Field(i);

				if (!field)
					continue;

				binaryInfo.first = (void*)(field);
				if (pattern->matchesBinary(binaryInfo, class_data)){
					const char* str = field->Name(pool);
					wstring name_symbol;
					if(str)
					{
						string temp(field->Name(pool), field->Name(pool) + field->NameLength(pool));
						name_symbol = CharOperation::ConvertUtf8ToUnicode(temp);
					}

					auto binaryType = openable->getType();
					if (!binaryType)
						continue;

					auto fieldHandle = binaryType->getField(name_symbol);
					matchSet->reportBinaryMatch(fieldHandle, accuracy);
				}	
			}
		}

	}

} // Close namespace Jikes block


