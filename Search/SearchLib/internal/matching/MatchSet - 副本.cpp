#include "stdafx.h"
#include "MatchSet.h"
#include "MatchLocator.h"
#include <SearchLib/internal/pattern/SearchPattern.h>
#include "MatchLocatorParserVisitor.h"
#include <SearchLib/internal/core/IJavaSearchResultCollector.h>
#include <JavaModelLib/internal/core/IJavaElementType.h>
#include <JCDT_Lib/internal/ast/AstAnonymousTypeDeclaration.h>
#include <JavaModelLib/internal/core/IJavaElement.h>
#include <JavaModelLib/internal/core/IType.h>
#include <JavaModelLib/internal/core/IField.h>
#include <JavaModelLib/internal/core/IMethod.h>
#include <JavaModelLib/internal/core/IInitializer.h>
#include <JavaModelLib/internal/core/CompilationUnit.h>
#include <JavaModelLib/internal/core/IImportDeclaration.h>

#include "MatchingOpenable.h"
#include <JavaModelLib/internal/core/ClassFile.h>
#include <JCDT_Lib/internal/lookup/TypeSymbol.h>
#include <JavaModelLib/internal/lookup/NameLookup.h>
#include "LocatorCompiler.h"
#include "SearchResultNode.h"

using namespace Jikes::JavaModel;

namespace Jikes { // Open namespace Jikes block
	namespace Search {
							 
	void MatchSet::getResult(FileResultElement* copyer,bool _copy_children)
	{
		if (!copyer)
			return;
		vector<MemberInFileElemnt*>& result_in_file = copyer->result_in_file;
		result_in_file.reserve(result.size());
		if(_copy_children)
		{
			for (auto it : result) 
			{

				if (!it.first)
					continue;

				MemberInFileElemnt* a_element = new MemberInFileElemnt();
				a_element->element = it.first->DeepCopy(it.first->getParent());
				copyer->count += it.second.size();
				a_element->result_in_member.swap(it.second);
				result_in_file.emplace_back(a_element);
			}
		}
		else
		{
			for (auto it : result) 
			{
				MemberInFileElemnt* a_element = new MemberInFileElemnt();
				auto java_element = it.first;

				if (!java_element)
					continue;

				if(java_element->fLEType == IJavaElementType::TYPE){
					auto _type_element =  reinterpret_cast<IType*>(java_element);
					a_element->element = _type_element->DeepCopyButNoChildren(java_element->getParent());
				}
				else{
					a_element->element = java_element->DeepCopy(java_element->getParent());
				}
				
				copyer->count += it.second.size();
				a_element->result_in_member.swap(it.second);
				result_in_file.emplace_back(a_element);
			}
		}
	}

	bool MatchSet::LocalDeclarationVisitor::visit(TypeDeclaration* node, AstNodeScope* scope)
	{
		_set->reportMatching(node, enclosingElement);
		return false;
	}

	

	MatchSet::MatchSet(MatchingOpenable* _opnable):opnable(_opnable), lex(nullptr){
		pattern = (_opnable->pattern);
		matchContainer = pattern->matchContainer();
	}

	MatchSet::~MatchSet()
	{
		
	}
	bool lessStartLocation(std::pair< Ast*, int>& left, std::pair< Ast*, int>& right)
	{
		if (left.second < right.second)
			return true;
		else if(left.second == right.second)
		{
			auto left_node = left.first;
			auto right_node = right.first;
			if (left_node && right_node 
				&& left_node->RightToken() && right_node->RightToken() &&
				left_node->RightToken()->EndLocation() > right_node->RightToken()->EndLocation())
			{
					return true;
			}
				
			else
				return  false;
		}
		return false;
	}
	std::vector<std::pair< Ast*, int> >* MatchSet::nodesInRange(int start, int end, boost::unordered_map<Ast*, int>& set)
	{
		// collect nodes in the given range
		std::vector< std::pair< Ast*, int> >*result = nullptr;
		for(auto it : set)
		{
			auto node = it.first;
			if (!node|| !node->LeftToken())
				continue;

			int start_loc = node->LeftToken()->start_location;
			if(start_loc >= start)
			{
				if(node->RightToken() && node->RightToken()->EndLocation() <= end)
				{
					if (!result){
						result = new std::vector<std::pair< Ast*, int> >;
					}
					result->emplace_back( make_pair(node, start_loc) );
				}
			}
		}
		if(result && result->size() > 1)
			std::sort(result->begin(), result->end(), lessStartLocation);

		return result;
	}

