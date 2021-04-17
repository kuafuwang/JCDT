%options ACTION, AN=javaact.cpp, HN=consumeRule.cpp, em, TABLE, GP=c++,
%options FILE-PREFIX=java, ESCAPE=$, PREFIX=TK_,em, OUTPUT-SIZE=125
%options HBLOCKB=\:,HBLOCKE=:\,NOGOTO-DEFAULT,SINGLE-PRODUCTIONS
%options LALR=1,jikes


--error recovering options.....
%options ERROR_MAPS 

--grammar understanding options
%options first follow
%options TRACE=FULL ,
%options VERBOSE

%options DEFERRED
%options NAMES=MAX
%options SCOPES


-- $Id: java.g,v 1.48 2004/03/25 13:32:27 ericb Exp $
-- This software is subject to the terms of the IBM Jikes Compiler
-- License Agreement available at the following URL:
-- http://ibm.com/developerworks/opensource/jikes.
-- Copyright (C) 1996, 2004 IBM Corporation and others.  All Rights Reserved.
-- You must accept the terms of that agreement to use this software.

------------------------------------------------------------------------
--
--                               J A V A
--
-- This Java grammar started from the grammar defined in chapter 19 of the
-- first edition of the Java Language Specification manual.  It has since been
-- updated with several additional rules to cover additional language features,
-- as well as simplified in places where it makes sense to share code. Comments
-- are given where this grammar differs from the original. Note that the
-- second edition JLS grammar (chapter 18) is completely broken.  This grammar
-- is in JIKES PG format with semantic actions following each rule. In
-- specifying the rules, the symbols are enclosed in single quotes, and the
-- keywords are all caps, so that they can be quickly distinguished from
-- non-terminals. Optional symbols are suffixed with "opt" and the rules
-- expanding such definitions can be found at the end. Also, some syntactic
-- markers have been added to aid the parser in skipping irrelevant
-- components during different parse phases.
--
-- The file consumeRule.cpp produced by JIKESPG from this file (jcdt.g) contains a
-- very readable definition of the grammar rules together with their
-- associated semantic actions. That file is marked with appropriate
-- location directives (automatically generated) which instructs the C++
-- compiler to issue error messages in terms of this file (jcdt.g).
-- Therefore, though the user is encouraged to read javaact.h, no
-- change should ever be made to that file. Instead, changes should
-- always be made in this file and javaact.h should be regenerated
-- using JIKESPG.
--
------------------------------------------------------------------------

$Define

--
-- This macro generates a header for an action function consisting
-- of the rule in question (commented) and a location directive.
--
$location
/.

//
// Rule $rule_number:  $rule_text
//
//#line $next_line "$input_file"./





$readableName 
/.1#$rule_number#./
$compliance
/.2#$rule_number#./
$recovery
/.2#$rule_number# recovery./

