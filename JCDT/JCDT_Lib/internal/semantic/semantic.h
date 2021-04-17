#ifndef semantic_INCLUDED
#define semantic_INCLUDED

#include "JCDT_Lib/internal/env/platform.h"
#include "JCDT_Lib/internal/ast/ast.h"
#include "JCDT_Lib/internal/problem/SemanticError.h"
#include "JCDT_Lib/internal/util/tuple.h"
#include "JCDT_Lib/internal/util/set.h"

#include "JCDT_Lib/internal/lex/LexStream.h"

#include "SemanticStack.h"
#include "BlockStack.h"
#include "DefiniteBlockStack.h"
#include "DefiniteFinalAssignmentStack.h"
#include "SemanticEnvironmentStack.h"
#include "SymbolTableStack.h"
#include "SemanticEnvironment.h"
#include <JCDT_Lib/internal/lookup/DirectorySymbol.h>
#include <JCDT_Lib/internal/lookup/PackageSymbol.h>
#include <JCDT_Lib/internal/lookup/TypeSymbol.h>
#include <JCDT_Lib/internal/lookup/FileSymbol.h>
#include <JCDT_Lib/internal/lookup/MethodSymbol.h>
#include <JCDT_Lib/internal/lookup/PathSymbol.h>
#include "JCDT_Lib/internal/lookup/BlockSymbol.h"

namespace Jikes { // Open namespace Jikes block



class LookupEnvironment;
class TypeShadowSymbol;
class MethodShadowSymbol;
class CPClassInfo;
class ConstantPool;
//
// A semantic object is associated with each compilation unit (ie. each .java
// file). It is subdivided into SemanticEnvironments, which detail the
// compilation state of the current class or interface.
//
class Semantic
{
public:
	
    LookupEnvironment& lookup_env; // The control object performing the compilation
    FileSymbol* source_file_symbol; // The source file name
    LexStream* lex_stream; // The source file contents
    CompilationUnitDeclaration* compilation_unit; // The syntax tree
    DirectorySymbol* directory_symbol; // The source file location

    SymbolSet types_to_be_processed;

    SymbolSet referenced_package_imports;
    SymbolSet referenced_type_imports;

    int return_code;

    // The constructor
	Semantic(LookupEnvironment& control_, FileSymbol* file_symbol_);

	~Semantic();

	// Report a multi-token semantic warning or error.
	void ReportSemError(SemanticError::SemanticErrorKind kind,
		Token* ltok, Token* rtok,
		const wchar_t* s1 = NULL, const wchar_t* s2 = NULL,
		const wchar_t* s3 = NULL, const wchar_t* s4 = NULL,
		const wchar_t* s5 = NULL, const wchar_t* s6 = NULL,
		const wchar_t* s7 = NULL, const wchar_t* s8 = NULL,
		const wchar_t* s9 = NULL);

    // Report a semantic warning or error on a syntax tree branch.
	void ReportSemErrorWithAst(SemanticError::SemanticErrorKind kind, Ast* ast,
		const wchar_t* s1 = NULL, const wchar_t* s2 = NULL,
		const wchar_t* s3 = NULL, const wchar_t* s4 = NULL,
		const wchar_t* s5 = NULL, const wchar_t* s6 = NULL,
		const wchar_t* s7 = NULL, const wchar_t* s8 = NULL,
		const wchar_t* s9 = NULL);

    // Report a single-token semantic warning or error.
	void ReportSemErrorSimple(SemanticError::SemanticErrorKind kind, Token* tok,
		const wchar_t* s1 = NULL, const wchar_t* s2 = NULL,
		const wchar_t* s3 = NULL, const wchar_t* s4 = NULL,
		const wchar_t* s5 = NULL, const wchar_t* s6 = NULL,
		const wchar_t* s7 = NULL, const wchar_t* s8 = NULL,
		const wchar_t* s9 = NULL);

	unsigned NumErrors() const
	{ return (error ? error->num_errors : 0); }
	unsigned NumIssues() const{
		return (error ? error->num_errors + error->num_warnings: 0);
	}
	void TransferError(vector<JikesError*>& errer_owner);
    //
    // If we had a bad compilation unit, print the parser messages.
    // If semantic errors were detected print them too.
    // Set the return code. Implemented in error.cpp.
    //
    void PrintMessages();

