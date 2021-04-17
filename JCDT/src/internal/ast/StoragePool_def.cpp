#include <JCDT_Lib/internal/ast/AstAnonymousTypeDeclaration.h>
#ifndef StoragePool_def_INCLUDED
#define StoragePool_def_INCLUDED
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
#include "JCDT_Lib/internal/lex/Token.h"
#include "JCDT_Lib/internal/ast/CaseElement.h"
#include "JCDT_Lib/internal/ast/ASTNode.h"


#include "JCDT_Lib/internal/ast/AstListNode.h"

#include "JCDT_Lib/internal/ast/AstType.h"
#include "JCDT_Lib/internal/ast/AstBlock.h"
#include "JCDT_Lib/internal/ast/AstName.h"
#include "JCDT_Lib/internal/ast/AstPrimitiveType.h"
#include "JCDT_Lib/internal/ast/AstBrackets.h"
#include "JCDT_Lib/internal/ast/AstArrayType.h"
#include "JCDT_Lib/internal/ast/AstWildcard.h"
#include "JCDT_Lib/internal/ast/AstTypeArguments.h"
#include "JCDT_Lib/internal/ast/AstTypeName.h"
#include "JCDT_Lib/internal/ast/AstMemberValuePair.h"
#include "JCDT_Lib/internal/ast/AstAnnotation.h"
#include "JCDT_Lib/internal/ast/AstModifierKeyword.h"
#include "JCDT_Lib/internal/ast/AstModifiers.h"
#include "JCDT_Lib/internal/ast/AstPackageDeclaration.h"
#include "JCDT_Lib/internal/ast/AstImportDeclaration.h"
#include "JCDT_Lib/internal/ast/CompilationUnitDeclaration.h"
#include "JCDT_Lib/internal/ast/AstEmptyDeclaration.h"
#include "JCDT_Lib/internal/ast/AstClassBody.h"
#include "JCDT_Lib/internal/ast/AstTypeParameter.h"
#include "JCDT_Lib/internal/ast/AstTypeParameters.h"
#include "JCDT_Lib/internal/ast/AstClassDeclaration.h"
#include "JCDT_Lib/internal/ast/AstArrayInitializer.h"
#include "JCDT_Lib/internal/ast/AstVariableDeclaratorId.h"
#include "JCDT_Lib/internal/ast/AstVariableDeclarator.h"
#include "JCDT_Lib/internal/ast/AstFieldDeclaration.h"
#include "JCDT_Lib/internal/ast/AstFormalParameter.h"
#include "JCDT_Lib/internal/ast/AstMethodDeclarator.h"
#include "JCDT_Lib/internal/ast/AstMethodBody.h"
#include "JCDT_Lib/internal/ast/AstMethodDeclaration.h"
#include "JCDT_Lib/internal/ast/AstInitializerDeclaration.h"
#include "JCDT_Lib/internal/ast/AstArguments.h"
#include "JCDT_Lib/internal/ast/AstThisCall.h"
#include "JCDT_Lib/internal/ast/AstSuperCall.h"
#include "JCDT_Lib/internal/ast/AstConstructorDeclaration.h"
#include "JCDT_Lib/internal/ast/AstEnumDeclaration.h"
#include "JCDT_Lib/internal/ast/AstEnumConstant.h"
#include "JCDT_Lib/internal/ast/AstInterfaceDeclaration.h"
#include "JCDT_Lib/internal/ast/AstAnnotationDeclaration.h"
#include "JCDT_Lib/internal/ast/AstLocalVariableStatement.h"
#include "JCDT_Lib/internal/ast/AstLocalClassStatement.h"
#include "JCDT_Lib/internal/ast/AstIfStatement.h"
#include "JCDT_Lib/internal/ast/AstEmptyStatement.h"
#include "JCDT_Lib/internal/ast/AstExpressionStatement.h"
#include "JCDT_Lib/internal/ast/AstSwitchLabel.h"
#include "JCDT_Lib/internal/ast/AstSwitchBlockStatement.h"
#include "JCDT_Lib/internal/ast/AstSwitchStatement.h"
#include "JCDT_Lib/internal/ast/AstWhileStatement.h"
#include "JCDT_Lib/internal/ast/AstDoStatement.h"
#include "JCDT_Lib/internal/ast/AstForStatement.h"
#include "JCDT_Lib/internal/ast/AstForeachStatement.h"
#include "JCDT_Lib/internal/ast/AstBreakStatement.h"
#include "JCDT_Lib/internal/ast/AstContinueStatement.h"
#include "JCDT_Lib/internal/ast/AstReturnStatement.h"
#include "JCDT_Lib/internal/ast/AstThrowStatement.h"
#include "JCDT_Lib/internal/ast/AstSynchronizedStatement.h"
#include "JCDT_Lib/internal/ast/AstAssertStatement.h"
#include "JCDT_Lib/internal/ast/AstCatchClause.h"
#include "JCDT_Lib/internal/ast/AstFinallyClause.h"
#include "JCDT_Lib/internal/ast/AstTryStatement.h"
#include "JCDT_Lib/internal/ast/AstIntegerLiteral.h"
#include "JCDT_Lib/internal/ast/AstLongLiteral.h"
#include "JCDT_Lib/internal/ast/AstFloatLiteral.h"
#include "JCDT_Lib/internal/ast/AstDoubleLiteral.h"
#include "JCDT_Lib/internal/ast/AstTrueLiteral.h"
#include "JCDT_Lib/internal/ast/AstFalseLiteral.h"
#include "JCDT_Lib/internal/ast/AstStringLiteral.h"
#include "JCDT_Lib/internal/ast/AstCharacterLiteral.h"
#include "JCDT_Lib/internal/ast/AstNullLiteral.h"
#include "JCDT_Lib/internal/ast/AstClassLiteral.h"
#include "JCDT_Lib/internal/ast/AstThisExpression.h"
#include "JCDT_Lib/internal/ast/AstSuperExpression.h"
#include "JCDT_Lib/internal/ast/AstParenthesizedExpression.h"
#include "JCDT_Lib/internal/ast/AstClassCreationExpression.h"
#include "JCDT_Lib/internal/ast/AstDimExpr.h"
#include "JCDT_Lib/internal/ast/AstArrayCreationExpression.h"
#include "JCDT_Lib/internal/ast/AstFieldAccess.h"
#include "JCDT_Lib/internal/ast/AstMethodInvocation.h"
#include "JCDT_Lib/internal/ast/AstArrayAccess.h"
#include "JCDT_Lib/internal/ast/AstPostUnaryExpression.h"
#include "JCDT_Lib/internal/ast/AstPreUnaryExpression.h"
#include "JCDT_Lib/internal/ast/AstCastExpression.h"
#include "JCDT_Lib/internal/ast/AstBinaryExpression.h"
#include "JCDT_Lib/internal/ast/AstInstanceofExpression.h"
#include "JCDT_Lib/internal/ast/AstConditionalExpression.h"
#include "JCDT_Lib/internal/ast/AstAssignmentExpression.h"

