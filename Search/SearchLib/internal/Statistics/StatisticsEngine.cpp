#include "stdafx.h"
#include "JCDT_Lib/internal/env/platform.h"
#include "JCDT_Lib/internal/env/LookupEnvironment.h"
#include "JCDT_Lib/internal/impl/option.h"

#include "JCDT_Lib/internal/env/INameEnvironment.h"
#include <JCDT_Lib/internal/lookup/FileSymbol.h>
#include <JCDT_Lib/internal/lookup/VariableSymbol.h>
#include "StatisticsEngine.h"
#include "JCDT_Lib/internal/lex/LexStream.h"
#include "ReferenceVisitor.h"
#include <JavaModelLib/internal/core/IMethod.h>
#include <JCDT_Lib/internal/lookup/MethodSymbol.h>
#include <JCDT_Lib/internal/lookup/PackageSymbol.h>
#include <JavaModelLib/internal/lookup/ISelectionRequestor.h>
#include "SearchLib/internal/matching/SearchResultNode.h"
#include <PathModelLib/MosesPath/MosesPath.h>
#include <JavaModelLib/internal/lookup/SelectionRequestor.h>
#include <JavaModelLib/internal/core/JavaModel.h>
#include <JavaModelLib/internal/core/OriginalJavaProject.h>
#include "FindReferenceOption.h"
#include <JavaModelLib/internal/lookup/NameLookup.h>
#include <JavaModelLib/internal/core/IType.h>
#include <CodeAssistLib/select/SelectUtil.h>

using namespace Jikes::JavaModel;

namespace Jikes {
	namespace Search {

		void ProcessTypeForZeroReference(TypeSymbol* type, map<Symbol*, unsigned int  >&  candidate, FindReferenceOption* option)
		{
			if (!option)
				return;

			if (!type)
				return;

			if (option->find_type_reference)
			{
				candidate.insert({ type, 0 });
			}
			if(type->Bad())
				return;

			auto num = type->NumNestedTypes();
			for (unsigned i = 0; i < num; i++) {
				auto nest_type = type->NestedType(i);
				
				ProcessTypeForZeroReference(nest_type, candidate, option);
			}
			num = type->NumAnonymousTypes();
			for (unsigned j = 0; j < num; j++)
			{
				auto anony_type = type->AnonymousType(j);
			
				ProcessTypeForZeroReference(anony_type, candidate, option);
			}
			
			
			if (option->find_method_reference)
			{
				vector<MethodSymbol*> methods;
				type->availableMethods(methods);
				for (auto it : methods)
				{
					if(!it || !it->file_location)
						continue;
					candidate.insert({ it, 0 });
				}
			}
			
			if (option->find_filed_reference)
			{
				vector<VariableSymbol*> fields;
				type->availableFields(fields);
				for (auto it : fields)
				{
					candidate.insert({ it, 0 });
				}
			}
				
		}
		void ProcessFileForZeroReference(map<Symbol*, unsigned int  >&  candidate, set<FileSymbol*> all_files, FindReferenceOption* option)
		{
			for(auto file : all_files)
			{
				if (!file)
					continue;

				for(auto type : file->types)
				{
					ProcessTypeForZeroReference(type, candidate, option);
				}
			}
		}
		

		StatisticsEngine::StatisticsEngine(INameEnvironment* name_env, Option* option_
		, JavaModel::JavaModel* _pJm, FindReferenceOption*  _reference_option)
			: Compiler(name_env, option_), m_pJM(_pJm), reference_option(_reference_option)
		{
			if(reference_option->IsZero())
			{
				visitor = new ZeroReferenceVisitor(reference_option);
			}
			else
			{
				visitor = new ReferenceVisitor(reference_option);
			}
		}


		StatisticsEngine::~StatisticsEngine()
		{
			clear();
			if (visitor)
				delete visitor;
			visitor = nullptr;
		}

		void StatisticsEngine::clear()
		{
			for(auto it : selectorRequestorMap)
			{
				delete it.second;
			}
			selectorRequestorMap.clear();
			files.clear();
			for(auto it : result)
			{
				delete it.second;
			}
			lookupEnvironment->cleanUp();
		}
		void StatisticsEngine::getUnits()
		{
			vector<FileSymbol*>&  input_files = lookupEnvironment->input_files;
			auto _size = input_files.size();
			for (size_t i = 0; i < _size; ++i)
			{
				
				auto file_location = FileLocation(input_files[i]);
				if (file_location.location) {
					
					auto findIt = files.find(file_location.location);
					if(files.end() != findIt)
					{
						findIt->second = input_files[i];
					}
				}
			}
		}
		class StatisticsResultInFile
		{
		public:
			wstring filePath;
			
		};
	
