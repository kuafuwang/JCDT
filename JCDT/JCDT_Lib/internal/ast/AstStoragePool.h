#ifndef AstStoragePool_INCLUDED
#define AstStoragePool_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include <JCDT_Lib/internal/util/StoragePool.h>
#include "JCDT_Lib/internal/ast/AstNode.h"
#include "AssignmentExpressionTag.h"
#include "PreUnaryExpressionTag.h"
#include "PostUnaryExpressionTag.h"
#include "BinaryExpressionTag.h"

namespace Jikes {
	
	class Token;

	// Open namespace Jikes block
	// This Storage pool is similar to dynamic arrays (class Tuple). The
	// difference is that instead of a Next() function we have an Alloc(size_t)
	// function. The value of the size_t argument represents the size of the
	// object to allocate. The allocated memory is guaranteed to be
	// zero-initialized.
	//
	// All AST nodes for a given parse should be allocated from the same storage
	// pool, so they have a placement new operator that requires a StoragePool.
	// You should never delete an AST object, as all resources they allocate come
	// from the same pool. Instead, to reclaim memory when processing is complete,
	// simply delete the underlying storage pool.
	//
	class AstStoragePool :public StoragePool
	{
	public:
		AstStoragePool(unsigned num_tokens):StoragePool(num_tokens){}
		// ********************************************************************

		VariableSymbolArray* NewVariableSymbolArray(unsigned size = 0);

		AstListNode* NewListNode();

		AstBlock* NewBlock();
		SwitchBlock* NewSwitchBlock();
		AstName* NewName(Token* token);

		AstPrimitiveType* NewPrimitiveType(Ast::AstKind kind, Token* t) ;

		AstBrackets* NewBrackets(Token* left, Token* right) ;

		AstArrayType* NewArrayType(AstType* type, AstBrackets* brackets);

		AstWildcard* NewWildcard(Token* question);

		AstTypeArguments* NewTypeArguments(Token* l, Token* r);

		AstTypeName* NewTypeName(AstName* name);

		AstMemberValuePair* NewMemberValuePair();

		AstAnnotation* NewAnnotation();

		AstModifierKeyword* NewModifierKeyword(Token* token);

		AstModifiers* NewModifiers();

		AstPackageDeclaration* NewPackageDeclaration();

		AstImportDeclaration* NewImportDeclaration();

		CompilationUnitDeclaration* NewCompilationUnit();

		EmptyTypeDeclaration* NewEmptyDeclaration(Token* t);

		AstClassBody* NewClassBody();

		AstTypeParameter* NewTypeParameter(Token* token);

		AstTypeParameters* NewTypeParameters();

		ClassTypeDeclaration* NewClassDeclaration();

		AstArrayInitializer* NewArrayInitializer();

		AstVariableDeclaratorId* NewVariableDeclaratorId();

		AstVariableDeclarator* NewVariableDeclarator();

		FieldDeclaration* NewFieldDeclaration();

		AstFormalParameter* NewFormalParameter();

		AstMethodDeclarator* NewMethodDeclarator();

		AstMethodBody* NewMethodBody();

		MethodDeclaration* NewMethodDeclaration();

		Initializer* NewInitializerDeclaration();

		AstArguments* NewArguments(Token* left, Token* right);

		AstThisCall* NewThisCall();

		AstSuperCall* NewSuperCall();

		ConstructorDeclaration* NewConstructorDeclaration();

		EnumTypeDeclaration* NewEnumDeclaration();

		AstEnumConstant* NewEnumConstant(Token* t);

		InterfaceTypeDeclaration* NewInterfaceDeclaration();

		AnnotationTypeDeclaration* NewAnnotationDeclaration(Token* t);

		LocalDeclaration* NewLocalVariableStatement();

		AstLocalClassStatement* NewLocalClassStatement(ClassTypeDeclaration* decl);

		AstLocalClassStatement* NewLocalClassStatement(EnumTypeDeclaration* decl);

		IfStatement* NewIfStatement();

		EmptyStatement* NewEmptyStatement(Token* token);

		AstExpressionStatement* NewExpressionStatement();

