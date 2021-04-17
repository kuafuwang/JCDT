// $Id: JCDT_Lib/internal/ast/ast.h,v 1.74 2004/03/25 13:32:26 ericb Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 2004 IBM Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#ifndef ast_INCLUDED
#define ast_INCLUDED

#include "JCDT_Lib/internal/env/platform.h"
#include "JCDT_Lib/internal/util/depend.h"
#include "JCDT_Lib/internal/lex/Token.h"

#include "JCDT_Lib/internal/lookup/Symbol.h"

#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/CaseElement.h"
#include "JCDT_Lib/internal/lookup/VariableSymbolArray.h"
#include "JCDT_Lib/internal/ast/ASTNode.h"

#include "JCDT_Lib/internal/ast/AstArray.h"

#include "JCDT_Lib/internal/ast/AstListNode.h"
#include "JCDT_Lib/internal/ast/AbstractStatement.h"
#include "JCDT_Lib/internal/ast/AstDeclared.h"
#include "JCDT_Lib/internal/ast/AstDeclaredType.h"

#include "JCDT_Lib/internal/ast/ReachComleteAbleStatement.h"
#include "JCDT_Lib/internal/ast/AstMemberValue.h"
#include "JCDT_Lib/internal/ast/AstExpression.h"
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
#include "JCDT_Lib/internal/ast/ImportReference.h"
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

#include "JCDT_Lib/internal/ast/AbstractVariableDeclaration.h"
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


	

//***************************************************************************
//
// TODO: This documentation is a bit out of date...
//
// This file contains the definitions of the classes used to construct the
// AST representation of a Java program.
//
// The node Ast is a base class of all other classes. (The name of the other
// classes start with the prefix "Ast".) The nodes associated with executable
// statements (e.g., IfStatement) are subclasses of Statement and nodes
// associated with expressions (e.g., BinaryExpression) are subclasses of
// Expression.
//
// The information contained in the AST nodes is described by a grammar where
// each rule consists of a left-hand side nonterminal followed by "-->"
// followed by a right-hand side symbol or a sequence enclosed in the pair of
// symbols "<" and ">". In defining the symbols, the following notation is
// used:
//
// Symbols that are capitalized (e.g., Type) are nonterminals. Symbols that are
// in all upper case (e.g., PACKAGE) represent node kinds. Symbols that contain
// the substring "_token" represents tokens in the source file. The suffix
// "_opt" indicates that a symbol is optional. For example, if Super_opt
// appears in a rule, it indicates that either Super or null can be expected.
// When a symbol is plural (e.g., Modifiers), it indicates zero or more
// instances of such a symbol (a list to be precise) can be expected. Thus,
// when "Modifiers" is specified in the right-hand side of a rule either no
// Modifier or a sequence of them may appear.
//
// Implementation Notes:
//
//    A complete AST tree for a Java program always contains an
//    CompilationUnitDeclaration root node. The kind of that node is
//    Ast::EMPTY_COMPILATION for a tree with no type declaration,
//    Ast::COMPILATION for a tree constructed from an otherwise valid program
//    and Ast::BAD_COMPILATION for a tree constructed from an invalid program.
//
//    All AST tree nodes belong to a AstStoragePool. When a new node must be
//    added, it is allocated from the same AstStoragePool. Thus, you should
//    never use operator new to construct an AST object, but instead use New*
//    or Gen* defined in AstStoragePool. Likewise, AST tree nodes never need
//    destruction - simply delete the pool to reclaim the entire tree.
//
//    When the preprocessor variable JIKES_DEBUG is defined the user may print
//    out an AST tree to standard output by calling the virtual function
//    "Print" for the root node of the tree.
//
//    DynamicArrays are used to implement lists. This representation has the
//    advantage of being very flexible and easy to use. However, it may be
//    slightly less time-efficient than a straightforward linked list. My
//    guess is no more than 10% which justifies this use, but that should be
//    checked at some point...
//
//***************************************************************************
//

//***********************************
	/*  FieldDeclaration* Ast::StaticFieldCast()
	{
		return DYNAMIC_CAST<FieldDeclaration*>
			(kind == FIELD &&
				other_tag == FieldDeclaration::STATIC ? this : NULL);
	}
*/
	/*  Initializer* Ast::StaticInitializerCast()
	{
		return DYNAMIC_CAST<Initializer*>
			(kind == INITIALIZER &&
				other_tag == Initializer::STATIC ? this : NULL);
	}*/

	//  AstClassBody* Ast::UnparsedClassBodyCast()
	//{
	//	return DYNAMIC_CAST<AstClassBody*>
	//		(kind == CLASS_BODY &&
	//			other_tag == AstClassBody::UNPARSED ? this : NULL);
	//}

	//  CompilationUnitDeclaration* Ast::BadCompilationUnitCast()
	//{
	//	return DYNAMIC_CAST<CompilationUnitDeclaration*>
	//		(
	//			(kind == COMPILATION &&
	//			other_tag == CompilationUnitDeclaration::BAD_COMPILATION) ? this : NULL);
	//}

	//  CompilationUnitDeclaration* Ast::EmptyCompilationUnitCast()
	//{
	//	return DYNAMIC_CAST<CompilationUnitDeclaration*>
	//		(kind == COMPILATION &&
	//			other_tag == CompilationUnitDeclaration::EMPTY_COMPILATION ? this : NULL);
	//}

// **********************************************




// ******************************************




} // Close namespace Jikes block


#endif // ast_INCLUDED