	PackageSymbol* Package();

    // Implemented in decl.cpp - performs first pass over .java file.
    void CheckPackage();
    void ProcessTypeNames();
    void ProcessImports();
    TypeSymbol* ReadType(FileSymbol*, PackageSymbol*, NameSymbol*, Token*);

    // Implemented in init.cpp - determines values of final fields.
    void ComputeFinalValue(VariableSymbol*);

    // Implemented in class.cpp - reads in a .class file.
    TypeSymbol* ProcessSignature(TypeSymbol*, const char*&, Token*);
    TypeSymbol* ReadTypeFromSignature(TypeSymbol*, const char*, int,
		Token*);
    TypeSymbol* ProcessNestedType(TypeSymbol*, NameSymbol*, Token*);

    // Implemented in expr.cpp - semantic checks of expressions
    bool IsConstantTrue(Expression* expr);
    bool IsConstantFalse(Expression* expr);
	inline bool IsSystermSemantic();
	inline void MarkSystemSemantic();


public:
    enum
    {
        INT_SHIFT_MASK = 0x1f,
        LONG_SHIFT_MASK = 0x3f
    };
	bool system_semantic = false;
	SemanticError* error = nullptr;

   
  
    // Implemented in decl.cpp - process a .java file for declarations
    void ProcessTypeHeader(ClassTypeDeclaration*);
    void ProcessTypeHeader(EnumTypeDeclaration*);
    void ProcessTypeHeader(InterfaceTypeDeclaration*);
    void ProcessTypeHeader(AnnotationTypeDeclaration*);
    TypeSymbol* ProcessTypeHeaders(AstClassBody*, TypeSymbol* = NULL);
    void ProcessSuperinterface(TypeSymbol*, AstTypeName*);
    void ProcessTypeParameters(TypeSymbol*, AstTypeParameters*);
    void ProcessConstructorMembers(AstClassBody*);
    void ProcessMethodMembers(AstClassBody*);
    void ProcessClassBodyForEffectiveJavaChecks(AstClassBody*);
   // void CheckForSerializationMistakes(AstClassBody*);
    void ProcessFieldMembers(AstClassBody*);
    void ProcessMembers(AstClassBody*);
    void CompleteSymbolTable(AstClassBody*);

    // Implemented in body.cpp - process method bodies and field initializers
    void ProcessExecutableBodies(AstClassBody*);

    friend class TypeSymbol;
    friend class VariableSymbol;

    // Used in the handling of imports - see decl.cpp
    Tuple<Symbol*> import_on_demand_packages;
    Tuple<TypeSymbol*> single_type_imports;

    //
    // Where am I?
    //
    PackageSymbol* this_package;

    // Look at state associated with the current type
	bool InDeprecatedContext();
	TypeSymbol* ThisType();
	MethodSymbol*& ThisMethod();
	VariableSymbol*& ThisVariable();
	ReachComleteAbleStatement*& ExplicitConstructorInvocation();
	SymbolTableStack& LocalSymbolTable();
	SemanticStack<SymbolSet*>& TryExceptionTableStack();
	SemanticStack<TryStatement*>& TryStatementStack();
	SemanticStack<AstBlock*>& BreakableStatementStack();
	SemanticStack<AstBlock*>& ContinuableStatementStack();
	SemanticStack<unsigned>& AbruptFinallyStack();
	BlockStack& LocalBlockStack();
	SemanticEnvironment* GetEnvironment(AstArguments* ast);
	bool StaticRegion();

	DefinitePair*& DefinitelyAssignedVariables();
	DefinitePair*& Universe();
	BitSet*& BlankFinals();
	BitSet*& ReachableAssignments();
   
	DefiniteBlockStack*& DefiniteBlocks();
   
	DefiniteFinalAssignmentStack*& DefiniteFinalAssignments();
   
	Tuple<VariableSymbol*>*& FinalFields();
   
	bool& ProcessingSimpleAssignment();
  

    // A stack to allow nested type processing
    SemanticEnvironmentStack<SemanticEnvironment*> state_stack;

    // Implemented in expr.cpp - semantic checks of expressions
    bool IsIntValueRepresentableInType(Expression*, const TypeSymbol*);

