
#include "stdafx.h"
#include "CorrectionAssistASTVisitor.h"
#include <JCDT_Lib/internal/parser/diagnose/RecoveryScanner.h>

namespace Jikes { // Open namespace Jikes block
	namespace Correction {
	
		CorrectionAssistASTVisitor::CorrectionAssistASTVisitor(unsigned _pos):
		
			nextTypeReferenceIsClass(false),
			nextTypeReferenceIsException(false),
			nextTypeReferenceIsInterface(false), 
			assistNodeIsInsideCase(false), 
			assistNodeIsExtendedType(false),
			lastInRangeMethodCallNode(nullptr), 
			end_visit(false) ,
			completNodeType(NoFoundCompletion), 
			astNode(nullptr),
			completeScope(nullptr), 
			pos(_pos)
		{
			trackAstNodes.reserve(8);
		}

		bool CorrectionAssistASTVisitor::checkTypeDeclaration(TypeDeclaration* node, AstNodeScope* scope)
		{
			if (!node)
				return false;

			completeScope = node;

			if (node->class_body && node->class_body->identifier_token)
			{
				
				if (node->class_body->identifier_token->IsInRange(pos))
				{
						completNodeType = CompletionOnTypeIdentifier;
						astNode = node;
						completeScope = scope;
						end_visit = true;
						return  false;
				}
				
			}
			return true;
		}

		



		
	 
	

	bool  CorrectionAssistASTVisitor::visit(AstBlock * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		completeScope = node;

		return true;
	}