$recovery_template
/. 
auto r_$rule_number =Parser::tokens_for_recovery_templates.insert(
 Parser::tokens_for_recovery_templates.end(),make_pair($rule_number, ./

$no_statements_recovery
/.
auto no_r_$rule_number = Parser::tokens_for_statements_recovery_filter.insert(
Parser::tokens_for_statements_recovery_filter.end(), $rule_number);
./



--
-- These macros are used to initialize the rule_action array
-- to a specific named function.
--
$pre_action
/.  $location
    rule_action[$rule_number] = ./




--
-- This macro generates a header for a named action function that is
-- already defined and will be shared.
--
$shared_function
/.

//
// Rule $rule_number:  $rule_text./

--
-- This macro generates a header for a rule that invokes the
-- no_function routine.
--
$shared_NoAction
/.

//
// Rule $rule_number:  $rule_text
//
// void NoAction();
//./

--
-- This macro generates a header for a rule that invokes the
-- null_function routine.
--
$shared_NullAction
/.

//
// Rule $rule_number:  $rule_text
//
// void NullAction();
//./

--
-- This macro generates a header for a rule that invokes the
-- StartList routine.
--
$shared_StartList
/.

//
// Rule $rule_number:  $rule_text
//
// void StartList();
//./

--
-- This macro generates a header for a rule that invokes the
-- AddList2 routine.
--
$shared_AddList2
/.

//
// Rule $rule_number:  $rule_text
//
// void AddList2();
//./

--
-- This macro generates a header for a rule that invokes the
-- AddList3 routine.
--
$shared_AddList3
/.

//
// Rule $rule_number:  $rule_text
//
// void AddList3();
//./

--
-- This macro generates a header for MakePreUnaryExpression.
--
$shared_Unary
/.

//
// Rule $rule_number:  $rule_text
//
// void MakePreUnaryExpression();
//./
--
-- This macro generates a header for MakeBinaryExpression.
--
$shared_Binary
/.

//
// Rule $rule_number:  $rule_text
//
// void MakeBinaryExpression();
//./




$consumeIntegerLiteral
/.
$pre_action &Parser::consumeIntegerLiteral;
./

$consumeLongLiteral
/.
$pre_action &Parser::consumeLongLiteral;
./

$consumeFloatLiteral
/.
$pre_action &Parser::consumeFloatLiteral;
./

$consumeDoubleLiteral
/.
$pre_action &Parser::consumeDoubleLiteral;
./

$consumeCharacterLiteral
/.
$pre_action &Parser::consumeCharacterLiteral;
./

$consumeStringLiteral
/.
$pre_action &Parser::consumeStringLiteral;
./

$consumeNullLiteral
/.
$pre_action &Parser::consumeNullLiteral;
./

$consumeBooleanTrueLiteral
/.
$pre_action &Parser::consumeBooleanTrueLiteral;
./

$consumeBooleanFalseLiteral
/.
$pre_action &Parser::consumeBooleanFalseLiteral;
./

$consumeBooleanLiteral
/.
$pre_action &Parser::consumeBooleanLiteral;
./

$consumeToken_byte
/.
$pre_action &Parser::consumeToken_byte;
./

$consumeToken_short
/.
$pre_action &Parser::consumeToken_short;
./

$consumeToken_int
/.
$pre_action &Parser::consumeToken_int;
./

$consumeToken_long
/.
$pre_action &Parser::consumeToken_long;
./

$consumeToken_char
/.
$pre_action &Parser::consumeToken_char;
./


$consumeToken_float
/.
$pre_action &Parser::consumeToken_float;
./


$consumeToken_double
/.
$pre_action &Parser::consumeToken_double;
./


$consumeToken_void
/.
$pre_action &Parser::consumeToken_void;
./

$consumeEnumConstantWithClassBodyOpt
/.
$pre_action &Parser::consumeEnumConstantWithClassBodyOpt;
./


$consumeArguments
/.
$pre_action &Parser::consumeArguments;
./

$consumeConstantDeclaration
/.
$pre_action &Parser::consumeConstantDeclaration;
./

$consumeBlock
/.
$pre_action &Parser::consumeBlock;
./

$consumeInvalidClassDeclaration
/.
$pre_action &Parser::consumeInvalidClassDeclaration;
./

$consumeInvalidEnumDeclaration
/.
$pre_action &Parser::consumeInvalidEnumDeclaration;
./

$consumeLocalVariableDeclarationStatement
/.
$pre_action &Parser::consumeLocalVariableDeclarationStatement;
./

$consumeEmptyStatement
/.
$pre_action &Parser::consumeEmptyStatement;
./

$consumeExpressionStatement
/.
$pre_action &Parser::consumeExpressionStatement;
./

$consumeStatementSwitch
/.
$pre_action &Parser::consumeStatementSwitch;
./

$consumeSwitchBlockWithSwitchBlockStatements
/.
$pre_action &Parser::consumeSwitchBlockWithSwitchBlockStatements;
./

$consumeSwitchBlockOnlyWithSwitchLabelsopt
/.
$pre_action &Parser::consumeSwitchBlockOnlyWithSwitchLabelsopt;
./

$consumeSwitchBlockStatement
/.
$pre_action &Parser::consumeSwitchBlockStatement;
./

$consumeStatementDo
/.
$pre_action &Parser::consumeStatementDo;
./

$consumeStatementBreak
/.
$pre_action &Parser::consumeStatementBreak;
./

$consumeStatementContinue
/.
$pre_action &Parser::consumeStatementContinue;
./

$consumeStatementReturn
/.
$pre_action &Parser::consumeStatementReturn;
./

$consumeStatementThrow
/.
$pre_action &Parser::consumeStatementThrow;
./

$consumeStatementSynchronized
/.
$pre_action &Parser::consumeStatementSynchronized;
./

$consumeStatementCatch
/.
$pre_action &Parser::consumeStatementCatch;
./

$consumeStatementFinally
/.
$pre_action &Parser::consumeStatementFinally;
./

$consumePrimaryNoNewArrayThis
/.
$pre_action &Parser::consumePrimaryNoNewArrayThis;
./

$consumePrimaryNoNewArrayNameThis
/.
$pre_action &Parser::consumePrimaryNoNewArrayNameThis;
./

$consumeClassInstanceCreationExpression
/.
$pre_action &Parser::consumeClassInstanceCreationExpression;
./

$consumeClassInstanceCreationExpressionWithTypeArguments
/.
$pre_action &Parser::consumeClassInstanceCreationExpressionWithTypeArguments;
./

$consumeDimWithExpr
/.
$pre_action &Parser::consumeDimWithExpr;
./

$consumeDimWithOutExpr
/.
$pre_action &Parser::consumeDimWithOutExpr;
./

$consumeDimensions
/.
$pre_action &Parser::consumeDimensions;
./

$consumeToken_super
/.
$pre_action &Parser::consumeToken_super;
./

$consumeQuilifiedSuperAccess
/.
$pre_action &Parser::consumeQuilifiedSuperAccess;
./

$consumeMethodInvocationPrimary
/.
$pre_action &Parser::consumeMethodInvocationPrimary;
./

$consumeUnaryExpressionPLUSPLUS
/.
$pre_action &Parser::consumeUnaryExpressionPLUSPLUS;
./

$consumeUnaryExpressionMINUSMINUS
/.
$pre_action &Parser::consumeUnaryExpressionMINUSMINUS;
./

$consumeCastExpressionTypeArgumentList1Dimsopt
/.
$pre_action &Parser::consumeCastExpressionTypeArgumentList1Dimsopt;
./

$consumeCastExpressionTypeArgumentList1DimsoptQualified
/.
$pre_action &Parser::consumeCastExpressionTypeArgumentList1DimsoptQualified;
./

$consumeAssignment
/.
$pre_action &Parser::consumeAssignment;
./

$consumePackageHeaderMarker
/.
$pre_action &Parser::consumePackageHeaderMarker;
./

$consumeMethodHeaderMarker
/.
$pre_action &Parser::consumeMethodHeaderMarker;
./

$consumeClassOrInterfaceName
/.  
$pre_action &Parser::consumeClassOrInterfaceName;
./

$consumeClassOrInterfaceTypeArgumentList1Name
/.
$pre_action &Parser::consumeClassOrInterfaceTypeArgumentList1Name;
./

$consumeClassOrInterfaceTypeArgumentList1NameWithDims
/.
$pre_action &Parser::consumeClassOrInterfaceTypeArgumentList1NameWithDims;
./


$consumeClassOrInterfaceTypeArgumentList1WithDims
/.
$pre_action &Parser::consumeClassOrInterfaceTypeArgumentList1WithDims;
./

$consumeSimpleName
/.
$pre_action &Parser::consumeSimpleName;
./

$consumeQualifiedName
/.
$pre_action &Parser::consumeQualifiedName;
./
$consumeMemberValuePair
/.
$pre_action &Parser::consumeMemberValuePair;
./

$consumeSingleMemberAnnotation
/.
$pre_action &Parser::consumeSingleMemberAnnotation;
./

$MakeTypeArguments
/.
$pre_action  &Parser::MakeTypeArguments;

./

$MakeArrayType
/.
$pre_action  &Parser::MakeArrayType;

./

$MakeCompilationUnit
/.
$pre_action  &Parser::MakeCompilationUnit;
./


$consumeTypeDeclarationsopt
/.
$pre_action  &Parser::consumeTypeDeclarationsopt;
./


$MakePackageDeclaration
/.

$pre_action  &Parser::MakePackageDeclaration;

./

$MakeImportDeclaration
/.

$pre_action  &Parser::MakeImportDeclaration;

./

$MakeModifier
/.

$pre_action  &Parser::MakeModifier;

./

$MakeAnnotation
/.

$pre_action  &Parser::MakeAnnotation;

./



$MakeArrayInitializer
/.

$pre_action  &Parser::MakeArrayInitializer;

./

$MakeClassDeclaration
/.

$pre_action  &Parser::MakeClassDeclaration;

./

$MakeClassBody
/.

$pre_action  &Parser::MakeClassBody;

./

$MakeFieldDeclaration
/.

$pre_action  &Parser::MakeFieldDeclaration;

./

$MakeMethodDeclaration
/.

$pre_action  &Parser::MakeMethodDeclaration;

./

$MakeMethodHeader
/.

$pre_action  &Parser::MakeMethodHeader;

./

$MakeMethodDeclarator
/.

$pre_action  &Parser::MakeMethodDeclarator;

./

$MakeFormalParameter
/.

$pre_action  &Parser::MakeFormalParameter;

./

$MakeInitializerDeclaration
/.

$pre_action  &Parser::MakeInitializerDeclaration;

./

$MakeConstructorDeclaration
/.

$pre_action  &Parser::MakeConstructorDeclaration;

./

$MakeQualifiedSuper
/.

$pre_action  &Parser::MakeQualifiedSuper;

./

$MakeEnumDeclaration
/.

$pre_action  &Parser::MakeEnumDeclaration;

./

$MakeEnumBody
/.

$pre_action  &Parser::MakeEnumBody;

./

$MakeInterfaceDeclaration
/.

$pre_action  &Parser::MakeInterfaceDeclaration;

./

$MakeAnnotationTypeDeclaration
/.

$pre_action  &Parser::MakeAnnotationTypeDeclaration;

./

$MakeAnnotationTypeMemberDeclaration
/.

$pre_action  &Parser::MakeAnnotationTypeMemberDeclaration;

./

$MakeLocalVariable
/.

$pre_action  &Parser::MakeLocalVariable;

./

$MakeLabeledStatement
/.

$pre_action  &Parser::MakeLabeledStatement;

./

$MakeExpressionStatement
/.

$pre_action  &Parser::MakeExpressionStatement;

./

$MakeIfThenElseStatement
/.

$pre_action  &Parser::MakeIfThenElseStatement;

./

$MakeSwitchLabel
/.

$pre_action  &Parser::MakeSwitchLabel;

./

$MakeWhileStatement
/.

$pre_action  &Parser::MakeWhileStatement;

./

$MakeForStatement
/.

$pre_action  &Parser::MakeForStatement;

./

$MakeForeachStatement
/.

$pre_action  &Parser::MakeForeachStatement;

./

$MakeAssertStatement
/.

$pre_action  &Parser::MakeAssertStatement;

./

$MakeTryStatement
/.

$pre_action  &Parser::MakeTryStatement;

./

$MakeParenthesizedExpression
/.

$pre_action  &Parser::MakeParenthesizedExpression;

./

$MakeClassLiteral
/.

$pre_action  &Parser::MakeClassLiteral;

./

$MakeQualifiedNew
/.

$pre_action  &Parser::MakeQualifiedNew;

./

$MakeArrayCreationUninitialized
/.

$pre_action  &Parser::MakeArrayCreationUninitialized;

./

$MakeArrayCreationInitialized
/.

$pre_action  &Parser::MakeArrayCreationInitialized;

./

$MakeFieldAccess
/.

$pre_action  &Parser::MakeFieldAccess;

./

$MakeMethodInvocation
/.

$pre_action  &Parser::MakeMethodInvocation;

./

$MakeArrayAccess
/.

$pre_action  &Parser::MakeArrayAccess;

./

$MakePreUnaryExpression
/.

$pre_action  &Parser::MakePreUnaryExpression;

./

$MakeCastExpression
/.

$pre_action  &Parser::MakeCastExpression;

./

$MakeBinaryExpression
/.

$pre_action  &Parser::MakeBinaryExpression;

./

$MakeInstanceofExpression
/.

$pre_action  &Parser::MakeInstanceofExpression;

./

$MakeConditionalExpression
/.

$pre_action  &Parser::MakeConditionalExpression;

./

$MakeWildcard
/.

$pre_action  &Parser::MakeWildcard;

./

$MakeTypeParameter
/.

$pre_action  &Parser::MakeTypeParameter;

./

$MakeTypeBound
/.

$pre_action  &Parser::MakeTypeBound;

./

$consumeEmptyTypeDeclaration
/.
$pre_action  &Parser::consumeEmptyTypeDeclaration;
./

$consumeVariableDeclaratorOnlyId
/.
$pre_action  &Parser::consumeVariableDeclaratorOnlyId;
./
$consumeVariableDeclaratorWithVariableInitializer
/.
$pre_action  &Parser::consumeVariableDeclaratorWithVariableInitializer;
./
$consumeVariableDeclaratorId
/.
$pre_action  &Parser::consumeVariableDeclaratorId;
./

$consumeMethodBody
/.
$pre_action  &Parser::consumeMethodBody;
./

$consumeExplicitConstructorThisCallInvocation
/.
$pre_action  &Parser::consumeExplicitConstructorThisCallInvocation;
./

$consumeExplicitConstructorThisCallInvocationWithTypeArguments
/.
$pre_action  &Parser::consumeExplicitConstructorThisCallInvocationWithTypeArguments;
./

$consumeExplicitConstructorSuperCallInvocation
/.
$pre_action  &Parser::consumeExplicitConstructorSuperCallInvocation;
./

$consumeExplicitConstructorSuperCallInvocationWithTypeArguments
/.
$pre_action  &Parser::consumeExplicitConstructorSuperCallInvocationWithTypeArguments;
./
$SetSym1ToSym2
/.

$pre_action  &Parser::SetSym1ToSym2;

./

$StartList
/.

$pre_action  &Parser::StartList;

./

$AddList2
/.

$pre_action  &Parser::AddList2;

./

$AddList3
/.

$pre_action  &Parser::AddList3;

./

--
-- This macro is used to initialize the rule_action array
-- to the NullAction function.
--
$NullAction
/.

$pre_action  &Parser::NullAction;

./

--
-- This macro is used to initialize the rule_action array
-- to the NoAction function.
--
$NoAction
/.

$pre_action  &Parser::NoAction;

./












$Terminals

    abstract assert boolean break byte case catch char class continue
    default do double else extends false final finally float for
    if implements import instanceof int interface long native new null
    package private protected public return short static strictfp super switch
    synchronized this throw throws transient true try void volatile while

    Identifier IntegerLiteral LongLiteral FloatLiteral DoubleLiteral
    CharacterLiteral StringLiteral

    PLUS_PLUS
    MINUS_MINUS
    EQUAL_EQUAL
    LESS_EQUAL
    GREATER_EQUAL
    NOT_EQUAL
    LEFT_SHIFT
    RIGHT_SHIFT
    UNSIGNED_RIGHT_SHIFT
    PLUS_EQUAL
    MINUS_EQUAL
    MULTIPLY_EQUAL
    DIVIDE_EQUAL
    AND_EQUAL
    OR_EQUAL
    XOR_EQUAL
    REMAINDER_EQUAL
    LEFT_SHIFT_EQUAL
    RIGHT_SHIFT_EQUAL
    UNSIGNED_RIGHT_SHIFT_EQUAL
    OR_OR
    AND_AND
    PLUS
    MINUS
    NOT
    REMAINDER
    XOR
    AND
    MULTIPLY
    OR
    TWIDDLE
    DIVIDE
    GREATER
    LESS
    LPAREN
    RPAREN
    LBRACE
    RBRACE
    LBRACKET
    RBRACKET
    SEMICOLON
    QUESTION
    COLON
    COMMA
    DOT
    EQUAL

-- These tokens will be used in JDK 1.5, but are not used now.
    enum
    AT
    ELLIPSIS

-- These remaining tokens are not used in the grammar.
    const
    goto
    ERROR
    EOF

-- This is a special token that allows us to do a 2-pass parse.
    BodyMarker
    CompilationUnitMarker
    MethodDeclarationMarker
    AnnotationTypeMemberDeclarationMarker
	ExpressionMarker


$Alias

    '++'   ::= PLUS_PLUS
    '--'   ::= MINUS_MINUS
    '=='   ::= EQUAL_EQUAL
    '<='   ::= LESS_EQUAL
    '>='   ::= GREATER_EQUAL
    '!='   ::= NOT_EQUAL
    '<<'   ::= LEFT_SHIFT
    '>>'   ::= RIGHT_SHIFT
    '>>>'  ::= UNSIGNED_RIGHT_SHIFT
    '+='   ::= PLUS_EQUAL
    '-='   ::= MINUS_EQUAL
    '*='   ::= MULTIPLY_EQUAL
    '/='   ::= DIVIDE_EQUAL
    '&='   ::= AND_EQUAL
    '|='   ::= OR_EQUAL
    '^='   ::= XOR_EQUAL
    '%='   ::= REMAINDER_EQUAL
    '<<='  ::= LEFT_SHIFT_EQUAL
    '>>='  ::= RIGHT_SHIFT_EQUAL
    '>>>=' ::= UNSIGNED_RIGHT_SHIFT_EQUAL
    '||'   ::= OR_OR
    '&&'   ::= AND_AND
    '+'    ::= PLUS
    '-'    ::= MINUS
    '!'    ::= NOT
    '%'    ::= REMAINDER
    '^'    ::= XOR
    '&'    ::= AND
    '*'    ::= MULTIPLY
    '|'    ::= OR
    '~'    ::= TWIDDLE
    '/'    ::= DIVIDE
    '>'    ::= GREATER
    '<'    ::= LESS
    '('    ::= LPAREN
    ')'    ::= RPAREN
    '{'    ::= LBRACE
    '}'    ::= RBRACE
    '['    ::= LBRACKET
    ']'    ::= RBRACKET
    ';'    ::= SEMICOLON
    '?'    ::= QUESTION
    ':'    ::= COLON
    ','    ::= COMMA
    '.'    ::= DOT
    '='    ::= EQUAL

    '@'    ::= AT
    '...'  ::= ELLIPSIS

    $EOF   ::= EOF
    $ERROR ::= ERROR

$Start

    Goal

$Rules


-- for consumeRule.cpp
\:
// $Id: jcdt.g,v 1.48 2015/12/04 13:32:27 ericb Exp $ -*- c++ -*-
// DO NOT MODIFY THIS FILE - it is generated using jikespg on jcdt.g.




#include <JCDT_Lib/internal/parser/parser.h>

namespace Jikes { // Open namespace Jikes block


void Parser::consumeRule()
{
    rule_action[0] = &Parser::BadAction;
:\



-- for javaact.cpp
/.
//#line $next_line "$input_file"


#include "JCDT_Lib/internal/env/platform.h"
#include "JCDT_Lib/internal/parser/parser.h"
#include "JCDT_Lib/internal/ast/ast.h"
#include "JCDT_Lib/internal/lex/stream.h"



namespace Jikes { // Open namespace Jikes block


//**************************************************************************//
//**************************************************************************//
//*                                                                        *//
//* Below, we show each rule of the Java grammar together with the         *//
//* semantic action that is invoked when the parser performs a reduction   *//
//* by that rule.                                                          *//
//**************************************************************************//
//**************************************************************************//
bool Parser::buildTable(){
   static bool init_Parser_buildTable = false;
   if(init_Parser_buildTable == false){
     init_Parser_buildTable = true;
   }
   else{
      return false;
   }
./




--18.2 Productions from 2.3: The syntactic Grammar

Goal ::= CompilationUnitMarker CompilationUnit
\:$SetSym1ToSym2:\
/.$location ./


Goal ::= MethodDeclarationMarker MethodDeclaration
\:$SetSym1ToSym2:\
/.$location ./


-- syntax diagnosis
Goal ::= AnnotationTypeMemberDeclarationMarker AnnotationTypeMemberDeclaration
\:$SetSym1ToSym2:\
/.$location ./



Goal ::= BodyMarker MethodBody
\:$SetSym1ToSym2:\
/.$location ./


-- completion parser
Goal ::= ExpressionMarker Expression
\:$SetSym1ToSym2:\
/.$location ./



--18.3 Productions from 3: Lexical Structure

Literal ::= IntegerLiteral
\:$consumeIntegerLiteral:\
/.$location ./


Literal ::= LongLiteral
\:$consumeLongLiteral:\
/.$location ./

Literal ::= FloatLiteral
\:$consumeFloatLiteral:\
/.$location ./

Literal ::= DoubleLiteral
\:$consumeDoubleLiteral:\
/.$location ./

Literal ::= BooleanLiteral
\:$NoAction:\
/.$shared_NoAction./

Literal ::= CharacterLiteral
\:$consumeCharacterLiteral:\
/.$location ./

Literal ::= StringLiteral
\:$consumeStringLiteral:\
/.$location ./


Literal ::= 'null'
\:$consumeNullLiteral:\
/.$location ./

BooleanLiteral ::= 'true'
\:$consumeBooleanTrueLiteral:\
/.$location ./

BooleanLiteral ::= 'false'
\:$consumeBooleanFalseLiteral:\
/.$location ./

--18.4 Productions from 4: Types, Values and Variables

Type ::= PrimitiveType
\:$NoAction:\
/.$shared_NoAction./

Type ::= ReferenceType
\:$NoAction:\
/.$shared_NoAction./

PrimitiveType ::= NumericType
\:$NoAction:\
/.$shared_NoAction./

PrimitiveType ::= 'boolean'
\:$consumeBooleanLiteral:\
/.$location ./

NumericType ::= IntegralType
\:$NoAction:\
/.$shared_NoAction./

NumericType ::= FloatingPointType
\:$NoAction:\
/.$shared_NoAction./

IntegralType ::= 'byte'
\:$consumeToken_byte:\
/.$location ./

IntegralType ::= 'short'
\:$consumeToken_short:\
/.$location ./

IntegralType ::= 'int'
\:$consumeToken_int:\
/.$location ./

IntegralType ::= 'long'
\:$consumeToken_long:\
/.$location ./

IntegralType ::= 'char'
\:$consumeToken_char:\
/.$location ./

FloatingPointType ::= 'float'
\:$consumeToken_float:\
/.$location ./

FloatingPointType ::= 'double'
\:$consumeToken_double:\
/.$location ./

--
-- Added rule to make parsing 'void' easier.
--
VoidType ::= 'void'
\:$consumeToken_void:\
/.$location ./

ReferenceType ::= ClassOrInterfaceType
\:$NoAction:\
/.$shared_NoAction./

ReferenceType ::= ArrayType
\:$NoAction:\
/.$shared_NoAction./

--
-- Simplify.
--
--ClassOrInterfaceType ::= ClassType
--ClassOrInterfaceType ::= InterfaceType
--ClassType ::= Name
--InterfaceType ::= Name
ClassOrInterfaceType ::= ClassOrInterface
\:$NoAction:\
/.$shared_NoAction./

--
-- Parameterized types were added in JSR 14.
-- Use of Marker allows us to easily find the closing '>'.
--
ClassOrInterfaceType ::= ClassOrInterface '<' TypeArgumentList1 Marker
\:$MakeTypeArguments:\
/.$location ./


ClassOrInterface ::= Name
\:$consumeClassOrInterfaceName:\
/.$location ./

--
-- Parameterized types were added in JSR 14.
--
ClassOrInterface ::= ClassOrInterface '<' TypeArgumentList1 '.' Name
\:$consumeClassOrInterfaceTypeArgumentList1Name:\
/.$location ./

--
-- These rules have been rewritten to avoid some conflicts introduced
-- by adding the 1.1 features, and to simplify syntax tree generation.
--
-- JLS1 lists:
--ArrayType ::= PrimitiveType '[' ']'
--ArrayType ::= Name '[' ']'
--ArrayType ::= ArrayType '[' ']'
-- JLS2 lists:
--ArrayType ::= Type '[' ']'
--
ArrayType ::= PrimitiveType Dims
\:$MakeArrayType:\
/.$location ./

--ArrayType ::= ClassOrInterfaceType Dims
ArrayType ::= Name Dims
\:$MakeArrayType:\
/.$shared_function
//
// void MakeArrayType();
//./

--
-- Parameterized types were added in JSR 14.
--
ArrayType ::= ClassOrInterface '<' TypeArgumentList1 '.' Name Dims
\:$consumeClassOrInterfaceTypeArgumentList1NameWithDims:\
/.$location ./

--
-- Parameterized types were added in JSR 14.
--
ArrayType ::= ClassOrInterface '<' TypeArgumentList1 Dims
\:$consumeClassOrInterfaceTypeArgumentList1WithDims:\
/.$location ./

--
-- Simplify the syntax tree.
--
--ClassType ::= ClassOrInterfaceType
--InterfaceType ::= ClassOrInterfaceType

--18.5 Productions from 6: Names

Name ::= 'Identifier'
\:$consumeSimpleName:\
/.$location ./

--
-- The use of Marker allows us to share code.
--
--Name ::= Name '.' 'Identifier'
Name ::= Name '.' Marker 'Identifier'
\:$consumeQualifiedName:\
/.$recovery_template vector<wstring>{L"Identifier"})); ./
/.$location ./

--18.6 Productions from 7: Packages

--
-- Annotations were added in JSR 175. As a result, we must inline expand
-- PackageDeclaration vs. TypeDeclaration in order to resolve the ambiguity
-- between '@A' starting '@A package B;' vs. '@A B{}'.
--
--CompilationUnit ::= PackageDeclarationopt ImportDeclarationsopt
--                    TypeDeclarationsopt
CompilationUnit ::= PackageDeclaration ImportDeclarationsopt
                    TypeDeclarationsopt
\:$MakeCompilationUnit:\
/.$location ./

--
-- The use of Marker allows us to share code.
--
CompilationUnit ::= Marker ImportDeclarations TypeDeclarationsopt
\:$MakeCompilationUnit:\
/.$shared_function
//
// void MakeCompilationUnit();
//./

--
-- See comments above why this is inline expanded.
--
CompilationUnit ::= TypeDeclarationsopt
\:$consumeTypeDeclarationsopt:\
/.$location ./

ImportDeclarations ::= ImportDeclaration
\:$StartList:\
/.$location ./

ImportDeclarations ::= ImportDeclarations ImportDeclaration
\:$AddList2:\
/.$location ./

ImportDeclarationsopt ::= $empty
\:$NullAction:\
/.$location ./


ImportDeclarationsopt ::= ImportDeclarations
\:$NoAction:\
/.$shared_NoAction./

TypeDeclarations ::= TypeDeclaration
\:$StartList:\
/.$shared_StartList./

TypeDeclarations ::= TypeDeclarations TypeDeclaration
\:$AddList2:\
/.$shared_AddList2./

TypeDeclarationsopt ::= $empty
\:$NullAction:\
/.$shared_NullAction./

TypeDeclarationsopt ::= TypeDeclarations
\:$NoAction:\
/.$shared_NoAction./

--
-- The use of Marker allows us to share code.
--
--PackageDeclaration ::= 'package' Name PackageHeaderMarker ';'
PackageDeclaration ::= Marker 'package' Name PackageHeaderMarker ';'
\:$MakePackageDeclaration:\
/.$location ./

--
-- Annotations were added in JSR 175. We must use Modifiers with a semantic
-- check that no modifier keywords appeared, because of the ambiguity between
-- '@A @B' starting '@A @B package C;' or '@A @B class C{}'.
--
PackageDeclaration ::= Modifiers 'package' Name PackageHeaderMarker ';'
\:$MakePackageDeclaration:\
/.$shared_function
//
// void MakePackageDeclaration();
//./

ImportDeclaration ::= SingleTypeImportDeclaration
\:$NoAction:\
/.$shared_NoAction./

ImportDeclaration ::= TypeImportOnDemandDeclaration
\:$NoAction:\
/.$shared_NoAction./

--
-- Static imports were added in JSR 201.
--
ImportDeclaration ::= SingleStaticImportDeclaration
\:$NoAction:\
/.$shared_NoAction./

--
-- Static imports were added in JSR 201.
--
ImportDeclaration ::= StaticImportOnDemandDeclaration
\:$NoAction:\
/.$shared_NoAction./

--
-- Note that semantically, Name must be qualified to be valid (since simple
-- type names are not in scope). However, the grammar accepts simple names.
-- The use of Marker allows us to share code.
--
--SingleTypeImportDeclaration ::= 'import' TypeName ';'
SingleTypeImportDeclaration ::= 'import' Marker Name Marker Marker ';'
\:$MakeImportDeclaration:\
/.$location ./

--
-- The use of Marker allows us to share code.
--
--TypeImportOnDemandDeclaration ::= 'import' PackageOrTypeName '.' '*' ';'
TypeImportOnDemandDeclaration ::= 'import' Marker Name '.' '*' ';'
\:$MakeImportDeclaration:\
/.$shared_function
//
// void MakeImportDeclaration();
//./

--
-- Static imports were added in JSR 201.
-- The use of Marker allows us to share code.
--
--SingleStaticImportDeclaration ::= 'import' 'static' TypeName '.'
--                                  'Identifier' ';'
SingleStaticImportDeclaration ::= 'import' 'static' Name Marker Marker ';'
\:$MakeImportDeclaration:\
/.$shared_function
//
// void MakeImportDeclaration();
//./

--
-- Static imports were added in JSR 201.
--
--StaticImportOnDemandDeclaration ::= 'import' 'static' TypeName '.' '*' ';'
StaticImportOnDemandDeclaration ::= 'import' 'static' Name '.' '*' ';'
\:$MakeImportDeclaration:\
/.$shared_function
//
// void MakeImportDeclaration();
//./

TypeDeclaration ::= ClassDeclaration
\:$NoAction:\
/.$shared_NoAction./

--
-- Enums were added in JSR 201.
--
TypeDeclaration ::= EnumDeclaration
\:$NoAction:\
/.$shared_NoAction./

TypeDeclaration ::= InterfaceDeclaration
\:$NoAction:\
/.$shared_NoAction./

--
-- Annotations were added in JSR 175.
--
TypeDeclaration ::= AnnotationTypeDeclaration
\:$NoAction:\
/.$shared_NoAction./

TypeDeclaration ::= ';'
\:$consumeEmptyTypeDeclaration:\
/.$location ./

--18.7 Only in the LALR(1) Grammar
-- Remember that we do semantic filtering on modifiers, for every context
-- they can appear in. For better error messages, we also accept all modifiers
-- for initializer blocks, formal parameters, and local variable declarations.
--
--ClassModifiers ::= Modifiers
--FieldModifiers ::= Modifiers
--MethodModifiers ::= Modifiers
--ConstructorModifiers ::= Modifiers
--InterfaceModifiers ::= Modifiers
--ConstantModifiers ::= Modifiers
--AbstractMethodModifiers ::= Modifiers
Modifiers ::= Modifier
\:$StartList:\
/.$shared_StartList./

Modifiers ::= Modifiers Modifier
\:$AddList2:\
/.$shared_AddList2./

Modifiersopt ::= $empty
\:$NullAction:\
/.$shared_NullAction./

Modifiersopt ::= Modifiers
\:$NoAction:\
/.$shared_NoAction./

Modifier ::= 'public'
\:$MakeModifier:\
/.$location ./

Modifier ::= 'protected'
\:$MakeModifier:\
/.$shared_function
//
// void MakeModifier();
//./

Modifier ::= 'private'
\:$MakeModifier:\
/.$shared_function
//
// void MakeModifier();
//./

Modifier ::= 'static'
\:$MakeModifier:\
/.$shared_function
//
// void MakeModifier();
//./

Modifier ::= 'abstract'
\:$MakeModifier:\
/.$shared_function
//
// void MakeModifier();
//./

Modifier ::= 'final'
\:$MakeModifier:\
/.$shared_function
//
// void MakeModifier();
//./

Modifier ::= 'native'
\:$MakeModifier:\
/.$shared_function
//
// void MakeModifier();
//./

Modifier ::= 'strictfp'
\:$MakeModifier:\
/.$shared_function
//
// void MakeModifier();
//./

Modifier ::= 'synchronized'
\:$MakeModifier:\
/.$shared_function
//
// void MakeModifier();
//./

Modifier ::= 'transient'
\:$MakeModifier:\
/.$shared_function
//
// void MakeModifier();
//./

Modifier ::= 'volatile'
\:$MakeModifier:\
/.$shared_function
//
// void MakeModifier();
//./

--
-- Annotations were added in JSR 175. They are valid anywhere a modifier is,
-- additionally they are valid on package declarations.
--
Modifier ::= Annotation
\:$NoAction:\
/.$shared_NoAction./

--
-- Annotations were added in JSR 175.
--
Annotation ::= NormalAnnotation
\:$NoAction:\
/.$shared_NoAction./

--
-- Annotations were added in JSR 175.
--
Annotation ::= MarkerAnnotation
\:$NoAction:\
/.$shared_NoAction./

--
-- Annotations were added in JSR 175.
--
Annotation ::= SingleMemberAnnotation
\:$NoAction:\
/.$shared_NoAction./

--
-- Annotations were added in JSR 175.
--
--NormalAnnotation ::= '@' TypeName '(' MemberValuePairsopt ')'
NormalAnnotation ::= '@' Name '(' MemberValuePairsopt ')'
\:$MakeAnnotation:\
/.$location ./

--
-- Annotations were added in JSR 175.
--
MemberValuePairs ::= MemberValuePair
\:$StartList:\
/.$shared_StartList./

--
-- Annotations were added in JSR 175.
--
MemberValuePairs ::= MemberValuePairs ',' MemberValuePair
\:$AddList3:\
/.$shared_AddList3./

--
-- Annotations were added in JSR 175.
--
MemberValuePairsopt ::= $empty
\:$NullAction:\
/.$shared_NullAction./

--
-- Annotations were added in JSR 175.
--
MemberValuePairsopt ::= MemberValuePairs
\:$NoAction:\
/.$shared_NoAction./

--
-- Annotations were added in JSR 175. We got rid of SimpleName.
--
--MemberValuePair ::= SimpleName '=' MemberValue
MemberValuePair ::= 'Identifier' '=' MemberValue
\:$consumeMemberValuePair:\
/.$location ./

--
-- Annotations were added in JSR 175.
--
MemberValue ::= ConditionalExpression
\:$NoAction:\
/.$shared_NoAction./

--
-- Annotations were added in JSR 175.
--
MemberValue ::= Annotation
\:$NoAction:\
/.$shared_NoAction./

--
-- Annotations were added in JSR 175.
--
MemberValue ::= MemberValueArrayInitializer
\:$NoAction:\
/.$recovery_template vector<wstring>{L"Identifier"})); ./
/.$location ./
/.$shared_NoAction./

--
-- Annotations were added in JSR 175. The rule was expanded inline below to
-- make the grammar LALR(1). The use of Marker allows us to share code.
--
--MemberValueArrayInitializer ::= '{' MemberValuesopt ,opt '}'
MemberValueArrayInitializer ::= '{' Marker ,opt '}'
\:$MakeArrayInitializer:\
/.$location ./

--
-- Annotations were added in JSR 175.
--
MemberValueArrayInitializer ::= '{' MemberValues ,opt '}'
\:$MakeArrayInitializer:\
/.$shared_function
//
// void MakeArrayInitializer();
//./

--
-- Annotations were added in JSR 175.
--
MemberValues ::= MemberValue
\:$StartList:\
/.$shared_StartList./

--
-- Annotations were added in JSR 175.
--
MemberValues ::= MemberValues ',' MemberValue
\:$AddList3:\
/.$shared_AddList3./

--
-- Annotations were added in JSR 175.
-- The use of Marker allows us to share code.
--
--MarkerAnnotation ::= '@' TypeName
MarkerAnnotation ::= '@' Name Marker Marker Marker
\:$MakeAnnotation:\
/.$shared_function
//
// void MakeAnnotation();
//./

--
-- Annotations were added in JSR 175.
--
--SingleMemberAnnotation ::= '@' TypeName '(' MemberValue ')'
SingleMemberAnnotation ::= '@' Name '(' MemberValue ')'
\:$consumeSingleMemberAnnotation:\
/.$location ./







--18.8 Productions from 8: Class Declarations
--18.8.1 Productions from 8.1: Class Declarations

--
-- Parameterized types were added in JSR 14.  We must inline expand
-- Modifiersopt to avoid ambiguity. The use of Marker allows us to share code.
--
--ClassDeclaration ::= ClassModifiersopt 'class' 'Identifier' Superopt
--                     Interfacesopt ClassBody

--ClassDeclaration ::= ClassModifiersopt 'class' 'Identifier' Superopt
--                     Interfacesopt ClassBody
ClassDeclaration ::= Marker 'class' 'Identifier' TypeParametersopt
                     Superopt Interfacesopt ClassBody
\:$MakeClassDeclaration:\
/.$location
./


--
-- Parameterized types were added in JSR 14.  We must inline expand
-- Modifiersopt to avoid ambiguity.
--
ClassDeclaration ::= Modifiers 'class' 'Identifier' TypeParametersopt
                     Superopt Interfacesopt ClassBody
\:$MakeClassDeclaration:\
/.$location
./

--
-- Simplify.
--
--Super ::= 'extends' ClassType
Super ::= 'extends' ClassOrInterfaceType
\:$SetSym1ToSym2:\
/.$shared_function
//
// void SetSym1ToSym2();
//./

Superopt ::= $empty
\:$NullAction:\
/.$shared_NullAction./

Superopt ::= Super
\:$NoAction:\
/.$shared_NoAction./

--
-- Simplify.
--
--Interfaces ::= 'implements' InterfaceTypeList
Interfaces ::= 'implements' TypeList
\:$SetSym1ToSym2:\
/.$shared_function
//
// void SetSym1ToSym2();
//./

Interfacesopt ::= $empty
\:$NullAction:\
/.$shared_NullAction./

Interfacesopt ::= Interfaces
\:$NoAction:\
/.$shared_NoAction./

--InterfaceTypeList ::= InterfaceType
--ClassTypeList ::= ClassType
TypeList ::= ClassOrInterfaceType
\:$StartList:\
/.$shared_StartList./

--InterfaceTypeList ::= InterfaceTypeList ',' InterfaceType
--ClassTypeList ::= ClassTypeList ',' ClassType
TypeList ::= TypeList ',' ClassOrInterfaceType
\:$AddList3:\
/.$shared_AddList3./

ClassBody ::= '{' ClassBodyDeclarationsopt '}'
\:$MakeClassBody:\

/.$no_statements_recovery ./
/.$location./

ClassBodyopt ::= $empty
\:$NullAction:\
/.$shared_NullAction./

ClassBodyopt ::= ClassBody
\:$NoAction:\
/.$no_statements_recovery ./
/.$location ./
/.$shared_NoAction./

ClassBodyDeclarations ::= ClassBodyDeclaration
\:$StartList:\
/.$shared_StartList./

ClassBodyDeclarations ::= ClassBodyDeclarations ClassBodyDeclaration
\:$AddList2:\
/.$shared_AddList2./

ClassBodyDeclarationsopt ::= $empty
\:$NullAction:\
/.$shared_NullAction./

ClassBodyDeclarationsopt ::= ClassBodyDeclarations
\:$NoAction:\
/.$shared_NoAction./

--
-- Simplify. See below.
--
--ClassBodyDeclaration ::= ClassMemberDeclaration

ClassBodyDeclaration ::= ConstructorDeclaration
\:$NoAction:\
/.$shared_NoAction./

--
-- For nicer semantic error messages, we lump static and instance initializers
-- together. Also, we parse arbitrary modifiers, but semantically only accept
-- static or no modifiers.
--
--ClassBodyDeclaration ::= StaticInitializer
--ClassBodyDeclaration ::= MethodBody
ClassBodyDeclaration ::= InitializerDeclaration
\:$NoAction:\
/.$shared_NoAction./

--
-- Simplify.
--
--ClassMemberDeclaration ::= FieldDeclaration
ClassBodyDeclaration ::= FieldDeclaration
\:$NoAction:\
/.$shared_NoAction./

--
-- Simplify.
--
--ClassMemberDeclaration ::= MethodDeclaration
ClassBodyDeclaration ::= MethodDeclaration
\:$NoAction:\
/.$shared_NoAction./

--1.1 feature
--
-- Consolidate and simplify.
--ClassMemberDeclaration ::= ClassDeclaration
--ClassMemberDeclaration ::= InterfaceDeclaration
--ClassMemberDeclaration ::= ';'
--
ClassBodyDeclaration ::= TypeDeclaration
\:$NoAction:\
/.$shared_NoAction./

--18.8.2 Productions from 8.3: Field Declarations

--
-- The use of Marker allows us to share code.  We must inline expand
-- Modifiersopt to avoid ambiguity.
--
--FieldDeclaration ::= FieldModifiersopt Type VariableDeclarators ';'
FieldDeclaration ::= Marker Marker Type VariableDeclarators ';'
\:$MakeFieldDeclaration:\
/.$recovery_template vector<wstring>{L";"})); ./
/.$location./

