#include "stdafx.h"
#include "FindAstNode.h"
#include "JCDT_Lib/internal/ast/ast.h"
#include "JCDT_Lib/internal/lex/LexStream.h"
namespace Jikes { // Open namespace Jikes block


	namespace CodeAssist {

			void FindAstNode::Find(CompilationUnitDeclaration* unit, LexStream* lex,unsigned int position)
			{
				if(!lex )
				{
					return;
				}
				int idx = lex->FindBadTokenIndex(position);
				if(idx != PositionNode::NO_FOUND)
				{
					return;
				}
				idx = lex->FindCommentIndex(position);
				if (idx != PositionNode::NO_FOUND)
				{
					return;
				}
				idx = lex->FindGoodTokenIndex(position);
				if (idx == PositionNode::NO_FOUND)
				{
					return;
				}
					
				Token* token = lex->getTokenObjNoIndexNoNext(idx);
				if(token->Kind() != TK_Identifier)
				{
					return;
				}	
			}
	} // Close namespace Jikes block

} // Close namespace CodeAssist block