	void MatchSet::reportMatching(AbstractMethodDeclaration* method, JavaModel::IJavaElement* parent){
		if (!method)
			return;

		auto find_it = _matchingNodes.find(method);
		if (find_it != _matchingNodes.end()) {
			if ((matchContainer & SearchPattern::CLASS) != 0) {
				reportMethodDeclaration(
					method,
					parent,
					find_it->second == SearchPattern::ACCURATE_MATCH ?
					IJavaSearchResultCollector::EXACT_MATCH :
					IJavaSearchResultCollector::POTENTIAL_MATCH);
			}
			_matchingNodes.erase(find_it);
		}


		// references in this method
		auto left_token = method->LeftToken();
		auto right_token = method->RightToken();
		if(!(left_token && right_token))
		{
			return;
		}
		auto nodes= matchingNodes(
			left_token->start_location,
			right_token->EndLocation()) ;
		
		if(nodes)
		{
			auto num = nodes->size();
			auto node_data = nodes->data();
			for (int i = 0; i < num; i++) 
			{

				auto node = node_data[i];
				find_it = _matchingNodes.find(node.first);
				if(_matchingNodes.end() != find_it)
				{
					if ((matchContainer & SearchPattern::METHOD) != 0) {
						reportReference(
							node.first,
							method,
							parent,
							find_it->second == SearchPattern::ACCURATE_MATCH ?
							IJavaSearchResultCollector::EXACT_MATCH :
							IJavaSearchResultCollector::POTENTIAL_MATCH);
					}
					_matchingNodes.erase(find_it);
				}
			
			}
			delete nodes;
		}
		AstMethodBody* body;
		if(method->kind == Ast::METHOD)
		{
			auto deviced_md = reinterpret_cast<MethodDeclaration*>(method);
			body = deviced_md->method_body_opt;
			if (!body)
				return;

			if (body->HasLocalTypeMASK) {

				auto localDeclarationVisitor = new LocalDeclarationVisitor();
				localDeclarationVisitor->enclosingElement =
				(parent->fLEType == JavaModel::IJavaElementType::TYPE) ?
				createMethodHandle(method, (JavaModel::IType*)parent) : parent;
			
				method->traverse(localDeclarationVisitor, nullptr);
				
			}
			if (!cuHasBeenResolved)
			{
				if (HasNodeInpotentialMatchingNodes(method->BodyStart(), method->BodyEnd()) == false) {
					// no need to resolve the statements in the method
					deviced_md->method_body_opt = nullptr;
				}
			}
		}
		else
		{
			auto deviced_md = reinterpret_cast<ConstructorDeclaration*>(method);
			body = deviced_md->constructor_body;
			if (!body)
				return;
			if (body->HasLocalTypeMASK != 0) {

				auto localDeclarationVisitor = new LocalDeclarationVisitor();
				localDeclarationVisitor->enclosingElement =
					(parent->fLEType == JavaModel::IJavaElementType::TYPE) ?
					createMethodHandle(method, (JavaModel::IType*)parent) :
					parent;

				method->traverse(localDeclarationVisitor, nullptr);	
			}
			if(!cuHasBeenResolved)
			{
				if (HasNodeInpotentialMatchingNodes(method->BodyStart(), method->BodyEnd()) == false) {
					// no need to resolve the statements in the method
					deviced_md->constructor_body->Reset();
				}
			}
		}
	}

