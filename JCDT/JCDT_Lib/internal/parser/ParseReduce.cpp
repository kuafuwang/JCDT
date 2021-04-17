
#include "stdafx.h"
#include "JCDT_Lib/internal/env/platform.h"
#include "JCDT_Lib/internal/parser/parser.h"
#include "JCDT_Lib/internal/ast/ast.h"
#include "JCDT_Lib/internal/lex/LexStream.h"
#include <JCDT_Lib/internal/ast/AstAnonymousTypeDeclaration.h>

namespace Jikes { // Open namespace Jikes block


//**************************************************************************//
//**************************************************************************//
//*                                                                        *//
//* Below, we show each rule of the Java grammar together with the         *//
//* semantic action that is invoked when the parser performs a reduction   *//
//* by that rule.                                                          *//
//**************************************************************************//
//**************************************************************************//

//
// The parse was bad. Give up now.
//
void Parser::BadAction() { assert(false); }


//
// Rule 1:  Goal ::= CompilationUnitMarker CompilationUnit
//

//
// Given a rule of the form A ::= x1 x2 ... xn        n >= 1
// Do nothing - Whatever Ast was produced for x1 is inherited by A.
//
void Parser::NoAction() {}



//
// Rule 2:  Goal ::= MethodDeclarationMarker MethodDeclaration
//
 

//
// Rule 3:  Goal ::= AnnotationTypeMemberDeclarationMarker AnnotationTypeMemberDeclaration
//
 
//
// This next rule was added to allow the parser to recognize the body of a
// funtion (constructor, method, or initializer) out of context. Note that
// the artificial terminal BodyMarker is added here to prevent an ordinary
// parse from accepting a body as a valid input - i.e., to recognize a body
// out-of-context, the BodyMarker terminal must be inserted in front of the
// input stream containing the body in question.

//
// Rule 4:  Goal ::= BodyMarker MethodBody
//

//
// Given a rule of the form A ::= x1 x2, inherit the result from x2.
//
void Parser::SetSym1ToSym2() { Sym(1) = Sym(2); }

//
// Rule 5:  Literal ::= IntegerLiteral
//

void Parser::consumeIntegerLiteral()
{
    Sym(1) = ast_pool -> NewIntegerLiteral(GetToken(1));
}

//
// Rule 6:  Literal ::= LongLiteral
//

void Parser::consumeLongLiteral()
{
    Sym(1) = ast_pool -> NewLongLiteral(GetToken(1));
}

//
// Rule 7:  Literal ::= FloatLiteral
//

void Parser::consumeFloatLiteral()
{
    Sym(1) = ast_pool -> NewFloatLiteral(GetToken(1));
}

//
// Rule 8:  Literal ::= DoubleLiteral
//

void Parser::consumeDoubleLiteral()
{
    Sym(1) = ast_pool -> NewDoubleLiteral(GetToken(1));
}

//
// Rule 9:  Literal ::= BooleanLiteral
//
// void NoAction();
//

//
// Rule 10:  Literal ::= CharacterLiteral
//

void Parser::consumeCharacterLiteral()
{
    Sym(1) = ast_pool -> NewCharacterLiteral(GetToken(1));
}

//
// Rule 11:  Literal ::= StringLiteral
//

void Parser::consumeStringLiteral()
{
    Sym(1) = ast_pool -> NewStringLiteral(GetToken(1));
}

//
// Rule 12:  Literal ::= null
//

void Parser::consumeNullLiteral()
{
    Sym(1) = ast_pool -> NewNullLiteral(GetToken(1));
}

//
// Rule 13:  BooleanLiteral ::= true
//

void Parser::consumeBooleanTrueLiteral()
{
    Sym(1) = ast_pool -> NewTrueLiteral(GetToken(1));
}

//
// Rule 14:  BooleanLiteral ::= false
//

void Parser::consumeBooleanFalseLiteral()
{
    Sym(1) = ast_pool -> NewFalseLiteral(GetToken(1));
}

//
// Rule 15:  Type ::= PrimitiveType
//
// void NoAction();
//

//
// Rule 16:  Type ::= ReferenceType
//
// void NoAction();
//

//
// Rule 17:  PrimitiveType ::= NumericType
//
// void NoAction();
//

//
// Rule 18:  PrimitiveType ::= boolean
//

void Parser::consumeBooleanLiteral()
{
    Sym(1) = ast_pool -> NewPrimitiveType(Ast::BOOLEAN, GetToken(1));
}

//
// Rule 19:  NumericType ::= IntegralType
//
// void NoAction();
//

//
// Rule 20:  NumericType ::= FloatingPointType
//
// void NoAction();
//

//
// Rule 21:  IntegralType ::= byte
//

void Parser::consumeToken_byte()
{
    Sym(1) = ast_pool -> NewPrimitiveType(Ast::BYTE, GetToken(1));
}

//
// Rule 22:  IntegralType ::= short
//

void Parser::consumeToken_short()
{
    Sym(1) = ast_pool -> NewPrimitiveType(Ast::SHORT, GetToken(1));
}

//
// Rule 23:  IntegralType ::= int
//

void Parser::consumeToken_int()
{
    Sym(1) = ast_pool -> NewPrimitiveType(Ast::INT, GetToken(1));
}

//
// Rule 24:  IntegralType ::= long
//

void Parser::consumeToken_long()
{
    Sym(1) = ast_pool -> NewPrimitiveType(Ast::LONG, GetToken(1));
}

//
// Rule 25:  IntegralType ::= char
//

void Parser::consumeToken_char()
{
    Sym(1) = ast_pool -> NewPrimitiveType(Ast::CHAR, GetToken(1));
}

//
// Rule 26:  FloatingPointType ::= float
//

void Parser::consumeToken_float()
{
    Sym(1) = ast_pool -> NewPrimitiveType(Ast::FLOAT, GetToken(1));
}

//
// Rule 27:  FloatingPointType ::= double
//

void Parser::consumeToken_double()
{
    Sym(1) = ast_pool -> NewPrimitiveType(Ast::DOUBLE, GetToken(1));
}

//
// Rule 28:  VoidType ::= void
//

void Parser::consumeToken_void()
{
    Sym(1) = ast_pool -> NewPrimitiveType(Ast::VOID_TYPE, GetToken(1));
}

//
// Rule 29:  ReferenceType ::= ClassOrInterfaceType
//
// void NoAction();
//

//
// Rule 30:  ReferenceType ::= ArrayType
//
// void NoAction();
//

//
// Rule 31:  ClassOrInterfaceType ::= ClassOrInterface
//
// void NoAction();
//

//
// Rule 32:  ClassOrInterfaceType ::= ClassOrInterface LESS TypeArgumentList1 Marker
//

void Parser::MakeTypeArguments() { Sym(1) = MakeTypeArguments(1); }

//
// Given AstName/AstType '<' TypeArgumentList1 at tokennum, generate the
// AstTypeName that includes the type arguments. There must be a production at
// tokennum + 3 to allow finding the closing '>'.
//
AstTypeName* Parser::MakeTypeArguments(int tokennum)
{
    AstTypeName* p = AstName::NameCast(Sym(tokennum))
        ? ast_pool -> NewTypeName(DYNAMIC_CAST<AstName*> (Sym(tokennum)))
        : DYNAMIC_CAST<AstTypeName*> (Sym(tokennum));
    AstListNode* tail = DYNAMIC_CAST<AstListNode*> (Sym(tokennum + 2));
    AstTypeArguments* q =
        ast_pool -> NewTypeArguments(GetToken(tokennum + 1),
                                     GetToken(tokennum + 3) - 1);
    q -> AllocateTypeArguments(tail -> index + 1);
    AstListNode* root = tail;
    do
    {
        root = root -> next;
        q -> AddTypeArgument(DYNAMIC_CAST<AstType*> (root -> element));
    } while (root != tail);
    FreeCircularList(tail);
    p -> type_arguments_opt = q;
    return p;
}

//
// Rule 33:  ClassOrInterface ::= Name
//

void Parser::consumeClassOrInterfaceName()
{
    AstTypeName* p = ast_pool -> NewTypeName(DYNAMIC_CAST<AstName*> (Sym(1)));
    Sym(1) = p;
}

//
// Rule 34:  ClassOrInterface ::= ClassOrInterface LESS TypeArgumentList1 DOT Name
//

void Parser::consumeClassOrInterfaceTypeArgumentList1Name()
{
    AstTypeName* p = ast_pool -> NewTypeName(DYNAMIC_CAST<AstName*> (Sym(5)));
    p -> base_opt = MakeTypeArguments(1);
    Sym(1) = p;
}

//
// Rule 35:  ArrayType ::= PrimitiveType Dims
//

void Parser::MakeArrayType() { Sym(1) = MakeArrayType(1); }

//
// Used on variants of "Type Dimsopt". If this type has dimensions, make an
// array type; otherwise return the type name.
//
AstType* Parser::MakeArrayType(int tokennum)
{
    AstType* p = AstName::NameCast(Sym(tokennum))
        ? ast_pool -> NewTypeName(DYNAMIC_CAST<AstName*> (Sym(tokennum)))
        : DYNAMIC_CAST<AstType*> (Sym(tokennum));
    return ! Sym(tokennum + 1) ? p
        : ast_pool -> NewArrayType(p, (DYNAMIC_CAST<AstBrackets*>
                                       (Sym(tokennum + 1))));
}

//
// Rule 36:  ArrayType ::= Name Dims
//
// void MakeArrayType();
//

//
// Rule 37:  ArrayType ::= ClassOrInterface LESS TypeArgumentList1 DOT Name Dims
//

void Parser::consumeClassOrInterfaceTypeArgumentList1NameWithDims()
{
    AstTypeName* p = ast_pool -> NewTypeName(DYNAMIC_CAST<AstName*> (Sym(5)));
    p -> base_opt = MakeTypeArguments(1);
    Sym(5) = p;
    Sym(1) = MakeArrayType(5);
}

//
// Rule 38:  ArrayType ::= ClassOrInterface LESS TypeArgumentList1 Dims
//

void Parser::consumeClassOrInterfaceTypeArgumentList1WithDims()
{
    Sym(3) = MakeTypeArguments(1);
    Sym(1) = MakeArrayType(3);
}

//
// Rule 39:  Name ::= Identifier
//

void Parser::consumeSimpleName() { MakeSimpleName(1); }

//
// Used on "Identifier", and sets the corresponding symbol to a simple name.
//
AstName* Parser::MakeSimpleName(int tokennum)
{
    AstName* name = ast_pool -> NewName(GetToken(tokennum));
    Sym(tokennum) = name;
    return name;
}
 

//
// Rule 40:  Name ::= Name DOT Marker Identifier
//

void Parser::consumeQualifiedName()
{
    AstName* p = ast_pool -> NewName(GetToken(4));
    p -> base_opt = DYNAMIC_CAST<AstName*> (Sym(1));
    Sym(1) = p;
}

//
// Rule 41:  CompilationUnit ::= PackageDeclaration ImportDeclarationsopt TypeDeclarationsopt
//

void Parser::MakeCompilationUnit()
{
   auto p = ast_pool -> NewCompilationUnit();
    p -> package_declaration_opt =
        DYNAMIC_CAST<AstPackageDeclaration*> (Sym(1));
    if (Sym(2))
    {
        AstListNode* tail = DYNAMIC_CAST<AstListNode*> (Sym(2));
        p -> AllocateImportDeclarations(tail -> index + 1);
        AstListNode* root = tail;
        do
        {
            root = root -> next;
            p -> AddImportDeclaration(DYNAMIC_CAST<AstImportDeclaration*>
                                      (root -> element));
        } while (root != tail);
        FreeCircularList(tail);
    }
	auto sym3 = Sym(3);
    if (sym3)
    {
		
        AstListNode* tail = DYNAMIC_CAST<AstListNode*> (sym3);
        p -> AllocateTypeDeclarations(tail -> index + 1);
        AstListNode* root = tail;
        do
        {
            root = root -> next;
			auto ele = root->element;
            p -> AddTypeDeclaration(DYNAMIC_CAST<TypeDeclaration*>
                                    (ele));
        } while (root != tail);
        FreeCircularList(tail);
    }
    Sym(1) = p;
}

//
// Rule 42:  CompilationUnit ::= Marker ImportDeclarations TypeDeclarationsopt
//
// void MakeCompilationUnit();
//

//
// Rule 43:  CompilationUnit ::= TypeDeclarationsopt
//

void Parser::consumeTypeDeclarationsopt()
{
    Sym(3) = Sym(1);
    Sym(1) = NULL;
    Sym(2) = NULL;
    MakeCompilationUnit();
}

//
// Rule 44:  ImportDeclarations ::= ImportDeclaration
//

//
// This starts a list containing a single element.
// Note that the list is circular so as to preserve the order of the elements.
//
void Parser::StartList()
{
    AstListNode* p = AllocateListNode();
    p -> next = p;
    p -> element = Sym(1);
    p -> index = 0;
    Sym(1) = p;
}

//
// Rule 45:  ImportDeclarations ::= ImportDeclarations ImportDeclaration
//

//
// This adds token 2 to an existing list.
// Note that the list is circular so as to preserve the order of the elements.
//
void Parser::AddList2()
{
    AstListNode* tail = DYNAMIC_CAST<AstListNode*> (Sym(1));
    AstListNode* p = AllocateListNode();
    p -> element = Sym(2);
    p -> index = tail -> index + 1;
    p -> next = tail -> next;
    tail -> next = p;
    Sym(1) = p;
}

//
// This adds token 3 to an existing list (thus, token 2 was a delimiter).
// Note that the list is circular so as to preserve the order of the elements.
//
void Parser::AddList3()
{
    AstListNode* tail = DYNAMIC_CAST<AstListNode*> (Sym(1));
    AstListNode* p = AllocateListNode();
    p -> element = Sym(3);
    p -> index = tail -> index + 1;
    p -> next = tail -> next;
    tail -> next = p;
    Sym(1) = p;
}

//
// Rule 46:  ImportDeclarationsopt ::=
//

//
// Given a rule of the form A ::= x1 x2 ... xn
//
// Construct a NULL Ast for A.
//
void Parser::NullAction() { Sym(1) = NULL; }

//
// Rule 47:  ImportDeclarationsopt ::= ImportDeclarations
//
// void NoAction();
//

//
// Rule 48:  TypeDeclarations ::= TypeDeclaration
//
// void StartList();
//

//
// Rule 49:  TypeDeclarations ::= TypeDeclarations TypeDeclaration
//
// void AddList2();
//

//
// Rule 50:  TypeDeclarationsopt ::=
//
// void NullAction();
//

//
// Rule 51:  TypeDeclarationsopt ::= TypeDeclarations
//
// void NoAction();
//

//
// Rule 52:  PackageDeclaration ::= Marker package Name PackageHeaderMarker SEMICOLON
//

void Parser::MakePackageDeclaration()
{
    AstPackageDeclaration* p = ast_pool -> NewPackageDeclaration();
    p -> modifiers_opt = MakeModifiers();
    p -> package_token = GetToken(2);
    p -> name = DYNAMIC_CAST<AstName*> (Sym(3));
    p -> semicolon_token = GetToken(5);
    Sym(1) = p;
}

//
// Converts the list at symbol 1 to AstModifiers.
//
AstModifiers* Parser::MakeModifiers()
{
    if (! Sym(1))
        return NULL;
    AstModifiers* p = ast_pool -> NewModifiers();
    AstListNode* tail = DYNAMIC_CAST<AstListNode*> (Sym(1));
    p -> AllocateModifiers(tail -> index + 1);
    AstListNode* root = tail;
    do
    {
        root = root -> next;
        if (AstModifierKeyword::ModifierKeywordCast(root->element))
        {
            AstModifierKeyword* mod = (AstModifierKeyword*) root -> element;
            p -> AddModifier(mod);
            if (mod -> modifier_token->_kind == TK_static)
                p -> static_token_opt = mod -> modifier_token;
        }
        else p -> AddModifier(DYNAMIC_CAST<AstAnnotation*> (root -> element));
    } while (root != tail);
    FreeCircularList(tail);
    return p;
}

//
// Rule 53:  PackageDeclaration ::= Modifiers package Name PackageHeaderMarker SEMICOLON
//
// void MakePackageDeclaration();
//

//
// Rule 54:  ImportDeclaration ::= SingleTypeImportDeclaration
//
// void NoAction();
//

//
// Rule 55:  ImportDeclaration ::= TypeImportOnDemandDeclaration
//
// void NoAction();
//

//
// Rule 56:  ImportDeclaration ::= SingleStaticImportDeclaration
//
// void NoAction();
//

//
// Rule 57:  ImportDeclaration ::= StaticImportOnDemandDeclaration
//
// void NoAction();
//

//
// Rule 58:  SingleTypeImportDeclaration ::= import Marker Name Marker Marker SEMICOLON
//

void Parser::MakeImportDeclaration()
{
    AstImportDeclaration* p = ast_pool -> NewImportDeclaration();
    p -> import_token = GetToken(1);
    if (GetToken(3)->index > GetToken(2)->index)
        p -> static_token_opt = GetToken(2);
    p -> name = DYNAMIC_CAST<AstName*> (Sym(3));
    if (GetToken(6)->index > GetToken(5)->index)
        p -> star_token_opt = GetToken(5);
    p -> semicolon_token = GetToken(6);
    Sym(1) = p;
}

//
// Rule 59:  TypeImportOnDemandDeclaration ::= import Marker Name DOT MULTIPLY SEMICOLON
//
// void MakeImportDeclaration();
//

//
// Rule 60:  SingleStaticImportDeclaration ::= import static Name Marker Marker SEMICOLON
//
// void MakeImportDeclaration();
//

//
// Rule 61:  StaticImportOnDemandDeclaration ::= import static Name DOT MULTIPLY SEMICOLON
//
// void MakeImportDeclaration();
//

//
// Rule 62:  TypeDeclaration ::= ClassDeclaration
//
// void NoAction();
//

//
// Rule 63:  TypeDeclaration ::= EnumDeclaration
//
// void NoAction();
//

//
// Rule 64:  TypeDeclaration ::= InterfaceDeclaration
//
// void NoAction();
//

//
// Rule 65:  TypeDeclaration ::= AnnotationTypeDeclaration
//
// void NoAction();
//

//
// Rule 66:  TypeDeclaration ::= SEMICOLON
//

void Parser::consumeEmptyTypeDeclaration()
{
    Sym(1) = ast_pool -> NewEmptyDeclaration(GetToken(1));
}

//
// Rule 67:  Modifiers ::= Modifier
//
// void StartList();
//

//
// Rule 68:  Modifiers ::= Modifiers Modifier
//
// void AddList2();
//

//
// Rule 69:  Modifiersopt ::=
//
// void NullAction();
//

//
// Rule 70:  Modifiersopt ::= Modifiers
//
// void NoAction();
//

//
// Rule 71:  Modifier ::= public
//

void Parser::MakeModifier()
{
    Sym(1) = ast_pool -> NewModifierKeyword(GetToken(1));
}

//
// Rule 72:  Modifier ::= protected
//
// void MakeModifier();
//

//
// Rule 73:  Modifier ::= private
//
// void MakeModifier();
//

//
// Rule 74:  Modifier ::= static
//
// void MakeModifier();
//

//
// Rule 75:  Modifier ::= abstract
//
// void MakeModifier();
//

//
// Rule 76:  Modifier ::= final
//
// void MakeModifier();
//

//
// Rule 77:  Modifier ::= native
//
// void MakeModifier();
//

//
// Rule 78:  Modifier ::= strictfp
//
// void MakeModifier();
//

//
// Rule 79:  Modifier ::= synchronized
//
// void MakeModifier();
//

//
// Rule 80:  Modifier ::= transient
//
// void MakeModifier();
//

//
// Rule 81:  Modifier ::= volatile
//
// void MakeModifier();
//

//
// Rule 82:  Modifier ::= Annotation
//
// void NoAction();
//

//
// Rule 83:  Annotation ::= NormalAnnotation
//
// void NoAction();
//

//
// Rule 84:  Annotation ::= MarkerAnnotation
//
// void NoAction();
//

//
// Rule 85:  Annotation ::= SingleMemberAnnotation
//
// void NoAction();
//

//
// Rule 86:  NormalAnnotation ::= AT Name LPAREN MemberValuePairsopt RPAREN
//

void Parser::MakeAnnotation()
{
    AstAnnotation* p = ast_pool -> NewAnnotation();
    p -> at_token = GetToken(1);
    p -> name = DYNAMIC_CAST<AstName*> (Sym(2));
    if (Sym(4))
    {
        AstListNode* tail = DYNAMIC_CAST<AstListNode*> (Sym(4));
        p -> AllocateMemberValuePairs(tail -> index + 1);
        AstListNode* root = tail;
        do
        {
            root = root -> next;
            p -> AddMemberValuePair(DYNAMIC_CAST<AstMemberValuePair*>
                                    (root -> element));
        } while (root != tail);
        FreeCircularList(tail);
    }
    if (GetToken(5)->index > GetToken(3)->index)
        p -> right_paren_token_opt = GetToken(5);
    Sym(1) = p;
}

//
// Rule 87:  MemberValuePairs ::= MemberValuePair
//
// void StartList();
//

//
// Rule 88:  MemberValuePairs ::= MemberValuePairs COMMA MemberValuePair
//
// void AddList3();
//

//
// Rule 89:  MemberValuePairsopt ::=
//
// void NullAction();
//

//
// Rule 90:  MemberValuePairsopt ::= MemberValuePairs
//
// void NoAction();
//

//
// Rule 91:  MemberValuePair ::= Identifier EQUAL MemberValue
//

void Parser::consumeMemberValuePair()
{
    AstMemberValuePair* p = ast_pool -> NewMemberValuePair();
    p -> identifier_token_opt = GetToken(1);
    p -> member_value = DYNAMIC_CAST<AstMemberValue*> (Sym(3));
    Sym(1) = p;
}

//
// Rule 92:  MemberValue ::= ConditionalExpression
//
// void NoAction();
//

//
// Rule 93:  MemberValue ::= Annotation
//
// void NoAction();
//
 

//
// Rule 94:  MemberValue ::= MemberValueArrayInitializer
//
 

//
// Rule 94:  MemberValue ::= MemberValueArrayInitializer
//
// void NoAction();
//

//
// Rule 95:  MemberValueArrayInitializer ::= LBRACE Marker ,opt RBRACE
//

void Parser::MakeArrayInitializer()
{
    AstArrayInitializer* p = ast_pool -> NewArrayInitializer();
    p -> left_brace_token = GetToken(1);
    if (Sym(2))
    {
        AstListNode* tail = DYNAMIC_CAST<AstListNode*> (Sym(2));
        p -> AllocateVariableInitializers(tail -> index + 1);
        AstListNode* root = tail;
        do
        {
            root = root -> next;
            p -> AddVariableInitializer(DYNAMIC_CAST<AstMemberValue*>
                                        (root -> element));
        } while (root != tail);
        FreeCircularList(tail);
    }
    p -> right_brace_token = GetToken(4);
    Sym(1) = p;
}

//
// Rule 96:  MemberValueArrayInitializer ::= LBRACE MemberValues ,opt RBRACE
//
// void MakeArrayInitializer();
//

//
// Rule 97:  MemberValues ::= MemberValue
//
// void StartList();
//

//
// Rule 98:  MemberValues ::= MemberValues COMMA MemberValue
//
// void AddList3();
//

//
// Rule 99:  MarkerAnnotation ::= AT Name Marker Marker Marker
//
// void MakeAnnotation();
//

//
// Rule 100:  SingleMemberAnnotation ::= AT Name LPAREN MemberValue RPAREN
//

void Parser::consumeSingleMemberAnnotation()
{
    AstMemberValuePair* mvp = ast_pool -> NewMemberValuePair();
    mvp -> member_value = DYNAMIC_CAST<AstMemberValue*> (Sym(4));
    AstListNode* p = AllocateListNode();
    p -> next = p;
    p -> element = mvp;
    p -> index = 0;
    Sym(4) = p;
    MakeAnnotation();
}

//
// Rule 101:  ClassDeclaration ::= Marker class Identifier TypeParametersopt Superopt Interfacesopt ClassBody
//

void Parser::MakeClassDeclaration()
{
    ClassTypeDeclaration* p = ast_pool -> NewClassDeclaration();
    p -> modifiers_opt = MakeModifiers();
    p -> class_token = GetToken(2);
    p -> type_parameters_opt = MakeTypeParameters(4);
    p -> super_opt = DYNAMIC_CAST<AstTypeName*> (Sym(5));
    if (Sym(6))
    {
        AstListNode* tail = DYNAMIC_CAST<AstListNode*> (Sym(6));
        p -> AllocateInterfaces(tail -> index + 1);
        AstListNode* root = tail;
        do
        {
            root = root -> next;
            p -> AddInterface(DYNAMIC_CAST<AstTypeName*> (root -> element));
        } while (root != tail);
        FreeCircularList(tail);
    }
    p -> class_body = DYNAMIC_CAST<AstClassBody*> (Sym(7));
    p -> class_body -> identifier_token = GetToken(3);
    p -> class_body -> owner = p;
    Sym(1) = p;
}

//
// Creates an AstTypeParameters node for insertion into an AstTypeName.
//
AstTypeParameters* Parser::MakeTypeParameters(int tokennum)
{
    if (! Sym(tokennum))
        return NULL;
    AstTypeParameters* p = ast_pool -> NewTypeParameters();
    AstListNode* tail = DYNAMIC_CAST<AstListNode*> (Sym(tokennum));
    p -> AllocateTypeParameters(tail -> index + 1);
    AstListNode* root = tail;
    do
    {
        root = root -> next;
        p -> AddTypeParameter(DYNAMIC_CAST<AstTypeParameter*>
                              (root -> element));
    } while (root != tail);
    FreeCircularList(tail);
    p -> right_angle_token = GetToken(tokennum + 1) - 1;
    return p;
}

//
// Rule 102:  ClassDeclaration ::= Modifiers class Identifier TypeParametersopt Superopt Interfacesopt ClassBody
//
// void MakeClassDeclaration();
//

//
// Rule 103:  Super ::= extends ClassOrInterfaceType
//
// void SetSym1ToSym2();
//

//
// Rule 104:  Superopt ::=
//
// void NullAction();
//

//
// Rule 105:  Superopt ::= Super
//
// void NoAction();
//

//
// Rule 106:  Interfaces ::= implements TypeList
//
// void SetSym1ToSym2();
//

//
// Rule 107:  Interfacesopt ::=
//
// void NullAction();
//

//
// Rule 108:  Interfacesopt ::= Interfaces
//
// void NoAction();
//

//
// Rule 109:  TypeList ::= ClassOrInterfaceType
//
// void StartList();
//

//
// Rule 110:  TypeList ::= TypeList COMMA ClassOrInterfaceType
//
// void AddList3();
//

 

//
// Rule 111:  ClassBody ::= LBRACE ClassBodyDeclarationsopt RBRACE
//

void Parser::MakeClassBody()
{
    AstClassBody* p = ast_pool -> NewClassBody();
    if (parse_header_only)
        p -> MarkUnparsed();
	else
	{
		auto temp = parse_header_only;

		
	}

    p -> left_brace_token = GetToken(1);
    if (Sym(2))
    {
        unsigned num_instance_variables = 0;
        unsigned num_class_variables = 0;
        unsigned num_methods = 0;
        unsigned num_constructors = 0;
        unsigned num_static_initializers = 0;
        unsigned num_instance_initializers = 0;
        unsigned num_inner_classes = 0;
        unsigned num_inner_enums = 0;
        unsigned num_inner_interfaces = 0;
        unsigned num_inner_annotations = 0;
        unsigned num_empty_declarations = 0;

        AstListNode* tail = DYNAMIC_CAST<AstListNode*> (Sym(2));
        p -> AllocateClassBodyDeclarations(tail -> index + 1);
        AstListNode* root = tail;
        do
        {
            root = root -> next;
            Ast* declaration =
                DYNAMIC_CAST<Ast*> (root -> element);
            FieldDeclaration* field_declaration =
                FieldDeclaration::FieldDeclarationCast(declaration);
            Initializer* initializer =
                Initializer::InitializerDeclarationCast(declaration);
            if (field_declaration)
            {
                if (field_declaration -> modifiers_opt &&
                    field_declaration -> modifiers_opt -> static_token_opt)
                {
                    field_declaration -> MarkStatic();
                }
                //
                // Interface fields were already marked static.
                //
                if (FieldDeclaration::StaticFieldCast(field_declaration))
                    num_class_variables++;
                else num_instance_variables++;
            }
            else if (MethodDeclaration::MethodDeclarationCast(declaration))
                num_methods++;
            else if (ConstructorDeclaration::ConstructorDeclarationCast(declaration))
                num_constructors++;
            else if (initializer)
            {
                if (initializer -> modifiers_opt &&
                    initializer -> modifiers_opt -> static_token_opt)
                {
                    initializer -> MarkStatic();
                    num_static_initializers++;
                }
                else num_instance_initializers++;
            }
            else if (ClassTypeDeclaration::ClassDeclarationCast(declaration))
                num_inner_classes++;
            else if (EnumTypeDeclaration::EnumDeclarationCast(declaration))
                num_inner_enums++;
            else if ( InterfaceTypeDeclaration::InterfaceDeclarationCast(declaration))
                num_inner_interfaces++;
            else if (AnnotationTypeDeclaration::AnnotationDeclarationCast(declaration))
                num_inner_annotations++;
			else if(EmptyTypeDeclaration::EmptyTypeDeclarationCast(declaration) ){
				num_empty_declarations++;
			}
			else{
				assert(false);
			}
        } while (root != tail);

        p -> AllocateInstanceVariables(num_instance_variables);
        p -> AllocateClassVariables(num_class_variables);
        p -> AllocateMethods(num_methods);
        p -> AllocateConstructors(num_constructors);
        p -> AllocateStaticInitializers(num_static_initializers);
        p -> AllocateInstanceInitializers(num_instance_initializers);
        p -> AllocateNestedClasses(num_inner_classes);
        p -> AllocateNestedEnums(num_inner_enums);
        p -> AllocateNestedInterfaces(num_inner_interfaces);
        p -> AllocateNestedAnnotations(num_inner_annotations);
        p -> AllocateEmptyDeclarations(num_empty_declarations);

        root = tail;
        do
        {
            root = root -> next;
            p -> AddClassBodyDeclaration(DYNAMIC_CAST<Ast*>
                                         (root -> element));
        } while (root != tail);
        FreeCircularList(tail);
    }
    p -> right_brace_token = GetToken(3);
    // from now on, this is the storage pool to use for this type
    p -> pool = body_pool;
    Sym(1) = p;
}

//
// Rule 112:  ClassBodyopt ::=
//
// void NullAction();
//

 

//
// Rule 113:  ClassBodyopt ::= ClassBody
//
 

//
// Rule 113:  ClassBodyopt ::= ClassBody
//
// void NoAction();
//

//
// Rule 114:  ClassBodyDeclarations ::= ClassBodyDeclaration
//
// void StartList();
//

//
// Rule 115:  ClassBodyDeclarations ::= ClassBodyDeclarations ClassBodyDeclaration
//
// void AddList2();
//

//
// Rule 116:  ClassBodyDeclarationsopt ::=
//
// void NullAction();
//

//
// Rule 117:  ClassBodyDeclarationsopt ::= ClassBodyDeclarations
//
// void NoAction();
//

//
// Rule 118:  ClassBodyDeclaration ::= ConstructorDeclaration
//
// void NoAction();
//

//
// Rule 119:  ClassBodyDeclaration ::= InitializerDeclaration
//
// void NoAction();
//

//
// Rule 120:  ClassBodyDeclaration ::= FieldDeclaration
//
// void NoAction();
//

//
// Rule 121:  ClassBodyDeclaration ::= MethodDeclaration
//
// void NoAction();
//

//
// Rule 122:  ClassBodyDeclaration ::= TypeDeclaration
//
// void NoAction();
//

//
// Rule 123:  FieldDeclaration ::= Marker Marker Type VariableDeclarators SEMICOLON
//

void Parser::MakeFieldDeclaration()
{
    FieldDeclaration* p = ast_pool -> NewFieldDeclaration();
    p -> modifiers_opt = MakeModifiers();
    p -> type = DYNAMIC_CAST<AstType*> (Sym(3));
    AstListNode* tail = DYNAMIC_CAST<AstListNode*> (Sym(4));
    p -> AllocateVariableDeclarators(tail -> index + 1);
    AstListNode* root = tail;
    do
    {
        root = root -> next;
        p -> AddVariableDeclarator(DYNAMIC_CAST<AstVariableDeclarator*>
                                   (root -> element));
    } while (root != tail);
    FreeCircularList(tail);
    p -> semicolon_token = GetToken(5);
    Sym(1) = p;
}

//
// Rule 124:  FieldDeclaration ::= Modifiers Marker Type VariableDeclarators SEMICOLON
//
// void MakeFieldDeclaration();
//

//
// Rule 125:  VariableDeclarators ::= VariableDeclarator
//
// void StartList();
//
 

//
// Rule 126:  VariableDeclarators ::= VariableDeclarators COMMA VariableDeclarator
//
 

//
// Rule 126:  VariableDeclarators ::= VariableDeclarators COMMA VariableDeclarator
//
// void AddList3();
//

//
// Rule 127:  VariableDeclarator ::= VariableDeclaratorId
//

void Parser::consumeVariableDeclaratorOnlyId()
{
    AstVariableDeclarator* p = ast_pool -> NewVariableDeclarator();
    p -> variable_declarator_name =
        DYNAMIC_CAST<AstVariableDeclaratorId*> (Sym(1));
    Sym(1) = p;
}

//
// Rule 128:  VariableDeclarator ::= VariableDeclaratorId EQUAL VariableInitializer
//

void Parser::consumeVariableDeclaratorWithVariableInitializer()
{
    AstVariableDeclarator* p = ast_pool -> NewVariableDeclarator();
    p -> variable_declarator_name =
        DYNAMIC_CAST<AstVariableDeclaratorId*> (Sym(1));
    p -> variable_initializer_opt = Sym(3);
    Sym(1) = p;
}
 

//
// Rule 129:  VariableDeclaratorId ::= Identifier Dimsopt
//

void Parser::consumeVariableDeclaratorId()
{
    AstVariableDeclaratorId* p = ast_pool -> NewVariableDeclaratorId();
    p -> identifier_token = GetToken(1);
    p -> brackets_opt = DYNAMIC_CAST<AstBrackets*> (Sym(2));
    Sym(1) = p;
}

//
// Rule 130:  VariableInitializer ::= Expression
//
// void NoAction();
//
 

//
// Rule 131:  VariableInitializer ::= ArrayInitializer
//
 

//
// Rule 131:  VariableInitializer ::= ArrayInitializer
//
// void NoAction();
//

//
// Rule 132:  MethodDeclaration ::= MethodHeader MethodHeaderMarker MethodBody
//

void Parser::MakeMethodDeclaration()
{
    MethodDeclaration* p = DYNAMIC_CAST<MethodDeclaration*> (Sym(1));
    if (Sym(3))
    {
		p->method_body_opt = DYNAMIC_CAST<AstMethodBody*>(Sym(3));
		if(p->method_body_opt)
			p->method_body_opt->owner_ast = p;
		
    }
	else
	{
		p->semicolon_token_opt = GetToken(4);
	}
    
	
}

//
// Rule 133:  MethodDeclaration ::= MethodHeader MethodHeaderMarker Marker SEMICOLON
//
// void MakeMethodDeclaration();
//

//
// Rule 134:  MethodHeader ::= Marker Marker Type MethodDeclarator Throwsopt
//

void Parser::MakeMethodHeader()
{
    MethodDeclaration* p = ast_pool -> NewMethodDeclaration();
    p -> modifiers_opt = MakeModifiers();
    p -> type_parameters_opt = MakeTypeParameters(2);
    p -> type = DYNAMIC_CAST<AstType*> (Sym(3));
    p -> method_declarator = DYNAMIC_CAST<AstMethodDeclarator*> (Sym(4));
    if (Sym(5))
    {
        AstListNode* tail = DYNAMIC_CAST<AstListNode*> (Sym(5));
        p -> AllocateThrows(tail -> index + 1);
        AstListNode* root = tail;
        do
        {
            root = root -> next;
            p -> AddThrow(DYNAMIC_CAST<AstTypeName*> (root -> element));
        } while (root != tail);
        FreeCircularList(tail);
    }
    Sym(1) = p;
}

//
// Rule 135:  MethodHeader ::= Modifiers Marker Type MethodDeclarator Throwsopt
//
// void MakeMethodHeader();
//

//
// Rule 136:  MethodHeader ::= Marker TypeParameters Type MethodDeclarator Throwsopt
//
// void MakeMethodHeader();
//

//
// Rule 137:  MethodHeader ::= Modifiers TypeParameters Type MethodDeclarator Throwsopt
//
// void MakeMethodHeader();
//

//
// Rule 138:  MethodHeader ::= Marker Marker VoidType MethodDeclarator Throwsopt
//
// void MakeMethodHeader();
//

//
// Rule 139:  MethodHeader ::= Modifiers Marker VoidType MethodDeclarator Throwsopt
//
// void MakeMethodHeader();
//

//
// Rule 140:  MethodHeader ::= Marker TypeParameters VoidType MethodDeclarator Throwsopt
//
// void MakeMethodHeader();
//

//
// Rule 141:  MethodHeader ::= Modifiers TypeParameters VoidType MethodDeclarator Throwsopt
//
// void MakeMethodHeader();
//

//
// Rule 142:  MethodDeclarator ::= Identifier LPAREN FormalParameterListopt RPAREN Dimsopt
//

void Parser::MakeMethodDeclarator()
{
    AstMethodDeclarator* p = ast_pool -> NewMethodDeclarator();
    p -> identifier_token = GetToken(1);
    p -> left_parenthesis_token = GetToken(2);
    if (Sym(3))
    {
        AstListNode* tail = DYNAMIC_CAST<AstListNode*> (Sym(3));
        p -> AllocateFormalParameters(tail -> index + 1);
        AstListNode* root = tail;
        do
        {
            root = root -> next;
            p -> AddFormalParameter(DYNAMIC_CAST<AstFormalParameter*>
                                    (root -> element));
        } while (root != tail);
        FreeCircularList(tail);
    }
    p -> right_parenthesis_token = GetToken(4);
    p -> brackets_opt = DYNAMIC_CAST<AstBrackets*> (Sym(5));
    Sym(1) = p;
}

//
// Rule 143:  FormalParameterList ::= LastFormalParameter
//
// void StartList();
//

//
// Rule 144:  FormalParameterList ::= FormalParameters COMMA LastFormalParameter
//
// void AddList3();
//

//
// Rule 145:  FormalParameterListopt ::=
//
// void NullAction();
//

//
// Rule 146:  FormalParameterListopt ::= FormalParameterList
//
// void NoAction();
//

//
// Rule 147:  FormalParameters ::= FormalParameter
//
// void StartList();
//

//
// Rule 148:  FormalParameters ::= FormalParameters COMMA FormalParameter
//
// void AddList3();
//

//
// Rule 149:  FormalParameter ::= Type Marker Marker VariableDeclaratorId
//

void Parser::MakeFormalParameter()
{
    AstFormalParameter* p = ast_pool -> NewFormalParameter();
    if (Sym(2))
    {
        p -> modifiers_opt = MakeModifiers();
        p -> type = DYNAMIC_CAST<AstType*> (Sym(2));
    }
    else p -> type = DYNAMIC_CAST<AstType*> (Sym(1));
    if (GetToken(4)->index > GetToken(3)->index)
        p -> ellipsis_token_opt = GetToken(3);
    AstVariableDeclarator* formal_declarator =
        ast_pool -> NewVariableDeclarator();
    formal_declarator -> variable_declarator_name =
        DYNAMIC_CAST<AstVariableDeclaratorId*> (Sym(4));
    p -> formal_declarator = formal_declarator;
	formal_declarator->owner = p;
    Sym(1) = p;
}

//
// Rule 150:  FormalParameter ::= Modifiers Type Marker VariableDeclaratorId
//
// void MakeFormalParameter();
//

//
// Rule 151:  LastFormalParameter ::= FormalParameter
//
// void NoAction();
//

//
// Rule 152:  LastFormalParameter ::= Type Marker ELLIPSIS VariableDeclaratorId
//
// void MakeFormalParameter();
//
 

//
// Rule 153:  LastFormalParameter ::= Modifiers Type ELLIPSIS VariableDeclaratorId
//
 

//
// Rule 153:  LastFormalParameter ::= Modifiers Type ELLIPSIS VariableDeclaratorId
//
// void MakeFormalParameter();
//

//
// Rule 154:  Throws ::= throws TypeList
//
// void SetSym1ToSym2();
//

//
// Rule 155:  Throwsopt ::=
//
// void NullAction();
//

//
// Rule 156:  Throwsopt ::= Throws
//
// void NoAction();
//


//
// Rule 157:  MethodBody ::= LBRACE BlockStatementsopt RBRACE
//

void Parser::consumeMethodBody()
{
    AstMethodBody* p = ast_pool -> NewMethodBody();
	if (has_localType_inScope)
	{
		p->HasLocalTypeMASK = true;
		has_localType_inScope = false;
	}
		

    p -> left_brace_token = GetToken(1);
    p -> right_brace_token = GetToken(3);

    if (Sym(2))
    {
        AstListNode* tail = DYNAMIC_CAST<AstListNode*> (Sym(2));
        // Allocate 1 extra for possible generated return statement.
        p -> AllocateStatements(tail -> index + 2);
        AstListNode* root = tail -> next;
        if (root -> element -> IsExplicitConstructorInvocation())
            p -> explicit_constructor_opt =
                DYNAMIC_CAST<ReachComleteAbleStatement*> (root -> element);
        else
            p -> AddStatement(DYNAMIC_CAST<ReachComleteAbleStatement*> (root -> element));
        while (root != tail)
        {
            root = root -> next;
            p -> AddStatement(DYNAMIC_CAST<ReachComleteAbleStatement*> (root -> element));
        }
        FreeCircularList(tail);
    }
    else p -> AllocateStatements(1);
    Sym(1) = p;
}

//
// Rule 158:  InitializerDeclaration ::= Marker MethodHeaderMarker MethodBody
//

void Parser::MakeInitializerDeclaration()
{
    Initializer* p = ast_pool -> NewInitializerDeclaration();
    p -> modifiers_opt = MakeModifiers();
    p -> block = DYNAMIC_CAST<AstMethodBody*> (Sym(3));
    Sym(1) = p;
}

//
// Rule 159:  InitializerDeclaration ::= Modifiers MethodHeaderMarker MethodBody
//
// void MakeInitializerDeclaration();
//

//
// Rule 160:  ConstructorDeclaration ::= Marker Marker ConstructorDeclarator Throwsopt MethodHeaderMarker MethodBody
//

void Parser::MakeConstructorDeclaration()
{
    ConstructorDeclaration* p = ast_pool -> NewConstructorDeclaration();
    p -> modifiers_opt = MakeModifiers();
    p -> type_parameters_opt = MakeTypeParameters(2);
    p ->method_declarator = DYNAMIC_CAST<AstMethodDeclarator*> (Sym(3));
    if (Sym(4))
    {
        AstListNode* tail = DYNAMIC_CAST<AstListNode*> (Sym(4));
        p -> AllocateThrows(tail -> index + 1);
        AstListNode* root = tail;
        do
        {
            root = root -> next;
            p -> AddThrow(DYNAMIC_CAST<AstTypeName*> (root -> element));
        } while (root != tail);
        FreeCircularList(tail);
    }

    p -> constructor_body = DYNAMIC_CAST<AstMethodBody*> (Sym(6));
	if(p->constructor_body)
		p->constructor_body->owner_ast = p;

    Sym(1) = p;
}

//
// Rule 161:  ConstructorDeclaration ::= Modifiers Marker ConstructorDeclarator Throwsopt MethodHeaderMarker MethodBody
//
// void MakeConstructorDeclaration();
//

//
// Rule 162:  ConstructorDeclaration ::= Marker TypeParameters ConstructorDeclarator Throwsopt MethodHeaderMarker MethodBody
//
// void MakeConstructorDeclaration();
//

//
// Rule 163:  ConstructorDeclaration ::= Modifiers TypeParameters ConstructorDeclarator Throwsopt MethodHeaderMarker...
//
// void MakeConstructorDeclaration();
//

//
// Rule 164:  ConstructorDeclarator ::= Identifier LPAREN FormalParameterListopt RPAREN Marker
//
// void MakeMethodDeclarator();
//

//
// Rule 165:  ExplicitConstructorInvocation ::= this Arguments SEMICOLON
//

void Parser::consumeExplicitConstructorThisCallInvocation()
{
    AstThisCall* p = ast_pool -> NewThisCall();
    p -> this_token = GetToken(1);
    p -> arguments = DYNAMIC_CAST<AstArguments*> (Sym(2));
    p -> semicolon_token = GetToken(3);
    Sym(1) = p;
}

//
// Rule 166:  ExplicitConstructorInvocation ::= TypeArguments this Arguments SEMICOLON
//

void Parser::consumeExplicitConstructorThisCallInvocationWithTypeArguments()
{
    AstThisCall* p = ast_pool -> NewThisCall();
    p -> type_arguments_opt = MakeExplicitTypeArguments(1);
    p -> this_token = GetToken(2);
    p -> arguments = DYNAMIC_CAST<AstArguments*> (Sym(3));
    p -> semicolon_token = GetToken(4);
    Sym(1) = p;
}

//
// Given TypeArgumentsopt at tokennum, generate the AstTypeArguments for use
// in a method call's explicit type arguments. There must be a production
// at tokennum + 1 to allow finding the closing '>'.
//
AstTypeArguments* Parser::MakeExplicitTypeArguments(int tokennum)
{
    if (! Sym(tokennum))
        return NULL;
    AstTypeArguments* p =
        ast_pool -> NewTypeArguments(GetToken(tokennum),
                                     GetToken(tokennum + 1) - 1);
    AstListNode* tail = DYNAMIC_CAST<AstListNode*> (Sym(tokennum));
    p -> AllocateTypeArguments(tail -> index + 1);
    AstListNode* root = tail;
    do
    {
        root = root -> next;
        p -> AddTypeArgument(DYNAMIC_CAST<AstType*> (root -> element));
    } while (root != tail);
    FreeCircularList(tail);
    return p;
}

//
// Rule 167:  ExplicitConstructorInvocation ::= super Arguments SEMICOLON
//

void Parser::consumeExplicitConstructorSuperCallInvocation()
{
    AstSuperCall* p = ast_pool -> NewSuperCall();
    p -> super_token = GetToken(1);
    p -> arguments = DYNAMIC_CAST<AstArguments*> (Sym(2));
    p -> semicolon_token = GetToken(3);
    Sym(1) = p;
}

//
// Rule 168:  ExplicitConstructorInvocation ::= TypeArguments super Arguments SEMICOLON
//

void Parser::consumeExplicitConstructorSuperCallInvocationWithTypeArguments()
{
    AstSuperCall* p = ast_pool -> NewSuperCall();
    p -> type_arguments_opt = MakeExplicitTypeArguments(1);
    p -> super_token = GetToken(2);
    p -> arguments = DYNAMIC_CAST<AstArguments*> (Sym(3));
    p -> semicolon_token = GetToken(4);
    Sym(1) = p;
}

//
// Rule 169:  ExplicitConstructorInvocation ::= Primary DOT TypeArgumentsopt super Arguments SEMICOLON
//

void Parser::MakeQualifiedSuper()
{
    AstSuperCall* p = ast_pool -> NewSuperCall();
    p -> base_opt = DYNAMIC_CAST<Expression*> (Sym(1));
    p -> type_arguments_opt = MakeExplicitTypeArguments(3);
    p -> super_token = GetToken(4);
    p -> arguments = DYNAMIC_CAST<AstArguments*> (Sym(5));
    p -> semicolon_token = GetToken(6);
    Sym(1) = p;
}

//
// Rule 170:  ExplicitConstructorInvocation ::= Name DOT Marker super Arguments SEMICOLON
//
// void MakeQualifiedSuper();
//

//
// Rule 171:  ExplicitConstructorInvocation ::= Name DOT TypeArguments super Arguments SEMICOLON
//
// void MakeQualifiedSuper();
//

//
// Rule 172:  EnumDeclaration ::= Marker enum Identifier Interfacesopt EnumBody
//

void Parser::MakeEnumDeclaration()
{
    //
    // Because of how we handle EnumConstants, the EnumBody production already
    // created the EnumTypeDeclaration, and populated the class_body field.
    //
    EnumTypeDeclaration* p = DYNAMIC_CAST<EnumTypeDeclaration*> (Sym(5));
    p -> modifiers_opt = MakeModifiers();
    p -> enum_token = GetToken(2);
    p -> class_body -> identifier_token = GetToken(3);
    if (Sym(4))
    {
        AstListNode* tail = DYNAMIC_CAST<AstListNode*> (Sym(4));
        p -> AllocateInterfaces(tail -> index + 1);
        AstListNode* root = tail;
        do
        {
            root = root -> next;
            p -> AddInterface(DYNAMIC_CAST<AstTypeName*> (root -> element));
        } while (root != tail);
        FreeCircularList(tail);
    }
    p -> class_body -> identifier_token = GetToken(3);
    Sym(1) = p;
}

//
// Rule 173:  EnumDeclaration ::= Modifiers enum Identifier Interfacesopt EnumBody
//
// void MakeEnumDeclaration();
//

//
// Rule 174:  EnumBody ::= LBRACE Marker ,opt EnumBodyDeclarationsopt RBRACE
//

void Parser::MakeEnumBody()
{
    //
    // The class_body was either created in EnumBodyDeclarations, or we must
    // generate it here.
    //
    EnumTypeDeclaration* p = ast_pool -> NewEnumDeclaration();
    if (Sym(2))
    {
        AstListNode* tail = DYNAMIC_CAST<AstListNode*> (Sym(2));
        p -> AllocateEnumConstants(tail -> index + 1);
        AstListNode* root = tail;
        do
        {
            root = root -> next;
            p -> AddEnumConstant(DYNAMIC_CAST<AstEnumConstant*>
                                 (root -> element));
        } while (root != tail);
        FreeCircularList(tail);
    }
    if (Sym(4))
    {
        p -> class_body = DYNAMIC_CAST<AstClassBody*> (Sym(4));
    }
    else
    {
        p -> class_body = ast_pool -> NewClassBody();
        p -> class_body -> right_brace_token = GetToken(5);
        p -> class_body -> pool = body_pool;
    }
    p -> class_body -> left_brace_token = GetToken(1);
    p -> class_body -> owner = p;
    Sym(1) = p;
}

//
// Rule 175:  EnumBody ::= LBRACE EnumConstants ,opt EnumBodyDeclarationsopt RBRACE
//
// void MakeEnumBody();
//

//
// Rule 176:  EnumConstants ::= EnumConstant
//
// void StartList();
//

//
// Rule 177:  EnumConstants ::= EnumConstants COMMA EnumConstant
//
// void AddList3();
//

//
// Rule 178:  EnumConstant ::= Modifiersopt Identifier Argumentsopt ClassBodyopt
//

void Parser::consumeEnumConstantWithClassBodyOpt()
{
    AstEnumConstant* p = ast_pool -> NewEnumConstant(GetToken(2));
    p -> modifiers_opt = MakeModifiers();
    p -> arguments_opt = DYNAMIC_CAST<AstArguments*> (Sym(3));
    p -> class_body_opt = DYNAMIC_CAST<AstClassBody*> (Sym(4));
    Sym(1) = p;
}

//
// Rule 179:  Arguments ::= LPAREN ArgumentListopt RPAREN
//

void Parser::consumeArguments()
{
    AstArguments* p = ast_pool -> NewArguments(GetToken(1), GetToken(3));
    if (Sym(2))
    {
        AstListNode* tail = DYNAMIC_CAST<AstListNode*> (Sym(2));
        p -> AllocateArguments(tail -> index + 1);
        AstListNode* root = tail;
        do
        {
            root = root -> next;
            p -> AddArgument(DYNAMIC_CAST<Expression*> (root -> element));
        } while (root != tail);
        FreeCircularList(tail);
    }
    Sym(1) = p;
}

//
// Rule 180:  Argumentsopt ::=
//
// void NullAction();
//

//
// Rule 181:  Argumentsopt ::= Arguments
//
// void NoAction();
//

//
// Rule 182:  EnumBodyDeclarations ::= SEMICOLON ClassBodyDeclarationsopt Marker
//
// void MakeClassBody();
//

//
// Rule 183:  EnumBodyDeclarationsopt ::=
//
// void NullAction();
//

//
// Rule 184:  EnumBodyDeclarationsopt ::= EnumBodyDeclarations
//
// void NoAction();
//

//
// Rule 185:  InterfaceDeclaration ::= Marker interface Identifier TypeParametersopt ExtendsInterfacesopt InterfaceBody
//

void Parser::MakeInterfaceDeclaration()
{
    InterfaceTypeDeclaration* p = ast_pool -> NewInterfaceDeclaration();
    p -> modifiers_opt = MakeModifiers();
    p -> interface_token = GetToken(2);
    p -> type_parameters_opt = MakeTypeParameters(4);
    if (Sym(5))
    {
        AstListNode* tail = DYNAMIC_CAST<AstListNode*> (Sym(5));
        p -> AllocateInterfaces(tail -> index + 1);
        AstListNode* root = tail;
        do
        {
            root = root -> next;
            p -> AddInterface(DYNAMIC_CAST<AstTypeName*> (root -> element));
        } while (root != tail);
        FreeCircularList(tail);
    }
    p -> class_body = DYNAMIC_CAST<AstClassBody*> (Sym(6));
    p -> class_body -> identifier_token = GetToken(3);
    p -> class_body -> owner = p;
    Sym(1) = p;
}

//
// Rule 186:  InterfaceDeclaration ::= Modifiers interface Identifier TypeParametersopt ExtendsInterfacesopt InterfaceBody
//
// void MakeInterfaceDeclaration();
//

//
// Rule 187:  ExtendsInterfaces ::= extends TypeList
//
// void SetSym1ToSym2();
//

//
// Rule 188:  ExtendsInterfacesopt ::=
//
// void NullAction();
//

//
// Rule 189:  ExtendsInterfacesopt ::= ExtendsInterfaces
//
// void NoAction();
//

//
// Rule 190:  InterfaceBody ::= LBRACE InterfaceMemberDeclarationsopt RBRACE
//
// void MakeClassBody();
//

//
// Rule 191:  InterfaceMemberDeclarations ::= InterfaceMemberDeclaration
//
// void StartList();
//

//
// Rule 192:  InterfaceMemberDeclarations ::= InterfaceMemberDeclarations InterfaceMemberDeclaration
//
// void AddList2();
//

//
// Rule 193:  InterfaceMemberDeclarationsopt ::=
//
// void NullAction();
//

//
// Rule 194:  InterfaceMemberDeclarationsopt ::= InterfaceMemberDeclarations
//
// void NoAction();
//

//
// Rule 195:  InterfaceMemberDeclaration ::= ConstantDeclaration
//
// void NoAction();
//

//
// Rule 196:  InterfaceMemberDeclaration ::= TypeDeclaration
//
// void NoAction();
//

//
// Rule 197:  ConstantDeclaration ::= FieldDeclaration
//

void Parser::consumeConstantDeclaration()
{
    DYNAMIC_CAST<FieldDeclaration*> (Sym(1)) -> MarkStatic();
}

//
// Rule 198:  InterfaceMemberDeclaration ::= MethodDeclaration
//
// void NoAction();
//

//
// Rule 199:  AnnotationTypeDeclaration ::= AT Marker interface Identifier AnnotationTypeBody
//

void Parser::MakeAnnotationTypeDeclaration()
{
    AnnotationTypeDeclaration* p =
        ast_pool -> NewAnnotationDeclaration(GetToken(3));
    if (GetToken(3)->index > GetToken(2)->index)
        p -> modifiers_opt = MakeModifiers();
    p -> class_body = DYNAMIC_CAST<AstClassBody*> (Sym(5));
    p -> class_body -> identifier_token = GetToken(4);
    p -> class_body -> owner = p;
    Sym(1) = p;
}

//
// Rule 200:  AnnotationTypeDeclaration ::= Modifiers AT interface Identifier AnnotationTypeBody
//
// void MakeAnnotationTypeDeclaration();
//

//
// Rule 201:  AnnotationTypeBody ::= LBRACE AnnotationTypeMemberDeclarationsopt RBRACE
//
// void MakeClassBody();
//

//
// Rule 202:  AnnotationTypeMemberDeclarations ::= AnnotationTypeMemberDeclaration
//
// void StartList();
//

//
// Rule 203:  AnnotationTypeMemberDeclarations ::= AnnotationTypeMemberDeclarations AnnotationTypeMemberDeclaration
//
// void AddList2();
//

//
// Rule 204:  AnnotationTypeMemberDeclarationsopt ::=
//
// void NullAction();
//

//
// Rule 205:  AnnotationTypeMemberDeclarationsopt ::= AnnotationTypeMemberDeclarations
//
// void NoAction();
//

//
// Rule 206:  AnnotationTypeMemberDeclaration ::= Marker Marker Type Identifier LPAREN RPAREN DefaultValueopt SEMICOLON
//

void Parser::MakeAnnotationTypeMemberDeclaration()
{
    MethodDeclaration* p = ast_pool -> NewMethodDeclaration();
    p -> modifiers_opt = MakeModifiers();
    p -> type = DYNAMIC_CAST<AstType*> (Sym(3));
    AstMethodDeclarator* q = ast_pool -> NewMethodDeclarator();
    q -> identifier_token = GetToken(4);
    q -> left_parenthesis_token = GetToken(5);
    q -> right_parenthesis_token = GetToken(6);
    p -> method_declarator = q;
    p -> default_value_opt = DYNAMIC_CAST<AstMemberValue*> (Sym(7));
    p -> semicolon_token_opt = GetToken(8);
    Sym(1) = p;
}

//
// Rule 207:  AnnotationTypeMemberDeclaration ::= Modifiers Marker Type Identifier LPAREN RPAREN DefaultValueopt SEMICOLON
//
// void MakeAnnotationTypeMemberDeclaration();
//

//
// Rule 208:  AnnotationTypeMemberDeclaration ::= ConstantDeclaration
//
// void NoAction();
//

//
// Rule 209:  AnnotationTypeMemberDeclaration ::= TypeDeclaration
//
// void NoAction();
//

//
// Rule 210:  DefaultValue ::= default MemberValue
//
// void SetSym1ToSym2();
//

//
// Rule 211:  DefaultValueopt ::=
//
// void NullAction();
//

//
// Rule 212:  DefaultValueopt ::= DefaultValue
//
// void NoAction();
//

//
// Rule 213:  ArrayInitializer ::= LBRACE Marker ,opt RBRACE
//
// void MakeArrayInitializer();
//
 

//
// Rule 214:  ArrayInitializer ::= LBRACE VariableInitializers ,opt RBRACE
//
 

//
// Rule 214:  ArrayInitializer ::= LBRACE VariableInitializers ,opt RBRACE
//
// void MakeArrayInitializer();
//

//
// Rule 215:  VariableInitializers ::= VariableInitializer
//
// void StartList();
//

//
// Rule 216:  VariableInitializers ::= VariableInitializers COMMA VariableInitializer
//
// void AddList3();
//

//
// Rule 217:  Block ::= LBRACE BlockStatementsopt RBRACE
//

void Parser::consumeBlock()
{
    AstBlock* p = ast_pool -> NewBlock();
    p -> left_brace_token = GetToken(1);
    if (Sym(2))
    {
        AstListNode* tail = DYNAMIC_CAST<AstListNode*> (Sym(2));
        p -> AllocateStatements(tail -> index + 1);
        AstListNode* root = tail;
        do
        {
            root = root -> next;
            p -> AddStatement(DYNAMIC_CAST<ReachComleteAbleStatement*> (root -> element));
        } while (root != tail);
        FreeCircularList(tail);
    }
    p -> right_brace_token = GetToken(3);
    Sym(1) = p;
}

//
// Rule 218:  BlockStatements ::= BlockStatement
//
// void StartList();
//

//
// Rule 219:  BlockStatements ::= BlockStatements BlockStatement
//
// void AddList2();
//

//
// Rule 220:  BlockStatementsopt ::=
//
// void NullAction();
//

//
// Rule 221:  BlockStatementsopt ::= BlockStatements
//
// void NoAction();
//

//
// Rule 222:  BlockStatement ::= LocalVariableDeclarationStatement
//
// void NoAction();
//

//
// Rule 223:  BlockStatement ::= Statement
//
// void NoAction();
//

//
// Rule 224:  BlockStatement ::= ClassDeclaration
//

void Parser::consumeInvalidClassDeclaration()
{
    Sym(1) = ast_pool ->
        NewLocalClassStatement(DYNAMIC_CAST<ClassTypeDeclaration*> (Sym(1)));
	has_localType_inScope = true;
}

//
// Rule 225:  BlockStatement ::= EnumDeclaration
//

void Parser::consumeInvalidEnumDeclaration()
{
    Sym(1) = ast_pool ->
        NewLocalClassStatement(DYNAMIC_CAST<EnumTypeDeclaration*> (Sym(1)));
	has_localType_inScope = true;
}

//
// Rule 226:  BlockStatement ::= ExplicitConstructorInvocation
//
// void NoAction();
//

//
// Rule 227:  LocalVariableDeclarationStatement ::= LocalVariableDeclaration SEMICOLON
//

void Parser::consumeLocalVariableDeclarationStatement()
{
    DYNAMIC_CAST<LocalDeclaration*> (Sym(1)) -> semicolon_token_opt =
        GetToken(2);
}

//
// Rule 228:  LocalVariableDeclaration ::= Type Marker Marker VariableDeclarators
//

void Parser::MakeLocalVariable()
{
    LocalDeclaration* p = ast_pool -> NewLocalVariableStatement();
    if (Sym(2))
    {
        p -> modifiers_opt = MakeModifiers();
        p -> type = DYNAMIC_CAST<AstType*> (Sym(2));
    }
    else p -> type = DYNAMIC_CAST<AstType*> (Sym(1));
    AstListNode* tail = DYNAMIC_CAST<AstListNode*> (Sym(4));
    p -> AllocateVariableDeclarators(tail -> index + 1);
    AstListNode* root = tail;
    do
    {
        root = root -> next;
        p -> AddVariableDeclarator(DYNAMIC_CAST<AstVariableDeclarator*>
                                   (root -> element));
    } while (root != tail);
    FreeCircularList(tail);
    Sym(1) = p;
}

//
// Rule 229:  LocalVariableDeclaration ::= Modifiers Type Marker VariableDeclarators
//
// void MakeLocalVariable();
//

//
// Rule 230:  Statement ::= StatementWithoutTrailingSubstatement
//
// void NoAction();
//

//
// Rule 231:  Statement ::= LabeledStatement
//
// void NoAction();
//

//
// Rule 232:  Statement ::= IfThenStatement
//
// void NoAction();
//

//
// Rule 233:  Statement ::= IfThenElseStatement
//
// void NoAction();
//

//
// Rule 234:  Statement ::= WhileStatement
//
// void NoAction();
//

//
// Rule 235:  Statement ::= ForStatement
//
// void NoAction();
//
 

//
// Rule 236:  Statement ::= ForeachStatement
//
 

//
// Rule 236:  Statement ::= ForeachStatement
//
// void NoAction();
//

//
// Rule 237:  StatementNoShortIf ::= StatementWithoutTrailingSubstatement
//
// void NoAction();
//

//
// Rule 238:  StatementNoShortIf ::= LabeledStatementNoShortIf
//
// void NoAction();
//

//
// Rule 239:  StatementNoShortIf ::= IfThenElseStatementNoShortIf
//
// void NoAction();
//

//
// Rule 240:  StatementNoShortIf ::= WhileStatementNoShortIf
//
// void NoAction();
//

//
// Rule 241:  StatementNoShortIf ::= ForStatementNoShortIf
//
// void NoAction();
//

//
// Rule 242:  StatementNoShortIf ::= ForeachStatementNoShortIf
//
// void NoAction();
//

//
// Rule 243:  StatementWithoutTrailingSubstatement ::= Block
//
// void NoAction();
//

//
// Rule 244:  StatementWithoutTrailingSubstatement ::= EmptyStatement
//
// void NoAction();
//

//
// Rule 245:  StatementWithoutTrailingSubstatement ::= ExpressionStatement
//
// void NoAction();
//

//
// Rule 246:  StatementWithoutTrailingSubstatement ::= SwitchStatement
//
// void NoAction();
//

//
// Rule 247:  StatementWithoutTrailingSubstatement ::= DoStatement
//
// void NoAction();
//

//
// Rule 248:  StatementWithoutTrailingSubstatement ::= BreakStatement
//
// void NoAction();
//

//
// Rule 249:  StatementWithoutTrailingSubstatement ::= ContinueStatement
//
// void NoAction();
//

//
// Rule 250:  StatementWithoutTrailingSubstatement ::= ReturnStatement
//
// void NoAction();
//

//
// Rule 251:  StatementWithoutTrailingSubstatement ::= SynchronizedStatement
//
// void NoAction();
//

//
// Rule 252:  StatementWithoutTrailingSubstatement ::= ThrowStatement
//
// void NoAction();
//

//
// Rule 253:  StatementWithoutTrailingSubstatement ::= TryStatement
//
// void NoAction();
//

//
// Rule 254:  StatementWithoutTrailingSubstatement ::= AssertStatement
//
// void NoAction();
//

//
// Rule 255:  EmptyStatement ::= SEMICOLON
//

void Parser::consumeEmptyStatement()
{
    Sym(1) = ast_pool -> NewEmptyStatement(GetToken(1));
}

//
// Rule 256:  LabeledStatement ::= Identifier COLON Statement
//

void Parser::MakeLabeledStatement()
{
    AstBlock* p = AstBlock::BlockCast(Sym(3));

    if (! p || p -> label_opt)
    {
        //
        // When a statement is labeled, it is enclosed in a block.
        // This is necessary in order to allow the same name to be
        // reused to label a subsequent statement at the same nesting
        // level... See ProcessBlock, ProcessStatement,...
        //
        p = ast_pool -> GenBlock();
        p -> AllocateStatements(1); // allocate 1 element
        p -> left_brace_token = GetToken(1);
        p -> AddStatement(DYNAMIC_CAST<ReachComleteAbleStatement*> (Sym(3)));
        p -> right_brace_token = Sym(3) -> RightToken();
    }

    p -> label_opt = GetToken(1); // add label to statement

    Sym(1) = p; // The final result is a block containing the labeled-statement
}

//
// Rule 257:  LabeledStatementNoShortIf ::= Identifier COLON StatementNoShortIf
//
// void MakeLabeledStatement();
//

//
// Rule 258:  ExpressionStatement ::= StatementExpression SEMICOLON
//

void Parser::consumeExpressionStatement()
{
    DYNAMIC_CAST<AstExpressionStatement*> (Sym(1)) -> semicolon_token_opt =
        GetToken(2);
}

//
// Rule 259:  StatementExpression ::= Assignment
//

void Parser::MakeExpressionStatement()
{
    AstExpressionStatement* p = ast_pool -> NewExpressionStatement();
    p -> expression = DYNAMIC_CAST<Expression*> (Sym(1));
    Sym(1) = p;
}

//
// Rule 260:  StatementExpression ::= PreIncrementExpression
//
// void MakeExpressionStatement();
//

//
// Rule 261:  StatementExpression ::= PreDecrementExpression
//
// void MakeExpressionStatement();
//

//
// Rule 262:  StatementExpression ::= PostIncrementExpression
//
// void MakeExpressionStatement();
//

//
// Rule 263:  StatementExpression ::= PostDecrementExpression
//
// void MakeExpressionStatement();
//

//
// Rule 264:  StatementExpression ::= MethodInvocation
//
// void MakeExpressionStatement();
//

//
// Rule 265:  StatementExpression ::= ClassInstanceCreationExpression
//
// void MakeExpressionStatement();
//

//
// Rule 266:  IfThenStatement ::= if LPAREN Expression RPAREN Statement Marker Marker
//

void Parser::MakeIfThenElseStatement()
{
    //
    // We wrap the true and false statements in a block, to make the semantic
    // pass easier.
    //
    IfStatement* p = ast_pool -> NewIfStatement();
    p -> if_token = GetToken(1);
    p -> expression = DYNAMIC_CAST<Expression*> (Sym(3));
    p -> true_statement = MakeBlock(5);
	p->true_statement->owner_ast = p;
	p->true_statement->SetTag(AstBlock::IF_TRUE_STATEMENT_BLOCK);
    p -> false_statement_opt = Sym(7) ? MakeBlock(7) : NULL;
	if(p->false_statement_opt)
	{
		p->false_statement_opt->owner_ast = p;
		p->false_statement_opt->SetTag(AstBlock::IF_FALSE_STATEMENT_BLOCK);
	}
    Sym(1) = p;
}

//
// Ensures the symbol at tokennum is an AstBlock (wrapping it in a generated
// block if necessary).
//
AstBlock* Parser::MakeBlock(int tokennum)
{
    AstBlock* block = AstBlock::BlockCast(Sym(tokennum));
    if (! block)
    {
        block = ast_pool -> GenBlock();
        block -> AllocateStatements(1); // allocate 1 element
        block -> left_brace_token = GetToken(tokennum);
        block -> AddStatement(DYNAMIC_CAST<ReachComleteAbleStatement*> (Sym(tokennum)));
        block -> right_brace_token = Sym(tokennum) -> RightToken();
    }
    return block;
}

//
// Rule 267:  IfThenElseStatement ::= if LPAREN Expression RPAREN StatementNoShortIf else Statement
//
// void MakeIfThenElseStatement();
//

//
// Rule 268:  IfThenElseStatementNoShortIf ::= if LPAREN Expression RPAREN StatementNoShortIf else StatementNoShortIf
//
// void MakeIfThenElseStatement();
//

//
// Rule 269:  SwitchStatement ::= switch LPAREN Expression RPAREN SwitchBlock
//

void Parser::consumeStatementSwitch()
{
    SwitchStatement* p = DYNAMIC_CAST<SwitchStatement*> (Sym(5));
    p -> switch_token = GetToken(1);
    p -> expression = DYNAMIC_CAST<Expression*> (Sym(3));
    Sym(1) = p;
}

//
// Rule 270:  SwitchBlock ::= LBRACE SwitchBlockStatements SwitchLabelsopt RBRACE
//

void Parser::consumeSwitchBlockWithSwitchBlockStatements()
{
    SwitchStatement* p = ast_pool -> NewSwitchStatement();
    auto block = ast_pool ->NewSwitchBlock();
	block->owner_ast = p;
    block -> left_brace_token = GetToken(1);
    AstListNode* tail = DYNAMIC_CAST<AstListNode*> (Sym(2));
    block -> AllocateStatements(tail -> index + (Sym(3) ? 2 : 1));
    AstListNode* root = tail;
    do
    {
        root = root -> next;
        block -> AddStatement(DYNAMIC_CAST<ReachComleteAbleStatement*> (root -> element));
    } while (root != tail);
    FreeCircularList(tail);
    if (Sym(3))
    {
		AstSwitchBlockStatement*  temp = (AstSwitchBlockStatement*)MakeSwitchBlockStatement(DYNAMIC_CAST<AstListNode*>(Sym(3)));
		temp->owner_ast = block;
		block->AddStatement(temp);
    }

    block -> right_brace_token = GetToken(4);

    p -> switch_block = block;
    Sym(1) = p;
}

//
// Creates an AstSwitchBlockStatement from the given non-null labels, and
// possibly null list of statements.
//
ReachComleteAbleStatement* Parser::MakeSwitchBlockStatement(AstListNode* labels,
                                               AstListNode* statements)
{
    AstSwitchBlockStatement* p = ast_pool -> NewSwitchBlockStatement();
    assert(labels);
    AstListNode* tail = labels;
    p -> AllocateSwitchLabels(tail -> index + 1);
    AstListNode* root = tail;
    do
    {
        root = root -> next;
        p -> AddSwitchLabel(DYNAMIC_CAST<AstSwitchLabel*> (root -> element));
    } while (root != tail);
    FreeCircularList(tail);
    if (statements)
    {
        tail = statements;
        p -> AllocateStatements(tail -> index + 1);
        root = tail;
        do
        {
            root = root -> next;
            p -> AddStatement(DYNAMIC_CAST<ReachComleteAbleStatement*> (root -> element));
        } while (root != tail);
        FreeCircularList(tail);
    }
    else
    {
        p -> AllocateStatements(1);
        p -> AddStatement(ast_pool -> GenEmptyStatement(labels ->
                                                        RightToken()));
    }
    p -> right_brace_token =
        p -> GetStatement(p -> NumStatements() - 1) -> RightToken();
    return p;
}

//
// Rule 271:  SwitchBlock ::= LBRACE SwitchLabelsopt RBRACE
//

void Parser::consumeSwitchBlockOnlyWithSwitchLabelsopt()
{
    SwitchStatement* p = ast_pool -> NewSwitchStatement();
    auto block = ast_pool ->NewSwitchBlock();
	block->owner_ast = p;

    block -> AllocateStatements(1); // allocate 1 element
    block -> left_brace_token = GetToken(1);
    if (Sym(2))
    {
		AstSwitchBlockStatement*  temp = (AstSwitchBlockStatement*)MakeSwitchBlockStatement(DYNAMIC_CAST<AstListNode*>(Sym(2)));
		temp->owner_ast = block;
		block->AddStatement(temp);
    }

    block -> right_brace_token = GetToken(3);
  
    p -> switch_block = block;

    Sym(1) = p;
}

//
// Rule 272:  SwitchBlockStatements ::= SwitchBlockStatement
//
// void StartList();
//

//
// Rule 273:  SwitchBlockStatements ::= SwitchBlockStatements SwitchBlockStatement
//
// void AddList2();
//

//
// Rule 274:  SwitchBlockStatement ::= SwitchLabels BlockStatements
//

void Parser::consumeSwitchBlockStatement()
{
    Sym(1) = MakeSwitchBlockStatement(DYNAMIC_CAST<AstListNode*> (Sym(1)),
                                      DYNAMIC_CAST<AstListNode*> (Sym(2)));
}

//
// Rule 275:  SwitchLabels ::= SwitchLabel
//
// void StartList();
//

//
// Rule 276:  SwitchLabels ::= SwitchLabels SwitchLabel
//
// void AddList2();
//

//
// Rule 277:  SwitchLabelsopt ::=
//
// void NullAction();
//

//
// Rule 278:  SwitchLabelsopt ::= SwitchLabels
//
// void NoAction();
//

//
// Rule 279:  SwitchLabel ::= case Expression COLON
//

void Parser::MakeSwitchLabel()
{
    AstSwitchLabel* p = ast_pool -> NewSwitchLabel();
    p -> case_token = GetToken(1);
    p -> expression_opt = DYNAMIC_CAST<Expression*> (Sym(2));
    p -> colon_token = GetToken(3);
    Sym(1) = p;
}

//
// Rule 280:  SwitchLabel ::= default Marker COLON
//
// void MakeSwitchLabel();
//

//
// Rule 281:  WhileStatement ::= while LPAREN Expression RPAREN Statement
//

void Parser::MakeWhileStatement()
{
    //
    // We wrap the loop statement in a block, to make the semantic pass easier.
    //
    WhileStatement* p = ast_pool -> NewWhileStatement();
    p -> while_token = GetToken(1);
    p -> expression = DYNAMIC_CAST<Expression*> (Sym(3));
    p -> statement = MakeBlock(5);
	if(p->statement)
	{
		p->statement->owner_ast = p;
		p->statement->SetTag(AstBlock::WHILE_STATEMENT_BLOCK);
	}

    //
    // We also wrap the loop in a block, to make the semantic pass easier.
    //
    AstBlock* block = ast_pool -> GenBlock();
    block -> AllocateStatements(1); // allocate 1 element
    block -> left_brace_token = GetToken(1);
    block -> AddStatement(p);
    block -> right_brace_token = Sym(5) -> RightToken();
    Sym(1) = block;
}

//
// Rule 282:  WhileStatementNoShortIf ::= while LPAREN Expression RPAREN StatementNoShortIf
//
// void MakeWhileStatement();
//

//
// Rule 283:  DoStatement ::= do Statement while LPAREN Expression RPAREN SEMICOLON
//

void Parser::consumeStatementDo()
{
    //
    // We wrap the loop statement in a block, to make the semantic pass easier.
    //
    DoStatement* p = ast_pool -> NewDoStatement();
    p -> do_token = GetToken(1);
    p -> statement = MakeBlock(2);
    p -> while_token = GetToken(3);
    p -> expression = DYNAMIC_CAST<Expression*> (Sym(5));
    p -> semicolon_token = GetToken(7);
	if(p->statement)
	{
		p->statement->owner_ast = p;
		p->statement->SetTag(AstBlock::DO_STATEMENT_BLOCK);
	}


    //
    // We also wrap the loop in a block, to make the semantic pass easier.
    //
    AstBlock* block = ast_pool -> GenBlock();
    block -> AllocateStatements(1); // allocate 1 element
    block -> left_brace_token = GetToken(1);
    block -> AddStatement(p);
    block -> right_brace_token = GetToken(7);
    Sym(1) = block;
}

//
// Rule 284:  ForStatement ::= for LPAREN ForInitopt SEMICOLON Expressionopt SEMICOLON ForUpdateopt RPAREN Statement
//

void Parser::MakeForStatement()
{
    //
    // We wrap the loop statement in a block, to make the semantic pass easier.
    //
    ForStatement* p = ast_pool -> NewForStatement();
    p -> for_token = GetToken(1);
    if (Sym(3))
    {
        AstListNode* tail = DYNAMIC_CAST<AstListNode*> (Sym(3));
        p -> AllocateForInitStatements(tail -> index + 1);
        AstListNode* root = tail;
        do
        {
            root = root -> next;
            p -> AddForInitStatement(DYNAMIC_CAST<ReachComleteAbleStatement*>
                                     (root -> element));
        } while (root != tail);
        FreeCircularList(tail);
    }
    p -> end_expression_opt = DYNAMIC_CAST<Expression*> (Sym(5));
    if (Sym(7))
    {
        AstListNode* tail = DYNAMIC_CAST<AstListNode*> (Sym(7));
        p -> AllocateForUpdateStatements(tail -> index + 1);
        AstListNode* root = tail;
        do
        {
            root = root -> next;
            p -> AddForUpdateStatement(DYNAMIC_CAST<AstExpressionStatement*>
                                       (root -> element));
        } while (root != tail);
        FreeCircularList(tail);
    }
    p -> statement = MakeBlock(9);
	if(p->statement)
	{
		p->statement->owner_ast = p;
		p->statement->SetTag(AstBlock::FOR_STATEMENT_BLOCK);
	}

    //
    // We also wrap the loop in a block, to make the semantic pass easier. In
    // particular, this lets us correctly handle "for(int i;;);for(int i;;);".
    //
    AstBlock* block = ast_pool -> NewBlock();
    block -> AllocateStatements(1); // allocate 1 element
    block -> left_brace_token = GetToken(1);
    block -> AddStatement(p);
    block -> right_brace_token = Sym(9) -> RightToken();
    block -> no_braces = true;
    Sym(1) = block;
}

//
// Rule 285:  ForStatementNoShortIf ::= for LPAREN ForInitopt SEMICOLON Expressionopt SEMICOLON ForUpdateopt RPAREN...
//
// void MakeForStatement();
//

//
// Rule 286:  ForInit ::= StatementExpressionList
//
// void NoAction();
//

//
// Rule 287:  ForInit ::= LocalVariableDeclaration
//
// void StartList();
//

//
// Rule 288:  ForInitopt ::=
//
// void NullAction();
//

//
// Rule 289:  ForInitopt ::= ForInit
//
// void NoAction();
//

//
// Rule 290:  ForUpdate ::= StatementExpressionList
//
// void NoAction();
//

//
// Rule 291:  ForUpdateopt ::=
//
// void NullAction();
//

//
// Rule 292:  ForUpdateopt ::= ForUpdate
//
// void NoAction();
//

//
// Rule 293:  StatementExpressionList ::= StatementExpression
//
// void StartList();
//

//
// Rule 294:  StatementExpressionList ::= StatementExpressionList COMMA StatementExpression
//
// void AddList3();
//

//
// Rule 295:  ForeachStatement ::= for LPAREN FormalParameter COLON Expression RPAREN Statement
//

void Parser::MakeForeachStatement()
{
    //
    // We wrap the loop statement in a block, to make the semantic pass easier.
    //
    ForeachStatement* p = ast_pool -> NewForeachStatement();
    p -> for_token = GetToken(1);
    p -> formal_parameter = DYNAMIC_CAST<AstFormalParameter*> (Sym(3));
    p -> expression = DYNAMIC_CAST<Expression*> (Sym(5));
    p -> statement = MakeBlock(7);

	if(p->statement)
	{
		p->statement->owner_ast = p;
		p->statement->SetTag(AstBlock::FOREACH_STATEMENT_BLOCK);
	}

    //
    // We also wrap the loop in a block, to make the semantic pass easier. In
    // particular, this lets us correctly handle
    // "for(int i:new int[0]);for(int i::new int[0]);".
    //
    AstBlock* block = ast_pool -> NewBlock();
    block -> AllocateStatements(1); // allocate 1 element
    block -> left_brace_token = GetToken(1);
    block -> AddStatement(p);
    block -> right_brace_token = Sym(7) -> RightToken();
    block -> no_braces = true;
    Sym(1) = block;
}

//
// Rule 296:  ForeachStatementNoShortIf ::= for LPAREN FormalParameter COLON Expression RPAREN StatementNoShortIf
//
// void MakeForeachStatement();
//

//
// Rule 297:  AssertStatement ::= assert Expression Marker Marker SEMICOLON
//

void Parser::MakeAssertStatement()
{
    AssertStatement* p = ast_pool -> NewAssertStatement();
    p -> assert_token = GetToken(1);
    p -> condition = DYNAMIC_CAST<Expression*> (Sym(2));
    p -> message_opt = DYNAMIC_CAST<Expression*> (Sym(4));
    p -> semicolon_token = GetToken(5);
    Sym(1) = p;
}

//
// Rule 298:  AssertStatement ::= assert Expression COLON Expression SEMICOLON
//
// void MakeAssertStatement();
//

//
// Rule 299:  BreakStatement ::= break Identifieropt SEMICOLON
//

void Parser::consumeStatementBreak()
{
    BreakStatement* p = ast_pool -> NewBreakStatement();
    p -> break_token = GetToken(1);
    if (GetToken(3)->index > GetToken(2)->index)
        p -> identifier_token_opt = GetToken(2);
    p -> semicolon_token = GetToken(3);
    Sym(1) = p;
}

//
// Rule 300:  ContinueStatement ::= continue Identifieropt SEMICOLON
//

void Parser::consumeStatementContinue()
{
    ContinueStatement* p = ast_pool -> NewContinueStatement();
    p -> continue_token = GetToken(1);
    if (GetToken(3)->index > GetToken(2)->index)
        p -> identifier_token_opt = GetToken(2);
    p -> semicolon_token = GetToken(3);
    Sym(1) = p;
}

//
// Rule 301:  ReturnStatement ::= return Expressionopt SEMICOLON
//

void Parser::consumeStatementReturn()
{
    ReturnStatement* p = ast_pool -> NewReturnStatement();
    p -> return_token = GetToken(1);
    p -> expression_opt = DYNAMIC_CAST<Expression*> (Sym(2));
    p -> semicolon_token = GetToken(3);
    Sym(1) = p;
}

//
// Rule 302:  ThrowStatement ::= throw Expression SEMICOLON
//

void Parser::consumeStatementThrow()
{
    ThrowStatement* p = ast_pool -> NewThrowStatement();
    p -> throw_token = GetToken(1);
    p -> expression = DYNAMIC_CAST<Expression*> (Sym(2));
    p -> semicolon_token = GetToken(3);
    Sym(1) = p;
}

//
// Rule 303:  SynchronizedStatement ::= synchronized LPAREN Expression RPAREN Block
//

void Parser::consumeStatementSynchronized()
{
    SynchronizedStatement* p = ast_pool -> NewSynchronizedStatement();
    p -> synchronized_token = GetToken(1);
    p -> expression = DYNAMIC_CAST<Expression*> (Sym(3));
    p -> block = DYNAMIC_CAST<AstBlock*> (Sym(5));
    p -> block -> SetTag(AstBlock::SYNCHRONIZED);

    Sym(1) = p;
}

//
// Rule 304:  TryStatement ::= try Block Catches Marker
//

void Parser::MakeTryStatement()
{
    TryStatement* p = ast_pool -> NewTryStatement();
    p -> try_token = GetToken(1);
    p -> block = DYNAMIC_CAST<AstBlock*> (Sym(2));
    if (Sym(3))
    {
        AstListNode* tail = DYNAMIC_CAST<AstListNode*> (Sym(3));
        p -> AllocateCatchClauses(tail -> index + 1);
        AstListNode* root = tail;
        do
        {
            root = root -> next;
            p -> AddCatchClause(DYNAMIC_CAST<AstCatchClause*>
                                (root -> element));
        } while (root != tail);
        FreeCircularList(tail);
    }
    if (Sym(4))
    {
        p -> block -> SetTag(AstBlock::TRY_CLAUSE_WITH_FINALLY);
        for (unsigned i = 0; i < p -> NumCatchClauses(); i++)
            p -> CatchClause(i) -> block ->
                SetTag(AstBlock::TRY_CLAUSE_WITH_FINALLY);
        p -> finally_clause_opt = DYNAMIC_CAST<AstFinallyClause*> (Sym(4));
    }
    Sym(1) = p;
}

//
// Rule 305:  TryStatement ::= try Block Catchesopt Finally
//
// void MakeTryStatement();
//

//
// Rule 306:  Catches ::= CatchClause
//
// void StartList();
//

//
// Rule 307:  Catches ::= Catches CatchClause
//
// void AddList2();
//

//
// Rule 308:  Catchesopt ::=
//
// void NullAction();
//

//
// Rule 309:  Catchesopt ::= Catches
//
// void NoAction();
//

//
// Rule 310:  CatchClause ::= catch LPAREN FormalParameter RPAREN Block
//

void Parser::consumeStatementCatch()
{
    AstCatchClause* p = ast_pool -> NewCatchClause();
    p -> catch_token = GetToken(1);
    p -> formal_parameter = DYNAMIC_CAST<AstFormalParameter*> (Sym(3));
    p -> block = DYNAMIC_CAST<AstBlock*> (Sym(5));

    Sym(1) = p;
}
 

//
// Rule 311:  Finally ::= finally Block
//

void Parser::consumeStatementFinally()
{

    AstFinallyClause* p = ast_pool -> NewFinallyClause();

    p -> finally_token = GetToken(1);

    p -> block = DYNAMIC_CAST<AstBlock*> (Sym(2));

    p -> block -> SetTag(AstBlock::FINALLY);

	p->block->owner_ast = p;

    Sym(1) = p;

}

//
// Rule 312:  Primary ::= PrimaryNoNewArray
//
// void NoAction();
//

//
// Rule 313:  Primary ::= ArrayCreationUninitialized
//
// void NoAction();
//

//
// Rule 314:  Primary ::= ArrayCreationInitialized
//
// void NoAction();
//

//
// Rule 315:  PrimaryNoNewArray ::= Literal
//
// void NoAction();
//

//
// Rule 316:  PrimaryNoNewArray ::= this
//

void Parser::consumePrimaryNoNewArrayThis()
{
    Sym(1) = ast_pool -> NewThisExpression(GetToken(1));
}

//
// Rule 317:  PrimaryNoNewArray ::= LPAREN Name Marker RPAREN
//

void Parser::MakeParenthesizedExpression()
{
    AstParenthesizedExpression* p = ast_pool -> NewParenthesizedExpression();
    p -> left_parenthesis_token = GetToken(1);
    p -> expression = DYNAMIC_CAST<Expression*> (Sym(2));
    p -> right_parenthesis_token = GetToken(4);
    Sym(1) = p;
}

//
// Rule 318:  PrimaryNoNewArray ::= LPAREN ExpressionNotName Marker RPAREN
//
// void MakeParenthesizedExpression();
//

//
// Rule 319:  PrimaryNoNewArray ::= ClassInstanceCreationExpression
//
// void NoAction();
//

//
// Rule 320:  PrimaryNoNewArray ::= FieldAccess
//
// void NoAction();
//

//
// Rule 321:  PrimaryNoNewArray ::= Name DOT this
//

void Parser::consumePrimaryNoNewArrayNameThis()
{
    AstThisExpression* p = ast_pool -> NewThisExpression(GetToken(3));
    p -> base_opt = ast_pool -> NewTypeName(DYNAMIC_CAST<AstName*> (Sym(1)));
    Sym(1) = p;
}

//
// Rule 322:  PrimaryNoNewArray ::= PrimitiveType Dimsopt DOT class
//

void Parser::MakeClassLiteral()
{
    ClassLiteralAccess* p = ast_pool -> NewClassLiteral(GetToken(4));
    if (GetToken(3)->index == GetToken(4)->index)
        Sym(2) = NULL;
    p -> type = MakeArrayType(1);
    Sym(1) = p;
}

//
// Rule 323:  PrimaryNoNewArray ::= Name Dims DOT class
//
// void MakeClassLiteral();
//

//
// Rule 324:  PrimaryNoNewArray ::= Name DOT Marker class
//
// void MakeClassLiteral();
//

//
// Rule 325:  PrimaryNoNewArray ::= VoidType DOT Marker class
//
// void MakeClassLiteral();
//

//
// Rule 326:  PrimaryNoNewArray ::= MethodInvocation
//
// void NoAction();
//

//
// Rule 327:  PrimaryNoNewArray ::= ArrayAccess
//
// void NoAction();
//

//
// Rule 328:  ClassInstanceCreationExpression ::= new ClassOrInterfaceType Arguments ClassBodyopt
//

void Parser::consumeClassInstanceCreationExpression()
{
    AstClassCreationExpression* p = ast_pool -> NewClassCreationExpression();
    p -> new_token = GetToken(1);
    p -> class_type = DYNAMIC_CAST<AstTypeName*> (Sym(2));
    p -> arguments = DYNAMIC_CAST<AstArguments*> (Sym(3));
    auto  class_body_opt = DYNAMIC_CAST<AstClassBody*> (Sym(4));
    if (class_body_opt)
    {
		has_localType_inScope = true;
		AstAnonymousTypeDeclaration* anonymous_type_opt = ast_pool->GenAnonymousTypeDeclaration();
		anonymous_type_opt->create_expr = p;
		anonymous_type_opt->class_body = class_body_opt;
		anonymous_type_opt->class_body->identifier_token =
			p->class_type->IdentifierToken();
		p->anonymous_type_opt = anonymous_type_opt;
    }
	
    Sym(1) = p;
}

//
// Rule 329:  ClassInstanceCreationExpression ::= new TypeArguments ClassOrInterfaceType Arguments ClassBodyopt
//

void Parser::consumeClassInstanceCreationExpressionWithTypeArguments()
{
    AstClassCreationExpression* p = ast_pool -> NewClassCreationExpression();
    p -> new_token = GetToken(1);
    p -> type_arguments_opt = MakeExplicitTypeArguments(2);
    p -> class_type = DYNAMIC_CAST<AstTypeName*> (Sym(3));
    p -> arguments = DYNAMIC_CAST<AstArguments*> (Sym(4));
    auto class_body_opt = DYNAMIC_CAST<AstClassBody*> (Sym(5));
	if (class_body_opt)
	{
		has_localType_inScope = true;
		AstAnonymousTypeDeclaration* anonymous_type_opt = ast_pool->GenAnonymousTypeDeclaration();
		anonymous_type_opt->create_expr = p;
		anonymous_type_opt->class_body = class_body_opt;
		anonymous_type_opt->class_body->identifier_token =
			p->class_type->IdentifierToken();
		p->anonymous_type_opt = anonymous_type_opt;
	}
    Sym(1) = p;
}

//
// Rule 330:  ClassInstanceCreationExpression ::= Primary DOT new TypeArgumentsopt Identifier TypeArgumentsopt Arguments...
//

void Parser::MakeQualifiedNew()
{
    AstClassCreationExpression* p = ast_pool -> NewClassCreationExpression();
    p -> base_opt = DYNAMIC_CAST<Expression*> (Sym(1));
    p -> new_token = GetToken(3);
    p -> type_arguments_opt = MakeExplicitTypeArguments(4);
    p -> class_type = ast_pool -> NewTypeName(MakeSimpleName(5));
    p -> class_type -> type_arguments_opt = MakeExplicitTypeArguments(6);
    p -> arguments = DYNAMIC_CAST<AstArguments*> (Sym(7));
    auto class_body_opt = DYNAMIC_CAST<AstClassBody*> (Sym(8));
	if (class_body_opt)
	{
		has_localType_inScope = true;
		AstAnonymousTypeDeclaration* anonymous_type_opt = ast_pool->GenAnonymousTypeDeclaration();
		anonymous_type_opt->create_expr = p;
		anonymous_type_opt->class_body = class_body_opt;
		anonymous_type_opt->class_body->identifier_token =
			GetToken(5);
		p->anonymous_type_opt = anonymous_type_opt;
	}
   
    Sym(1) = p;
}

//
// Rule 331:  ClassInstanceCreationExpression ::= Name DOT new TypeArgumentsopt Identifier TypeArgumentsopt Arguments...
//
// void MakeQualifiedNew();
//

//
// Rule 332:  ArgumentList ::= Expression
//
// void StartList();
//

//
// Rule 333:  ArgumentList ::= ArgumentList COMMA Expression
//
// void AddList3();
//

//
// Rule 334:  ArgumentListopt ::=
//
// void NullAction();
//

//
// Rule 335:  ArgumentListopt ::= ArgumentList
//
// void NoAction();
//

//
// Rule 336:  ArrayCreationUninitialized ::= new PrimitiveType DimExprs Dimsopt
//

void Parser::MakeArrayCreationUninitialized()
{
    AstArrayCreationExpression* p = ast_pool -> NewArrayCreationExpression();
    p -> new_token = GetToken(1);
    p -> array_type = DYNAMIC_CAST<AstType*> (Sym(2));
    AstListNode* tail = DYNAMIC_CAST<AstListNode*> (Sym(3));
    p -> AllocateDimExprs(tail -> index + 1);
    AstListNode* root = tail;
    do
    {
        root = root -> next;
        p -> AddDimExpr(DYNAMIC_CAST<AstDimExpr*> (root -> element));
    } while (root != tail);
    FreeCircularList(tail);
    p -> brackets_opt = DYNAMIC_CAST<AstBrackets*> (Sym(4));
    Sym(1) = p;
}

//
// Rule 337:  ArrayCreationUninitialized ::= new ClassOrInterfaceType DimExprs Dimsopt
//
// void MakeArrayCreationUninitialized();
//

//
// Rule 338:  ArrayCreationInitialized ::= new PrimitiveType Dims ArrayInitializer
//

void Parser::MakeArrayCreationInitialized()
{
    AstArrayCreationExpression* p = ast_pool -> NewArrayCreationExpression();
    p -> new_token = GetToken(1);
    p -> array_type = MakeArrayType(2);
    p -> array_initializer_opt = DYNAMIC_CAST<AstArrayInitializer*> (Sym(4));
    Sym(1) = p;
}

//
// Rule 339:  ArrayCreationInitialized ::= new ClassOrInterfaceType Dims ArrayInitializer
//
// void MakeArrayCreationInitialized();
//

//
// Rule 340:  DimExprs ::= DimExpr
//
// void StartList();
//

//
// Rule 341:  DimExprs ::= DimExprs DimExpr
//
// void AddList2();
//

//
// Rule 342:  DimExpr ::= LBRACKET Expression RBRACKET
//

void Parser::consumeDimWithExpr()
{
    AstDimExpr* p = ast_pool -> NewDimExpr();
    p -> left_bracket_token = GetToken(1);
    p -> expression = DYNAMIC_CAST<Expression*> (Sym(2));
    p -> right_bracket_token = GetToken(3);
    Sym(1) = p;
}

//
// Rule 343:  Dims ::= LBRACKET RBRACKET
//

void Parser::consumeDimWithOutExpr()
{
    Sym(1) = ast_pool -> NewBrackets(GetToken(1), GetToken(2));
}

//
// Rule 344:  Dims ::= Dims LBRACKET RBRACKET
//

void Parser::consumeDimensions()
{
    AstBrackets* p = DYNAMIC_CAST<AstBrackets*> (Sym(1));
    p -> right_bracket_token = GetToken(2);
    p -> dims++;
}

//
// Rule 345:  Dimsopt ::=
//
// void NullAction();
//

//
// Rule 346:  Dimsopt ::= Dims
//
// void NoAction();
//

//
// Rule 347:  SuperAccess ::= super
//

void Parser::consumeToken_super()
{
    Sym(1) = ast_pool -> NewSuperExpression(GetToken(1));
}

//
// Rule 348:  SuperAccess ::= Name DOT Marker super
//

void Parser::consumeQuilifiedSuperAccess()
{
    AstSuperExpression* p = ast_pool -> NewSuperExpression(GetToken(4));
    p -> base_opt = ast_pool -> NewTypeName(DYNAMIC_CAST<AstName*> (Sym(1)));
    Sym(1) = p;
}

//
// Rule 349:  FieldAccess ::= Primary DOT Marker Identifier
//

void Parser::MakeFieldAccess()
{
    FieldReference* p = ast_pool -> NewFieldAccess();
    p -> base = DYNAMIC_CAST<Expression*> (Sym(1));
    p -> identifier_token = GetToken(4);
    Sym(1) = p;
}

//
// Rule 350:  FieldAccess ::= SuperAccess DOT Marker Identifier
//
// void MakeFieldAccess();
//

//
// Rule 351:  MethodInvocation ::= Identifier Arguments
//

void Parser::consumeMethodInvocationPrimary()
{
    AstMethodInvocation* p = ast_pool -> NewMethodInvocation(GetToken(1));
    p -> arguments = DYNAMIC_CAST<AstArguments*> (Sym(2));
    Sym(1) = p;
}

//
// Rule 352:  MethodInvocation ::= Name DOT Marker Identifier Arguments
//

void Parser::MakeMethodInvocation()
{
    AstMethodInvocation* p = ast_pool -> NewMethodInvocation(GetToken(4));
    p -> base_opt = DYNAMIC_CAST<Expression*> (Sym(1));
    p -> type_arguments_opt = MakeExplicitTypeArguments(3);
    p -> arguments = DYNAMIC_CAST<AstArguments*> (Sym(5));
    Sym(1) = p;
}

//
// Rule 353:  MethodInvocation ::= Name DOT TypeArguments Identifier Arguments
//
// void MakeMethodInvocation();
//

//
// Rule 354:  MethodInvocation ::= Primary DOT Marker Identifier Arguments
//
// void MakeMethodInvocation();
//

//
// Rule 355:  MethodInvocation ::= Primary DOT TypeArguments Identifier Arguments
//
// void MakeMethodInvocation();
//

//
// Rule 356:  MethodInvocation ::= SuperAccess DOT Marker Identifier Arguments
//
// void MakeMethodInvocation();
//

//
// Rule 357:  MethodInvocation ::= SuperAccess DOT TypeArguments Identifier Arguments
//
// void MakeMethodInvocation();
//

//
// Rule 358:  ArrayAccess ::= Name LBRACKET Expression RBRACKET
//

void Parser::MakeArrayAccess()
{
    ArrayReference* p = ast_pool -> NewArrayAccess();
    p -> base = DYNAMIC_CAST<Expression*> (Sym(1));
    p -> left_bracket_token = GetToken(2);
    p -> expression = DYNAMIC_CAST<Expression*> (Sym(3));
    p -> right_bracket_token = GetToken(4);
    Sym(1) = p;
}

//
// Rule 359:  ArrayAccess ::= PrimaryNoNewArray LBRACKET Expression RBRACKET
//
// void MakeArrayAccess();
//

//
// Rule 360:  ArrayAccess ::= ArrayCreationInitialized LBRACKET Expression RBRACKET
//
// void MakeArrayAccess();
//

//
// Rule 361:  PostfixExpression ::= Primary
//
// void NoAction();
//

//
// Rule 362:  PostfixExpression ::= Name
//
// void NoAction();
//

//
// Rule 363:  PostfixExpression ::= PostIncrementExpression
//
// void NoAction();
//

//
// Rule 364:  PostfixExpression ::= PostDecrementExpression
//
// void NoAction();
//

//
// Rule 365:  PostfixExpressionNotName ::= Primary
//
// void NoAction();
//

//
// Rule 366:  PostfixExpressionNotName ::= PostIncrementExpression
//
// void NoAction();
//

//
// Rule 367:  PostfixExpressionNotName ::= PostDecrementExpression
//
// void NoAction();
//

//
// Rule 368:  PostIncrementExpression ::= PostfixExpression PLUS_PLUS
//

void Parser::consumeUnaryExpressionPLUSPLUS()
{
    AstPostUnaryExpression* p =
        ast_pool -> NewPostUnaryExpression(PostUnaryExpressionEnum::PLUSPLUS);
    p -> expression = DYNAMIC_CAST<Expression*> (Sym(1));
    p -> post_operator_token = GetToken(2);
    Sym(1) = p;
}

//
// Rule 369:  PostDecrementExpression ::= PostfixExpression MINUS_MINUS
//

void Parser::consumeUnaryExpressionMINUSMINUS()
{
    AstPostUnaryExpression* p =
        ast_pool -> NewPostUnaryExpression(PostUnaryExpressionEnum::MINUSMINUS);
    p -> expression = DYNAMIC_CAST<Expression*> (Sym(1));
    p -> post_operator_token = GetToken(2);
    Sym(1) = p;
}

//
// Rule 370:  UnaryExpression ::= PreIncrementExpression
//
// void NoAction();
//

//
// Rule 371:  UnaryExpression ::= PreDecrementExpression
//
// void NoAction();
//

//
// Rule 372:  UnaryExpression ::= PLUS UnaryExpression
//

void Parser::MakePreUnaryExpression()
{
    PreUnaryExpressionEnum::PreUnaryExpressionTag tag;
    switch (GetToken(1)->_kind)
    {
    case TK_PLUS_PLUS: tag = PreUnaryExpressionEnum::PLUSPLUS; break;
    case TK_MINUS_MINUS: tag = PreUnaryExpressionEnum::MINUSMINUS; break;
    case TK_PLUS: tag = PreUnaryExpressionEnum::PLUS; break;
    case TK_MINUS: tag = PreUnaryExpressionEnum::MINUS; break;
    case TK_TWIDDLE: tag = PreUnaryExpressionEnum::TWIDDLE; break;
    case TK_NOT: tag = PreUnaryExpressionEnum::NOT; break;
    default: tag = PreUnaryExpressionEnum::NONE;
    }
    AstPreUnaryExpression* p = ast_pool -> NewPreUnaryExpression(tag);
    p -> pre_operator_token = GetToken(1);
    p -> expression = DYNAMIC_CAST<Expression*> (Sym(2));
    Sym(1) = p;
}

//
// Rule 373:  UnaryExpression ::= MINUS UnaryExpression
//
// void MakePreUnaryExpression();
//

//
// Rule 374:  UnaryExpression ::= UnaryExpressionNotPlusMinus
//
// void NoAction();
//

//
// Rule 375:  UnaryExpressionNotName ::= PreIncrementExpression
//
// void NoAction();
//

//
// Rule 376:  UnaryExpressionNotName ::= PreDecrementExpression
//
// void NoAction();
//

//
// Rule 377:  UnaryExpressionNotName ::= PLUS UnaryExpression
//
// void MakePreUnaryExpression();
//

//
// Rule 378:  UnaryExpressionNotName ::= MINUS UnaryExpression
//
// void MakePreUnaryExpression();
//

//
// Rule 379:  UnaryExpressionNotName ::= UnaryExpressionNotPlusMinusNotName
//
// void NoAction();
//

//
// Rule 380:  PreIncrementExpression ::= PLUS_PLUS UnaryExpression
//
// void MakePreUnaryExpression();
//

//
// Rule 381:  PreDecrementExpression ::= MINUS_MINUS UnaryExpression
//
// void MakePreUnaryExpression();
//

//
// Rule 382:  UnaryExpressionNotPlusMinus ::= PostfixExpression
//
// void NoAction();
//

//
// Rule 383:  UnaryExpressionNotPlusMinus ::= TWIDDLE UnaryExpression
//
// void MakePreUnaryExpression();
//

//
// Rule 384:  UnaryExpressionNotPlusMinus ::= NOT UnaryExpression
//
// void MakePreUnaryExpression();
//

//
// Rule 385:  UnaryExpressionNotPlusMinus ::= CastExpression
//
// void NoAction();
//

//
// Rule 386:  UnaryExpressionNotPlusMinusNotName ::= PostfixExpressionNotName
//
// void NoAction();
//

//
// Rule 387:  UnaryExpressionNotPlusMinusNotName ::= TWIDDLE UnaryExpression
//
// void MakePreUnaryExpression();
//

//
// Rule 388:  UnaryExpressionNotPlusMinusNotName ::= NOT UnaryExpression
//
// void MakePreUnaryExpression();
//

//
// Rule 389:  UnaryExpressionNotPlusMinusNotName ::= CastExpression
//
// void NoAction();
//

//
// Rule 390:  CastExpression ::= LPAREN PrimitiveType Dimsopt RPAREN UnaryExpression
//

void Parser::MakeCastExpression() { MakeCastExpression(MakeArrayType(2), 4); }

//
// Builds a cast expression. type must be the target AstType, and tokennum
// should point to the ')'.
//
void Parser::MakeCastExpression(AstType* type, int tokennum)
{
    CastExpression* p = ast_pool -> NewCastExpression();
    p -> left_parenthesis_token = GetToken(1);
    p -> type = type;
    p -> right_parenthesis_token = GetToken(tokennum);
    p -> expression = DYNAMIC_CAST<Expression*> (Sym(tokennum + 1));
    Sym(1) = p;
}

//
// Rule 391:  CastExpression ::= LPAREN Name Marker RPAREN UnaryExpressionNotPlusMinus
//
// void MakeCastExpression();
//

//
// Rule 392:  CastExpression ::= LPAREN Name Dims RPAREN UnaryExpressionNotPlusMinus
//
// void MakeCastExpression();
//

//
// Rule 393:  CastExpression ::= LPAREN Name LESS TypeArgumentList1 Dimsopt RPAREN UnaryExpressionNotPlusMinus
//

void Parser::consumeCastExpressionTypeArgumentList1Dimsopt()
{
    Sym(4) = MakeTypeArguments(2);
    MakeCastExpression(MakeArrayType(4), 6);
}

//
// Rule 394:  CastExpression ::= LPAREN Name LESS TypeArgumentList1 DOT ClassOrInterfaceType Dimsopt RPAREN...
//

void Parser::consumeCastExpressionTypeArgumentList1DimsoptQualified()
{
    AstTypeName* p = DYNAMIC_CAST<AstTypeName*> (Sym(6));
    while (p -> base_opt)
        p = p -> base_opt;
    p -> base_opt = MakeTypeArguments(2);
    MakeCastExpression(MakeArrayType(6), 8);
}

//
// Rule 395:  MultiplicativeExpression ::= UnaryExpression
//
// void NoAction();
//

//
// Rule 396:  MultiplicativeExpression ::= MultiplicativeExpression MULTIPLY UnaryExpression
//

//
// This creates a binary expression of the named type.
//
void Parser::MakeBinaryExpression()
{
	BinaryExpressionEnum::BinaryExpressionTag tag;
    switch (GetToken(2)->_kind)
    {
    case TK_MULTIPLY: tag = BinaryExpressionEnum::STAR; break;
    case TK_DIVIDE: tag = BinaryExpressionEnum::SLASH; break;
    case TK_REMAINDER: tag = BinaryExpressionEnum::MOD; break;
    case TK_PLUS: tag = BinaryExpressionEnum::PLUS; break;
    case TK_MINUS: tag = BinaryExpressionEnum::MINUS; break;
    case TK_LEFT_SHIFT: tag = BinaryExpressionEnum::LEFT_SHIFT; break;
    case TK_RIGHT_SHIFT: tag = BinaryExpressionEnum::RIGHT_SHIFT; break;
    case TK_UNSIGNED_RIGHT_SHIFT:
        tag = BinaryExpressionEnum::UNSIGNED_RIGHT_SHIFT; break;
    case TK_LESS: tag = BinaryExpressionEnum::LESS; break;
    case TK_GREATER: tag = BinaryExpressionEnum::GREATER; break;
    case TK_LESS_EQUAL: tag = BinaryExpressionEnum::LESS_EQUAL; break;
    case TK_GREATER_EQUAL: tag = BinaryExpressionEnum::GREATER_EQUAL; break;
    case TK_EQUAL_EQUAL: tag = BinaryExpressionEnum::EQUAL_EQUAL; break;
    case TK_NOT_EQUAL: tag = BinaryExpressionEnum::NOT_EQUAL; break;
    case TK_AND: tag = BinaryExpressionEnum::AND; break;
    case TK_XOR: tag = BinaryExpressionEnum::XOR; break;
    case TK_OR: tag = BinaryExpressionEnum::IOR; break;
    case TK_AND_AND: tag = BinaryExpressionEnum::AND_AND; break;
    case TK_OR_OR: tag = BinaryExpressionEnum::OR_OR; break;
    default: tag = BinaryExpressionEnum::OR_OR;
    }
    BinaryExpression* p = ast_pool -> NewBinaryExpression(tag);
    p -> left_expression = DYNAMIC_CAST<Expression*> (Sym(1));
    p -> binary_operator_token = GetToken(2);
    p -> right_expression = DYNAMIC_CAST<Expression*> (Sym(3));
    Sym(1) = p;
}

//
// Rule 397:  MultiplicativeExpression ::= MultiplicativeExpression DIVIDE UnaryExpression
//
// void MakeBinaryExpression();
//

//
// Rule 398:  MultiplicativeExpression ::= MultiplicativeExpression REMAINDER UnaryExpression
//
// void MakeBinaryExpression();
//

//
// Rule 399:  MultiplicativeExpressionNotName ::= UnaryExpressionNotName
//
// void NoAction();
//

//
// Rule 400:  MultiplicativeExpressionNotName ::= MultiplicativeExpressionNotName MULTIPLY UnaryExpression
//
// void MakeBinaryExpression();
//

//
// Rule 401:  MultiplicativeExpressionNotName ::= Name MULTIPLY UnaryExpression
//
// void MakeBinaryExpression();
//

//
// Rule 402:  MultiplicativeExpressionNotName ::= MultiplicativeExpressionNotName DIVIDE UnaryExpression
//
// void MakeBinaryExpression();
//

//
// Rule 403:  MultiplicativeExpressionNotName ::= Name DIVIDE UnaryExpression
//
// void MakeBinaryExpression();
//

//
// Rule 404:  MultiplicativeExpressionNotName ::= MultiplicativeExpressionNotName REMAINDER UnaryExpression
//
// void MakeBinaryExpression();
//

//
// Rule 405:  MultiplicativeExpressionNotName ::= Name REMAINDER UnaryExpression
//
// void MakeBinaryExpression();
//

//
// Rule 406:  AdditiveExpression ::= MultiplicativeExpression
//
// void NoAction();
//

//
// Rule 407:  AdditiveExpression ::= AdditiveExpression PLUS MultiplicativeExpression
//
// void MakeBinaryExpression();
//

//
// Rule 408:  AdditiveExpression ::= AdditiveExpression MINUS MultiplicativeExpression
//
// void MakeBinaryExpression();
//

//
// Rule 409:  AdditiveExpressionNotName ::= MultiplicativeExpressionNotName
//
// void NoAction();
//

//
// Rule 410:  AdditiveExpressionNotName ::= AdditiveExpressionNotName PLUS MultiplicativeExpression
//
// void MakeBinaryExpression();
//

//
// Rule 411:  AdditiveExpressionNotName ::= Name PLUS MultiplicativeExpression
//
// void MakeBinaryExpression();
//

//
// Rule 412:  AdditiveExpressionNotName ::= AdditiveExpressionNotName MINUS MultiplicativeExpression
//
// void MakeBinaryExpression();
//

//
// Rule 413:  AdditiveExpressionNotName ::= Name MINUS MultiplicativeExpression
//
// void MakeBinaryExpression();
//

//
// Rule 414:  ShiftExpression ::= AdditiveExpression
//
// void NoAction();
//

//
// Rule 415:  ShiftExpression ::= ShiftExpression LEFT_SHIFT AdditiveExpression
//
// void MakeBinaryExpression();
//

//
// Rule 416:  ShiftExpression ::= ShiftExpression RIGHT_SHIFT AdditiveExpression
//
// void MakeBinaryExpression();
//

//
// Rule 417:  ShiftExpression ::= ShiftExpression UNSIGNED_RIGHT_SHIFT AdditiveExpression
//
// void MakeBinaryExpression();
//

//
// Rule 418:  ShiftExpressionNotName ::= AdditiveExpressionNotName
//
// void NoAction();
//

//
// Rule 419:  ShiftExpressionNotName ::= ShiftExpressionNotName LEFT_SHIFT AdditiveExpression
//
// void MakeBinaryExpression();
//

//
// Rule 420:  ShiftExpressionNotName ::= Name LEFT_SHIFT AdditiveExpression
//
// void MakeBinaryExpression();
//

//
// Rule 421:  ShiftExpressionNotName ::= ShiftExpressionNotName RIGHT_SHIFT AdditiveExpression
//
// void MakeBinaryExpression();
//

//
// Rule 422:  ShiftExpressionNotName ::= Name RIGHT_SHIFT AdditiveExpression
//
// void MakeBinaryExpression();
//

//
// Rule 423:  ShiftExpressionNotName ::= ShiftExpressionNotName UNSIGNED_RIGHT_SHIFT AdditiveExpression
//
// void MakeBinaryExpression();
//

//
// Rule 424:  ShiftExpressionNotName ::= Name UNSIGNED_RIGHT_SHIFT AdditiveExpression
//
// void MakeBinaryExpression();
//

//
// Rule 425:  RelationalExpression ::= ShiftExpression
//
// void NoAction();
//

//
// Rule 426:  RelationalExpression ::= ShiftExpression LESS ShiftExpression
//
// void MakeBinaryExpression();
//

//
// Rule 427:  RelationalExpression ::= RelationalExpression GREATER ShiftExpression
//
// void MakeBinaryExpression();
//

//
// Rule 428:  RelationalExpression ::= RelationalExpression LESS_EQUAL ShiftExpression
//
// void MakeBinaryExpression();
//

//
// Rule 429:  RelationalExpression ::= RelationalExpression GREATER_EQUAL ShiftExpression
//
// void MakeBinaryExpression();
//

//
// Rule 430:  RelationalExpression ::= RelationalExpression instanceof ReferenceType
//

void Parser::MakeInstanceofExpression()
{
    InstanceofExpression* p = ast_pool -> NewInstanceofExpression();
    p -> expression = DYNAMIC_CAST<Expression*> (Sym(1));
    p -> instanceof_token = GetToken(2);
    p -> type = DYNAMIC_CAST<AstType*> (Sym(3));
    Sym(1) = p;
}

//
// Rule 431:  RelationalExpressionNotName ::= ShiftExpressionNotName
//
// void NoAction();
//

//
// Rule 432:  RelationalExpressionNotName ::= ShiftExpressionNotName LESS ShiftExpression
//
// void MakeBinaryExpression();
//

//
// Rule 433:  RelationalExpressionNotName ::= Name LESS ShiftExpression
//
// void MakeBinaryExpression();
//

//
// Rule 434:  RelationalExpressionNotName ::= ShiftExpressionNotName GREATER ShiftExpression
//
// void MakeBinaryExpression();
//

//
// Rule 435:  RelationalExpressionNotName ::= Name GREATER ShiftExpression
//
// void MakeBinaryExpression();
//

//
// Rule 436:  RelationalExpressionNotName ::= RelationalExpressionNotName LESS_EQUAL ShiftExpression
//
// void MakeBinaryExpression();
//

//
// Rule 437:  RelationalExpressionNotName ::= Name LESS_EQUAL ShiftExpression
//
// void MakeBinaryExpression();
//

//
// Rule 438:  RelationalExpressionNotName ::= RelationalExpressionNotName GREATER_EQUAL ShiftExpression
//
// void MakeBinaryExpression();
//

//
// Rule 439:  RelationalExpressionNotName ::= Name GREATER_EQUAL ShiftExpression
//
// void MakeBinaryExpression();
//

//
// Rule 440:  RelationalExpressionNotName ::= RelationalExpressionNotName instanceof ReferenceType
//
// void MakeInstanceofExpression();
//

//
// Rule 441:  RelationalExpressionNotName ::= Name instanceof ReferenceType
//
// void MakeInstanceofExpression();
//

//
// Rule 442:  EqualityExpression ::= RelationalExpression
//
// void NoAction();
//

//
// Rule 443:  EqualityExpression ::= EqualityExpression EQUAL_EQUAL RelationalExpression
//
// void MakeBinaryExpression();
//

//
// Rule 444:  EqualityExpression ::= EqualityExpression NOT_EQUAL RelationalExpression
//
// void MakeBinaryExpression();
//

//
// Rule 445:  EqualityExpressionNotName ::= RelationalExpressionNotName
//
// void NoAction();
//

//
// Rule 446:  EqualityExpressionNotName ::= EqualityExpressionNotName EQUAL_EQUAL RelationalExpression
//
// void MakeBinaryExpression();
//

//
// Rule 447:  EqualityExpressionNotName ::= Name EQUAL_EQUAL RelationalExpression
//
// void MakeBinaryExpression();
//

//
// Rule 448:  EqualityExpressionNotName ::= EqualityExpressionNotName NOT_EQUAL RelationalExpression
//
// void MakeBinaryExpression();
//

//
// Rule 449:  EqualityExpressionNotName ::= Name NOT_EQUAL RelationalExpression
//
// void MakeBinaryExpression();
//

//
// Rule 450:  AndExpression ::= EqualityExpression
//
// void NoAction();
//

//
// Rule 451:  AndExpression ::= AndExpression AND EqualityExpression
//
// void MakeBinaryExpression();
//

//
// Rule 452:  AndExpressionNotName ::= EqualityExpressionNotName
//
// void NoAction();
//

//
// Rule 453:  AndExpressionNotName ::= AndExpressionNotName AND EqualityExpression
//
// void MakeBinaryExpression();
//

//
// Rule 454:  AndExpressionNotName ::= Name AND EqualityExpression
//
// void MakeBinaryExpression();
//

//
// Rule 455:  ExclusiveOrExpression ::= AndExpression
//
// void NoAction();
//

//
// Rule 456:  ExclusiveOrExpression ::= ExclusiveOrExpression XOR AndExpression
//
// void MakeBinaryExpression();
//

//
// Rule 457:  ExclusiveOrExpressionNotName ::= AndExpressionNotName
//
// void NoAction();
//

//
// Rule 458:  ExclusiveOrExpressionNotName ::= ExclusiveOrExpressionNotName XOR AndExpression
//
// void MakeBinaryExpression();
//

//
// Rule 459:  ExclusiveOrExpressionNotName ::= Name XOR AndExpression
//
// void MakeBinaryExpression();
//

//
// Rule 460:  InclusiveOrExpression ::= ExclusiveOrExpression
//
// void NoAction();
//

//
// Rule 461:  InclusiveOrExpression ::= InclusiveOrExpression OR ExclusiveOrExpression
//
// void MakeBinaryExpression();
//

//
// Rule 462:  InclusiveOrExpressionNotName ::= ExclusiveOrExpressionNotName
//
// void NoAction();
//

//
// Rule 463:  InclusiveOrExpressionNotName ::= InclusiveOrExpressionNotName OR ExclusiveOrExpression
//
// void MakeBinaryExpression();
//

//
// Rule 464:  InclusiveOrExpressionNotName ::= Name OR ExclusiveOrExpression
//
// void MakeBinaryExpression();
//

//
// Rule 465:  ConditionalAndExpression ::= InclusiveOrExpression
//
// void NoAction();
//

//
// Rule 466:  ConditionalAndExpression ::= ConditionalAndExpression AND_AND InclusiveOrExpression
//
// void MakeBinaryExpression();
//

//
// Rule 467:  ConditionalAndExpressionNotName ::= InclusiveOrExpressionNotName
//
// void NoAction();
//

//
// Rule 468:  ConditionalAndExpressionNotName ::= ConditionalAndExpressionNotName AND_AND InclusiveOrExpression
//
// void MakeBinaryExpression();
//

//
// Rule 469:  ConditionalAndExpressionNotName ::= Name AND_AND InclusiveOrExpression
//
// void MakeBinaryExpression();
//

//
// Rule 470:  ConditionalOrExpression ::= ConditionalAndExpression
//
// void NoAction();
//

//
// Rule 471:  ConditionalOrExpression ::= ConditionalOrExpression OR_OR ConditionalAndExpression
//
// void MakeBinaryExpression();
//

//
// Rule 472:  ConditionalOrExpressionNotName ::= ConditionalAndExpressionNotName
//
// void NoAction();
//

//
// Rule 473:  ConditionalOrExpressionNotName ::= ConditionalOrExpressionNotName OR_OR ConditionalAndExpression
//
// void MakeBinaryExpression();
//

//
// Rule 474:  ConditionalOrExpressionNotName ::= Name OR_OR ConditionalAndExpression
//
// void MakeBinaryExpression();
//

//
// Rule 475:  ConditionalExpression ::= ConditionalOrExpression
//
// void NoAction();
//

//
// Rule 476:  ConditionalExpression ::= ConditionalOrExpression QUESTION Expression COLON ConditionalExpression
//

void Parser::MakeConditionalExpression()
{
    ConditionalExpression* p = ast_pool -> NewConditionalExpression();
    p -> test_expression = DYNAMIC_CAST<Expression*> (Sym(1));
    p -> question_token = GetToken(2);
    p -> true_expression = DYNAMIC_CAST<Expression*> (Sym(3));
    p -> colon_token = GetToken(4);
    p -> false_expression = DYNAMIC_CAST<Expression*> (Sym(5));
    Sym(1) = p;
}

//
// Rule 477:  ConditionalExpressionNotName ::= ConditionalOrExpressionNotName
//
// void NoAction();
//

//
// Rule 478:  ConditionalExpressionNotName ::= ConditionalOrExpressionNotName QUESTION Expression COLON...
//
// void MakeConditionalExpression();
//

//
// Rule 479:  ConditionalExpressionNotName ::= Name QUESTION Expression COLON ConditionalExpression
//
// void MakeConditionalExpression();
//

//
// Rule 480:  AssignmentExpression ::= ConditionalExpression
//
// void NoAction();
//
 

//
// Rule 481:  AssignmentExpression ::= Assignment
//
 

//
// Rule 481:  AssignmentExpression ::= Assignment
//
// void NoAction();
//

//
// Rule 482:  AssignmentExpressionNotName ::= ConditionalExpressionNotName
//
// void NoAction();
//

//
// Rule 483:  AssignmentExpressionNotName ::= Assignment
//
// void NoAction();
//

//
// Rule 484:  Assignment ::= PostfixExpression AssignmentOperator AssignmentExpression
//

void Parser::consumeAssignment()
{
	AssignmentExpressionEnum::AssignmentExpressionTag tag;
    switch (GetToken(2)->_kind)
    {
    case TK_EQUAL: tag = AssignmentExpressionEnum::SIMPLE_EQUAL; break;
    case TK_MULTIPLY_EQUAL: tag = AssignmentExpressionEnum::STAR_EQUAL; break;
    case TK_DIVIDE_EQUAL: tag = AssignmentExpressionEnum::SLASH_EQUAL; break;
    case TK_REMAINDER_EQUAL: tag = AssignmentExpressionEnum::MOD_EQUAL; break;
    case TK_PLUS_EQUAL: tag = AssignmentExpressionEnum::PLUS_EQUAL; break;
    case TK_MINUS_EQUAL: tag = AssignmentExpressionEnum::MINUS_EQUAL; break;
    case TK_LEFT_SHIFT_EQUAL:
        tag = AssignmentExpressionEnum::LEFT_SHIFT_EQUAL; break;
    case TK_RIGHT_SHIFT_EQUAL:
        tag = AssignmentExpressionEnum::RIGHT_SHIFT_EQUAL; break;
    case TK_UNSIGNED_RIGHT_SHIFT_EQUAL:
        tag = AssignmentExpressionEnum::UNSIGNED_RIGHT_SHIFT_EQUAL; break;
    case TK_AND_EQUAL: tag = AssignmentExpressionEnum::AND_EQUAL; break;
    case TK_XOR_EQUAL: tag = AssignmentExpressionEnum::XOR_EQUAL; break;
    case TK_OR_EQUAL: tag = AssignmentExpressionEnum::IOR_EQUAL; break;
    default: tag = AssignmentExpressionEnum::NONE;
    }
    AstAssignmentExpression* p =
        ast_pool -> NewAssignmentExpression(tag, GetToken(2));
    p -> left_hand_side = DYNAMIC_CAST<Expression*> (Sym(1));
    p -> expression = DYNAMIC_CAST<Expression*> (Sym(3));
    Sym(1) = p;
}

//
// Rule 485:  AssignmentOperator ::= EQUAL
//
// void NoAction();
//

//
// Rule 486:  AssignmentOperator ::= MULTIPLY_EQUAL
//
// void NoAction();
//

//
// Rule 487:  AssignmentOperator ::= DIVIDE_EQUAL
//
// void NoAction();
//

//
// Rule 488:  AssignmentOperator ::= REMAINDER_EQUAL
//
// void NoAction();
//

//
// Rule 489:  AssignmentOperator ::= PLUS_EQUAL
//
// void NoAction();
//

//
// Rule 490:  AssignmentOperator ::= MINUS_EQUAL
//
// void NoAction();
//

//
// Rule 491:  AssignmentOperator ::= LEFT_SHIFT_EQUAL
//
// void NoAction();
//

//
// Rule 492:  AssignmentOperator ::= RIGHT_SHIFT_EQUAL
//
// void NoAction();
//

//
// Rule 493:  AssignmentOperator ::= UNSIGNED_RIGHT_SHIFT_EQUAL
//
// void NoAction();
//

//
// Rule 494:  AssignmentOperator ::= AND_EQUAL
//
// void NoAction();
//

//
// Rule 495:  AssignmentOperator ::= XOR_EQUAL
//
// void NoAction();
//
 

//
// Rule 496:  AssignmentOperator ::= OR_EQUAL
//
 

//
// Rule 496:  AssignmentOperator ::= OR_EQUAL
//
// void NoAction();
//
 

//
// Rule 497:  Expression ::= AssignmentExpression
//
 

//
// Rule 497:  Expression ::= AssignmentExpression
//
// void NoAction();
//

//
// Rule 498:  Expressionopt ::=
//
// void NullAction();
//

//
// Rule 499:  Expressionopt ::= Expression
//
// void NoAction();
//

//
// Rule 500:  ExpressionNotName ::= AssignmentExpressionNotName
//
// void NoAction();
//

//
// Rule 501:  Marker ::=
//
// void NullAction();
//

//
// Rule 502:  ,opt ::=
//
// void NoAction();
//

//
// Rule 503:  ,opt ::= COMMA
//
// void NoAction();
//

//
// Rule 504:  Identifieropt ::=
//
// void NoAction();
//

//
// Rule 505:  Identifieropt ::= Identifier
//
// void NoAction();
//

//
// Rule 506:  PackageHeaderMarker ::=
//

//
// When this function is invoked, if the "parse_package_header_only" flag
// is turned on, we skip to the end-of-file token.
//
void Parser::consumePackageHeaderMarker()
{
    if (parse_package_header_only)
        // point to the EOF token
        lex_stream -> Reset(lex_stream -> NumTokens() - 1);
    Sym(1) = NULL;
}

//
// Rule 507:  MethodHeaderMarker ::=
//

//
// When this function is invoked, if the "parse_header_only" flag
// is turned on, the body of the method being parsed is skipped.
//
void Parser::consumeMethodHeaderMarker()
{
    if (parse_header_only)
    {
        auto token = GetToken(1);
        // If the first token immediately following the method header
        // is not an open brace, then we have a syntactic error. Do
        // nothing and let the error recovery take care of it.
        if (token->_kind == TK_LBRACE){
			lex_stream->Reset(token->MatchingBrace());
        }
          
    }
    Sym(1) = NULL;
}

//
// Rule 508:  TypeArguments ::= LESS TypeArgumentList1
//
// void SetSym1ToSym2();
//

//
// Rule 509:  TypeArgumentsopt ::=
//
// void NullAction();
//

//
// Rule 510:  TypeArgumentsopt ::= TypeArguments
//
// void NoAction();
//

//
// Rule 511:  Wildcard ::= QUESTION Marker Marker Marker
//

void Parser::MakeWildcard()
{
    AstWildcard* p = ast_pool -> NewWildcard(GetToken(1));
    if (GetToken(3)->index > GetToken(2)->index)
    {
        p -> extends_token_opt = GetToken(2);
        p -> bounds_opt = DYNAMIC_CAST<AstType*> (Sym(4));
    }
    else if (GetToken(4)->index > GetToken(3)->index)
    {
        p -> super_token_opt = GetToken(3);
        p -> bounds_opt = DYNAMIC_CAST<AstType*> (Sym(4));
    }
    Sym(1) = p;
}

//
// Rule 512:  Wildcard ::= QUESTION extends Marker ReferenceType
//
// void MakeWildcard();
//

//
// Rule 513:  Wildcard ::= QUESTION Marker super ReferenceType
//
// void MakeWildcard();
//

//
// Rule 514:  Wildcard1 ::= QUESTION Marker Marker GREATER
//
// void MakeWildcard();
//

//
// Rule 515:  Wildcard1 ::= QUESTION extends Marker ReferenceType1
//
// void MakeWildcard();
//

//
// Rule 516:  Wildcard1 ::= QUESTION Marker super ReferenceType1
//
// void MakeWildcard();
//

//
// Rule 517:  Wildcard2 ::= QUESTION Marker Marker RIGHT_SHIFT
//
// void MakeWildcard();
//

//
// Rule 518:  Wildcard2 ::= QUESTION extends Marker ReferenceType2
//
// void MakeWildcard();
//

//
// Rule 519:  Wildcard2 ::= QUESTION Marker super ReferenceType2
//
// void MakeWildcard();
//

//
// Rule 520:  Wildcard3 ::= QUESTION Marker Marker UNSIGNED_RIGHT_SHIFT
//
// void MakeWildcard();
//

//
// Rule 521:  Wildcard3 ::= QUESTION extends Marker ReferenceType3
//
// void MakeWildcard();
//

//
// Rule 522:  Wildcard3 ::= QUESTION Marker super ReferenceType3
//
// void MakeWildcard();
//

//
// Rule 523:  TypeArgumentList ::= TypeArgument
//
// void StartList();
//

//
// Rule 524:  TypeArgumentList ::= TypeArgumentList COMMA TypeArgument
//
// void AddList3();
//

//
// Rule 525:  TypeArgumentList1 ::= TypeArgument1
//
// void StartList();
//

//
// Rule 526:  TypeArgumentList1 ::= TypeArgumentList COMMA TypeArgument1
//
// void AddList3();
//

//
// Rule 527:  TypeArgumentList2 ::= TypeArgument2
//
// void StartList();
//

//
// Rule 528:  TypeArgumentList2 ::= TypeArgumentList COMMA TypeArgument2
//
// void AddList3();
//

//
// Rule 529:  TypeArgumentList3 ::= TypeArgument3
//
// void StartList();
//

//
// Rule 530:  TypeArgumentList3 ::= TypeArgumentList COMMA TypeArgument3
//
// void AddList3();
//

//
// Rule 531:  TypeArgument ::= ReferenceType
//
// void NoAction();
//

//
// Rule 532:  TypeArgument ::= Wildcard
//
// void NoAction();
//

//
// Rule 533:  TypeArgument1 ::= ReferenceType1
//
// void NoAction();
//

//
// Rule 534:  TypeArgument1 ::= Wildcard1
//
// void NoAction();
//

//
// Rule 535:  TypeArgument2 ::= ReferenceType2
//
// void NoAction();
//

//
// Rule 536:  TypeArgument2 ::= Wildcard2
//
// void NoAction();
//

//
// Rule 537:  TypeArgument3 ::= ReferenceType3
//
// void NoAction();
//

//
// Rule 538:  TypeArgument3 ::= Wildcard3
//
// void NoAction();
//

//
// Rule 539:  ReferenceType1 ::= ReferenceType GREATER
//
// void NoAction();
//

//
// Rule 540:  ReferenceType1 ::= ClassOrInterface LESS TypeArgumentList2 Marker
//
// void MakeTypeArguments();
//

//
// Rule 541:  ReferenceType2 ::= ReferenceType RIGHT_SHIFT
//
// void NoAction();
//

//
// Rule 542:  ReferenceType2 ::= ClassOrInterface LESS TypeArgumentList3 Marker
//
// void MakeTypeArguments();
//

//
// Rule 543:  ReferenceType3 ::= ReferenceType UNSIGNED_RIGHT_SHIFT
//
// void NoAction();
//

//
// Rule 544:  TypeParameters ::= LESS TypeParameterList1
//
// void SetSym1ToSym2();
//

//
// Rule 545:  TypeParametersopt ::=
//
// void NullAction();
//

//
// Rule 546:  TypeParametersopt ::= TypeParameters
//
// void NoAction();
//

//
// Rule 547:  TypeParameterList ::= TypeParameter
//
// void StartList();
//

//
// Rule 548:  TypeParameterList ::= TypeParameterList COMMA TypeParameter
//
// void AddList3();
//

//
// Rule 549:  TypeParameterList1 ::= TypeParameter1
//
// void StartList();
//

//
// Rule 550:  TypeParameterList1 ::= TypeParameterList COMMA TypeParameter1
//
// void AddList3();
//

//
// Rule 551:  TypeParameter ::= Identifier TypeBoundopt
//

void Parser::MakeTypeParameter()
{
    AstTypeParameter* p = ast_pool -> NewTypeParameter(GetToken(1));
    if (Sym(2))
    {
        //
        // Remember, we built the circular list with the first element at
        // the tail of the list, because of the grammar of TypeBound.
        //
        AstListNode* tail = DYNAMIC_CAST<AstListNode*> (Sym(2));
        p -> AllocateBounds(tail -> index + 1);
        AstListNode* root = tail;
        do
        {
            p -> AddBound(DYNAMIC_CAST<AstTypeName*> (root -> element));
            root = root -> next;
        } while (root != tail);
        FreeCircularList(tail);
    }
    Sym(1) = p;
}

//
// Rule 552:  TypeParameter1 ::= Identifier Marker GREATER
//
// void MakeTypeParameter();
//

//
// Rule 553:  TypeParameter1 ::= Identifier TypeBound1
//
// void MakeTypeParameter();
//

//
// Rule 554:  TypeBound ::= extends ReferenceType AdditionalBoundListopt
//

void Parser::MakeTypeBound()
{
    //
    // Unlike most AstListNodes, we stick Sym(2) at the end of the list
    // instead of the beginning. MakeTypeParameter expects this unusual
    // ordering, because it is easier than rewriting the grammar to build the
    // list in lexical order.
    //
    if (Sym(3))
    {
        Sym(1) = Sym(3);
        AddList2();
    }
    else
    {
        Sym(1) = Sym(2);
        StartList();
    }
}

//
// Rule 555:  TypeBoundopt ::=
//
// void NullAction();
//

//
// Rule 556:  TypeBoundopt ::= TypeBound
//
// void NoAction();
//

//
// Rule 557:  TypeBound1 ::= extends ReferenceType1 Marker
//
// void MakeTypeBound();
//

//
// Rule 558:  TypeBound1 ::= extends ReferenceType AdditionalBoundList1
//
// void MakeTypeBound();
//

//
// Rule 559:  AdditionalBoundList ::= AdditionalBound
//
// void StartList();
//

//
// Rule 560:  AdditionalBoundList ::= AdditionalBoundList AdditionalBound
//
// void AddList2();
//

//
// Rule 561:  AdditionalBoundListopt ::=
//
// void NullAction();
//

//
// Rule 562:  AdditionalBoundListopt ::= AdditionalBoundList
//
// void NoAction();
//

//
// Rule 563:  AdditionalBoundList1 ::= AdditionalBound1
//
// void StartList();
//

//
// Rule 564:  AdditionalBoundList1 ::= AdditionalBoundList AdditionalBound1
//
// void AddList2();
//

//
// Rule 565:  AdditionalBound ::= AND ClassOrInterfaceType
//
// void SetSym1ToSym2();
//

//
// Rule 566:  AdditionalBound1 ::= AND ClassOrInterfaceType1
//
// void SetSym1ToSym2();
//

//
// Rule 567:  ClassOrInterfaceType1 ::= ClassOrInterfaceType GREATER
//
// void NoAction();
//

//
// Rule 568:  ClassOrInterfaceType1 ::= ClassOrInterface LESS TypeArgumentList2 Marker
//
// void MakeTypeArguments();
//



vector<unsigned short> Parser::recovery_templates_index;
wstring Parser::recovery_templates;
vector<unsigned short> Parser::statements_recovery_filter;
std::vector< std::pair<int, std::vector<std::wstring> > > Parser::tokens_for_recovery_templates;
std::vector< int > Parser::tokens_for_statements_recovery_filter;

int getSymbol(wstring& terminalName, vector<wstring>& newName, vector<int>& newReverse) {
	auto size = newName.size();
	for (size_t j = 0; j < size; j++) {
		if (terminalName == newName[j]) {
			return newReverse[j];
		}
	}
	return -1;
}

void Parser::buildFilesForRecoveryTemplates(
	const unsigned short newTerminalIndex[],
	int newTerminalIndexLength,
	const unsigned short newNonTerminalIndex[],
	int newNonTerminalIndexLength,
	vector< std::wstring >& newName,
	const unsigned short newLhs[],
	std::vector< std::pair<int, std::vector<std::wstring> > >& tokens,
	OUT vector<unsigned short>&newRecoveyTemplatesIndex,
	OUT wstring& newRecoveyTemplates
	)
{

	vector<int> newReverse;
	computeReverseTable(newTerminalIndex, newTerminalIndexLength, newNonTerminalIndex, newNonTerminalIndexLength,
		newName, newReverse);


	newRecoveyTemplatesIndex.assign(newNonTerminalIndexLength, 0);
	newRecoveyTemplates.clear();
	newRecoveyTemplates.reserve(newNonTerminalIndexLength);
	int newRecoveyTemplatesPtr = 0;


	for (size_t i = 0; i < tokens.size(); ++i) {

		newRecoveyTemplatesPtr++;
		newRecoveyTemplates.push_back(0);

		int index = newLhs[tokens[i].first];

		newRecoveyTemplatesIndex[index] = (unsigned short)newRecoveyTemplatesPtr;



		std::vector<std::wstring>&  terminalNames = tokens[i].second;


		for (size_t j = 0; j < terminalNames.size(); j++) {
			int symbol = getSymbol(terminalNames[j], newName, newReverse);
			if (symbol > -1) {


				newRecoveyTemplates.push_back((wchar_t)symbol);
				newRecoveyTemplatesPtr++;
			}
		}

	}
	newRecoveyTemplatesPtr++;
	newRecoveyTemplates.push_back(0);
}

void Parser::computeReverseTable(
	const unsigned short newTerminalIndex[],
	int newTerminalIndexLength,
	const unsigned short newNonTerminalIndex[],
	int newNonTerminalIndexLength,
	vector< std::wstring>& newName,
	vector<int>& newReverseTable) {
	newReverseTable.clear();
	newReverseTable.insert(newReverseTable.end(), newName.size(), 0);
	size_t newNameSize = newName.size();

	for (size_t j = 0; j < newNameSize; j++) {
		{
			for (int k = 0; k < newTerminalIndexLength; k++) {
				if (newTerminalIndex[k] == j) {
					newReverseTable[j] = k;
					goto found;
				}
			}
			for (int k = 0; k < newNonTerminalIndexLength; k++) {
				if (newNonTerminalIndex[k] == j) {
					newReverseTable[j] = -k;
					goto found;
				}
			}
		}
	found:
		continue;
	}

}
vector< std::wstring> Parser::name;
vector<int> Parser::reverse_index;

void  Parser::computeName(vector< std::wstring>& OUT _name)
{
	vector<std::wstring> temp(Parser::name_size);

	for (int name_index = 0; name_index < Parser::name_size; ++name_index)
	{
		int len = Parser::name_length[name_index];
		const wchar_t* name_ = nullptr;
		name_ = &Parser::string_buffer[Parser::name_start[name_index]];
		wstring an_name_(name_, name_ + len);
		temp[name_index].assign(name_, name_ + len);
	}
	_name.swap(temp);

	vector< std::pair<wstring, int> > extra_name;
	vector< std::pair<wstring, int> > token_name;
	for (auto i = 0; i < Parser::terminal_index_length / 2; ++i)
	{
		auto idx = javaprs_table::terminal_index[i];
		if (i > 0x7f)
			extra_name.push_back(std::make_pair(Parser::name[idx], idx));
		else
		{
			token_name.push_back(std::make_pair(Parser::name[idx], idx));
		}
	}
	auto d = extra_name.size();
}
void  Parser::buildFilesForStatementsRecoveryFilter(
	const unsigned short newNonTerminalIndex[],
	const int newNonTerminalIndexLength,
	const unsigned short newLhs[],
	OUT std::vector< int >& tokens,
	vector< unsigned short>& newStatementsRecoveryFilter)
{

	newStatementsRecoveryFilter.assign(newNonTerminalIndexLength, 0);
	for (size_t i = 0; i < tokens.size(); ++i) {
		int index = newLhs[i];
		newStatementsRecoveryFilter[index] = 1;
	}
}


bool init_table_568 = Parser::buildTable();



} // Close namespace Jikes block


