
#include "stdafx.h"
#include <CodeAssistLib/impl/AstNoFound.h>
#include "SelectWhenDebugAssistASTVisitor.h"
#include "AstFound.h"

#include <CodeAssistLib/select/SelectionOnNameReference.h>
#include "SelectionWhenDebug.h"


namespace Jikes { // Open namespace Jikes block
	namespace CodeAssist {

		void checkTypeDeclaration(TypeDeclaration* node, unsigned pos)
		{
			if (node && node->class_body && node->class_body->identifier_token
				&& node->class_body->identifier_token->IsInRange(pos))
			{	
				throw AstFound(node, AstFound::FIND_TYPE);	
			}
		}


		SelectWhenDebugAssistASTVisitor::SelectWhenDebugAssistASTVisitor(vector<TypeDeclaration*>& _type_stack)
			:enter_point_(ENTER_NULL), pos(0), type_stack(_type_stack)
		{
				
		}

		wstring SelectWhenDebugAssistASTVisitor::getDeclareTypeName()
		{
			wstring _typeName;
			int _size = type_stack.size();
			for (auto i = 0; i < _size; ++i)
			{
				if(type_stack[i] && type_stack[i]->class_body && type_stack[i]->class_body->identifier_token)
				{
					auto _name = type_stack[i]->class_body->identifier_token->NameString();
					if (i == 0) {
						_typeName.append(_name);
					}
					else {
						_typeName.append(L".").append(_name);
					}
				}	
			}
			return _typeName;
		}

	 
	

	bool  SelectWhenDebugAssistASTVisitor::visit(AstBlock * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;
		return true;
	}