	bool  CorrectionAssistASTVisitor::visit(AstName * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

	
		if (node->identifier_token->IsInRange(pos) && node->identifier_token->getName() != RecoveryScanner::FAKE_IDENTIFIER) {
			completNodeType = CompletionOnNameReference;		
			astNode = node;
			completeScope = scope;
			end_visit = true;
			return  false;
			
		}
		return true;
	}

	
	bool  CorrectionAssistASTVisitor::visit(AstArrayType * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(AstArrayType * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(AstWildcard * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(AstWildcard * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(AstTypeArguments * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(AstTypeArguments * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(AstTypeName * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		if(node->name->identifier_token->IsInRange(pos))
		{
			if(node->name->identifier_token->getName() == RecoveryScanner::FAKE_IDENTIFIER){
				return  true;
			}
			if(nextTypeReferenceIsInterface)
				completNodeType = CompletionOnClassReference;
			else if(nextTypeReferenceIsInterface){
				completNodeType = CompletionOnInterfaceReference;
			}
			else{
				completNodeType = CompletionOnAstTypeName;
			}
			astNode = node;
			completeScope = scope;
			end_visit = true;
			return  false;
		}
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(AstTypeName * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(AstMemberValuePair * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(AstMemberValuePair * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(AstAnnotation * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(AstAnnotation * node,AstNodeScope* scope)
	{

	}

		bool CorrectionAssistASTVisitor::visit(AstModifiers* node, AstNodeScope* scope)
		{
			if (!IsInRange(node, scope))
				return false;
			return true;
		}

		bool CorrectionAssistASTVisitor::visit(AstModifierKeyword* node, AstNodeScope* scope)
		{
			if(node->modifier_token->IsInRange(pos))
			{
				completNodeType = CompletionOnModifiers;
				astNode = node;
				completeScope = scope;
				end_visit = true;
				return  false;
			}
			return false;
		}

		bool  CorrectionAssistASTVisitor::visit(AstPackageDeclaration * node,AstNodeScope* scope)
	{

		if (!IsInRange(node,scope))
			return false;
		
		if (!IsInRange(node->name, scope))
			return false;

		completNodeType = CompletionOnPackageReference;
		astNode = node;
		completeScope = scope;
		end_visit = true;

		return false;
	}

	void  CorrectionAssistASTVisitor::endVisit(AstPackageDeclaration * node,AstNodeScope* scope)
	{
		
	}
	bool  CorrectionAssistASTVisitor::visit(AstImportDeclaration * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		enterpoint_node = node;
	
		if (!IsInRange(node->name, scope))
			return false;

		completNodeType = CompletionOnImportReference;
		astNode = node;
		completeScope = scope;
		end_visit = true;
		return  false;
	}

	void  CorrectionAssistASTVisitor::endVisit(AstImportDeclaration * node,AstNodeScope* scope)
	{
	}
	bool  CorrectionAssistASTVisitor::visit(CompilationUnitDeclaration * node,AstNodeScope* scope)
	{
		completeScope = node;
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(CompilationUnitDeclaration * node,AstNodeScope* scope)
	{

	}
	
	bool  CorrectionAssistASTVisitor::visit(AstClassBody * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return  true;
	}

	void  CorrectionAssistASTVisitor::endVisit(AstClassBody * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(AstTypeParameter * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(AstTypeParameter * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(AstTypeParameters * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(AstTypeParameters * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(ClassTypeDeclaration * node,AstNodeScope* scope)
	{

		if (!IsInRange(node,scope))
			return false;
		if (!checkTypeDeclaration(node ,scope))
			return false;

		if(node->super_opt){
			if (IsInRange(node->super_opt)){
				
				if(node->super_opt->IdentifierToken()->IsInRange(pos))
					assistNodeIsExtendedType = true;

				 nextTypeReferenceIsClass= true;
			}
		}

		if(node->interfaces && !nextTypeReferenceIsClass){
			if (!IsInRange(node->class_body)) {
				nextTypeReferenceIsInterface = true;
			}	
		}
		
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(ClassTypeDeclaration * node,AstNodeScope* scope)
	{
		
	}
	bool  CorrectionAssistASTVisitor::visit(AstArrayInitializer * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(AstArrayInitializer * node,AstNodeScope* scope)
	{

	}
	
	bool  CorrectionAssistASTVisitor::visit(AstVariableDeclarator * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		auto identifier_token = node->variable_declarator_name->identifier_token;
		if (identifier_token->IsInRange(pos) && identifier_token->getName() != RecoveryScanner::FAKE_IDENTIFIER){

		

			if(node->isField())
			{
				completNodeType = CompletionOnFieldName;
			}
			else
			{
				completNodeType = CompletionOnLocalFieldName;
			}
			astNode = node;
			completeScope = scope;
			end_visit = true;
			return  false;
		}
		return true;

	}

	void  CorrectionAssistASTVisitor::endVisit(AstVariableDeclarator * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(FieldDeclaration * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		auto identifier_token = node->type->IdentifierToken();
		if (identifier_token->IsInRange(pos) && identifier_token->getName() != RecoveryScanner::FAKE_IDENTIFIER) {
			
			completNodeType = CompletionOnFieldType;
			astNode = node;
			completeScope = scope;
			end_visit = true;
			return  false;
		}
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(FieldDeclaration * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(AstFormalParameter * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(AstFormalParameter * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(AstMethodDeclarator * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		

		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(AstMethodDeclarator * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(AstMethodBody * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		completeScope = node;

		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(AstMethodBody * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(MethodDeclaration * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		auto identifier_token = node->type->IdentifierToken();
		if ( identifier_token ->IsInRange(pos) && identifier_token->getName() != RecoveryScanner::FAKE_IDENTIFIER) {
			
			completNodeType = CompletionOnMethodReturnType;
			astNode = node;
			completeScope = scope;
			end_visit = true;
			return  false;
		}

		auto declarator = node->method_declarator;
		if (declarator->identifier_token)
		{
			if (declarator->identifier_token->IsInRange(pos)  && identifier_token->getName() != RecoveryScanner::FAKE_IDENTIFIER)
			{
				completNodeType = CompletionOnMethodName;
				astNode = node;
				completeScope = scope;
				end_visit = true;
				return  false;
			}
		}




		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(MethodDeclaration * node,AstNodeScope* scope)
	{
		
	}
	bool  CorrectionAssistASTVisitor::visit(Initializer * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(Initializer * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(AstArguments * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(AstArguments * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(AstThisCall * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		if (node->this_token)
		{
			if (node->this_token->IsInRange(pos))
			{
		
				completNodeType = CompletionOnExplicitThisConstructorCall;
				astNode = node;
				completeScope = scope;
				end_visit = true;
				return  false;
			
			}

		}
		auto  arguments = node->arguments;
		if (arguments->left_parenthesis_token->start_location <= pos
			&& arguments->right_parenthesis_token->EndLocation() >=pos
			) {
			lastInRangeMethodCallNode = node;
			completeScope = scope;
			astNode = node;
		}
	
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(AstThisCall * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(AstSuperCall * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		if (node->super_token)
		{
			if (node->super_token->IsInRange(pos))
			{

				completNodeType = CompletionOnExplicitSuperConstructorCall;
				astNode = node;
				completeScope = scope;
				end_visit = true;
				return  false;

			}

		}
		auto  arguments = node->arguments;
		if (arguments->left_parenthesis_token->start_location <= pos
			&& arguments->right_parenthesis_token->EndLocation() >=pos
			) {
			lastInRangeMethodCallNode = node;
			completeScope = scope;
			astNode = node;
		}
		
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(AstSuperCall * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(ConstructorDeclaration * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		auto declarator = node->method_declarator;
		if (declarator->identifier_token)
		{
			if (declarator->identifier_token->IsInRange(pos))
			{
				end_visit = true;
				return  false;
			}
			
		}

		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(ConstructorDeclaration * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(EnumTypeDeclaration * node,AstNodeScope* scope)
	{
		
		if (!IsInRange(node,scope))
			return false;
	
		if (!checkTypeDeclaration(node, scope))
			return false;
		if (!IsInRange(node->class_body, scope)) {
			nextTypeReferenceIsInterface = true;
		}



		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(EnumTypeDeclaration * node,AstNodeScope* scope)
	{
		
	}
	bool  CorrectionAssistASTVisitor::visit(AstEnumConstant * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
	
		if (node->identifier_token)
		{
			if (node->identifier_token->IsInRange(pos))
			{
				
		
			}
		}

		
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(AstEnumConstant * node,AstNodeScope* scope)
	{

	}
	
	bool  CorrectionAssistASTVisitor::visit(InterfaceTypeDeclaration * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		if (!checkTypeDeclaration(node, scope))
			return false;

		if(!IsInRange(node->class_body, scope)){
			nextTypeReferenceIsInterface = true;
		}
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(InterfaceTypeDeclaration * node,AstNodeScope* scope)
	{
		
	}
	bool  CorrectionAssistASTVisitor::visit(AnnotationTypeDeclaration * node,AstNodeScope* scope)
	{
		
		if (!IsInRange(node,scope))
			return false;
	
		if (!checkTypeDeclaration(node, scope))
			return false;

		
		return true;
	}

	

	bool  CorrectionAssistASTVisitor::visit(LocalDeclaration * node, AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		if (!IsInRange(node,scope))
			return false;
		auto  identifier_token = node->type->IdentifierToken();
		if (identifier_token->IsInRange(pos) && identifier_token->getName() != RecoveryScanner::FAKE_IDENTIFIER) {
		
			completNodeType = CompletionOnLocalFieldType;
			astNode = node;
			completeScope = scope;
			end_visit = true;

			return true;
		}
		return true;
	}
		  
	bool  CorrectionAssistASTVisitor::visit(AstLocalClassStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	
	bool  CorrectionAssistASTVisitor::visit(IfStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	
	
	bool  CorrectionAssistASTVisitor::visit(AstExpressionStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	
	bool  CorrectionAssistASTVisitor::visit(AstSwitchLabel * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		if(node->expression_opt)
		{
			if(node->expression_opt->kind == Ast::NAME)
			{
				auto ast_name = reinterpret_cast<AstName*>(node->expression_opt);
				if (!ast_name->IsQualifiedName() && ast_name->identifier_token->VirtualType() != Token::NORMAL)
				{
					completNodeType = CompletionOnCaseStatement;
					astNode = node;
					completeScope = scope;
					end_visit = true;
					return true;
				}
			}

			if(IsInRange(node->expression_opt)){
				assistNodeIsInsideCase = true;
			}
			if(node->colon_token->start_location > pos)
			{
				completNodeType = CompletionOnCaseStatement;
				astNode = node;
				completeScope = scope;
				end_visit = true;
				return false;
			}
		}
		else
		{
			if(node->colon_token->start_location > pos)
			{
				completNodeType = CompletionOnCaseStatement;
				astNode = node;
				completeScope = scope;
				end_visit = true;
				return false;
			}
		}
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(AstSwitchLabel * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(AstSwitchBlockStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(AstSwitchBlockStatement * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(SwitchStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(SwitchStatement * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(WhileStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(WhileStatement * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(DoStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(DoStatement * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(ForStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(ForStatement * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(ForeachStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(ForeachStatement * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(BreakStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		else
		{
			
		}
		return true;
	}

	
	bool  CorrectionAssistASTVisitor::visit(ContinueStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		else
		{
		
		}
		return true;
	}

	
	bool  CorrectionAssistASTVisitor::visit(ReturnStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		if(!node->expression_opt)
		{
			if(node->semicolon_token->start_location > pos){
				completNodeType = CompletionOnReturnStateMent;
				astNode = node;
				completeScope = scope;
				end_visit = true;
				return false;
			}
		}

		return true;
	}


	bool  CorrectionAssistASTVisitor::visit(ThrowStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(ThrowStatement * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(SynchronizedStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(SynchronizedStatement * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(AssertStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(AssertStatement * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(AstCatchClause * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		if(node->formal_parameter && node->formal_parameter->type->IdentifierToken()->IsInRange(pos))
		{
			if (node->formal_parameter->type->IdentifierToken()->getName() == RecoveryScanner::FAKE_IDENTIFIER) {
				return  true;
			}
			completNodeType = CompletionOnExceptionReference;
			astNode = node->formal_parameter->type;
			completeScope = scope;
			end_visit = true;
			return  false;
		}
		
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(AstCatchClause * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(AstFinallyClause * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(AstFinallyClause * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(TryStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(TryStatement * node,AstNodeScope* scope)
	{

	}
	
	
	bool  CorrectionAssistASTVisitor::visit(ClassLiteralAccess * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(ClassLiteralAccess * node,AstNodeScope* scope)
	{
		
	}
	bool  CorrectionAssistASTVisitor::visit(AstThisExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		if (node->this_token)
		{
			if (node->this_token->IsInRange(pos))
			{

				completNodeType = CompletionOnExplicittThisExpr;
				astNode = node;
				completeScope = scope;
				end_visit = true;
				return  false;

			}

		}
		
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(AstThisExpression * node,AstNodeScope* scope)
	{
		
	}
	bool  CorrectionAssistASTVisitor::visit(AstSuperExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		if (node->super_token)
		{
			if (node->super_token->IsInRange(pos))
			{

				completNodeType = CompletionOnExplicittSuperExpr;
				astNode = node;
				completeScope = scope;
				end_visit = true;
				return  false;

			}

		}
		
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(AstSuperExpression * node,AstNodeScope* scope)
	{
		
	}
	bool  CorrectionAssistASTVisitor::visit(AstParenthesizedExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(AstParenthesizedExpression * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(AstClassCreationExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		if (IsInRange(node->class_type, scope)){
			completNodeType = CompletionOnClassCreationExpressionOnTypeName;
			astNode = node;
			completeScope = scope;
			end_visit = true;
			return  false;
		}
		if(node->new_token->IsInRange(pos))
		{
			end_visit = true;
			return  false;
		}

		auto  arguments = node->arguments;
		if (arguments->left_parenthesis_token->start_location <= pos
			&& arguments->right_parenthesis_token->EndLocation() >=pos
			) {
			lastInRangeMethodCallNode = node;
			completeScope = scope;
			astNode = node;
		}
	

		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(AstClassCreationExpression * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(AstDimExpr * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(AstDimExpr * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(AstArrayCreationExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(AstArrayCreationExpression * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(FieldReference * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		auto  identifier_token = node->identifier_token;
		if(identifier_token && identifier_token->IsInRange(pos))
		{
			if (identifier_token->getName() == RecoveryScanner::FAKE_IDENTIFIER) {
				return  true;
			}
			completNodeType = CompletionOnMemberAccess;
			astNode = node;
			completeScope = scope;
			end_visit = true;
			return  false;
		}
		
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(FieldReference * node,AstNodeScope* scope)
	{
	}
	bool  CorrectionAssistASTVisitor::visit(AstMethodInvocation * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		auto identifier_token = node->identifier_token;
		if (identifier_token && identifier_token->IsInRange(pos)){
			if(identifier_token->getName() == RecoveryScanner::FAKE_IDENTIFIER)
			{
				return true;
			}

			completNodeType = CompletionOnMessageSend;
			astNode = node;
			completeScope = scope;
			end_visit = true;
			return  false;
		}

		auto  arguments = node->arguments;
		if (arguments->left_parenthesis_token->start_location <= pos
			&& arguments->right_parenthesis_token->EndLocation() >=pos
			){
			lastInRangeMethodCallNode = node;
			completeScope = scope;
			astNode = node;
		}
	
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(AstMethodInvocation * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(ArrayReference * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
	   
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(ArrayReference * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(AstPostUnaryExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(AstPostUnaryExpression * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(AstPreUnaryExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(AstPreUnaryExpression * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(CastExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(CastExpression * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(BinaryExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(BinaryExpression * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(InstanceofExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(InstanceofExpression * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(ConditionalExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(ConditionalExpression * node,AstNodeScope* scope)
	{

	}
	bool  CorrectionAssistASTVisitor::visit(AstAssignmentExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		auto token = node->assignment_operator_token;
		if(token->_kind == TK_EQUAL)
		{
			if (token->IsInRange(pos) && token->VirtualType() == Token::NORMAL)
			{
				completNodeType = CompletionOnAssignmentExpressionEqualOpt;
				astNode = node;
				completeScope = scope;
				end_visit = true;
			}
			else{
				if (token->EndLocation() < pos  && node->expression->kind == Ast::NAME)
				{
					auto ast_name = reinterpret_cast<AstName*>(node->expression);
					if (!ast_name->IsQualifiedName() && ast_name->identifier_token->VirtualType() != Token::NORMAL)
					{
						completNodeType = CompletionOnAssignmentExpressionBLANK;
						astNode = node;
						completeScope = scope;
						end_visit = true;
					}
				}
			}

		}
		
		return true;
	}

	void  CorrectionAssistASTVisitor::endVisit(AstAssignmentExpression * node,AstNodeScope* scope)
	{

	}
	bool CorrectionAssistASTVisitor::IsInRange(Ast* node)
	{

		if (end_visit)
			return false;
		auto leftToken = node->LeftToken();
		if (!leftToken)
			return false;

		auto start = leftToken->start_location;
		if (pos < start)
		{
			return false;
		}
		
		auto rightToken = node->RightToken();
		if (!rightToken)
		{
			return false;
		}
		auto end = rightToken->EndLocation();
		if (pos > end)
		{
			return false;
		}
		
		return true;
	}
	bool CorrectionAssistASTVisitor::IsInRange(Ast* node, AstNodeScope* scope)
	{

		if (end_visit)
			return false;
		auto leftToken = node->LeftToken();
		if (!leftToken)
			return false;

		auto start = leftToken->start_location;
		if (pos < start)
		{
			return false;
		}
		
		auto rightToken = node->RightToken();
		if(!rightToken)
		{
			return false;
		}
		auto end = rightToken->EndLocation();
		if (pos > end)
		{
			return false;
		}
		completeScope = scope;
		trackAstNodes.push_back(node);
		return true;
	}
}//  namespace CodeAssist 
	

} // Close namespace Jikes block