namespace Jikes { // Open namespace Jikes block


	VariableSymbolArray* AstStoragePool::NewVariableSymbolArray(unsigned size)
	{
		return new (Alloc(sizeof(VariableSymbolArray)))
			VariableSymbolArray(this, size);
	}

	AstListNode* AstStoragePool::NewListNode()
	{
		return new (this) AstListNode();
	}

	AstBlock* AstStoragePool::NewBlock()
	{
		return new (this) AstBlock(this);
	}

	SwitchBlock* AstStoragePool::NewSwitchBlock()
	{
		return new (this) SwitchBlock(this);
	}

	AstName* AstStoragePool::NewName(Token* token)
	{
		return new (this) AstName(token);
	}

	AstPrimitiveType* AstStoragePool::NewPrimitiveType(Ast::AstKind kind, Token* t) 
	{
		return new (this) AstPrimitiveType(kind, t);
	}

	AstBrackets* AstStoragePool::NewBrackets(Token* left, Token* right) 
	{
		return new (this) AstBrackets(left, right);
	}

	AstArrayType* AstStoragePool::NewArrayType(AstType* type, AstBrackets* brackets)
	{
		return new (this) AstArrayType(type, brackets);
	}

	AstWildcard* AstStoragePool::NewWildcard(Token* question)
	{
		return new (this) AstWildcard(question);
	}

	AstTypeArguments* AstStoragePool::NewTypeArguments(Token* l, Token* r)
	{
		return new (this) AstTypeArguments(this, l, r);
	}

	AstTypeName* AstStoragePool::NewTypeName(AstName* name)
	{
		return new (this) AstTypeName(name);
	}

	AstMemberValuePair* AstStoragePool::NewMemberValuePair()
	{
		return new (this) AstMemberValuePair();
	}

	AstAnnotation* AstStoragePool::NewAnnotation()
	{
		return new (this) AstAnnotation(this);
	}

	AstModifierKeyword* AstStoragePool::NewModifierKeyword(Token* token)
	{
		return new (this) AstModifierKeyword(token);
	}

