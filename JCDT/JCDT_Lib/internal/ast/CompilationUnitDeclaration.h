#ifndef AstCompilationUnit_INCLUDED
#define AstCompilationUnit_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/impl/ReferenceContext.h"
#include "JCDT_Lib/internal/ast/AstNode.h"
#include "AstArray.h"
#include "JCDT_Lib/internal/ast/AstImportDeclaration.h"
#include "AstNodeScope.h"
namespace Jikes { // Open namespace Jikes block
	// The root node for compilation.
	class CompilationUnitDeclaration : public Ast,public ReferenceContext, public AstNodeScope
	{
	public:
		AstArray<AstImportDeclaration*>* import_declarations;
		AstArray<TypeDeclaration*>* type_declarations;


		enum ParseState
		{
			HEAD_PARSE,
			ALL_PARSE
		};
		ParseState state;
		static CompilationUnitDeclaration* CompilationUnitCast(Ast* node)
		{
			return DYNAMIC_CAST<CompilationUnitDeclaration*>
				(node->kind == COMPILATION ? node : NULL);
		}
		static CompilationUnitDeclaration* EmptyCompilationUnitCast(Ast *node)
		{
			return DYNAMIC_CAST<CompilationUnitDeclaration*>
				(node->kind == COMPILATION &&
					node->other_tag == CompilationUnitDeclaration::EMPTY_COMPILATION ? node : NULL);
		}
		bool IsEmpty();
		static CompilationUnitDeclaration* BadCompilationUnitCast(Ast* node)
		{
			return DYNAMIC_CAST<CompilationUnitDeclaration*>
				(
					(node->kind == COMPILATION &&
						node->other_tag == CompilationUnitDeclaration::BAD_COMPILATION) ? node : NULL);
		}
		virtual void abort(int abortLevel, int problem)
		{
			
		}
		virtual CompilationResult* compilationResult()
		{
			return nullptr;
		}
		virtual bool hasErrors()
		{
			return false;
		}
		virtual void tagAsHavingErrors()
		{
			 
		}

	public:
		enum CompilationTag
		{
			NONE,
			BAD_COMPILATION,
			EMPTY_COMPILATION
		};

		AstStoragePool* ast_pool;

		AstPackageDeclaration* package_declaration_opt;

		CompilationUnitDeclaration(AstStoragePool* p);

		virtual ~CompilationUnitDeclaration() {}

		void FreeAst();

		  void MarkBad()
		{
			other_tag = BAD_COMPILATION;
		}
		  void MarkEmpty() { other_tag = EMPTY_COMPILATION; }

		  AstImportDeclaration*& ImportDeclaration(unsigned i)
		{
			return (*import_declarations)[i];
		}
		  unsigned NumImportDeclarations()
		{
			return import_declarations ? import_declarations->Length() : 0;
		}
		  void AllocateImportDeclarations(unsigned estimate = 1);
		  void AddImportDeclaration(AstImportDeclaration*);

		  TypeDeclaration*& GetTypeDeclaration(unsigned i)
		{
			return (*type_declarations)[i];
		}
		  unsigned NumTypeDeclarations()
		{
			return type_declarations ? type_declarations->Length() : 0;
		}
		  void AllocateTypeDeclarations(unsigned estimate = 1);
		  void AddTypeDeclaration(TypeDeclaration*);

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream&, LexStream*);

		// special forms
		virtual void Unparse(LexStream*, const char* const directory);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;
		virtual Token* LeftToken();

		virtual Token* RightToken();
		int GetScopeKind() override {
			return COMPILATION_UNIT_SCOPE;
		}

		
	};


	inline    void CompilationUnitDeclaration::AllocateImportDeclarations(unsigned estimate)
	{
		assert(!import_declarations);
		import_declarations =
			new (ast_pool) AstArray<AstImportDeclaration*>(ast_pool, estimate);
	}

	inline    void CompilationUnitDeclaration::AddImportDeclaration(AstImportDeclaration* import_declaration)
	{
		assert(import_declarations);
		import_declarations->Next() = import_declaration;
	}

	inline    void CompilationUnitDeclaration::AllocateTypeDeclarations(unsigned estimate)
	{
		assert(!type_declarations);
		type_declarations =
			new (ast_pool) AstArray<TypeDeclaration*>(ast_pool, estimate);
	}

	inline    void CompilationUnitDeclaration::AddTypeDeclaration(TypeDeclaration* type_declaration)
	{
		assert(type_declarations);
		type_declarations->Next() = type_declaration;
	}

} // Close namespace Jikes block


#endif // AstCompilationUnit_INCLUDED
