#pragma once

#ifndef ASTNode_INCLUDED
#define ASTNode_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/lex/Token.h"
#include  <assert.h>


namespace Jikes { // Open namespace Jikes block



	class Token;

	class Ast
	{
	public:
		//
		// These tags are used to identify nodes that can represent more than
		// one kind of objects.
		//
		enum AstTag
		{
			NO_TAG,
			PRIMITIVE_TYPE,
			STATEMENT,
			EXPRESSION,

			_num_tags = EXPRESSION
		};

		//
		// These are the different kinds for the Ast objects.
		//
		enum AstKind
		{
			AST, // must be first
				 // Expressions
			NAME,
			DOT,
			INTEGER_LITERAL,
			LONG_LITERAL,
			FLOAT_LITERAL,
			DOUBLE_LITERAL,
			TRUE_LITERAL,
			FALSE_LITERAL,
			STRING_LITERAL,
			CHARACTER_LITERAL,
			NULL_LITERAL,
			CLASS_LITERAL,
			THIS_EXPRESSION,
			SUPER_EXPRESSION,
			PARENTHESIZED_EXPRESSION,
			ARRAY_ACCESS,
			CALL,
			CLASS_CREATION,
			ARRAY_CREATION,
			POST_UNARY,
			PRE_UNARY,
			CAST,
			BINARY,
			INSTANCEOF,
			CONDITIONAL,
			ASSIGNMENT,
			_num_expression_kinds,
			// Statements
			THIS_CALL,
			SUPER_CALL,
			BLOCK,
			IF,
			EMPTY_STATEMENT,
			EXPRESSION_STATEMENT,
			SWITCH,
			SWITCH_BLOCK,
			LOCAL_VARIABLE_DECLARATION,
			LOCAL_CLASS,
			WHILE,
			DO,
			FOR,
			FOREACH,
			BREAK,
			CONTINUE,
			RETURN,
			THROW,
			SYNCHRONIZED_STATEMENT,
			ASSERT_KIND,
			TRY_KIND,
			_num_expr_or_stmt_kinds,
			// All others
			ARGUMENTS = _num_expr_or_stmt_kinds,
			DIM,
			LIST_NODE,
			INT,
			DOUBLE,
			CHAR,
			LONG,
			FLOAT,
			BYTE,
			SHORT,
			BOOLEAN,
			VOID_TYPE,
			ARRAY,
			WILDCARD,
			TYPE_ARGUMENTS,
			TYPE,
			COMPILATION,
			MEMBER_VALUE_PAIR,
			ANNOTATION,
			MODIFIER_KEYWORD,
			MODIFIERS,
			PACKAGE,
			IMPORT,
			EMPTY_DECLARATION,
			ANONYMOUS_DECLARATION,
			CLASS,
			TYPE_PARAM,
			PARAM_LIST,
			CLASS_BODY,
			FIELD,
			VARIABLE_DECLARATOR,
			VARIABLE_DECLARATOR_NAME,
			BRACKETS,
			METHOD,
			METHOD_DECLARATOR,
			PARAMETER,
			CONSTRUCTOR,
			ENUM_TYPE,
			ENUM_CONSTANT,
			INTERFACE_TYPE,
			ANNOTATION_TYPE,
			ARRAY_INITIALIZER,
			INITIALIZER,
			METHOD_BODY,
			SWITCH_LABEL,
			CATCH,
			FINALLY,
			_num_kinds
		};

		//
		// Every node has a unique kind, and class_tag marks groups of similar
		// nodes. The bit-fields allow smaller Ast objects without sacrificing
		// type-safety or debug visibility.
		//
		const AstKind kind ;
		const AstTag class_tag;

		//
		// This is a catch-all set of bits free for the use of subclasses.
		// See CompilationTag, FieldDeclarationTag, InitializerDeclarationTag,
		// ClassBodyTag, BlockTag, PreUnaryExpressionEnum::PreUnaryExpressionTag, PostUnaryExpressionTag,
		// BinaryExpressionTag, AssignmentExpressionTag.
		//

		unsigned other_tag ;

		//
		// "generated" is a boolean value that indicates whether or not a node
		// is associated with a construct in a source file or that is was generated
		// by the compiler. See functions "gen_ ..." and "new_ ..." below.
		//
	public:
		bool generated;

#ifdef JIKES_DEBUG
		const unsigned id;
		static unsigned count;
		static bool debug_unparse;
#endif // JIKES_DEBUG

		//
		// Note that ALL fields of an Ast are initialized to 0 unless modified
		// by the constructor, thanks to the 0-initialization guaranteed by
		// operator new.  This allows for more efficiency by not redundantly
		// setting a field to 0, false, or NULL.
		//
		 Ast(AstKind k, AstTag t = NO_TAG)
			: kind(k)
			, class_tag(t), other_tag(0), generated(false)
#ifdef JIKES_DEBUG
			, id(++count)
#endif // JIKES_DEBUG
		{}