	AstModifiers* AstStoragePool::NewModifiers()
	{
		return new (this) AstModifiers(this);
	}

	AstPackageDeclaration* AstStoragePool::NewPackageDeclaration()
	{
		return new (this) AstPackageDeclaration();
	}

	AstImportDeclaration* AstStoragePool::NewImportDeclaration()
	{
		return new (this) AstImportDeclaration();
	}

	CompilationUnitDeclaration* AstStoragePool::NewCompilationUnit()
	{
		return new (this) CompilationUnitDeclaration(this);
	}

	EmptyTypeDeclaration* AstStoragePool::NewEmptyDeclaration(Token* t)
	{
		return new (this) EmptyTypeDeclaration(t);
	}

	AstClassBody* AstStoragePool::NewClassBody()
	{
		return new (this) AstClassBody(this);
	}

	AstTypeParameter* AstStoragePool::NewTypeParameter(Token* token)
	{
		return new (this) AstTypeParameter(this, token);
	}

	AstTypeParameters* AstStoragePool::NewTypeParameters()
	{
		return new (this) AstTypeParameters(this);
	}

	ClassTypeDeclaration* AstStoragePool::NewClassDeclaration()
	{
		return new (this) ClassTypeDeclaration(this);
	}

	AstArrayInitializer* AstStoragePool::NewArrayInitializer()
	{
		return new (this) AstArrayInitializer(this);
	}

	AstVariableDeclaratorId* AstStoragePool::NewVariableDeclaratorId()
	{
		return new (this) AstVariableDeclaratorId();
	}

	AstVariableDeclarator* AstStoragePool::NewVariableDeclarator()
	{
		return new (this) AstVariableDeclarator();
	}

	FieldDeclaration* AstStoragePool::NewFieldDeclaration()
	{
		return new (this) FieldDeclaration(this);
	}

	AstFormalParameter* AstStoragePool::NewFormalParameter()
	{
		return new (this) AstFormalParameter();
	}

	AstMethodDeclarator* AstStoragePool::NewMethodDeclarator()
	{
		return new (this) AstMethodDeclarator(this);
	}

	AstMethodBody* AstStoragePool::NewMethodBody()
	{
		return new (this) AstMethodBody(this);
	}

	MethodDeclaration* AstStoragePool::NewMethodDeclaration()
	{
		return new (this) MethodDeclaration(this);
	}

	Initializer* AstStoragePool::NewInitializerDeclaration()
	{
		return new (this) Initializer();
	}

	AstArguments* AstStoragePool::NewArguments(Token* left, Token* right)
	{
		return new (this) AstArguments(this, left, right);
	}

	AstThisCall* AstStoragePool::NewThisCall()
	{
		return new (this) AstThisCall();
	}

	AstSuperCall* AstStoragePool::NewSuperCall()
	{
		return new (this) AstSuperCall();
	}

	ConstructorDeclaration* AstStoragePool::NewConstructorDeclaration()
	{
		return new (this) ConstructorDeclaration(this);
	}

	EnumTypeDeclaration* AstStoragePool::NewEnumDeclaration()
	{
		return new (this) EnumTypeDeclaration(this);
	}

	AstEnumConstant* AstStoragePool::NewEnumConstant(Token* t)
	{
		return new (this) AstEnumConstant(t);
	}

	InterfaceTypeDeclaration* AstStoragePool::NewInterfaceDeclaration()
	{
		return new (this) InterfaceTypeDeclaration(this);
	}

	AnnotationTypeDeclaration* AstStoragePool::NewAnnotationDeclaration(Token* t)
	{
		return new (this) AnnotationTypeDeclaration(t);
	}

	LocalDeclaration* AstStoragePool::NewLocalVariableStatement()
	{
		return new (this) LocalDeclaration(this);
	}

	AstLocalClassStatement* AstStoragePool::NewLocalClassStatement(ClassTypeDeclaration* decl)
	{
		return new (this) AstLocalClassStatement(decl);
	}

	AstLocalClassStatement* AstStoragePool::NewLocalClassStatement(EnumTypeDeclaration* decl)
	{
		return new (this) AstLocalClassStatement(decl);
	}

	IfStatement* AstStoragePool::NewIfStatement()
	{
		return new (this) IfStatement();
	}

	EmptyStatement* AstStoragePool::NewEmptyStatement(Token* token)
	{
		return new (this) EmptyStatement(token);
	}

	AstExpressionStatement* AstStoragePool::NewExpressionStatement()
	{
		return new (this) AstExpressionStatement();
	}

