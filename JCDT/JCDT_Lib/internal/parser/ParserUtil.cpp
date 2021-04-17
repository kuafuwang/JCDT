#include "ParserUtil.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include "diagnose/diagnose.h"
#include <JCDT_Lib/internal/ast/AstAnonymousTypeDeclaration.h>
#include <JCDT_Lib/internal/ast/AstClassBody.h>
#include <JCDT_Lib/internal/ast/AstEmptyDeclaration.h>
#include <JCDT_Lib/internal/ast/CompilationUnitDeclaration.h>
#include <JCDT_Lib/internal/ast/AstClassDeclaration.h>
#include <JCDT_Lib/internal/ast/AstEnumDeclaration.h>
#include <JCDT_Lib/internal/ast/AstAnnotationDeclaration.h>
#include <JCDT_Lib/internal/ast/AstInterfaceDeclaration.h>

namespace Jikes { // Open namespace Jikes block


	void ParserUtil::DiagnoseAndCure(BaseLexStream* lex_stream, ParseError* error, 
		bool record_error, LookupEnvironment& lookup_env)
	{
		auto end_token_index = LexStream::LEX_INFINITY;
		auto start_token_index = LexStream::LEX_START;
		auto dianose = new DiagnoseParser(lex_stream, TK_CompilationUnitMarker,
			start_token_index, end_token_index, error, lookup_env);
		dianose->diagnoseParse(record_error);
		delete dianose;
	}

	CompilationUnitDeclaration* ParserUtil::CompleteParser(
		BaseLexStream* lex_stream, ParseError* error,
		bool record_error, LookupEnvironment& lookup_env,
		Parser* parser)
	{
		ParserUtil::DiagnoseAndCure(lex_stream,
			error, record_error, lookup_env);

	auto	compilation_unit =
			parser->HeaderParse(lex_stream, nullptr);
	if (!compilation_unit)
		return nullptr;

	for (unsigned k = 0; k < compilation_unit->NumTypeDeclarations(); k++) {
		TypeDeclaration* declaration = compilation_unit->GetTypeDeclaration(k);

		if (!EmptyTypeDeclaration::EmptyDeclarationCast(declaration)) {
			CheckNestedMembers(lex_stream, 
				declaration->class_body,
				parser);
		}
	}
	compilation_unit->state = CompilationUnitDeclaration::ALL_PARSE;

	return compilation_unit;

	}


	void ParserUtil::ProcessNestedTypeName(BaseLexStream* lex_stream,
		TypeDeclaration* declaration, Parser* parser)
	{
		AstClassBody* class_body = declaration->class_body;
		CheckNestedMembers(lex_stream, class_body, parser);
	}

	void ParserUtil::CheckNestedMembers(BaseLexStream* lex_stream,
		AstClassBody* class_body, Parser* parser)
	{
		unsigned i;
		for (i = 0; i < class_body->NumNestedClasses(); i++)
		{
			ClassTypeDeclaration* decl = class_body->NestedClass(i);
			ProcessNestedTypeName(lex_stream, decl, parser);
		}
		for (i = 0; i < class_body->NumNestedEnums(); i++)
		{
			EnumTypeDeclaration* decl = class_body->NestedEnum(i);
			ProcessNestedTypeName(lex_stream, decl, parser);
		}
		for (i = 0; i < class_body->NumNestedInterfaces(); i++)
		{
			InterfaceTypeDeclaration* decl = class_body->NestedInterface(i);
			ProcessNestedTypeName(lex_stream, decl, parser);
		}
		for (i = 0; i < class_body->NumNestedAnnotations(); i++)
		{
			AnnotationTypeDeclaration* decl = class_body->NestedAnnotation(i);
			ProcessNestedTypeName(lex_stream, decl, parser);
		}

		if (!parser->InitializerParse(lex_stream, class_body)) {
			//source_file_symbol->compilation_unit->MarkBad();
			//class_body->initializerParseBad = true;
		}

		if (!parser->BodyParse(lex_stream, class_body)) {
			//source_file_symbol->compilation_unit->MarkBad();
			//class_body->bodyParseBad = true;
		}
	}

} // Close namespace Jikes block



