

#include "stdafx.h"
#include <CodeAssistLib/impl/AstNoFound.h>
#include "ScopetAssistASTVisitor.h"
#include "AstFound.h"

#include <CodeAssistLib/select/SelectionOnNameReference.h>

namespace Jikes { // Open namespace Jikes block
	namespace CodeAssist {
		ScopetAssistASTVisitor::ScopetAssistASTVisitor()
			: selectedScope(nullptr), pos(0)
		{
		}

		void ScopetAssistASTVisitor::checkTypeDeclaration(TypeDeclaration* node, unsigned pos)
		{
			selectedScope = node;

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

		



	
	bool  ScopetAssistASTVisitor::visit(AstBlock * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		selectedScope = node;
		return true;
	}

	bool  ScopetAssistASTVisitor::visit(AstName * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		return true;
	}

	
	bool  ScopetAssistASTVisitor::visit(AstArrayType * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		return true;
	}

	void  ScopetAssistASTVisitor::endVisit(AstArrayType * node,AstNodeScope* scope)
	{

	}
	bool  ScopetAssistASTVisitor::visit(AstWildcard * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	void  ScopetAssistASTVisitor::endVisit(AstWildcard * node,AstNodeScope* scope)
	{

	}
	bool  ScopetAssistASTVisitor::visit(AstTypeArguments * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}


	bool  ScopetAssistASTVisitor::visit(AstTypeName * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		return true;
	}

	bool  ScopetAssistASTVisitor::visit(AstMemberValuePair * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		return true;
	}

	
	bool  ScopetAssistASTVisitor::visit(AstAnnotation * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		return true;
	}


	
	
	bool  ScopetAssistASTVisitor::visit(CompilationUnitDeclaration * node,AstNodeScope* scope)
	{
		
		if (!IsInRange(node,scope))
			return false;
		selectedScope = node;
		return true;
	}

	
	
	bool  ScopetAssistASTVisitor::visit(AstClassBody * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	
	
	bool  ScopetAssistASTVisitor::visit(ClassTypeDeclaration * node,AstNodeScope* scope)
	{
		
		if (!IsInRange(node,scope))
			return false;
		checkTypeDeclaration(node, pos);
		
		return true;
	}

	
	bool  ScopetAssistASTVisitor::visit(AstArrayInitializer * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	
	bool  ScopetAssistASTVisitor::visit(AstVariableDeclarator * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		return true;

	}

	
	bool  ScopetAssistASTVisitor::visit(FieldDeclaration * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	
	bool  ScopetAssistASTVisitor::visit(AstFormalParameter * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	
	bool  ScopetAssistASTVisitor::visit(AstMethodDeclarator * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	
	bool  ScopetAssistASTVisitor::visit(AstMethodBody * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		selectedScope = node;
		return true;
	}

	
	bool  ScopetAssistASTVisitor::visit(MethodDeclaration * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

	
		return true;
	}

	bool  ScopetAssistASTVisitor::visit(Initializer * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	
	bool  ScopetAssistASTVisitor::visit(AstArguments * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	
	
	bool  ScopetAssistASTVisitor::visit(ConstructorDeclaration * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

	
		return true;
	}

	
	bool  ScopetAssistASTVisitor::visit(EnumTypeDeclaration * node,AstNodeScope* scope)
	{
		
		if (!IsInRange(node,scope))
			return false;
	
		
		return true;
	}


	bool  ScopetAssistASTVisitor::visit(AstEnumConstant * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
	
		
		return true;
	}


	bool  ScopetAssistASTVisitor::visit(InterfaceTypeDeclaration * node,AstNodeScope* scope)
	{
		
		if (!IsInRange(node,scope))
			return false;
		checkTypeDeclaration(node, pos);
		
		
		return true;
	}

	
	bool  ScopetAssistASTVisitor::visit(AnnotationTypeDeclaration * node,AstNodeScope* scope)
	{
		
		if (!IsInRange(node,scope))
			return false;
	
		checkTypeDeclaration(node, pos);

		
		return true;
	}

	bool  ScopetAssistASTVisitor::visit(LocalDeclaration * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}


	bool  ScopetAssistASTVisitor::visit(AstLocalClassStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	
	bool  ScopetAssistASTVisitor::visit(IfStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	
	
	bool  ScopetAssistASTVisitor::visit(AstExpressionStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	
	bool  ScopetAssistASTVisitor::visit(AstSwitchLabel * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	
	bool  ScopetAssistASTVisitor::visit(AstSwitchBlockStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}


	bool  ScopetAssistASTVisitor::visit(SwitchStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	
	bool  ScopetAssistASTVisitor::visit(WhileStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	
	bool  ScopetAssistASTVisitor::visit(DoStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

	
		return true;
	}


	bool  ScopetAssistASTVisitor::visit(ForStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}


	bool  ScopetAssistASTVisitor::visit(ForeachStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}


	bool  ScopetAssistASTVisitor::visit(BreakStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		else
		{
			throw AstNoFound();
		}
		return true;
	}

	
	bool  ScopetAssistASTVisitor::visit(ContinueStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		else
		{
			throw AstNoFound();
		}
		return true;
	}

	
	bool  ScopetAssistASTVisitor::visit(ReturnStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}


	bool  ScopetAssistASTVisitor::visit(ThrowStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	bool  ScopetAssistASTVisitor::visit(SynchronizedStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	bool  ScopetAssistASTVisitor::visit(AssertStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	bool  ScopetAssistASTVisitor::visit(AstCatchClause * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	bool  ScopetAssistASTVisitor::visit(AstFinallyClause * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	
	bool  ScopetAssistASTVisitor::visit(TryStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	
	
	
	bool  ScopetAssistASTVisitor::visit(ClassLiteralAccess * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	
	bool  ScopetAssistASTVisitor::visit(AstThisExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
	
		return true;
	}


	bool  ScopetAssistASTVisitor::visit(AstParenthesizedExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	
	bool  ScopetAssistASTVisitor::visit(AstClassCreationExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		if (IsInRange(node->class_type, scope)) {		
			throw SelectionOnNameReference(node, SelectionOnNameReference::SelectionClassCreation,scope);
		}

		return true;
	}

	
	bool  ScopetAssistASTVisitor::visit(AstDimExpr * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	
	bool  ScopetAssistASTVisitor::visit(AstArrayCreationExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	
	bool  ScopetAssistASTVisitor::visit(FieldReference * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
	
		return true;
	}

	void  ScopetAssistASTVisitor::endVisit(FieldReference * node,AstNodeScope* scope)
	{
	}
	bool  ScopetAssistASTVisitor::visit(AstMethodInvocation * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		return true;
	}

	void  ScopetAssistASTVisitor::endVisit(AstMethodInvocation * node,AstNodeScope* scope)
	{

	}
	bool  ScopetAssistASTVisitor::visit(ArrayReference * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
	
		return true;
	}

	void  ScopetAssistASTVisitor::endVisit(ArrayReference * node,AstNodeScope* scope)
	{

	}
	bool  ScopetAssistASTVisitor::visit(AstPostUnaryExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  ScopetAssistASTVisitor::endVisit(AstPostUnaryExpression * node,AstNodeScope* scope)
	{

	}
	bool  ScopetAssistASTVisitor::visit(AstPreUnaryExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  ScopetAssistASTVisitor::endVisit(AstPreUnaryExpression * node,AstNodeScope* scope)
	{

	}
	bool  ScopetAssistASTVisitor::visit(CastExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  ScopetAssistASTVisitor::endVisit(CastExpression * node,AstNodeScope* scope)
	{

	}
	bool  ScopetAssistASTVisitor::visit(BinaryExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  ScopetAssistASTVisitor::endVisit(BinaryExpression * node,AstNodeScope* scope)
	{

	}
	bool  ScopetAssistASTVisitor::visit(InstanceofExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  ScopetAssistASTVisitor::endVisit(InstanceofExpression * node,AstNodeScope* scope)
	{

	}
	bool  ScopetAssistASTVisitor::visit(ConditionalExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  ScopetAssistASTVisitor::endVisit(ConditionalExpression * node,AstNodeScope* scope)
	{

	}
	bool  ScopetAssistASTVisitor::visit(AstAssignmentExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  ScopetAssistASTVisitor::endVisit(AstAssignmentExpression * node,AstNodeScope* scope)
	{

	}

	 void ScopetAssistASTVisitor::TokenInPosThenNoFound(Token* tk)const
	{
		if (tk)
		{
			if (tk->IsInRange(pos))
			{
				throw AstNoFound();
			}
		}
	}
}//  namespace CodeAssist 
	

} // Close namespace Jikes block




