#include <JCDT_Lib/internal/ast/AbstractMethodDeclaration.h>
#include <JCDT_Lib/internal/ast/AstMethodDeclarator.h>
#include  <boost/algorithm/string.hpp>
#include <JCDT_Lib/internal/ast/AstFormalParameter.h>
#include <JCDT_Lib/internal/ast/AstType.h>
#include <JCDT_Lib/internal/ast/AstVariableDeclarator.h>
#include <JCDT_Lib/internal/ast/AstVariableDeclaratorId.h>
#include <JCDT_Lib/internal/lookup/MethodSymbol.h>
namespace Jikes { // Open namespace Jikes block
	wstring AbstractMethodDeclaration::selector()
	{
		return method_declarator->identifier_token->getName();
	}

	std::pair<int, int> AbstractMethodDeclaration::getNameRange()
	{
		std::pair<int, int> name_rang;
		name_rang.first = method_declarator->identifier_token->start_location;
		name_rang.second = method_declarator->identifier_token->EndLocation();
		return name_rang;
	}
	void AbstractMethodDeclaration::getParameterTypesAndNames(vector<wstring>* parameterTypes,
		vector<wstring>*parameterNames)
	{
		auto  num_paramenter = method_declarator->NumFormalParameters();
		for (unsigned l = 0; l < num_paramenter; l++) {
			AstFormalParameter* parameter =
				method_declarator->FormalParameter(l);

			if(parameterTypes){
				auto typeNameTemp = boost::join(parameter->type->getParameterizedTypeName(), L".");
				unsigned dims = parameter->formal_declarator->variable_declarator_name->NumBrackets();
				for (size_t i = 0; i < dims; ++i) {
					typeNameTemp.append(L"[]");
				}
				parameterTypes->push_back(typeNameTemp);
			}

			if (parameterNames)
			{
				Token* identifier_token;
				if (parameter->ellipsis_token_opt) {
					identifier_token = parameter->ellipsis_token_opt;

				}
				else {
					identifier_token = parameter->formal_declarator->variable_declarator_name->identifier_token;
				}
				parameterNames->push_back(identifier_token->getName());
			}
	
		}
	}

	Symbol* AbstractMethodDeclaration::GetSymbol()
	{
		return method_symbol;
	}
} // Close namespace Jikes block