FieldDeclaration ::= Modifiers Marker Type VariableDeclarators ';'
/.$recovery_template vector<wstring>{L";"})); ./
\:$MakeFieldDeclaration:\
/.$shared_function
//
// void MakeFieldDeclaration();
//./

VariableDeclarators ::= VariableDeclarator
\:$StartList:\
/.$shared_StartList./

VariableDeclarators ::= VariableDeclarators ',' VariableDeclarator
\:$AddList3:\
/.$recovery_template vector<wstring>{L"Identifier"})); ./
/.$location ./
/.$shared_AddList3./

VariableDeclarator ::= VariableDeclaratorId
\:$consumeVariableDeclaratorOnlyId:\
/.$location ./

VariableDeclarator ::= VariableDeclaratorId '=' VariableInitializer
\:$consumeVariableDeclaratorWithVariableInitializer:\
/.$recovery_template vector<wstring>{L"Identifier"})); ./
/.$location./

VariableDeclaratorId ::= 'Identifier' Dimsopt
\:$consumeVariableDeclaratorId:\
/.$recovery_template vector<wstring>{L"Identifier"})); ./
/.$location./

VariableInitializer ::= Expression
\:$NoAction:\
/.$shared_NoAction./

VariableInitializer ::= ArrayInitializer
\:$NoAction:\
/.$recovery_template vector<wstring>{L"Identifier"})); ./
/.$location ./
/.$shared_NoAction./

--18.8.3 Productions from 8.4: Method Declarations
--
-- We use "MethodHeaderMarker" to speed up parsing while minimizing memory.
-- During the first pass, we only care about declarations, so we skip
-- everything inside { }. On the second pass, we parse only one method at a
-- time (see the production of Goal above).
--
-- Also, we expanded MethodBody inline to enable the sharing of MethodBody
-- between methods, constructors, and initializers. Note that MethodBody
-- can support an explicit constructor call; so it requires semantic filtering.
--
--MethodDeclaration ::= MethodHeader MethodBody
MethodDeclaration ::= MethodHeader MethodHeaderMarker MethodBody
\:$MakeMethodDeclaration:\
/.$location./

--
-- The use of Marker allows us to share code.
--
MethodDeclaration ::= MethodHeader MethodHeaderMarker Marker ';'
\:$MakeMethodDeclaration:\
/.$shared_function
//
// void MakeMethodDeclaration();
//./


--
-- The use of Marker allows us to share code.  We must inline expand
-- Modifiersopt to avoid ambiguity.
--
--MethodHeader ::= MethodModifiersopt Type MethodDeclarator Throwsopt
MethodHeader ::= Marker Marker Type MethodDeclarator Throwsopt
\:$MakeMethodHeader:\
/.$location./

--
-- The use of Marker allows us to share code.  We must inline expand
-- Modifiersopt to avoid ambiguity.
--
MethodHeader ::= Modifiers Marker Type MethodDeclarator Throwsopt
\:$MakeMethodHeader:\
/.$shared_function
//
// void MakeMethodHeader();
//./

--
-- Parameterized types were added in JSR 14.  We must inline expand
-- Modifiersopt to avoid ambiguity. The use of Marker allows us to share code.
--
--MethodHeader ::= MethodModifiersopt Type MethodDeclarator Throwsopt
MethodHeader ::= Marker TypeParameters Type MethodDeclarator Throwsopt
\:$MakeMethodHeader:\
/.$shared_function
//
// void MakeMethodHeader();
//./

