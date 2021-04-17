#pragma  once 
class ASTVisitor;
#include "..\\lookup\\lookupPackageBrower.h"
#include "..\\ast\\ASTPackageBrower.h"
#include "..\\..\\compiler\IProblem.h"
#include "..\\parser\\Parser.h"

class ASTVisitor;
class ASTVisito_ClassBodyDeclarations;
class MethodVisitor;
class	TypeVisitor;

class ASTVisitor
{
public:
	virtual ~ASTVisitor(){
	}
	 void acceptProblem(IProblem* problem) {
		// do nothing by default
	}
	 void endVisit(
		AllocationExpression* allocationExpression,
		BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(AND_AND_Expression* and_and_Expression, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(
			AnnotationMethodDeclaration* annotationTypeDeclaration,
			ClassScope* classScope) {
			// do nothing by default
	}
	 void endVisit(Argument* argument, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(Argument* argument,ClassScope* scope) {
		// do nothing by default
	}
	 void endVisit(
    		ArrayAllocationExpression* arrayAllocationExpression,
    		BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(ArrayInitializer* arrayInitializer, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(
		ArrayQualifiedTypeReference* arrayQualifiedTypeReference,
		BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(
		ArrayQualifiedTypeReference* arrayQualifiedTypeReference,
		ClassScope* scope) {
		// do nothing by default
	}
	 void endVisit(ArrayReference* arrayReference, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(ArrayTypeReference* arrayTypeReference, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(ArrayTypeReference* arrayTypeReference, ClassScope* scope) {
		// do nothing by default
	}
	 void endVisit(AssertStatement* assertStatement, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(Assignment* assignment, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(BinaryExpression* binaryExpression, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(Block* block, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(BreakStatement* breakStatement, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(CaseStatement* caseStatement, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(CastExpression* castExpression, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(CharLiteral* charLiteral, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(ClassLiteralAccess* classLiteral, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(Clinit* clinit, ClassScope* scope) {
		// do nothing by default
	}
	 void endVisit(
		CompilationUnitDeclaration* compilationUnitDeclaration,
		CompilationUnitScope* scope) {
		// do nothing by default
	}
	 void endVisit(CompoundAssignment* compoundAssignment, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(
			ConditionalExpression* conditionalExpression,
			BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(
		ConstructorDeclaration* constructorDeclaration,
		ClassScope* scope) {
		// do nothing by default
	}
	 void endVisit(ContinueStatement* continueStatement, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(DoStatement* doStatement, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(DoubleLiteral* doubleLiteral, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(EmptyStatement* emptyStatement, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(EqualExpression* equalExpression, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(
		ExplicitConstructorCall* explicitConstructor,
		BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(
		ExtendedStringLiteral* extendedStringLiteral,
		BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(FalseLiteral* falseLiteral, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(FieldDeclaration* fieldDeclaration, MethodScope* scope) {
		// do nothing by default
	}
	 void endVisit(FieldReference* fieldReference, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(FieldReference* fieldReference, ClassScope* scope) {
		// do nothing by default
	}
	 void endVisit(FloatLiteral* floatLiteral, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(ForeachStatement* forStatement, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(ForStatement* forStatement, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(IfStatement* ifStatement, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(ImportReference* importRef, CompilationUnitScope* scope) {
		// do nothing by default
	}
	 void endVisit(Initializer* initializer, MethodScope* scope) {
		// do nothing by default
	}
	 void endVisit(
    		InstanceOfExpression* instanceOfExpression,
    		BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(IntLiteral* intLiteral, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(Javadoc* javadoc, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(Javadoc* javadoc, ClassScope* scope) {
		// do nothing by default
	}
	 void endVisit(JavadocAllocationExpression* expression, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(JavadocAllocationExpression* expression, ClassScope* scope) {
		// do nothing by default
	}
	 void endVisit(JavadocArgumentExpression* expression, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(JavadocArgumentExpression* expression, ClassScope* scope) {
		// do nothing by default
	}
	 void endVisit(JavadocArrayQualifiedTypeReference* typeRef, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(JavadocArrayQualifiedTypeReference* typeRef, ClassScope* scope) {
		// do nothing by default
	}
	 void endVisit(JavadocArraySingleTypeReference* typeRef, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(JavadocArraySingleTypeReference* typeRef, ClassScope* scope) {
		// do nothing by default
	}
	 void endVisit(JavadocFieldReference* fieldRef, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(JavadocFieldReference* fieldRef, ClassScope* scope) {
		// do nothing by default
	}
	 void endVisit(JavadocImplicitTypeReference* implicitTypeReference, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(JavadocImplicitTypeReference* implicitTypeReference, ClassScope* scope) {
		// do nothing by default
	}
	 void endVisit(JavadocMessageSend* messageSend, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(JavadocMessageSend* messageSend, ClassScope* scope) {
		// do nothing by default
	}
	 void endVisit(JavadocQualifiedTypeReference* typeRef, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(JavadocQualifiedTypeReference* typeRef, ClassScope* scope) {
		// do nothing by default
	}
	 void endVisit(JavadocReturnStatement* statement, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(JavadocReturnStatement* statement, ClassScope* scope) {
		// do nothing by default
	}
	 void endVisit(JavadocSingleNameReference* argument, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(JavadocSingleNameReference* argument, ClassScope* scope) {
		// do nothing by default
	}
	 void endVisit(JavadocSingleTypeReference* typeRef, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(JavadocSingleTypeReference* typeRef, ClassScope* scope) {
		// do nothing by default
	}
	 void endVisit(LabeledStatement* labeledStatement, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(LocalDeclaration* localDeclaration, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(LongLiteral* longLiteral, BlockScope* scope) {
		// do nothing by default
	}
	/**
	 * @param annotation
	 * @param scope
	 * @since 3.1
	 */
	 void endVisit(MarkerAnnotation* annotation, BlockScope* scope) {
		// do nothing by default
	}
	/**
	 * @param pair
	 * @param scope
	 */
	 void endVisit(MemberValuePair* pair, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(MessageSend* messageSend, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(MethodDeclaration* methodDeclaration, ClassScope* scope) {
		// do nothing by default
	}
	 void endVisit(StringLiteralConcatenation* literal, BlockScope* scope) {
		// do nothing by default
	}
	/**
	 * @param annotation
	 * @param scope
	 * @since 3.1
	 */
	 void endVisit(NormalAnnotation* annotation, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(NullLiteral* nullLiteral, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(OR_OR_Expression* or_or_Expression, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(ParameterizedQualifiedTypeReference* parameterizedQualifiedTypeReference, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(ParameterizedQualifiedTypeReference* parameterizedQualifiedTypeReference, ClassScope* scope) {
		// do nothing by default
	}
	 void endVisit(ParameterizedSingleTypeReference* parameterizedSingleTypeReference, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(ParameterizedSingleTypeReference* parameterizedSingleTypeReference, ClassScope* scope) {
		// do nothing by default
	}
	 void endVisit(PostfixExpression* postfixExpression, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(PrefixExpression* prefixExpression, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(
    		QualifiedAllocationExpression* qualifiedAllocationExpression,
    		BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(
		QualifiedNameReference* qualifiedNameReference,
		BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(
			QualifiedNameReference* qualifiedNameReference,
			ClassScope* scope) {
		// do nothing by default
	}
	 void endVisit(
    		QualifiedSuperReference* qualifiedSuperReference,
    		BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(
    		QualifiedSuperReference* qualifiedSuperReference,
    		ClassScope* scope) {
		// do nothing by default
	}
	 void endVisit(
    		QualifiedThisReference* qualifiedThisReference,
    		BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(
    		QualifiedThisReference* qualifiedThisReference,
    		ClassScope* scope) {
		// do nothing by default
	}
	 void endVisit(
    		QualifiedTypeReference* qualifiedTypeReference,
    		BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(
    		QualifiedTypeReference* qualifiedTypeReference,
    		ClassScope* scope) {
		// do nothing by default
	}
	 void endVisit(ReturnStatement* returnStatement, BlockScope* scope) {
		// do nothing by default
	}
	/**
	 * @param annotation
	 * @param scope
	 * @since 3.1
	 */
	 void endVisit(SingleMemberAnnotation* annotation, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(
    		SingleNameReference* singleNameReference,
    		BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(
			SingleNameReference* singleNameReference,
			ClassScope* scope) {
			// do nothing by default
	}
	 void endVisit(
    		SingleTypeReference* singleTypeReference,
    		BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(
    		SingleTypeReference* singleTypeReference,
    		ClassScope* scope) {
		// do nothing by default
	}
	 void endVisit(StringLiteral* stringLiteral, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(SuperReference* superReference, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(SwitchStatement* switchStatement, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(
		SynchronizedStatement* synchronizedStatement,
		BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(ThisReference* thisReference, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(ThisReference* thisReference, ClassScope* scope) {
		// do nothing by default
	}
	 void endVisit(ThrowStatement* throwStatement, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(TrueLiteral* trueLiteral, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(TryStatement* tryStatement, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(
		TypeDeclaration* localTypeDeclaration,
		BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(
		TypeDeclaration* memberTypeDeclaration,
		ClassScope* scope) {
		// do nothing by default
	}
	 void endVisit(
		TypeDeclaration* typeDeclaration,
		CompilationUnitScope* scope) {
		// do nothing by default
	}
	 void endVisit(TypeParameter* typeParameter, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(TypeParameter* typeParameter, ClassScope* scope) {
		// do nothing by default
	}
	 void endVisit(UnaryExpression* unaryExpression, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(
			UnionTypeReference* unionTypeReference,
			BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(
			UnionTypeReference* unionTypeReference,
			ClassScope* scope) {
		// do nothing by default
	}
	 void endVisit(WhileStatement* whileStatement, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(Wildcard* wildcard, BlockScope* scope) {
		// do nothing by default
	}
	 void endVisit(Wildcard* wildcard, ClassScope* scope) {
		// do nothing by default
	}
	 bool visit(
    		AllocationExpression* allocationExpression,
    		BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(AND_AND_Expression* and_and_Expression, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(
			AnnotationMethodDeclaration* annotationTypeDeclaration,
			ClassScope* classScope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(Argument* argument, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(Argument* argument, ClassScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(
		ArrayAllocationExpression* arrayAllocationExpression,
		BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(ArrayInitializer* arrayInitializer, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(
		ArrayQualifiedTypeReference* arrayQualifiedTypeReference,
		BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(
		ArrayQualifiedTypeReference* arrayQualifiedTypeReference,
		ClassScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(ArrayReference* arrayReference, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(ArrayTypeReference* arrayTypeReference, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(ArrayTypeReference* arrayTypeReference, ClassScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(AssertStatement* assertStatement, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(Assignment* assignment, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(BinaryExpression* binaryExpression, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(Block* block, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(BreakStatement* breakStatement, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(CaseStatement* caseStatement, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(CastExpression* castExpression, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(CharLiteral* charLiteral, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(ClassLiteralAccess* classLiteral, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(Clinit* clinit, ClassScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(
		CompilationUnitDeclaration* compilationUnitDeclaration,
		CompilationUnitScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(CompoundAssignment* compoundAssignment, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(
    		ConditionalExpression* conditionalExpression,
    		BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(
		ConstructorDeclaration* constructorDeclaration,
		ClassScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(ContinueStatement* continueStatement, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(DoStatement* doStatement, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(DoubleLiteral* doubleLiteral, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(EmptyStatement* emptyStatement, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(EqualExpression* equalExpression, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(
		ExplicitConstructorCall* explicitConstructor,
		BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(
		ExtendedStringLiteral* extendedStringLiteral,
		BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(FalseLiteral* falseLiteral, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(FieldDeclaration* fieldDeclaration, MethodScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(FieldReference* fieldReference, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(FieldReference* fieldReference, ClassScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(FloatLiteral* floatLiteral, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(ForeachStatement* forStatement, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(ForStatement* forStatement, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(IfStatement* ifStatement, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(ImportReference* importRef, CompilationUnitScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(Initializer* initializer, MethodScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(
    		InstanceOfExpression* instanceOfExpression,
    		BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(IntLiteral* intLiteral, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(Javadoc* javadoc, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(Javadoc* javadoc, ClassScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(JavadocAllocationExpression* expression, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(JavadocAllocationExpression* expression, ClassScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(JavadocArgumentExpression* expression, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(JavadocArgumentExpression* expression, ClassScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(JavadocArrayQualifiedTypeReference* typeRef, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(JavadocArrayQualifiedTypeReference* typeRef, ClassScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(JavadocArraySingleTypeReference* typeRef, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(JavadocArraySingleTypeReference* typeRef, ClassScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(JavadocFieldReference* fieldRef, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(JavadocFieldReference* fieldRef, ClassScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(JavadocImplicitTypeReference* implicitTypeReference, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(JavadocImplicitTypeReference* implicitTypeReference, ClassScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(JavadocMessageSend* messageSend, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(JavadocMessageSend* messageSend, ClassScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(JavadocQualifiedTypeReference* typeRef, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(JavadocQualifiedTypeReference* typeRef, ClassScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(JavadocReturnStatement* statement, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(JavadocReturnStatement* statement, ClassScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(JavadocSingleNameReference* argument, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(JavadocSingleNameReference* argument, ClassScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(JavadocSingleTypeReference* typeRef, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(JavadocSingleTypeReference* typeRef, ClassScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(LabeledStatement* labeledStatement, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(LocalDeclaration* localDeclaration, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(LongLiteral* longLiteral, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	/**
	 * @param annotation
	 * @param scope
	 * @since 3.1
	 */
	 bool visit(MarkerAnnotation* annotation, BlockScope* scope) {
		return true;
	}
	/**
	 * @param pair
	 * @param scope
	 * @since 3.1
	 */
	 bool visit(MemberValuePair* pair, BlockScope* scope) {
		return true;
	}
	 bool visit(MessageSend* messageSend, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(MethodDeclaration* methodDeclaration, ClassScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(
			StringLiteralConcatenation* literal,
			BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	/**
	 * @param annotation
	 * @param scope
	 * @since 3.1
	 */
	 bool visit(NormalAnnotation* annotation, BlockScope* scope) {
		return true;
	}
	 bool visit(NullLiteral* nullLiteral, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(OR_OR_Expression* or_or_Expression, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(ParameterizedQualifiedTypeReference* parameterizedQualifiedTypeReference, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(ParameterizedQualifiedTypeReference* parameterizedQualifiedTypeReference, ClassScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(ParameterizedSingleTypeReference* parameterizedSingleTypeReference, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(ParameterizedSingleTypeReference* parameterizedSingleTypeReference, ClassScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(PostfixExpression* postfixExpression, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(PrefixExpression* prefixExpression, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(
    		QualifiedAllocationExpression* qualifiedAllocationExpression,
    		BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(
			QualifiedNameReference* qualifiedNameReference,
			BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(
			QualifiedNameReference* qualifiedNameReference,
			ClassScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(
    		QualifiedSuperReference* qualifiedSuperReference,
    		BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(
    		QualifiedSuperReference* qualifiedSuperReference,
    		ClassScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(
			QualifiedThisReference* qualifiedThisReference,
			BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(
			QualifiedThisReference* qualifiedThisReference,
			ClassScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(
    		QualifiedTypeReference* qualifiedTypeReference,
    		BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(
    		QualifiedTypeReference* qualifiedTypeReference,
    		ClassScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(ReturnStatement* returnStatement, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	/**
	 * @param annotation
	 * @param scope
	 * @since 3.1
	 */
	 bool visit(SingleMemberAnnotation* annotation, BlockScope* scope) {
		return true;
	}
	 bool visit(
		SingleNameReference* singleNameReference,
		BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(
			SingleNameReference* singleNameReference,
			ClassScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(
    		SingleTypeReference* singleTypeReference,
    		BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(
    		SingleTypeReference* singleTypeReference,
    		ClassScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(StringLiteral* stringLiteral, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(SuperReference* superReference, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(SwitchStatement* switchStatement, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(
		SynchronizedStatement* synchronizedStatement,
		BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(ThisReference* thisReference, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(ThisReference* thisReference, ClassScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(ThrowStatement* throwStatement, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(TrueLiteral* trueLiteral, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(TryStatement* tryStatement, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(
		TypeDeclaration* localTypeDeclaration,
		BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(
		TypeDeclaration* memberTypeDeclaration,
		ClassScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(
		TypeDeclaration* typeDeclaration,
		CompilationUnitScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(TypeParameter* typeParameter, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(TypeParameter* typeParameter, ClassScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(UnaryExpression* unaryExpression, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(
			UnionTypeReference* unionTypeReference,
			BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(
			UnionTypeReference* unionTypeReference,
			ClassScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(WhileStatement* whileStatement, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(Wildcard* wildcard, BlockScope* scope) {
		return true; // do nothing by default, keep traversing
	}
	 bool visit(Wildcard* wildcard, ClassScope* scope) {
		return true; // do nothing by default, keep traversing
	}

};



class ASTVisito_ClassBodyDeclarations :public   ASTVisitor
{
public:
	std::vector<ASTNode*>& bodyDeclarations;
	ASTVisito_ClassBodyDeclarations(std::vector<ASTNode*>& _bodyDeclarations):bodyDeclarations(_bodyDeclarations)
	{
		
	}
	
	virtual ~ASTVisito_ClassBodyDeclarations(){
	}
	bool visit(MethodDeclaration* methodDeclaration, ClassScope* scope) ;
	bool visit(FieldDeclaration* fieldDeclaration, MethodScope* scope) ;
	bool visit(TypeDeclaration* memberTypeDeclaration, ClassScope* scope) ;
};


class MethodVisitor :public ASTVisitor 
{
public:
	JavaParser* pParser;

	virtual ~MethodVisitor(){
	}
	ASTVisitor* typeVisitor;

	TypeDeclaration* enclosingType; // used only for initializer

	std::vector<TypeDeclaration*> types;
	int typePtr ;

	void endVisit(ConstructorDeclaration* constructorDeclaration, ClassScope* scope) ;

	void endVisit(Initializer* initializer, MethodScope* scope);

	void endVisit(MethodDeclaration* methodDeclaration, ClassScope* scope);

	void endVisitMethod(AbstractMethodDeclaration* methodDeclaration, ClassScope* scope);

	bool visit(ConstructorDeclaration* constructorDeclaration, ClassScope* scope) ;

	bool visit(Initializer* initializer, MethodScope* scope) ;

	bool visit(MethodDeclaration* methodDeclaration,ClassScope* scope);

	bool visit(TypeDeclaration* typeDeclaration) ;

	bool visit(TypeDeclaration* typeDeclaration, BlockScope* scope);

	bool visit(TypeDeclaration* typeDeclaration, ClassScope* scope) ;

};
class TypeVisitor :public ASTVisitor
{
public:

	virtual ~TypeVisitor(){
	}
	MethodVisitor* methodVisitor;
	
	std::vector<TypeDeclaration*> types;
	int typePtr ;

	void endVisit(TypeDeclaration* typeDeclaration, BlockScope* scope);

	void endVisit(TypeDeclaration* typeDeclaration, ClassScope* scope) ;

	void endVisitType() ;

	bool visit(ConstructorDeclaration* constructorDeclaration, ClassScope* scope) ;

	bool visit(Initializer* initializer, MethodScope* scope);

	bool visit(MethodDeclaration* methodDeclaration, ClassScope* scope) ;

	bool visit(TypeDeclaration* typeDeclaration) ;

	bool visit(TypeDeclaration* typeDeclaration, BlockScope* scope);

	bool visit(TypeDeclaration* typeDeclaration, ClassScope* scope) ;

};


