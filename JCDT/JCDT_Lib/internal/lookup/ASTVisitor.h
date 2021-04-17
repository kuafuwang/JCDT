

#ifndef ASTVisitor_INCLUDED
#define ASTVisitor_INCLUDED
#include  "JCDT_Lib/internal/ast/ast.h"

namespace Jikes { // Open namespace Jikes block


	class ASTVisitor
	{
	private:
		bool default_ret_value = false;
	protected:
		void setDefaultRetValue(bool _vl){
			default_ret_value = _vl;
		}
	public:
		virtual ~ASTVisitor()
		{
		}

		virtual bool visit(AstListNode * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstListNode * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstDeclared * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstDeclared * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(TypeDeclaration * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(TypeDeclaration * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(ReachComleteAbleStatement * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(ReachComleteAbleStatement * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstMemberValue * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstMemberValue * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(Expression * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(Expression * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstType * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstType * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstBlock * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstBlock * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstName * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstName * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstPrimitiveType * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstPrimitiveType * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstBrackets * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstBrackets * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstArrayType * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstArrayType * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstWildcard * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstWildcard * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstTypeArguments * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstTypeArguments * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstTypeName * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstTypeName * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstMemberValuePair * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstMemberValuePair * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstAnnotation * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstAnnotation * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstModifierKeyword * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstModifierKeyword * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstModifiers * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstModifiers * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstPackageDeclaration * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstPackageDeclaration * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstImportDeclaration * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstImportDeclaration * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(CompilationUnitDeclaration * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(CompilationUnitDeclaration * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(EmptyTypeDeclaration * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(EmptyTypeDeclaration * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstAnonymousTypeDeclaration * node, AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstAnonymousTypeDeclaration * node, AstNodeScope* scope)
		{

		}
		
		virtual bool visit(AstClassBody * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstClassBody * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstTypeParameter * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstTypeParameter * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstTypeParameters * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstTypeParameters * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(ClassTypeDeclaration * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(ClassTypeDeclaration * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstArrayInitializer * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstArrayInitializer * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstVariableDeclaratorId * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstVariableDeclaratorId * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstVariableDeclarator * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstVariableDeclarator * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(FieldDeclaration * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(FieldDeclaration * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstFormalParameter * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstFormalParameter * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstMethodDeclarator * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstMethodDeclarator * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstMethodBody * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstMethodBody * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(MethodDeclaration * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(MethodDeclaration * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(Initializer * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(Initializer * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstArguments * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstArguments * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstThisCall * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstThisCall * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstSuperCall * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstSuperCall * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(ConstructorDeclaration * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(ConstructorDeclaration * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(EnumTypeDeclaration * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(EnumTypeDeclaration * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstEnumConstant * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstEnumConstant * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(InterfaceTypeDeclaration * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(InterfaceTypeDeclaration * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AnnotationTypeDeclaration * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AnnotationTypeDeclaration * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(LocalDeclaration * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(LocalDeclaration * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstLocalClassStatement * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstLocalClassStatement * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(IfStatement * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(IfStatement * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(EmptyStatement * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(EmptyStatement * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstExpressionStatement * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstExpressionStatement * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstSwitchLabel * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstSwitchLabel * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstSwitchBlockStatement * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstSwitchBlockStatement * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(SwitchStatement * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(SwitchStatement * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(WhileStatement * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(WhileStatement * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(DoStatement * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(DoStatement * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(ForStatement * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(ForStatement * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(ForeachStatement * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(ForeachStatement * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(BreakStatement * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(BreakStatement * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(ContinueStatement * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(ContinueStatement * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(ReturnStatement * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(ReturnStatement * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(ThrowStatement * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(ThrowStatement * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(SynchronizedStatement * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(SynchronizedStatement * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AssertStatement * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AssertStatement * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstCatchClause * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstCatchClause * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstFinallyClause * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstFinallyClause * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(TryStatement * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(TryStatement * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(IntegerLiteral * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(IntegerLiteral * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(LongLiteral * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(LongLiteral * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(FloatLiteral * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(FloatLiteral * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(DoubleLiteral * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(DoubleLiteral * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(TrueLiteral * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(TrueLiteral * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(FalseLiteral * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(FalseLiteral * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(StringLiteral * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(StringLiteral * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(CharLiteral * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(CharLiteral * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(NullLiteral * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(NullLiteral * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(ClassLiteralAccess * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(ClassLiteralAccess * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstThisExpression * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstThisExpression * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstSuperExpression * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstSuperExpression * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstParenthesizedExpression * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstParenthesizedExpression * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstClassCreationExpression * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstClassCreationExpression * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstDimExpr * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstDimExpr * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstArrayCreationExpression * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstArrayCreationExpression * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(FieldReference * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(FieldReference * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstMethodInvocation * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstMethodInvocation * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(ArrayReference * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(ArrayReference * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstPostUnaryExpression * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstPostUnaryExpression * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstPreUnaryExpression * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstPreUnaryExpression * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(CastExpression * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(CastExpression * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(BinaryExpression * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(BinaryExpression * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(InstanceofExpression * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(InstanceofExpression * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(ConditionalExpression * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(ConditionalExpression * node,AstNodeScope* scope)
		{

		}
		virtual bool visit(AstAssignmentExpression * node,AstNodeScope* scope)
		{
			return default_ret_value;
		}

		virtual void endVisit(AstAssignmentExpression * node,AstNodeScope* scope)
		{

		}
};

	

} // Close namespace Jikes block


#endif // ASTVisitor_INCLUDED

