#include "stdafx.h"
#include "JCDT_Lib/internal/env/platform.h"
#include "JCDT_Lib/internal/env/LookupEnvironment.h"
#include "JCDT_Lib/internal/impl/option.h"

#include "JCDT_Lib/internal/env/INameEnvironment.h"
#include <JCDT_Lib/internal/lookup/FileSymbol.h>
#include <JCDT_Lib/internal/lookup/VariableSymbol.h>

#include "JCDT_Lib/internal/lex/LexStream.h"

#include <JavaModelLib/internal/core/IMethod.h>
#include <JCDT_Lib/internal/lookup/MethodSymbol.h>
#include <JCDT_Lib/internal/lookup/PackageSymbol.h>
#include <JavaModelLib/internal/lookup/ISelectionRequestor.h>
#include <JavaModelLib/internal/core/JavaModel.h>
#include <JavaModelLib/internal/core/OriginalJavaProject.h>
#include <JavaModelLib/internal/lookup/NameLookup.h>
#include <JavaModelLib/internal/core/IType.h>
#include <JavaModelLib/internal/info/SourceFieldElementInfo.h>
#include <JavaModelLib/internal/core/IField.h>
#include "JavaModelLib/internal/core/SourceField.h"
#include <JCDT_Lib/internal/ast/ScopeUtil.h>
#include <JavaModelLib/internal/core/IInitializer.h>
#include "AstFound.h"
#include "FindLocalFieldOwnerVisitor.h"
#include "SelectUtil.h"
using namespace Jikes::JavaModel;

namespace Jikes {
	namespace CodeAssist {

	
		vector<JavaModel::IMethod*> SelectUtil::getSelectOnMethod(
			MethodSymbol* method_symbol,
			wstring selector, 
			JavaModel::ISelectionRequestor* requestor
			)
		{

			if (!method_symbol ||!requestor)
				return {};
			auto type_symbol = method_symbol->containing_type;
			if (!type_symbol)
				return {};
			auto package_symbol = type_symbol->ContainingPackage();
			if (!package_symbol)
				return vector<JavaModel::IMethod*>();

			wstring declaringTypePackageName;
			wstring declaringTypeName = type_symbol->qualifiedSourceName('.');

			vector<wstring> parameterPackageNames;
			vector<wstring> parameterTypeNames;
			const wchar_t* temp = package_symbol->PackageName();
			if (temp)
				declaringTypePackageName.assign(temp, temp + package_symbol->PackageNameLength());

		

			method_symbol->GetParameterNames(parameterPackageNames, parameterTypeNames);

			boost::replace_all(declaringTypePackageName, L"/", L".");

			auto _ret = requestor->getMethod(declaringTypePackageName, declaringTypeName, selector,
				parameterPackageNames, parameterTypeNames, method_symbol->IsConstructor());
			if (_ret.empty()) {
				_ret = requestor->getMethod(declaringTypePackageName, declaringTypeName, selector, method_symbol->IsConstructor());
			}
			return  _ret;
		}
		