	void MatchSet::reportMatching(CompilationUnitDeclaration* unit)
	{
		if (!unit)
			return;

		if (cuHasBeenResolved) {
			for(auto it : _potentialMatchingNodes)
			{
				auto node = it.first;
				auto level = pattern->matchLevel(node, true);
				if (level == SearchPattern::ACCURATE_MATCH || level == SearchPattern::INACCURATE_MATCH) {
					_matchingNodes.insert(   make_pair(node,level) );
				}
			}
		}
		auto end = _matchingNodes.end();
		if (unit->type_declarations)
		{
			// types
			AstArray<TypeDeclaration*>& types = *(unit->type_declarations);

			for (int i = 0; i < types.Length(); i++) {
				auto type = types[i];
				reportMatching(type, nullptr);
			}

		}
		auto pkg = unit->package_declaration_opt;
		if (pkg)
		{
			auto find_it = _matchingNodes.find(pkg);
			if (find_it != end) {
				if ((matchContainer & SearchPattern::COMPILATION_UNIT) != 0) {
					reportPackageDeclaration(unit->package_declaration_opt);
				}
				_matchingNodes.erase(find_it);
			}
		}

		if (unit->import_declarations){
			AstArray<AstImportDeclaration*>& imports = *(unit->import_declarations);
			auto length = imports.Length();
			for (int i = 0; i < length; i++)
			{
				auto* importRef = imports[i];

				if (!importRef)
					continue;

				auto find_it = _matchingNodes.find(importRef->name);
				if (find_it != end) {
					if ((matchContainer & SearchPattern::COMPILATION_UNIT) != 0) {
						reportImport(
							importRef,
							find_it->second == SearchPattern::ACCURATE_MATCH ?
							IJavaSearchResultCollector::EXACT_MATCH :
							IJavaSearchResultCollector::POTENTIAL_MATCH);
					}
					_matchingNodes.erase(find_it);
				}
			}
		}
	}

	void MatchSet::reportMatching(AstVariableDeclarator* field, JavaModel::IJavaElement* parent, TypeDeclaration* type){

		if (!field)
			return;

		auto find_it = _matchingNodes.find(field);
		if (find_it != _matchingNodes.end()) {
			if ((matchContainer & SearchPattern::CLASS) != 0) {
				reportFieldDeclaration(
					field,
					parent,
					find_it->second == SearchPattern::ACCURATE_MATCH ?
					IJavaSearchResultCollector::EXACT_MATCH :
					IJavaSearchResultCollector::POTENTIAL_MATCH);
			}
			_matchingNodes.erase(find_it);
		}
		auto left_token = field->LeftToken();
		auto right_token = field->RightToken();

		if (!left_token || !right_token)
			return;

		auto nodes = matchingNodes(left_token->start_location, right_token->EndLocation());
		if(nodes)
		{
			auto num = nodes->size();
			auto data_nodes = nodes->data();
			for (int i = 0; i < num; i++) {
				auto node = data_nodes[i];
				find_it = _matchingNodes.find( node.first );
				if (find_it != _matchingNodes.end())
				{
					if ((matchContainer & SearchPattern::FIELD) != 0) {
						reportReference(
							node.first,
							type,
							field,
							parent,
							find_it->second == SearchPattern::ACCURATE_MATCH ?
							IJavaSearchResultCollector::EXACT_MATCH :
							IJavaSearchResultCollector::POTENTIAL_MATCH);
						_matchingNodes.erase(find_it);
					}
				}
			}

			delete nodes;
		}
		
		if(field->variable_initializer_opt && field->variable_initializer_opt->kind == Ast::CLASS_CREATION)
		{
			auto localDeclarationVisitor = new LocalDeclarationVisitor();

			if (!parent)
				return;

			if (parent->fLEType == JavaModel::IJavaElementType::TYPE)
				localDeclarationVisitor->enclosingElement = createFieldHandle(field, (JavaModel::IType*)parent);
			else
				localDeclarationVisitor->enclosingElement =  parent;
			
				field->traverse(localDeclarationVisitor, nullptr);
			
			
		}
	}
	void MatchSet::reportMatching(AstType* fieldType, JavaModel::IJavaElement* parent,
		TypeDeclaration* type)
	{
		if (!fieldType)
			return;
		auto find_it = _matchingNodes.find(fieldType);
		if (find_it != _matchingNodes.end()) {
			if ((matchContainer & SearchPattern::CLASS) != 0) {
				reportFieldDeclaration(
					fieldType,
					parent,
					find_it->second == SearchPattern::ACCURATE_MATCH ?
					IJavaSearchResultCollector::EXACT_MATCH :
					IJavaSearchResultCollector::POTENTIAL_MATCH);
			}
			_matchingNodes.erase(find_it);
		}
		auto left_token = fieldType->LeftToken();
		auto right_token = fieldType->RightToken();

		if (!left_token || !right_token)
			return;

		auto nodes = matchingNodes(left_token->start_location, right_token->EndLocation());
		if (nodes)
		{
			auto num = nodes->size();
			auto data_nodes = nodes->data();
			for (int i = 0; i < num; i++) {
				auto node = data_nodes[i];
				find_it = _matchingNodes.find(node.first);
				if (find_it != _matchingNodes.end())
				{
					if ((matchContainer & SearchPattern::FIELD) != 0) {
						reportReference(
							node.first,
							type,
							fieldType,
							parent,
							find_it->second == SearchPattern::ACCURATE_MATCH ?
							IJavaSearchResultCollector::EXACT_MATCH :
							IJavaSearchResultCollector::POTENTIAL_MATCH);
						_matchingNodes.erase(find_it);
					}
				}
			}

			delete nodes;
		}
	}
	void MatchSet::reportMatching(Initializer* initializer, JavaModel::IJavaElement* parent, TypeDeclaration* type)
	{
		if (!initializer)
			return;
		auto left_token = initializer->LeftToken();
		auto right_token = initializer->RightToken();

		if (!left_token || !right_token)
			return;
		auto nodes = matchingNodes(left_token->start_location, right_token->EndLocation());
		if (nodes)
		{
			auto num = nodes->size();
			auto data_nodes = nodes->data();
			for (int i = 0; i < num; i++) {
				auto node = data_nodes[i];
				auto find_it = _matchingNodes.find( node.first );
				if (find_it != _matchingNodes.end())
				{
					if ((matchContainer & SearchPattern::FIELD) != 0) {

						reportReference(
							node.first,
							type,
							initializer,
							parent,
							find_it->second == SearchPattern::ACCURATE_MATCH ?
							IJavaSearchResultCollector::EXACT_MATCH :
							IJavaSearchResultCollector::POTENTIAL_MATCH);
						_matchingNodes.erase(find_it);

					}
				}
			}

			delete nodes;
		}

		if (initializer->block && initializer->block->HasLocalTypeMASK )
		{
			if (!parent)
				return;

			auto localDeclarationVisitor = new LocalDeclarationVisitor();
			if (parent->fLEType == JavaModel::IJavaElementType::TYPE)
				localDeclarationVisitor->enclosingElement = createInitializerHandle(initializer, (JavaModel::IType*)parent);
			else
				localDeclarationVisitor->enclosingElement = parent;
			initializer->traverse(localDeclarationVisitor, nullptr);
		}
	}

