#include "stdafx.h"
#include <JCDT_Lib/internal/lookup/MethodSymbol.h>
#include "FindReferenceOption.h"
#include <JCDT_Lib/internal/lookup/TypeSymbol.h>
#include <JCDT_Lib/internal/lookup/VariableSymbol.h>
#include "ReferenceVisitor.h"

namespace Jikes {


	namespace Search {

		
		bool ZeroReferenceVisitor::visit(AstMethodInvocation* node, AstNodeScope* scope)
		{
			if (!node)
				return false;

			return ProcessMethod(node->symbol);
		}

	

	

		

		bool ZeroReferenceVisitor::visit(AstName* node, AstNodeScope* scope)
		{

			if (!node)
				return false;

			Symbol* node_symol = node->symbol;
			if(!node_symol)
			{
				return  true;
			}
			
			if(node_symol->_kind == Symbol::VARIABLE)
				return PrcoessVariable(node->symbol);
			else if(node_symol->_kind == Symbol::TYPE)
			{
				if (!option->find_type_reference)
					return true;


				auto typeBinding = (TypeSymbol*)node_symol;

				if (typeBinding->IsArray())
					typeBinding = typeBinding->base_type;
				FileSymbol* file_symbol;
				if (!typeBinding->file_symbol || !((file_symbol = typeBinding->file_symbol)))
				{
					return true;
				}

				if (all_files.find(file_symbol) == all_files.end())
				{
					return true;
				}
				auto findIt = candidate.find(typeBinding);
				if (findIt != candidate.end())
				{
					candidate.erase(findIt);
				}
				return true;

			}

			return true;
			
		}

		bool ZeroReferenceVisitor::visit(AstClassCreationExpression* node, AstNodeScope* scope)
		{
			if (!node)
				return false;
			return  ProcessMethod(node->symbol);
		}

		bool ZeroReferenceVisitor::visit(AstThisCall* node, AstNodeScope* scope)
		{
			if (!node)
				return false;
			return ProcessMethod(node->symbol);
		}

		bool ZeroReferenceVisitor::visit(AstSuperCall* node, AstNodeScope* scope)
		{
			if (!node)
				return false;
			return ProcessMethod(node->symbol);
		}

		bool ZeroReferenceVisitor::ProcessMethod(Symbol* symbol)
		{

			if (!option->find_method_reference )
				return true;

			if ( !symbol || symbol->Kind() != Symbol::METHOD)
			{
				return true;
			}

			MethodSymbol* methodSymbol = (MethodSymbol*)(symbol);
			FileSymbol* file_symbol;
			if (!methodSymbol->containing_type || !((file_symbol = methodSymbol->containing_type->file_symbol)))
			{
				return true;
			}

			if (all_files.find(file_symbol) == all_files.end())
			{
				return true;
			}
			auto findIt = candidate.find(symbol);
			if (findIt != candidate.end())
			{
				candidate.erase(findIt);
			}
			return true;
		}


		ZeroReferenceVisitor::ZeroReferenceVisitor(FindReferenceOption* _option) :ReferenceVisitorBase(_option)
		{
			setDefaultRetValue(true);
		}

		bool ZeroReferenceVisitor::PrcoessVariable(Symbol* node_symol)
		{
			if (!option->find_filed_reference && !option->find_local_filed_reference)
				return true;
			
			if (!node_symol ||  node_symol->_kind != Symbol::VARIABLE)
			{
				return true;
			}

			auto symbol = (VariableSymbol*)(node_symol);

			if (!symbol->owner)
			{
				return true;
			}
			FileSymbol* file_symbol;
			bool local;
			if (Symbol::METHOD == symbol->owner->_kind)
			{
				local = true;
			}
			else if (Symbol::TYPE == symbol->owner->_kind)
			{
				local = false;
			}
			else
			{

				return true;
			}

			if (!symbol->ContainingType() || !((file_symbol = symbol->ContainingType()->file_symbol)))
			{
				return true;
			}


			if (all_files.find(file_symbol) == all_files.end())
			{
				return true;
			}
			if (local)
			{
				if (!option->find_local_filed_reference)
				{
					return true;
				}
				auto findIt = templocalFieldCandidate.find(symbol);
				if (findIt != templocalFieldCandidate.end())
				{
					templocalFieldLosers.insert({ symbol,findIt->second });
					templocalFieldCandidate.erase(findIt);
					localFieldScopeDatas.erase(symbol);
				}
				else
				{
					auto findSetIt = templocalFieldLosers.find(symbol);
					if (findSetIt == templocalFieldLosers.end())
					{
						templocalFieldCandidate.insert({ symbol ,0 });
					}
				}
			}
			else
			{
				auto findIt = candidate.find(symbol);
				if (candidate.end() != findIt)
				{
					candidate.erase(findIt);
				}
			}
			return true;

		}
		bool ZeroReferenceVisitor::visit(FieldReference* node, AstNodeScope* scope)
		{
		
			return PrcoessVariable(node->symbol);

		}

		void ZeroReferenceVisitor::clear()
		{
			 all_files.clear();

			

			 templocalFieldCandidate.clear();


			theOnes.clear();;

			candidate.clear();;


		}

	}
}
