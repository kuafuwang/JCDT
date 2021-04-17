#include "stdafx.h"
#include "MatchLocatorParserVisitor.h"
#include "MatchSet.h"
#include <JCDT_Lib/internal/ast/AstAnonymousTypeDeclaration.h>
namespace Jikes { // Open namespace Jikes block

	namespace Search {
		MatchLocatorParserVisitor::MatchLocatorParserVisitor()
		{
			setDefaultRetValue(true);
		}

		bool MatchLocatorParserVisitor::visit(AstClassCreationExpression* node, AstNodeScope* scope)
		{
			matchSet->checkMatching(node);
			return true;
		}

		bool MatchLocatorParserVisitor::visit(AstAssignmentExpression* node, AstNodeScope* scope)
		{
			matchSet->checkMatching(node);
			return true;
		}

		bool MatchLocatorParserVisitor::visit(AstSuperCall* node, AstNodeScope* scope)
		{
			matchSet->checkMatching(node);
			return true;
		}

		bool MatchLocatorParserVisitor::visit(AstThisCall* node, AstNodeScope* scope)
		{
			matchSet->checkMatching(node);
			return true;
		}

		bool MatchLocatorParserVisitor::visit(FieldReference* node, AstNodeScope* scope)
		{
			matchSet->checkMatching(node);
			return true;
		}

		bool MatchLocatorParserVisitor::visit(AstMethodInvocation* node, AstNodeScope* scope)
		{
			matchSet->checkMatching(node);
			return true;
		}

		bool MatchLocatorParserVisitor::visit(AstImportDeclaration* node, AstNodeScope* scope)
		{
			matchSet->checkMatching(node);
			return true;
		}

		bool MatchLocatorParserVisitor::visit(AstPostUnaryExpression* node, AstNodeScope* scope)
		{
			matchSet->checkMatching(node);
			return true;
		}

		bool MatchLocatorParserVisitor::visit(AstPreUnaryExpression* node, AstNodeScope* scope)
		{
			matchSet->checkMatching(node);
			return true;
		}

		bool MatchLocatorParserVisitor::visit(AstTypeName* node, AstNodeScope* scope)
		{
			matchSet->checkMatching(node);
			return true;
		}

		bool MatchLocatorParserVisitor::visit(AstArrayType* node, AstNodeScope* scope)
		{
			if (matchSet->removePossibleMatch(node->type) != 0) {
				matchSet->addPossibleMatch(node);
				return true;
			}
			else if (matchSet->removeTrustedMatch(node->type) != 0) {
				matchSet->addTrustedMatch(node);
				return true;
			}

			return true;
		}

		bool MatchLocatorParserVisitor::visit(AstName* node, AstNodeScope* scope)
		{
			matchSet->checkMatching(node);
			return true;
		}

		 bool MatchLocatorParserVisitor::visit(ClassTypeDeclaration * node, AstNodeScope* scope)
		{
			 if (!node)
				 return false;

			auto _level = node->_level;
			if (_level == TypeDeclaration::ANONYMOUS_LEVEL ||
				_level == TypeDeclaration::LOCAL_LEVEL){
				if ((matchSet->matchContainer & SearchPattern::METHOD) != 0) {
					matchSet->checkMatching(node);
				}
			}
			else if (_level == TypeDeclaration::MEMBER_LEVEL){
				if ((matchSet->matchContainer & SearchPattern::CLASS) != 0) {
					matchSet->checkMatching(node);
				}
			}else
			{
				if( (matchSet->matchContainer & SearchPattern::CLASS) != 0
					|| (matchSet->matchContainer & SearchPattern::METHOD) != 0)
				{
					matchSet->checkMatching(node);
				}
			}
			return true;
		}
		  bool MatchLocatorParserVisitor::visit(InterfaceTypeDeclaration * node, AstNodeScope* scope)
		 {
			  if (!node)
				  return false;
			 auto _level = node->_level;
			 if (_level == TypeDeclaration::ANONYMOUS_LEVEL ||
				 _level == TypeDeclaration::LOCAL_LEVEL) {
				 if ((matchSet->matchContainer & SearchPattern::METHOD) != 0) {
					 matchSet->checkMatching(node);
				 }
			 }
			 else if (_level == TypeDeclaration::MEMBER_LEVEL) {
				 if ((matchSet->matchContainer & SearchPattern::CLASS) != 0) {
					 matchSet->checkMatching(node);
				 }
			 }
			 else
			 {
				 if ((matchSet->matchContainer & SearchPattern::CLASS) != 0
					 || (matchSet->matchContainer & SearchPattern::METHOD) != 0)
				 {
					 matchSet->checkMatching(node);
				 }
			 }
			 return true;
		 }