--
-- Parameterized types were added in JSR 14.  We must inline expand
-- Modifiersopt to avoid ambiguity.
--
MethodHeader ::= Modifiers TypeParameters Type MethodDeclarator Throwsopt
\:$MakeMethodHeader:\
/.$shared_function
//
// void MakeMethodHeader();
//./

--
-- The use of Marker allows us to share code.  We must inline expand
-- Modifiersopt to avoid ambiguity.
--
--MethodHeader ::= MethodModifiersopt 'void' MethodDeclarator Throwsopt
MethodHeader ::= Marker Marker VoidType MethodDeclarator Throwsopt
\:$MakeMethodHeader:\
/.$shared_function
//
// void MakeMethodHeader();
//./

--
-- The use of Marker allows us to share code.  We must inline expand
-- Modifiersopt to avoid ambiguity.
--
MethodHeader ::= Modifiers Marker VoidType MethodDeclarator Throwsopt
\:$MakeMethodHeader:\
/.$shared_function
//
// void MakeMethodHeader();
//./

--
-- Parameterized types were added in JSR 14.  We must inline expand
-- Modifiersopt to avoid ambiguity. The use of Marker allows us to share code.
--
--MethodHeader ::= Modifiersopt 'void' MethodDeclarator Throwsopt
MethodHeader ::= Marker TypeParameters VoidType MethodDeclarator Throwsopt
\:$MakeMethodHeader:\
/.$shared_function
//
// void MakeMethodHeader();
//./

--
-- Parameterized types were added in JSR 14.  We must inline expand
-- Modifiersopt to avoid ambiguity.
--
MethodHeader ::= Modifiers TypeParameters VoidType MethodDeclarator Throwsopt
\:$MakeMethodHeader:\
/.$shared_function
//
// void MakeMethodHeader();
//./

MethodDeclarator ::= 'Identifier' '(' FormalParameterListopt ')' Dimsopt
\:$MakeMethodDeclarator:\
/.$location./

--
-- Varargs were added in JSR 201.
--
--FormalParameterList ::= FormalParameter
FormalParameterList ::= LastFormalParameter
\:$StartList:\
/.$shared_StartList./

--
-- Varargs were added in JSR 201.
--
--FormalParameterList ::= FormalParameterList ',' FormalParameter
FormalParameterList ::= FormalParameters ',' LastFormalParameter
\:$AddList3:\
/.$shared_AddList3./

FormalParameterListopt ::= $empty
\:$NullAction:\
/.$shared_NullAction./

FormalParameterListopt ::= FormalParameterList
\:$NoAction:\
/.$shared_NoAction./

--
-- Varargs were added in JSR 201.
--
FormalParameters ::= FormalParameter
\:$StartList:\
/.$shared_StartList./

--
-- Varargs were added in JSR 201.
--
FormalParameters ::= FormalParameters ',' FormalParameter
\:$AddList3:\
/.$shared_AddList3./

--
-- For nicer error messages, we accept all modifiers, even though only
-- 'final' and annotations are valid. Also, we must inline expand finalopt
-- to avoid ambiguity. The use of Marker allows us to share code.
--
--FormalParameter ::= finalopt Type VariableDeclaratorId
FormalParameter ::= Type Marker Marker VariableDeclaratorId
\:$MakeFormalParameter:\
/.$location./

--1.1 feature
--
-- For nicer error messages, we accept all modifiers, even though only
-- 'final' and annotations are valid. Also, we must inline expand finalopt
-- to avoid ambiguity. The use of Marker allows us to share code.
--
--FormalParameter ::= final Type VariableDeclaratorId
FormalParameter ::= Modifiers Type Marker VariableDeclaratorId
\:$MakeFormalParameter:\
/.$shared_function
//
// void MakeFormalParameter();
//./

--
-- Varargs were added in JSR 201. We must match the inline expansion of
-- FormalParameter to avoid ambiguity.
--
--LastFormalParameter ::= Modifiersopt Type ...opt VariableDeclaratorId
LastFormalParameter ::= FormalParameter
\:$NoAction:\
/.$shared_NoAction./

--
-- Varargs were added in JSR 201. The use of Marker allows us to share code.
--
LastFormalParameter ::= Type Marker '...' VariableDeclaratorId
\:$MakeFormalParameter:\
/.$shared_function
//
// void MakeFormalParameter();
//./

--
-- Varargs were added in JSR 201.
-- For nicer error messages, we accept all modifiers, even though only
-- 'final' and annotations are valid.
--
LastFormalParameter ::= Modifiers Type '...' VariableDeclaratorId
\:$MakeFormalParameter:\
/.$recovery_template vector<wstring>{L"Identifier",L"Identifier"})); ./
/.$location ./
/.$shared_function
//
// void MakeFormalParameter();
//./

--
-- Simplify.
--
--Throws ::= 'throws' ClassTypeList
Throws ::= 'throws' TypeList
\:$SetSym1ToSym2:\
/.$shared_function
//
// void SetSym1ToSym2();
//./

Throwsopt ::= $empty
\:$NullAction:\
/.$shared_NullAction./

Throwsopt ::= Throws
\:$NoAction:\
/.$shared_NoAction./

--
-- Notice that we filter out an initial explicit constructor invocation,
-- since we have modified Statement to include this() and super(). Other
-- explicit constructor calls are ignored now, and flagged as errors
-- during semantic analysis.
--
MethodBody ::= '{' BlockStatementsopt '}'
\:$consumeMethodBody:\
/. $no_statements_recovery ./
/.$location./

--
-- Instead of directly including this rule, we have expanded it inline above.
--
--MethodBody ::= ';'
--

--18.8.4 Productions from 8.5: Static Initializers
--
-- For nicer error messages, we accept arbitrary modifiers. Thus this rule can
-- parse static and instance initializers. The use of MethodHeaderMarker
-- allows the 2-pass parsing. See comments of MethodDeclaration.
-- The use of Marker allows us to share code.
--
--StaticInitializer ::= 'static' MethodBody
InitializerDeclaration ::= Marker MethodHeaderMarker MethodBody
\:$MakeInitializerDeclaration:\
/.$location./

-- For nicer error messages, we accept arbitrary modifiers. Thus this rule can
-- parse static and instance initializers. The use of MethodHeaderMarker
-- allows the 2-pass parsing. See comments of MethodDeclaration.
--
InitializerDeclaration ::= Modifiers MethodHeaderMarker MethodBody
\:$MakeInitializerDeclaration:\
/.$shared_function
//
// void MakeInitializerDeclaration();
//./

--18.8.5 Productions from 8.6: Constructor Declarations
--
-- The use of Marker allows us to share code. MethodHeaderMarker allows us to
-- do 2-pass parsing, and MethodBody was rewritten to handle constructor
-- bodies. We must inline expand Modifiersopt to avoid ambiguity.
--

--ConstructorDeclaration ::= ConstructorModifiersopt ConstructorDeclarator
--                           Throwsopt ConstructorBody

ConstructorDeclaration ::= Marker Marker ConstructorDeclarator Throwsopt  MethodHeaderMarker  MethodBody
\:$MakeConstructorDeclaration:\
/.$location./

--
-- The use of Marker allows us to share code. MethodHeaderMarker allows us to
-- do 2-pass parsing, and MethodBody was rewritten to handle constructor
-- bodies. We must inline expand Modifiersopt to avoid ambiguity.
--
ConstructorDeclaration ::= Modifiers Marker ConstructorDeclarator Throwsopt
                           MethodHeaderMarker  MethodBody
\:$MakeConstructorDeclaration:\
/.$location./

--
-- Parameterized types were added in JSR 14.  We must inline expand
-- Modifiersopt to avoid ambiguity. The use of Marker allows us to share code.
--
--ConstructorDeclaration ::= ConstructorModifiersopt ConstructorDeclarator
--                           Throwsopt ConstructorBody
ConstructorDeclaration ::= Marker TypeParameters ConstructorDeclarator
                           Throwsopt MethodHeaderMarker  MethodBody
\:$MakeConstructorDeclaration:\
/.$location./

--
-- Parameterized types were added in JSR 14.  We must inline expand
-- Modifiersopt to avoid ambiguity.
--
ConstructorDeclaration ::= Modifiers TypeParameters ConstructorDeclarator
                           Throwsopt MethodHeaderMarker  MethodBody
\:$MakeConstructorDeclaration:\
/.$location./

--
-- The use of Marker allows us to share code. Also, we got rid of SimpleName.
--
--ConstructorDeclarator ::= SimpleName '(' FormalParameterListopt ')'
ConstructorDeclarator ::= 'Identifier' '(' FormalParameterListopt ')' Marker
\:$MakeMethodDeclarator:\
/.$shared_function
//
// void MakeMethodDeclarator();
//./

--
-- For better error reporting, we have coalesced ExplicitConstructorInvocation
-- into BlockStatement. Therefore, we do not need a rule for ConstructorBody,
-- since MethodBody does the same amount of work. During semantic analysis,
-- we then check calls of an explicit constructor invocation out of context.
--
--ConstructorBody ::= '{' ExplicitConstructorInvocationopt
--                    BlockStatementsopt '}'
--

--
-- Simplify.
--
--ExplicitConstructorInvocation ::= 'this' '(' ArgumentListopt ')' ';'
ExplicitConstructorInvocation ::= 'this' Arguments ';'
\:$consumeExplicitConstructorThisCallInvocation:\
/.$location./

--
-- Parameterized types were added in JSR 14.
--
ExplicitConstructorInvocation ::= TypeArguments 'this' Arguments ';'
\:$consumeExplicitConstructorThisCallInvocationWithTypeArguments:\
/.$location./

--
-- Simplify.
--
--ExplicitConstructorInvocation ::= 'super' '(' ArgumentListopt ')' ';'
ExplicitConstructorInvocation ::= 'super' Arguments ';'
\:$consumeExplicitConstructorSuperCallInvocation:\
/.$location./

--
-- Parameterized types were added in JSR 14.
--
ExplicitConstructorInvocation ::= TypeArguments 'super' Arguments ';'
\:$consumeExplicitConstructorSuperCallInvocationWithTypeArguments:\
/.$location./

--1.1 feature
--
-- Parameterized types were added in JSR 14.
--
--ExplicitConstructorInvocation ::= Primary '.' 'super' '(' ArgumentListopt ')'
--                                  ';'
ExplicitConstructorInvocation ::= Primary '.' TypeArgumentsopt 'super'
                                  Arguments ';'
\:$MakeQualifiedSuper:\
/.$location./

--1.1 feature
--
-- The use of Marker allows us to share code.
--
--ExplicitConstructorInvocation ::= Name '.' 'super' '(' ArgumentListopt ')'
--                                  ';'
ExplicitConstructorInvocation ::= Name '.' Marker 'super' Arguments ';'
\:$MakeQualifiedSuper:\
/.$shared_function
//
// void MakeQualifiedSuper();
//./

--1.1 feature
--
-- Parameterized types were added in JSR 14.
--
--ExplicitConstructorInvocation ::= Name '.' 'super' '(' ArgumentListopt ')'
--                                  ';'
ExplicitConstructorInvocation ::= Name '.' TypeArguments 'super' Arguments ';'
\:$MakeQualifiedSuper:\
/.$shared_function
//
// void MakeQualifiedSuper();
//./

--
-- Enums were added in JSR 201. We must inline expand Modifiersopt to avoid
-- ambiguity. The use of Marker allows us to share code.

--EnumDeclaration ::= ClassModifiersopt 'enum' 'Identifier' Interfacesopt
--                    EnumBody
EnumDeclaration ::= Marker 'enum' 'Identifier' Interfacesopt EnumBody
\:$MakeEnumDeclaration:\
/.$location./


--
-- Enums were added in JSR 201. We must inline expand Modifiersopt to avoid
-- ambiguity. The use of Marker allows us to share code.
--
EnumDeclaration ::= Modifiers 'enum' 'Identifier' Interfacesopt EnumBody
\:$MakeEnumDeclaration:\
/.$location./


--
-- Enums were added in JSR 201. The rule was expanded inline below to
-- make the grammar LALR(1). The use of Marker allows us to share code.
--
--EnumBody ::= '{' EnumConstantsopt ,opt EnumBodyDeclarationsopt '}'
EnumBody ::= '{' Marker ,opt EnumBodyDeclarationsopt '}'
\:$MakeEnumBody:\
/.$location./

--
-- Enums were added in JSR 201.
--
EnumBody ::= '{' EnumConstants ,opt EnumBodyDeclarationsopt '}'
\:$MakeEnumBody:\
/.$shared_function
//
// void MakeEnumBody();
//./

--
-- Enums were added in JSR 201.
--
EnumConstants ::= EnumConstant
\:$StartList:\
/.$shared_StartList./

--
-- Enums were added in JSR 201.
--
EnumConstants ::= EnumConstants ',' EnumConstant
\:$AddList3:\
/.$shared_AddList3./

--
-- Enums were added in JSR 201.
--
EnumConstant ::= Modifiersopt 'Identifier' Argumentsopt ClassBodyopt
\:$consumeEnumConstantWithClassBodyOpt:\
/.$location./




--
-- Enums were added in JSR 201. As long as enums introduce this production, we
-- use it elsewhere, too.
--
Arguments ::= '(' ArgumentListopt ')'
\:$consumeArguments:\
/.$location./

--
-- Enums were added in JSR 201.
--
Argumentsopt ::= $empty
\:$NullAction:\
/.$shared_NullAction./

--
-- Enums were added in JSR 201.
--
Argumentsopt ::= Arguments
\:$NoAction:\
/.$shared_NoAction./

--
-- Enums were added in JSR 201. The use of Marker allows us to share code.
--
--EnumBodyDeclarations ::= ';' ClassBodyDeclarationsopt
EnumBodyDeclarations ::= ';' ClassBodyDeclarationsopt Marker
\:$MakeClassBody:\
/.$shared_function
//
// void MakeClassBody();
//./

--
-- Enums were added in JSR 201.
--
EnumBodyDeclarationsopt ::= $empty
\:$NullAction:\
/.$shared_NullAction./

--
-- Enums were added in JSR 201.
--
EnumBodyDeclarationsopt ::= EnumBodyDeclarations
\:$NoAction:\
/.$shared_NoAction./

--18.9 Productions from 9: Interface Declarations
--18.9.1 Productions from 9.1: Interface Declarations
--
-- Parameterized types were added in JSR 14.  We must inline expand
-- Modifiersopt to avoid ambiguity. The use of Marker allows us to share code.
--
--InterfaceDeclaration ::= InterfaceModifiersopt 'interface' 'Identifier'
--                         ExtendsInterfacesopt InterfaceBody
InterfaceDeclaration ::= Marker 'interface' 'Identifier' TypeParametersopt
                         ExtendsInterfacesopt  InterfaceBody
\:$MakeInterfaceDeclaration:\
/.$location./


--
-- Parameterized types were added in JSR 14.  We must inline expand
-- Modifiersopt to avoid ambiguity.
--
InterfaceDeclaration ::= Modifiers 'interface' 'Identifier' TypeParametersopt
                         ExtendsInterfacesopt InterfaceBody
\:$MakeInterfaceDeclaration:\
/.$location./


--
-- Simplify.
--
--ExtendsInterfaces ::= 'extends' InterfaceTypeList
ExtendsInterfaces ::= 'extends' TypeList
\:$SetSym1ToSym2:\
/.$shared_function
//
// void SetSym1ToSym2();
//./

ExtendsInterfacesopt ::= $empty
\:$NullAction:\
/.$shared_NullAction./

ExtendsInterfacesopt ::= ExtendsInterfaces
\:$NoAction:\
/.$shared_NoAction./

InterfaceBody ::= '{' InterfaceMemberDeclarationsopt '}'
\:$MakeClassBody:\
/.$shared_function
//
// void MakeClassBody();
//./

InterfaceMemberDeclarations ::= InterfaceMemberDeclaration
\:$StartList:\
/.$shared_StartList./

InterfaceMemberDeclarations ::= InterfaceMemberDeclarations
                                InterfaceMemberDeclaration
\:$AddList2:\
/.$shared_AddList2./

InterfaceMemberDeclarationsopt ::= $empty
\:$NullAction:\
/.$shared_NullAction./

