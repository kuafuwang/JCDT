#pragma once

#ifndef AstListNode_INCLUDED
#define AstListNode_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstNode.h"
namespace Jikes { // Open namespace Jikes block


	//
	// The Ast list node. This is a temporary object used in constructing lists
	// while parsing the grammar; once constructed, the contents are extracted
	// and this list is reclaimed. It is circular to make insertion easy while
	// maintaining declaration order.
	//
	class AstListNode : public Ast
	{
	public:
		AstListNode* next;
		Ast* element;
		unsigned index;
		static AstListNode* ListNodeCast(Ast *node)
		{
			return DYNAMIC_CAST<AstListNode*>(node->kind == LIST_NODE ? node : NULL);
		}
		  AstListNode()
			: Ast(LIST_NODE), next(nullptr), element(nullptr), index(0)
		{
#ifdef JIKES_DEBUG
			--count; // don't count these nodes
#endif // JIKES_DEBUG
		}

		~AstListNode() {}

		//
		// These next three functions should never be called, since list nodes
		// only exist long enough to create the AST tree and then are reclaimed.
		//
		virtual Ast* Clone(AstStoragePool*) { assert(false); return NULL; }

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override{ assert(false); };
#ifdef JIKES_DEBUG
		virtual void Print(Ostream&,LexStream&) { assert(false); }
		virtual void Unparse(Ostream&, LexStream*) { assert(false); }
#endif // JIKES_DEBUG

		virtual Token* LeftToken() { return element->LeftToken(); }
		virtual Token* RightToken() { return element->RightToken(); }
	};




} // Close namespace Jikes block


#endif // AstListNode_INCLUDED

