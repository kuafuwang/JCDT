
#include "stdafx.h"
#include "CompleteAssistASTVisitor.h"
#include <JCDT_Lib/internal/parser/diagnose/RecoveryScanner.h>

namespace Jikes { // Open namespace Jikes block
	namespace CodeAssist {
		void CompleteAssistASTVisitor::ResetForHomeMakeAstTraverse()
		{
			lastInRangeMethodCallNode = (nullptr);
			homeMakeAstLPos = (0);
			end_visit = (false);
			completNodeType = (NoFoundCompletion);
			astNode = (nullptr);
			
		}

		CompleteAssistASTVisitor::CompleteAssistASTVisitor(unsigned _pos):
			tokenKind(0), 
			nextTypeReferenceIsClass(false),
			nextTypeReferenceIsException(false),
			nextTypeReferenceIsInterface(false), 
			assistNodeIsInsideCase(false), 
			assistNodeIsExtendedType(false),
			lastInRangeMethodCallNode(nullptr), 
			homeMakeAstLPos(0),
			end_visit(false) ,
			completNodeType(NoFoundCompletion), 
			astNode(nullptr),
			completeScope(nullptr), 
			pos(_pos)
		{

		}

		bool CompleteAssistASTVisitor::checkTypeDeclaration(TypeDeclaration* node)
		{
			if (!node)
				return false;

			completeScope = node;

			if (node->class_body && (node->class_body->identifier_token)
				&& node->class_body->identifier_token->IsInRange(pos))	
			{
				end_visit = true;
				return  false;
			}
		

			return true;
		}

		



		
	 
	

	bool  CompleteAssistASTVisitor::visit(AstBlock * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		completeScope = node;

		return true;
	}