		JavaModel::IType* SelectUtil::getSelectOnType(TypeSymbol* type_symbol, JavaModel::ISelectionRequestor* requestor)
		{
			if (!requestor || !type_symbol)
				return nullptr;

			const wchar_t* temp = nullptr;
			wstring declaringTypePackageName;
		
			vector<wstring> parameterPackageNames;
			vector<wstring> parameterTypeNames;
			auto package_symbol = type_symbol->ContainingPackage();
			if (package_symbol)
			{
				declaringTypePackageName =package_symbol->PackageNameString();
			}

			wstring declaringTypeName = type_symbol->qualifiedSourceName('.');

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

		IJavaElement* GetLocalFieldInMethodItem(VariableSymbol* field_symbol,
			AbstractMethodDeclaration* methodDecl,
			JavaModel::ISelectionRequestor* requestor, AstNodeScope*scope)
		{
			if (!field_symbol || !methodDecl || !requestor)
				return nullptr;

			auto method_symbol = methodDecl->method_symbol;
			if (!method_symbol)
				return nullptr;

			wstring selector = methodDecl->selector();
			auto methods = SelectUtil::getSelectOnMethod(method_symbol, selector,  requestor);

			if (methods.size())
			{
				auto method = methods[0];
				if (!method)
					return nullptr;

				wstring declaringTypeName;
				wstring name;

				const wchar_t*  temp = field_symbol->Name();
				if (temp)
					name.assign(temp, temp + field_symbol->NameLength());

				auto type_symbol_parameter = field_symbol->Type();

				if(!type_symbol_parameter)
				{
					return nullptr;
				}

				temp = type_symbol_parameter->Name();
				if (temp)
					declaringTypeName.assign(temp, temp + type_symbol_parameter->NameLength());


				wstring declaringTypePackageName;

				auto packae_symbol_parameter = type_symbol_parameter->ContainingPackage();
				if(packae_symbol_parameter)
				{
					temp = packae_symbol_parameter->PackageName();
					if (temp)
						declaringTypePackageName.assign(temp,temp + packae_symbol_parameter->PackageNameLength());
				}


				if (declaringTypePackageName.size()) {
					boost::replace_all(declaringTypePackageName, L"/", L".");
					declaringTypeName = declaringTypePackageName + L"." + declaringTypeName;
				}

				auto  local = method->getLocalField(name, declaringTypeName);
				if (!local) {

					local = new JavaModel::SourceField(method, name,
						JavaModel::IJavaElementType::LOCAL_FIELD);
					auto info = reinterpret_cast <JavaModel::SourceFieldElementInfo*>(local->getElementInfo());
					if(!info)
					{
						return nullptr;
					}
					Token* left = nullptr;
					Token* right = nullptr;
					LexStream* lex = nullptr;
					if(field_symbol->declarator)
					{
						left = field_symbol->declarator->LeftToken();
						right = field_symbol->declarator->RightToken();
					}
					const auto typeSymbol = field_symbol->ContainingType();
					if(typeSymbol)
					{
						FileSymbol* cunit = typeSymbol->file_symbol;
						if(cunit)
							lex =  cunit->lex_stream;
					}
					if(!left || !right || !lex || !field_symbol->declarator->variable_declarator_name)
					{
						return nullptr;
					}
					
					

					auto  left_line_no = lex->LineFromPos(left->start_location);
					auto left_column_no = lex->ColumnFromPos(left->start_location);
					auto  right_line_no = lex->LineFromPos(right->start_location);
					auto  right_column_no = lex->RightColumnFromPos(right->EndLocation());
					auto rng = new JavaModel::ISourceRange(left_column_no, left_line_no, right_column_no, right_line_no);
					info->setSourceRange(rng);

					auto name_identifier = field_symbol->declarator->variable_declarator_name->identifier_token;
					left_line_no = lex->LineFromPos(name_identifier->start_location);
					left_column_no = lex->ColumnFromPos(name_identifier->start_location);
					right_line_no = lex->LineFromPos(name_identifier->start_location);
					right_column_no = lex->RightColumnFromPos(name_identifier->EndLocation());
					rng = new JavaModel::ISourceRange(left_column_no, left_line_no, right_column_no, right_line_no);
					info->setNameRange(rng);

					method->addChild(local);
					local->SetType(declaringTypeName);

					if (field_symbol->initial_value) {
						info->initial_value = field_symbol->initial_value->Clone();
					}
				}

				return local;
			}
			return nullptr;
		}

		IJavaElement* GetLocalFieldInInitilize(VariableSymbol* field_symbol,
			JavaModel::ISelectionRequestor* requestor, AstNodeScope*scope)
		{
			if(!field_symbol || !requestor)
			{
				return nullptr;
			}
			const auto type_symbol = field_symbol->ContainingType();
			if (!type_symbol)
			{
				return nullptr;
			}
			auto _type = SelectUtil::getSelectOnType(type_symbol, requestor);


			const auto initializeBody = dynamic_cast<AstBlock*> (ScopeUtil::getInitializerBodyScop(scope));
			if (!(_type && initializeBody && initializeBody->owner_ast && initializeBody->owner_ast->kind == Ast::INITIALIZER))
			{
				return nullptr;
			}

			auto  initializer = reinterpret_cast<Initializer*>(initializeBody->owner_ast);
			if(!initializer || !initializer->LeftToken())
			{
				return nullptr;
			}
			auto initializer_element = _type->getInitializer(initializer->LeftToken()->start_location);
			if (!initializer_element) {
				return nullptr;
			}
			wstring declaringTypeName;
			wstring name;

			const wchar_t*  temp = field_symbol->Name();
			if (temp)
				name.assign(temp, temp + field_symbol->NameLength());

			const auto type_symbol_parameter = field_symbol->Type();
			if(!type_symbol_parameter)
			{
				return nullptr;
			}

			temp = type_symbol_parameter->Name();
			if (temp)
				declaringTypeName.assign(temp, temp + type_symbol_parameter->NameLength());

			wstring declaringTypePackageName;

			const auto packae_symbol_parameter = type_symbol_parameter->ContainingPackage();
			if(packae_symbol_parameter)
			{
				temp = packae_symbol_parameter->PackageName();
				if (temp)
					declaringTypePackageName.assign(temp,
						temp + packae_symbol_parameter->PackageNameLength());
			}


			if (!declaringTypePackageName.empty()) {
				boost::replace_all(declaringTypePackageName, L"/", L".");
				declaringTypeName = declaringTypePackageName + L"." + declaringTypeName;
			}

			auto  local = initializer_element->getLocalField(name, declaringTypeName);
			if (!local) {


				Token* left = nullptr;
				Token* right = nullptr;
				if(field_symbol->declarator && field_symbol->declarator->variable_declarator_name)
				{
					left = field_symbol->declarator->LeftToken();
					right = field_symbol->declarator->RightToken();
				}
				else
				{
					return nullptr;
				}
				LexStream* lex = nullptr;
				if(field_symbol->ContainingType())
				{
					auto cunit = field_symbol->ContainingType()->file_symbol;
					lex = cunit->lex_stream;
				}

				if(!lex || !left || !right)
				{
					return nullptr;
				}
				
				auto  left_line_no = lex->LineFromPos(left->start_location);
				auto left_column_no = lex->ColumnFromPos(left->EndLocation());
				auto  right_line_no = lex->LineFromPos(right->start_location);
				auto  right_column_no = lex->RightColumnFromPos(right->EndLocation());
				auto rng = new JavaModel::ISourceRange(left_column_no, left_line_no, right_column_no, right_line_no);

				local = new JavaModel::SourceField(initializer_element, name,
					JavaModel::IJavaElementType::LOCAL_FIELD);
				auto info = reinterpret_cast <JavaModel::SourceFieldElementInfo*>(local->getElementInfo());

				info->setSourceRange(rng);



				auto name_identifier = field_symbol->declarator->variable_declarator_name->identifier_token;
				left_line_no = lex->LineFromPos(name_identifier->start_location);
				left_column_no = lex->ColumnFromPos(name_identifier->start_location);
				right_line_no = lex->LineFromPos(name_identifier->start_location);
				right_column_no = lex->RightColumnFromPos(name_identifier->EndLocation());
				rng = new JavaModel::ISourceRange(left_column_no, left_line_no, right_column_no, right_line_no);
				info->setNameRange(rng);

				if (field_symbol->initial_value) {
					info->initial_value = field_symbol->initial_value->Clone();
				}

				initializer_element->addChild(local);
				local->SetType(declaringTypeName);
			}
			return local;

			
		}
	
		
		IJavaElement* SelectUtil::selectOnField(AstNodeScope*scope,VariableSymbol* field_symbol,
			JavaModel::ISelectionRequestor* requestor)
		{
			if (!field_symbol || !requestor )
				return nullptr;
			if (field_symbol->IsField())
			{
				wstring declaringTypePackageName;
				wstring declaringTypeName;
				wstring name;
				const auto type_symbol_parameter = field_symbol->ContainingType();

				if(type_symbol_parameter)
				{
					const wchar_t*  temp = field_symbol->Name();
					if (temp)
						name.assign(temp, temp + field_symbol->NameLength());

					auto packae_symbol_parameter = type_symbol_parameter->ContainingPackage();
					if(packae_symbol_parameter)
					{
						declaringTypePackageName = packae_symbol_parameter->PackageNameString();
					}
					declaringTypeName = type_symbol_parameter->qualifiedSourceName('.');
					
				}
				boost::replace_all(declaringTypePackageName, L"/", L".");

				auto type = requestor->getType(declaringTypePackageName, declaringTypeName,
					JavaModel::NameLookup::ACCEPT_CLASSES | JavaModel::NameLookup::ACCEPT_INTERFACES);

				if (type != nullptr) {
					auto field = type->getField(name);
					if (field) {
						auto info = reinterpret_cast <JavaModel::SourceFieldElementInfo*>(field->getElementInfo());
						if (field_symbol->initial_value && !info->initial_value) {
							info->initial_value = field_symbol->initial_value->Clone();
						}
						return  field;
					}
				}

				return nullptr;
			}
			
			if (!field_symbol->declarator) 
			{
				auto type_symbol_parameter = field_symbol->Type();
				if (type_symbol_parameter)
				{
					return getSelectOnType(type_symbol_parameter, requestor);
				}	
				return nullptr;
			}
				
			AstNodeScope* parentScope = ScopeUtil::enclosingMethod(scope);
			vector<JavaModel::IMethod*> methods;
			if (parentScope)
			{
				auto methodDecl = dynamic_cast<AbstractMethodDeclaration*>(parentScope);
				if (!methodDecl)
					return nullptr;
				return GetLocalFieldInMethodItem(field_symbol, methodDecl, requestor, scope);
			}	
		
			auto initializeBody = dynamic_cast<AstBlock*> (ScopeUtil::getInitializerBodyScop(scope));
			if(initializeBody)
			{
				return GetLocalFieldInInitilize(field_symbol, requestor, scope);
			}
				
			auto visitor = new FindLocalFieldOwnerVisitor();
			visitor->pos = field_symbol->declarator->LeftToken()->start_location;

			try 
			{
				if(field_symbol->ContainingType())
				{
					auto cunit = field_symbol->ContainingType()->file_symbol;
					if(cunit && cunit->compilation_unit)
						cunit->compilation_unit->traverse(visitor, nullptr);
				}	
			}
			catch (AstFound& ret)
			{
				AstFound::AstFoundKind _kind = ret.kind;
				Ast* node = ret.node;
				vector<JavaModel::IMethod*> methods;
				if (_kind == AstFound::FIND_METHOD || _kind == AstFound::FIND_CONSTRUCTOR)
				{
					auto methodDecl = dynamic_cast<AbstractMethodDeclaration*>(node);
					if (!methodDecl)
						return nullptr;
					return GetLocalFieldInMethodItem(field_symbol, methodDecl, requestor, scope);
				
				}
				else if (_kind == AstFound::FIND_INITIALIZER)
				{
					return GetLocalFieldInInitilize(field_symbol, requestor, scope);
				}
			}
				
			

			return nullptr;
		}

	}//namespace CodeAssist
} // Close namespace Jikes block


