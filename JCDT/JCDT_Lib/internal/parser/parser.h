// $Id: JCDT_Lib/internal/parser/parser.h,v 1.28 2004/03/25 13:32:28 ericb Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 2004 IBM Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#ifndef parser_INCLUDED
#define parser_INCLUDED

#include "JCDT_Lib/internal/env/platform.h"
#include "LPG/lpginput.h"
#include "JCDT_Lib/internal/lex/Token.h"
#include "JCDT_Lib/internal/lex/LexStream.h"
#include <JCDT_Lib/internal/problem/ParseError.h>
#include "SecondaryRepairInfo.h"

#include <vector>

namespace Jikes {
	class Initializer;
	class ConstructorDeclaration;
	class MethodDeclaration;
	// Open namespace Jikes block



class AstStoragePool;
class Ast;
class AstListNode;
class ReachComleteAbleStatement;
class AstBlock;
class AstName;
class AstType;
class AstTypeArguments;
class AstTypeName;
class AstModifiers;
class AstPackageDeclaration;
class CompilationUnitDeclaration;
class AstClassBody;
class AstTypeParameters;
class AstMethodBody;
class ReferenceContext;
class Expression;

class Parser : public javaprs_table
{
public:
	bool  has_localType_inScope = false;
	vector<int> nestedMethod; //the ptr is nestedType
	int nestedType;
	static vector< std::wstring > name;
	static vector<int> reverse_index ;
	static vector<unsigned short> recovery_templates_index;
	static  wstring recovery_templates ;
	static vector<unsigned short> statements_recovery_filter ;

	static std::vector< std::pair<int, std::vector<std::wstring> > > tokens_for_recovery_templates;
	static std::vector< int > tokens_for_statements_recovery_filter;


	static const int   name_size;

	static const int  terminal_index_length;
	static const int  non_terminal_index_length;

	static const int  scope_rhs_length;

	static bool buildTable();

	static void   computeName(vector< std::wstring>& OUT _name);

	static void  buildFilesForStatementsRecoveryFilter(
		const unsigned short newNonTerminalIndex[],
		const int newNonTerminalIndexLength,
		const unsigned short newLhs[],
		OUT std::vector< int >& tokens,
		vector< unsigned short>& newStatementsRecoveryFilter);

	static void buildFilesForRecoveryTemplates(
		const unsigned short newTerminalIndex[],
		int newTerminalIndexLength,
		const unsigned short newNonTerminalIndex[],
		int newNonTerminalIndexLength,
		vector< std::wstring >& newName,
		const unsigned short newLhs[],
		std::vector< std::pair<int, std::vector<std::wstring> > >& tokens,
		OUT vector<unsigned short>&newRecoveyTemplatesIndex,
		OUT wstring& newRecoveyTemplates
		);


	static void computeReverseTable(
		const unsigned short newTerminalIndex[],
		int newTerminalIndexLength,
		const unsigned short newNonTerminalIndex[],
		int newNonTerminalIndexLength,
		vector< std::wstring>& newName,
		vector<int>& newReverseTable);

	ReferenceContext* referenceContext = nullptr;

	bool statementRecoveryActivated;
	bool methodRecoveryActivated;

	Parser(bool no_semantic_action_ = false);
	virtual ~Parser();
	
    AstPackageDeclaration* PackageHeaderParse(BaseLexStream*,
		AstStoragePool*);
	
     CompilationUnitDeclaration* HeaderParse(BaseLexStream* lex,
		 AstStoragePool* ast_pool_ = nullptr);

    bool InitializerParse(BaseLexStream*, AstClassBody*);

    bool BodyParse(BaseLexStream*, AstClassBody*);

	Expression* ExpressParse(BaseLexStream*,
		AstStoragePool*);
	AstMethodBody* ParseMethodBody(BaseLexStream*,
		AstStoragePool*);

	bool  parse(ReferenceContext* rc,Initializer* initializer);
	bool  parse(ReferenceContext* rc, ConstructorDeclaration* constructor_decl);
	bool  parse(ReferenceContext* rc, MethodDeclaration* method_decl);
	
protected:
	CompilationUnitDeclaration* CompilationUnitParse(BaseLexStream*, AstStoragePool*);
	
	
	
	int  getFirstToken() const;
	void goForCompilationUnit();
	void goForMethodBody();
	void goForGenericMethodDeclaration();
	void goForAnnotationTypeMemberDeclaration();
	void recoverStatements();
	void goForExpression();
	
	CompilationUnitDeclaration* compilationUnit = nullptr; /*the result from parse()*/
	bool no_semantic_action;
    TokenObject buffer[BUFF_SIZE];
    TokenObject end_token_index;
	TokenObject start_token_index;
	ParseError* error = nullptr;
	void initialize();
    bool InitializerClassBody(AstClassBody*);
    bool Body(AstClassBody*);
	AstMethodBody* ParseMethodBody(AstMethodBody* block);

