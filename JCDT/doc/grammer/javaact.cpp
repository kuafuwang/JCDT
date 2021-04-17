//#line 1144 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"


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

//
// Rule 1:  Goal ::= CompilationUnitMarker CompilationUnit
//
//#line 1182 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 2:  Goal ::= MethodDeclarationMarker MethodDeclaration
//
//#line 1187 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 3:  Goal ::= AnnotationTypeMemberDeclarationMarker AnnotationTypeMemberDeclaration
//
//#line 1193 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 4:  Goal ::= BodyMarker MethodBody
//
//#line 1199 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 5:  Goal ::= ExpressionMarker Expression
//
//#line 1205 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 6:  Literal ::= IntegerLiteral
//
//#line 1213 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 7:  Literal ::= LongLiteral
//
//#line 1218 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 8:  Literal ::= FloatLiteral
//
//#line 1222 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 9:  Literal ::= DoubleLiteral
//
//#line 1226 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 10:  Literal ::= BooleanLiteral
//
// void NoAction();
//

//
// Rule 11:  Literal ::= CharacterLiteral
//
//#line 1234 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 12:  Literal ::= StringLiteral
//
//#line 1238 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 13:  Literal ::= null
//
//#line 1243 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 14:  BooleanLiteral ::= true
//
//#line 1247 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 15:  BooleanLiteral ::= false
//
//#line 1251 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 16:  Type ::= PrimitiveType
//
// void NoAction();
//

//
// Rule 17:  Type ::= ReferenceType
//
// void NoAction();
//

//
// Rule 18:  PrimitiveType ::= NumericType
//
// void NoAction();
//

//
// Rule 19:  PrimitiveType ::= boolean
//
//#line 1269 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 20:  NumericType ::= IntegralType
//
// void NoAction();
//

//
// Rule 21:  NumericType ::= FloatingPointType
//
// void NoAction();
//

//
// Rule 22:  IntegralType ::= byte
//
//#line 1281 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 23:  IntegralType ::= short
//
//#line 1285 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 24:  IntegralType ::= int
//
//#line 1289 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 25:  IntegralType ::= long
//
//#line 1293 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 26:  IntegralType ::= char
//
//#line 1297 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 27:  FloatingPointType ::= float
//
//#line 1301 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 28:  FloatingPointType ::= double
//
//#line 1305 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 29:  VoidType ::= void
//
//#line 1312 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 30:  ReferenceType ::= ClassOrInterfaceType
//
// void NoAction();
//

//
// Rule 31:  ReferenceType ::= ArrayType
//
// void NoAction();
//

//
// Rule 32:  ClassOrInterfaceType ::= ClassOrInterface
//
// void NoAction();
//

//
// Rule 33:  ClassOrInterfaceType ::= ClassOrInterface LESS TypeArgumentList1 Marker
//
//#line 1339 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 34:  ClassOrInterface ::= Name
//
//#line 1344 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 35:  ClassOrInterface ::= ClassOrInterface LESS TypeArgumentList1 DOT Name
//
//#line 1351 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 36:  ArrayType ::= PrimitiveType Dims
//
//#line 1366 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 37:  ArrayType ::= Name Dims
//
// void MakeArrayType();
//

//
// Rule 38:  ArrayType ::= ClassOrInterface LESS TypeArgumentList1 DOT Name Dims
//
//#line 1381 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 39:  ArrayType ::= ClassOrInterface LESS TypeArgumentList1 Dims
//
//#line 1388 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 40:  Name ::= Identifier
//
//#line 1400 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 
 
auto r_41 =Parser::tokens_for_recovery_templates.insert(
 Parser::tokens_for_recovery_templates.end(),make_pair(41,  vector<wstring>{L"Identifier"})); 

//
// Rule 41:  Name ::= Name DOT Marker Identifier
//
//#line 1409 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 42:  CompilationUnit ::= PackageDeclaration ImportDeclarationsopt TypeDeclarationsopt
//
//#line 1423 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 43:  CompilationUnit ::= Marker ImportDeclarations TypeDeclarationsopt
//
// void MakeCompilationUnit();
//

//
// Rule 44:  CompilationUnit ::= TypeDeclarationsopt
//
//#line 1440 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 45:  ImportDeclarations ::= ImportDeclaration
//
//#line 1444 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 46:  ImportDeclarations ::= ImportDeclarations ImportDeclaration
//
//#line 1448 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 47:  ImportDeclarationsopt ::=
//
//#line 1452 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 48:  ImportDeclarationsopt ::= ImportDeclarations
//
// void NoAction();
//

//
// Rule 49:  TypeDeclarations ::= TypeDeclaration
//
// void StartList();
//

//
// Rule 50:  TypeDeclarations ::= TypeDeclarations TypeDeclaration
//
// void AddList2();
//

//
// Rule 51:  TypeDeclarationsopt ::=
//
// void NullAction();
//

//
// Rule 52:  TypeDeclarationsopt ::= TypeDeclarations
//
// void NoAction();
//

//
// Rule 53:  PackageDeclaration ::= Marker package Name PackageHeaderMarker SEMICOLON
//
//#line 1481 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 54:  PackageDeclaration ::= Modifiers package Name PackageHeaderMarker SEMICOLON
//
// void MakePackageDeclaration();
//

//
// Rule 55:  ImportDeclaration ::= SingleTypeImportDeclaration
//
// void NoAction();
//

//
// Rule 56:  ImportDeclaration ::= TypeImportOnDemandDeclaration
//
// void NoAction();
//

//
// Rule 57:  ImportDeclaration ::= SingleStaticImportDeclaration
//
// void NoAction();
//

//
// Rule 58:  ImportDeclaration ::= StaticImportOnDemandDeclaration
//
// void NoAction();
//

//
// Rule 59:  SingleTypeImportDeclaration ::= import Marker Name Marker Marker SEMICOLON
//
//#line 1525 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 60:  TypeImportOnDemandDeclaration ::= import Marker Name DOT MULTIPLY SEMICOLON
//
// void MakeImportDeclaration();
//

//
// Rule 61:  SingleStaticImportDeclaration ::= import static Name Marker Marker SEMICOLON
//
// void MakeImportDeclaration();
//

//
// Rule 62:  StaticImportOnDemandDeclaration ::= import static Name DOT MULTIPLY SEMICOLON
//
// void MakeImportDeclaration();
//

//
// Rule 63:  TypeDeclaration ::= ClassDeclaration
//
// void NoAction();
//

//
// Rule 64:  TypeDeclaration ::= EnumDeclaration
//
// void NoAction();
//

//
// Rule 65:  TypeDeclaration ::= InterfaceDeclaration
//
// void NoAction();
//

//
// Rule 66:  TypeDeclaration ::= AnnotationTypeDeclaration
//
// void NoAction();
//

//
// Rule 67:  TypeDeclaration ::= SEMICOLON
//
//#line 1586 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 68:  Modifiers ::= Modifier
//
// void StartList();
//

//
// Rule 69:  Modifiers ::= Modifiers Modifier
//
// void AddList2();
//

//
// Rule 70:  Modifiersopt ::=
//
// void NullAction();
//

//
// Rule 71:  Modifiersopt ::= Modifiers
//
// void NoAction();
//

//
// Rule 72:  Modifier ::= public
//
//#line 1618 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 73:  Modifier ::= protected
//
// void MakeModifier();
//

//
// Rule 74:  Modifier ::= private
//
// void MakeModifier();
//

//
// Rule 75:  Modifier ::= static
//
// void MakeModifier();
//

//
// Rule 76:  Modifier ::= abstract
//
// void MakeModifier();
//

//
// Rule 77:  Modifier ::= final
//
// void MakeModifier();
//

//
// Rule 78:  Modifier ::= native
//
// void MakeModifier();
//

//
// Rule 79:  Modifier ::= strictfp
//
// void MakeModifier();
//

//
// Rule 80:  Modifier ::= synchronized
//
// void MakeModifier();
//

//
// Rule 81:  Modifier ::= transient
//
// void MakeModifier();
//

//
// Rule 82:  Modifier ::= volatile
//
// void MakeModifier();
//

//
// Rule 83:  Modifier ::= Annotation
//
// void NoAction();
//

//
// Rule 84:  Annotation ::= NormalAnnotation
//
// void NoAction();
//

//
// Rule 85:  Annotation ::= MarkerAnnotation
//
// void NoAction();
//

//
// Rule 86:  Annotation ::= SingleMemberAnnotation
//
// void NoAction();
//

//
// Rule 87:  NormalAnnotation ::= AT Name LPAREN MemberValuePairsopt RPAREN
//
//#line 1725 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 88:  MemberValuePairs ::= MemberValuePair
//
// void StartList();
//

//
// Rule 89:  MemberValuePairs ::= MemberValuePairs COMMA MemberValuePair
//
// void AddList3();
//

