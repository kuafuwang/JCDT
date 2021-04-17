

#include "stdafx.h"
#include <CodeAssistLib/impl/AstNoFound.h>
#include "SelectAssistASTVisitor.h"
#include "AstFound.h"

#include <CodeAssistLib/select/SelectionOnNameReference.h>

namespace Jikes { // Open namespace Jikes block
	namespace CodeAssist {
		SelectAssistASTVisitor::SelectAssistASTVisitor(vector<TypeDeclaration*>& _type_stack)
			: selectedScope(nullptr), enter_point_(ENTER_NULL), pos(0), type_stack(_type_stack)
		{
		}

		void SelectAssistASTVisitor::checkTypeDeclaration(TypeDeclaration* node, unsigned pos)
		{
			if (node->class_body)
			{
				if (node->class_body->identifier_token)
				{
					if (node->class_body->identifier_token->IsInRange(pos))
					{
						throw AstFound(node, AstFound::FIND_TYPE);
					}
				}
			}
		}

		



		
	 
	

	bool  SelectAssistASTVisitor::visit(AstBlock * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		return true;
	}

	bool  SelectAssistASTVisitor::visit(AstName * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

	
		if (node->identifier_token->IsInRange(pos)){
			
			if(enter_point_ == ENTER_IMPORT){
				auto import_decl = dynamic_cast<AstImportDeclaration*>(enterpoint_node);
				import_decl->name = node;	
				throw AstFound(import_decl, AstFound::FIND_IMPORT);
			}
			
			if(enter_point_ == ENTER_PACKAGE){
				auto pkg = dynamic_cast<AstPackageDeclaration*>(enterpoint_node);
				pkg->name = node;
				throw AstFound(pkg, AstFound::FIND_PACKAGE);
			}

			throw SelectionOnNameReference(node, SelectionOnNameReference::SelectionOnExpression, scope);
		}
		return true;
	}

	
	bool  SelectAssistASTVisitor::visit(AstArrayType * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(AstArrayType * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(AstWildcard * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(AstWildcard * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(AstTypeArguments * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(AstTypeArguments * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(AstTypeName * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(AstTypeName * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(AstMemberValuePair * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(AstMemberValuePair * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(AstAnnotation * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(AstAnnotation * node,AstNodeScope* scope)
	{

	}
	
	bool  SelectAssistASTVisitor::visit(AstPackageDeclaration * node,AstNodeScope* scope)
	{

		if (!IsInRange(node,scope))
			return false;
		enterpoint_node = node;
		enter_point_ = ENTER_PACKAGE;

		return true;
	}

	void  SelectAssistASTVisitor::endVisit(AstPackageDeclaration * node,AstNodeScope* scope)
	{
		
	}
	bool  SelectAssistASTVisitor::visit(AstImportDeclaration * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		enterpoint_node = node;
		enter_point_ = ENTER_IMPORT;

		return true;
	}

	void  SelectAssistASTVisitor::endVisit(AstImportDeclaration * node,AstNodeScope* scope)
	{
	}
	bool  SelectAssistASTVisitor::visit(CompilationUnitDeclaration * node,AstNodeScope* scope)
	{
		
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(CompilationUnitDeclaration * node,AstNodeScope* scope)
	{

	}
	
	bool  SelectAssistASTVisitor::visit(AstClassBody * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		

		return true;
	}

	void  SelectAssistASTVisitor::endVisit(AstClassBody * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(AstTypeParameter * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(AstTypeParameter * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(AstTypeParameters * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(AstTypeParameters * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(ClassTypeDeclaration * node,AstNodeScope* scope)
	{
		type_stack.push_back(node);
		if (!IsInRange(node,scope))
			return false;
		checkTypeDeclaration(node, pos);
		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(ClassTypeDeclaration * node,AstNodeScope* scope)
	{
		type_stack.pop_back();
	}
	bool  SelectAssistASTVisitor::visit(AstArrayInitializer * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(AstArrayInitializer * node,AstNodeScope* scope)
	{

	}
	
	bool  SelectAssistASTVisitor::visit(AstVariableDeclarator * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		if (node->variable_declarator_name->identifier_token->IsInRange(pos)){
			throw SelectionOnNameReference(node, SelectionOnNameReference::SelectionOnField,scope);
		}
		return true;

	}

	void  SelectAssistASTVisitor::endVisit(AstVariableDeclarator * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(FieldDeclaration * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(FieldDeclaration * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(AstFormalParameter * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(AstFormalParameter * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(AstMethodDeclarator * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(AstMethodDeclarator * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(AstMethodBody * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(AstMethodBody * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(MethodDeclaration * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		if (node->method_declarator)
		{
			if (node->method_declarator->identifier_token)
			{
				if (node->method_declarator->identifier_token->IsInRange(pos))
				{
					throw SelectionOnNameReference(node, SelectionOnNameReference::SelectionOnMethod,scope);
				}
			}
		}
		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(MethodDeclaration * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(Initializer * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(Initializer * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(AstArguments * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(AstArguments * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(AstThisCall * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		if (node->this_token)
		{
			if (node->this_token->IsInRange(pos))
			{

				throw SelectionOnNameReference(node,
					SelectionOnNameReference::SelectionOnThisCall, scope);
			}

		}
		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(AstThisCall * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(AstSuperCall * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		if (node->super_token)
		{
			if (node->super_token->IsInRange(pos))
			{
				throw SelectionOnNameReference(node,
					SelectionOnNameReference::SelectionOnSuperCall,scope);
			}

		}
		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(AstSuperCall * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(ConstructorDeclaration * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		if (node->method_declarator)
		{
			if (node->method_declarator->identifier_token)
			{
				if (node->method_declarator->identifier_token->IsInRange(pos))
				{
					throw SelectionOnNameReference(node, SelectionOnNameReference::SelectionOnConstructor, scope);
					
				}
			}
		}

		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(ConstructorDeclaration * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(EnumTypeDeclaration * node,AstNodeScope* scope)
	{
		type_stack.push_back(node);
		if (!IsInRange(node,scope))
			return false;
	
		checkTypeDeclaration(node, pos);
		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(EnumTypeDeclaration * node,AstNodeScope* scope)
	{
		type_stack.pop_back();
	}
	bool  SelectAssistASTVisitor::visit(AstEnumConstant * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
	
		if (node->identifier_token)
		{
			if (node->identifier_token->IsInRange(pos))
			{
				
				throw SelectionOnNameReference(node, SelectionOnNameReference::SelectionEnumConstant, scope);
			}
		}

		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(AstEnumConstant * node,AstNodeScope* scope)
	{

	}
	
	bool  SelectAssistASTVisitor::visit(InterfaceTypeDeclaration * node,AstNodeScope* scope)
	{
		type_stack.push_back(node);
		if (!IsInRange(node,scope))
			return false;
		checkTypeDeclaration(node, pos);
		
		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(InterfaceTypeDeclaration * node,AstNodeScope* scope)
	{
		type_stack.pop_back();
	}
	bool  SelectAssistASTVisitor::visit(AnnotationTypeDeclaration * node,AstNodeScope* scope)
	{
		type_stack.push_back(node);
		if (!IsInRange(node,scope))
			return false;
	
		checkTypeDeclaration(node, pos);

		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(AnnotationTypeDeclaration * node,AstNodeScope* scope)
	{
		type_stack.pop_back();
	}
	bool  SelectAssistASTVisitor::visit(LocalDeclaration * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(LocalDeclaration * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(AstLocalClassStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(AstLocalClassStatement * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(IfStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(IfStatement * node,AstNodeScope* scope)
	{

	}
	
	bool  SelectAssistASTVisitor::visit(AstExpressionStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(AstExpressionStatement * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(AstSwitchLabel * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(AstSwitchLabel * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(AstSwitchBlockStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(AstSwitchBlockStatement * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(SwitchStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(SwitchStatement * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(WhileStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(WhileStatement * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(DoStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(DoStatement * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(ForStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(ForStatement * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(ForeachStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(ForeachStatement * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(BreakStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		else
		{
			throw AstNoFound();
		}
		return true;
	}

	
	bool  SelectAssistASTVisitor::visit(ContinueStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		else
		{
			throw AstNoFound();
		}
		return true;
	}

	
	bool  SelectAssistASTVisitor::visit(ReturnStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}


	bool  SelectAssistASTVisitor::visit(ThrowStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(ThrowStatement * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(SynchronizedStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(SynchronizedStatement * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(AssertStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(AssertStatement * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(AstCatchClause * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(AstCatchClause * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(AstFinallyClause * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(AstFinallyClause * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(TryStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(TryStatement * node,AstNodeScope* scope)
	{

	}
	
	
	bool  SelectAssistASTVisitor::visit(ClassLiteralAccess * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(ClassLiteralAccess * node,AstNodeScope* scope)
	{
		
	}
	bool  SelectAssistASTVisitor::visit(AstThisExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		if (node->this_token)
		{
			if (node->this_token->IsInRange(pos))
			{
				
				throw SelectionOnNameReference(node,
					SelectionOnNameReference::SelectionOnExpression, scope);
			}

		}
		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(AstThisExpression * node,AstNodeScope* scope)
	{
		
	}
	bool  SelectAssistASTVisitor::visit(AstSuperExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		if (node->super_token)
		{
			if (node->super_token->IsInRange(pos))
			{
				throw SelectionOnNameReference(node,
					SelectionOnNameReference::SelectionOnExpression, scope);
			}

		}
		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(AstSuperExpression * node,AstNodeScope* scope)
	{
		
	}
	bool  SelectAssistASTVisitor::visit(AstParenthesizedExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(AstParenthesizedExpression * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(AstClassCreationExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		if (IsInRange(node->class_type, scope)) {		
			throw SelectionOnNameReference(node, SelectionOnNameReference::SelectionClassCreation, scope);
		}

		return true;
	}

	void  SelectAssistASTVisitor::endVisit(AstClassCreationExpression * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(AstDimExpr * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(AstDimExpr * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(AstArrayCreationExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(AstArrayCreationExpression * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(FieldReference * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		if(node->identifier_token)
		{
			if(node->identifier_token->IsInRange(pos))
			{
				
				throw SelectionOnNameReference(node, SelectionOnNameReference::SelectionOnExpression, scope);
			}
		}
		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(FieldReference * node,AstNodeScope* scope)
	{
	}
	bool  SelectAssistASTVisitor::visit(AstMethodInvocation * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		if (node->identifier_token)
		{
			if (node->identifier_token->IsInRange(pos))
			{
				throw SelectionOnNameReference(node,
					SelectionOnNameReference::SelectionOnExpression, scope);
			}

		}
		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(AstMethodInvocation * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(ArrayReference * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
	
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(ArrayReference * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(AstPostUnaryExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(AstPostUnaryExpression * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(AstPreUnaryExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(AstPreUnaryExpression * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(CastExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(CastExpression * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(BinaryExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(BinaryExpression * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(InstanceofExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(InstanceofExpression * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(ConditionalExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(ConditionalExpression * node,AstNodeScope* scope)
	{

	}
	bool  SelectAssistASTVisitor::visit(AstAssignmentExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  SelectAssistASTVisitor::endVisit(AstAssignmentExpression * node,AstNodeScope* scope)
	{

	}

	 void SelectAssistASTVisitor::TokenInPosThenNoFound(Token* tk)const
	{
		if (tk)
		{
			if (tk->IsInRange(pos))
			{
				throw AstNoFound();
			}
		}
	}

	 bool SelectAssistASTVisitor::IsInRange(Ast* node, AstNodeScope* scope)
	 {
		 if (!node)
			 return false;

		 auto left_token = node->LeftToken();
	     if(!left_token)
			 return false;

		 auto start = left_token->start_location;
		 if (pos < start)
		 {
			 return false;
		 }
		 auto right_token = node->RightToken();
		 if (!right_token)
			 return false;
		 auto end = right_token->EndLocation();
		 if (pos > end)
		 {
			 return false;
		 }
		 selectedScope = scope;
		 return true;
	 }
}//  namespace CodeAssist 
	

} // Close namespace Jikes block