    // Implemented in decl.cpp - nested class processing
    void CheckNestedMembers(TypeSymbol*, AstClassBody*);
    void CheckNestedTypeDuplication(SemanticEnvironment*, Token*);
    TypeSymbol* ProcessNestedTypeName(TypeSymbol*, TypeDeclaration*);
    TypeSymbol* FindTypeInShadow(TypeShadowSymbol*, Token*);
    void ReportTypeInaccessible(Token*, Token*, TypeSymbol*);
	void ReportTypeInaccessible(Ast* ast, TypeSymbol* type);
    TypeSymbol* GetBadNestedType(TypeSymbol*, Token*);
    TypeSymbol* FindNestedType(TypeSymbol*, Token*);
    TypeSymbol* MustFindNestedType(TypeSymbol*, AstName*);
    void ProcessImportQualifiedName(AstName*);
    void ProcessPackageOrType(AstName*);
    void ProcessTypeImportOnDemandDeclaration(AstImportDeclaration*);
    TypeSymbol* FindSimpleNameType(PackageSymbol*, Token*);
    void ProcessSingleTypeImportDeclaration(AstImportDeclaration*);

    // Implemented in modifier.cpp - process declaration modifiers
    AccessFlags ProcessModifiers(AstModifiers*, const wchar_t*, u2, u2 = 0);
    AccessFlags ProcessPackageModifiers(AstPackageDeclaration*);
    AccessFlags ProcessTopLevelTypeModifiers(TypeDeclaration*);
    AccessFlags ProcessNestedTypeModifiers(TypeSymbol*, TypeDeclaration*);
    AccessFlags ProcessLocalClassModifiers(TypeDeclaration*);
    AccessFlags ProcessFieldModifiers(FieldDeclaration*);
    AccessFlags ProcessLocalModifiers(LocalDeclaration*);
    AccessFlags ProcessFormalModifiers(AstFormalParameter*);
    AccessFlags ProcessMethodModifiers(MethodDeclaration*);
    AccessFlags ProcessConstructorModifiers(ConstructorDeclaration*);
    AccessFlags ProcessInterfaceFieldModifiers(FieldDeclaration*);
    AccessFlags ProcessInterfaceMethodModifiers(MethodDeclaration*);
    AccessFlags ProcessInitializerModifiers(Initializer*);
    AccessFlags ProcessEnumConstantModifiers(AstEnumConstant*);

    // Implemented in decl.cpp - process declarations
    void AddDefaultConstructor(TypeSymbol*);
    void ProcessConstructorDeclaration(ConstructorDeclaration*);
    void ProcessMethodDeclaration(MethodDeclaration*);
    void ProcessFieldDeclaration(FieldDeclaration*);
    void ProcessFormalParameters(BlockSymbol*, AstMethodDeclarator*);
    void CheckFieldDeclaration(FieldDeclaration*, AstVariableDeclaratorId*,
                               const AccessFlags&);
    void CheckFieldName(AstVariableDeclaratorId*, NameSymbol*, bool);
    TypeSymbol* ImportType(Token*, NameSymbol*);
    TypeSymbol* FindPrimitiveType(AstPrimitiveType*);
    TypeSymbol* FindType(Token*);
    TypeSymbol* FindInaccessibleType(AstName*);
    TypeSymbol* MustFindType(AstName*);
    void ProcessType(AstType*);

    // Implemented in decl.cpp - process initializers
    void InitializeVariable(FieldDeclaration*, MethodSymbol*);
    void ProcessInitializer(Initializer*, MethodSymbol*);
    void ProcessStaticInitializers(AstClassBody*);
    void ProcessInstanceInitializers(AstClassBody*);
    MethodSymbol* GetStaticInitializerMethod(unsigned estimate = 0);

    // Implemented in expr.cpp - expression processing
    inline bool CanWideningPrimitiveConvert(const TypeSymbol*,
                                            const TypeSymbol*);
    inline bool CanNarrowingPrimitiveConvert(const TypeSymbol*,
                                             const TypeSymbol*);
    bool CanCastConvert(TypeSymbol*, TypeSymbol*, Token* = nullptr);

public:
    bool CanMethodInvocationConvert(const TypeSymbol*, const TypeSymbol*);