    void parse();
	void (Parser::*rule_action[NUM_RULES + 1]) ();
    void consumeRule();
	void consumeRule(int act);
    //********************************************************************
    //
    // Given a rule of the form     A ::= x1 x2 ... xn     n > 0
    //
    // the function GetToken(i) yields the symbol xi, if xi is a terminal
    // or ti, if xi is a nonterminal that produced a string of the form
    // xi => ti w.
    //
    //********************************************************************
    inline Token * GetToken(int i) 
    {
        auto idx =  location_stack[state_stack_top + (i - 1)];
		return lex_stream->token_stream[idx];
    }

    //********************************************************************
    //
    // Given a rule of the form     A ::= x1 x2 ... xn     n > 0
    //
    // the function Sym(i) yields the AST subtree associated with symbol
    // xi. NOTE that if xi is a terminal, Sym(i) is undefined !
    //
    //********************************************************************
    inline Ast*& Sym(int i) { return parse_stack[state_stack_top + (i - 1)]; }

    //********************************************************************
    //
    // When a token is shifted, we also construct a null AST for
    // it.  This is necessary in case we encounter an error and need to
    // delete AST subtrees from the parse stack - those corresponding to
    // shifted tokens should also have a valid subtree.
    //
    //********************************************************************
    inline void TokenAction(TokenObject) { Sym(1) = NULL; }

	BaseLexStream* lex_stream;

    AstStoragePool* ast_pool;
    AstStoragePool* body_pool;
    AstStoragePool* list_node_pool;

    AstListNode* free_list_nodes;
    AstListNode* AllocateListNode();
    void FreeCircularList(AstListNode*);

    bool parse_header_only,
         parse_package_header_only;

    //
    // LOCATION_STACK is a stack that is "parallel" to
    // (STATE_)STACK that is used to keep
    // track of the location of the first token on which an action
    // was executed in the corresponding state.
    //
    Location* location_stack;
	int* locationStartStack;

    Ast** parse_stack;

    enum { STACK_INCREMENT = 256 };

    int stack_length;
    int state_stack_top;
    int* stack;

    int temp_stack_top;
    int* temp_stack;
	int firstToken;
	int lastAct;
    static inline int Min(int x, int y) { return x < y ? x : y; }
    static inline int Max(int x, int y) { return x > y ? x : y; }

    //void AllocateErrorStacks();
    virtual void ReallocateStacks();
    void ErrorRepair(TokenObject error_token);
    void RepairParse(TokenObject);
    SecondaryRepairInfo ErrorSurgery(int stack[],
                                     int stack_top,
                                     int last_index,
                                     SecondaryRepairInfo repair);
    int ParseCheck(int stack[], int stack_top, int first_token,
                   int buffer_position);


public:
#define HEADERS
	AstTypeName* MakeTypeArguments(int tokennum);
	AstType* MakeArrayType(int tokennum);
	AstName* MakeSimpleName(int tokennum);
	AstModifiers* MakeModifiers();
	AstTypeParameters* MakeTypeParameters(int tokennum);
	AstTypeArguments* MakeExplicitTypeArguments(int tokennum);
	AstBlock* MakeBlock(int tokennum);
	ReachComleteAbleStatement* MakeSwitchBlockStatement(AstListNode* labels,
		AstListNode* statements = NULL);
	void MakeCastExpression(AstType* type, int tokennum);
	void BadAction();
	void NoAction();
	void NullAction();
	void SetSym1ToSym2();
	void StartList();
	void AddList2();
	void AddList3();
	void MakeTypeArguments();
	void MakeArrayType();
	void MakeCompilationUnit();
	void MakePackageDeclaration();
	void MakeImportDeclaration();
	void MakeModifier();
	void MakeAnnotation();
	void MakeArrayInitializer();
	void MakeClassDeclaration();
	void MakeClassBody();
	void MakeMethodDeclaration();
	void MakeFieldDeclaration();
	void MakeMethodHeader();
	void MakeMethodDeclarator();
	void MakeFormalParameter();
	void MakeInitializerDeclaration();
	void MakeConstructorDeclaration();
	void MakeQualifiedSuper();
	void MakeEnumDeclaration();
	void MakeEnumBody();
	void MakeInterfaceDeclaration();
	void MakeAnnotationTypeDeclaration();
	void MakeAnnotationTypeMemberDeclaration();
	void MakeLocalVariable();
	void MakeLabeledStatement();
	void MakeExpressionStatement();
	void MakeIfThenElseStatement();
	void MakeSwitchLabel();
	void MakeWhileStatement();
	void MakeForStatement();
	void MakeForeachStatement();
	void MakeAssertStatement();
	void MakeTryStatement();
	void MakeParenthesizedExpression();
	void MakeClassLiteral();
	void MakeQualifiedNew();
	void MakeArrayCreationUninitialized();
	void MakeArrayCreationInitialized();
	void MakeFieldAccess();
	void MakeMethodInvocation();
	void MakeArrayAccess();
	void MakePreUnaryExpression();
	void MakeCastExpression();
	void MakeBinaryExpression();
	void MakeInstanceofExpression();
	void MakeConditionalExpression();
	void MakeWildcard();
	void MakeTypeParameter();
	void MakeTypeBound();