	AstSwitchLabel* AstStoragePool::NewSwitchLabel()
	{
		return new (this) AstSwitchLabel();
	}

	AstSwitchBlockStatement* AstStoragePool::NewSwitchBlockStatement()
	{
		return new (this) AstSwitchBlockStatement(this);
	}

	SwitchStatement* AstStoragePool::NewSwitchStatement()
	{
		return new (this) SwitchStatement(this);
	}

	WhileStatement* AstStoragePool::NewWhileStatement()
	{
		return new (this) WhileStatement();
	}

	DoStatement* AstStoragePool::NewDoStatement()
	{
		return new (this) DoStatement();
	}

	ForStatement* AstStoragePool::NewForStatement()
	{
		return new (this) ForStatement(this);
	}

	ForeachStatement* AstStoragePool::NewForeachStatement()
	{
		return new (this) ForeachStatement();
	}

	BreakStatement* AstStoragePool::NewBreakStatement()
	{
		return new (this) BreakStatement();
	}

	ContinueStatement* AstStoragePool::NewContinueStatement()
	{
		return new (this) ContinueStatement();
	}

	ReturnStatement* AstStoragePool::NewReturnStatement()
	{
		return new (this) ReturnStatement();
	}

	ThrowStatement* AstStoragePool::NewThrowStatement()
	{
		return new (this) ThrowStatement();
	}

	SynchronizedStatement* AstStoragePool::NewSynchronizedStatement()
	{
		return new (this) SynchronizedStatement();
	}

	AssertStatement* AstStoragePool::NewAssertStatement()
	{
		return new (this) AssertStatement();
	}

	AstCatchClause* AstStoragePool::NewCatchClause()
	{
		return new (this) AstCatchClause();
	}

	AstFinallyClause* AstStoragePool::NewFinallyClause()
	{
		return new (this) AstFinallyClause();
	}

	TryStatement* AstStoragePool::NewTryStatement()
	{
		return new (this) TryStatement(this);
	}

	IntegerLiteral* AstStoragePool::NewIntegerLiteral(Token* token)
	{
		return new (this) IntegerLiteral(token);
	}

	LongLiteral* AstStoragePool::NewLongLiteral(Token* token)
	{
		return new (this) LongLiteral(token);
	}

	FloatLiteral* AstStoragePool::NewFloatLiteral(Token* token)
	{
		return new (this) FloatLiteral(token);
	}

	DoubleLiteral* AstStoragePool::NewDoubleLiteral(Token* token)
	{
		return new (this) DoubleLiteral(token);
	}

	TrueLiteral* AstStoragePool::NewTrueLiteral(Token* token)
	{
		return new (this) TrueLiteral(token);
	}

	FalseLiteral* AstStoragePool::NewFalseLiteral(Token* token)
	{
		return new (this) FalseLiteral(token);
	}

	StringLiteral* AstStoragePool::NewStringLiteral(Token* token)
	{
		return new (this) StringLiteral(token);
	}

	CharLiteral* AstStoragePool::NewCharacterLiteral(Token* token)
	{
		return new (this) CharLiteral(token);
	}

	NullLiteral* AstStoragePool::NewNullLiteral(Token* token)
	{
		return new (this) NullLiteral(token);
	}

	ClassLiteralAccess* AstStoragePool::NewClassLiteral(Token* token)
	{
		return new (this) ClassLiteralAccess(token);
	}

	AstThisExpression* AstStoragePool::NewThisExpression(Token* token)
	{
		return new (this) AstThisExpression(token);
	}

	AstSuperExpression* AstStoragePool::NewSuperExpression(Token* token)
	{
		return new (this) AstSuperExpression(token);
	}

	AstParenthesizedExpression* AstStoragePool::NewParenthesizedExpression()
	{
		return new (this) AstParenthesizedExpression();
	}

	AstClassCreationExpression* AstStoragePool::NewClassCreationExpression()
	{
		return new (this) AstClassCreationExpression();
	}

	AstDimExpr* AstStoragePool::NewDimExpr()
	{
		return new (this) AstDimExpr();
	}

	AstArrayCreationExpression* AstStoragePool::NewArrayCreationExpression()
	{
		return new (this) AstArrayCreationExpression(this);
	}

	FieldReference* AstStoragePool::NewFieldAccess()
	{
		return new (this) FieldReference();
	}

	AstMethodInvocation* AstStoragePool::NewMethodInvocation(Token* t)
	{
		return new (this) AstMethodInvocation(t);
	}