		AstSwitchLabel* NewSwitchLabel();

		AstSwitchBlockStatement* NewSwitchBlockStatement();

		SwitchStatement* NewSwitchStatement();

		WhileStatement* NewWhileStatement();

		DoStatement* NewDoStatement();

		ForStatement* NewForStatement();

		ForeachStatement* NewForeachStatement();

		BreakStatement* NewBreakStatement();

		ContinueStatement* NewContinueStatement();

		ReturnStatement* NewReturnStatement();

		ThrowStatement* NewThrowStatement();

		SynchronizedStatement* NewSynchronizedStatement();

		AssertStatement* NewAssertStatement();

		AstCatchClause* NewCatchClause();

		AstFinallyClause* NewFinallyClause();

		TryStatement* NewTryStatement();

		IntegerLiteral* NewIntegerLiteral(Token* token);

		LongLiteral* NewLongLiteral(Token* token);

		FloatLiteral* NewFloatLiteral(Token* token);

		DoubleLiteral* NewDoubleLiteral(Token* token);

		TrueLiteral* NewTrueLiteral(Token* token);

		FalseLiteral* NewFalseLiteral(Token* token);

		StringLiteral* NewStringLiteral(Token* token);

		CharLiteral* NewCharacterLiteral(Token* token);

		NullLiteral* NewNullLiteral(Token* token);

		ClassLiteralAccess* NewClassLiteral(Token* token);

		AstThisExpression* NewThisExpression(Token* token);

		AstSuperExpression* NewSuperExpression(Token* token);

		AstParenthesizedExpression* NewParenthesizedExpression();

		AstClassCreationExpression* NewClassCreationExpression();

		AstDimExpr* NewDimExpr();

		AstArrayCreationExpression* NewArrayCreationExpression();

		FieldReference* NewFieldAccess();

		AstMethodInvocation* NewMethodInvocation(Token* t);

		ArrayReference* NewArrayAccess();

		AstPostUnaryExpression* NewPostUnaryExpression(PostUnaryExpressionEnum::PostUnaryExpressionTag tag);

		AstPreUnaryExpression* NewPreUnaryExpression(PreUnaryExpressionEnum::PreUnaryExpressionTag tag);

		CastExpression* NewCastExpression();

		BinaryExpression* NewBinaryExpression(BinaryExpressionEnum::BinaryExpressionTag tag);

		InstanceofExpression* NewInstanceofExpression();

		ConditionalExpression* NewConditionalExpression();

		AstAssignmentExpression* NewAssignmentExpression(AssignmentExpressionEnum:: AssignmentExpressionTag tag,
		                                                 Token* token);

		// *********************************************************************

		//
		// Note that CaseElement nodes are always generated. Since they are not
		// Ast nodes they do not need to be marked.
		//
		CaseElement* GenCaseElement(unsigned block_index,
		                            unsigned case_index);

		AstBlock* GenBlock();

		AstName* GenName(Token* token);

		AstPrimitiveType* GenPrimitiveType(Ast::AstKind kind, Token* t);

		AstBrackets* GenBrackets(Token* left, Token* right);

		AstArrayType* GenArrayType(AstType* type, AstBrackets* brackets);

		AstWildcard* GenWildcard(Token* question);

		AstTypeArguments* GenTypeArguments(Token* l, Token* r);

		AstTypeName* GenTypeName(AstName* type);

		AstMemberValuePair* GenMemberValuePair();

		AstAnnotation* GenAnnotation();

		AstModifierKeyword* GenModifierKeyword(Token* token);

		AstModifiers* GenModifiers();

		AstPackageDeclaration* GenPackageDeclaration();

		AstImportDeclaration* GenImportDeclaration();

		CompilationUnitDeclaration* GenCompilationUnit();

		AstAnonymousTypeDeclaration* GenAnonymousTypeDeclaration();
		EmptyTypeDeclaration* GenEmptyDeclaration(Token* t);

		AstClassBody* GenClassBody();

		AstTypeParameter* GenTypeParameter(Token* token);

		AstTypeParameters* GenTypeParameters();

		ClassTypeDeclaration* GenClassDeclaration();

