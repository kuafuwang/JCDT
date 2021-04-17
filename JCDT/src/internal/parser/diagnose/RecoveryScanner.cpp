
#include "JCDT_Lib/internal/parser/diagnose/RecoveryScanner.h"
#include <JCDT_Lib/internal/env/INameEnvironment.h>
#include <JCDT_Lib/internal/parser/parser.h>
#include <JCDT_Lib/internal/lex/scanner.h>
#include "JCDT_Lib/internal/env/LookupEnvironment.h"

namespace Jikes { // Open namespace Jikes block



	wstring RecoveryScanner::NO_CHAR;
	wstring RecoveryScanner::FAKE_IDENTIFIER = L"$missingToken$"; //$NON-NLS-1$

	RecoveryScanner::~RecoveryScanner()
	{
		delete data;
	}

	vector<int>& RecoveryScanner::reverse(vector<int>& tokens)
	{
		int length = tokens.size();
		for (int i = 0, max = length / 2; i < max; i++) {
			int tmp = tokens[i];
			tokens[i] = tokens[length - i - 1];
			tokens[length - i - 1] = tmp;
		}
		return tokens;
	}

	void RecoveryScanner::insertTokens(vector<int>& tokens, int completedToken, int position)
	{
		if (!this->record) return;

		if (completedToken > -1 && Parser::statements_recovery_filter[completedToken] != 0) return;

		this->data->insertedTokensPtr+=1;
		if (this->data->insertedTokens.size() == 0) {
			this->data->insertedTokens = std::vector<std::vector<int>>(10);
			this->data->insertedTokensPosition = std::vector<int>(10);
			this->data->insertedTokenUsed = std::vector<bool>(10);
		}
		else if (this->data->insertedTokens.size() == this->data->insertedTokensPtr) {
			int length = this->data->insertedTokens.size();
			this->data->insertedTokens.insert(this->data->insertedTokens.end(), length, std::vector<int>());
			this->data->insertedTokensPosition.insert(this->data->insertedTokensPosition.end(), length, 0);
			this->data->insertedTokenUsed.insert(this->data->insertedTokenUsed.end(), length, false);
			
		}
		this->data->insertedTokens[this->data->insertedTokensPtr] = reverse(tokens);
		this->data->insertedTokensPosition[this->data->insertedTokensPtr] = position;
		this->data->insertedTokenUsed[this->data->insertedTokensPtr] = false;
	}

	void RecoveryScanner::replaceTokens(vector<int>& tokens, int start, TokenIndex end_relate_tokon_index)
	{
		if (!this->record) return;
		this->data->replacedTokensPtr+=1;
		if (this->data->replacedTokensStart.size() == 0) {
			this->data->replacedTokens = std::vector<std::vector<int>>(10);
			this->data->replacedTokensStart = std::vector<int>(10);
			this->data->replacedTokensEnd = std::vector<TokenIndex>(10);
			this->data->replacedTokenUsed = std::vector<bool>(10);
		}
		else if (this->data->replacedTokensStart.size() == this->data->replacedTokensPtr) {
			int length = this->data->replacedTokensStart.size();
			this->data->replacedTokens.insert(this->data->replacedTokens.end(), length, std::vector<int>());
			this->data->replacedTokensStart.insert(this->data->replacedTokensStart.end(), length, 0);
			this->data->replacedTokensEnd.insert(this->data->replacedTokensEnd.end(), length, 0);
			this->data->replacedTokenUsed.insert(this->data->replacedTokenUsed.end(), length, false);
		}
		this->data->replacedTokens[this->data->replacedTokensPtr] = reverse(tokens);
		this->data->replacedTokensStart[this->data->replacedTokensPtr] = start;
		this->data->replacedTokensEnd[this->data->replacedTokensPtr] = end_relate_tokon_index;
		this->data->replacedTokenUsed[this->data->replacedTokensPtr] = false;
	}

	
	void RecoveryScanner::removeTokens(int start, TokenIndex end_relate_tokon_index) const
	{
		if (!this->record) return;
		this->data->removedTokensPtr+=1;
		if (this->data->removedTokensStart.size() == 0) {
			this->data->removedTokensStart = std::vector<int>(10);
			this->data->removedTokensEnd = std::vector<TokenIndex>(10);
			this->data->removedTokenUsed = std::vector<bool>(10);
		}
		else if (this->data->removedTokensStart.size() == this->data->removedTokensPtr) {
			int length = this->data->removedTokensStart.size();

			this->data->removedTokensStart.insert(this->data->removedTokensStart.end(), length, 0);
			this->data->removedTokensEnd.insert(this->data->removedTokensEnd.end(), length, 0);
			this->data->removedTokenUsed.insert(this->data->removedTokenUsed.end(), length, false);
	
		}
		this->data->removedTokensStart[this->data->removedTokensPtr] = start;
		this->data->removedTokensEnd[this->data->removedTokensPtr] = end_relate_tokon_index;
		this->data->removedTokenUsed[this->data->removedTokensPtr] = false;
	}

