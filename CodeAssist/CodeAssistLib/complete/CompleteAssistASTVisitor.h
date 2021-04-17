
#ifndef CodeAssist_CompletAssistASTVisitor_INCLUDED
#define CodeAssist_CompletAssistASTVisitor_INCLUDED

#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <stack>

namespace Jikes { // Open namespace Jikes block
	class TypeDeclaration;
	namespace CodeAssist {

		class CompleteAssistASTVisitor :public ASTVisitor
		{
		public:
		
			unsigned	int tokenKind;
			enum  CompletNodeType
			{

				NoFoundCompletion,

				CompletionOnMemberAccess,
				CompletionOnMessageSend,

				CompletionOnArrayReference,
				CompletionOnAssignmentExpressionEqualOpt,
				CompletionOnAssignmentExpressionBLANK,

				CompletionParenthesizedExpression,
				CompletionCastExpression,

				CompletionOnExplicitConstructorCall,

				CompletionOnExplicittThisExpr,
				CompletionOnExplicittSuperExpr,



				CompletionOnNameReference,

				CompletionOnAstTypeName,

				CompletionOnImportReference,

				CompletionOnPackageReference,


				CompletionOnClassCreationExpressionOnTypeName,

				CompletionOnClassCreationExpressionDotSymbolInArguent,


				CompletionOnMethodName,
				CompletionOnMethoDotSymbolInArguent,

				CompletionOnMethodReturnType,


				CompletionOnClassReference,
				CompletionOnInterfaceReference,
				CompletionOnExceptionReference,


				CompletionOnFieldName,
				CompletionOnLocalFieldName,
				CompletionOnFieldType,
				CompletionOnLocalFieldType,


				CompletionOnModifiers,

				CompletionOnReturnStateMent,
				CompletionOnCaseStatement

			};

			bool nextTypeReferenceIsClass;
			bool nextTypeReferenceIsException;
			bool nextTypeReferenceIsInterface;
			bool assistNodeIsInsideCase;
			bool assistNodeIsExtendedType;

			 Ast*   lastInRangeMethodCallNode;
			 int  homeMakeAstLPos;
			bool end_visit;

			void ResetForHomeMakeAstTraverse();
			CompleteAssistASTVisitor(unsigned _pos);
		
			Ast* enterpoint_node = nullptr;
			bool checkTypeDeclaration(TypeDeclaration* node);
			CompletNodeType completNodeType;
			Ast*  astNode;
			AstNodeScope* completeScope;

			unsigned pos;
		
			bool IsInRange(Ast*, AstNodeScope* ) ;
			bool IsInRange(Ast*);
			void TokenInPosThenNoFound(Token* tk) const;
	
			
			 bool visit(AstBlock * node,AstNodeScope* scope)override;

	
			 bool visit(AstName * node,AstNodeScope* scope)override;

	

			 bool visit(AstArrayType * node,AstNodeScope* scope)override;


			 void endVisit(AstArrayType * node,AstNodeScope* scope)override;

			 bool visit(AstWildcard * node,AstNodeScope* scope)override;


			 void endVisit(AstWildcard * node,AstNodeScope* scope)override;



			 bool visit(AstTypeArguments * node,AstNodeScope* scope)override;


			 void endVisit(AstTypeArguments * node,AstNodeScope* scope)override;

			 bool visit(AstTypeName * node,AstNodeScope* scope)override;

			 void endVisit(AstTypeName * node,AstNodeScope* scope)override;

			 bool visit(AstMemberValuePair * node,AstNodeScope* scope)override;

			 void endVisit(AstMemberValuePair * node,AstNodeScope* scope)override;

			 bool visit(AstAnnotation * node,AstNodeScope* scope)override;

			 void endVisit(AstAnnotation * node,AstNodeScope* scope)override;
			 bool visit(AstModifiers* node, AstNodeScope* scope) override;
			 bool visit(AstModifierKeyword* node, AstNodeScope* scope);