	ArrayReference* AstStoragePool::NewArrayAccess()
	{
		return new (this) ArrayReference();
	}

	AstPostUnaryExpression* AstStoragePool::NewPostUnaryExpression(PostUnaryExpressionEnum::PostUnaryExpressionTag tag)
	{
		return new (this) AstPostUnaryExpression(tag);
	}

	AstPreUnaryExpression* AstStoragePool::NewPreUnaryExpression(PreUnaryExpressionEnum::PreUnaryExpressionTag tag)
	{
		return new (this) AstPreUnaryExpression(tag);
	}

	CastExpression* AstStoragePool::NewCastExpression()
	{
		return new (this) CastExpression();
	}

	BinaryExpression* AstStoragePool::NewBinaryExpression(BinaryExpressionEnum::BinaryExpressionTag tag)
	{
		return new (this) BinaryExpression(tag);
	}

	InstanceofExpression* AstStoragePool::NewInstanceofExpression()
	{
		return new (this) InstanceofExpression();
	}

	ConditionalExpression* AstStoragePool::NewConditionalExpression()
	{
		return new (this) ConditionalExpression();
	}

	AstAssignmentExpression* AstStoragePool::NewAssignmentExpression(AssignmentExpressionEnum::AssignmentExpressionTag tag, Token* token)
	{
		return new (this) AstAssignmentExpression(tag, token);
	}

	CaseElement* AstStoragePool::GenCaseElement(unsigned block_index, unsigned case_index)
	{
		CaseElement* p = new (Alloc(sizeof(CaseElement))) CaseElement();
		p->block_index = block_index;
		p->case_index = case_index;
		return p;
	}

	AstBlock* AstStoragePool::GenBlock()
	{
		AstBlock* p = NewBlock();
		p->generated = true;
		p->no_braces = true;
		return p;
	}

	AstName* AstStoragePool::GenName(Token* token)
	{
		AstName* p = NewName(token);
		p->generated = true;
		return p;
	}

	AstPrimitiveType* AstStoragePool::GenPrimitiveType(Ast::AstKind kind, Token* t)
	{
		AstPrimitiveType* p = NewPrimitiveType(kind, t);
		p->generated = true;
		return p;
	}

	AstBrackets* AstStoragePool::GenBrackets(Token* left, Token* right)
	{
		AstBrackets* p = NewBrackets(left, right);
		p->generated = true;
		return p;
	}

	AstArrayType* AstStoragePool::GenArrayType(AstType* type, AstBrackets* brackets)
	{
		AstArrayType* p = NewArrayType(type, brackets);
		p->generated = true;
		return p;
	}

	AstWildcard* AstStoragePool::GenWildcard(Token* question)
	{
		AstWildcard* p = NewWildcard(question);
		p->generated = true;
		return p;
	}

	AstTypeArguments* AstStoragePool::GenTypeArguments(Token* l, Token* r)
	{
		AstTypeArguments* p = NewTypeArguments(l, r);
		p->generated = true;
		return p;
	}

	AstTypeName* AstStoragePool::GenTypeName(AstName* type)
	{
		AstTypeName* p = NewTypeName(type);
		p->generated = true;
		return p;
	}

	AstMemberValuePair* AstStoragePool::GenMemberValuePair()
	{
		AstMemberValuePair* p = NewMemberValuePair();
		p->generated = true;
		return p;
	}

	AstAnnotation* AstStoragePool::GenAnnotation()
	{
		AstAnnotation* p = NewAnnotation();
		p->generated = true;
		return p;
	}

	AstModifierKeyword* AstStoragePool::GenModifierKeyword(Token* token)
	{
		AstModifierKeyword* p = NewModifierKeyword(token);
		p->generated = true;
		return p;
	}

	AstModifiers* AstStoragePool::GenModifiers()
	{
		AstModifiers* p = NewModifiers();
		p->generated = true;
		return p;
	}

	AstPackageDeclaration* AstStoragePool::GenPackageDeclaration()
	{
		AstPackageDeclaration* p = NewPackageDeclaration();
		p->generated = true;
		return p;
	}

	AstImportDeclaration* AstStoragePool::GenImportDeclaration()
	{
		AstImportDeclaration* p = NewImportDeclaration();
		p->generated = true;
		return p;
	}