//
// Rule 90:  MemberValuePairsopt ::=
//
// void NullAction();
//

//
// Rule 91:  MemberValuePairsopt ::= MemberValuePairs
//
// void NoAction();
//

//
// Rule 92:  MemberValuePair ::= Identifier EQUAL MemberValue
//
//#line 1761 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 93:  MemberValue ::= ConditionalExpression
//
// void NoAction();
//

//
// Rule 94:  MemberValue ::= Annotation
//
// void NoAction();
//
 
auto r_95 =Parser::tokens_for_recovery_templates.insert(
 Parser::tokens_for_recovery_templates.end(),make_pair(95,  vector<wstring>{L"Identifier"})); 

//
// Rule 95:  MemberValue ::= MemberValueArrayInitializer
//
//#line 1783 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 95:  MemberValue ::= MemberValueArrayInitializer
//
// void NoAction();
//

//
// Rule 96:  MemberValueArrayInitializer ::= LBRACE Marker ,opt RBRACE
//
//#line 1793 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 97:  MemberValueArrayInitializer ::= LBRACE MemberValues ,opt RBRACE
//
// void MakeArrayInitializer();
//

//
// Rule 98:  MemberValues ::= MemberValue
//
// void StartList();
//

//
// Rule 99:  MemberValues ::= MemberValues COMMA MemberValue
//
// void AddList3();
//

//
// Rule 100:  MarkerAnnotation ::= AT Name Marker Marker Marker
//
// void MakeAnnotation();
//

//
// Rule 101:  SingleMemberAnnotation ::= AT Name LPAREN MemberValue RPAREN
//
//#line 1837 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 102:  ClassDeclaration ::= Marker class Identifier TypeParametersopt Superopt Interfacesopt ClassBody
//
//#line 1860 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 103:  ClassDeclaration ::= Modifiers class Identifier TypeParametersopt Superopt Interfacesopt ClassBody
//
//#line 1871 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 104:  Super ::= extends ClassOrInterfaceType
//
// void SetSym1ToSym2();
//

//
// Rule 105:  Superopt ::=
//
// void NullAction();
//

//
// Rule 106:  Superopt ::= Super
//
// void NoAction();
//

//
// Rule 107:  Interfaces ::= implements TypeList
//
// void SetSym1ToSym2();
//

//
// Rule 108:  Interfacesopt ::=
//
// void NullAction();
//

//
// Rule 109:  Interfacesopt ::= Interfaces
//
// void NoAction();
//

//
// Rule 110:  TypeList ::= ClassOrInterfaceType
//
// void StartList();
//

//
// Rule 111:  TypeList ::= TypeList COMMA ClassOrInterfaceType
//
// void AddList3();
//
auto no_r_112 = Parser::tokens_for_statements_recovery_filter.insert(
Parser::tokens_for_statements_recovery_filter.end(), 112);
 

//
// Rule 112:  ClassBody ::= LBRACE ClassBodyDeclarationsopt RBRACE
//
//#line 1928 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 113:  ClassBodyopt ::=
//
// void NullAction();
//
auto no_r_114 = Parser::tokens_for_statements_recovery_filter.insert(
Parser::tokens_for_statements_recovery_filter.end(), 114);
 

//
// Rule 114:  ClassBodyopt ::= ClassBody
//
//#line 1937 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 114:  ClassBodyopt ::= ClassBody
//
// void NoAction();
//

//
// Rule 115:  ClassBodyDeclarations ::= ClassBodyDeclaration
//
// void StartList();
//

//
// Rule 116:  ClassBodyDeclarations ::= ClassBodyDeclarations ClassBodyDeclaration
//
// void AddList2();
//

//
// Rule 117:  ClassBodyDeclarationsopt ::=
//
// void NullAction();
//

//
// Rule 118:  ClassBodyDeclarationsopt ::= ClassBodyDeclarations
//
// void NoAction();
//

//
// Rule 119:  ClassBodyDeclaration ::= ConstructorDeclaration
//
// void NoAction();
//

//
// Rule 120:  ClassBodyDeclaration ::= InitializerDeclaration
//
// void NoAction();
//

//
// Rule 121:  ClassBodyDeclaration ::= FieldDeclaration
//
// void NoAction();
//

//
// Rule 122:  ClassBodyDeclaration ::= MethodDeclaration
//
// void NoAction();
//

//
// Rule 123:  ClassBodyDeclaration ::= TypeDeclaration
//
// void NoAction();
//

//
// Rule 124:  FieldDeclaration ::= Marker Marker Type VariableDeclarators SEMICOLON
//
//#line 2012 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 125:  FieldDeclaration ::= Modifiers Marker Type VariableDeclarators SEMICOLON
//
// void MakeFieldDeclaration();
//

//
// Rule 126:  VariableDeclarators ::= VariableDeclarator
//
// void StartList();
//
 
auto r_127 =Parser::tokens_for_recovery_templates.insert(
 Parser::tokens_for_recovery_templates.end(),make_pair(127,  vector<wstring>{L"Identifier"})); 

//
// Rule 127:  VariableDeclarators ::= VariableDeclarators COMMA VariableDeclarator
//
//#line 2028 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 127:  VariableDeclarators ::= VariableDeclarators COMMA VariableDeclarator
//
// void AddList3();
//

//
// Rule 128:  VariableDeclarator ::= VariableDeclaratorId
//
//#line 2033 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 
 
auto r_129 =Parser::tokens_for_recovery_templates.insert(
 Parser::tokens_for_recovery_templates.end(),make_pair(129,  vector<wstring>{L"Identifier"})); 

//
// Rule 129:  VariableDeclarator ::= VariableDeclaratorId EQUAL VariableInitializer
//
//#line 2038 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"
 
auto r_130 =Parser::tokens_for_recovery_templates.insert(
 Parser::tokens_for_recovery_templates.end(),make_pair(130,  vector<wstring>{L"Identifier"})); 

//
// Rule 130:  VariableDeclaratorId ::= Identifier Dimsopt
//
//#line 2043 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 131:  VariableInitializer ::= Expression
//
// void NoAction();
//
 
auto r_132 =Parser::tokens_for_recovery_templates.insert(
 Parser::tokens_for_recovery_templates.end(),make_pair(132,  vector<wstring>{L"Identifier"})); 

//
// Rule 132:  VariableInitializer ::= ArrayInitializer
//
//#line 2052 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 132:  VariableInitializer ::= ArrayInitializer
//
// void NoAction();
//

//
// Rule 133:  MethodDeclaration ::= MethodHeader MethodHeaderMarker MethodBody
//
//#line 2069 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 134:  MethodDeclaration ::= MethodHeader MethodHeaderMarker Marker SEMICOLON
//
// void MakeMethodDeclaration();
//

//
// Rule 135:  MethodHeader ::= Marker Marker Type MethodDeclarator Throwsopt
//
//#line 2089 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 136:  MethodHeader ::= Modifiers Marker Type MethodDeclarator Throwsopt
//
// void MakeMethodHeader();
//

//
// Rule 137:  MethodHeader ::= Marker TypeParameters Type MethodDeclarator Throwsopt
//
// void MakeMethodHeader();
//

//
// Rule 138:  MethodHeader ::= Modifiers TypeParameters Type MethodDeclarator Throwsopt
//
// void MakeMethodHeader();
//

//
// Rule 139:  MethodHeader ::= Marker Marker VoidType MethodDeclarator Throwsopt
//
// void MakeMethodHeader();
//

//
// Rule 140:  MethodHeader ::= Modifiers Marker VoidType MethodDeclarator Throwsopt
//
// void MakeMethodHeader();
//

//
// Rule 141:  MethodHeader ::= Marker TypeParameters VoidType MethodDeclarator Throwsopt
//
// void MakeMethodHeader();
//

//
// Rule 142:  MethodHeader ::= Modifiers TypeParameters VoidType MethodDeclarator Throwsopt
//
// void MakeMethodHeader();
//

//
// Rule 143:  MethodDeclarator ::= Identifier LPAREN FormalParameterListopt RPAREN Dimsopt
//
//#line 2173 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 144:  FormalParameterList ::= LastFormalParameter
//
// void StartList();
//

//
// Rule 145:  FormalParameterList ::= FormalParameters COMMA LastFormalParameter
//
// void AddList3();
//

//
// Rule 146:  FormalParameterListopt ::=
//
// void NullAction();
//

//
// Rule 147:  FormalParameterListopt ::= FormalParameterList
//
// void NoAction();
//

//
// Rule 148:  FormalParameters ::= FormalParameter
//
// void StartList();
//

//
// Rule 149:  FormalParameters ::= FormalParameters COMMA FormalParameter
//
// void AddList3();
//

//
// Rule 150:  FormalParameter ::= Type Marker Marker VariableDeclaratorId
//
//#line 2221 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 151:  FormalParameter ::= Modifiers Type Marker VariableDeclaratorId
//
// void MakeFormalParameter();
//

