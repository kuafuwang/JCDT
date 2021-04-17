#include "stdafx.h"
#include "ConstructorReferencePattern.h"
#include <JCDT_Lib/internal/ast/ASTNode.h>
#include <JCDT_Lib/internal/ast/AstSuperCall.h>
#include <JCDT_Lib/internal/lookup/MethodSymbol.h>
#include <JCDT_Lib/internal/lookup/VariableSymbol.h>
#include <SearchLib/internal/matching/MatchSet.h>
#include <JCDT_Lib/internal/ast/AstAnonymousTypeDeclaration.h>

namespace Jikes { // Open namespace Jikes block


	
	namespace Search {


		int ConstructorReferencePattern::matchLevel(AstClassCreationExpression* allocation, bool resolve)
		{
			if (!allocation || allocation->anonymous_type_opt)
				return IMPOSSIBLE_MATCH;

			// constructor name is simple type name
			
			if (declaringSimpleName.empty() != true)
			{
				if(!allocation->class_type )
					return IMPOSSIBLE_MATCH;

				auto typeName = allocation->class_type->getTypeName();
				if(!matchesName(declaringSimpleName, typeName[typeName.size() - 1]))
				{
					return IMPOSSIBLE_MATCH;
				}
			}
				

			if (resolve) {
				return matchLevel(allocation->symbol);
			}
			else {
				// argument types
				int argumentCount = parameterSimpleNames == nullptr ? -1 : parameterSimpleNames->size();
				if (argumentCount > -1) {
					int parameterCount = allocation->arguments == nullptr ? 0 : allocation->arguments->NumArguments();
					if (parameterCount != argumentCount)
						return IMPOSSIBLE_MATCH;
				}
				return _needsResolve ? POSSIBLE_MATCH : ACCURATE_MATCH;
			}
		}

		int ConstructorReferencePattern::matchLevel(Ast* node, bool resolve)
		{
			if (!node)
				return IMPOSSIBLE_MATCH;

			if (node->kind == Ast::CLASS_CREATION) {
				return matchLevel((AstClassCreationExpression*)node, resolve);
			}
			else if (node->kind == Ast::THIS_CALL) {
				return matchLevel((AstThisCall*)node, resolve);
			}
			else if (node->kind == Ast::SUPER_CALL) {
				return matchLevel((AstSuperCall*)node, resolve);
			}
			return IMPOSSIBLE_MATCH;
		}

		int ConstructorReferencePattern::matchLevel(AstSuperCall* call, bool resolve)
		{
			// TBD: constructor name is super simple type name
			if (!call)
				return IMPOSSIBLE_MATCH;

			if (resolve) {
				return matchLevel(call->symbol);
			}
			else {
				// argument types
				int argumentCount = parameterSimpleNames == nullptr ? -1 : parameterSimpleNames->size();
				if (argumentCount > -1) {
					int parameterCount = call->arguments == nullptr ? 0 : call->arguments->NumArguments();
					if (parameterCount != argumentCount)
						return IMPOSSIBLE_MATCH;
				}
				return _needsResolve ? POSSIBLE_MATCH : ACCURATE_MATCH;
			}
		}
		int ConstructorReferencePattern::matchLevel(AstThisCall* call, bool resolve)
		{
			// TBD: constructor name is super simple type name
			if(!call)
				return IMPOSSIBLE_MATCH;

			if (resolve) {
				return matchLevel(call->symbol);
			}
			else {
				// argument types
				int argumentCount = parameterSimpleNames == nullptr ? -1 : parameterSimpleNames->size();
				if (argumentCount > -1) {
					int parameterCount = call->arguments == nullptr ? 0 : call->arguments->NumArguments();
					if (parameterCount != argumentCount)
						return IMPOSSIBLE_MATCH;
				}
				return _needsResolve ? POSSIBLE_MATCH : ACCURATE_MATCH;
			}
		}
		int ConstructorReferencePattern::matchLevel(Symbol* binding)
		{
			if (binding == nullptr) return INACCURATE_MATCH;
			if (!(binding->_kind == Symbol::METHOD))
				return IMPOSSIBLE_MATCH;
		

			// declaring type
			MethodSymbol* method = (MethodSymbol*)binding;
			auto  declaringBinding = method->containing_type;
			int level = matchLevelForType(
				declaringSimpleName,
				declaringQualification,
				declaringBinding);

			if (level == IMPOSSIBLE_MATCH)
				return IMPOSSIBLE_MATCH;

			// argument types
			int argumentCount = parameterSimpleNames == nullptr ? -1 : parameterSimpleNames->size();
			if (argumentCount > -1) 
			{
				if (argumentCount > 0) 
				{
					int parameterCount = method->NumFormalParameters();
					if (argumentCount != parameterCount)
						return IMPOSSIBLE_MATCH;
				
					for (int i = 0; i < parameterCount; i++) 
					{
						auto& qualification = parameterQualifications->operator[](i);
						auto& type = parameterSimpleNames->operator[](i);
						auto parameter = method->FormalParameter(i);

						if (!parameter)
							continue;

						int newLevel = matchLevelForType(type, qualification, parameter->Type());
						switch (newLevel) 
						{
						case IMPOSSIBLE_MATCH:
							return IMPOSSIBLE_MATCH;
						case ACCURATE_MATCH: // keep previous level
							break;
						default: // ie. INACCURATE_MATCH
							level = newLevel;
							break;
						}
					}		
				}
			}
			return level;
		}

		void ConstructorReferencePattern::matchReportReference(Ast* reference, 
			JavaModel::IJavaElement* element, int accuracy, MatchSet* locator)
		{
			if (!reference || !locator)
				return;

			if (reference->kind == Ast::CLASS_CREATION) {
				auto  atual_node = (AstClassCreationExpression*)reference;
				Token* leftTokon = nullptr;
				if(atual_node->anonymous_type_opt){
					 leftTokon = atual_node->anonymous_type_opt->LeftToken();	
				}
				else if(atual_node->class_type)
				{
					leftTokon = atual_node->class_type->RightToken();
				}

				if(leftTokon)
				{
					locator->report(
						leftTokon->start_location,
						leftTokon->EndLocation(),
						element,
						accuracy,0);
				}
			}
			else if (reference->kind == Ast::THIS_CALL) {
				auto  atual_node = (AstThisCall*)reference;
				Token* leftTokon = atual_node->this_token;
				if (leftTokon)
				{
					locator->report(
						leftTokon->start_location,
						leftTokon->EndLocation(),
						element,
						accuracy, 0);
				}
			}
			else if (reference->kind == Ast::SUPER_CALL) {
				auto  atual_node = (AstSuperCall*)reference;
				Token* leftTokon = atual_node->super_token;
				if (leftTokon)
				{
					locator->report(
						leftTokon->start_location,
						leftTokon->EndLocation(),
						element,
						accuracy, 0);
				}
			}
			else {
				MethodPattern::matchReportReference(reference, element, accuracy, locator);
			}

		}

		bool ConstructorReferencePattern::IsMayInTheFile(BaseLexStream* lex)
		{
			return SearchPattern::IsMayInTheFile(lex);
		}
	}
} // Close namespace Jikes block