InterfaceMemberDeclarationsopt ::= InterfaceMemberDeclarations
\:$NoAction:\
/.$shared_NoAction./

--
--
-- For less code duplication and better semantic messages, we parse
-- non-abstract method bodies and non-initialized fields now, then do a
-- semantic check that this was valid.
--
InterfaceMemberDeclaration ::= ConstantDeclaration
\:$NoAction:\
/.$shared_NoAction./

--
-- Simplify. See below.
--
--InterfaceMemberDeclaration ::= AbstractMethodDeclaration

--1.1 feature
--
-- Consolidate and simplify.
--
--InterfaceMemberDeclaration ::= ClassDeclaration
--InterfaceMemberDeclaration ::= InterfaceDeclaration
--InterfaceMemberDeclaration ::= ';'
InterfaceMemberDeclaration ::= TypeDeclaration
\:$NoAction:\
/.$shared_NoAction./

ConstantDeclaration ::= FieldDeclaration
\:$consumeConstantDeclaration:\
/.$location./

--
-- Simplify.
--
--AbstractMethodDeclaration ::= MethodHeader ';'
InterfaceMemberDeclaration ::= MethodDeclaration
\:$NoAction:\
/.$shared_NoAction./

--
-- Annotations were added in JSR 175. We must inline expand Modifiersopt to
-- avoid ambiguity: does 'public @' start 'public @A class B{}' or
-- 'public @interface A{}'. The use of Marker allows us to share code.
--

--AnnotationTypeDeclaration ::= InterfaceModifiersopt '@' 'interface'
--                              'Identifier' AnnotationTypeBody
AnnotationTypeDeclaration ::= '@' Marker 'interface' 'Identifier' AnnotationTypeBody                             
\:$MakeAnnotationTypeDeclaration:\
/.$location./

-- Annotations were added in JSR 175. We must inline expand Modifiersopt to
-- avoid ambiguity: does 'public @' start 'public @A class B{}' or
-- 'public @interface A{}'.
--
AnnotationTypeDeclaration ::= Modifiers '@' 'interface' 'Identifier' AnnotationTypeBody
\:$MakeAnnotationTypeDeclaration:\
/.$location./


--
-- Annotations were added in JSR 175.
--
AnnotationTypeBody ::= '{' AnnotationTypeMemberDeclarationsopt '}'
\:$MakeClassBody:\
/.$shared_function
//
// void MakeClassBody();
//./

--
-- Annotations were added in JSR 175.
--
AnnotationTypeMemberDeclarations ::= AnnotationTypeMemberDeclaration
\:$StartList:\
/.$shared_StartList./

--
-- Annotations were added in JSR 175.
--
AnnotationTypeMemberDeclarations ::= AnnotationTypeMemberDeclarations
                                     AnnotationTypeMemberDeclaration
\:$AddList2:\
/.$shared_AddList2./

--
-- Annotations were added in JSR 175.
--
AnnotationTypeMemberDeclarationsopt ::= $empty
\:$NullAction:\
/.$shared_NullAction./

--
-- Annotations were added in JSR 175.
--
AnnotationTypeMemberDeclarationsopt ::= AnnotationTypeMemberDeclarations
\:$NoAction:\
/.$shared_NoAction./

--
-- Annotations were added in JSR 175. We must inline expand Modifiersopt to
-- avoid ambiguity. The use of Marker allows us to share code.
--
--AnnotationTypeMemberDeclaration ::= AbstractMethodModifiersopt Type
--                                    'Identifier' '(' ')' DefaultValueopt ';'
AnnotationTypeMemberDeclaration ::= Marker Marker Type 'Identifier' '(' ')'
                                    DefaultValueopt ';'
\:$MakeAnnotationTypeMemberDeclaration:\
/.$location./

--
-- Annotations were added in JSR 175. We must inline expand Modifiersopt to
-- avoid ambiguity.
--
AnnotationTypeMemberDeclaration ::= Modifiers Marker Type 'Identifier' '(' ')'
                                    DefaultValueopt ';'
\:$MakeAnnotationTypeMemberDeclaration:\
/.$shared_function
//
// void MakeAnnotationTypeMemberDeclaration();
//./

--
-- Annotations were added in JSR 175.
--
AnnotationTypeMemberDeclaration ::= ConstantDeclaration
\:$NoAction:\
/.$shared_NoAction./

--
-- Annotations were added in JSR 175. Consolidate and simplify.
--
--AnnotationTypeMemberDeclaration ::= ClassDeclaration
--AnnotationTypeMemberDeclaration ::= InterfaceDeclaration
--AnnotationTypeMemberDeclaration ::= EnumDeclaration
--AnnotationTypeMemberDeclaration ::= AnnotationTypeDeclaration
--AnnotationTypeMemberDeclaration ::= ';'
AnnotationTypeMemberDeclaration ::= TypeDeclaration
\:$NoAction:\
/.$shared_NoAction./

--
-- Annotations were added in JSR 175.
--
DefaultValue ::= 'default' MemberValue
\:$SetSym1ToSym2:\
/.$shared_function
//
// void SetSym1ToSym2();
//./

--
-- Annotations were added in JSR 175.
--
DefaultValueopt ::= $empty
\:$NullAction:\
/.$shared_NullAction./

--
-- Annotations were added in JSR 175.
--
DefaultValueopt ::= DefaultValue
\:$NoAction:\
/.$shared_NoAction./

--18.10 Productions from 10: Arrays
--
-- The rule VariableInitializersopt was expanded inline below to make the
-- grammar LALR(1). The use of Marker allows us to share code.
--
-- ArrayInitializer ::= '{' VariableInitializersopt ,opt '}'
ArrayInitializer ::= '{' Marker ,opt '}'
\:$MakeArrayInitializer:\
/.$shared_function
//
// void MakeArrayInitializer();
//./

ArrayInitializer ::= '{' VariableInitializers ,opt '}'
\:$MakeArrayInitializer:\
/.$recovery_template vector<wstring>{L"Identifier"})); ./
/.$location ./
/.$shared_function
//
// void MakeArrayInitializer();
//./

VariableInitializers ::= VariableInitializer
\:$StartList:\
/.$shared_StartList./

VariableInitializers ::= VariableInitializers ',' VariableInitializer
\:$AddList3:\
/.$shared_AddList3./

--18.11 Productions from 13: Blocks and Statements

Block ::= '{' BlockStatementsopt '}'
\:$consumeBlock:\
/.$location./

BlockStatements ::= BlockStatement
\:$StartList:\
/.$shared_StartList./

BlockStatements ::= BlockStatements BlockStatement
\:$AddList2:\
/.$shared_AddList2./

BlockStatementsopt ::= $empty
\:$NullAction:\
/.$shared_NullAction./

BlockStatementsopt ::= BlockStatements
\:$NoAction:\
/.$shared_NoAction./

BlockStatement ::= LocalVariableDeclarationStatement
\:$NoAction:\
/.$shared_NoAction./

BlockStatement ::= Statement
\:$NoAction:\
/.$shared_NoAction./

--1.1 feature
BlockStatement ::= ClassDeclaration
\:$consumeInvalidClassDeclaration:\
/.$location./

--
-- Enums were added in JSR 201.
--
BlockStatement ::= EnumDeclaration
\:$consumeInvalidEnumDeclaration:\
/.$location./

--
-- NOTE: This rule is not in the original grammar. We added it, and changed
-- the rule for ConstructorBody, in order to issue a nicer error message
-- when this() or super() is encountered out of context.
--
BlockStatement ::= ExplicitConstructorInvocation
\:$NoAction:\
/.$shared_NoAction./

LocalVariableDeclarationStatement ::= LocalVariableDeclaration ';'
\:$consumeLocalVariableDeclarationStatement:\
/.$location./

--
-- For nicer error messages, we accept all modifiers, even though only
-- 'final' and annotations are valid. Also, we must inline expand finalopt
-- to avoid ambiguity. The use of Marker allows us to share code.
--
--LocalVariableDeclaration ::= finalopt Type VariableDeclarators
LocalVariableDeclaration ::= Type Marker Marker VariableDeclarators
\:$MakeLocalVariable:\
/.$location./

--1.1 feature
--
-- For nicer error messages, we accept all modifiers, even though only
-- 'final' and annotations are valid. Also, we must inline expand finalopt
-- to avoid ambiguity. The use of Marker allows us to share code.
--
--LocalVariableDeclaration ::= final Type VariableDeclarators
LocalVariableDeclaration ::= Modifiers Type Marker VariableDeclarators
\:$MakeLocalVariable:\
/.$shared_function
//
// void MakeLocalVariable();
//./

Statement ::= StatementWithoutTrailingSubstatement
\:$NoAction:\
/.$shared_NoAction./

Statement ::= LabeledStatement
\:$NoAction:\
/.$shared_NoAction./

Statement ::= IfThenStatement
\:$NoAction:\
/.$shared_NoAction./

Statement ::= IfThenElseStatement
\:$NoAction:\
/.$shared_NoAction./

Statement ::= WhileStatement
\:$NoAction:\
/.$shared_NoAction./

Statement ::= ForStatement
\:$NoAction:\
/.$shared_NoAction./

--
-- Enhanced for statements (also known as foreach, but without a new keyword),
-- were added in JDK 1.5, as part of JSR 201.
--
Statement ::= ForeachStatement
\:$NoAction:\
/.$recovery_template vector<wstring>{L";"})); ./
/.$location ./
/.$shared_NoAction./

StatementNoShortIf ::= StatementWithoutTrailingSubstatement
\:$NoAction:\
/.$shared_NoAction./

StatementNoShortIf ::= LabeledStatementNoShortIf
\:$NoAction:\
/.$shared_NoAction./

StatementNoShortIf ::= IfThenElseStatementNoShortIf
\:$NoAction:\
/.$shared_NoAction./

StatementNoShortIf ::= WhileStatementNoShortIf
\:$NoAction:\
/.$shared_NoAction./

StatementNoShortIf ::= ForStatementNoShortIf
\:$NoAction:\
/.$shared_NoAction./

--
-- Enhanced for statements (also known as foreach, but without a new keyword),
-- were added in JDK 1.5, as part of JSR 201.
--
StatementNoShortIf ::= ForeachStatementNoShortIf
\:$NoAction:\
/.$shared_NoAction./

StatementWithoutTrailingSubstatement ::= Block
\:$NoAction:\
/.$shared_NoAction./

StatementWithoutTrailingSubstatement ::= EmptyStatement
\:$NoAction:\
/.$shared_NoAction./

StatementWithoutTrailingSubstatement ::= ExpressionStatement
\:$NoAction:\
/.$shared_NoAction./

StatementWithoutTrailingSubstatement ::= SwitchStatement
\:$NoAction:\
/.$shared_NoAction./

StatementWithoutTrailingSubstatement ::= DoStatement
\:$NoAction:\
/.$shared_NoAction./

StatementWithoutTrailingSubstatement ::= BreakStatement
\:$NoAction:\
/.$shared_NoAction./

StatementWithoutTrailingSubstatement ::= ContinueStatement
\:$NoAction:\
/.$shared_NoAction./

StatementWithoutTrailingSubstatement ::= ReturnStatement
\:$NoAction:\
/.$shared_NoAction./

StatementWithoutTrailingSubstatement ::= SynchronizedStatement
\:$NoAction:\
/.$shared_NoAction./

StatementWithoutTrailingSubstatement ::= ThrowStatement
\:$NoAction:\
/.$shared_NoAction./

StatementWithoutTrailingSubstatement ::= TryStatement
\:$NoAction:\
/.$shared_NoAction./

StatementWithoutTrailingSubstatement ::= AssertStatement
\:$NoAction:\
/.$shared_NoAction./

EmptyStatement ::= ';'
\:$consumeEmptyStatement:\
/.$location./

LabeledStatement ::= 'Identifier' ':' Statement
\:$MakeLabeledStatement:\
/.$location./

LabeledStatementNoShortIf ::= 'Identifier' ':' StatementNoShortIf
\:$MakeLabeledStatement:\
/.$shared_function
//
// void MakeLabeledStatement();
//./

ExpressionStatement ::= StatementExpression ';'
\:$consumeExpressionStatement:\
/.$location./

StatementExpression ::= Assignment
\:$MakeExpressionStatement:\
/.$location./

StatementExpression ::= PreIncrementExpression
\:$MakeExpressionStatement:\
/.$shared_function
//
// void MakeExpressionStatement();
//./

StatementExpression ::= PreDecrementExpression
\:$MakeExpressionStatement:\
/.$shared_function
//
// void MakeExpressionStatement();
//./

StatementExpression ::= PostIncrementExpression
\:$MakeExpressionStatement:\
/.$shared_function
//
// void MakeExpressionStatement();
//./

StatementExpression ::= PostDecrementExpression
\:$MakeExpressionStatement:\
/.$shared_function
//
// void MakeExpressionStatement();
//./

StatementExpression ::= MethodInvocation
\:$MakeExpressionStatement:\
/.$shared_function
//
// void MakeExpressionStatement();
//./

StatementExpression ::= ClassInstanceCreationExpression
\:$MakeExpressionStatement:\
/.$shared_function
//
// void MakeExpressionStatement();
//./

--
-- The use of Marker allows us to share code.
--
--IfThenStatement ::= 'if' '(' Expression ')' Statement
IfThenStatement ::= 'if' '(' Expression ')' Statement Marker Marker
\:$MakeIfThenElseStatement:\
/.$location./

IfThenElseStatement ::= 'if' '(' Expression ')' StatementNoShortIf
                        'else' Statement
\:$MakeIfThenElseStatement:\
/.$shared_function
//
// void MakeIfThenElseStatement();
//./

IfThenElseStatementNoShortIf ::= 'if' '(' Expression ')' StatementNoShortIf
                                 'else' StatementNoShortIf
\:$MakeIfThenElseStatement:\
/.$shared_function
//
// void MakeIfThenElseStatement();
//./

SwitchStatement ::= 'switch' '(' Expression ')' SwitchBlock
\:$consumeStatementSwitch:\
/.$location./

--
-- To avoid ambiguity with consecutive optional items, and to special
-- case trailing labels, we expand this inline.
--
--SwitchBlock ::= '{' SwitchBlockStatementsopt SwitchLabelsopt '}'
--
SwitchBlock ::= '{' SwitchBlockStatements SwitchLabelsopt '}'
\:$consumeSwitchBlockWithSwitchBlockStatements:\
/.$location./

SwitchBlock ::= '{' SwitchLabelsopt '}'
\:$consumeSwitchBlockOnlyWithSwitchLabelsopt:\
/.$location./

SwitchBlockStatements ::= SwitchBlockStatement
\:$StartList:\
/.$shared_StartList./

SwitchBlockStatements ::= SwitchBlockStatements SwitchBlockStatement
\:$AddList2:\
/.$shared_AddList2./

SwitchBlockStatement ::= SwitchLabels BlockStatements
\:$consumeSwitchBlockStatement:\
/.$location./

SwitchLabels ::= SwitchLabel
\:$StartList:\
/.$shared_StartList./

SwitchLabels ::= SwitchLabels SwitchLabel
\:$AddList2:\
/.$shared_AddList2./

SwitchLabelsopt ::= $empty
\:$NullAction:\
/.$shared_NullAction./

SwitchLabelsopt ::= SwitchLabels
\:$NoAction:\
/.$shared_NoAction./

--
-- Simplify.
--
--SwitchLabel ::= 'case' ConstantExpression ':'
SwitchLabel ::= 'case' Expression ':'
\:$MakeSwitchLabel:\
/.$location./

--
-- The use of Marker allows us to share code.
--
--SwitchLabel ::= 'default' ':'
SwitchLabel ::= 'default' Marker ':'
\:$MakeSwitchLabel:\
/.$location./

WhileStatement ::= 'while' '(' Expression ')' Statement
\:$MakeWhileStatement:\
/.$location./

WhileStatementNoShortIf ::= 'while' '(' Expression ')' StatementNoShortIf
\:$MakeWhileStatement:\
/.$location./

DoStatement ::= 'do' Statement 'while' '(' Expression ')' ';'
\:$consumeStatementDo:\
/.$location./

ForStatement ::= 'for' '(' ForInitopt ';' Expressionopt ';' ForUpdateopt ')'
                 Statement
\:$MakeForStatement:\
/.$location./

