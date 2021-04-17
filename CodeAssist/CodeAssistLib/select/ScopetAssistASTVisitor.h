// $Id: option.h,v 1.43 2004/10/10 23:17:44 elliott-oss Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C)override ; 1996, 1998, 1999, 2000, 2001, 2002 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#ifndef CodeAssist_ScopetAssistASTVisitor_INCLUDED
#define CodeAssist_ScopetAssistASTVisitor_INCLUDED

#include <JCDT_Lib/internal/lookup/ASTVisitor.h>

//#include <CodeAssistLib/impl/AstNoFound.h>


namespace Jikes { // Open namespace Jikes block
	class TypeDeclaration;
	 
	namespace CodeAssist {
		class ScopetAssistASTVisitor :public ASTVisitor
		{
		public:

			AstNodeScope* selectedScope;

		
			unsigned pos;

			ScopetAssistASTVisitor();
			 void checkTypeDeclaration(TypeDeclaration* node, unsigned pos);
		
			bool IsInRange(Ast*, AstNodeScope*) ;

			void TokenInPosThenNoFound(Token* tk) const;
		
			 bool visit(AstBlock * node,AstNodeScope* scope)override;

			 bool visit(AstName * node,AstNodeScope* scope)override;

			 bool visit(AstArrayType * node,AstNodeScope* scope)override;


			 void endVisit(AstArrayType * node,AstNodeScope* scope)override;

			 bool visit(AstWildcard * node,AstNodeScope* scope)override;


			 void endVisit(AstWildcard * node,AstNodeScope* scope)override;



			 bool visit(AstTypeArguments * node,AstNodeScope* scope)override;


			 bool visit(AstTypeName * node,AstNodeScope* scope)override;

		
			 bool visit(AstMemberValuePair * node,AstNodeScope* scope)override;

		
			 bool visit(AstAnnotation * node,AstNodeScope* scope)override;

		
			 bool visit(CompilationUnitDeclaration * node,AstNodeScope* scope)override;

		
	

			 bool visit(AstClassBody * node,AstNodeScope* scope)override;


		

			 bool visit(ClassTypeDeclaration * node,AstNodeScope* scope)override;


		
			 bool visit(AstArrayInitializer * node,AstNodeScope* scope)override;


		
			 bool visit(AstVariableDeclarator * node,AstNodeScope* scope)override;


		
			 bool visit(FieldDeclaration * node,AstNodeScope* scope)override;


		
			 bool visit(AstFormalParameter * node,AstNodeScope* scope)override;


		
			 bool visit(AstMethodDeclarator * node,AstNodeScope* scope)override;


		
			 bool visit(AstMethodBody * node,AstNodeScope* scope)override;


		
			 bool visit(MethodDeclaration * node,AstNodeScope* scope)override;

		
			 bool visit(Initializer * node,AstNodeScope* scope)override;


		
			 bool visit(AstArguments * node,AstNodeScope* scope)override;

		
			 bool visit(ConstructorDeclaration * node,AstNodeScope* scope)override;


			
			 bool visit(EnumTypeDeclaration * node,AstNodeScope* scope)override;


			 bool visit(AstEnumConstant * node,AstNodeScope* scope)override;


			 bool visit(InterfaceTypeDeclaration * node,AstNodeScope* scope)override;


			 bool visit(AnnotationTypeDeclaration * node,AstNodeScope* scope)override;


			 bool visit(LocalDeclaration * node,AstNodeScope* scope)override;


			 bool visit(AstLocalClassStatement * node,AstNodeScope* scope)override;

			 bool visit(IfStatement * node,AstNodeScope* scope)override;


			 bool visit(AstExpressionStatement * node,AstNodeScope* scope)override;

			 bool visit(AstSwitchLabel * node,AstNodeScope* scope)override;

			 bool visit(AstSwitchBlockStatement * node,AstNodeScope* scope)override;

			 bool visit(SwitchStatement * node,AstNodeScope* scope)override;


			 bool visit(WhileStatement * node,AstNodeScope* scope)override;

			
			 bool visit(DoStatement * node,AstNodeScope* scope)override;


		
			 bool visit(ForStatement * node,AstNodeScope* scope)override;


			
			 bool visit(ForeachStatement * node,AstNodeScope* scope)override;


			 bool visit(BreakStatement * node,AstNodeScope* scope)override;


			 bool visit(ContinueStatement * node,AstNodeScope* scope)override;


			 bool visit(ReturnStatement * node,AstNodeScope* scope)override;

	
			 bool visit(ThrowStatement * node,AstNodeScope* scope)override;


			 bool visit(SynchronizedStatement * node,AstNodeScope* scope)override;


			 bool visit(AssertStatement * node,AstNodeScope* scope)override;


			 bool visit(AstCatchClause * node,AstNodeScope* scope)override;


			 bool visit(AstFinallyClause * node,AstNodeScope* scope)override;


			 bool visit(TryStatement * node,AstNodeScope* scope)override;


			 bool visit(ClassLiteralAccess * node,AstNodeScope* scope)override;


			 bool visit(AstThisExpression * node,AstNodeScope* scope)override;


			 bool visit(AstParenthesizedExpression * node,AstNodeScope* scope)override;

			 bool visit(AstClassCreationExpression * node,AstNodeScope* scope)override;


		
			 bool visit(AstDimExpr * node,AstNodeScope* scope)override;


			 bool visit(AstArrayCreationExpression * node,AstNodeScope* scope)override;


			 bool visit(FieldReference * node,AstNodeScope* scope)override;


			 void endVisit(FieldReference * node,AstNodeScope* scope)override;

			 bool visit(AstMethodInvocation * node,AstNodeScope* scope)override;


			 void endVisit(AstMethodInvocation * node,AstNodeScope* scope)override;

			 bool visit(ArrayReference * node,AstNodeScope* scope)override;


			 void endVisit(ArrayReference * node,AstNodeScope* scope)override;

			 bool visit(AstPostUnaryExpression * node,AstNodeScope* scope)override;


			 void endVisit(AstPostUnaryExpression * node,AstNodeScope* scope)override;

			 bool visit(AstPreUnaryExpression * node,AstNodeScope* scope)override;


			 void endVisit(AstPreUnaryExpression * node,AstNodeScope* scope)override;

			 bool visit(CastExpression * node,AstNodeScope* scope)override;


			 void endVisit(CastExpression * node,AstNodeScope* scope)override;

			 bool visit(BinaryExpression * node,AstNodeScope* scope)override;

			 void endVisit(BinaryExpression * node,AstNodeScope* scope)override;

			 bool visit(InstanceofExpression * node,AstNodeScope* scope)override;


			 void endVisit(InstanceofExpression * node,AstNodeScope* scope)override;

			 bool visit(ConditionalExpression * node,AstNodeScope* scope)override;


			 void endVisit(ConditionalExpression * node,AstNodeScope* scope)override;

			 bool visit(AstAssignmentExpression * node,AstNodeScope* scope)override;


			 void endVisit(AstAssignmentExpression * node,AstNodeScope* scope)override;

		};
	
		inline bool ScopetAssistASTVisitor::IsInRange(Ast* node, AstNodeScope* scope) 
		{
			auto start = node->LeftToken()->start_location;
			if (pos < start)
			{
				return false;
			}
			auto end = node->RightToken()->EndLocation();
			if (pos > end)
			{
				return false;
			}
		
			return true;
		}


		
  }//namespace CodeAssist

} // Close namespace Jikes block


#endif // CodeAssist_ScopetAssistASTVisitor_INCLUDED

