#include "stdafx.h"

#include <JCDT_Lib/internal/ast/ast.h>
#include <JavaModelLib/internal/core/SourceType.h>
#include <JavaModelLib/internal/core/SourceType.h>
#include <JavaModelLib/internal/core/Initializer.h>

#include <JavaModelLib/internal/core/SourceField.h>
#include <JavaModelLib/internal/core/SourceMethod.h>
#include <JavaModelLib/internal/core/PackageFragment.h>
#include <JCDT_Lib/internal/lookup/TypeSymbol.h>
#include <JCDT_Lib/internal/lookup/MethodSymbol.h>
#include <JCDT_Lib/internal/lookup/VariableSymbol.h>
#include <JCDT_Lib/internal/lookup/PackageSymbol.h>
#include  <boost/algorithm/string.hpp>
#include <JCDT_Lib/internal/lookup/FileSymbol.h>
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JavaModelLib/internal/core/ISourceRange.h>
#include <JCDT_Lib/internal/util/AccessFlagsUtil.h>


namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	


	void ProcessMethodMembers(
		SourceType* _type, 
		SourceTypeElementInfo* type_element_info, 
		const vector<wstring>& qualifiedTypeNames,
		LexStream* lex ,
		AbstractMethodDeclaration* astMethod,
		AstMethodDeclarator* declarator,bool is_constructor
		 ) {

		//MethodDeclaration or ConstructorDeclaration
		if(!declarator || !declarator->identifier_token  || !astMethod || !lex || !type_element_info)
		{
			return;
		}
		
		wstring method_selector = declarator->identifier_token->getName();
		vector<wstring> parameterNames;
		vector<wstring> parameterTypes;
		astMethod->getParameterTypesAndNames(parameterTypes, parameterNames);
		SourceMethod* source_method = new SourceMethod(_type, method_selector, parameterTypes);

		

		auto method_info = reinterpret_cast<SourceMethodElementInfo*>(source_method->getElementInfo());
		if (!method_info)
			return;
		method_info->paramenterNames = parameterNames;
		source_method->MarkConstructor(is_constructor);
		auto returnTypeNames = astMethod->getReturnTypeName();
		if (!returnTypeNames.empty())
			method_info->return_type = boost::join(returnTypeNames, L".");

		method_info->qualifiedTypeNames.assign(qualifiedTypeNames.begin(), qualifiedTypeNames.end());
		method_info->name= (method_selector);

		type_element_info->InsertMethodSymbol(source_method);
		if (declarator && lex)
		{
			auto right = astMethod->RightToken();
			auto left = astMethod->LeftToken();
			if (right && left) {
				auto  left_line_no = lex->LineFromPos(left->start_location);
				auto left_column_no = lex->ColumnFromPos(left->start_location);

				auto  right_line_no = lex->LineFromPos(right->start_location);
				auto  right_column_no = lex->RightColumnFromPos(right->EndLocation());
			
				auto rng  = new ISourceRange(left_column_no, left_line_no, right_column_no, right_line_no);
				source_method->setSourceRange(rng);
			}
			AccessFlagsUtil::GetModifer(method_info->modifiers, astMethod->modifiers_opt, lex);
			source_method->setAccessFlags(
				AccessFlagsUtil::ProcessModifiers(astMethod->modifiers_opt, 0).Flags()
			);


			auto name_identifier = declarator->identifier_token;

			auto  left_line_no = lex->LineFromPos(name_identifier->start_location);
			auto left_column_no = lex->ColumnFromPos(name_identifier->start_location);
			auto ene_pos = name_identifier->EndLocation();
			auto  right_line_no = lex->LineFromPos(ene_pos);
			auto  right_column_no = lex->RightColumnFromPos(ene_pos);
			auto name_rng = new ISourceRange(left_column_no, left_line_no, right_column_no, right_line_no);
			source_method->setNameRange(name_rng);

		}
		source_method->SetParamenterNames(parameterNames);
		if (astMethod->kind != Ast::CONSTRUCTOR)
		{
			auto method_type = reinterpret_cast<MethodDeclaration*>(astMethod)->type;
			if (method_type){	
				method_info->return_type =boost::join(method_type->getParameterizedTypeName(), L".");
			}	
		}
	 }
	

	void ProcessInitializers(SourceType* _type,
		SourceTypeElementInfo* type_element_info, 
		const vector<wstring>& qualifiedTypeNames,
		AstClassBody* declaration, LexStream* lex)
	{ 
		if (!declaration || !type_element_info || !lex)
			return;
		unsigned k = 0;
		auto num = declaration->NumStaticInitializers();
		while (k < num) {
			auto ast_node = declaration->StaticInitializer(k++);
			auto left = ast_node->LeftToken();
			auto right = ast_node->RightToken();
			if (right && left) {
				auto  left_line_no = lex->LineFromPos(left->start_location);
				auto left_column_no = lex->ColumnFromPos(left->start_location);
				auto  right_line_no = lex->LineFromPos(right->start_location);
				auto  right_column_no = lex->RightColumnFromPos(right->EndLocation());
				Initializer* initializer = new Initializer(_type, left->start_location);

				auto rng = new ISourceRange(left_column_no, left_line_no, right_column_no, right_line_no);
				initializer->setSourceRange(rng);
				initializer->setNameRange(rng->Clone());

				initializer->getAccessFlags().SetACC_STATIC();
				type_element_info->InsertInitializer(initializer);

			}
		}
		k = 0;
		num = declaration->NumInstanceInitializers();
		while (k < num) {
			auto ast_node = declaration->InstanceInitializer(k++);
			auto left = ast_node->LeftToken();
			auto right = ast_node->RightToken();
			if (right && left) {
				auto  left_line_no = lex->LineFromPos(left->start_location);
				auto left_column_no = lex->ColumnFromPos(left->start_location);
				auto  right_line_no = lex->LineFromPos(right->start_location);
				auto  right_column_no = lex->RightColumnFromPos(right->EndLocation());
				Initializer* initializer = new Initializer(_type, left->start_location);

				auto rng = new ISourceRange(left_column_no, left_line_no, right_column_no, right_line_no);
				initializer->setSourceRange(rng);
				initializer->setNameRange(rng->Clone());

				type_element_info->InsertInitializer(initializer);
			}
		}
	}


	void ProcessFieldDeclaration(SourceType* _type, 
		SourceTypeElementInfo* type_element_info,
		const vector<wstring>& qualifiedTypeNames,
		FieldDeclaration* field_declaration, LexStream* lex)
	{
		if (!field_declaration  || !field_declaration->type || !type_element_info)
			return;

		auto access = AccessFlagsUtil::ProcessModifiers(field_declaration->modifiers_opt, 0);
		
		auto field_type_name = boost::join(field_declaration->type->getParameterizedTypeName(), L".");


		auto num = field_declaration->NumVariableDeclarators();
		for (unsigned i = 0; i < num; i++) {

			AstVariableDeclarator* declarator =
				field_declaration->VariableDeclarator(i);
			if (!declarator )
				continue;
			AstVariableDeclaratorId* name = declarator->variable_declarator_name;

			if (!name || !name->identifier_token)
				continue;
			unsigned dims = name->NumBrackets();
			for (size_t k = 0; k < dims; ++k) {
				field_type_name.append(L"[]");
			}


			auto fieldName = name->identifier_token->getName();
			SourceField* variable = new SourceField(_type, fieldName, FIELD);
				

			auto field_info =  reinterpret_cast<SourceFieldElementInfo*>(variable->getElementInfo());
			field_info->access.SetFlags(access.Flags());
			field_info->qualifiedTypeNames.assign(qualifiedTypeNames.begin(),
				qualifiedTypeNames.end());

			field_info->name = (fieldName);
			field_info->_type = field_type_name;
			AccessFlagsUtil::GetModifer(field_info->modifiers, field_declaration->modifiers_opt, lex);

			type_element_info->InsertField(variable);

			if (lex && declarator) 
			{
				auto left = declarator->LeftToken();
				auto right = declarator->RightToken();
				if (right && left) {
					auto  left_line_no = lex->LineFromPos(left->start_location);
					auto left_column_no = lex->ColumnFromPos(left->start_location);
					auto  right_line_no = lex->LineFromPos(right->start_location);
					auto  right_column_no = lex->RightColumnFromPos(right->EndLocation());
					auto rng = new ISourceRange(left_column_no, left_line_no, right_column_no, right_line_no);
					variable->setSourceRange(rng);
				}
				auto name_identifier = name->identifier_token;

				auto  left_line_no = lex->LineFromPos(name_identifier->start_location);
				auto left_column_no = lex->ColumnFromPos(name_identifier->start_location);
				auto ene_pos = name_identifier->EndLocation();
				auto  right_line_no = lex->LineFromPos(ene_pos);
				auto  right_column_no = lex->RightColumnFromPos(ene_pos);
				auto name_rng = new ISourceRange(left_column_no, left_line_no, right_column_no, right_line_no);
				variable->setNameRange(name_rng);
			}
			
		}
	}



	IJavaElement* SourceType::DeepCopy(IJavaElement* parent)
	{
		auto clone = reinterpret_cast<SourceType*>(ShallowCopy(parent));
		if(clone)
			clone->HelpToDeepCopy(*this);
		return clone;
	}
	

	IJavaElement* SourceType::ShallowCopy(IJavaElement* parent)
	{
		auto clone = new SourceType(*this);
		clone->fParent = parent;
	
		return clone;
	}

	SourceType::SourceType(IJavaElement* parent, const wstring& name)
	: BaseJavaType(parent, name)
		
	{
		HelpToDeepCopy(*this);
	}

	SourceType::SourceType(SourceType& o): 
		BaseJavaType(o)
	{
	}


	void SourceType::BuildStructure(SourceType* _type,
		SourceTypeElementInfo*type_element_info,
		TypeDeclaration* declaration,
		LexStream* lex)
	{
		if (!declaration || !lex || !_type || !type_element_info || !declaration->class_body){
			return;
		}
		if (declaration->getTypeDeclarationKind() == TypeDeclaration::INTERFACE_DECL)
			_type->MarkInterface();
		else
			_type->MarkClass();


		auto left = declaration->LeftToken();
		auto right = declaration->RightToken();
		if (right && left) {
			auto  left_line_no = lex->LineFromPos(left->start_location);
			auto left_column_no = lex->ColumnFromPos(left->start_location);
			auto  right_line_no = lex->LineFromPos(right->start_location);
			auto  right_column_no = lex->RightColumnFromPos(right->EndLocation());
			auto rng = new ISourceRange(left_column_no, left_line_no, right_column_no, right_line_no);
			_type->setSourceRange(rng);
		}
		type_element_info->fIsStructureKnown = true;
		AccessFlagsUtil::GetModifer(type_element_info->modifiers, declaration->modifiers_opt, lex);
		_type->setAccessFlags(AccessFlagsUtil::ProcessTopLevelTypeModifiers(declaration).Flags());
	
		auto class_body = declaration->class_body;


		auto name_identifier = class_body->identifier_token;
		if (!name_identifier)
			return;

		auto  left_line_no = lex->LineFromPos(name_identifier->start_location);
		auto left_column_no = lex->ColumnFromPos(name_identifier->start_location);
		auto ene_pos = name_identifier->EndLocation();
		auto  right_line_no = lex->LineFromPos(ene_pos);
		auto  right_column_no = lex->RightColumnFromPos(ene_pos);
		auto name_rng = new ISourceRange(left_column_no, left_line_no, right_column_no, right_line_no);
		type_element_info->setNameRange(name_rng);

		unsigned i, num;

		vector<wstring> qualifiedTypeNames(type_element_info->qualifiedTypeNames);
		qualifiedTypeNames.emplace_back(type_element_info->name);


		for (i = 0, num = class_body->NumNestedClasses(); i < num; i++)
		{
			ClassTypeDeclaration* decl = class_body->NestedClass(i);
			if (!decl || !decl->class_body || !decl->class_body->identifier_token)
				continue;

			auto name = decl->class_body->identifier_token->getName();
			auto inner_type = new SourceType(_type, name);
			inner_type->info = new SourceTypeElementInfo();
			inner_type->info->qualifiedTypeNames.assign(qualifiedTypeNames.begin(),
				qualifiedTypeNames.end());
			inner_type->info->name = (name);

			SourceType::BuildStructure(inner_type, inner_type->info, decl, lex);
			type_element_info->InsertNestedTypeSymbol(inner_type);
		}

		for (i = 0, num = class_body->NumNestedEnums(); i < num; i++)
		{
			EnumTypeDeclaration* decl = class_body->NestedEnum(i);

			if (!decl || !decl->class_body || !decl->class_body->identifier_token)
				continue;

			auto name = decl->class_body->identifier_token->getName();
			auto inner_type = new SourceType(_type, name);
			inner_type->info = new SourceTypeElementInfo();
			inner_type->info->qualifiedTypeNames.assign(qualifiedTypeNames.begin(),
				qualifiedTypeNames.end());
			inner_type->info->name = (name);

			SourceType::BuildStructure(inner_type, inner_type->info, decl, lex);
			type_element_info->InsertNestedTypeSymbol(inner_type);
		}
		for (i = 0, num = class_body->NumNestedInterfaces(); i < num; i++)
		{
			InterfaceTypeDeclaration* decl = class_body->NestedInterface(i);
			auto name = decl->class_body->identifier_token->getName();
			if (!decl || !decl->class_body || !decl->class_body->identifier_token)
				continue;
			auto inner_type = new SourceType(_type, name);
			inner_type->info = new SourceTypeElementInfo();
			inner_type->info->qualifiedTypeNames.assign(qualifiedTypeNames.begin(),
				qualifiedTypeNames.end());
			inner_type->info->name = (name);

			SourceType::BuildStructure(inner_type, inner_type->info, decl, lex);
			type_element_info->InsertNestedTypeSymbol(inner_type);
		}

		for (i = 0, num = class_body->NumNestedAnnotations(); i < num; i++)
		{
			AnnotationTypeDeclaration* decl = class_body->NestedAnnotation(i);
			if (!decl || !decl->class_body || !decl->class_body->identifier_token)
				continue;
			auto name = decl->class_body->identifier_token->getName();
			auto inner_type = new SourceType(_type, name);
			inner_type->info = new SourceTypeElementInfo();
			inner_type->info->qualifiedTypeNames.assign(qualifiedTypeNames.begin(),
				qualifiedTypeNames.end());
			inner_type->info->name = (name);

			SourceType::BuildStructure(inner_type, inner_type->info, decl, lex);
			type_element_info->InsertNestedTypeSymbol(inner_type);
		}


		for (i = 0, num = class_body->NumMethods(); i < num; i++) {
			auto md = class_body->Method(i);
			ProcessMethodMembers(_type, type_element_info, qualifiedTypeNames,
				lex, md, md->method_declarator,false);
		}

		for (i = 0, num = class_body->NumConstructors(); i < num; i++) {
			auto cd = class_body->Constructor(i);
			if (!cd)
				continue;

			ProcessMethodMembers(_type, type_element_info, qualifiedTypeNames, 
				lex, cd, cd->method_declarator,true);
		}

		for (i = 0, num = class_body->NumInstanceVariables(); i < num; i++)
			ProcessFieldDeclaration(_type, type_element_info, qualifiedTypeNames, 
				class_body->InstanceVariable(i), lex);

		for (i = 0, num = class_body->NumClassVariables(); i < num; i++)
			ProcessFieldDeclaration(_type, type_element_info, qualifiedTypeNames,
				class_body->ClassVariable(i), lex);

		ProcessInitializers(_type, type_element_info, qualifiedTypeNames,
			class_body, lex);

	}


	
}// Close namespace JavaModel block
	

} // Close namespace Jikes block