ForStatementNoShortIf ::= 'for' '(' ForInitopt ';' Expressionopt ';'
                          ForUpdateopt ')' StatementNoShortIf
\:$MakeForStatement:\
/.$location./

ForInit ::= StatementExpressionList
\:$NoAction:\
/.$shared_NoAction./

ForInit ::= LocalVariableDeclaration
\:$StartList:\
/.$shared_StartList./

ForInitopt ::= $empty
\:$NullAction:\
/.$shared_NullAction./

ForInitopt ::= ForInit
\:$NoAction:\
/.$shared_NoAction./

ForUpdate ::= StatementExpressionList
\:$NoAction:\
/.$shared_NoAction./

ForUpdateopt ::= $empty
\:$NullAction:\
/.$shared_NullAction./

ForUpdateopt ::= ForUpdate
\:$NoAction:\
/.$shared_NoAction./

StatementExpressionList ::= StatementExpression
\:$StartList:\
/.$shared_StartList./

StatementExpressionList ::= StatementExpressionList ',' StatementExpression
\:$AddList3:\
/.$shared_AddList3./

ForeachStatement ::= 'for' '(' FormalParameter ':' Expression ')' Statement
\:$MakeForeachStatement:\
/.$location./

ForeachStatementNoShortIf ::= 'for' '(' FormalParameter ':' Expression ')'
                              StatementNoShortIf
\:$MakeForeachStatement:\
/.$location./
--
-- Assert statements were added in JDK 1.4, as part of JSR 41.
-- The use of Marker allows us to share code.
--
--AssertStatement ::= 'assert' Expression ';'
AssertStatement ::= 'assert' Expression Marker Marker ';'
\:$MakeAssertStatement:\
/.$location./

AssertStatement ::= 'assert' Expression ':' Expression ';'
\:$MakeAssertStatement:\
/.$location./

BreakStatement ::= 'break' Identifieropt ';'
\:$consumeStatementBreak:\
/.$location./

ContinueStatement ::= 'continue' Identifieropt ';'
\:$consumeStatementContinue:\
/.$location./

ReturnStatement ::= 'return' Expressionopt ';'
\:$consumeStatementReturn:\
/.$location./

ThrowStatement ::= 'throw' Expression ';'
\:$consumeStatementThrow:\
/.$location./

SynchronizedStatement ::= 'synchronized' '(' Expression ')' Block
\:$consumeStatementSynchronized:\
/.$location./

--
-- The use of Marker allows us to share code.
--
--TryStatement ::= 'try' Block Catches
TryStatement ::= 'try' Block Catches Marker
\:$MakeTryStatement:\
/.$location./

TryStatement ::= 'try' Block Catchesopt Finally
\:$MakeTryStatement:\
/.$shared_function
//
// void MakeTryStatement();
//./

Catches ::= CatchClause
\:$StartList:\
/.$shared_StartList./

Catches ::= Catches CatchClause
\:$AddList2:\
/.$shared_AddList2./

Catchesopt ::= $empty
\:$NullAction:\
/.$shared_NullAction./

Catchesopt ::= Catches
\:$NoAction:\
/.$shared_NoAction./

CatchClause ::= 'catch' '(' FormalParameter ')' Block
\:$consumeStatementCatch:\
/.$location./

Finally ::= 'finally' Block
\:$consumeStatementFinally:\
/.$recovery_template vector<wstring>{L"finally", L"{", L"}" })); ./
/.$location./

--18.12 Productions from 14: Expressions

Primary ::= PrimaryNoNewArray
\:$NoAction:\
/.$shared_NoAction./

--1.2 feature
--
-- It is legal to access an element of an initialized array, as in
-- new int[] {0}[0]; this requires splitting the original rule for
-- array creation into two.
--
--Primary ::= ArrayCreationExpression
Primary ::= ArrayCreationUninitialized
\:$NoAction:\
/.$shared_NoAction./

Primary ::= ArrayCreationInitialized
\:$NoAction:\
/.$shared_NoAction./

PrimaryNoNewArray ::= Literal
\:$NoAction:\
/.$shared_NoAction./

PrimaryNoNewArray ::= 'this'
\:$consumePrimaryNoNewArrayThis:\
/.$location./

--
-- We split this into two rules to allow better parsing of parenthesized
-- expressions vs. casts.  All expressions have a dual *NotName form, so that
-- the decision of whether "(name)" starts a cast or is a primary does not
-- cause parsing ambiguities. The use of Marker allows us to share code.
-- Also note that splitting this rule aids in parsing generics.
--
--PrimaryNoNewArray ::= '(' Expression ')'
PrimaryNoNewArray ::= '(' Name Marker ')'
\:$MakeParenthesizedExpression:\
/.$location./

--
-- The use of Marker allows us to share code.
--
PrimaryNoNewArray ::= '(' ExpressionNotName Marker ')'
\:$MakeParenthesizedExpression:\
/.$location./


PrimaryNoNewArray ::= ClassInstanceCreationExpression
\:$NoAction:\
/.$shared_NoAction./

PrimaryNoNewArray ::= FieldAccess
\:$NoAction:\
/.$shared_NoAction./

--1.1 feature
--
-- Note that we had to rework this to avoid ambiguity
--
--PrimaryNoNewArray ::= ClassType '.' 'this'
PrimaryNoNewArray ::= Name '.' 'this'
\:$consumePrimaryNoNewArrayNameThis:\
/.$location./

--1.1 feature
--
-- Note that we had to rework this to avoid ambiguity.
--
--PrimaryNoNewArray ::= Type '.' 'class'
PrimaryNoNewArray ::= PrimitiveType Dimsopt '.' 'class'
\:$MakeClassLiteral:\
/.$location./

PrimaryNoNewArray ::= Name Dims '.' 'class'
\:$MakeClassLiteral:\
/.$location./

--
-- The use of Marker allows us to share code.
--
--PrimaryNoNewArray ::= Name '.' 'class'
PrimaryNoNewArray ::= Name '.' Marker 'class'
\:$MakeClassLiteral:\
/.$location./

--
-- The use of Marker allows us to share code.
--
--PrimaryNoNewArray ::= 'void' '.' 'class'
PrimaryNoNewArray ::= VoidType '.' Marker 'class'
\:$MakeClassLiteral:\
/.$location./

PrimaryNoNewArray ::= MethodInvocation
\:$NoAction:\
/.$shared_NoAction./

PrimaryNoNewArray ::= ArrayAccess
\:$NoAction:\
/.$shared_NoAction./

--1.1 feature
--
-- In Java 1.0 a ClassBody could not appear at all in a
-- ClassInstanceCreationExpression. Simplify.
--




--ClassInstanceCreationExpression ::= 'new' ClassOrInterfaceType '('
--                                    ArgumentListopt ')' ClassBodyopt
ClassInstanceCreationExpression ::= 'new' ClassOrInterfaceType Arguments  ClassBodyopt
                                    
\:$consumeClassInstanceCreationExpression:\
/.$location./

--
-- Parameterized types were added in JSR 14.
--
--ClassInstanceCreationExpression ::= 'new' ClassOrInterfaceType '('
--                                    ArgumentListopt ')' ClassBodyopt
ClassInstanceCreationExpression ::= 'new' TypeArguments ClassOrInterfaceType
                                    Arguments ClassBodyopt
\:$consumeClassInstanceCreationExpressionWithTypeArguments:\
/.$location./

--1.1 feature
--
-- Parameterized types were added in JSR 14.
--
--ClassInstanceCreationExpression ::= Primary '.' 'new' 'Identifier' '('
--                                    ArgumentListopt ')' ClassBodyopt
ClassInstanceCreationExpression ::= Primary '.' 'new' TypeArgumentsopt
                                    'Identifier' TypeArgumentsopt Arguments
                                    ClassBodyopt
\:$MakeQualifiedNew:\
/.$location./

--1.1 feature
--
-- Parameterized types were added in JSR 14.
--
--ClassInstanceCreationExpression ::= Name '.' 'new' 'Identifier' '('
--                                    ArgumentListopt ')' ClassBodyopt
ClassInstanceCreationExpression ::= Name '.' 'new' TypeArgumentsopt
                                    'Identifier' TypeArgumentsopt Arguments
                                    ClassBodyopt
\:$MakeQualifiedNew:\
/.$location./

ArgumentList ::= Expression
\:$StartList:\
/.$shared_StartList./

ArgumentList ::= ArgumentList ',' Expression
\:$AddList3:\
/.$shared_AddList3./

ArgumentListopt ::= $empty
\:$NullAction:\
/.$shared_NullAction./

ArgumentListopt ::= ArgumentList
\:$NoAction:\
/.$shared_NoAction./

--1.2 feature
--
-- ArrayCreationExpression is split into two parsing categories, to
-- allow array access on an initialized array.  See above.
--
--ArrayCreationExpression ::= 'new' PrimitiveType DimExprs Dimsopt
ArrayCreationUninitialized ::= 'new' PrimitiveType DimExprs Dimsopt
\:$MakeArrayCreationUninitialized:\
/.$location./

--ArrayCreationExpression ::= 'new' ClassOrInterfaceType DimExprs Dimsopt
ArrayCreationUninitialized ::= 'new' ClassOrInterfaceType DimExprs Dimsopt
\:$MakeArrayCreationUninitialized:\
/.$location./




--1.1 feature
--
--ArrayCreationExpression ::= 'new' ArrayType ArrayInitializer
ArrayCreationInitialized ::= 'new' PrimitiveType Dims ArrayInitializer
\:$MakeArrayCreationInitialized:\
/.$location./

ArrayCreationInitialized ::= 'new' ClassOrInterfaceType Dims ArrayInitializer
\:$MakeArrayCreationInitialized:\
/.$location./


DimExprs ::= DimExpr
\:$StartList:\
/.$shared_StartList./

DimExprs ::= DimExprs DimExpr
\:$AddList2:\
/.$shared_AddList2./

DimExpr ::= '[' Expression ']'
\:$consumeDimWithExpr:\
/.$location./

Dims ::= '[' ']'
\:$consumeDimWithOutExpr:\
/.$location./

Dims ::= Dims '[' ']'
\:$consumeDimensions:\
/.$location./

Dimsopt ::= $empty
\:$NullAction:\
/.$shared_NullAction./

Dimsopt ::= Dims
\:$NoAction:\
/.$shared_NoAction./

--
-- Added rule to make parsing 'super' '.' easier.
--
SuperAccess ::= 'super'
\:$consumeToken_super:\
/.$location./
--
-- Added rule to make parsing 'super' '.' easier. Technically, only ClassType
-- is allowed instead of Name, but that would be ambiguous with qualified
-- names. The use of Marker allows us to share code.
--
--SuperAccess ::= ClassType '.' 'super'
SuperAccess ::= Name '.' Marker 'super'
\:$consumeQuilifiedSuperAccess:\
/.$location./

--
-- The use of Marker allows us to share code.
--
--FieldAccess ::= Primary '.' 'Identifier'
FieldAccess ::= Primary '.' Marker 'Identifier'
\:$MakeFieldAccess:\
/.$recovery_template vector<wstring>{L"Identifier"})); ./
/.$location./

--
-- The use of Marker allows us to share code.  Likewise, SuperAccess
-- simplifies tree creation.
--
--FieldAccess ::= 'super' '.' 'Identifier'
--FieldAccess ::= ClassType '.' 'super' '.' 'Identifier'
FieldAccess ::= SuperAccess '.' Marker 'Identifier'
\:$MakeFieldAccess:\
/.$recovery_template vector<wstring>{L"Identifier"})); ./
/.$location./

--
-- Inline expand Name so we can distinguish the optional base from the required
-- method identifier.
--
--MethodInvocation ::= Name '(' ArgumentListopt ')'
MethodInvocation ::= 'Identifier' Arguments
\:$consumeMethodInvocationPrimary:\
/.$location./

--
-- Inline expand Name so we can distinguish the optional base from the required
-- method identifier.  The use of Marker allows us to share code.
--
--MethodInvocation ::= Name '(' ArgumentListopt ')'
MethodInvocation ::= Name '.' Marker 'Identifier' Arguments
\:$MakeMethodInvocation:\
/.$location./
--
-- Parameterized types were added in JSR 14.
--
MethodInvocation ::= Name '.' TypeArguments 'Identifier' Arguments
\:$MakeMethodInvocation:\
/.$location./

--
-- The use of Marker allows us to share code.
--
--MethodInvocation ::= Primary '.' 'Identifier' '(' ArgumentListopt ')'
MethodInvocation ::= Primary '.' Marker 'Identifier' Arguments
\:$MakeMethodInvocation:\
/.$location./

--
-- Parameterized types were added in JSR 14.
--
MethodInvocation ::= Primary '.' TypeArguments 'Identifier' Arguments
\:$MakeMethodInvocation:\
/.$location./

--
-- The use of Marker allows us to share code.  Likewise, SuperAccess
-- simplifies tree creation.
--
--MethodInvocation ::= 'super' '.' 'Identifier' '(' ArgumentListopt ')'
--MethodInvocation ::= ClassType '.' 'super' '.' 'Identifier' '('
--                     ArgumentListopt ')'
MethodInvocation ::= SuperAccess '.' Marker 'Identifier' Arguments
\:$MakeMethodInvocation:\
/.$location./

--
-- Parameterized types were added in JSR 14.  Likewise, SuperAccess
-- simplifies tree creation.
--
--MethodInvocation ::= 'super' '.' TypeArguments 'Identifier' '('
--                     ArgumentListopt ')'
--MethodInvocation ::= ClassType '.' 'super' '.' TypeArguments 'Identifier' '('
--                     ArgumentListopt ')'
MethodInvocation ::= SuperAccess '.' TypeArguments 'Identifier' Arguments
\:$MakeMethodInvocation:\
/.$location./

ArrayAccess ::= Name '[' Expression ']'
\:$MakeArrayAccess:\
/.$location./

ArrayAccess ::= PrimaryNoNewArray '[' Expression ']'
\:$MakeArrayAccess:\
/.$location./

--1.2 feature
--
-- Access of an initialized array is legal.  See above.
--
ArrayAccess ::= ArrayCreationInitialized '[' Expression ']'
\:$MakeArrayAccess:\
/.$location./

PostfixExpression ::= Primary
\:$NoAction:\
/.$shared_NoAction./

PostfixExpression ::= Name
\:$NoAction:\
/.$shared_NoAction./

PostfixExpression ::= PostIncrementExpression
\:$NoAction:\
/.$shared_NoAction./

PostfixExpression ::= PostDecrementExpression
\:$NoAction:\
/.$shared_NoAction./

PostfixExpressionNotName ::= Primary
\:$NoAction:\
/.$shared_NoAction./

PostfixExpressionNotName ::= PostIncrementExpression
\:$NoAction:\
/.$shared_NoAction./

PostfixExpressionNotName ::= PostDecrementExpression
\:$NoAction:\
/.$shared_NoAction./

PostIncrementExpression ::= PostfixExpression '++'
\:$consumeUnaryExpressionPLUSPLUS:\
/.$location./

PostDecrementExpression ::= PostfixExpression '--'
\:$consumeUnaryExpressionMINUSMINUS:\
/.$location./

UnaryExpression ::= PreIncrementExpression
\:$NoAction:\
/.$shared_NoAction./

UnaryExpression ::= PreDecrementExpression
\:$NoAction:\
/.$shared_NoAction./

UnaryExpression ::= '+' UnaryExpression
\:$MakePreUnaryExpression:\
/.$location./

UnaryExpression ::= '-' UnaryExpression
\:$MakePreUnaryExpression:\
/.$shared_Unary./

UnaryExpression ::= UnaryExpressionNotPlusMinus
\:$NoAction:\
/.$shared_NoAction./

UnaryExpressionNotName ::= PreIncrementExpression
\:$NoAction:\
/.$shared_NoAction./

UnaryExpressionNotName ::= PreDecrementExpression
\:$NoAction:\
/.$shared_NoAction./

UnaryExpressionNotName ::= '+' UnaryExpression
\:$MakePreUnaryExpression:\
/.$shared_Unary./

UnaryExpressionNotName ::= '-' UnaryExpression
\:$MakePreUnaryExpression:\
/.$shared_Unary./

UnaryExpressionNotName ::= UnaryExpressionNotPlusMinusNotName
\:$NoAction:\
/.$shared_NoAction./