		void StatisticsEngine::Staticstics(set<wstring>& filePaths)
		{

		
			PreProcess(filePaths);
			map < wstring, vector< pair<Symbol*,unsigned int>  > > files_map;
			wstring doc_path;
			for (auto it : visitor->candidate)
			{
				if(!reference_option->IsTheOne(it.second))
				{
					continue;
				}
				if(!it.first)
				{
					assert(false);
					continue;
				}
			    switch(it.first->_kind)
			    {
				case  Symbol::METHOD:
					{
						auto file = ((MethodSymbol*)(it.first))->file_location;
						if(!file || !file->location)
						{
							continue;
						}
						doc_path = file->location;
						auto indx = doc_path.rfind(L":");
						if (wstring::npos != indx)
						{
							doc_path = doc_path.substr(0, indx);
						}
						files_map[doc_path].push_back({ it.first, it.second });
					}
					break;
				case Symbol::TYPE:
					{
						auto file_symbol = ((TypeSymbol*)(it.first))->file_symbol;

						if (!file_symbol)
							continue;

						 doc_path = file_symbol->GetUnicodeFileName();
						
						 files_map[doc_path].push_back({ it.first, it.second });
					}
					break;

				case Symbol::VARIABLE: 
					{
						auto _type = ((VariableSymbol*)(it.first))->ContainingType();
						if(_type && _type->file_symbol)
						{
							doc_path = _type->file_symbol->GetUnicodeFileName();
							files_map[doc_path].push_back({ it.first, it.second });
						}
					
						
					}

				default:
					break;
			    }


		

			}
			ProcessMehod(files_map);
			
		}
		JavaModel::IType* getSelectOnType(TypeSymbol* type_symbol, JavaModel::ISelectionRequestor* requestor)
		{

			if (!type_symbol)
				return nullptr;

			if (!requestor)
				return nullptr;

			const wchar_t* temp = nullptr;
			wstring declaringTypePackageName;
			wstring declaringTypeName;
			vector<wstring> parameterPackageNames;
			vector<wstring> parameterTypeNames;
			auto package_symbol = type_symbol->ContainingPackage();
			if (package_symbol)
			{
				temp = package_symbol->PackageName();
				if (temp)
					declaringTypePackageName.assign(temp, temp + package_symbol->PackageNameLength());

			}

			temp = type_symbol->Name();
			if (temp)
				declaringTypeName.assign(temp, temp + type_symbol->NameLength());
			boost::replace_all(declaringTypePackageName, L"/", L".");
			int acceptFlags;
			if (type_symbol->ACC_INTERFACE())
			{
				acceptFlags = JavaModel::NameLookup::ACCEPT_INTERFACES;
			}
			else
			{
				acceptFlags = JavaModel::NameLookup::ACCEPT_CLASSES;
			}

			return requestor->getType(declaringTypePackageName, declaringTypeName, acceptFlags);
		}
		