	Token* RecoveryScanner::getNextToken()
	{
		Token* lexNextTokon = nullptr;
		if (this->pendingTokensPtr > -1) {
			int token_kid = this->pendingTokens[this->pendingTokensPtr--];
			lexNextTokon = lex.getNewAndUnInitToken(startPosition);
			lexNextTokon->SetVirtualType(Token::INSERT);
			if (token_kid == TK_Identifier){ 
				fakeTokenSource = &FAKE_IDENTIFIER;
				lexNextTokon->SetNameSymbol(lookup_env.nameEnv->FindOrInsertName(FAKE_IDENTIFIER.c_str(), FAKE_IDENTIFIER.size()));
			}
			else {
				ProcessInserOrReplaceLiteralToken(lexNextTokon, token_kid);
				fakeTokenSource = &NO_CHAR;
			}
			lexNextTokon->SetKind(token_kid);

			return  lexNextTokon;;
		}

		this->fakeTokenSource = nullptr;
		this->precededByRemoved = false;

		if (this->data->insertedTokens.size() != 0) {
			for (int i = 0; i <= this->data->insertedTokensPtr; i++) {
				if (this->data->insertedTokensPosition[i] == this->currentPosition - 1 && i > skipNextInsertedTokens) {
					this->data->insertedTokenUsed[i] = true;
					this->pendingTokens = this->data->insertedTokens[i];
					this->pendingTokensPtr = this->data->insertedTokens[i].size() - 1;
					this->isInserted = true;
					this->startPosition = this->currentPosition ;
					this->skipNextInsertedTokens = i;
					int token_kid = this->pendingTokens[this->pendingTokensPtr--];
					lexNextTokon = lex.getNewAndUnInitToken(startPosition);
					lexNextTokon->SetVirtualType(Token::INSERT);
					if (token_kid == TK_Identifier) {
						this->fakeTokenSource = &FAKE_IDENTIFIER;
						lexNextTokon->SetNameSymbol(lookup_env.nameEnv->FindOrInsertName(FAKE_IDENTIFIER.c_str(), FAKE_IDENTIFIER.size()));
					}
					else {
						ProcessInserOrReplaceLiteralToken(lexNextTokon, token_kid);
						this->fakeTokenSource = &NO_CHAR;
					}
					lexNextTokon->SetKind(token_kid);
					return lexNextTokon;
				}
			}
			this->skipNextInsertedTokens = -1;
		}

		int previousLocation = this->currentPosition;
		//int currentToken;
		{
			//currentToken = super.getNextToken();
			curTokenIndex   = lex.Gettoken();
			lexNextTokon = lex.getTokenObjNoIndexNoNext(curTokenIndex);
			this->startPosition = lexNextTokon->start_location;
			this->currentPosition = this->startPosition + lex.NameStringLength(curTokenIndex)  ;
		}
		

		if (this->data->replacedTokens.size() != 0) {
			for (int i = 0; i <= this->data->replacedTokensPtr; i++) {
			
				TokenIndex end_relate_tokon_index = this->data->replacedTokensEnd[i];
						 
				int real_replacedTokensEnd = lex.end(end_relate_tokon_index);
				
				if (this->data->replacedTokensStart[i] >= previousLocation &&
					this->data->replacedTokensStart[i] <= this->startPosition &&
					real_replacedTokensEnd >= this->currentPosition - 1) {
					this->data->replacedTokenUsed[i] = true;
					this->pendingTokens = this->data->replacedTokens[i];
					this->pendingTokensPtr = this->data->replacedTokens[i].size() - 1;
					this->fakeTokenSource = &FAKE_IDENTIFIER;
					this->isInserted = false;

					//this->currentPosition = this->data->replacedTokensEnd[i] + 1;
					this->currentPosition = real_replacedTokensEnd + 1;
					curTokenIndex = end_relate_tokon_index;

					int token_kid = this->pendingTokens[this->pendingTokensPtr--];
					lexNextTokon = lex.getNewAndUnInitToken(startPosition);
					lexNextTokon->SetVirtualType(Token::REPLACE);
					if (token_kid == TK_Identifier) {
						this->fakeTokenSource = &FAKE_IDENTIFIER;
						lexNextTokon->SetNameSymbol(lookup_env.nameEnv->FindOrInsertName(FAKE_IDENTIFIER.c_str(), FAKE_IDENTIFIER.size()));
					}
					else {
						ProcessInserOrReplaceLiteralToken(lexNextTokon, token_kid);
						this->fakeTokenSource = &NO_CHAR;
					}
					lexNextTokon->SetKind(token_kid);
					return lexNextTokon;
				}
			}
		}
		if (this->data->removedTokensStart.size() != 0) {
			for (int i = 0; i <= this->data->removedTokensPtr; i++) {
				
				TokenIndex end_relate_tokon_index = this->data->removedTokensEnd[i];

				int real_replacedTokensEnd = lex.end(end_relate_tokon_index);


				if (this->data->removedTokensStart[i] >= previousLocation &&
					this->data->removedTokensStart[i] <= this->startPosition &&
					real_replacedTokensEnd >= this->currentPosition - 1) {
					this->data->removedTokenUsed[i] = true;

					//this->currentPosition = this->data->removedTokensEnd[i] + 1;
					this->currentPosition = real_replacedTokensEnd + 1;
					curTokenIndex = end_relate_tokon_index;

					this->precededByRemoved = false;
					return getNextToken();
				}
			}
		}
		return lexNextTokon;
	}