	void MatchSet::reportMatching(TypeDeclaration* type, JavaModel::IJavaElement* parent)
	{
		if (!type)
			return;

		auto end = _matchingNodes.end();
		auto find_it = _matchingNodes.find(type);
		if (find_it != end) {
			if ((matchContainer & SearchPattern::CLASS) != 0) {
				reportTypeDeclaration(
					type,
					parent,
					find_it->second == SearchPattern::ACCURATE_MATCH ?
					IJavaSearchResultCollector::EXACT_MATCH :
					IJavaSearchResultCollector::POTENTIAL_MATCH);
			}
			_matchingNodes.erase(find_it);
		}


		
		JavaModel::IJavaElement* enclosingElement;
		if (parent == nullptr) {
			enclosingElement = createTypeHandle(type->getName());
		}
		else if (parent->fLEType == JavaModel::IJavaElementType::TYPE) {
			enclosingElement = createTypeHandle((JavaModel::IType*)parent, type->getName());
			if (enclosingElement == nullptr) return;
		}
		else {
			enclosingElement = parent;
		}

		auto class_body = type->class_body;
		if (!class_body)
			return;

		unsigned i, num;
		for (i = 0, num = class_body->NumNestedClasses(); i < num; i++)
		{
			ClassTypeDeclaration* memberType = class_body->NestedClass(i);
			reportMatching(memberType, enclosingElement);
		}

		for (i = 0, num = class_body->NumNestedEnums(); i < num; i++)
		{
			EnumTypeDeclaration* memberType = class_body->NestedEnum(i);
			reportMatching(memberType, enclosingElement);
		}
		for (i = 0, num = class_body->NumNestedInterfaces(); i < num; i++)
		{
			InterfaceTypeDeclaration* memberType = class_body->NestedInterface(i);
			reportMatching(memberType, enclosingElement);
		}

		for (i = 0, num = class_body->NumNestedAnnotations(); i < num; i++)
		{
			AnnotationTypeDeclaration* memberType = class_body->NestedAnnotation(i);
			reportMatching(memberType, enclosingElement);
		}


		for (i = 0, num = class_body->NumMethods(); i < num; i++) {
			auto method = class_body->Method(i);		
			reportMatching(method, enclosingElement);
		}

		for (i = 0, num = class_body->NumConstructors(); i < num; i++) {
			auto method = class_body->Constructor(i);
			reportMatching(method, enclosingElement);
		}

		for (i = 0, num = class_body->NumStaticInitializers(); i < num; i++){
			auto initialize = class_body->StaticInitializer(i);
			reportMatching(initialize, enclosingElement, type);
		}

		for (i = 0, num = class_body->NumInstanceInitializers(); i < num; i++){
			auto initialize = class_body->InstanceInitializer(i);
			reportMatching(initialize, enclosingElement, type);
		}

		for (i = 0, num = class_body->NumInstanceVariables(); i < num; i++){
			FieldDeclaration* field_declaration = class_body->InstanceVariable(i);

			if (!field_declaration)
				continue;
			field_declaration->type
			auto num_vairable = field_declaration->NumVariableDeclarators();
			for (unsigned k = 0; k < num_vairable; k++) {
				AstVariableDeclarator* declarator =
					field_declaration->VariableDeclarator(k);
				reportMatching(declarator, enclosingElement, type);
			}
		}

		for (i = 0, num = class_body->NumClassVariables(); i < num; i++){
			FieldDeclaration* field_declaration = class_body->ClassVariable(i);
			if (!field_declaration)
				continue;
			auto num_vairable = field_declaration->NumVariableDeclarators();
			for (unsigned k = 0; k < num_vairable; k++) {
				auto declarator = field_declaration->VariableDeclarator(k);
				reportMatching(declarator, enclosingElement, type);
			}
		}

		// super types
		auto   type_kind =  type->getTypeDeclarationKind();
		if (TypeDeclaration::ANONYMOUS_TYPE_DECLARE == type_kind ) {
			auto  actual_type = reinterpret_cast<AstAnonymousTypeDeclaration*>(type);
		
			if (actual_type->create_expr && actual_type->create_expr->class_type)
			{
				auto superType = actual_type->create_expr->class_type;
				find_it = _matchingNodes.find(superType->name);
				if (find_it != end) {
					if ((matchContainer & SearchPattern::CLASS) != 0) {
						reportSuperTypeReference(
							superType,
							enclosingElement,
							find_it->second == SearchPattern::ACCURATE_MATCH ?
							IJavaSearchResultCollector::EXACT_MATCH :
							IJavaSearchResultCollector::POTENTIAL_MATCH);
					}
					_matchingNodes.erase(find_it);
				}
			}
		}
		else if(TypeDeclaration::INTERFACE_DECL == type_kind)
		{
			auto  actual_type = reinterpret_cast<InterfaceTypeDeclaration*>(type);
			for(i=0, num = actual_type->NumInterfaces(); i < num; ++i)
			{
				auto superType = actual_type->Interface(i);
				if (!superType)
					continue;

				find_it = _matchingNodes.find( superType->name );
				if (find_it != end) {
					if ((matchContainer & SearchPattern::CLASS) != 0) {
						reportSuperTypeReference(
							superType,
							enclosingElement,
							find_it->second == SearchPattern::ACCURATE_MATCH ?
							IJavaSearchResultCollector::EXACT_MATCH :
							IJavaSearchResultCollector::POTENTIAL_MATCH);
					}
					_matchingNodes.erase(find_it);
				}
			}

			
		}
		else if (TypeDeclaration::CLASS_DECL == type_kind)
		{
			auto  actual_type = reinterpret_cast<ClassTypeDeclaration*>(type);
			auto superType = actual_type->super_opt;
			if(superType)
			{
				find_it = _matchingNodes.find(superType->name);
				if (find_it != end) {
					if ((matchContainer & SearchPattern::CLASS) != 0) {
						reportSuperTypeReference(
							superType,
							enclosingElement,
							find_it->second == SearchPattern::ACCURATE_MATCH ?
							IJavaSearchResultCollector::EXACT_MATCH :
							IJavaSearchResultCollector::POTENTIAL_MATCH);
					}
					_matchingNodes.erase(find_it);
				}
			}
			for (i = 0, num = actual_type->NumInterfaces(); i < num; ++i)
			{
				auto superType = actual_type->Interface(i);

				if (!superType)
					continue;

				find_it = _matchingNodes.find(superType->name);
				if (find_it != end) {
					if ((matchContainer & SearchPattern::CLASS) != 0) {
						reportSuperTypeReference(
							superType,
							enclosingElement,
							find_it->second == SearchPattern::ACCURATE_MATCH ?
							IJavaSearchResultCollector::EXACT_MATCH :
							IJavaSearchResultCollector::POTENTIAL_MATCH);
					}
					_matchingNodes.erase(find_it);
				}
			}

		}
	}
	JavaModel::IType* MatchSet::lookupType(TypeSymbol* typeBinding)
	{
		if (!typeBinding)
			return nullptr;

		auto temp = typeBinding->ContainingPackageName();
		auto packageName = wstring(temp);
		
	
		wstring typeName = typeBinding->getCompoundName();

		auto idx = typeName.find(packageName);
		if(idx != wstring::npos){
			typeName = typeName.substr(packageName.size(), typeName.size());
		}
		boost::replace_all(packageName, L"/", L".");
		if (packageName.length() == 0)
			packageName = IPackageFragment::DEFAULT_PACKAGE_NAME;

		// find package fragments
		NameLookup* nameLookup = opnable->compiler.getNameLookup();
		if(nameLookup)
		{
			auto
				pkgs = nameLookup->findPackageFragments(packageName, false);

			// iterate type lookup in each package fragment
			for (int i = 0, length = pkgs.size(); i < length; i++)
			{
				IType* type =
					nameLookup->findType(
						typeName,
						pkgs[i],
						false,
						typeBinding->ACC_INTERFACE() ?
						NameLookup::ACCEPT_INTERFACES :
						NameLookup::ACCEPT_CLASSES);

				if (type != nullptr)
					return type;
			}
		}
		
		// search inside enclosing element
		vector<wstring>	qualifiedName;
		
		 boost::split(qualifiedName, typeName, boost::is_any_of(L"."));

		int length = qualifiedName.size();
		if (length == 0) 
			return nullptr;
		IType *type = createTypeHandle(qualifiedName[0]);

		if (type == nullptr) return nullptr;

		for (int i = 1; i < length; i++){
			type = createTypeHandle(type, qualifiedName[i]);
			if (type == nullptr) 
				return nullptr;
		}
		 return type;
	}

