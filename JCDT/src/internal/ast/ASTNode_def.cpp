#ifndef AstNode_def_INCLUDED
#define AstNode_def_INCLUDED
#include "JCDT_Lib/internal/ast/ASTNode.h"
#include "JCDT_Lib/internal/ast/AstStoragePool.h"

namespace Jikes { // Open namespace Jikes block


	bool Ast::debug_unparse = false;

#ifdef JIKES_DEBUG
	unsigned Ast::count = 0;
#endif
	 void* Ast::operator new(size_t size, AstStoragePool* pool)
	{
		return pool->Alloc(size);
	}

	void Ast::operator delete(void* p, AstStoragePool* pool)
	{
		
	}

	
} // Close namespace Jikes block


#endif // AstNode_def_INCLUDED