	void RecoveryScanner::ProcessInserOrReplaceLiteralToken(Token* token, int tokenKid)
	{
		switch (tokenKid)
		{
		case TK_CharacterLiteral:
		case TK_IntegerLiteral:
		case TK_LongLiteral:
		{
			token ->
				SetSymbol(lookup_env.nameEnv->int_table.FindOrInsertLiteral(L"0", 1));
		}
		break;
		case TK_FloatLiteral:
		{
			token ->
				SetSymbol(lookup_env.nameEnv->float_table.FindOrInsertLiteral(L"0", 1));
		}
		break;
		case 	TK_DoubleLiteral:
		{
			token ->
				SetSymbol(lookup_env.nameEnv->double_table.FindOrInsertLiteral(L"0", 1));
		}
		break;
	
		case 	TK_StringLiteral:
		{
			token ->
				SetSymbol(lookup_env.nameEnv->string_table.FindOrInsertLiteral(L"0", 1));
		}
		break;

		default:
			break;
		}
	}


	void RecoveryScanner::Reborn()
	{
		if (data->isEmpty())
			return;
		auto last_end_token_index = lex.GetEndTokenIndex();
		lex.SetTokenNumAsEndTokenIndex();

		lex.Reset();

		unsigned _kind = 0;
		vector<Token*> token_stream;
		auto index = 0;
		token_stream.push_back(lex.getTokenObjNoIndexNoNext(index)); // Get 0th token.

		auto current_token = token_stream[index];
		current_token->SetKind(0);
		BraceStack<TokenIndex> lbrace_stack;
		Token* token;
		do {
			token = getNextToken();
			token_stream.push_back(token);
			index += 1;
			token->index = index;
			_kind = token->Kind();


			switch (_kind) {
			case  TK_LBRACE:
				lbrace_stack.push(index);
				break;
			case TK_RBRACE:
				{
					TokenIndex left_brace = lbrace_stack.top();
					if (left_brace) // This right brace is matched by a left one
					{
						token_stream[left_brace]->SetRightBrace(token_stream[index] );
						lbrace_stack.pop();
					}
				}
				break;

			//case TK_class:
			//case TK_enum:
			//case TK_interface:
			//	{
			//		//
			//		// If this is a top-level type keyword (not in braces), we keep track
			//		// of it by adding it to a list.
			//		//
			//		if (lbrace_stack.size() == 0)
			//			lex.type_index.push_back(index);
			//	}
			//	break;

			case TK_package:
				lex.package = index;
				break;
			default:

				break;
			}

		} while (_kind != TK_EOF);

		for (TokenIndex left_brace = lbrace_stack.top();
		     left_brace; left_brace = lbrace_stack.top())
		{
			token_stream[left_brace]->SetRightBrace(token_stream[index] );
			lbrace_stack.pop();
		}
		lex.RccordeBraceBalance(true);
		lex.ChangeTokenStream(token_stream);
		lex.SetEndTokenIndex(last_end_token_index);

	}
} // Open namespace Jikes block