    bool CanAssignmentConvert(const TypeSymbol*, Expression*);
    bool CanAssignmentConvertReference(const TypeSymbol*, const TypeSymbol*);
    LiteralValue* CastValue(const TypeSymbol*, Expression*);
    Expression* ConvertToType(Expression*, TypeSymbol*);
    Expression* PromoteUnaryNumericExpression(Expression*);
    void BinaryNumericPromotion(AstAssignmentExpression*);
    void BinaryNumericPromotion(BinaryExpression*);
    void BinaryNumericPromotion(ConditionalExpression*);
    TypeSymbol* BinaryNumericPromotion(Expression*&, Expression*&);
    void MethodInvocationConversion(AstArguments*, MethodSymbol*);

    // Implemented in definite.cpp - definite (un)assignment analysis
    void (Semantic::*DefiniteStmt[Ast::_num_expr_or_stmt_kinds])(Ast*);
    inline void DefiniteStatement(Ast*);

    void DefiniteLoopBody(BitSet&);

    void DefiniteBlock(Ast*);
    void DefiniteLocalClassStatement(Ast*);
    void DefiniteLocalVariableStatement(Ast*);
    void DefiniteExpressionStatement(Ast*);
    void DefiniteSynchronizedStatement(Ast*);
    void DefiniteIfStatement(Ast*);
    void DefiniteWhileStatement(Ast*);
    void DefiniteForStatement(Ast*);
    void DefiniteForeachStatement(Ast*);
    void DefiniteSwitchStatement(Ast*);
    void DefiniteDoStatement(Ast*);
    void DefiniteBreakStatement(Ast*);
    void DefiniteContinueStatement(Ast*);
    void DefiniteReturnStatement(Ast*);
    void DefiniteThrowStatement(Ast*);
    void DefiniteTryStatement(Ast*);
    void DefiniteAssertStatement(Ast*);
    void DefiniteDefaultStatement(Ast*) {}
    void DefiniteThisCall(AstThisCall*);
    void DefiniteSuperCall(AstSuperCall*);
    void DefiniteInvalidStatement(Ast*) { assert(false); }
    DefiniteAssignmentSet* DefiniteInvalidExpression(Expression*,
                                                     DefinitePair&)
    {
        assert(false);
        return NULL;
    }
    void DefiniteInvalidExpression(Ast*) { assert(false); }

    VariableSymbol* DefiniteFinal(FieldReference*);

    DefiniteAssignmentSet* (Semantic::*DefiniteExpr[Ast::_num_expression_kinds])(Expression*, DefinitePair&);
    DefiniteAssignmentSet* DefiniteName(Expression*, DefinitePair&);
    DefiniteAssignmentSet* DefiniteArrayAccess(Expression*,
                                               DefinitePair&);
    DefiniteAssignmentSet* DefiniteMethodInvocation(Expression*,
                                                    DefinitePair&);
    DefiniteAssignmentSet* DefiniteClassCreationExpression(Expression*,
                                                           DefinitePair&);
    DefiniteAssignmentSet* DefiniteArrayCreationExpression(Expression*,
                                                           DefinitePair&);
    DefiniteAssignmentSet* DefinitePreUnaryExpression(Expression*,
                                                      DefinitePair&);
    DefiniteAssignmentSet* DefinitePostUnaryExpression(Expression*,
                                                       DefinitePair&);
    DefiniteAssignmentSet* DefiniteBinaryExpression(Expression*,
                                                    DefinitePair&);
    DefiniteAssignmentSet* DefiniteInstanceofExpression(Expression*,
                                                        DefinitePair&);
    DefiniteAssignmentSet* DefiniteConditionalExpression(Expression*,
                                                         DefinitePair&);
    DefiniteAssignmentSet* DefiniteAssignmentExpression(Expression*,
                                                        DefinitePair&);
    DefiniteAssignmentSet* DefiniteDefaultExpression(Expression*,
                                                     DefinitePair&)
    {
        return NULL;
    }
    DefiniteAssignmentSet* DefiniteFieldAccess(Expression*,
                                               DefinitePair&);
    DefiniteAssignmentSet* DefiniteParenthesizedExpression(Expression*,
                                                           DefinitePair&);
    DefiniteAssignmentSet* DefiniteCastExpression(Expression*,
                                                  DefinitePair&);
    DefiniteAssignmentSet* DefiniteBooleanExpression(Expression*,
                                                     DefinitePair&);
    void DefiniteExpression(Expression*, DefinitePair&);