//
// Rule 152:  LastFormalParameter ::= FormalParameter
//
// void NoAction();
//

//
// Rule 153:  LastFormalParameter ::= Type Marker ELLIPSIS VariableDeclaratorId
//
// void MakeFormalParameter();
//
 
auto r_154 =Parser::tokens_for_recovery_templates.insert(
 Parser::tokens_for_recovery_templates.end(),make_pair(154,  vector<wstring>{L"Identifier",L"Identifier"})); 

//
// Rule 154:  LastFormalParameter ::= Modifiers Type ELLIPSIS VariableDeclaratorId
//
//#line 2264 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 154:  LastFormalParameter ::= Modifiers Type ELLIPSIS VariableDeclaratorId
//
// void MakeFormalParameter();
//

//
// Rule 155:  Throws ::= throws TypeList
//
// void SetSym1ToSym2();
//

//
// Rule 156:  Throwsopt ::=
//
// void NullAction();
//

//
// Rule 157:  Throwsopt ::= Throws
//
// void NoAction();
//
 auto no_r_158 = Parser::tokens_for_statements_recovery_filter.insert(
Parser::tokens_for_statements_recovery_filter.end(), 158);
 

//
// Rule 158:  MethodBody ::= LBRACE BlockStatementsopt RBRACE
//
//#line 2298 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 159:  InitializerDeclaration ::= Marker MethodHeaderMarker MethodBody
//
//#line 2316 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 160:  InitializerDeclaration ::= Modifiers MethodHeaderMarker MethodBody
//
// void MakeInitializerDeclaration();
//

//
// Rule 161:  ConstructorDeclaration ::= Marker Marker ConstructorDeclarator Throwsopt MethodHeaderMarker MethodBody
//
//#line 2341 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 162:  ConstructorDeclaration ::= Modifiers Marker ConstructorDeclarator Throwsopt MethodHeaderMarker MethodBody
//
//#line 2351 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 163:  ConstructorDeclaration ::= Marker TypeParameters ConstructorDeclarator Throwsopt MethodHeaderMarker MethodBody
//
//#line 2362 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 164:  ConstructorDeclaration ::= Modifiers TypeParameters ConstructorDeclarator Throwsopt MethodHeaderMarker...
//
//#line 2371 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 165:  ConstructorDeclarator ::= Identifier LPAREN FormalParameterListopt RPAREN Marker
//
// void MakeMethodDeclarator();
//

//
// Rule 166:  ExplicitConstructorInvocation ::= this Arguments SEMICOLON
//
//#line 2400 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 167:  ExplicitConstructorInvocation ::= TypeArguments this Arguments SEMICOLON
//
//#line 2407 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 168:  ExplicitConstructorInvocation ::= super Arguments SEMICOLON
//
//#line 2415 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 169:  ExplicitConstructorInvocation ::= TypeArguments super Arguments SEMICOLON
//
//#line 2422 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 170:  ExplicitConstructorInvocation ::= Primary DOT TypeArgumentsopt super Arguments SEMICOLON
//
//#line 2433 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 171:  ExplicitConstructorInvocation ::= Name DOT Marker super Arguments SEMICOLON
//
// void MakeQualifiedSuper();
//

//
// Rule 172:  ExplicitConstructorInvocation ::= Name DOT TypeArguments super Arguments SEMICOLON
//
// void MakeQualifiedSuper();
//

//
// Rule 173:  EnumDeclaration ::= Marker enum Identifier Interfacesopt EnumBody
//
//#line 2469 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 174:  EnumDeclaration ::= Modifiers enum Identifier Interfacesopt EnumBody
//
//#line 2478 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 175:  EnumBody ::= LBRACE Marker ,opt EnumBodyDeclarationsopt RBRACE
//
//#line 2488 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 176:  EnumBody ::= LBRACE EnumConstants ,opt EnumBodyDeclarationsopt RBRACE
//
// void MakeEnumBody();
//

//
// Rule 177:  EnumConstants ::= EnumConstant
//
// void StartList();
//

//
// Rule 178:  EnumConstants ::= EnumConstants COMMA EnumConstant
//
// void AddList3();
//

//
// Rule 179:  EnumConstant ::= Modifiersopt Identifier Argumentsopt ClassBodyopt
//
//#line 2519 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 180:  Arguments ::= LPAREN ArgumentListopt RPAREN
//
//#line 2530 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 181:  Argumentsopt ::=
//
// void NullAction();
//

//
// Rule 182:  Argumentsopt ::= Arguments
//
// void NoAction();
//

//
// Rule 183:  EnumBodyDeclarations ::= SEMICOLON ClassBodyDeclarationsopt Marker
//
// void MakeClassBody();
//

//
// Rule 184:  EnumBodyDeclarationsopt ::=
//
// void NullAction();
//

//
// Rule 185:  EnumBodyDeclarationsopt ::= EnumBodyDeclarations
//
// void NoAction();
//

//
// Rule 186:  InterfaceDeclaration ::= Marker interface Identifier TypeParametersopt ExtendsInterfacesopt InterfaceBody
//
//#line 2582 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 187:  InterfaceDeclaration ::= Modifiers interface Identifier TypeParametersopt ExtendsInterfacesopt InterfaceBody
//
//#line 2592 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 188:  ExtendsInterfaces ::= extends TypeList
//
// void SetSym1ToSym2();
//

//
// Rule 189:  ExtendsInterfacesopt ::=
//
// void NullAction();
//

//
// Rule 190:  ExtendsInterfacesopt ::= ExtendsInterfaces
//
// void NoAction();
//

//
// Rule 191:  InterfaceBody ::= LBRACE InterfaceMemberDeclarationsopt RBRACE
//
// void MakeClassBody();
//

//
// Rule 192:  InterfaceMemberDeclarations ::= InterfaceMemberDeclaration
//
// void StartList();
//

//
// Rule 193:  InterfaceMemberDeclarations ::= InterfaceMemberDeclarations InterfaceMemberDeclaration
//
// void AddList2();
//

//
// Rule 194:  InterfaceMemberDeclarationsopt ::=
//
// void NullAction();
//

//
// Rule 195:  InterfaceMemberDeclarationsopt ::= InterfaceMemberDeclarations
//
// void NoAction();
//

//
// Rule 196:  InterfaceMemberDeclaration ::= ConstantDeclaration
//
// void NoAction();
//

//
// Rule 197:  InterfaceMemberDeclaration ::= TypeDeclaration
//
// void NoAction();
//

//
// Rule 198:  ConstantDeclaration ::= FieldDeclaration
//
//#line 2666 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 199:  InterfaceMemberDeclaration ::= MethodDeclaration
//
// void NoAction();
//

//
// Rule 200:  AnnotationTypeDeclaration ::= AT Marker interface Identifier AnnotationTypeBody
//
//#line 2686 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 201:  AnnotationTypeDeclaration ::= Modifiers AT interface Identifier AnnotationTypeBody
//
//#line 2694 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 202:  AnnotationTypeBody ::= LBRACE AnnotationTypeMemberDeclarationsopt RBRACE
//
// void MakeClassBody();
//

//
// Rule 203:  AnnotationTypeMemberDeclarations ::= AnnotationTypeMemberDeclaration
//
// void StartList();
//

//
// Rule 204:  AnnotationTypeMemberDeclarations ::= AnnotationTypeMemberDeclarations AnnotationTypeMemberDeclaration
//
// void AddList2();
//

//
// Rule 205:  AnnotationTypeMemberDeclarationsopt ::=
//
// void NullAction();
//

//
// Rule 206:  AnnotationTypeMemberDeclarationsopt ::= AnnotationTypeMemberDeclarations
//
// void NoAction();
//

//
// Rule 207:  AnnotationTypeMemberDeclaration ::= Marker Marker Type Identifier LPAREN RPAREN DefaultValueopt SEMICOLON
//
//#line 2745 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 208:  AnnotationTypeMemberDeclaration ::= Modifiers Marker Type Identifier LPAREN RPAREN DefaultValueopt SEMICOLON
//
// void MakeAnnotationTypeMemberDeclaration();
//

//
// Rule 209:  AnnotationTypeMemberDeclaration ::= ConstantDeclaration
//
// void NoAction();
//

//
// Rule 210:  AnnotationTypeMemberDeclaration ::= TypeDeclaration
//
// void NoAction();
//

//
// Rule 211:  DefaultValue ::= default MemberValue
//
// void SetSym1ToSym2();
//

//
// Rule 212:  DefaultValueopt ::=
//
// void NullAction();
//

//
// Rule 213:  DefaultValueopt ::= DefaultValue
//
// void NoAction();
//

//
// Rule 214:  ArrayInitializer ::= LBRACE Marker ,opt RBRACE
//
// void MakeArrayInitializer();
//
 