	CompilationUnitDeclaration* AstStoragePool::GenCompilationUnit()
	{
		CompilationUnitDeclaration* p = NewCompilationUnit();
		p->generated = true;
		return p;
	}
	AstAnonymousTypeDeclaration* AstStoragePool::GenAnonymousTypeDeclaration()
	{
		auto p =  new (this) AstAnonymousTypeDeclaration();
		p->generated = true;
		return p;
	}
	EmptyTypeDeclaration* AstStoragePool::GenEmptyDeclaration(Token* t)
	{
		EmptyTypeDeclaration* p = NewEmptyDeclaration(t);
		p->generated = true;
		return p;
	}

	AstClassBody* AstStoragePool::GenClassBody()
	{
		AstClassBody* p = NewClassBody();
		p->generated = true;
		return p;
	}

	AstTypeParameter* AstStoragePool::GenTypeParameter(Token* token)
	{
		AstTypeParameter* p = NewTypeParameter(token);
		p->generated = true;
		return p;
	}

	AstTypeParameters* AstStoragePool::GenTypeParameters()
	{
		AstTypeParameters* p = NewTypeParameters();
		p->generated = true;
		return p;
	}

	ClassTypeDeclaration* AstStoragePool::GenClassDeclaration()
	{
		ClassTypeDeclaration* p = NewClassDeclaration();
		p->generated = true;
		return p;
	}

	AstArrayInitializer* AstStoragePool::GenArrayInitializer()
	{
		AstArrayInitializer* p = NewArrayInitializer();
		p->generated = true;
		return p;
	}

	AstVariableDeclaratorId* AstStoragePool::GenVariableDeclaratorId()
	{
		AstVariableDeclaratorId* p = NewVariableDeclaratorId();
		p->generated = true;
		return p;
	}

	AstVariableDeclarator* AstStoragePool::GenVariableDeclarator()
	{
		AstVariableDeclarator* p = NewVariableDeclarator();
		p->generated = true;
		return p;
	}

	FieldDeclaration* AstStoragePool::GenFieldDeclaration()
	{
		FieldDeclaration* p = NewFieldDeclaration();
		p->generated = true;
		return p;
	}

	AstFormalParameter* AstStoragePool::GenFormalParameter()
	{
		AstFormalParameter* p = NewFormalParameter();
		p->generated = true;
		return p;
	}

	AstMethodDeclarator* AstStoragePool::GenMethodDeclarator()
	{
		AstMethodDeclarator* p = NewMethodDeclarator();
		p->generated = true;
		return p;
	}

	AstMethodBody* AstStoragePool::GenMethodBody()
	{
		AstMethodBody* p = NewMethodBody();
		p->generated = true;
		return p;
	}

	MethodDeclaration* AstStoragePool::GenMethodDeclaration()
	{
		MethodDeclaration* p = NewMethodDeclaration();
		p->generated = true;
		return p;
	}

	Initializer* AstStoragePool::GenInitializerDeclaration()
	{
		Initializer* p = NewInitializerDeclaration();
		p->generated = true;
		return p;
	}

	AstArguments* AstStoragePool::GenArguments(Token* left, Token* right)
	{
		AstArguments* p = NewArguments(left, right);
		p->generated = true;
		return p;
	}

	AstThisCall* AstStoragePool::GenThisCall()
	{
		AstThisCall* p = NewThisCall();
		p->generated = true;
		return p;
	}

	AstSuperCall* AstStoragePool::GenSuperCall()
	{
		AstSuperCall* p = NewSuperCall();
		p->generated = true;
		return p;
	}

	ConstructorDeclaration* AstStoragePool::GenConstructorDeclaration()
	{
		ConstructorDeclaration* p = NewConstructorDeclaration();
		p->generated = true;
		return p;
	}

	EnumTypeDeclaration* AstStoragePool::GenEnumDeclaration()
	{
		EnumTypeDeclaration* p = NewEnumDeclaration();
		p->generated = true;
		return p;
	}

	AstEnumConstant* AstStoragePool::GenEnumConstant(Token* t)
	{
		AstEnumConstant* p = NewEnumConstant(t);
		p->generated = true;
		return p;
	}

	InterfaceTypeDeclaration* AstStoragePool::GenInterfaceDeclaration()
	{
		InterfaceTypeDeclaration* p = NewInterfaceDeclaration();
		p->generated = true;
		return p;
	}

	AnnotationTypeDeclaration* AstStoragePool::GenAnnotationDeclaration(Token* t)
	{
		AnnotationTypeDeclaration* p = NewAnnotationDeclaration(t);
		p->generated = true;
		return p;
	}

	LocalDeclaration* AstStoragePool::GenLocalVariableStatement()
	{
		LocalDeclaration* p = NewLocalVariableStatement();
		p->generated = true;
		return p;
	}