    DefiniteAssignmentSet* (Semantic::*DefinitePreUnaryExpr[AstPreUnaryExpression::_num_kinds])(Expression*, DefinitePair&);
    DefiniteAssignmentSet* DefiniteDefaultPreUnaryExpression(Expression*,
                                                             DefinitePair&);
    DefiniteAssignmentSet* DefiniteNOT(Expression*, DefinitePair&);
    DefiniteAssignmentSet* DefinitePLUSPLUSOrMINUSMINUS(Expression*,
                                                        DefinitePair&);

    DefiniteAssignmentSet* (Semantic::*DefiniteBinaryExpr[BinaryExpression::_num_kinds])(BinaryExpression*, DefinitePair&);
    DefiniteAssignmentSet* DefiniteDefaultBinaryExpression(BinaryExpression*,
                                                           DefinitePair&);
    DefiniteAssignmentSet* DefiniteAND_AND(BinaryExpression*,
                                           DefinitePair&);
    DefiniteAssignmentSet* DefiniteOR_OR(BinaryExpression*,
                                         DefinitePair&);

    void DefiniteArrayInitializer(AstArrayInitializer*, DefinitePair&);
    void DefiniteArrayInitializer(AstArrayInitializer*);
    void DefiniteVariableInitializer(AstVariableDeclarator*);
    void DefiniteBlockStatements(AstBlock*);
    void DefiniteMethodBody(MethodDeclaration*);
    void DefiniteConstructorBody(ConstructorDeclaration*);
    void DefiniteBlockInitializer(AstBlock*, int);
    void DefiniteFieldInitializer(AstVariableDeclarator*);
    void DefiniteSetup();
    void DefiniteCleanUp();

    // Implemented in body.cpp - method bodies and statements
    void ProcessBlockStatements(AstBlock*);
    void ProcessThisCall(AstThisCall*);
    void ProcessSuperCall(AstSuperCall*);
    void WarnOfAccessibleFieldWithName(SemanticError::SemanticErrorKind,
                                       AstVariableDeclaratorId*, NameSymbol*,
                                       bool);
    void CheckThrow(AstTypeName*, Tuple<AstTypeName*>*);
    void ProcessMethodBody(MethodDeclaration*);
    void ProcessConstructorBody(ConstructorDeclaration*);
    bool CheckedException(TypeSymbol*);
    bool UncaughtException(TypeSymbol*);
    wstring UncaughtExceptionContext();

    // Implemented in expr.cpp - expression processing
    wchar_t* Header(const NameSymbol*, AstArguments*);
    void ReportConstructorNotFound(Ast*, TypeSymbol*);
    void ReportMethodNotFound(AstMethodInvocation*, TypeSymbol*);
    MethodSymbol* FindConstructor(TypeSymbol*, Ast*, Token*, Token*);
    inline bool MoreSpecific(MethodSymbol*, MethodSymbol*);
    inline bool MoreSpecific(MethodSymbol*, Tuple<MethodSymbol*>&);
    inline bool NoMethodMoreSpecific(Tuple<MethodSymbol*>&, MethodSymbol*);
    inline bool MoreSpecific(MethodSymbol*, Tuple<MethodShadowSymbol*>&);
    inline bool NoMethodMoreSpecific(Tuple<MethodShadowSymbol*>&,
                                     MethodSymbol*);
    void FindMethodInEnvironment(Tuple<MethodShadowSymbol*>&,
                                 SemanticEnvironment*&,
                                 SemanticEnvironment*, AstMethodInvocation*);
    MethodSymbol* FindMisspelledMethodName(TypeSymbol*,
                                           AstMethodInvocation*,
                                           NameSymbol*);
	MethodSymbol* FindMisspelledMethodName(TypeSymbol*,
		AstMethodInvocation*,
		NameSymbol* , TypeSymbol*);
    MethodShadowSymbol* FindMethodInEnvironment(SemanticEnvironment*&,
                                                AstMethodInvocation*);
    MethodShadowSymbol* FindMethodInType(TypeSymbol*, AstMethodInvocation*,
                                         NameSymbol* = NULL);