auto r_215 =Parser::tokens_for_recovery_templates.insert(
 Parser::tokens_for_recovery_templates.end(),make_pair(215,  vector<wstring>{L"Identifier"})); 

//
// Rule 215:  ArrayInitializer ::= LBRACE VariableInitializers ,opt RBRACE
//
//#line 2818 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 215:  ArrayInitializer ::= LBRACE VariableInitializers ,opt RBRACE
//
// void MakeArrayInitializer();
//

//
// Rule 216:  VariableInitializers ::= VariableInitializer
//
// void StartList();
//

//
// Rule 217:  VariableInitializers ::= VariableInitializers COMMA VariableInitializer
//
// void AddList3();
//

//
// Rule 218:  Block ::= LBRACE BlockStatementsopt RBRACE
//
//#line 2836 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 219:  BlockStatements ::= BlockStatement
//
// void StartList();
//

//
// Rule 220:  BlockStatements ::= BlockStatements BlockStatement
//
// void AddList2();
//

//
// Rule 221:  BlockStatementsopt ::=
//
// void NullAction();
//

//
// Rule 222:  BlockStatementsopt ::= BlockStatements
//
// void NoAction();
//

//
// Rule 223:  BlockStatement ::= LocalVariableDeclarationStatement
//
// void NoAction();
//

//
// Rule 224:  BlockStatement ::= Statement
//
// void NoAction();
//

//
// Rule 225:  BlockStatement ::= ClassDeclaration
//
//#line 2865 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 226:  BlockStatement ::= EnumDeclaration
//
//#line 2872 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 227:  BlockStatement ::= ExplicitConstructorInvocation
//
// void NoAction();
//

//
// Rule 228:  LocalVariableDeclarationStatement ::= LocalVariableDeclaration SEMICOLON
//
//#line 2885 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 229:  LocalVariableDeclaration ::= Type Marker Marker VariableDeclarators
//
//#line 2895 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 230:  LocalVariableDeclaration ::= Modifiers Type Marker VariableDeclarators
//
// void MakeLocalVariable();
//

//
// Rule 231:  Statement ::= StatementWithoutTrailingSubstatement
//
// void NoAction();
//

//
// Rule 232:  Statement ::= LabeledStatement
//
// void NoAction();
//

//
// Rule 233:  Statement ::= IfThenStatement
//
// void NoAction();
//

//
// Rule 234:  Statement ::= IfThenElseStatement
//
// void NoAction();
//

//
// Rule 235:  Statement ::= WhileStatement
//
// void NoAction();
//

//
// Rule 236:  Statement ::= ForStatement
//
// void NoAction();
//
 
auto r_237 =Parser::tokens_for_recovery_templates.insert(
 Parser::tokens_for_recovery_templates.end(),make_pair(237,  vector<wstring>{L";"})); 

//
// Rule 237:  Statement ::= ForeachStatement
//
//#line 2942 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 237:  Statement ::= ForeachStatement
//
// void NoAction();
//

//
// Rule 238:  StatementNoShortIf ::= StatementWithoutTrailingSubstatement
//
// void NoAction();
//

//
// Rule 239:  StatementNoShortIf ::= LabeledStatementNoShortIf
//
// void NoAction();
//

//
// Rule 240:  StatementNoShortIf ::= IfThenElseStatementNoShortIf
//
// void NoAction();
//

//
// Rule 241:  StatementNoShortIf ::= WhileStatementNoShortIf
//
// void NoAction();
//

//
// Rule 242:  StatementNoShortIf ::= ForStatementNoShortIf
//
// void NoAction();
//

//
// Rule 243:  StatementNoShortIf ::= ForeachStatementNoShortIf
//
// void NoAction();
//

//
// Rule 244:  StatementWithoutTrailingSubstatement ::= Block
//
// void NoAction();
//

//
// Rule 245:  StatementWithoutTrailingSubstatement ::= EmptyStatement
//
// void NoAction();
//

//
// Rule 246:  StatementWithoutTrailingSubstatement ::= ExpressionStatement
//
// void NoAction();
//

//
// Rule 247:  StatementWithoutTrailingSubstatement ::= SwitchStatement
//
// void NoAction();
//

//
// Rule 248:  StatementWithoutTrailingSubstatement ::= DoStatement
//
// void NoAction();
//

//
// Rule 249:  StatementWithoutTrailingSubstatement ::= BreakStatement
//
// void NoAction();
//

//
// Rule 250:  StatementWithoutTrailingSubstatement ::= ContinueStatement
//
// void NoAction();
//

//
// Rule 251:  StatementWithoutTrailingSubstatement ::= ReturnStatement
//
// void NoAction();
//

//
// Rule 252:  StatementWithoutTrailingSubstatement ::= SynchronizedStatement
//
// void NoAction();
//

//
// Rule 253:  StatementWithoutTrailingSubstatement ::= ThrowStatement
//
// void NoAction();
//

//
// Rule 254:  StatementWithoutTrailingSubstatement ::= TryStatement
//
// void NoAction();
//

//
// Rule 255:  StatementWithoutTrailingSubstatement ::= AssertStatement
//
// void NoAction();
//

//
// Rule 256:  EmptyStatement ::= SEMICOLON
//
//#line 3023 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 257:  LabeledStatement ::= Identifier COLON Statement
//
//#line 3027 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 258:  LabeledStatementNoShortIf ::= Identifier COLON StatementNoShortIf
//
// void MakeLabeledStatement();
//

//
// Rule 259:  ExpressionStatement ::= StatementExpression SEMICOLON
//
//#line 3038 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 260:  StatementExpression ::= Assignment
//
//#line 3042 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 261:  StatementExpression ::= PreIncrementExpression
//
// void MakeExpressionStatement();
//

//
// Rule 262:  StatementExpression ::= PreDecrementExpression
//
// void MakeExpressionStatement();
//

//
// Rule 263:  StatementExpression ::= PostIncrementExpression
//
// void MakeExpressionStatement();
//

//
// Rule 264:  StatementExpression ::= PostDecrementExpression
//
// void MakeExpressionStatement();
//

//
// Rule 265:  StatementExpression ::= MethodInvocation
//
// void MakeExpressionStatement();
//

//
// Rule 266:  StatementExpression ::= ClassInstanceCreationExpression
//
// void MakeExpressionStatement();
//

//
// Rule 267:  IfThenStatement ::= if LPAREN Expression RPAREN Statement Marker Marker
//
//#line 3092 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 268:  IfThenElseStatement ::= if LPAREN Expression RPAREN StatementNoShortIf else Statement
//
// void MakeIfThenElseStatement();
//

//
// Rule 269:  IfThenElseStatementNoShortIf ::= if LPAREN Expression RPAREN StatementNoShortIf else StatementNoShortIf
//
// void MakeIfThenElseStatement();
//

//
// Rule 270:  SwitchStatement ::= switch LPAREN Expression RPAREN SwitchBlock
//
//#line 3112 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 271:  SwitchBlock ::= LBRACE SwitchBlockStatements SwitchLabelsopt RBRACE
//
//#line 3122 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 272:  SwitchBlock ::= LBRACE SwitchLabelsopt RBRACE
//
//#line 3126 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 273:  SwitchBlockStatements ::= SwitchBlockStatement
//
// void StartList();
//

//
// Rule 274:  SwitchBlockStatements ::= SwitchBlockStatements SwitchBlockStatement
//
// void AddList2();
//

//
// Rule 275:  SwitchBlockStatement ::= SwitchLabels BlockStatements
//
//#line 3138 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 276:  SwitchLabels ::= SwitchLabel
//
// void StartList();
//

//
// Rule 277:  SwitchLabels ::= SwitchLabels SwitchLabel
//
// void AddList2();
//

//
// Rule 278:  SwitchLabelsopt ::=
//
// void NullAction();
//

//
// Rule 279:  SwitchLabelsopt ::= SwitchLabels
//
// void NoAction();
//

//
// Rule 280:  SwitchLabel ::= case Expression COLON
//
//#line 3162 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 281:  SwitchLabel ::= default Marker COLON
//
//#line 3170 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 282:  WhileStatement ::= while LPAREN Expression RPAREN Statement
//
//#line 3174 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 283:  WhileStatementNoShortIf ::= while LPAREN Expression RPAREN StatementNoShortIf
//
//#line 3178 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 284:  DoStatement ::= do Statement while LPAREN Expression RPAREN SEMICOLON
//
//#line 3182 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 285:  ForStatement ::= for LPAREN ForInitopt SEMICOLON Expressionopt SEMICOLON ForUpdateopt RPAREN Statement
//
//#line 3187 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 286:  ForStatementNoShortIf ::= for LPAREN ForInitopt SEMICOLON Expressionopt SEMICOLON ForUpdateopt RPAREN...
//
//#line 3192 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 287:  ForInit ::= StatementExpressionList
//
// void NoAction();
//