	AstLocalClassStatement* AstStoragePool::GenLocalClassStatement(ClassTypeDeclaration* decl)
	{
		AstLocalClassStatement* p = NewLocalClassStatement(decl);
		p->generated = true;
		return p;
	}

	AstLocalClassStatement* AstStoragePool::GenLocalClassStatement(EnumTypeDeclaration* decl)
	{
		AstLocalClassStatement* p = NewLocalClassStatement(decl);
		p->generated = true;
		return p;
	}

	IfStatement* AstStoragePool::GenIfStatement()
	{
		IfStatement* p = NewIfStatement();
		p->generated = true;
		return p;
	}

	EmptyStatement* AstStoragePool::GenEmptyStatement(Token* token)
	{
		EmptyStatement* p = NewEmptyStatement(token);
		p->generated = true;
		return p;
	}

	AstExpressionStatement* AstStoragePool::GenExpressionStatement()
	{
		AstExpressionStatement* p = NewExpressionStatement();
		p->generated = true;
		return p;
	}

	AstSwitchLabel* AstStoragePool::GenSwitchLabel()
	{
		AstSwitchLabel* p = NewSwitchLabel();
		p->generated = true;
		return p;
	}

	AstSwitchBlockStatement* AstStoragePool::GenSwitchBlockStatement()
	{
		AstSwitchBlockStatement* p = NewSwitchBlockStatement();
		p->generated = true;
		return p;
	}

	SwitchStatement* AstStoragePool::GenSwitchStatement()
	{
		SwitchStatement* p = NewSwitchStatement();
		p->generated = true;
		return p;
	}

	WhileStatement* AstStoragePool::GenWhileStatement()
	{
		WhileStatement* p = NewWhileStatement();
		p->generated = true;
		return p;
	}

	DoStatement* AstStoragePool::GenDoStatement()
	{
		DoStatement* p = NewDoStatement();
		p->generated = true;
		return p;
	}

	ForStatement* AstStoragePool::GenForStatement()
	{
		ForStatement* p = NewForStatement();
		p->generated = true;
		return p;
	}

	ForeachStatement* AstStoragePool::GenForeachStatement()
	{
		ForeachStatement* p = NewForeachStatement();
		p->generated = true;
		return p;
	}

	BreakStatement* AstStoragePool::GenBreakStatement()
	{
		BreakStatement* p = NewBreakStatement();
		p->generated = true;
		return p;
	}

	ContinueStatement* AstStoragePool::GenContinueStatement()
	{
		ContinueStatement* p = NewContinueStatement();
		p->generated = true;
		return p;
	}

	ReturnStatement* AstStoragePool::GenReturnStatement()
	{
		ReturnStatement* p = NewReturnStatement();
		p->generated = true;
		return p;
	}

	ThrowStatement* AstStoragePool::GenThrowStatement()
	{
		ThrowStatement* p = NewThrowStatement();
		p->generated = true;
		return p;
	}

	SynchronizedStatement* AstStoragePool::GenSynchronizedStatement()
	{
		SynchronizedStatement* p = NewSynchronizedStatement();
		p->generated = true;
		return p;
	}

	AssertStatement* AstStoragePool::GenAssertStatement()
	{
		AssertStatement* p = NewAssertStatement();
		p->generated = true;
		return p;
	}

	AstCatchClause* AstStoragePool::GenCatchClause()
	{
		AstCatchClause* p = NewCatchClause();
		p->generated = true;
		return p;
	}

	AstFinallyClause* AstStoragePool::GenFinallyClause()
	{
		AstFinallyClause* p = NewFinallyClause();
		p->generated = true;
		return p;
	}

	TryStatement* AstStoragePool::GenTryStatement()
	{
		TryStatement* p = NewTryStatement();
		p->generated = true;
		return p;
	}

	IntegerLiteral* AstStoragePool::GenIntegerLiteral(Token* token)
	{
		IntegerLiteral* p = NewIntegerLiteral(token);
		p->generated = true;
		return p;
	}

	LongLiteral* AstStoragePool::GenLongLiteral(Token* token)
	{
		LongLiteral* p = NewLongLiteral(token);
		p->generated = true;
		return p;
	}

	FloatLiteral* AstStoragePool::GenFloatLiteral(Token* token)
	{
		FloatLiteral* p = NewFloatLiteral(token);
		p->generated = true;
		return p;
	}

	DoubleLiteral* AstStoragePool::GenDoubleLiteral(Token* token)
	{
		DoubleLiteral* p = NewDoubleLiteral(token);
		p->generated = true;
		return p;
	}