    void ReportVariableNotFound(Expression*, TypeSymbol*);
    void FindVariableInEnvironment(Tuple<VariableSymbol*>&,
                                   SemanticEnvironment*&,
                                   SemanticEnvironment*, NameSymbol*,
                                   Token*);
    VariableSymbol* FindMisspelledVariableName(TypeSymbol*,
                                               Expression*);
    VariableSymbol* FindVariableInEnvironment(SemanticEnvironment*&,
		Token*);
    VariableSymbol* FindVariableInType(TypeSymbol*, Expression*,
                                       NameSymbol* = NULL);
    VariableSymbol* FindLocalVariable(VariableSymbol*, TypeSymbol*);
    Expression* FindEnclosingInstance(Expression*, TypeSymbol*, bool);
    Expression* CreateAccessToType(Ast*, TypeSymbol*);
    void CreateAccessToScopedVariable(AstName*, TypeSymbol*);
    void CreateAccessToScopedMethod(AstMethodInvocation*, TypeSymbol*);

    bool TypeAccessCheck(TypeSymbol*);
    bool ConstructorAccessCheck(MethodSymbol*, bool);
	bool ConstructorAccessCheck(MethodSymbol* constructor, bool explicit_ctor, TypeSymbol* this_type);

	bool MemberAccessCheck(TypeSymbol* base_type, Symbol* symbol, Expression* base = NULL){
		return MemberAccessCheck(base_type, symbol, ThisType(), base);
    }
	bool MemberAccessCheck(TypeSymbol*, Symbol*, TypeSymbol*, Expression* = NULL);
    bool ProtectedAccessCheck(TypeSymbol*);

    void (Semantic::*ProcessPreUnaryExpr[AstPreUnaryExpression::_num_kinds])(AstPreUnaryExpression*);
    void ProcessPLUS(AstPreUnaryExpression*);
    void ProcessMINUS(AstPreUnaryExpression*);
    void ProcessTWIDDLE(AstPreUnaryExpression*);
    void ProcessNOT(AstPreUnaryExpression*);
    void ProcessPLUSPLUSOrMINUSMINUS(AstPreUnaryExpression*);

    void (Semantic::*ProcessBinaryExpr[BinaryExpression::_num_kinds])(BinaryExpression*);
    void ProcessPLUS(BinaryExpression*);
    void ProcessShift(BinaryExpression*);
    void ProcessShiftCount(TypeSymbol*, Expression*);
    void ProcessLEFT_SHIFT(BinaryExpression*);
    void ProcessRIGHT_SHIFT(BinaryExpression*);
    void ProcessUNSIGNED_RIGHT_SHIFT(BinaryExpression*);
    void ProcessLESS(BinaryExpression*);
    void ProcessGREATER(BinaryExpression*);
    void ProcessLESS_EQUAL(BinaryExpression*);
    void ProcessGREATER_EQUAL(BinaryExpression*);
    void ProcessAND(BinaryExpression*);
    void ProcessXOR(BinaryExpression*);
    void ProcessIOR(BinaryExpression*);
    void ProcessAND_AND(BinaryExpression*);
    void ProcessOR_OR(BinaryExpression*);
    void ProcessEQUAL_EQUAL(BinaryExpression*);
    void ProcessNOT_EQUAL(BinaryExpression*);
    void ProcessSTAR(BinaryExpression*);
    void ProcessMINUS(BinaryExpression*);
    void ProcessSLASH(BinaryExpression*);
    void ProcessMOD(BinaryExpression*);

    MethodShadowSymbol* FindMethodMember(TypeSymbol*, AstMethodInvocation*);
    void ProcessMethodName(AstMethodInvocation*);

    //
    // An array of member methods, to dispatch the various expressions and
    // statements for processing.
    //
    void (Semantic::*ProcessExprOrStmt[Ast::_num_expr_or_stmt_kinds])(Ast*);

	inline void ProcessStatement(ReachComleteAbleStatement* stmt);

	inline void ProcessExpression(Expression* expr);

    void ProcessExpressionOrStringConstant(Expression* expr);

    // Implemented in body.cpp - statement processing
    void CheckForAssignmentUsedAsTruthValue(Ast*);
    void ProcessLocalVariableStatement(Ast*);
    void ProcessBlock(Ast*);
    void ProcessForStatement(Ast*);
    void ProcessForeachStatement(Ast*);
    void ProcessSwitchStatement(Ast*);
    void ProcessThrowStatement(Ast*);
    void ProcessTryStatement(Ast*);
    void ProcessExpressionStatement(Ast*);
    void ProcessSynchronizedStatement(Ast*);
    void ProcessIfStatement(Ast*);
    void ProcessWhileStatement(Ast*);
    void ProcessDoStatement(Ast*);
    void ProcessBreakStatement(Ast*);
    void ProcessContinueStatement(Ast*);
    void ProcessReturnStatement(Ast*);
    void ProcessAssertStatement(Ast*);
    void ProcessEmptyStatement(Ast*);
	void ProcessInvalidKind(Ast* ast);