//
// Rule 288:  ForInit ::= LocalVariableDeclaration
//
// void StartList();
//

//
// Rule 289:  ForInitopt ::=
//
// void NullAction();
//

//
// Rule 290:  ForInitopt ::= ForInit
//
// void NoAction();
//

//
// Rule 291:  ForUpdate ::= StatementExpressionList
//
// void NoAction();
//

//
// Rule 292:  ForUpdateopt ::=
//
// void NullAction();
//

//
// Rule 293:  ForUpdateopt ::= ForUpdate
//
// void NoAction();
//

//
// Rule 294:  StatementExpressionList ::= StatementExpression
//
// void StartList();
//

//
// Rule 295:  StatementExpressionList ::= StatementExpressionList COMMA StatementExpression
//
// void AddList3();
//

//
// Rule 296:  ForeachStatement ::= for LPAREN FormalParameter COLON Expression RPAREN Statement
//
//#line 3232 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 297:  ForeachStatementNoShortIf ::= for LPAREN FormalParameter COLON Expression RPAREN StatementNoShortIf
//
//#line 3237 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 298:  AssertStatement ::= assert Expression Marker Marker SEMICOLON
//
//#line 3245 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 299:  AssertStatement ::= assert Expression COLON Expression SEMICOLON
//
//#line 3249 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 300:  BreakStatement ::= break Identifieropt SEMICOLON
//
//#line 3253 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 301:  ContinueStatement ::= continue Identifieropt SEMICOLON
//
//#line 3257 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 302:  ReturnStatement ::= return Expressionopt SEMICOLON
//
//#line 3261 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 303:  ThrowStatement ::= throw Expression SEMICOLON
//
//#line 3265 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 304:  SynchronizedStatement ::= synchronized LPAREN Expression RPAREN Block
//
//#line 3269 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 305:  TryStatement ::= try Block Catches Marker
//
//#line 3277 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 306:  TryStatement ::= try Block Catchesopt Finally
//
// void MakeTryStatement();
//

//
// Rule 307:  Catches ::= CatchClause
//
// void StartList();
//

//
// Rule 308:  Catches ::= Catches CatchClause
//
// void AddList2();
//

//
// Rule 309:  Catchesopt ::=
//
// void NullAction();
//

//
// Rule 310:  Catchesopt ::= Catches
//
// void NoAction();
//

//
// Rule 311:  CatchClause ::= catch LPAREN FormalParameter RPAREN Block
//
//#line 3304 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"
 
auto r_312 =Parser::tokens_for_recovery_templates.insert(
 Parser::tokens_for_recovery_templates.end(),make_pair(312,  vector<wstring>{L"finally", L"{", L"}" })); 

//
// Rule 312:  Finally ::= finally Block
//
//#line 3309 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 313:  Primary ::= PrimaryNoNewArray
//
// void NoAction();
//

//
// Rule 314:  Primary ::= ArrayCreationUninitialized
//
// void NoAction();
//

//
// Rule 315:  Primary ::= ArrayCreationInitialized
//
// void NoAction();
//

//
// Rule 316:  PrimaryNoNewArray ::= Literal
//
// void NoAction();
//

//
// Rule 317:  PrimaryNoNewArray ::= this
//
//#line 3338 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 318:  PrimaryNoNewArray ::= LPAREN Name Marker RPAREN
//
//#line 3350 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 319:  PrimaryNoNewArray ::= LPAREN ExpressionNotName Marker RPAREN
//
//#line 3357 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 320:  PrimaryNoNewArray ::= ClassInstanceCreationExpression
//
// void NoAction();
//

//
// Rule 321:  PrimaryNoNewArray ::= FieldAccess
//
// void NoAction();
//

//
// Rule 322:  PrimaryNoNewArray ::= Name DOT this
//
//#line 3375 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 323:  PrimaryNoNewArray ::= PrimitiveType Dimsopt DOT class
//
//#line 3384 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 324:  PrimaryNoNewArray ::= Name Dims DOT class
//
//#line 3388 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 325:  PrimaryNoNewArray ::= Name DOT Marker class
//
//#line 3396 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 326:  PrimaryNoNewArray ::= VoidType DOT Marker class
//
//#line 3404 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 327:  PrimaryNoNewArray ::= MethodInvocation
//
// void NoAction();
//

//
// Rule 328:  PrimaryNoNewArray ::= ArrayAccess
//
// void NoAction();
//

//
// Rule 329:  ClassInstanceCreationExpression ::= new ClassOrInterfaceType Arguments ClassBodyopt
//
//#line 3428 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 330:  ClassInstanceCreationExpression ::= new TypeArguments ClassOrInterfaceType Arguments ClassBodyopt
//
//#line 3438 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 331:  ClassInstanceCreationExpression ::= Primary DOT new TypeArgumentsopt Identifier TypeArgumentsopt Arguments...
//
//#line 3450 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 332:  ClassInstanceCreationExpression ::= Name DOT new TypeArgumentsopt Identifier TypeArgumentsopt Arguments...
//
//#line 3462 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 333:  ArgumentList ::= Expression
//
// void StartList();
//

//
// Rule 334:  ArgumentList ::= ArgumentList COMMA Expression
//
// void AddList3();
//

//
// Rule 335:  ArgumentListopt ::=
//
// void NullAction();
//

//
// Rule 336:  ArgumentListopt ::= ArgumentList
//
// void NoAction();
//

//
// Rule 337:  ArrayCreationUninitialized ::= new PrimitiveType DimExprs Dimsopt
//
//#line 3488 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 338:  ArrayCreationUninitialized ::= new ClassOrInterfaceType DimExprs Dimsopt
//
//#line 3493 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 339:  ArrayCreationInitialized ::= new PrimitiveType Dims ArrayInitializer
//
//#line 3503 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 340:  ArrayCreationInitialized ::= new ClassOrInterfaceType Dims ArrayInitializer
//
//#line 3507 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 341:  DimExprs ::= DimExpr
//
// void StartList();
//

//
// Rule 342:  DimExprs ::= DimExprs DimExpr
//
// void AddList2();
//

//
// Rule 343:  DimExpr ::= LBRACKET Expression RBRACKET
//
//#line 3520 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 344:  Dims ::= LBRACKET RBRACKET
//
//#line 3524 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 345:  Dims ::= Dims LBRACKET RBRACKET
//
//#line 3528 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 346:  Dimsopt ::=
//
// void NullAction();
//

//
// Rule 347:  Dimsopt ::= Dims
//
// void NoAction();
//

//
// Rule 348:  SuperAccess ::= super
//
//#line 3543 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 349:  SuperAccess ::= Name DOT Marker super
//
//#line 3552 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 350:  FieldAccess ::= Primary DOT Marker Identifier
//
//#line 3560 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"
 
auto r_351 =Parser::tokens_for_recovery_templates.insert(
 Parser::tokens_for_recovery_templates.end(),make_pair(351,  vector<wstring>{L"Identifier"})); 

//
// Rule 351:  FieldAccess ::= SuperAccess DOT Marker Identifier
//
//#line 3571 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 352:  MethodInvocation ::= Identifier Arguments
//
//#line 3580 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 353:  MethodInvocation ::= Name DOT Marker Identifier Arguments
//
//#line 3589 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 354:  MethodInvocation ::= Name DOT TypeArguments Identifier Arguments
//
//#line 3595 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 355:  MethodInvocation ::= Primary DOT Marker Identifier Arguments
//
//#line 3603 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 356:  MethodInvocation ::= Primary DOT TypeArguments Identifier Arguments
//
//#line 3610 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 357:  MethodInvocation ::= SuperAccess DOT Marker Identifier Arguments
//
//#line 3621 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 358:  MethodInvocation ::= SuperAccess DOT TypeArguments Identifier Arguments
//
//#line 3633 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 359:  ArrayAccess ::= Name LBRACKET Expression RBRACKET
//
//#line 3637 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 360:  ArrayAccess ::= PrimaryNoNewArray LBRACKET Expression RBRACKET
//
//#line 3641 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 361:  ArrayAccess ::= ArrayCreationInitialized LBRACKET Expression RBRACKET
//
//#line 3649 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 362:  PostfixExpression ::= Primary
//
// void NoAction();
//

//
// Rule 363:  PostfixExpression ::= Name
//
// void NoAction();
//

//
// Rule 364:  PostfixExpression ::= PostIncrementExpression
//
// void NoAction();
//

//
// Rule 365:  PostfixExpression ::= PostDecrementExpression
//
// void NoAction();
//

//
// Rule 366:  PostfixExpressionNotName ::= Primary
//
// void NoAction();
//

//
// Rule 367:  PostfixExpressionNotName ::= PostIncrementExpression
//
// void NoAction();
//