	bool  SelectWhenDebugAssistASTVisitor::visit(AstName * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;

		
		if (node->identifier_token->IsInRange(pos))
		{
			
			if(enter_point_ == ENTER_IMPORT)
			{
				auto import_decl = dynamic_cast<AstImportDeclaration*>(enterpoint_node);
				import_decl->name = node;	
				throw AstFound(import_decl, AstFound::FIND_IMPORT);
			}
			
			if(enter_point_ == ENTER_PACKAGE)
			{
				auto pkg = dynamic_cast<AstPackageDeclaration*>(enterpoint_node);
				pkg->name = node;
				throw AstFound(pkg, AstFound::FIND_PACKAGE);
			}
		
			throw SelectionWhenDebug(node,SelectionWhenDebug::AstExpressOnName,scope);	
		}
		

		return true;
	}

	
	bool  SelectWhenDebugAssistASTVisitor::visit(AstArrayType * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;
		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(AstArrayType * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(AstWildcard * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;
		throw AstNoFound();
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(AstWildcard * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(AstTypeArguments * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;
		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(AstTypeArguments * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(AstTypeName * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;
		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(AstTypeName * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(AstMemberValuePair * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(AstMemberValuePair * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(AstAnnotation * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(AstAnnotation * node,AstNodeScope* scope)
	{

	}
	
	bool  SelectWhenDebugAssistASTVisitor::visit(AstPackageDeclaration * node,AstNodeScope* scope)
	{

		if (!IsInRange(node))
			return false;
		enterpoint_node = node;
		enter_point_ = ENTER_PACKAGE;

		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(AstPackageDeclaration * node,AstNodeScope* scope)
	{
		
	}
	bool  SelectWhenDebugAssistASTVisitor::visit(AstImportDeclaration * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;
		enterpoint_node = node;
		enter_point_ = ENTER_IMPORT;

		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(AstImportDeclaration * node,AstNodeScope* scope)
	{
	}
	bool  SelectWhenDebugAssistASTVisitor::visit(CompilationUnitDeclaration * node,AstNodeScope* scope)
	{
		
		if (!IsInRange(node))
			return false;
		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(CompilationUnitDeclaration * node,AstNodeScope* scope)
	{

	}
	
	bool  SelectWhenDebugAssistASTVisitor::visit(AstClassBody * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;
		

		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(AstClassBody * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(AstTypeParameter * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;
		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(AstTypeParameter * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(AstTypeParameters * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;
		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(AstTypeParameters * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(ClassTypeDeclaration * node,AstNodeScope* scope)
	{
		type_stack.push_back(node);
		if (!IsInRange(node))
			return false;
		checkTypeDeclaration(node, pos);
		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(ClassTypeDeclaration * node,AstNodeScope* scope)
	{
		type_stack.pop_back();
	}
	bool  SelectWhenDebugAssistASTVisitor::visit(AstArrayInitializer * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;
		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(AstArrayInitializer * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(AstVariableDeclaratorId * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;
	
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(AstVariableDeclaratorId * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(AstVariableDeclarator * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;
		if (node->variable_declarator_name && node->variable_declarator_name->identifier_token
			&&node->variable_declarator_name->identifier_token->IsInRange(pos))
		{
			throw SelectionWhenDebug(node, SelectionWhenDebug::OnAstVariableDeclarator, scope);
		}
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(AstVariableDeclarator * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(FieldDeclaration * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;
		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(FieldDeclaration * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(AstFormalParameter * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;
		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(AstFormalParameter * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(AstMethodDeclarator * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;
		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(AstMethodDeclarator * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(AstMethodBody * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;
		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(AstMethodBody * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(MethodDeclaration * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;

		if (node->method_declarator && node->method_declarator->identifier_token)
		{
			
				if (node->method_declarator->identifier_token->IsInRange(pos))
				{
					throw SelectionOnNameReference(node, SelectionOnNameReference::SelectionOnMethod, scope);
				}
			
		}
		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(MethodDeclaration * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(Initializer * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;

		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(Initializer * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(AstArguments * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;

		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(AstArguments * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(AstThisCall * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;

		if (!IsInRange(node))
			return false;
		if (node->this_token && node->this_token->IsInRange(pos))
		{
				throw SelectionOnNameReference(node,
					SelectionOnNameReference::SelectionOnThisCall, scope);
		}

		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(AstThisCall * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(AstSuperCall * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;
		if (!IsInRange(node))
			return false;
		if (node->super_token && node->super_token->IsInRange(pos) )
		{
				throw SelectionOnNameReference(node,
					SelectionOnNameReference::SelectionOnSuperCall, scope);
		}
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(AstSuperCall * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(ConstructorDeclaration * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;

		if (node->method_declarator && node->method_declarator->identifier_token)
		{
			if (node->method_declarator->identifier_token->IsInRange(pos))
			{
				throw SelectionOnNameReference(node, SelectionOnNameReference::SelectionOnConstructor, scope);	
			}
		}
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(ConstructorDeclaration * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(EnumTypeDeclaration * node,AstNodeScope* scope)
	{
		type_stack.push_back(node);
		if (!IsInRange(node))
			return false;
	
		checkTypeDeclaration(node, pos);
		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(EnumTypeDeclaration * node,AstNodeScope* scope)
	{
		type_stack.pop_back();
	}
	bool  SelectWhenDebugAssistASTVisitor::visit(AstEnumConstant * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
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

	void  SelectWhenDebugAssistASTVisitor::endVisit(AstEnumConstant * node,AstNodeScope* scope)
	{

	}
	
	bool  SelectWhenDebugAssistASTVisitor::visit(InterfaceTypeDeclaration * node,AstNodeScope* scope)
	{
		type_stack.push_back(node);
		if (!IsInRange(node))
			return false;
		checkTypeDeclaration(node, pos);
		
		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(InterfaceTypeDeclaration * node,AstNodeScope* scope)
	{
		type_stack.pop_back();
	}
	bool  SelectWhenDebugAssistASTVisitor::visit(AnnotationTypeDeclaration * node,AstNodeScope* scope)
	{
		type_stack.push_back(node);
		if (!IsInRange(node))
			return false;
	
		checkTypeDeclaration(node, pos);

		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(AnnotationTypeDeclaration * node,AstNodeScope* scope)
	{
		type_stack.pop_back();
	}
	bool  SelectWhenDebugAssistASTVisitor::visit(LocalDeclaration * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;

		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(LocalDeclaration * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(AstLocalClassStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;

		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(AstLocalClassStatement * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(IfStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;

		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(IfStatement * node,AstNodeScope* scope)
	{

	}
	
	bool  SelectWhenDebugAssistASTVisitor::visit(AstExpressionStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;

		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(AstExpressionStatement * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(AstSwitchLabel * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;
		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(AstSwitchLabel * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(AstSwitchBlockStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;

		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(AstSwitchBlockStatement * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(SwitchStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;
		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(SwitchStatement * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(WhileStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;

		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(WhileStatement * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(DoStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;

		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(DoStatement * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(ForStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;
		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(ForStatement * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(ForeachStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;

		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(ForeachStatement * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(BreakStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;
		else
		{
			throw AstNoFound();
		}
		return true;
	}

	
	bool  SelectWhenDebugAssistASTVisitor::visit(ContinueStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;
		else
		{
			throw AstNoFound();
		}
		return true;
	}

	
	bool  SelectWhenDebugAssistASTVisitor::visit(ReturnStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;
		
		return true;
	}


	bool  SelectWhenDebugAssistASTVisitor::visit(ThrowStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;

		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(ThrowStatement * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(SynchronizedStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;

		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(SynchronizedStatement * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(AssertStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;

		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(AssertStatement * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(AstCatchClause * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;

		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(AstCatchClause * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(AstFinallyClause * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;
		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(AstFinallyClause * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(TryStatement * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;

		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(TryStatement * node,AstNodeScope* scope)
	{

	}
	
	
	bool  SelectWhenDebugAssistASTVisitor::visit(ClassLiteralAccess * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;

		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(ClassLiteralAccess * node,AstNodeScope* scope)
	{
		
	}
	bool  SelectWhenDebugAssistASTVisitor::visit(AstThisExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;
		if (node->this_token && node->this_token->IsInRange(pos))
		{
			throw SelectionWhenDebug(node, SelectionWhenDebug::AstThisExpressionOnThis, scope);
		}
		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(AstThisExpression * node,AstNodeScope* scope)
	{
		
	}
	bool  SelectWhenDebugAssistASTVisitor::visit(AstSuperExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;
		if (node->super_token && node->super_token->IsInRange(pos))
		{
			throw SelectionWhenDebug(node, SelectionWhenDebug::AstSuperExpressionOnSuper, scope);
		}
		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(AstSuperExpression * node,AstNodeScope* scope)
	{
		
	}
	bool  SelectWhenDebugAssistASTVisitor::visit(AstParenthesizedExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;

		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(AstParenthesizedExpression * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(AstClassCreationExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;
	
		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(AstClassCreationExpression * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(AstDimExpr * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;

		if (node->left_bracket_token->IsInRange(pos))
			throw AstNoFound();

		if (node->right_bracket_token->IsInRange(pos))
			throw AstNoFound();
	
		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(AstDimExpr * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(AstArrayCreationExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;
		// 对于调试来说，前面的没有记录的意义了。
		if(node->brackets_opt){
			if (IsInRange(node->brackets_opt)){
				throw AstNoFound();
			}
		}
		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(AstArrayCreationExpression * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(FieldReference * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;
		if(node->identifier_token && node->identifier_token->IsInRange(pos))
		{
			throw SelectionWhenDebug(node, SelectionWhenDebug::AstExpressOnFieldReference, scope);
		}
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(FieldReference * node,AstNodeScope* scope)
	{
	}
	bool  SelectWhenDebugAssistASTVisitor::visit(AstMethodInvocation * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;
		
		if (node->identifier_token && node->identifier_token->IsInRange(pos))
		{
				throw SelectionOnNameReference(node,
					SelectionOnNameReference::SelectionOnExpression, scope);

		}
		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(AstMethodInvocation * node,AstNodeScope* scope)
	{

	}

	bool InBracket(ArrayReference * node,int _pos){

		if( !(node->expression && node->expression->RightToken() 
			                    && node->expression->LeftToken()) )
		{
			return false;
		}

		auto l_express_pos = node->expression->LeftToken()->start_location;

		if(_pos < l_express_pos){
			auto l_pos = node->left_bracket_token->start_location;
			if(l_pos <= _pos ){
				return true;
			}
			return false;
		}
		auto r_exress_pos = node->expression->RightToken()->EndLocation();
		if(_pos > r_exress_pos)
		{
		    if(node->right_bracket_token)
			{
				auto r_pos = node->right_bracket_token->start_location;
				if (_pos <= r_pos) 
				{
					return true;
				}
		    }
		}
		return false;
	}
	bool  SelectWhenDebugAssistASTVisitor::visit(ArrayReference * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;	
		bool _in_bracke = InBracket(node, pos);
		if(_in_bracke){
			throw SelectionWhenDebug(node, SelectionWhenDebug::ArrayReferenceExepression, scope);
		}

	
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(ArrayReference * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(AstPostUnaryExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;

		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(AstPostUnaryExpression * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(AstPreUnaryExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;

		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(AstPreUnaryExpression * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(CastExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;
		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(CastExpression * node,AstNodeScope* scope)
	{
		
	}
	bool  SelectWhenDebugAssistASTVisitor::visit(BinaryExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;

		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(BinaryExpression * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(InstanceofExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;

		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(InstanceofExpression * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(ConditionalExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;

		
		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(ConditionalExpression * node,AstNodeScope* scope)
	{

	}
	bool  SelectWhenDebugAssistASTVisitor::visit(AstAssignmentExpression * node,AstNodeScope* scope)
	{
		if (!IsInRange(node))
			return false;

		

		return true;
	}

	void  SelectWhenDebugAssistASTVisitor::endVisit(AstAssignmentExpression * node,AstNodeScope* scope)
	{

	}


}//  namespace CodeAssist 
	

} // Close namespace Jikes block




