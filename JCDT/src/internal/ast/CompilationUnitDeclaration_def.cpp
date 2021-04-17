#include <JCDT_Lib/internal/ast/AstImportDeclaration.h>
#include <JCDT_Lib/internal/ast/AstAnnotationDeclaration.h>
#include <JCDT_Lib/internal/ast/AstPackageDeclaration.h>
#ifndef AstCompilationUnit_def_INCLUDED
#define AstCompilationUnit_def_INCLUDED
#include "JCDT_Lib/internal/ast/CompilationUnitDeclaration.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>

namespace Jikes { // Open namespace Jikes block


    void CompilationUnitDeclaration::Unparse(Ostream& os, LexStream* lex_stream)
    {
        unsigned i;
        if (debug_unparse)
            os << "/*CompilationUnitDeclaration:#" << id << "*/";
        os << "// " << lex_stream -> FileName() << endl;
        if (package_declaration_opt)
            package_declaration_opt -> Unparse(os, lex_stream);
        for (i = 0; i < NumImportDeclarations(); i++)
            ImportDeclaration(i) -> Unparse(os, lex_stream);
        for (i = 0; i < NumTypeDeclarations(); i++)
            GetTypeDeclaration(i) -> Unparse(os, lex_stream);
        if (debug_unparse)
            os << "/*:CompilationUnitDeclaration#" << id << "*/";
    }

	bool CompilationUnitDeclaration::IsEmpty()
	{
		if (package_declaration_opt)
			return false;
		if (NumImportDeclarations())
			return false;
		if (NumTypeDeclarations())
			return false;
		return true;
	}

	CompilationUnitDeclaration::CompilationUnitDeclaration(AstStoragePool* p)
		: Ast(COMPILATION), import_declarations(nullptr),
		type_declarations(nullptr) , 
		ast_pool(p), package_declaration_opt(nullptr), state(HEAD_PARSE)
	{}

	void CompilationUnitDeclaration::FreeAst()
	{
		delete ast_pool;
		ast_pool = nullptr;
	}

	Ast* CompilationUnitDeclaration::Clone(AstStoragePool* ast_pool)
	{
		unsigned i;
		CompilationUnitDeclaration* clone = ast_pool->GenCompilationUnit();
		clone->other_tag = other_tag;
		if (package_declaration_opt)
			clone->package_declaration_opt = (AstPackageDeclaration*)
			package_declaration_opt->Clone(ast_pool);
		clone->AllocateImportDeclarations(NumImportDeclarations());
		for (i = 0; i < NumImportDeclarations(); i++)
			clone->AddImportDeclaration((AstImportDeclaration*)
				ImportDeclaration(i)->Clone(ast_pool));
		clone->AllocateTypeDeclarations(NumTypeDeclarations());
		for (i = 0; i < NumTypeDeclarations(); i++)
			clone->AddTypeDeclaration((TypeDeclaration*)GetTypeDeclaration(i) ->
				Clone(ast_pool));
		return clone;
	}



	
void   CompilationUnitDeclaration::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{

	if (visitor->visit(this, this)) {
		unsigned i;
		if (package_declaration_opt)
			package_declaration_opt->traverse(visitor, this);
		unsigned num = NumImportDeclarations();
		for (i = 0; i < num; i++)
			(*import_declarations)[i]->traverse(visitor, this);
		num = NumTypeDeclarations();
		for (i = 0; i < num; i++)
			(*type_declarations)[i]->traverse(visitor, this);
	}

	
	visitor->endVisit(this, this);
}
void CompilationUnitDeclaration::Print(Ostream& os,LexStream& lex_stream)
{
    unsigned i;
    os << endl << "AST structure for "
            << lex_stream.FileName()
            << ':' << endl << endl
            << '#' << id << " (CompilationUnit):  #"
            << (package_declaration_opt ? package_declaration_opt -> id : 0)
            << " (";
    for (i = 0; i < NumImportDeclarations(); i++)
        os << " #" << ImportDeclaration(i) -> id;
    os << " ) (";
    for (i = 0; i < NumTypeDeclarations(); i++)
        os << " #" << GetTypeDeclaration(i) -> id;
    os << ')' << endl;

    if (package_declaration_opt)
        package_declaration_opt -> Print(os, lex_stream);
    for (i = 0; i < NumImportDeclarations(); i++)
        ImportDeclaration(i) -> Print(os, lex_stream);
    for (i = 0; i < NumTypeDeclarations(); i++)
        GetTypeDeclaration(i) -> Print(os, lex_stream);
}
 Token* CompilationUnitDeclaration::LeftToken()
{
	if (package_declaration_opt)
		return package_declaration_opt->package_token;
	if (NumImportDeclarations())
		return ImportDeclaration(0)->import_token;
	if (NumTypeDeclarations())
		return GetTypeDeclaration(0)->LeftToken();
	return 0;
}

// Special top-level form
void CompilationUnitDeclaration::Unparse(LexStream* lex_stream,
	const char* const directory)
{
	auto in_file_name = lex_stream->FileName();
	// const char* suffix = ".unparse";
	const char* suffix = "";
	char* out_file_name = strcat3(directory, in_file_name, suffix);
	// Create the directory if necessary
	SystemMkdirhierForFile(out_file_name);
	ofstream os_base(out_file_name);
	if (!os_base)
	{
		Ostream() << "Cannot open output file " << out_file_name << endl;
		::abort();
	}
	Ostream os(&os_base);
	Unparse(os, lex_stream);
	delete[] out_file_name;
}

 Token* CompilationUnitDeclaration::RightToken()
{
	if (NumTypeDeclarations())
		return GetTypeDeclaration(NumTypeDeclarations() - 1)->RightToken();
	if (NumImportDeclarations())
		return ImportDeclaration(NumImportDeclarations() - 1) ->
		semicolon_token;
	if (package_declaration_opt)
		return package_declaration_opt->semicolon_token;
	return 0;
}


} // Close namespace Jikes block


#endif // AstCompilationUnit_def_INCLUDED
