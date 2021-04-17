#include "stdafx.h"
#include "DeclarationOfAccessedFieldsPattern.h"
#include <SearchLib/internal/core/IJavaSearchResultCollector.h>
#include <JCDT_Lib/internal/lookup/VariableSymbol.h>
#include <JavaModelLib/internal/core/IJavaElement.h>
#include <JavaModelLib/internal/core/IType.h>
#include <JavaModelLib/internal/core/IField.h>
#include <SearchLib/internal/matching/MatchLocator.h>
#include <JCDT_Lib/internal/lookup/TypeSymbol.h>
#include <SearchLib/internal/matching/MatchSet.h>

namespace Jikes { // Open namespace Jikes block


	
namespace Search{

	DeclarationOfAccessedFieldsPattern::DeclarationOfAccessedFieldsPattern(JavaModel::IJavaElement* _enclosingElement)
	:FieldReferencePattern(L"", PATTERN_MATCH,false,L"",L"",L"",L"",true,true), enclosingElement(_enclosingElement)
{
	_needsResolve = true;
	}

	void DeclarationOfAccessedFieldsPattern::matchReportReference(Ast* reference,
		JavaModel::IJavaElement* element, int accuracy, MatchSet* locator)
	{
		// need accurate match to be able to open on type ref
		if (accuracy == IJavaSearchResultCollector::POTENTIAL_MATCH) 
			return;

		// element that references the field must be included in the enclosing element
		while (element != nullptr && !enclosingElement->equals(element)) {
			element = element->getParent();
		}
		if (!element || !reference)
			return;

		if ( FieldReference::FieldAccessCast(reference)) 
		{
			auto symbol = ((FieldReference*)reference)->symbol;
			if(symbol && symbol->_kind == Symbol::VARIABLE)
			{
				reportDeclaration(reinterpret_cast<VariableSymbol*>(symbol), locator);
			}
			
		}
		else if (reference->kind == Ast::NAME ) 
		{
			auto symbol = ((AstName*)reference)->symbol;
			if (symbol &&  symbol->_kind == Symbol::VARIABLE)
			{
				reportDeclaration(reinterpret_cast<VariableSymbol*>(symbol), locator);
			}
		}

	}

	void DeclarationOfAccessedFieldsPattern::reportDeclaration(VariableSymbol* fieldBinding, MatchSet* locator)
	{
		// ignore length field
		if (!fieldBinding || !locator)
			return;
		name = fieldBinding->getNameString();
		if(name == L"length")
		{
			return;
		}
		

		auto declaringClass = fieldBinding->ContainingType();

		if (!declaringClass)
			return;

		JavaModel::IType* type = locator->lookupType(declaringClass);
		if (!type) 
			return; // case of a secondary type


		JavaModel::IField* field = type->getField(name);

		if (!field)
			return;

		if (knownFields.find(reinterpret_cast<JavaModel::IJavaElement*> (field) ) != knownFields.end())
			return;

		knownFields.insert(reinterpret_cast<JavaModel::IJavaElement*> (field));
		
		bool isBinary = type->isBinary();
	
		if (isBinary) {
			locator->reportBinaryMatch(field, IJavaSearchResultCollector::EXACT_MATCH);
		}
		else {
			auto class_body = declaringClass->declaration;
			if (!class_body)
				return;
			AstVariableDeclarator* declarator = nullptr;
			for (unsigned i = 0, num = class_body->NumInstanceVariables(); i < num; i++)
			{
				FieldDeclaration* field_declaration = class_body->InstanceVariable(i);

				if (!field_declaration)
					continue;

				auto num_vairable = field_declaration->NumVariableDeclarators();
				for (unsigned k = 0; k < num_vairable; k++)
				{
					 declarator = field_declaration->VariableDeclarator(k);

					 if (!declarator || !declarator->variable_declarator_name || declarator->variable_declarator_name->identifier_token)
						 continue;

					 if(name == declarator->variable_declarator_name->identifier_token->getName())
					 {
						 break;
					 }
				}
			}

			if(declarator && declarator->LeftToken() && declarator->RightToken())
			{
				locator->report(declarator->LeftToken()->start_location, declarator->RightToken()->EndLocation()
					, field, IJavaSearchResultCollector::EXACT_MATCH,0);
				return;
			}

			for (unsigned i = 0, num = class_body->NumClassVariables(); i < num; i++)
			{
				FieldDeclaration* field_declaration = class_body->ClassVariable(i);

				if (!field_declaration)
					continue;

				auto num_vairable = field_declaration->NumVariableDeclarators();
				for (unsigned k = 0; k < num_vairable; k++) {
					 declarator =
						field_declaration->VariableDeclarator(k);
					 if (declarator && declarator->variable_declarator_name
						 && declarator->variable_declarator_name->identifier_token
						 && name == declarator->variable_declarator_name->identifier_token->getName())
					 {
						 break;
					 }
				
				}
			}
			if (declarator && declarator->LeftToken() && declarator->RightToken()) {
				locator->report(declarator->LeftToken()->start_location, declarator->RightToken()->EndLocation()
					, field, IJavaSearchResultCollector::EXACT_MATCH,0);
			}
		}
	}
}


} // Close namespace Jikes block