	void MatchSet::reportImport(ImportReference* reference, int accuracy)
	{
		if (!reference)
			return;

		// create defining import handle
		IJavaElement* importHandle = createImportHandle(reference);

		if(reference->kind == Ast::IMPORT)
		{
			auto name = reinterpret_cast<AstImportDeclaration*>(reference)->name;
			if (!name || !name->identifier_token)
				return;

			auto name_token = name->identifier_token;
			
			report(name_token->start_location, name_token->EndLocation(), importHandle, accuracy,0);
		}
		// accept reference
	}

	void MatchSet::reportMethodDeclaration(AbstractMethodDeclaration* methodDeclaration,
		JavaModel::IJavaElement* parent, int accuracy)
	{
		if (!parent || !methodDeclaration)
			return;

		IJavaElement* enclosingElement;
		if (parent->fLEType == TYPE) {
			// create method handle
			enclosingElement = createMethodHandle(methodDeclaration, (IType*)parent);
			if (enclosingElement == nullptr) return;
		}
		else {
			enclosingElement = parent;
		}
		const auto range = methodDeclaration->getNameRange();
		
		// accept method declaration
		report(range.first, range.second, enclosingElement, accuracy,0);
	}

	void MatchSet::reportReference(Ast* reference, AbstractMethodDeclaration* methodDeclaration,
		JavaModel::IJavaElement* parent, int accuracy)
	{
		if (!parent)
			return;

			IJavaElement * enclosingElement;
			if (parent->fLEType == IJavaElementType::TYPE){
				// create defining method handle
				enclosingElement = createMethodHandle(methodDeclaration, (IType*)parent);
				if (enclosingElement == nullptr)
					return; // case of a match found in a type other than the current class file
			}
			else
			{
				enclosingElement = parent;
			}

		// accept reference
		pattern->matchReportReference(reference, enclosingElement, accuracy, this);
	}