PreIncrementExpression ::= '++' UnaryExpression
\:$MakePreUnaryExpression:\
/.$shared_Unary./

PreDecrementExpression ::= '--' UnaryExpression
\:$MakePreUnaryExpression:\
/.$shared_Unary./

UnaryExpressionNotPlusMinus ::= PostfixExpression
\:$NoAction:\
/.$shared_NoAction./

UnaryExpressionNotPlusMinus ::= '~' UnaryExpression
\:$MakePreUnaryExpression:\
/.$shared_Unary./

UnaryExpressionNotPlusMinus ::= '!' UnaryExpression
\:$MakePreUnaryExpression:\
/.$shared_Unary./

UnaryExpressionNotPlusMinus ::= CastExpression
\:$NoAction:\
/.$shared_NoAction./

UnaryExpressionNotPlusMinusNotName ::= PostfixExpressionNotName
\:$NoAction:\
/.$shared_NoAction./

UnaryExpressionNotPlusMinusNotName ::= '~' UnaryExpression
\:$MakePreUnaryExpression:\
/.$shared_Unary./

UnaryExpressionNotPlusMinusNotName ::= '!' UnaryExpression
\:$MakePreUnaryExpression:\
/.$shared_Unary./

UnaryExpressionNotPlusMinusNotName ::= CastExpression
\:$NoAction:\
/.$shared_NoAction./

--
-- Due to grammar ambiguities, we must rewrite this (otherwise, it is not
-- obvious whether "(a<b" starts a parenthesized expression or a cast). Note
-- that our rewrite guarantees that the contents of the parenthesis will
-- syntactically be a type, based on the way we factored parenthesized
-- expressions in Primary.
--
-- JLS2 15 lists:
--CastExpression ::= '(' PrimitiveType ')' UnaryExpression
--CastExpression ::= '(' ReferenceType ')' UnaryExpressionNotPlusMinus
-- JLS1 suggests:
--CastExpression ::= '(' PrimitiveType Dimsopt ')' UnaryExpression
--CastExpression ::= '(' Expression ')' UnaryExpressionNotPlusMinus
--CastExpression ::= '(' Name Dims ')' UnaryExpressionNotPlusMinus
--
CastExpression ::= '(' PrimitiveType Dimsopt ')' UnaryExpression
\:$MakeCastExpression:\
/.$location./

--
-- The use of Marker allows us to share code.
--
CastExpression ::= '(' Name Marker ')' UnaryExpressionNotPlusMinus
\:$MakeCastExpression:\
/.$location./

CastExpression ::= '(' Name Dims ')' UnaryExpressionNotPlusMinus
\:$MakeCastExpression:\
/.$location./

--
-- Parameterized types were added in JSR 14.
--
CastExpression ::= '(' Name '<' TypeArgumentList1 Dimsopt ')'
                   UnaryExpressionNotPlusMinus
\:$consumeCastExpressionTypeArgumentList1Dimsopt:\
/.$location./

--
-- Parameterized types were added in JSR 14.
--
CastExpression ::= '(' Name '<' TypeArgumentList1 '.' ClassOrInterfaceType
                   Dimsopt ')' UnaryExpressionNotPlusMinus
\:$consumeCastExpressionTypeArgumentList1DimsoptQualified:\
/.$location./

MultiplicativeExpression ::= UnaryExpression
\:$NoAction:\
/.$shared_NoAction./

MultiplicativeExpression ::= MultiplicativeExpression '*' UnaryExpression
\:$MakeBinaryExpression:\
/.$location./

MultiplicativeExpression ::= MultiplicativeExpression '/' UnaryExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

MultiplicativeExpression ::= MultiplicativeExpression '%' UnaryExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

MultiplicativeExpressionNotName ::= UnaryExpressionNotName
\:$NoAction:\
/.$shared_NoAction./

MultiplicativeExpressionNotName ::= MultiplicativeExpressionNotName '*'
                                    UnaryExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

MultiplicativeExpressionNotName ::= Name '*' UnaryExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

MultiplicativeExpressionNotName ::= MultiplicativeExpressionNotName '/'
                                    UnaryExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

MultiplicativeExpressionNotName ::= Name '/' UnaryExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

MultiplicativeExpressionNotName ::= MultiplicativeExpressionNotName '%'
                                    UnaryExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

MultiplicativeExpressionNotName ::= Name '%' UnaryExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

AdditiveExpression ::= MultiplicativeExpression
\:$NoAction:\
/.$shared_NoAction./

AdditiveExpression ::= AdditiveExpression '+' MultiplicativeExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

AdditiveExpression ::= AdditiveExpression '-' MultiplicativeExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

AdditiveExpressionNotName ::= MultiplicativeExpressionNotName
\:$NoAction:\
/.$shared_NoAction./

AdditiveExpressionNotName ::= AdditiveExpressionNotName '+'
                              MultiplicativeExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

AdditiveExpressionNotName ::= Name '+' MultiplicativeExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

AdditiveExpressionNotName ::= AdditiveExpressionNotName '-'
                              MultiplicativeExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

AdditiveExpressionNotName ::= Name '-' MultiplicativeExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

ShiftExpression ::= AdditiveExpression
\:$NoAction:\
/.$shared_NoAction./

ShiftExpression ::= ShiftExpression '<<' AdditiveExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

ShiftExpression ::= ShiftExpression '>>' AdditiveExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

ShiftExpression ::= ShiftExpression '>>>' AdditiveExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

ShiftExpressionNotName ::= AdditiveExpressionNotName
\:$NoAction:\
/.$shared_NoAction./

ShiftExpressionNotName ::= ShiftExpressionNotName '<<' AdditiveExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

ShiftExpressionNotName ::= Name '<<' AdditiveExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

ShiftExpressionNotName ::= ShiftExpressionNotName '>>' AdditiveExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

ShiftExpressionNotName ::= Name '>>' AdditiveExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

ShiftExpressionNotName ::= ShiftExpressionNotName '>>>' AdditiveExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

ShiftExpressionNotName ::= Name '>>>' AdditiveExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

RelationalExpression ::= ShiftExpression
\:$NoAction:\
/.$shared_NoAction./

--
-- Relational expressions do not operate on boolean. Rewriting this
-- rule avoids an ambiguity in generics with no semantic penalty. The
-- alternative is to lower the precedence of instanceof.
--
--RelationalExpression ::= RelationalExpression '<' ShiftExpression
RelationalExpression ::= ShiftExpression '<' ShiftExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

RelationalExpression ::= RelationalExpression '>' ShiftExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

RelationalExpression ::= RelationalExpression '<=' ShiftExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

RelationalExpression ::= RelationalExpression '>=' ShiftExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

RelationalExpression ::= RelationalExpression 'instanceof' ReferenceType
\:$MakeInstanceofExpression:\
/.$location./

RelationalExpressionNotName ::= ShiftExpressionNotName
\:$NoAction:\
/.$shared_NoAction./

--RelationalExpressionNotName ::= RelationalExpressionNotName '<'
--                                ShiftExpression
RelationalExpressionNotName ::= ShiftExpressionNotName '<' ShiftExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

RelationalExpressionNotName ::= Name '<' ShiftExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

--RelationalExpressionNotName ::= RelationalExpressionNotName '>'
--                                ShiftExpression
RelationalExpressionNotName ::= ShiftExpressionNotName '>' ShiftExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

RelationalExpressionNotName ::= Name '>' ShiftExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

RelationalExpressionNotName ::= RelationalExpressionNotName '<='
                                ShiftExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

RelationalExpressionNotName ::= Name '<=' ShiftExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

RelationalExpressionNotName ::= RelationalExpressionNotName '>='
                                ShiftExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

RelationalExpressionNotName ::= Name '>=' ShiftExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

RelationalExpressionNotName ::= RelationalExpressionNotName 'instanceof'
                                ReferenceType
\:$MakeInstanceofExpression:\
/.$location./

RelationalExpressionNotName ::= Name 'instanceof' ReferenceType
\:$MakeInstanceofExpression:\
/.$location./

EqualityExpression ::= RelationalExpression
\:$NoAction:\
/.$shared_NoAction./

EqualityExpression ::= EqualityExpression '==' RelationalExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

EqualityExpression ::= EqualityExpression '!=' RelationalExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

EqualityExpressionNotName ::= RelationalExpressionNotName
\:$NoAction:\
/.$shared_NoAction./

EqualityExpressionNotName ::= EqualityExpressionNotName '=='
                              RelationalExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

EqualityExpressionNotName ::= Name '==' RelationalExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

EqualityExpressionNotName ::= EqualityExpressionNotName '!='
                              RelationalExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

EqualityExpressionNotName ::= Name '!=' RelationalExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

AndExpression ::= EqualityExpression
\:$NoAction:\
/.$shared_NoAction./

AndExpression ::= AndExpression '&' EqualityExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

AndExpressionNotName ::= EqualityExpressionNotName
\:$NoAction:\
/.$shared_NoAction./

AndExpressionNotName ::= AndExpressionNotName '&' EqualityExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

AndExpressionNotName ::= Name '&' EqualityExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

ExclusiveOrExpression ::= AndExpression
\:$NoAction:\
/.$shared_NoAction./

ExclusiveOrExpression ::= ExclusiveOrExpression '^' AndExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

ExclusiveOrExpressionNotName ::= AndExpressionNotName
\:$NoAction:\
/.$shared_NoAction./

ExclusiveOrExpressionNotName ::= ExclusiveOrExpressionNotName '^' AndExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

ExclusiveOrExpressionNotName ::= Name '^' AndExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

InclusiveOrExpression ::= ExclusiveOrExpression
\:$NoAction:\
/.$shared_NoAction./

InclusiveOrExpression ::= InclusiveOrExpression '|' ExclusiveOrExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

InclusiveOrExpressionNotName ::= ExclusiveOrExpressionNotName
\:$NoAction:\
/.$shared_NoAction./

InclusiveOrExpressionNotName ::= InclusiveOrExpressionNotName '|'
                                 ExclusiveOrExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

InclusiveOrExpressionNotName ::= Name '|' ExclusiveOrExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

ConditionalAndExpression ::= InclusiveOrExpression
\:$NoAction:\
/.$shared_NoAction./

ConditionalAndExpression ::= ConditionalAndExpression '&&'
                             InclusiveOrExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

ConditionalAndExpressionNotName ::= InclusiveOrExpressionNotName
\:$NoAction:\
/.$shared_NoAction./

ConditionalAndExpressionNotName ::= ConditionalAndExpressionNotName '&&'
                                    InclusiveOrExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

ConditionalAndExpressionNotName ::= Name '&&' InclusiveOrExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

ConditionalOrExpression ::= ConditionalAndExpression
\:$NoAction:\
/.$shared_NoAction./

ConditionalOrExpression ::= ConditionalOrExpression '||'
                            ConditionalAndExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

ConditionalOrExpressionNotName ::= ConditionalAndExpressionNotName
\:$NoAction:\
/.$shared_NoAction./

ConditionalOrExpressionNotName ::= ConditionalOrExpressionNotName '||'
                                   ConditionalAndExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

ConditionalOrExpressionNotName ::= Name '||' ConditionalAndExpression
\:$MakeBinaryExpression:\
/.$shared_Binary./

ConditionalExpression ::= ConditionalOrExpression
\:$NoAction:\
/.$shared_NoAction./

ConditionalExpression ::= ConditionalOrExpression '?' Expression ':'
                          ConditionalExpression
\:$MakeConditionalExpression:\
/.$location./

ConditionalExpressionNotName ::= ConditionalOrExpressionNotName
\:$NoAction:\
/.$shared_NoAction./

ConditionalExpressionNotName ::= ConditionalOrExpressionNotName '?' Expression
                                 ':' ConditionalExpression
\:$MakeConditionalExpression:\
/.$location./


ConditionalExpressionNotName ::= Name '?' Expression ':' ConditionalExpression
\:$MakeConditionalExpression:\
/.$location./


AssignmentExpression ::= ConditionalExpression
\:$NoAction:\
/.$shared_NoAction./

AssignmentExpression ::= Assignment
\:$NoAction:\
/.$recovery_template vector<wstring>{L"Identifier"})); ./
/.$location ./
/.$shared_NoAction./

AssignmentExpressionNotName ::= ConditionalExpressionNotName
\:$NoAction:\
/.$shared_NoAction./

AssignmentExpressionNotName ::= Assignment
\:$NoAction:\
/.$shared_NoAction./

--
-- The original grammar uses LeftHandSide, instead of PostfixExpression.
-- However, parenthesized variables were added in JLS 2, and the
-- grammar is ambiguous unless we include all non-assignment
-- expressions. The semantic pass will filter out bad left-hand sides.
--
--Assignment ::= LeftHandSide AssignmentOperator AssignmentExpression
Assignment ::= PostfixExpression AssignmentOperator AssignmentExpression
\:$consumeAssignment:\
/.$location./


--
-- See comments above for Assignment - LeftHandSide is now a useless rule.
--
--LeftHandSide -> Name
--LeftHandSide -> FieldAccess
--LeftHandSide -> ArrayAccess

AssignmentOperator ::= '='
\:$NoAction:\
/.$shared_NoAction./

AssignmentOperator ::= '*='
\:$NoAction:\
/.$shared_NoAction./

AssignmentOperator ::= '/='
\:$NoAction:\
/.$shared_NoAction./

AssignmentOperator ::= '%='
\:$NoAction:\
/.$shared_NoAction./

AssignmentOperator ::= '+='
\:$NoAction:\
/.$shared_NoAction./

AssignmentOperator ::= '-='
\:$NoAction:\
/.$shared_NoAction./

AssignmentOperator ::= '<<='
\:$NoAction:\
/.$shared_NoAction./

AssignmentOperator ::= '>>='
\:$NoAction:\
/.$shared_NoAction./

AssignmentOperator ::= '>>>='
\:$NoAction:\
/.$shared_NoAction./

AssignmentOperator ::= '&='
\:$NoAction:\
/.$shared_NoAction./

AssignmentOperator ::= '^='
\:$NoAction:\
/.$shared_NoAction./

AssignmentOperator ::= '|='
\:$NoAction:\
/.$recovery_template vector<wstring>{L"="})); ./
/.$location ./
/.$shared_NoAction./

Expression ::= AssignmentExpression
\:$NoAction:\
/.$recovery_template vector<wstring>{L"Identifier"})); ./
/.$location ./
/.$shared_NoAction./

Expressionopt ::= $empty
\:$NullAction:\
/.$shared_NullAction./

Expressionopt ::= Expression
\:$NoAction:\
/.$shared_NoAction./

ExpressionNotName ::= AssignmentExpressionNotName
\:$NoAction:\
/.$shared_NoAction./

--
-- Simplify the syntax tree.
--
--ConstantExpression ::= Expression

-----------------------------------------------------------------------------
--
-- The following rules are for optional and helper productions.
--
-----------------------------------------------------------------------------

--
-- This rule exists solely to put NULL on the symbol stack, allowing us to
-- share productions that differ by the presence or absence of a rule.
--
Marker ::= $empty
\:$NullAction:\
/.$shared_NullAction./

,opt ::= $empty
\:$NoAction:\
/.$shared_NoAction./

,opt ::= ','
\:$NoAction:\
/.$shared_NoAction./

Identifieropt ::= $empty
\:$NoAction:\
/.$shared_NoAction./

Identifieropt ::= 'Identifier'
\:$NoAction:\
/.$shared_NoAction./

PackageHeaderMarker ::= $empty
\:$consumePackageHeaderMarker:\
/.$location./


MethodHeaderMarker ::= $empty
\:$consumeMethodHeaderMarker:\
/.$location./


-----------------------------------------------------------------------------
--
-- These rules add generics. Also search for JSR 14 in the comments above.
--
-----------------------------------------------------------------------------

--
-- Parameterized types were added in JSR 14.
--
TypeArguments ::= '<' TypeArgumentList1
\:$SetSym1ToSym2:\
/.$location./


--
-- Parameterized types were added in JSR 14.
--
TypeArgumentsopt ::= $empty
\:$NullAction:\
/.$shared_NullAction./