    TypeSymbol* GetLocalType(TypeDeclaration*);
    void ProcessClassDeclaration(Ast*);

    // Implemented in expr.cpp - expression processing
    void CheckSimpleName(AstName*, SemanticEnvironment* where_found);
    void ProcessName(Ast*);
    void FindVariableMember(TypeSymbol*, Expression*);
    void ProcessAmbiguousName(AstName*);
    void ProcessFieldAccess(Ast*);
    void ProcessIntegerLiteral(Ast*);
    void ProcessLongLiteral(Ast*);
    void ProcessFloatLiteral(Ast*);
    void ProcessDoubleLiteral(Ast*);
    void ProcessTrueLiteral(Ast*);
    void ProcessFalseLiteral(Ast*);
    void ProcessStringLiteral(Ast*);
    void ProcessCharacterLiteral(Ast*);
    void ProcessArrayAccess(Ast*);
    bool ProcessArguments(AstArguments*);
    void ProcessMethodInvocation(Ast*);
    void ProcessNullLiteral(Ast*);
    void ProcessClassLiteral(Ast*);
    void ProcessThisExpression(Ast*);
    void ProcessSuperExpression(Ast*);
    void ProcessParenthesizedExpression(Ast*);
    void UpdateLocalConstructors(TypeSymbol*);
    void GetAnonymousConstructor(AstClassCreationExpression*,
                                 TypeSymbol*);
    TypeSymbol* GetAnonymousType(AstClassCreationExpression*,
                                 TypeSymbol*);
    void ProcessClassCreationExpression(Ast*);
    void ProcessArrayCreationExpression(Ast*);
    void ProcessPostUnaryExpression(Ast*);
    void ProcessPreUnaryExpression(Ast*);
    void ProcessCastExpression(Ast*);
    void ProcessBinaryExpression(Ast*);
    void ProcessInstanceofExpression(Ast*);
    void ProcessConditionalExpression(Ast*);
    void ProcessAssignmentExpression(Ast*);

    void ProcessVariableInitializer(AstVariableDeclarator*);
    void ProcessArrayInitializer(AstArrayInitializer*, TypeSymbol*);

    // Implemented in decl.cpp - inheritance of declared members
    void CheckMethodOverride(MethodSymbol*, MethodSymbol*, TypeSymbol*);
    void AddInheritedTypes(TypeSymbol*, TypeSymbol*);
    void AddInheritedFields(TypeSymbol*, TypeSymbol*);
    void AddInheritedMethods(TypeSymbol*, TypeSymbol*, Token*);
    void ComputeTypesClosure(TypeSymbol*, Token*);
    void ComputeFieldsClosure(TypeSymbol*, Token*);
    void ComputeMethodsClosure(TypeSymbol*, Token*);

    // Implemented in class.cpp - reads in a .class file.
    TypeSymbol* RetrieveNestedTypes(TypeSymbol*, wchar_t*, Token*);
    TypeSymbol* GetType(TypeSymbol*, CPClassInfo*, const ConstantPool&,
		Token*);
    void ProcessClassFile(TypeSymbol*, const char*, unsigned, Token*);
    void ReadClassFile(TypeSymbol*, Token*);