	void MatchSet::reportReference(Ast* reference, 
		TypeDeclaration* typeDeclaration, AstVariableDeclarator* fieldDeclaration,
		JavaModel::IJavaElement* parent, int accuracy)
	{
		if (!fieldDeclaration ||!parent)
			return;

		IJavaElement* enclosingElement;
		if (fieldDeclaration->isField())
		{
			if (parent->fLEType == IJavaElementType::TYPE)
			{
				// create defining field handle
				enclosingElement = createFieldHandle(fieldDeclaration, (IType*)parent);
				if (enclosingElement == nullptr) return;
			}
			else
			{
				enclosingElement = parent;
			}

			// accept reference
			pattern->matchReportReference(reference, enclosingElement, accuracy, this);
		}
		else
		{ 
			
		}
	}


	 IJavaElement* MatchSet::createImportHandle(ImportReference *importRef) {

		 if (!importRef)
			 return nullptr;

		 wstring compoundName;
		if(importRef->kind == Ast::IMPORT){
			auto import_ = reinterpret_cast<AstImportDeclaration*>(importRef);
			compoundName = import_->getFullName();
		}
		else{
			compoundName = importRef->getImportName();
		}
		auto currentOpenable = opnable->openable;
		if (!currentOpenable)
			return nullptr;
		if (   currentOpenable->fLEType == JavaModel::IJavaElementType::COMPILATION_UNIT
			|| currentOpenable->fLEType == JavaModel::IJavaElementType::JAR_COMPILATION_UNIT) {

			// creates compilation unit
			auto unit = reinterpret_cast<CompilationUnit*> (currentOpenable);
			// create type
			return unit->getImport(compoundName);

		}
		else{
			auto unit = reinterpret_cast<JavaModel::ClassFile*> (currentOpenable);
			return unit->getType();
		}

	}
	 IInitializer* MatchSet::createInitializerHandle(
		 Initializer* initializer,IType* type) 
	 {
		if (!type || !initializer)
			return nullptr;
		// find occurence count of the given initializer in its type declaration
		int loc = 0;
		if(initializer->LeftToken())
		{
			loc = initializer->LeftToken()->start_location;
		}
		return type->getInitializer(loc);


	}
	void MatchSet::reportReference(Ast* reference, TypeDeclaration* typeDeclaration, Initializer* initializer, 
		JavaModel::IJavaElement* parent, int accuracy)
	{
		IJavaElement* enclosingElement;

		if (!parent)
			return;

		// initializer
		if (parent->fLEType == IJavaElementType::TYPE){
			// create defining initializer
			enclosingElement =
				createInitializerHandle(
					initializer,
					(IType*)parent);
			if (enclosingElement == nullptr)
				return;
		}
		else{
			enclosingElement = parent;
		}

		// accept reference
		pattern->matchReportReference(reference, enclosingElement, accuracy, this);

	}

