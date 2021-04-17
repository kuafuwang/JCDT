#pragma  once 


#include "stream.h"
#include "RecoveryScannerData.h"
#include <string>
#include <vector>
#include "parser.h"
using std::vector;
using std::wstring;
#ifdef HAVE_JIKES_NAMESPACE
namespace Jikes { // Open namespace Jikes block

wstring NO_CHAR;
 class RecoveryScanner  {
	 static wstring FAKE_IDENTIFIER;
	 TokenIndex curTokenIndex = 0;
	 RecoveryScannerData* data;
	 int currentPosition, startPosition;
	 vector<int> pendingTokens;
	 int pendingTokensPtr = -1;
	 wstring* fakeTokenSource = nullptr;
	 bool isInserted = true;
	 bool precededByRemoved = false;
	 int skipNextInsertedTokens = -1;
	 Jikes::LexStream*  lex = nullptr;
	 bool record = true;

	 RecoveryScanner(Jikes::LexStream*  lex,RecoveryScannerData* data) {
		 this->lex = lex;
		this->setData(data);
	}

	 void insertToken(int token, int completedToken, int position) {
		 vector<int> tokons(1, token);
		insertTokens(tokons, completedToken, position);
	}

	 vector<int>& reverse(vector<int>& tokens) {
		int length = tokens.size();
		for (int i = 0, max = length / 2; i < max; i++) {
			int tmp = tokens[i];
			tokens[i] = tokens[length - i - 1];
			tokens[length - i - 1] = tmp;
		}
		return tokens;
	}
	 void insertTokens(vector<int>& tokens, int completedToken, int position) {
		if (!this->record) return;

		//if (completedToken > -1 && Jikes::Parser::statements_recovery_filter[completedToken] != 0) return;

		this->data->insertedTokensPtr++;
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

	 void replaceTokens(int token, int start, int end) {
		 vector<int> tokons(1, token);
		replaceTokens(tokons, start, end);
	}

	 void replaceTokens(vector<int>& tokens, int start, int end) {
		if (!this->record) return;
		this->data->replacedTokensPtr++;
		if (this->data->replacedTokensStart.size() == 0) {
			this->data->replacedTokens = std::vector<std::vector<int>>(10);
			this->data->replacedTokensStart = std::vector<int>(10);
			this->data->replacedTokensEnd = std::vector<int>(10);
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
		this->data->replacedTokensEnd[this->data->replacedTokensPtr] = end;
		this->data->replacedTokenUsed[this->data->replacedTokensPtr] = false;
	}

	 void removeTokens(int start, int end) {
		if (!this->record) return;
		this->data->removedTokensPtr++;
		if (this->data->removedTokensStart.size() == 0) {
			this->data->removedTokensStart = std::vector<int>(10);
			this->data->removedTokensEnd = std::vector<int>(10);
			this->data->removedTokenUsed = std::vector<bool>(10);
		}
		else if (this->data->removedTokensStart.size() == this->data->removedTokensPtr) {
			int length = this->data->removedTokensStart.size();

			this->data->removedTokensStart.insert(this->data->removedTokensStart.end(), length, 0);
			this->data->removedTokensEnd.insert(this->data->removedTokensEnd.end(), length, 0);
			this->data->removedTokenUsed.insert(this->data->removedTokenUsed.end(), length, false);
	
		}
		this->data->removedTokensStart[this->data->removedTokensPtr] = start;
		this->data->removedTokensEnd[this->data->removedTokensPtr] = end;
		this->data->removedTokenUsed[this->data->removedTokensPtr] = false;
	}

	 int getNextToken()   {
		if (this->pendingTokensPtr > -1) {
			int nextToken = this->pendingTokens[this->pendingTokensPtr--];
			if (nextToken == Jikes::TK_Identifier){ 
				this->fakeTokenSource = &FAKE_IDENTIFIER;
			}
			else {
				this->fakeTokenSource = &NO_CHAR;
			}
			return nextToken;
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
					this->startPosition = this->currentPosition;
					this->skipNextInsertedTokens = i;
					int nextToken = this->pendingTokens[this->pendingTokensPtr--];
					if (nextToken == Jikes::TK_Identifier) {
						this->fakeTokenSource = &FAKE_IDENTIFIER;
					}
					else {
						this->fakeTokenSource = &NO_CHAR;
					}
					return nextToken;
				}
			}
			this->skipNextInsertedTokens = -1;
		}

		int previousLocation = this->currentPosition;
		int currentToken;
		{
			//currentToken = super.getNextToken();
			curTokenIndex   = lex->Gettoken();
			this->startPosition = lex->Location(curTokenIndex);
			this->currentPosition = lex->NameStringLength(curTokenIndex);
			currentToken = lex->Kind(curTokenIndex);
		}
		

		if (this->data->replacedTokens.size() != 0) {
			for (int i = 0; i <= this->data->replacedTokensPtr; i++) {
				if (this->data->replacedTokensStart[i] >= previousLocation &&
					this->data->replacedTokensStart[i] <= this->startPosition &&
					this->data->replacedTokensEnd[i] >= this->currentPosition - 1) {
					this->data->replacedTokenUsed[i] = true;
					this->pendingTokens = this->data->replacedTokens[i];
					this->pendingTokensPtr = this->data->replacedTokens[i].size() - 1;
					this->fakeTokenSource = &FAKE_IDENTIFIER;
					this->isInserted = false;

					this->currentPosition = this->data->replacedTokensEnd[i] + 1;
					int nextToken = this->pendingTokens[this->pendingTokensPtr--];
					if (nextToken == Jikes::TK_Identifier) {
						this->fakeTokenSource = &FAKE_IDENTIFIER;
					}
					else {
						this->fakeTokenSource = &NO_CHAR;
					}
					return nextToken;
				}
			}
		}
		if (this->data->removedTokensStart.size() != 0) {
			for (int i = 0; i <= this->data->removedTokensPtr; i++) {
				if (this->data->removedTokensStart[i] >= previousLocation &&
					this->data->removedTokensStart[i] <= this->startPosition &&
					this->data->removedTokensEnd[i] >= this->currentPosition - 1) {
					this->data->removedTokenUsed[i] = true;
					this->currentPosition = this->data->removedTokensEnd[i] + 1;
					this->precededByRemoved = false;
					return getNextToken();
				}
			}
		}
		return currentToken;
	}