		//
		// Ast nodes should be created from a storage pool. Use the syntax
		// new (pool) AstSubclass(constructor arguments). The resultant Ast
		// will be zero-initialized except for what the constructor explicitly
		// sets, due to the properties of AstStoragePool. Note that there are
		// no Ast[]; rather, use AstArray<Ast*>.
		//
		 void* operator new(size_t, AstStoragePool*);
		 void operator delete(void*, AstStoragePool*);
	private:
		void* operator new[](size_t, void* p) { assert(false); return p; }
		void  operator delete[](void* p,size_t, AstStoragePool*) { assert(false); return ; }
	public:

		//
		// ASTs should not be destructed. Instead, delete the containing
		// AstStoragePool.
		//
		virtual ~Ast() { assert(false && "Use the associated AstStoragePool"); }

#ifdef JIKES_DEBUG
		virtual void Print(Ostream&, LexStream&) = 0;
		virtual void Unparse(Ostream&, LexStream*) = 0;
#endif // JIKES_DEBUG

		//
		// General queries.
		//
		bool IsLeftHandSide();
		bool IsExplicitConstructorInvocation();
		bool IsGenerated();
		inline unsigned sourceStart()
		{
			return LeftToken()->start_location;
		}

		inline  unsigned sourceEnd()
		{
			return RightToken()->EndLocation();
		}
		//
		// The Conversion functions below are provided as a convenient way to
		// cast a generic Ast node into a specific node. Note that if one knows
		// the type of a node for sure, it is more efficient to use a specific
		// cast expression. For example, if one knows that a "Ast* p" pointer
		// dereferences a FieldDeclaration then a cast expression should be
		// used to cast p, as follows:
		//
		//       FieldDeclaration* fp = (FieldDeclaration*) p;
		//
		// However, if p points to a ClassBodyDeclaration, it may be a
		// FieldDeclaration, MethodDeclaration, ConstructorDeclaration,
		// InitializerDeclaration, ClassDeclaration, EnumDeclaration,
		// InterfaceDeclaration, or AnnotationDeclaration; and the following
		// sequence of code may be used:
		//
		//    FieldDeclaration* fp = FieldDeclarationCast();
		//    MethodDeclaration* mp = MethodDeclarationCast();
		//    ConstructorDeclaration* cp = ConstructorDeclarationCast();
		//    Initializer* sp = InitializerdeclarationCast();
		//    ClassTypeDeclaration* Cp = ClassDeclarationCast(); // 1.1 only
		//    EnumTypeDeclaration* Ep = EnumDeclarationCast(); // 1.5 only
		//    InterfaceTypeDeclaration* Ip = InterfaceDeclarationCast(); // 1.1 only
		//    AnnotationTypeDeclaration* Ap = AnnotationDeclarationCast(); // 1.5
		//
		//    if (fp)
		//        ...
		//    else if (mp)
		//        ...
		//    else if (cp)
		//        ...
		//    else if (sp)
		//        ...
		//    else if (Cp)
		//        ...
		//    else if (Ep)
		//        ...
		//    else if (Ip)
		//        ...
		//    else if (Ap)
		//        ...
		//

		//
		// These cast functions are used for classes that represent more than
		// one kind of nodes.  The functions must be listed after the subclasses
		// have been defined.
		//
		//  ReachComleteAbleStatement* StatementCast();
		//  AstMemberValue* MemberValueCast();
		//  Expression* ExpressionCast();
		//  AstPrimitiveType* PrimitiveTypeCast();
		/*  FieldDeclaration* StaticFieldCast();*/
		//  Initializer* StaticInitializerCast();
		//  AstClassBody* UnparsedClassBodyCast();
		//  CompilationUnitDeclaration* BadCompilationUnitCast();
		//  CompilationUnitDeclaration* EmptyCompilationUnitCast();

		//
		// These cast functions are used for classes that represent exactly
		// one kind of node.
		//
		//  AstListNode* ListNodeCast();
		//  AstBlock* BlockCast();
		//  AstName* NameCast();
		//  AstBrackets* BracketsCast();
		//  AstArrayType* ArrayTypeCast();
		//  AstWildcard* WildcardCast();
		//  AstTypeArguments* TypeArgumentsCast();
		//  AstTypeName* TypeNameCast();
		//  AstMemberValuePair* MemberValuePairCast();
		//  AstAnnotation* AnnotationCast();
		//  AstModifierKeyword* ModifierKeywordCast();
		//  AstModifiers* ModifiersCast();
		//  AstPackageDeclaration* PackageDeclarationCast();
		//  AstImportDeclaration* ImportDeclarationCast();
		//  CompilationUnitDeclaration* CompilationUnitCast();
		//  EmptyTypeDeclaration* EmptyDeclarationCast();
		//  AstClassBody* ClassBodyCast();
		//  AstTypeParameter* TypeParameterCast();
		//  AstTypeParameters* TypeParametersCast();
		//  ClassTypeDeclaration* ClassDeclarationCast();
		//  AstArrayInitializer* ArrayInitializerCast();
		//  AstVariableDeclaratorId* VariableDeclaratorIdCast();
		//  AstVariableDeclarator* VariableDeclaratorCast();
		//  FieldDeclaration* FieldDeclarationCast();
		//  AstFormalParameter* FormalParameterCast();
		//  AstMethodDeclarator* MethodDeclaratorCast();
		//  AstMethodBody* MethodBodyCast();
		//  MethodDeclaration* MethodDeclarationCast();
		//  Initializer* InitializerDeclarationCast();
		//  AstArguments* ArgumentsCast();
		//  AstThisCall* ThisCallCast();
		//  AstSuperCall* SuperCallCast();