//
// Rule 368:  PostfixExpressionNotName ::= PostDecrementExpression
//
// void NoAction();
//

//
// Rule 369:  PostIncrementExpression ::= PostfixExpression PLUS_PLUS
//
//#line 3681 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 370:  PostDecrementExpression ::= PostfixExpression MINUS_MINUS
//
//#line 3685 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 371:  UnaryExpression ::= PreIncrementExpression
//
// void NoAction();
//

//
// Rule 372:  UnaryExpression ::= PreDecrementExpression
//
// void NoAction();
//

//
// Rule 373:  UnaryExpression ::= PLUS UnaryExpression
//
//#line 3697 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 374:  UnaryExpression ::= MINUS UnaryExpression
//
// void MakePreUnaryExpression();
//

//
// Rule 375:  UnaryExpression ::= UnaryExpressionNotPlusMinus
//
// void NoAction();
//

//
// Rule 376:  UnaryExpressionNotName ::= PreIncrementExpression
//
// void NoAction();
//

//
// Rule 377:  UnaryExpressionNotName ::= PreDecrementExpression
//
// void NoAction();
//

//
// Rule 378:  UnaryExpressionNotName ::= PLUS UnaryExpression
//
// void MakePreUnaryExpression();
//

//
// Rule 379:  UnaryExpressionNotName ::= MINUS UnaryExpression
//
// void MakePreUnaryExpression();
//

//
// Rule 380:  UnaryExpressionNotName ::= UnaryExpressionNotPlusMinusNotName
//
// void NoAction();
//

//
// Rule 381:  PreIncrementExpression ::= PLUS_PLUS UnaryExpression
//
// void MakePreUnaryExpression();
//

//
// Rule 382:  PreDecrementExpression ::= MINUS_MINUS UnaryExpression
//
// void MakePreUnaryExpression();
//

//
// Rule 383:  UnaryExpressionNotPlusMinus ::= PostfixExpression
//
// void NoAction();
//

//
// Rule 384:  UnaryExpressionNotPlusMinus ::= TWIDDLE UnaryExpression
//
// void MakePreUnaryExpression();
//

//
// Rule 385:  UnaryExpressionNotPlusMinus ::= NOT UnaryExpression
//
// void MakePreUnaryExpression();
//

//
// Rule 386:  UnaryExpressionNotPlusMinus ::= CastExpression
//
// void NoAction();
//

//
// Rule 387:  UnaryExpressionNotPlusMinusNotName ::= PostfixExpressionNotName
//
// void NoAction();
//

//
// Rule 388:  UnaryExpressionNotPlusMinusNotName ::= TWIDDLE UnaryExpression
//
// void MakePreUnaryExpression();
//

//
// Rule 389:  UnaryExpressionNotPlusMinusNotName ::= NOT UnaryExpression
//
// void MakePreUnaryExpression();
//

//
// Rule 390:  UnaryExpressionNotPlusMinusNotName ::= CastExpression
//
// void NoAction();
//

//
// Rule 391:  CastExpression ::= LPAREN PrimitiveType Dimsopt RPAREN UnaryExpression
//
//#line 3784 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 392:  CastExpression ::= LPAREN Name Marker RPAREN UnaryExpressionNotPlusMinus
//
//#line 3791 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 393:  CastExpression ::= LPAREN Name Dims RPAREN UnaryExpressionNotPlusMinus
//
//#line 3795 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 394:  CastExpression ::= LPAREN Name LESS TypeArgumentList1 Dimsopt RPAREN UnaryExpressionNotPlusMinus
//
//#line 3803 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 395:  CastExpression ::= LPAREN Name LESS TypeArgumentList1 DOT ClassOrInterfaceType Dimsopt RPAREN...
//
//#line 3811 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 396:  MultiplicativeExpression ::= UnaryExpression
//
// void NoAction();
//

//
// Rule 397:  MultiplicativeExpression ::= MultiplicativeExpression MULTIPLY UnaryExpression
//
//#line 3819 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 398:  MultiplicativeExpression ::= MultiplicativeExpression DIVIDE UnaryExpression
//
// void MakeBinaryExpression();
//

//
// Rule 399:  MultiplicativeExpression ::= MultiplicativeExpression REMAINDER UnaryExpression
//
// void MakeBinaryExpression();
//

//
// Rule 400:  MultiplicativeExpressionNotName ::= UnaryExpressionNotName
//
// void NoAction();
//

//
// Rule 401:  MultiplicativeExpressionNotName ::= MultiplicativeExpressionNotName MULTIPLY UnaryExpression
//
// void MakeBinaryExpression();
//

//
// Rule 402:  MultiplicativeExpressionNotName ::= Name MULTIPLY UnaryExpression
//
// void MakeBinaryExpression();
//

//
// Rule 403:  MultiplicativeExpressionNotName ::= MultiplicativeExpressionNotName DIVIDE UnaryExpression
//
// void MakeBinaryExpression();
//

//
// Rule 404:  MultiplicativeExpressionNotName ::= Name DIVIDE UnaryExpression
//
// void MakeBinaryExpression();
//

//
// Rule 405:  MultiplicativeExpressionNotName ::= MultiplicativeExpressionNotName REMAINDER UnaryExpression
//
// void MakeBinaryExpression();
//

//
// Rule 406:  MultiplicativeExpressionNotName ::= Name REMAINDER UnaryExpression
//
// void MakeBinaryExpression();
//

//
// Rule 407:  AdditiveExpression ::= MultiplicativeExpression
//
// void NoAction();
//

//
// Rule 408:  AdditiveExpression ::= AdditiveExpression PLUS MultiplicativeExpression
//
// void MakeBinaryExpression();
//

//
// Rule 409:  AdditiveExpression ::= AdditiveExpression MINUS MultiplicativeExpression
//
// void MakeBinaryExpression();
//

//
// Rule 410:  AdditiveExpressionNotName ::= MultiplicativeExpressionNotName
//
// void NoAction();
//

//
// Rule 411:  AdditiveExpressionNotName ::= AdditiveExpressionNotName PLUS MultiplicativeExpression
//
// void MakeBinaryExpression();
//

//
// Rule 412:  AdditiveExpressionNotName ::= Name PLUS MultiplicativeExpression
//
// void MakeBinaryExpression();
//

//
// Rule 413:  AdditiveExpressionNotName ::= AdditiveExpressionNotName MINUS MultiplicativeExpression
//
// void MakeBinaryExpression();
//

//
// Rule 414:  AdditiveExpressionNotName ::= Name MINUS MultiplicativeExpression
//
// void MakeBinaryExpression();
//

//
// Rule 415:  ShiftExpression ::= AdditiveExpression
//
// void NoAction();
//

//
// Rule 416:  ShiftExpression ::= ShiftExpression LEFT_SHIFT AdditiveExpression
//
// void MakeBinaryExpression();
//

//
// Rule 417:  ShiftExpression ::= ShiftExpression RIGHT_SHIFT AdditiveExpression
//
// void MakeBinaryExpression();
//

//
// Rule 418:  ShiftExpression ::= ShiftExpression UNSIGNED_RIGHT_SHIFT AdditiveExpression
//
// void MakeBinaryExpression();
//

//
// Rule 419:  ShiftExpressionNotName ::= AdditiveExpressionNotName
//
// void NoAction();
//

//
// Rule 420:  ShiftExpressionNotName ::= ShiftExpressionNotName LEFT_SHIFT AdditiveExpression
//
// void MakeBinaryExpression();
//

//
// Rule 421:  ShiftExpressionNotName ::= Name LEFT_SHIFT AdditiveExpression
//
// void MakeBinaryExpression();
//

//
// Rule 422:  ShiftExpressionNotName ::= ShiftExpressionNotName RIGHT_SHIFT AdditiveExpression
//
// void MakeBinaryExpression();
//

//
// Rule 423:  ShiftExpressionNotName ::= Name RIGHT_SHIFT AdditiveExpression
//
// void MakeBinaryExpression();
//

//
// Rule 424:  ShiftExpressionNotName ::= ShiftExpressionNotName UNSIGNED_RIGHT_SHIFT AdditiveExpression
//
// void MakeBinaryExpression();
//

//
// Rule 425:  ShiftExpressionNotName ::= Name UNSIGNED_RIGHT_SHIFT AdditiveExpression
//
// void MakeBinaryExpression();
//

//
// Rule 426:  RelationalExpression ::= ShiftExpression
//
// void NoAction();
//

//
// Rule 427:  RelationalExpression ::= ShiftExpression LESS ShiftExpression
//
// void MakeBinaryExpression();
//

//
// Rule 428:  RelationalExpression ::= RelationalExpression GREATER ShiftExpression
//
// void MakeBinaryExpression();
//

//
// Rule 429:  RelationalExpression ::= RelationalExpression LESS_EQUAL ShiftExpression
//
// void MakeBinaryExpression();
//