			bool visit(AstPackageDeclaration * node,AstNodeScope* scope)override;

			 void endVisit(AstPackageDeclaration * node,AstNodeScope* scope)override;

			 bool visit(AstImportDeclaration * node,AstNodeScope* scope)override;


			 void endVisit(AstImportDeclaration * node,AstNodeScope* scope)override;

			 bool visit(CompilationUnitDeclaration * node,AstNodeScope* scope)override;

			 void endVisit(CompilationUnitDeclaration * node,AstNodeScope* scope)override;

	

			 bool visit(AstClassBody * node,AstNodeScope* scope)override;


			 void endVisit(AstClassBody * node,AstNodeScope* scope)override;

			 bool visit(AstTypeParameter * node,AstNodeScope* scope)override;


			 void endVisit(AstTypeParameter * node,AstNodeScope* scope)override;

			 bool visit(AstTypeParameters * node,AstNodeScope* scope)override;


			 void endVisit(AstTypeParameters * node,AstNodeScope* scope)override;

			 bool visit(ClassTypeDeclaration * node,AstNodeScope* scope)override;


			 void endVisit(ClassTypeDeclaration * node,AstNodeScope* scope)override;

			 bool visit(AstArrayInitializer * node,AstNodeScope* scope)override;


			 void endVisit(AstArrayInitializer * node,AstNodeScope* scope)override;

		
			 bool visit(AstVariableDeclarator * node,AstNodeScope* scope)override;


			 void endVisit(AstVariableDeclarator * node,AstNodeScope* scope)override;

			 bool visit(FieldDeclaration * node,AstNodeScope* scope)override;


			 void endVisit(FieldDeclaration * node,AstNodeScope* scope)override;

			 bool visit(AstFormalParameter * node,AstNodeScope* scope)override;


			 void endVisit(AstFormalParameter * node,AstNodeScope* scope)override;

			 bool visit(AstMethodDeclarator * node,AstNodeScope* scope)override;


			 void endVisit(AstMethodDeclarator * node,AstNodeScope* scope)override;

			 bool visit(AstMethodBody * node,AstNodeScope* scope)override;


			 void endVisit(AstMethodBody * node,AstNodeScope* scope)override;

			 bool visit(MethodDeclaration * node,AstNodeScope* scope)override;

			 void endVisit(MethodDeclaration * node,AstNodeScope* scope)override;

			 bool visit(Initializer * node,AstNodeScope* scope)override;


			 void endVisit(Initializer * node,AstNodeScope* scope)override;

			 bool visit(AstArguments * node,AstNodeScope* scope)override;

			 void endVisit(AstArguments * node,AstNodeScope* scope)override;

			 bool visit(AstThisCall * node,AstNodeScope* scope)override;


			 void endVisit(AstThisCall * node,AstNodeScope* scope)override;

			 bool visit(AstSuperCall * node,AstNodeScope* scope)override;


			 void endVisit(AstSuperCall * node,AstNodeScope* scope)override;

			 bool visit(ConstructorDeclaration * node,AstNodeScope* scope)override;


			 void endVisit(ConstructorDeclaration * node,AstNodeScope* scope)override;

			 bool visit(EnumTypeDeclaration * node,AstNodeScope* scope)override;


			 void endVisit(EnumTypeDeclaration * node,AstNodeScope* scope)override;

			 bool visit(AstEnumConstant * node,AstNodeScope* scope)override;


			 void endVisit(AstEnumConstant * node,AstNodeScope* scope)override;

			 bool visit(InterfaceTypeDeclaration * node,AstNodeScope* scope)override;


			 void endVisit(InterfaceTypeDeclaration * node,AstNodeScope* scope)override;



			 bool visit(AnnotationTypeDeclaration * node,AstNodeScope* scope)override;


			
			 bool visit(LocalDeclaration * node,AstNodeScope* scope)override;


		
			 bool visit(AstLocalClassStatement * node,AstNodeScope* scope)override;


			
			 bool visit(IfStatement * node,AstNodeScope* scope)override;


	