		AstArrayInitializer* GenArrayInitializer();

		AstVariableDeclaratorId* GenVariableDeclaratorId();

		AstVariableDeclarator* GenVariableDeclarator();

		FieldDeclaration* GenFieldDeclaration();

		AstFormalParameter* GenFormalParameter();

		AstMethodDeclarator* GenMethodDeclarator();

		AstMethodBody* GenMethodBody();

		MethodDeclaration* GenMethodDeclaration();

		Initializer* GenInitializerDeclaration();

		AstArguments* GenArguments(Token* left, Token* right);

		AstThisCall* GenThisCall();

		AstSuperCall* GenSuperCall();

		ConstructorDeclaration* GenConstructorDeclaration();

		EnumTypeDeclaration* GenEnumDeclaration();

		AstEnumConstant* GenEnumConstant(Token* t);

		InterfaceTypeDeclaration* GenInterfaceDeclaration();

		AnnotationTypeDeclaration* GenAnnotationDeclaration(Token* t);

		LocalDeclaration* GenLocalVariableStatement();

		AstLocalClassStatement* GenLocalClassStatement(ClassTypeDeclaration* decl);

		AstLocalClassStatement* GenLocalClassStatement(EnumTypeDeclaration* decl);

		IfStatement* GenIfStatement();

		EmptyStatement* GenEmptyStatement(Token* token);

		AstExpressionStatement* GenExpressionStatement();

		AstSwitchLabel* GenSwitchLabel();

		AstSwitchBlockStatement* GenSwitchBlockStatement();

		SwitchStatement* GenSwitchStatement();

		WhileStatement* GenWhileStatement();

		DoStatement* GenDoStatement();

		ForStatement* GenForStatement();

		ForeachStatement* GenForeachStatement();

		BreakStatement* GenBreakStatement();

		ContinueStatement* GenContinueStatement();

		ReturnStatement* GenReturnStatement();

		ThrowStatement* GenThrowStatement();

		SynchronizedStatement* GenSynchronizedStatement();

		AssertStatement* GenAssertStatement();

		AstCatchClause* GenCatchClause();

		AstFinallyClause* GenFinallyClause();

		TryStatement* GenTryStatement();

		IntegerLiteral* GenIntegerLiteral(Token* token);

		LongLiteral* GenLongLiteral(Token* token);

		FloatLiteral* GenFloatLiteral(Token* token);

		DoubleLiteral* GenDoubleLiteral(Token* token);

		TrueLiteral* GenTrueLiteral(Token* token);

		FalseLiteral* GenFalseLiteral(Token* token);

		StringLiteral* GenStringLiteral(Token* token);

		CharLiteral* GenCharacterLiteral(Token* token);

		NullLiteral* GenNullLiteral(Token* token);

		ClassLiteralAccess* GenClassLiteral(Token* token);

		AstThisExpression* GenThisExpression(Token* token);

		AstSuperExpression* GenSuperExpression(Token* token);

		AstParenthesizedExpression* GenParenthesizedExpression();

		AstClassCreationExpression* GenClassCreationExpression();

		AstDimExpr* GenDimExpr();

		AstArrayCreationExpression* GenArrayCreationExpression();

		FieldReference* GenFieldAccess();

		AstMethodInvocation* GenMethodInvocation(Token* t);

		ArrayReference* GenArrayAccess();

		AstPostUnaryExpression* GenPostUnaryExpression(PostUnaryExpressionEnum::PostUnaryExpressionTag tag);

		AstPreUnaryExpression* GenPreUnaryExpression(PreUnaryExpressionEnum::PreUnaryExpressionTag tag);

		CastExpression* GenCastExpression();

		BinaryExpression* GenBinaryExpression(BinaryExpressionEnum::BinaryExpressionTag tag);

		InstanceofExpression* GenInstanceofExpression();

		ConditionalExpression* GenConditionalExpression();

		AstAssignmentExpression* GenAssignmentExpression(AssignmentExpressionEnum::AssignmentExpressionTag tag,
		                                                 Token* token);
	};






} // Close namespace Jikes block


#endif // AstStoragePool_INCLUDED