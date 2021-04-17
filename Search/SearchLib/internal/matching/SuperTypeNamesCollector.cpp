#include "stdafx.h"
#include "SuperTypeNamesCollector.h"
#include <SearchLib/internal/pattern/MethodReferencepattern.h>
#include <PathModelLib/MosesPath/MosesPath.h>
#include <JavaModelLib/internal/core/OriginalJavaProject.h>
#include <JavaModelLib/internal/core/JavaModel.h>

#include <JavaModelLib/internal/core/CloneJavaProject.h>
#include <JavaModelLib/internal/core/OriginalJavaProject.h>
#include <JavaModelLib/internal/core/JarPackageFragmentRoot.h>
#include <JavaModelLib/internal/core/JarPackageFragment.h>
#include <JavaModelLib/internal/core/CompilationUnit.h>


#include <vector>
#include <set>
#include <JCDT_Lib/internal/parser/parser.h>
#include <JCDT_Lib/internal/lex/scanner.h>
#include <JCDT_Lib/internal/lookup/FileSymbol.h>
#include <SearchLib/internal/pattern/Searchpattern.h>
#include "MatchingOpenable.h"
#include <JCDT_Lib/internal/env/LookupEnvironment.h>
#include <JCDT_Lib/internal/env/INameEnvironment.h>
#include "LocatorCompiler.h"
#include <JCDT_Lib/internal/impl/JikesClassPaths.h>
#include <SearchLib/internal/core/JavaSearchScopeData.h>
#include <SearchLib/internal/core/IJavaSearchResultCollector.h>
#include "MatchSet.h"
#include <PathModelLib/MosesPath/MosesPath.h>
#include <JavaModelLib/compiler/BraveCompiler.h>
#include <JCDT_Lib/internal/lookup/FileSymbol.h>

namespace Jikes { // Open namespace Jikes block


	
namespace Search{
	SuperTypeNamesCollector::TypeDeclarationVisitor::TypeDeclarationVisitor(SuperTypeNamesCollector* _collector)
		:super_type_names_collector_(_collector)
	{
		setDefaultRetValue(true);
	}
	bool SuperTypeNamesCollector::TypeDeclarationVisitor::visit(ClassTypeDeclaration* node, AstNodeScope* scope)
	{
		if (!node)
			return false;
		if( super_type_names_collector_->matches(node->symbol) ){
			super_type_names_collector_->collectSuperTypeNames(node->symbol);
		}
		return true;
	}

	bool SuperTypeNamesCollector::TypeDeclarationVisitor::visit(InterfaceTypeDeclaration* node, AstNodeScope* scope)
	{
		if (super_type_names_collector_->matches(node->symbol)) {
			super_type_names_collector_->collectSuperTypeNames(node->symbol);
		}
		return true;
	}

	bool SuperTypeNamesCollector::TypeDeclarationVisitor::visit(AnnotationTypeDeclaration* node, AstNodeScope* scope)
	{
		if (!node)
			return false;
		if (super_type_names_collector_->matches(node->symbol)) {
			super_type_names_collector_->collectSuperTypeNames(node->symbol);
		}
		return true;
	}

	bool SuperTypeNamesCollector::TypeDeclarationVisitor::visit(EnumTypeDeclaration* node, AstNodeScope* scope){
		if (!node)
			return false;
		if (super_type_names_collector_->matches(node->symbol)) {
			super_type_names_collector_->collectSuperTypeNames(node->symbol);
		}
		return true;
	}