--
-- Parameterized types were added in JSR 14.
--
TypeArgumentsopt ::= TypeArguments
\:$NoAction:\
/.$shared_NoAction./

--
-- Parameterized types were added in JSR 14. We inline expanded WildcardBounds
-- for simplicity. The use of Marker allows us to share code.
--
--Wildcard ::= '?' WildcardBoundsopt
--WildcardBounds ::= extends ReferenceType
--WildcardBounds ::= super ReferenceType
--
Wildcard ::= '?' Marker Marker Marker
\:$MakeWildcard:\
/.$location./


--
-- Parameterized types were added in JSR 14.
-- The use of Marker allows us to share code.
--
Wildcard ::= '?' 'extends' Marker ReferenceType
\:$MakeWildcard:\
/.$location./


--
-- Parameterized types were added in JSR 14.
-- The use of Marker allows us to share code.
--
Wildcard ::= '?' Marker 'super' ReferenceType
\:$MakeWildcard:\
/.$location./

--
-- Parameterized types were added in JSR 14.
-- The use of Marker allows us to share code.
--
Wildcard1 ::= '?' Marker Marker '>'
\:$MakeWildcard:\
/.$location./


--
-- Parameterized types were added in JSR 14.
-- The use of Marker allows us to share code.
--
Wildcard1 ::= '?' 'extends' Marker ReferenceType1
\:$MakeWildcard:\
/.$location./


--
-- Parameterized types were added in JSR 14.
-- The use of Marker allows us to share code.
--
Wildcard1 ::= '?' Marker 'super' ReferenceType1
\:$MakeWildcard:\
/.$location./


--
-- Parameterized types were added in JSR 14.
-- The use of Marker allows us to share code.
--
Wildcard2 ::= '?' Marker Marker '>>'
\:$MakeWildcard:\
/.$location./


--
-- Parameterized types were added in JSR 14.
-- The use of Marker allows us to share code.
--
Wildcard2 ::= '?' 'extends' Marker ReferenceType2
\:$MakeWildcard:\
/.$location./


--
-- Parameterized types were added in JSR 14.
-- The use of Marker allows us to share code.
--
Wildcard2 ::= '?' Marker 'super' ReferenceType2
\:$MakeWildcard:\
/.$location./


--
-- Parameterized types were added in JSR 14.
-- The use of Marker allows us to share code.
--
Wildcard3 ::= '?' Marker Marker '>>>'
\:$MakeWildcard:\
/.$location./


--
-- Parameterized types were added in JSR 14.
-- The use of Marker allows us to share code.
--
Wildcard3 ::= '?' 'extends' Marker ReferenceType3
\:$MakeWildcard:\
/.$location./


--
-- Parameterized types were added in JSR 14.
-- The use of Marker allows us to share code.
--
Wildcard3 ::= '?' Marker 'super' ReferenceType3
\:$MakeWildcard:\
/.$location./


--
-- Parameterized types were added in JSR 14.
--
TypeArgumentList ::= TypeArgument
\:$StartList:\
/.$shared_StartList./

--
-- Parameterized types were added in JSR 14.
--
TypeArgumentList ::= TypeArgumentList ',' TypeArgument
\:$AddList3:\
/.$shared_AddList3./

--
-- Parameterized types were added in JSR 14.
--
TypeArgumentList1 ::= TypeArgument1
\:$StartList:\
/.$shared_StartList./

--
-- Parameterized types were added in JSR 14.
--
TypeArgumentList1 ::= TypeArgumentList ',' TypeArgument1
\:$AddList3:\
/.$shared_AddList3./

--
-- Parameterized types were added in JSR 14.
--
TypeArgumentList2 ::= TypeArgument2
\:$StartList:\
/.$shared_StartList./

--
-- Parameterized types were added in JSR 14.
--
TypeArgumentList2 ::= TypeArgumentList ',' TypeArgument2
\:$AddList3:\
/.$shared_AddList3./

--
-- Parameterized types were added in JSR 14.
--
TypeArgumentList3 ::= TypeArgument3
\:$StartList:\
/.$shared_StartList./

--
-- Parameterized types were added in JSR 14.
--
TypeArgumentList3 ::= TypeArgumentList ',' TypeArgument3
\:$AddList3:\
/.$shared_AddList3./

--
-- Parameterized types were added in JSR 14.
--
TypeArgument ::= ReferenceType
\:$NoAction:\
/.$shared_NoAction./

--
-- Parameterized types were added in JSR 14.
--
TypeArgument ::= Wildcard
\:$NoAction:\
/.$shared_NoAction./

--
-- Parameterized types were added in JSR 14.
--
TypeArgument1 ::= ReferenceType1
\:$NoAction:\
/.$shared_NoAction./

--
-- Parameterized types were added in JSR 14.
--
TypeArgument1 ::= Wildcard1
\:$NoAction:\
/.$shared_NoAction./

--
-- Parameterized types were added in JSR 14.
--
TypeArgument2 ::= ReferenceType2
\:$NoAction:\
/.$shared_NoAction./

--
-- Parameterized types were added in JSR 14.
--
TypeArgument2 ::= Wildcard2
\:$NoAction:\
/.$shared_NoAction./

--
-- Parameterized types were added in JSR 14.
--
TypeArgument3 ::= ReferenceType3
\:$NoAction:\
/.$shared_NoAction./

--
-- Parameterized types were added in JSR 14.
--
TypeArgument3 ::= Wildcard3
\:$NoAction:\
/.$shared_NoAction./

--
-- Parameterized types were added in JSR 14.
--
ReferenceType1 ::= ReferenceType '>'
\:$NoAction:\
/.$shared_NoAction./

--
-- Parameterized types were added in JSR 14.
-- Use of Marker allows us to easily find the closing '>>'.
--
ReferenceType1 ::= ClassOrInterface '<' TypeArgumentList2 Marker
\:$MakeTypeArguments:\
/.$location./

--
-- Parameterized types were added in JSR 14.
--
ReferenceType2 ::= ReferenceType '>>'
\:$NoAction:\
/.$shared_NoAction./

--
-- Parameterized types were added in JSR 14.
-- Use of Marker allows us to easily find the closing '>>>'.
--
ReferenceType2 ::= ClassOrInterface '<' TypeArgumentList3 Marker
\:$MakeTypeArguments:\
/.$location./


--
-- Parameterized types were added in JSR 14.
--
ReferenceType3 ::= ReferenceType '>>>'
\:$NoAction:\
/.$shared_NoAction./

--
-- Parameterized types were added in JSR 14.
--
TypeParameters ::= '<' TypeParameterList1
\:$SetSym1ToSym2:\
/.$location./


--
-- Parameterized types were added in JSR 14.
--
TypeParametersopt ::= $empty
\:$NullAction:\
/.$shared_NullAction./

--
-- Parameterized types were added in JSR 14.
--
TypeParametersopt ::= TypeParameters
\:$NoAction:\
/.$shared_NoAction./

--
-- Parameterized types were added in JSR 14.
--
TypeParameterList ::= TypeParameter
\:$StartList:\
/.$shared_StartList./

--
-- Parameterized types were added in JSR 14.
--
TypeParameterList ::= TypeParameterList ',' TypeParameter
\:$AddList3:\
/.$shared_AddList3./

--
-- Parameterized types were added in JSR 14.
--
TypeParameterList1 ::= TypeParameter1
\:$StartList:\
/.$shared_StartList./

--
-- Parameterized types were added in JSR 14.
--
TypeParameterList1 ::= TypeParameterList ',' TypeParameter1
\:$AddList3:\
/.$shared_AddList3./

--
-- Parameterized types were added in JSR 14.
--
TypeParameter ::= 'Identifier' TypeBoundopt
\:$MakeTypeParameter:\
/.$location./


--
-- Parameterized types were added in JSR 14.
-- The use of Marker allows us to share code.
--
TypeParameter1 ::= 'Identifier' Marker '>'
\:$MakeTypeParameter:\
/.$location./


--
-- Parameterized types were added in JSR 14.
--
TypeParameter1 ::= 'Identifier' TypeBound1
\:$MakeTypeParameter:\
/.$location./


--
-- Parameterized types were added in JSR 14.
--
TypeBound ::= 'extends' ReferenceType AdditionalBoundListopt
\:$MakeTypeBound:\
/.$location./


--
-- Parameterized types were added in JSR 14.
--
TypeBoundopt ::= $empty
\:$NullAction:\
/.$shared_NullAction./

--
-- Parameterized types were added in JSR 14.
--
TypeBoundopt ::= TypeBound
\:$NoAction:\
/.$shared_NoAction./

--
-- Parameterized types were added in JSR 14.
-- The use of Marker allows us to share code.
--
TypeBound1 ::= 'extends' ReferenceType1 Marker
\:$MakeTypeBound:\
/.$location./


--
-- Parameterized types were added in JSR 14.
--
TypeBound1 ::= 'extends' ReferenceType AdditionalBoundList1
\:$MakeTypeBound:\
/.$location./


--
-- Parameterized types were added in JSR 14.
--
AdditionalBoundList ::= AdditionalBound
\:$StartList:\
/.$shared_StartList./

--
-- Parameterized types were added in JSR 14.
--
AdditionalBoundList ::= AdditionalBoundList AdditionalBound
\:$AddList2:\
/.$shared_AddList2./

--
-- Parameterized types were added in JSR 14.
--
AdditionalBoundListopt ::= $empty
\:$NullAction:\
/.$shared_NullAction./

--
-- Parameterized types were added in JSR 14.
--
AdditionalBoundListopt ::= AdditionalBoundList
\:$NoAction:\
/.$shared_NoAction./

--
-- Parameterized types were added in JSR 14.
--
AdditionalBoundList1 ::= AdditionalBound1
\:$StartList:\
/.$shared_StartList./

--
-- Parameterized types were added in JSR 14.
--
AdditionalBoundList1 ::= AdditionalBoundList AdditionalBound1
\:$AddList2:\
/.$shared_AddList2./

--
-- Parameterized types were added in JSR 14.
--
AdditionalBound ::= '&' ClassOrInterfaceType
\:$SetSym1ToSym2:\
/.$location./


--
-- Parameterized types were added in JSR 14.
--
AdditionalBound1 ::= '&' ClassOrInterfaceType1
\:$SetSym1ToSym2:\
/.$location./


--
-- Parameterized types were added in JSR 14.
--
ClassOrInterfaceType1 ::= ClassOrInterfaceType '>'
\:$NoAction:\
/.$shared_NoAction./

--
-- Parameterized types were added in JSR 14.
-- Use of Marker allows us to easily find the closing '>>'.
--
ClassOrInterfaceType1 ::= ClassOrInterface '<' TypeArgumentList2 Marker
\:$MakeTypeArguments:\
/.$location./




------ Finish off the files

--  for javaact.cpp
/.
		 computeName(name);

		 computeReverseTable(
			 javaprs_table::terminal_index, 
			 Parser::terminal_index_length,
			 javaprs_table::non_terminal_index,
			 Parser::non_terminal_index_length,
			 Parser::name, 
			 Parser::reverse_index);


		 buildFilesForStatementsRecoveryFilter(
			 javaprs_table::non_terminal_index,
			 Parser::non_terminal_index_length,
			 javaprs_table::lhs,
			 Parser::tokens_for_statements_recovery_filter,
			 Parser::statements_recovery_filter
			 );

		 buildFilesForRecoveryTemplates(
			 javaprs_table::terminal_index,
			 Parser::terminal_index_length,
			 javaprs_table::non_terminal_index,
			 Parser::non_terminal_index_length,
			 Parser::name,
			 javaprs_table::lhs,
			 Parser::tokens_for_recovery_templates,
			 Parser::recovery_templates_index,
			 Parser::recovery_templates
			 );

		 return true;

}// end

} // Close namespace Jikes block


./



--  for consumeRule.cpp
\:

    return;
}


} // Close namespace Jikes block




:\




-- Names allow diagnose.cpp debug output to be more legible
$names

BodyMarker ::= '"class Identifier { ... MethodHeader "'

PLUS_PLUS ::= '++'
MINUS_MINUS ::= '--'
EQUAL_EQUAL ::= '=='
LESS_EQUAL ::= '<='
GREATER_EQUAL ::= '>='
NOT_EQUAL ::= '!='
LEFT_SHIFT ::= '<<'
RIGHT_SHIFT ::= '>>'
UNSIGNED_RIGHT_SHIFT ::= '>>>'
PLUS_EQUAL ::= '+='
MINUS_EQUAL ::= '-='
MULTIPLY_EQUAL ::= '*='
DIVIDE_EQUAL ::= '/='
AND_EQUAL ::= '&='
OR_EQUAL ::= '|='
XOR_EQUAL ::= '^='
REMAINDER_EQUAL ::= '%='
LEFT_SHIFT_EQUAL ::= '<<='
RIGHT_SHIFT_EQUAL ::= '>>='
UNSIGNED_RIGHT_SHIFT_EQUAL ::= '>>>='
OR_OR ::= '||'
AND_AND ::= '&&'
PLUS ::= '+'
MINUS ::= '-'
NOT ::= '!'
REMAINDER ::= '%'
XOR ::= '^'
AND ::= '&'
MULTIPLY ::= '*'
OR ::= '|'
TWIDDLE ::= '~'
DIVIDE ::= '/'
GREATER ::= '>'
LESS ::= '<'
LPAREN ::= '('
RPAREN ::= ')'
LBRACE ::= '{'
RBRACE ::= '}'
LBRACKET ::= '['
RBRACKET ::= ']'
SEMICOLON ::= ';'
QUESTION ::= '?'
COLON ::= ':'
COMMA ::= ','
DOT ::= '.'
EQUAL ::= '='
AT ::= '@'
ELLIPSIS ::= '...'

StatementNoShortIf ::= 'Statement'
StatementWithoutTrailingSubstatement ::= 'Statement'
LabeledStatementNoShortIf ::= 'LabeledStatement'
IfThenElseStatementNoShortIf ::= 'IfThenElseStatement'
WhileStatementNoShortIf ::= 'WhileStatement'
ForStatementNoShortIf ::= 'ForStatement'
ForeachStatementNoShortIf ::= 'ForeachStatement'
UnaryExpressionNotPlusMinus ::= 'UnaryExpression'

PostfixExpressionNotName ::= 'PostfixExpression'
UnaryExpressionNotName ::= 'UnaryExpression'
UnaryExpressionNotPlusMinusNotName ::= 'UnaryExpression'
MultiplicativeExpressionNotName ::= 'MultiplicativeExpression'
AdditiveExpressionNotName ::= 'AdditiveExpression'
ShiftExpressionNotName ::= 'ShiftExpression'
RelationalExpressionNotName ::= 'RelationalExpression'
EqualityExpressionNotName ::= 'EqualityExpression'
AndExpressionNotName ::= 'AndExpression'
ExclusiveOrExpressionNotName ::= 'ExclusiveOrExpression'
InclusiveOrExpressionNotName ::= 'InclusiveOrExpression'
ConditionalAndExpressionNotName ::= 'ConditionalAndExpression'
ConditionalOrExpressionNotName ::= 'ConditionalOrExpression'
ConditionalExpressionNotName ::= 'ConditionalExpression'
AssignmentExpressionNotName ::= 'AssignmentExpression'
ExpressionNotName ::= 'Expression'

Wildcard1 ::= 'Wildcard'
Wildcard2 ::= 'Wildcard'
Wildcard3 ::= 'Wildcard'
TypeArgumentList1 ::= 'TypeArgumentList'
TypeArgumentList2 ::= 'TypeArgumentList'
TypeArgumentList3 ::= 'TypeArgumentList'
TypeArgument1 ::= 'TypeArgument'
TypeArgument2 ::= 'TypeArgument'
TypeArgument3 ::= 'TypeArgument'
ReferenceType1 ::= 'ReferenceType'
ReferenceType2 ::= 'ReferenceType'
ReferenceType3 ::= 'ReferenceType'
TypeParameterList1 ::= 'TypeParameterList'
TypeParameter1 ::= 'TypeParameter'
TypeBound1 ::= 'TypeBound'
AdditionalBoundList1 ::= 'AdditionalBoundList'
AdditionalBound1 ::= 'AdditionalBound'
ClassOrInterfaceType1 ::= 'ClassOrInterfaceType'

$end
