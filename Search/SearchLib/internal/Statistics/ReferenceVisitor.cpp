#include "stdafx.h"
#include "ReferenceVisitor.h"
#include <JCDT_Lib/internal/lookup/MethodSymbol.h>
#include "FindReferenceOption.h"
#include <JCDT_Lib/internal/lookup/TypeSymbol.h>
#include <JCDT_Lib/internal/lookup/VariableSymbol.h>

namespace Jikes {


	namespace Search {

		/*ReferenceVisitorResult::ReferenceVisitorResult() :number(0)
		{

		}



		ScopeSet::ScopeSet(Ast* o) : original(o)
		{
		}

		ScopeSet::~ScopeSet()
		{
			for (auto it : matchs)
			{
				delete it;
			}
			matchs.clear();
			original = nullptr;
		}

		ReferenceVisitorResult::~ReferenceVisitorResult()
		{
			for (auto it : all_sets)
			{
				delete it.second;
			}
			all_sets.clear();

		}*/

	
		ReferenceVisitorBase::ReferenceVisitorBase(FindReferenceOption* _option)
		:option(_option), topMethodBody(nullptr)
		{
			setDefaultRetValue(true);
		}

		
		

		bool ReferenceVisitorBase::visit(AstVariableDeclarator* node, AstNodeScope* scope)
		{
			if (!option->find_local_filed_reference)
				return true;
			if(!node)
			{
				return false;
			}
			if (node->isField())
			{
				return true;
			}
			if (node->symbol &&node->symbol->_kind == Symbol::VARIABLE)
			{
				templocalFieldCandidate.insert({ node->symbol, 0 });
				localFieldScopeDatas.insert({ node->symbol,scope });
			}

			return true;
		}
		bool ReferenceVisitorBase::visit(AstMethodBody* node, AstNodeScope* scope)
		{
			if (!node)
			{
				return false;
			}
			if(!topMethodBody)
			{
				topMethodBody = node;
			}
			return true;
		}

		void ReferenceVisitorBase::endVisit(AstMethodBody* node, AstNodeScope* scope)
		{
			if(node == topMethodBody)
			{
				topMethodBody = nullptr;
				for(auto it : templocalFieldCandidate)
				{
					theOnes.insert(it);
				}
				templocalFieldCandidate.clear();
				templocalFieldLosers.clear();
			}
		}


		void ReferenceVisitorBase::clear()
		{
			losers.clear();
			candidate.clear();
		}










		bool ReferenceVisitor::PrcoessVariable(Symbol* node_symol)
		{
			if (!option->find_filed_reference && !option->find_local_filed_reference)
				return true;
			if (!node_symol || node_symol->_kind != Symbol::VARIABLE)
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
				if(!option->find_local_filed_reference)
				{
					return true;
				}
				auto findIt = templocalFieldCandidate.find(symbol);
				if (findIt != templocalFieldCandidate.end())
				{
					auto number = findIt->second;

					findIt->second += 1;
					if (!option->IsCandidate(number))
					{
						templocalFieldLosers.insert({ symbol,findIt->second });
						templocalFieldCandidate.erase(findIt);
						localFieldScopeDatas.erase(symbol);
					}

				}
				else
				{
					auto findSetIt = templocalFieldLosers.find(symbol);
					if (findSetIt == templocalFieldLosers.end())
					{
						templocalFieldCandidate.insert({ symbol ,1 });
					}
				}
			}
			else
			{
				Filter(symbol);
			}


			

			
			return true;

		}

		void ReferenceVisitor::Filter(Symbol* symbol)
		{

		
			auto findIt = candidate.find(symbol);
			if (findIt != candidate.end())
			{
				auto number = findIt->second;

				findIt->second += 1;
				if (!option->IsCandidate(number))
				{
					losers.insert({ symbol ,number });
					candidate.erase(findIt);
				}

			}
			else
			{
				auto findSetIt = losers.find(symbol);
				if (findSetIt == losers.end())
				{
					candidate.insert({ symbol ,1 });
				}
			}
		}




	
		ReferenceVisitor::ReferenceVisitor(FindReferenceOption* _option) : ReferenceVisitorBase(_option)
		{
		}


		bool ReferenceVisitor::visit(FieldReference* node, AstNodeScope* scope)
		{
			if (!node)
			{
				return false;
			}
			return PrcoessVariable(node->symbol);
		}

		bool ReferenceVisitor::visit(AstMethodInvocation* node, AstNodeScope* scope)
		{
			if (!node)
			{
				return false;
			}
			return  ProcessMethod(node->symbol);
		}

		bool ReferenceVisitor::visit(AstClassCreationExpression* node, AstNodeScope* scope)
		{
			if (!node)
			{
				return false;
			}
			return  ProcessMethod(node->symbol);
		}

		bool ReferenceVisitor::visit(AstThisCall* node, AstNodeScope* scope)
		{
			if (!node)
			{
				return false;
			}
			return ProcessMethod(node->symbol);
		}

		bool ReferenceVisitor::visit(AstSuperCall* node, AstNodeScope* scope)
		{
			if (!node)
			{
				return false;
			}
			return ProcessMethod(node->symbol);
		}

		bool ReferenceVisitor::ProcessMethod(Symbol* symbol)
		{
			if (!option->find_method_reference)
				return true;

			
			if (!symbol || symbol->_kind != Symbol::METHOD)
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

			Filter(symbol);
			return true;
		}


		bool ReferenceVisitor::visit(AstName* node, AstNodeScope* scope)
		{
			if (!node)
			{
				return false;
			}
			Symbol* node_symol = node->symbol;
			if (!node_symol)
			{
				return  true;
			}

			if (node_symol->_kind == Symbol::VARIABLE)
				return PrcoessVariable(node->symbol);
			else if (node_symol->_kind == Symbol::TYPE)
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
				Filter(typeBinding);

				return true;

			}


			return true;
		}

	}
}