		void StatisticsEngine::ProcessMehod(map < wstring, vector< pair<Symbol*,unsigned int>  > >&files_map)
		{
			for(auto firstLayerIt : files_map)
			{
				auto&  symbolInFile = firstLayerIt.second;
				if(symbolInFile.size() == 0)
					continue;
				const wstring& doc_path = firstLayerIt.first;
				
				auto _pos = m_pJM->getLayerPos(doc_path);
				if (!_pos)
					return;

				auto java_project = m_pJM->getJavaProject(_pos->proj);
				if (!java_project) {
					delete _pos;
					return;
				}
				auto requestor = selectorRequestorMap[_pos->proj];
				delete _pos;


				auto  itFile = result.find(doc_path);
				ReferenceFileResultElement* fileElement;
				if (itFile == result.end())
				{
					fileElement = new ReferenceFileResultElement();
					result.insert({ doc_path ,fileElement });
				}
				else
				{
					fileElement = itFile->second;
				}
				auto& result_in_file = fileElement->result_in_file;

				for(auto pairData : symbolInFile)
				{
					if (!pairData.first)
						continue;

					auto _kind = pairData.first->_kind;
					switch (_kind)
					{
					case Symbol::METHOD:
						{
						MethodSymbol* method_symbol = (MethodSymbol*)(pairData.first);
						wstring selector;
						if(method_symbol->IsConstructor())
						{
							if (method_symbol->containing_type)
								selector = method_symbol->containing_type->getNameString();
						}
						else
						{
							const wchar_t* temp = method_symbol->Name();
							if (temp)
								selector.assign(temp, temp + method_symbol->NameLength());
						}
						
						auto _ret = CodeAssist::SelectUtil::getSelectOnMethod(method_symbol, selector, requestor);
						if (!_ret.size() || !_ret[0])
							continue;

						auto methodElement = _ret[0];
						fileElement->count += 1;
						auto moses = (MemberInFileMoses*) methodElement->getMosesPath();

						if (!moses)
							continue;

						auto fileMose = new FileMoses(moses->proj, moses->root, moses->pkg, moses->file, false);

						fileElement->resouce = fileMose;
						auto methodInFileEle = new ReferenceMemberInFileElemnt();
						methodInFileEle->result_count = pairData.second;
						methodInFileEle->element = methodElement;
						result_in_file[_kind].push_back(methodInFileEle);
						}
						break;

					case Symbol::VARIABLE:
						{
						  auto field_symbol = (VariableSymbol*)(pairData.first);
						  auto findIt = visitor->localFieldScopeDatas.find(field_symbol);

						  AstNodeScope* scope ;
						  if (findIt != visitor->localFieldScopeDatas.end())
						  {
							  scope = findIt->second;
						  }
						  else
						  {
							  scope = nullptr;
						  }
							 
						  auto element = CodeAssist::SelectUtil::selectOnField( scope, field_symbol, requestor);
						  if(!element)
							  continue;

						  fileElement->count += 1;
						  auto moses = (MemberInFileMoses*)element->getMosesPath();

						  if (!moses)
							  continue;

						  auto fileMose = new FileMoses(moses->proj, moses->root, moses->pkg, moses->file, false);
						  fileElement->resouce = fileMose;
						  auto methodInFileEle = new ReferenceMemberInFileElemnt();
						  methodInFileEle->result_count = pairData.second;
						  methodInFileEle->element = element;
						  result_in_file[_kind].push_back(methodInFileEle);

						}
						break;

					case Symbol::TYPE:
						{
						TypeSymbol* type_symbol = (TypeSymbol*)(pairData.first);

						const wchar_t* temp = nullptr;
						wstring declaringTypePackageName;
						wstring declaringTypeName;
						vector<wstring> parameterPackageNames;
						vector<wstring> parameterTypeNames;
						auto package_symbol = type_symbol->ContainingPackage();
						if (package_symbol)
						{
							temp = package_symbol->PackageName();
							if (temp)
								declaringTypePackageName.assign(temp, temp + package_symbol->PackageNameLength());

						}

						temp = type_symbol->Name();
						if (temp)
							declaringTypeName.assign(temp, temp + type_symbol->NameLength());
						boost::replace_all(declaringTypePackageName, L"/", L".");
						int acceptFlags;
						if (type_symbol->ACC_INTERFACE())
						{
							acceptFlags = JavaModel::NameLookup::ACCEPT_INTERFACES;
						}
						else
						{
							acceptFlags = JavaModel::NameLookup::ACCEPT_CLASSES;
						}

						auto typeIt = requestor->getType(declaringTypePackageName, declaringTypeName, acceptFlags);
						if (!typeIt)
							continue;

						fileElement->count += 1;
						auto moses = (MemberInFileMoses*)typeIt->getMosesPath();

						if (!moses)
							continue;

						auto fileMose = new FileMoses(moses->proj, moses->root, moses->pkg, moses->file, false);
						
						fileElement->resouce = fileMose;
						auto methodInFileEle = new ReferenceMemberInFileElemnt();
						methodInFileEle->result_count = pairData.second;
						methodInFileEle->element = typeIt;
						result_in_file[_kind].push_back(methodInFileEle);

						}
						break;

					default:
						break;
					}
					
				}
				if(fileElement->count ==0)
				{
					itFile = result.find(doc_path);
					result.erase(itFile);
				}
				
			}
		}
	
		void StatisticsEngine::PreProcess(set<wstring>& filePaths)
		{

			set<wstring> 	chang_file_set, remove_file_set, _file_with_eror_set;

			lookupEnvironment->RecordIncrementalRecompilationUnit(filePaths, chang_file_set, remove_file_set, _file_with_eror_set);
			for (size_t j = 0; j < lookupEnvironment->input_files.size(); j++) {
				lookupEnvironment->ProcessFile(lookupEnvironment->input_files[j]);
			}

			for (auto it : filePaths)
			{
				files[it] = NULL;
			}
			// Брвы
			getUnits();

			for (auto it : files)
			{
				visitor->all_files.insert(it.second);
				
			}
			if(reference_option->IsZero())
			{
				for (auto it : files)
				{
					ProcessFileForZeroReference(visitor->candidate, visitor->all_files, reference_option);
				}
			}
		
			map<Symbol*, int > theOnes;
			for (auto it : files)
			{
				auto cunit = it.second;
				cunit->compilation_unit->traverse(visitor, nullptr);
				for(auto one : visitor->theOnes)
				{
					theOnes.insert(one);
				}
				visitor->theOnes.clear();
			}

			for(auto one : theOnes)
			{
				visitor->candidate.insert(one);
			}

		}
	}//namespace CodeAssist
} // Close namespace Jikes block