    // Implemented in depend.cpp - class dependence tracking.
    void AddDependence(TypeSymbol*, TypeSymbol*, bool = false);
};


// Report a single-token semantic warning or error.
inline void Semantic::ReportSemErrorSimple(SemanticError::SemanticErrorKind kind, Token* tok,
	const wchar_t* s1 , const wchar_t* s2 ,
	const wchar_t* s3, const wchar_t* s4 ,
	const wchar_t* s5 , const wchar_t* s6 ,
	const wchar_t* s7 , const wchar_t* s8 ,
	const wchar_t* s9 )
{
	
	ReportSemError(kind, tok, tok, s1, s2, s3, s4, s5, s6, s7, s8, s9);
}


inline PackageSymbol* Semantic::Package() { return this_package; }


inline bool Semantic::IsSystermSemantic()
{
	return system_semantic;
}
inline void Semantic::MarkSystemSemantic()
{
	system_semantic = true;
}


inline bool Semantic::InDeprecatedContext()
{
	return ThisType()->IsDeprecated() ||
		(ThisMethod() && ThisMethod()->IsDeprecated()) ||
		(ThisVariable() && ThisVariable()->IsDeprecated());
}


inline TypeSymbol* Semantic::ThisType() { return state_stack.Top()->Type(); }
inline MethodSymbol*& Semantic::ThisMethod() { return state_stack.Top()->this_method; }
inline VariableSymbol*& Semantic::ThisVariable()
{
	return state_stack.Top()->this_variable;
}
inline ReachComleteAbleStatement*& Semantic::ExplicitConstructorInvocation()
{
	return state_stack.Top()->explicit_constructor;
}
inline SymbolTableStack& Semantic::LocalSymbolTable()
{
	return state_stack.Top()->symbol_table;
}
inline SemanticStack<SymbolSet*>& Semantic::TryExceptionTableStack()
{
	return state_stack.Top()->try_exception_table_stack;
}
inline SemanticStack<TryStatement*>& Semantic::TryStatementStack()
{
	return state_stack.Top()->try_statement_stack;
}
inline SemanticStack<AstBlock*>& Semantic::BreakableStatementStack()
{
	return state_stack.Top()->breakable_statement_stack;
}
inline SemanticStack<AstBlock*>& Semantic::ContinuableStatementStack()
{
	return state_stack.Top()->continuable_statement_stack;
}
inline SemanticStack<unsigned>& Semantic::AbruptFinallyStack()
{
	return state_stack.Top()->abrupt_finally_stack;
}
inline BlockStack& Semantic::LocalBlockStack()
{
	return state_stack.Top()->block_stack;
}
inline SemanticEnvironment* Semantic::GetEnvironment(AstArguments* ast)
{
	return state_stack.Top()->GetEnvironment(ast);
}
inline bool Semantic::StaticRegion()
{
	return state_stack.Top()->StaticRegion();
}

inline DefinitePair*& Semantic::DefinitelyAssignedVariables()
{
	return state_stack.Top()->definitely_assigned_variables;
}
inline DefinitePair*& Semantic::Universe() { return state_stack.Top()->universe; }
inline BitSet*& Semantic::BlankFinals() { return state_stack.Top()->blank_finals; }
inline BitSet*& Semantic::ReachableAssignments()
{
	return state_stack.Top()->reachable_assignments;
}
inline DefiniteBlockStack*& Semantic::DefiniteBlocks()
{
	return state_stack.Top()->definite_block_stack;
}
inline DefiniteFinalAssignmentStack*& Semantic::DefiniteFinalAssignments()
{
	return state_stack.Top()->definite_final_assignment_stack;
}
inline Tuple<VariableSymbol*>*& Semantic::FinalFields()
{
	return state_stack.Top()->final_fields;
}
inline bool& Semantic::ProcessingSimpleAssignment()
{
	return state_stack.Top()->processing_simple_assignment;
}
inline  void Semantic::ReportTypeInaccessible(Ast* ast, TypeSymbol* type)
{
	ReportTypeInaccessible(ast->LeftToken(), ast->RightToken(), type);
}
inline void Semantic::ProcessStatement(ReachComleteAbleStatement* stmt)
{
	(this->*ProcessExprOrStmt[stmt->kind])(stmt);
}

inline void Semantic::ProcessExpression(Expression* expr)
{
	if (expr->symbol)
	{
		assert(expr->generated);
		return;
		// already processed, make sure it was compiler-generated
	}
		
		
	else (this->*ProcessExprOrStmt[expr->kind])(expr);
}

inline void Semantic::ProcessInvalidKind(Ast* ast)
{
	assert(ast->IsExplicitConstructorInvocation());
	ReachComleteAbleStatement* statement = (ReachComleteAbleStatement*)ast;
	statement->can_complete_normally = statement->is_reachable;
	ReportSemError(SemanticError::MISPLACED_EXPLICIT_CONSTRUCTOR,
		statement->LeftToken(),
		statement->RightToken());
}
} // Close namespace Jikes block


#endif // semantic_INCLUDED