	TrueLiteral* AstStoragePool::GenTrueLiteral(Token* token)
	{
		TrueLiteral* p = NewTrueLiteral(token);
		p->generated = true;
		return p;
	}

	FalseLiteral* AstStoragePool::GenFalseLiteral(Token* token)
	{
		FalseLiteral* p = NewFalseLiteral(token);
		p->generated = true;
		return p;
	}

	StringLiteral* AstStoragePool::GenStringLiteral(Token* token)
	{
		StringLiteral* p = NewStringLiteral(token);
		p->generated = true;
		return p;
	}

	CharLiteral* AstStoragePool::GenCharacterLiteral(Token* token)
	{
		CharLiteral* p = NewCharacterLiteral(token);
		p->generated = true;
		return p;
	}

	NullLiteral* AstStoragePool::GenNullLiteral(Token* token)
	{
		NullLiteral* p = NewNullLiteral(token);
		p->generated = true;
		return p;
	}

	ClassLiteralAccess* AstStoragePool::GenClassLiteral(Token* token)
	{
		ClassLiteralAccess* p = NewClassLiteral(token);
		p->generated = true;
		return p;
	}

	AstThisExpression* AstStoragePool::GenThisExpression(Token* token)
	{
		AstThisExpression* p = NewThisExpression(token);
		p->generated = true;
		return p;
	}

	AstSuperExpression* AstStoragePool::GenSuperExpression(Token* token)
	{
		AstSuperExpression* p = NewSuperExpression(token);
		p->generated = true;
		return p;
	}

	AstParenthesizedExpression* AstStoragePool::GenParenthesizedExpression()
	{
		AstParenthesizedExpression* p = NewParenthesizedExpression();
		p->generated = true;
		return p;
	}

	AstClassCreationExpression* AstStoragePool::GenClassCreationExpression()
	{
		AstClassCreationExpression* p = NewClassCreationExpression();
		p->generated = true;
		return p;
	}

	AstDimExpr* AstStoragePool::GenDimExpr()
	{
		AstDimExpr* p = NewDimExpr();
		p->generated = true;
		return p;
	}

	AstArrayCreationExpression* AstStoragePool::GenArrayCreationExpression()
	{
		AstArrayCreationExpression* p = NewArrayCreationExpression();
		p->generated = true;
		return p;
	}

	FieldReference* AstStoragePool::GenFieldAccess()
	{
		FieldReference* p = NewFieldAccess();
		p->generated = true;
		return p;
	}

	AstMethodInvocation* AstStoragePool::GenMethodInvocation(Token* t)
	{
		AstMethodInvocation* p = NewMethodInvocation(t);
		p->generated = true;
		return p;
	}

	ArrayReference* AstStoragePool::GenArrayAccess()
	{
		ArrayReference* p = NewArrayAccess();
		p->generated = true;
		return p;
	}

	AstPostUnaryExpression* AstStoragePool::GenPostUnaryExpression(PostUnaryExpressionEnum::PostUnaryExpressionTag tag)
	{
		AstPostUnaryExpression* p = NewPostUnaryExpression(tag);
		p->generated = true;
		return p;
	}

	AstPreUnaryExpression* AstStoragePool::GenPreUnaryExpression(PreUnaryExpressionEnum::PreUnaryExpressionTag tag)
	{
		AstPreUnaryExpression* p = NewPreUnaryExpression(tag);
		p->generated = true;
		return p;
	}

	CastExpression* AstStoragePool::GenCastExpression()
	{
		CastExpression* p = NewCastExpression();
		p->generated = true;
		return p;
	}

	BinaryExpression* AstStoragePool::GenBinaryExpression(BinaryExpressionEnum::BinaryExpressionTag tag)
	{
		BinaryExpression* p = NewBinaryExpression(tag);
		p->generated = true;
		return p;
	}

	InstanceofExpression* AstStoragePool::GenInstanceofExpression()
	{
		InstanceofExpression* p = NewInstanceofExpression();
		p->generated = true;
		return p;
	}

	ConditionalExpression* AstStoragePool::GenConditionalExpression()
	{
		ConditionalExpression* p = NewConditionalExpression();
		p->generated = true;
		return p;
	}

	AstAssignmentExpression* AstStoragePool::GenAssignmentExpression(AssignmentExpressionEnum::AssignmentExpressionTag tag, Token* token)
	{
		AstAssignmentExpression* p = NewAssignmentExpression(tag, token);
		p->generated = true;
		return p;
	}
} // Close namespace Jikes block


#endif // StoragePool_def_INCLUDED