	bool  CompleteAssistASTVisitor::visit(AstName * node,AstNodeScope* scope)
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

	
	bool  CompleteAssistASTVisitor::visit(AstArrayType * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(AstArrayType * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(AstWildcard * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(AstWildcard * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(AstTypeArguments * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(AstTypeArguments * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(AstTypeName * node,AstNodeScope* scope)
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

	void  CompleteAssistASTVisitor::endVisit(AstTypeName * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(AstMemberValuePair * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(AstMemberValuePair * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(AstAnnotation * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(AstAnnotation * node,AstNodeScope* scope)
	{

	}

		bool CompleteAssistASTVisitor::visit(AstModifiers* node, AstNodeScope* scope)
		{
			if (!IsInRange(node, scope))
				return false;
			return true;
		}

		bool CompleteAssistASTVisitor::visit(AstModifierKeyword* node, AstNodeScope* scope)
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

		bool  CompleteAssistASTVisitor::visit(AstPackageDeclaration * node,AstNodeScope* scope)
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

	void  CompleteAssistASTVisitor::endVisit(AstPackageDeclaration * node,AstNodeScope* scope)
	{
		
	}
	bool  CompleteAssistASTVisitor::visit(AstImportDeclaration * node,AstNodeScope* scope)
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

	void  CompleteAssistASTVisitor::endVisit(AstImportDeclaration * node,AstNodeScope* scope)
	{
	}
	bool  CompleteAssistASTVisitor::visit(CompilationUnitDeclaration * node,AstNodeScope* scope)
	{
		completeScope = node;
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(CompilationUnitDeclaration * node,AstNodeScope* scope)
	{

	}
	
	bool  CompleteAssistASTVisitor::visit(AstClassBody * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
	
		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(AstClassBody * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(AstTypeParameter * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(AstTypeParameter * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(AstTypeParameters * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(AstTypeParameters * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(ClassTypeDeclaration * node,AstNodeScope* scope)
	{

		if (!IsInRange(node,scope))
			return false;
		if (!checkTypeDeclaration(node))
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

	void  CompleteAssistASTVisitor::endVisit(ClassTypeDeclaration * node,AstNodeScope* scope)
	{
		
	}
	bool  CompleteAssistASTVisitor::visit(AstArrayInitializer * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(AstArrayInitializer * node,AstNodeScope* scope)
	{

	}
	
	bool  CompleteAssistASTVisitor::visit(AstVariableDeclarator * node,AstNodeScope* scope)
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

	void  CompleteAssistASTVisitor::endVisit(AstVariableDeclarator * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(FieldDeclaration * node,AstNodeScope* scope)
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

	void  CompleteAssistASTVisitor::endVisit(FieldDeclaration * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(AstFormalParameter * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(AstFormalParameter * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(AstMethodDeclarator * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		

		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(AstMethodDeclarator * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(AstMethodBody * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		completeScope = node;

		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(AstMethodBody * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(MethodDeclaration * node,AstNodeScope* scope)
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

	void  CompleteAssistASTVisitor::endVisit(MethodDeclaration * node,AstNodeScope* scope)
	{
		
	}
	bool  CompleteAssistASTVisitor::visit(Initializer * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(Initializer * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(AstArguments * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(AstArguments * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(AstThisCall * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		if (node->this_token)
		{
			if (node->this_token->IsInRange(pos))
			{
		
				completNodeType = CompletionOnExplicitConstructorCall;
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
		if (tokenKind == TK_COMMA) {
			completNodeType = CompletionOnClassCreationExpressionDotSymbolInArguent;// 继续找，因为 可有有 a( b( d( e(1,[cursor]) ) ) ) 所以end_visit 不设置为0;
		}
		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(AstThisCall * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(AstSuperCall * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		if (node->super_token)
		{
			if (node->super_token->IsInRange(pos))
			{

				completNodeType = CompletionOnExplicitConstructorCall;
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
		if (tokenKind == TK_COMMA) {
			completNodeType = CompletionOnClassCreationExpressionDotSymbolInArguent;// 继续找，因为 可有有 a( b( d( e(1,[cursor]) ) ) ) 所以end_visit 不设置为0;
		}
		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(AstSuperCall * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(ConstructorDeclaration * node,AstNodeScope* scope)
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

	void  CompleteAssistASTVisitor::endVisit(ConstructorDeclaration * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(EnumTypeDeclaration * node,AstNodeScope* scope)
	{
		
		if (!IsInRange(node,scope))
			return false;
	
		if (!checkTypeDeclaration(node))
			return false;
		if (!IsInRange(node->class_body, scope)) {
			nextTypeReferenceIsInterface = true;
		}



		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(EnumTypeDeclaration * node,AstNodeScope* scope)
	{
		
	}
	bool  CompleteAssistASTVisitor::visit(AstEnumConstant * node,AstNodeScope* scope)
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

	void  CompleteAssistASTVisitor::endVisit(AstEnumConstant * node,AstNodeScope* scope)
	{

	}
	
	bool  CompleteAssistASTVisitor::visit(InterfaceTypeDeclaration * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		if (!checkTypeDeclaration(node))
			return false;

		if(!IsInRange(node->class_body, scope)){
			nextTypeReferenceIsInterface = true;
		}
		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(InterfaceTypeDeclaration * node,AstNodeScope* scope)
	{
		
	}
	bool  CompleteAssistASTVisitor::visit(AnnotationTypeDeclaration * node,AstNodeScope* scope)
	{
		
		if (!IsInRange(node,scope))
			return false;
	
		if (!checkTypeDeclaration(node))
			return false;

		
		return true;
	}

	

	bool  CompleteAssistASTVisitor::visit(LocalDeclaration * node, AstNodeScope* scope)
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
		  
	bool  CompleteAssistASTVisitor::visit(AstLocalClassStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	
	bool  CompleteAssistASTVisitor::visit(IfStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	
	
	bool  CompleteAssistASTVisitor::visit(AstExpressionStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	
	bool  CompleteAssistASTVisitor::visit(AstSwitchLabel * node,AstNodeScope* scope)
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

	void  CompleteAssistASTVisitor::endVisit(AstSwitchLabel * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(AstSwitchBlockStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(AstSwitchBlockStatement * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(SwitchStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(SwitchStatement * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(WhileStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(WhileStatement * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(DoStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(DoStatement * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(ForStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(ForStatement * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(ForeachStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(ForeachStatement * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(BreakStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		else
		{
			
		}
		return true;
	}

	
	bool  CompleteAssistASTVisitor::visit(ContinueStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		else
		{
		
		}
		return true;
	}

	
	bool  CompleteAssistASTVisitor::visit(ReturnStatement * node,AstNodeScope* scope)
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


	bool  CompleteAssistASTVisitor::visit(ThrowStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(ThrowStatement * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(SynchronizedStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(SynchronizedStatement * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(AssertStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(AssertStatement * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(AstCatchClause * node,AstNodeScope* scope)
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

	void  CompleteAssistASTVisitor::endVisit(AstCatchClause * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(AstFinallyClause * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		
		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(AstFinallyClause * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(TryStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(TryStatement * node,AstNodeScope* scope)
	{

	}
	
	
	bool  CompleteAssistASTVisitor::visit(ClassLiteralAccess * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(ClassLiteralAccess * node,AstNodeScope* scope)
	{
		
	}
	bool  CompleteAssistASTVisitor::visit(AstThisExpression * node,AstNodeScope* scope)
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

	void  CompleteAssistASTVisitor::endVisit(AstThisExpression * node,AstNodeScope* scope)
	{
		
	}
	bool  CompleteAssistASTVisitor::visit(AstSuperExpression * node,AstNodeScope* scope)
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

	void  CompleteAssistASTVisitor::endVisit(AstSuperExpression * node,AstNodeScope* scope)
	{
		
	}
	bool  CompleteAssistASTVisitor::visit(AstParenthesizedExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		if (tokenKind == TK_DOT  && node->right_parenthesis_token->EndLocation() == pos)
		{
			completNodeType = CompletionParenthesizedExpression;
			astNode = node;
			completeScope = scope;
			end_visit = true;
		}
		
		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(AstParenthesizedExpression * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(AstClassCreationExpression * node,AstNodeScope* scope)
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
		if (tokenKind == TK_COMMA) {
			completNodeType = CompletionOnClassCreationExpressionDotSymbolInArguent;// 继续找，因为 可有有 a( b( d( e(1,[cursor]) ) ) ) 所以end_visit 不设置为0;
		}

		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(AstClassCreationExpression * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(AstDimExpr * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(AstDimExpr * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(AstArrayCreationExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(AstArrayCreationExpression * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(FieldReference * node,AstNodeScope* scope)
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

	void  CompleteAssistASTVisitor::endVisit(FieldReference * node,AstNodeScope* scope)
	{
	}
	bool  CompleteAssistASTVisitor::visit(AstMethodInvocation * node,AstNodeScope* scope)
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
		if (tokenKind == TK_COMMA) {
			completNodeType = CompletionOnMethoDotSymbolInArguent;// 继续找，因为 可有有 a( b( d( e(1,[cursor]) ) ) ) 所以end_visit 不设置为0;
		}
		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(AstMethodInvocation * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(ArrayReference * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
	    if(tokenKind == TK_DOT  && node->right_bracket_token->EndLocation() == pos)
	    {
			completNodeType = CompletionOnArrayReference;
			astNode = node;
			completeScope = scope;
			end_visit = true;
	    }
		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(ArrayReference * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(AstPostUnaryExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(AstPostUnaryExpression * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(AstPreUnaryExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(AstPreUnaryExpression * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(CastExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;
		if (tokenKind == TK_DOT  && node->right_parenthesis_token->EndLocation() == pos)
		{
			completNodeType = CompletionOnArrayReference;
			astNode = node;
			completeScope = scope;
			end_visit = true;
		}
		
		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(CastExpression * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(BinaryExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(BinaryExpression * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(InstanceofExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(InstanceofExpression * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(ConditionalExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node,scope))
			return false;

		
		return true;
	}

	void  CompleteAssistASTVisitor::endVisit(ConditionalExpression * node,AstNodeScope* scope)
	{

	}
	bool  CompleteAssistASTVisitor::visit(AstAssignmentExpression * node,AstNodeScope* scope)
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

	void  CompleteAssistASTVisitor::endVisit(AstAssignmentExpression * node,AstNodeScope* scope)
	{

	}
	bool CompleteAssistASTVisitor::IsInRange(Ast* node)
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
		homeMakeAstLPos = start;
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
	bool CompleteAssistASTVisitor::IsInRange(Ast* node, AstNodeScope* scope)
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
		homeMakeAstLPos = start;
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
	
		return true;
	}
}//  namespace CodeAssist 
	

} // Close namespace Jikes block