//
// Rule 430:  RelationalExpression ::= RelationalExpression GREATER_EQUAL ShiftExpression
//
// void MakeBinaryExpression();
//

//
// Rule 431:  RelationalExpression ::= RelationalExpression instanceof ReferenceType
//
//#line 3966 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 432:  RelationalExpressionNotName ::= ShiftExpressionNotName
//
// void NoAction();
//

//
// Rule 433:  RelationalExpressionNotName ::= ShiftExpressionNotName LESS ShiftExpression
//
// void MakeBinaryExpression();
//

//
// Rule 434:  RelationalExpressionNotName ::= Name LESS ShiftExpression
//
// void MakeBinaryExpression();
//

//
// Rule 435:  RelationalExpressionNotName ::= ShiftExpressionNotName GREATER ShiftExpression
//
// void MakeBinaryExpression();
//

//
// Rule 436:  RelationalExpressionNotName ::= Name GREATER ShiftExpression
//
// void MakeBinaryExpression();
//

//
// Rule 437:  RelationalExpressionNotName ::= RelationalExpressionNotName LESS_EQUAL ShiftExpression
//
// void MakeBinaryExpression();
//

//
// Rule 438:  RelationalExpressionNotName ::= Name LESS_EQUAL ShiftExpression
//
// void MakeBinaryExpression();
//

//
// Rule 439:  RelationalExpressionNotName ::= RelationalExpressionNotName GREATER_EQUAL ShiftExpression
//
// void MakeBinaryExpression();
//

//
// Rule 440:  RelationalExpressionNotName ::= Name GREATER_EQUAL ShiftExpression
//
// void MakeBinaryExpression();
//

//
// Rule 441:  RelationalExpressionNotName ::= RelationalExpressionNotName instanceof ReferenceType
//
//#line 4013 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 442:  RelationalExpressionNotName ::= Name instanceof ReferenceType
//
//#line 4017 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 443:  EqualityExpression ::= RelationalExpression
//
// void NoAction();
//

//
// Rule 444:  EqualityExpression ::= EqualityExpression EQUAL_EQUAL RelationalExpression
//
// void MakeBinaryExpression();
//

//
// Rule 445:  EqualityExpression ::= EqualityExpression NOT_EQUAL RelationalExpression
//
// void MakeBinaryExpression();
//

//
// Rule 446:  EqualityExpressionNotName ::= RelationalExpressionNotName
//
// void NoAction();
//

//
// Rule 447:  EqualityExpressionNotName ::= EqualityExpressionNotName EQUAL_EQUAL RelationalExpression
//
// void MakeBinaryExpression();
//

//
// Rule 448:  EqualityExpressionNotName ::= Name EQUAL_EQUAL RelationalExpression
//
// void MakeBinaryExpression();
//

//
// Rule 449:  EqualityExpressionNotName ::= EqualityExpressionNotName NOT_EQUAL RelationalExpression
//
// void MakeBinaryExpression();
//

//
// Rule 450:  EqualityExpressionNotName ::= Name NOT_EQUAL RelationalExpression
//
// void MakeBinaryExpression();
//

//
// Rule 451:  AndExpression ::= EqualityExpression
//
// void NoAction();
//

//
// Rule 452:  AndExpression ::= AndExpression AND EqualityExpression
//
// void MakeBinaryExpression();
//

//
// Rule 453:  AndExpressionNotName ::= EqualityExpressionNotName
//
// void NoAction();
//

//
// Rule 454:  AndExpressionNotName ::= AndExpressionNotName AND EqualityExpression
//
// void MakeBinaryExpression();
//

//
// Rule 455:  AndExpressionNotName ::= Name AND EqualityExpression
//
// void MakeBinaryExpression();
//

//
// Rule 456:  ExclusiveOrExpression ::= AndExpression
//
// void NoAction();
//

//
// Rule 457:  ExclusiveOrExpression ::= ExclusiveOrExpression XOR AndExpression
//
// void MakeBinaryExpression();
//

//
// Rule 458:  ExclusiveOrExpressionNotName ::= AndExpressionNotName
//
// void NoAction();
//

//
// Rule 459:  ExclusiveOrExpressionNotName ::= ExclusiveOrExpressionNotName XOR AndExpression
//
// void MakeBinaryExpression();
//

//
// Rule 460:  ExclusiveOrExpressionNotName ::= Name XOR AndExpression
//
// void MakeBinaryExpression();
//

//
// Rule 461:  InclusiveOrExpression ::= ExclusiveOrExpression
//
// void NoAction();
//

//
// Rule 462:  InclusiveOrExpression ::= InclusiveOrExpression OR ExclusiveOrExpression
//
// void MakeBinaryExpression();
//

//
// Rule 463:  InclusiveOrExpressionNotName ::= ExclusiveOrExpressionNotName
//
// void NoAction();
//

//
// Rule 464:  InclusiveOrExpressionNotName ::= InclusiveOrExpressionNotName OR ExclusiveOrExpression
//
// void MakeBinaryExpression();
//

//
// Rule 465:  InclusiveOrExpressionNotName ::= Name OR ExclusiveOrExpression
//
// void MakeBinaryExpression();
//

//
// Rule 466:  ConditionalAndExpression ::= InclusiveOrExpression
//
// void NoAction();
//

//
// Rule 467:  ConditionalAndExpression ::= ConditionalAndExpression AND_AND InclusiveOrExpression
//
// void MakeBinaryExpression();
//

//
// Rule 468:  ConditionalAndExpressionNotName ::= InclusiveOrExpressionNotName
//
// void NoAction();
//

//
// Rule 469:  ConditionalAndExpressionNotName ::= ConditionalAndExpressionNotName AND_AND InclusiveOrExpression
//
// void MakeBinaryExpression();
//

//
// Rule 470:  ConditionalAndExpressionNotName ::= Name AND_AND InclusiveOrExpression
//
// void MakeBinaryExpression();
//

//
// Rule 471:  ConditionalOrExpression ::= ConditionalAndExpression
//
// void NoAction();
//

//
// Rule 472:  ConditionalOrExpression ::= ConditionalOrExpression OR_OR ConditionalAndExpression
//
// void MakeBinaryExpression();
//

//
// Rule 473:  ConditionalOrExpressionNotName ::= ConditionalAndExpressionNotName
//
// void NoAction();
//

//
// Rule 474:  ConditionalOrExpressionNotName ::= ConditionalOrExpressionNotName OR_OR ConditionalAndExpression
//
// void MakeBinaryExpression();
//

//
// Rule 475:  ConditionalOrExpressionNotName ::= Name OR_OR ConditionalAndExpression
//
// void MakeBinaryExpression();
//

//
// Rule 476:  ConditionalExpression ::= ConditionalOrExpression
//
// void NoAction();
//

//
// Rule 477:  ConditionalExpression ::= ConditionalOrExpression QUESTION Expression COLON ConditionalExpression
//
//#line 4165 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 478:  ConditionalExpressionNotName ::= ConditionalOrExpressionNotName
//
// void NoAction();
//

//
// Rule 479:  ConditionalExpressionNotName ::= ConditionalOrExpressionNotName QUESTION Expression COLON...
//
//#line 4174 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 480:  ConditionalExpressionNotName ::= Name QUESTION Expression COLON ConditionalExpression
//
//#line 4179 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 481:  AssignmentExpression ::= ConditionalExpression
//
// void NoAction();
//
 
auto r_482 =Parser::tokens_for_recovery_templates.insert(
 Parser::tokens_for_recovery_templates.end(),make_pair(482,  vector<wstring>{L"Identifier"})); 

//
// Rule 482:  AssignmentExpression ::= Assignment
//
//#line 4189 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 482:  AssignmentExpression ::= Assignment
//
// void NoAction();
//

//
// Rule 483:  AssignmentExpressionNotName ::= ConditionalExpressionNotName
//
// void NoAction();
//

//
// Rule 484:  AssignmentExpressionNotName ::= Assignment
//
// void NoAction();
//

//
// Rule 485:  Assignment ::= PostfixExpression AssignmentOperator AssignmentExpression
//
//#line 4209 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 486:  AssignmentOperator ::= EQUAL
//
// void NoAction();
//

//
// Rule 487:  AssignmentOperator ::= MULTIPLY_EQUAL
//
// void NoAction();
//

//
// Rule 488:  AssignmentOperator ::= DIVIDE_EQUAL
//
// void NoAction();
//

//
// Rule 489:  AssignmentOperator ::= REMAINDER_EQUAL
//
// void NoAction();
//

//
// Rule 490:  AssignmentOperator ::= PLUS_EQUAL
//
// void NoAction();
//

//
// Rule 491:  AssignmentOperator ::= MINUS_EQUAL
//
// void NoAction();
//

//
// Rule 492:  AssignmentOperator ::= LEFT_SHIFT_EQUAL
//
// void NoAction();
//