	void MatchSet::reportSuperTypeReference(AstType* typeRef, JavaModel::IJavaElement* type, int accuracy)
	{
		// accept type reference
		pattern->matchReportReference(typeRef, type, accuracy, this);
	}

	void MatchSet::reportBinaryMatch(JavaModel::Member* binaryMember, int accuracy)
	{
		if (!binaryMember)
			return;
		JavaModel::ISourceRange* _rng = binaryMember->getSourceRange();
		JavaModel::ISourceRange* rng = nullptr;
		if (_rng){
			rng = _rng->Clone();
		}
		result[binaryMember].emplace_back(new SearchMatch(rng, accuracy, L"",0));
	}

	void MatchSet::report(Ast* reference, JavaModel::IJavaElement* element, int accuracy, int extra_info)
	{
		if (!reference)
			return;

		report(reference->sourceStart(), reference->sourceEnd(), element, accuracy, extra_info);
	}

	void MatchSet::report(int sourceStart,int sourceEnd, JavaModel::IJavaElement* element, int accuracy,int extra_info)
	{
		if(!element){
			return;
		}
		if(lex){
			auto  left_line_no = lex->LineFromPos(sourceStart);
			auto left_column_no = lex->ColumnFromPos(sourceStart);
			auto  right_line_no = lex->LineFromPos(sourceEnd);
			auto  right_column_no = lex->ColumnFromPos(sourceEnd);
			auto rng = new ISourceRange(left_column_no, left_line_no, right_column_no, right_line_no);
			ErrorString s;
			lex->OutputLine(left_line_no, s);
			result[element].emplace_back(new SearchMatch(rng, accuracy, s.ArrayString(), extra_info));
		

		}
	}

