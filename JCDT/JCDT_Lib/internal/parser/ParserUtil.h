#pragma  once 
namespace Jikes {
	class LookupEnvironment;
	class ParseError;
	class BaseLexStream;
	class CompilationUnitDeclaration;
	class Parser;
	class TypeDeclaration;
	class AstClassBody;
class ParserUtil
{
public:
	static void DiagnoseAndCure(BaseLexStream* lex_stream,
		ParseError* error,
		bool record_error, LookupEnvironment& lookup_env);

	static CompilationUnitDeclaration* CompleteParser(BaseLexStream* lex_stream,
		ParseError* error,
		bool record_error, LookupEnvironment& lookup_env, Parser* parser);

	static	void ProcessNestedTypeName(BaseLexStream*, TypeDeclaration* declaration, Parser* parser);
	static	void CheckNestedMembers(BaseLexStream*, AstClassBody* class_body, Parser* parser);

};

}