//
// Rule 493:  AssignmentOperator ::= RIGHT_SHIFT_EQUAL
//
// void NoAction();
//

//
// Rule 494:  AssignmentOperator ::= UNSIGNED_RIGHT_SHIFT_EQUAL
//
// void NoAction();
//

//
// Rule 495:  AssignmentOperator ::= AND_EQUAL
//
// void NoAction();
//

//
// Rule 496:  AssignmentOperator ::= XOR_EQUAL
//
// void NoAction();
//
 
auto r_497 =Parser::tokens_for_recovery_templates.insert(
 Parser::tokens_for_recovery_templates.end(),make_pair(497,  vector<wstring>{L"="})); 

//
// Rule 497:  AssignmentOperator ::= OR_EQUAL
//
//#line 4266 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 497:  AssignmentOperator ::= OR_EQUAL
//
// void NoAction();
//
 
auto r_498 =Parser::tokens_for_recovery_templates.insert(
 Parser::tokens_for_recovery_templates.end(),make_pair(498,  vector<wstring>{L"Identifier"})); 

//
// Rule 498:  Expression ::= AssignmentExpression
//
//#line 4272 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g" 

//
// Rule 498:  Expression ::= AssignmentExpression
//
// void NoAction();
//

//
// Rule 499:  Expressionopt ::=
//
// void NullAction();
//

//
// Rule 500:  Expressionopt ::= Expression
//
// void NoAction();
//

//
// Rule 501:  ExpressionNotName ::= AssignmentExpressionNotName
//
// void NoAction();
//

//
// Rule 502:  Marker ::=
//
// void NullAction();
//

//
// Rule 503:  ,opt ::=
//
// void NoAction();
//

//
// Rule 504:  ,opt ::= COMMA
//
// void NoAction();
//

//
// Rule 505:  Identifieropt ::=
//
// void NoAction();
//

//
// Rule 506:  Identifieropt ::= Identifier
//
// void NoAction();
//

//
// Rule 507:  PackageHeaderMarker ::=
//
//#line 4324 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 508:  MethodHeaderMarker ::=
//
//#line 4329 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 509:  TypeArguments ::= LESS TypeArgumentList1
//
//#line 4343 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 510:  TypeArgumentsopt ::=
//
// void NullAction();
//

//
// Rule 511:  TypeArgumentsopt ::= TypeArguments
//
// void NoAction();
//

//
// Rule 512:  Wildcard ::= QUESTION Marker Marker Marker
//
//#line 4370 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 513:  Wildcard ::= QUESTION extends Marker ReferenceType
//
//#line 4379 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 514:  Wildcard ::= QUESTION Marker super ReferenceType
//
//#line 4388 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 515:  Wildcard1 ::= QUESTION Marker Marker GREATER
//
//#line 4396 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 516:  Wildcard1 ::= QUESTION extends Marker ReferenceType1
//
//#line 4405 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 517:  Wildcard1 ::= QUESTION Marker super ReferenceType1
//
//#line 4414 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 518:  Wildcard2 ::= QUESTION Marker Marker RIGHT_SHIFT
//
//#line 4423 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 519:  Wildcard2 ::= QUESTION extends Marker ReferenceType2
//
//#line 4432 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 520:  Wildcard2 ::= QUESTION Marker super ReferenceType2
//
//#line 4441 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 521:  Wildcard3 ::= QUESTION Marker Marker UNSIGNED_RIGHT_SHIFT
//
//#line 4450 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 522:  Wildcard3 ::= QUESTION extends Marker ReferenceType3
//
//#line 4459 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 523:  Wildcard3 ::= QUESTION Marker super ReferenceType3
//
//#line 4468 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 524:  TypeArgumentList ::= TypeArgument
//
// void StartList();
//

//
// Rule 525:  TypeArgumentList ::= TypeArgumentList COMMA TypeArgument
//
// void AddList3();
//

//
// Rule 526:  TypeArgumentList1 ::= TypeArgument1
//
// void StartList();
//

//
// Rule 527:  TypeArgumentList1 ::= TypeArgumentList COMMA TypeArgument1
//
// void AddList3();
//

//
// Rule 528:  TypeArgumentList2 ::= TypeArgument2
//
// void StartList();
//

//
// Rule 529:  TypeArgumentList2 ::= TypeArgumentList COMMA TypeArgument2
//
// void AddList3();
//

//
// Rule 530:  TypeArgumentList3 ::= TypeArgument3
//
// void StartList();
//

//
// Rule 531:  TypeArgumentList3 ::= TypeArgumentList COMMA TypeArgument3
//
// void AddList3();
//

//
// Rule 532:  TypeArgument ::= ReferenceType
//
// void NoAction();
//

//
// Rule 533:  TypeArgument ::= Wildcard
//
// void NoAction();
//

//
// Rule 534:  TypeArgument1 ::= ReferenceType1
//
// void NoAction();
//

//
// Rule 535:  TypeArgument1 ::= Wildcard1
//
// void NoAction();
//

//
// Rule 536:  TypeArgument2 ::= ReferenceType2
//
// void NoAction();
//

//
// Rule 537:  TypeArgument2 ::= Wildcard2
//
// void NoAction();
//

//
// Rule 538:  TypeArgument3 ::= ReferenceType3
//
// void NoAction();
//

//
// Rule 539:  TypeArgument3 ::= Wildcard3
//
// void NoAction();
//

//
// Rule 540:  ReferenceType1 ::= ReferenceType GREATER
//
// void NoAction();
//

//
// Rule 541:  ReferenceType1 ::= ClassOrInterface LESS TypeArgumentList2 Marker
//
//#line 4596 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 542:  ReferenceType2 ::= ReferenceType RIGHT_SHIFT
//
// void NoAction();
//

//
// Rule 543:  ReferenceType2 ::= ClassOrInterface LESS TypeArgumentList3 Marker
//
//#line 4611 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 544:  ReferenceType3 ::= ReferenceType UNSIGNED_RIGHT_SHIFT
//
// void NoAction();
//

//
// Rule 545:  TypeParameters ::= LESS TypeParameterList1
//
//#line 4626 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 546:  TypeParametersopt ::=
//
// void NullAction();
//

//
// Rule 547:  TypeParametersopt ::= TypeParameters
//
// void NoAction();
//

//
// Rule 548:  TypeParameterList ::= TypeParameter
//
// void StartList();
//

//
// Rule 549:  TypeParameterList ::= TypeParameterList COMMA TypeParameter
//
// void AddList3();
//

//
// Rule 550:  TypeParameterList1 ::= TypeParameter1
//
// void StartList();
//

//
// Rule 551:  TypeParameterList1 ::= TypeParameterList COMMA TypeParameter1
//
// void AddList3();
//

//
// Rule 552:  TypeParameter ::= Identifier TypeBoundopt
//
//#line 4676 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 553:  TypeParameter1 ::= Identifier Marker GREATER
//
//#line 4685 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 554:  TypeParameter1 ::= Identifier TypeBound1
//
//#line 4693 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 555:  TypeBound ::= extends ReferenceType AdditionalBoundListopt
//
//#line 4701 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 556:  TypeBoundopt ::=
//
// void NullAction();
//

//
// Rule 557:  TypeBoundopt ::= TypeBound
//
// void NoAction();
//

//
// Rule 558:  TypeBound1 ::= extends ReferenceType1 Marker
//
//#line 4724 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 559:  TypeBound1 ::= extends ReferenceType AdditionalBoundList1
//
//#line 4732 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 560:  AdditionalBoundList ::= AdditionalBound
//
// void StartList();
//

//
// Rule 561:  AdditionalBoundList ::= AdditionalBoundList AdditionalBound
//
// void AddList2();
//

//
// Rule 562:  AdditionalBoundListopt ::=
//
// void NullAction();
//

//
// Rule 563:  AdditionalBoundListopt ::= AdditionalBoundList
//
// void NoAction();
//

//
// Rule 564:  AdditionalBoundList1 ::= AdditionalBound1
//
// void StartList();
//

//
// Rule 565:  AdditionalBoundList1 ::= AdditionalBoundList AdditionalBound1
//
// void AddList2();
//

//
// Rule 566:  AdditionalBound ::= AND ClassOrInterfaceType
//
//#line 4782 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 567:  AdditionalBound1 ::= AND ClassOrInterfaceType1
//
//#line 4790 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"

//
// Rule 568:  ClassOrInterfaceType1 ::= ClassOrInterfaceType GREATER
//
// void NoAction();
//

//
// Rule 569:  ClassOrInterfaceType1 ::= ClassOrInterface LESS TypeArgumentList2 Marker
//
//#line 4806 "D:\workspace\JCIDE\Debuggers\JCIDE_V3_Brave\JCDT\doc\grammer\jcdt数组版本.g"
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