		   bool MatchLocatorParserVisitor::visit(AnnotationTypeDeclaration * node, AstNodeScope* scope)
		  {
			   if (!node)
				   return false;
			  auto _level = node->_level;
			  if (_level == TypeDeclaration::ANONYMOUS_LEVEL ||
				  _level == TypeDeclaration::LOCAL_LEVEL) {
				  if ((matchSet->matchContainer & SearchPattern::METHOD) != 0) {
					  matchSet->checkMatching(node);
				  }
			  }
			  else if (_level == TypeDeclaration::MEMBER_LEVEL) {
				  if ((matchSet->matchContainer & SearchPattern::CLASS) != 0) {
					  matchSet->checkMatching(node);
				  }
			  }
			  else
			  {
				  if ((matchSet->matchContainer & SearchPattern::CLASS) != 0
					  || (matchSet->matchContainer & SearchPattern::METHOD) != 0)
				  {
					  matchSet->checkMatching(node);
				  }
			  }
			  return true;
		  }

	
		   bool MatchLocatorParserVisitor::visit(EnumTypeDeclaration * node, AstNodeScope* scope)
		  {
			   if (!node)
				   return false;

			  auto _level = node->_level;
			  if (_level == TypeDeclaration::ANONYMOUS_LEVEL ||
				  _level == TypeDeclaration::LOCAL_LEVEL) {
				  if ((matchSet->matchContainer & SearchPattern::METHOD) != 0) {
					  matchSet->checkMatching(node);
				  }
			  }
			  else if (_level == TypeDeclaration::MEMBER_LEVEL) {
				  if ((matchSet->matchContainer & SearchPattern::CLASS) != 0) {
					  matchSet->checkMatching(node);
				  }
			  }
			  else
			  {
				  if ((matchSet->matchContainer & SearchPattern::CLASS) != 0
					  || (matchSet->matchContainer & SearchPattern::METHOD) != 0)
				  {
					  matchSet->checkMatching(node);
				  }
			  }
			  return true;
		  }
		bool MatchLocatorParserVisitor::visit(ConstructorDeclaration* node, AstNodeScope* scope)
		{
			if ((matchSet->matchContainer & SearchPattern::CLASS) != 0) {
				matchSet->checkMatching(node);
			}
			
			return true;
		}

		bool MatchLocatorParserVisitor::visit(AstVariableDeclarator* node, AstNodeScope* scope)
		{
			if ((matchSet->matchContainer & SearchPattern::CLASS) != 0) {
				matchSet->checkMatching(node);
			}
			else if((matchSet->matchContainer & SearchPattern::METHOD) != 0){
				if(node && !node->isField()){
					matchSet->checkMatching(node);
				}
			}
			return true;
		}

		bool MatchLocatorParserVisitor::visit(Initializer* node, AstNodeScope* scope)
		{
			if ((matchSet->matchContainer & SearchPattern::CLASS) != 0) {
				matchSet->checkMatching(node);
			}
			
			return true;
		}

		bool MatchLocatorParserVisitor::visit(AstLocalClassStatement* node, AstNodeScope* scope)
		{
			if ((matchSet->matchContainer & SearchPattern::METHOD) != 0) {
				matchSet->checkMatching(node);
			}
			return true;
		}

		
		bool MatchLocatorParserVisitor::visit(MethodDeclaration* node, AstNodeScope* scope)
		{
			if ((matchSet->matchContainer & SearchPattern::CLASS) != 0) {
				matchSet->checkMatching(node);
			}
			return true;
		}
	}//namespace CodeAssist

} // Close namespace Jikes block