	/* wstring getCurrentIdentifierSource() {
		if (this->fakeTokenSource != nullptr) 
			return wstring(*this->fakeTokenSource);
		return super.getCurrentIdentifierSource();
	}

	wstring getCurrentTokenSourceString() {
		if (this->fakeTokenSource != nullptr)
			return wstring(*this->fakeTokenSource);
		return super.getCurrentTokenSourceString();
	}

	 wstring getCurrentTokenSource() {
		 if (this->fakeTokenSource != nullptr)
			 return wstring(*this->fakeTokenSource);
		return super.getCurrentTokenSource();
	}
*/
	 RecoveryScannerData* getData() {
		return this->data;
	}

	  bool isFakeToken() {
		 return this->fakeTokenSource != nullptr;
	 }

	  bool isInsertedToken() {
		 return this->fakeTokenSource != nullptr && this->isInserted;
	 }

	  bool isReplacedToken() {
		 return this->fakeTokenSource != nullptr && !this->isInserted;
	 }



	 bool isPrecededByRemovedToken() {
		return this->precededByRemoved;
	}

	 void setData(RecoveryScannerData* data) {
		if (data == nullptr) {
			this->data = new RecoveryScannerData();
		}
		else {
			this->data = data;
		}
	}
};

 wstring RecoveryScanner::FAKE_IDENTIFIER = L"$missing$"; //$NON-NLS-1$
#ifdef HAVE_JIKES_NAMESPACE
} // Close namespace Jikes block