	SuperTypeNamesCollector::SuperTypeNamesCollector(MethodReferencePattern* _p )
		:pattern(_p)
	{

	}


	
	 bool SuperTypeNamesCollector::matches(TypeSymbol* type) {
		if (type == nullptr ) return false;
		auto  name = type->getCompoundName();
		if (!name)
			return false;
		
		wstring compoundName_temp(name);
		vector<wstring> compoundName;

		boost::split(compoundName, compoundName_temp, boost::is_any_of(L"./"));
		return matches(compoundName);
	}
	 bool  SuperTypeNamesCollector::matches(const vector<wstring>& compoundName) const
	 {
		int length = compoundName.size();
		if (length == 0) return false;
		auto  compoundName_data  =  compoundName.data();
		wstring simpleName = compoundName_data[length - 1];
		auto& declaringSimpleName = pattern->declaringSimpleName;
		auto& declaringQualification = pattern->declaringQualification;
		int last = length - 1;
		if (declaringSimpleName.empty() != true) {
			// most frequent case: simple name equals last segment of compoundName
			if (pattern->matchesName(simpleName, declaringSimpleName)) {
				vector<wstring> qualification(compoundName_data, compoundName_data + last);
				auto str_decode_qualification = boost::join(qualification, L".");
				return
					pattern->matchesName(declaringQualification, str_decode_qualification);

			}
			else if (!CharOperation::endsWith(simpleName, declaringSimpleName)) {
				return false;
			}
			else {
				return false;
			}
		}
		else {
			vector<wstring> qualification(compoundName_data, compoundName_data + last);
			auto str_decode_qualification = boost::join(qualification, L".");
			return
				pattern->matchesName(
					declaringQualification, str_decode_qualification);
		}
	}


	void  SuperTypeNamesCollector::collectSuperTypeNames(TypeSymbol* type) {

		if (!type)
			return;

		// superclass
		auto superclass = type->super;
		if (superclass) {
			wstring compoundName = superclass->getCompoundName() ;
			boost::replace_all(compoundName, L"/", L".");
			allSuperDeclaringTypeNames.insert(compoundName);
			collectSuperTypeNames(superclass);
		}

		// interfaces
		auto _interfaces = type->interfaces;
		if (_interfaces != nullptr) {
			auto length = _interfaces->size();
			auto data = _interfaces->data();
			for (int i = 0; i < length; i++) {
				auto  _interfase = data[i];
				if (!_interfase)
					continue;
				wstring compoundName = _interfase->getCompoundName();
				boost::replace_all(compoundName, L"/", L".");
				allSuperDeclaringTypeNames.insert(compoundName);
				collectSuperTypeNames(_interfase);
			}
		}
	}
	void SuperTypeNamesCollector::collect()
	{
		if(pattern->declaringType){
			auto actual_point = pattern->declaringType;
			Jikes::JavaModel::JavaModel* pjm = nullptr ;
			if (!pjm || !actual_point)
				return;
			wstring source_file;
			if(actual_point->binary){
				Option* option = new Option();
				option->bytecode = false;
				JikesClassPaths jikes_class_paths;
				pjm->getExpandClasspath(jikes_class_paths, actual_point->proj);
				INameEnvironment* nameEnv = new INameEnvironment(option, jikes_class_paths);
				auto compiler = new JavaModel::BraveCompiler(nameEnv, option);
				auto type_part = boost::join(actual_point->enclosing_types, L"/");
				auto pkg_part  = boost::replace_all_copy(actual_point->pkg, L".",L"/");
				string qualifiedName = CharOperation::ConvertUnicodeToUtf8(pkg_part + L"/"  + type_part );
				auto binding =compiler->lookupEnvironment->getType(qualifiedName);
				collectSuperTypeNames(binding);
				delete compiler;
			}
			else{

				Option* option = new Option();
				option->bytecode = false;
				JikesClassPaths jikes_class_paths;
				pjm->getExpandClasspath(jikes_class_paths, actual_point->proj);
				INameEnvironment* nameEnv = new INameEnvironment(option, jikes_class_paths);
				auto compiler = new JavaModel::BraveCompiler(nameEnv, option);
				source_file = actual_point->root;
				source_file += L"/" + boost::replace_all_copy(actual_point->pkg, L".", L"/");
				source_file += L"/" + actual_point->file + L".java";
				compiler->add_file_set.insert(source_file);
				compiler->Compile();
				if (!compiler->lookupEnvironment->input_files.size())
					return;
				FileSymbol* unit = compiler->lookupEnvironment->input_files[0];
				auto parsedUnit = unit->compilation_unit;
				if (!parsedUnit)
					return;
				auto visitor = new TypeDeclarationVisitor(this);
				parsedUnit->traverse(visitor, nullptr);
				delete visitor;
				delete compiler;
			}
		}
	}
}


} // Close namespace Jikes block