	void consumeIntegerLiteral();
	void consumeLongLiteral();
	void consumeFloatLiteral();
	void consumeDoubleLiteral();
	void consumeCharacterLiteral();
	void consumeStringLiteral();
	void consumeNullLiteral();
	void consumeBooleanTrueLiteral();
	void consumeBooleanFalseLiteral();
	void consumeBooleanLiteral();

	void consumeToken_byte();
	void consumeToken_short();
	void consumeToken_int();
	void consumeToken_long();
	void consumeToken_char();
	void consumeToken_float();
	void consumeToken_double();
	void consumeToken_void();
	void consumeTypeDeclarationsopt();


	void consumeEnumConstantWithClassBodyOpt();
	void consumeArguments();
	void consumeConstantDeclaration();
	void consumeBlock();
	void consumeInvalidClassDeclaration();
	void consumeInvalidEnumDeclaration();
	void consumeLocalVariableDeclarationStatement();
	void consumeEmptyStatement();
	void consumeExpressionStatement();
	void consumeStatementSwitch();
	void consumeSwitchBlockWithSwitchBlockStatements();
	void consumeSwitchBlockOnlyWithSwitchLabelsopt();
	void consumeSwitchBlockStatement();
	void consumeStatementDo();
	void consumeStatementBreak();
	void consumeStatementContinue();
	void consumeStatementReturn();
	void consumeStatementThrow();
	void consumeStatementSynchronized();
	void consumeStatementCatch();
	void consumeStatementFinally();
	void consumePrimaryNoNewArrayThis();
	void consumePrimaryNoNewArrayNameThis();
	void consumeClassInstanceCreationExpression();
	void consumeClassInstanceCreationExpressionWithTypeArguments();
	void consumeDimWithExpr();
	void consumeDimWithOutExpr();
	void consumeDimensions();
	void consumeToken_super();
	void consumeQuilifiedSuperAccess();
	void consumeMethodInvocationPrimary();
	void consumeUnaryExpressionPLUSPLUS();
	void consumeUnaryExpressionMINUSMINUS();
	void consumeCastExpressionTypeArgumentList1Dimsopt();
	void consumeCastExpressionTypeArgumentList1DimsoptQualified();
	void consumeAssignment();
	void consumePackageHeaderMarker();
	void consumeMethodHeaderMarker();


	void consumeClassOrInterfaceName();
	void consumeClassOrInterfaceTypeArgumentList1Name();
	void consumeClassOrInterfaceTypeArgumentList1NameWithDims();
	void consumeClassOrInterfaceTypeArgumentList1WithDims();
	void consumeSimpleName();
	void consumeQualifiedName();
	void consumeEmptyTypeDeclaration();
	void consumeSingleMemberAnnotation();
	void consumeVariableDeclaratorOnlyId();
	void consumeVariableDeclaratorWithVariableInitializer();
	void consumeVariableDeclaratorId();
	void consumeMethodBody();
	void consumeExplicitConstructorThisCallInvocation();
	void consumeExplicitConstructorThisCallInvocationWithTypeArguments();
	void consumeExplicitConstructorSuperCallInvocation();
	void consumeExplicitConstructorSuperCallInvocationWithTypeArguments();
	void consumeMemberValuePair();



};

 inline int Parser::getFirstToken() const {
	return firstToken;
}
 inline void Parser::goForCompilationUnit() {
	 //tells the scanner to go for compilation unit parsing
	 this->firstToken = TK_CompilationUnitMarker;
 }

 inline void Parser::goForMethodBody() {
	 //tells the scanner to go for method body parsing
	 this->firstToken = TK_BodyMarker;
 }
 inline void Parser::goForGenericMethodDeclaration() {
	 //tells the scanner to go for method body parsing
	 this->firstToken = TK_MethodDeclarationMarker;
 }
 inline void Parser::goForAnnotationTypeMemberDeclaration() {
	 //tells the scanner to go for method body parsing
	 this->firstToken = TK_AnnotationTypeMemberDeclarationMarker;
 }
 inline void Parser::goForExpression()
 {
	 firstToken = TK_ExpressionMarker;
 }




} // Close namespace Jikes block


#endif // parser_INCLUDED

