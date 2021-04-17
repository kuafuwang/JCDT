
#ifndef ast_Pre_declaration_INCLUDED
#define ast_Pre_declaration_INCLUDED
#include  <assert.h>
#include "JCDT_Lib/internal/env/platform.h"
#include <vector>
namespace Jikes { // Open namespace Jikes block



class Parser;
class SemanticEnvironment;
class LexStream;
class LiteralValue;
class Symbol;
class BlockSymbol;
class VariableSymbol;
class MethodSymbol;
class TypeSymbol;
class AstStoragePool;
struct CaseElement;
// This is a complete list of all Ast nodes declared here to allow
// forward references.
//
class VariableSymbolArray;

class Ast;
class  AbstractStatement;
template <typename T> class AstArray;
class AstListNode;
class AstDeclared;
class TypeDeclaration;
enum FieldDeclarationTag;


class ReachComleteAbleStatement;
class AstMemberValue;
class Expression;
class AstType;

class AstBlock;
class SwitchBlock;

class AstName;
class AstPrimitiveType;
class AstBrackets;
class AstArrayType;
class AstWildcard;
class AstTypeArguments;
class AstTypeName;
class AstMemberValuePair;
class AstAnnotation;
class AstModifierKeyword;
class AstModifiers;

class ImportReference;
class AstPackageDeclaration;
class AstImportDeclaration;

class CompilationUnitDeclaration;
class EmptyTypeDeclaration;
class AstAnonymousTypeDeclaration;
class AstClassBody;
class AstTypeParameter;
class AstTypeParameters;
class ClassTypeDeclaration;
class AstArrayInitializer;
class AstVariableDeclaratorId;
class AstVariableDeclarator;
class FieldDeclaration;
class AstFormalParameter;
class AstMethodDeclarator;
class AstMethodBody;

class AbstractMethodDeclaration;
class AstNodeScope;
class MethodDeclaration;
class Initializer;
class AstArguments;
class AstThisCall;
class ExplicitConstructorCall;
class AstSuperCall;
class ConstructorDeclaration;
class EnumTypeDeclaration;
class AstEnumConstant;
class InterfaceTypeDeclaration;
class AnnotationTypeDeclaration;

class AbstractVariableDeclaration;
class LocalDeclaration;

class AstLocalClassStatement;
class IfStatement;
class EmptyStatement;
class AstExpressionStatement;
class AstSwitchLabel;
class AstSwitchBlockStatement;
class SwitchStatement;
class WhileStatement;
class DoStatement;
class ForStatement;
class ForeachStatement;
class BreakStatement;
class ContinueStatement;
class ReturnStatement;
class ThrowStatement;
class SynchronizedStatement;
class AssertStatement;
class AstCatchClause;
class AstFinallyClause;
class TryStatement;
class IntegerLiteral;
class LongLiteral;
class FloatLiteral;
class DoubleLiteral;
class TrueLiteral;
class FalseLiteral;
class StringLiteral;
class CharLiteral;
class NullLiteral;
class ClassLiteralAccess;
class AstThisExpression;
class AstSuperExpression;
class AstParenthesizedExpression;
class AstClassCreationExpression;
class AstDimExpr;
class AstArrayCreationExpression;
class FieldReference;
class AstMethodInvocation;
class ArrayReference;
class AstPostUnaryExpression;
class AstPreUnaryExpression;
class CastExpression;
class BinaryExpression;
class InstanceofExpression;
class ConditionalExpression;
class AstAssignmentExpression;
class ASTVisitor;

} // Close namespace Jikes block


#endif // ast_Pre_declaration_INCLUDED