	void MatchSet::reportFieldDeclaration(AstVariableDeclarator* fieldDeclaration,
		JavaModel::IJavaElement* parent, int accuracy)
	{

		if (!fieldDeclaration || !parent || !fieldDeclaration->variable_declarator_name)
			return;

		// accept field declaration
		auto token = fieldDeclaration->variable_declarator_name->identifier_token;
		report(
			token->start_location ,
			token->EndLocation(),
			(parent->fLEType == IJavaElementType::TYPE) ?
			((IType*)parent)->getField(fieldDeclaration->getName()) :
			parent,
			accuracy,0
		)
			;
	}

	void MatchSet::reportTypeDeclaration(TypeDeclaration* typeDeclaration, JavaModel::IJavaElement* parent, int accuracy)
	{

		if (!typeDeclaration || !parent || !typeDeclaration->class_body)
			return;

		// accept class or interface declaration
		auto  token = typeDeclaration->class_body->identifier_token;
		if  (!token)
			return;

		report(
			token->start_location,
			token->EndLocation(),
			(parent == nullptr) ?
			  createTypeHandle(typeDeclaration->getName()) :
			(parent->fLEType == IJavaElementType::TYPE) ?
			 createTypeHandle((IType*)parent, typeDeclaration->getName()) :
			parent,
			accuracy,0
		)
			;
	}

	JavaModel::IType* MatchSet::createTypeHandle(const wstring& simpleTypeName)
	{
		auto currentOpenable = opnable->openable;

		if (!currentOpenable)
			return nullptr;

		if (currentOpenable->fLEType == JavaModel::IJavaElementType::COMPILATION_UNIT
			|| currentOpenable->fLEType == JavaModel::IJavaElementType::JAR_COMPILATION_UNIT){
			// creates compilation unit
			auto unit = (JavaModel::CompilationUnit*)currentOpenable;
			// create type
			return unit->getType(simpleTypeName);
		}
		else{
			
			IType* type =  reinterpret_cast<JavaModel::ClassFile*>(currentOpenable)->getType();
			/// ensure this is a top level type (see bug 20011  Searching for Inner Classes gives bad search results)
			if (!type)
				return nullptr;

			IType* declaringType = type->getDeclaringType();
			while (declaringType != nullptr){
				type = declaringType;
				declaringType = type->getDeclaringType();
			}
			return type;
		}
	}

	JavaModel::IType* MatchSet::createTypeHandle(JavaModel::IType* parent, const wstring& simpleTypeName)
	{
		if (!parent)
			return nullptr;
		return parent->getType(simpleTypeName);
	}
	JavaModel::IField* MatchSet::createFieldHandle(
		AstVariableDeclarator* field,	JavaModel::IType* type) {
	
		if (!type || !field)
			return nullptr;

		return type->getField(field->getName());

	}

	JavaModel::IMethod* MatchSet::createMethodHandle(AbstractMethodDeclaration* method, JavaModel::IType* type)
	{
		if (!type || !method)
			return nullptr;

		vector<wstring> parameterTypes;
		method->getParameterTypes(parameterTypes);
		return type->getMethod(method->selector(), parameterTypes);

	}
}


} // Close namespace Jikes block