		//  ConstructorDeclaration* ConstructorDeclarationCast();
		//  EnumTypeDeclaration* EnumDeclarationCast();
		//  AstEnumConstant* EnumConstantCast();
		//  InterfaceTypeDeclaration* InterfaceDeclarationCast();
		//  AnnotationTypeDeclaration* AnnotationDeclarationCast();
		//  LocalDeclaration* LocalVariableStatementCast();
		//  AstLocalClassStatement* LocalClassStatementCast();
		//  IfStatement* IfStatementCast();
		//  EmptyStatement* EmptyStatementCast();
		//  AstExpressionStatement* ExpressionStatementCast();
		//  AstSwitchLabel* SwitchLabelCast();
		//  AstSwitchBlockStatement* SwitchBlockStatementCast();
		//  SwitchStatement* SwitchStatementCast();
		//  WhileStatement* WhileStatementCast();
		//  DoStatement* DoStatementCast();
	//	  ForStatement* ForStatementCast();
	//	  ForeachStatement* ForeachStatementCast();
		//  BreakStatement* BreakStatementCast();
	//	  ContinueStatement* ContinueStatementCast();
	//	  ReturnStatement* ReturnStatementCast();
		//  ThrowStatement* ThrowStatementCast();
	//	  SynchronizedStatement* SynchronizedStatementCast();
	//	  AssertStatement* AssertStatementCast();
	//	  AstCatchClause* CatchClauseCast();
	//	  AstFinallyClause* FinallyClauseCast();
	//	  TryStatement* TryStatementCast();
	//	  IntegerLiteral* IntegerLiteralCast();
	//	  LongLiteral* LongLiteralCast();
	//	  FloatLiteral* FloatLiteralCast();
	//	  DoubleLiteral* DoubleLiteralCast();
		//  TrueLiteral* TrueLiteralCast();
		//  FalseLiteral* FalseLiteralCast();
	//	  StringLiteral* StringLiteralCast();
	//	  CharLiteral* CharacterLiteralCast();
	//	  NullLiteral* NullLiteralCast();
	//	  ClassLiteralAccess* ClassLiteralCast();
	//	  AstThisExpression* ThisExpressionCast();
	//	  AstSuperExpression* SuperExpressionCast();
	//	  AstParenthesizedExpression* ParenthesizedExpressionCast();
	//	  AstClassCreationExpression* ClassCreationExpressionCast();
	//	  AstDimExpr* DimExprCast();
	//	  AstArrayCreationExpression* ArrayCreationExpressionCast();
	//	  FieldReference* FieldAccessCast();
	//	  AstMethodInvocation* MethodInvocationCast();
		//  ArrayReference* ArrayAccessCast();
//		  AstPostUnaryExpression* PostUnaryExpressionCast();
	//	  AstPreUnaryExpression* PreUnaryExpressionCast();
//		  CastExpression* CastExpressionCast();
//		  BinaryExpression* BinaryExpressionCast();
//		  InstanceofExpression* InstanceofExpressionCast();
	//	  ConditionalExpression* ConditionalExpressionCast();
		  AstAssignmentExpression* AssignmentExpressionCast();
		//  EmptyTypeDeclaration*  EmptyTypeDeclarationCast();
		//
		// It would be nice if this could be covariant, as it would allow
		// less casting. But MSVC++ can't yet handle covariant return
		// types at all, and both GCC and HP's aCC croak with covariance during
		// multiple inheritance (bummer). So, there is a bunch of hideous casting
		// in ast.cpp that could otherwise be avoided if standards were followed.
		//
		// Clones are used for various things, such as pre-evaluating final
		// constant values.
		//
		virtual Ast* Clone(AstStoragePool*) = 0;

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) =0;
	
		//
		// These functions return the left and right tokens of this tree branch.
		//
		virtual Token* LeftToken() = 0;
		virtual Token* RightToken() = 0;
	};
	
	//
	// Given an Ast tree, check whether it is a variable (not a value).
	//
	inline    bool Ast::IsLeftHandSide()
	{
		return kind == NAME || kind == DOT || kind == ARRAY_ACCESS;
	}


	//
	// Given an Ast tree, check whether it is an explicit constructor invocation.
	//
	inline    bool Ast::IsExplicitConstructorInvocation()
	{
		return kind == THIS_CALL || kind == SUPER_CALL;
	}


	//
	// Given an Ast tree, check whether or not it is generated.
	//
	inline    bool Ast::IsGenerated()
	{
		return generated;
	}

	
	//
	// Overridden placement new operator allows us to allocate storage from the
	// same pool as everything else in the compilation unit.
	//


} // Close namespace Jikes block


#endif // ASTNode_INCLUDED