			 bool visit(AstExpressionStatement * node,AstNodeScope* scope)override;


			
			 bool visit(AstSwitchLabel * node,AstNodeScope* scope)override;


			 void endVisit(AstSwitchLabel * node,AstNodeScope* scope)override;

			 bool visit(AstSwitchBlockStatement * node,AstNodeScope* scope)override;

			 void endVisit(AstSwitchBlockStatement * node,AstNodeScope* scope)override;

			 bool visit(SwitchStatement * node,AstNodeScope* scope)override;



			 void endVisit(SwitchStatement * node,AstNodeScope* scope)override;


			 bool visit(WhileStatement * node,AstNodeScope* scope)override;

			 void endVisit(WhileStatement * node,AstNodeScope* scope)override;

			 bool visit(DoStatement * node,AstNodeScope* scope)override;


			 void endVisit(DoStatement * node,AstNodeScope* scope)override;

			 bool visit(ForStatement * node,AstNodeScope* scope)override;


			 void endVisit(ForStatement * node,AstNodeScope* scope)override;

			 bool visit(ForeachStatement * node,AstNodeScope* scope)override;


			 void endVisit(ForeachStatement * node,AstNodeScope* scope)override;

			 bool visit(BreakStatement * node,AstNodeScope* scope)override;


			 bool visit(ContinueStatement * node,AstNodeScope* scope)override;


			 bool visit(ReturnStatement * node,AstNodeScope* scope)override;

	
			 bool visit(ThrowStatement * node,AstNodeScope* scope)override;


			 void endVisit(ThrowStatement * node,AstNodeScope* scope)override;

			 bool visit(SynchronizedStatement * node,AstNodeScope* scope)override;


			 void endVisit(SynchronizedStatement * node,AstNodeScope* scope)override;

			 bool visit(AssertStatement * node,AstNodeScope* scope)override;


			 void endVisit(AssertStatement * node,AstNodeScope* scope)override;

			 bool visit(AstCatchClause * node,AstNodeScope* scope)override;


			 void endVisit(AstCatchClause * node,AstNodeScope* scope)override;

			 bool visit(AstFinallyClause * node,AstNodeScope* scope)override;


			 void endVisit(AstFinallyClause * node,AstNodeScope* scope)override;

			 bool visit(TryStatement * node,AstNodeScope* scope)override;


			 void endVisit(TryStatement * node,AstNodeScope* scope)override;

	
			 bool visit(ClassLiteralAccess * node,AstNodeScope* scope)override;


			 void endVisit(ClassLiteralAccess * node,AstNodeScope* scope)override;

			 bool visit(AstThisExpression * node,AstNodeScope* scope)override;


			 void endVisit(AstThisExpression * node,AstNodeScope* scope)override;

			 bool visit(AstSuperExpression * node,AstNodeScope* scope)override;


			 void endVisit(AstSuperExpression * node,AstNodeScope* scope)override;

			 bool visit(AstParenthesizedExpression * node,AstNodeScope* scope)override;

			 void endVisit(AstParenthesizedExpression * node,AstNodeScope* scope)override;

			 bool visit(AstClassCreationExpression * node,AstNodeScope* scope)override;


			 void endVisit(AstClassCreationExpression * node,AstNodeScope* scope)override;


			 bool visit(AstDimExpr * node,AstNodeScope* scope)override;


			 void endVisit(AstDimExpr * node,AstNodeScope* scope)override;

			 bool visit(AstArrayCreationExpression * node,AstNodeScope* scope)override;


			 void endVisit(AstArrayCreationExpression * node,AstNodeScope* scope)override;

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


		inline void CompleteAssistASTVisitor::TokenInPosThenNoFound(Token* tk)const
		{
			if (tk)
			{
				if (tk->IsInRange(pos))
				{
					
				}
			}
		}
		


		
  }//namespace CodeAssist

} // Close namespace Jikes block


#endif // ASTVisitor_INCLUDED

