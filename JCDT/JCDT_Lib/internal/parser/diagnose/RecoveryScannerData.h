#pragma  once 
#include <vector>

class RecoveryScannerData {
 public:
	  int insertedTokensPtr = -1;
	  std::vector<std::vector<int>> insertedTokens;
	  std::vector<int> insertedTokensPosition;
	  std::vector<bool> insertedTokenUsed;

	  int replacedTokensPtr = -1;
	  std::vector<std::vector<int>> replacedTokens;
	  std::vector<int>  replacedTokensStart;
	  std::vector<TokenIndex>  replacedTokensEnd;
	  std::vector<bool> replacedTokenUsed;

	  int removedTokensPtr = -1;
	  std::vector<int> removedTokensStart;
	  std::vector<TokenIndex> removedTokensEnd;
	  std::vector<bool>  removedTokenUsed;

	  RecoveryScannerData* removeUnused() {
		if (this->insertedTokens.size()!= 0) {
			int newInsertedTokensPtr = -1;
			for (int i = 0; i <= this->insertedTokensPtr; i++) {
				if (this->insertedTokenUsed[i]) {
					newInsertedTokensPtr++;
					this->insertedTokens[newInsertedTokensPtr] = this->insertedTokens[i];
					this->insertedTokensPosition[newInsertedTokensPtr] = this->insertedTokensPosition[i];
					this->insertedTokenUsed[newInsertedTokensPtr] = this->insertedTokenUsed[i];
				}
			}
			this->insertedTokensPtr = newInsertedTokensPtr;
		}

		if (this->replacedTokens.size() != 0) {
			int newReplacedTokensPtr = -1;
			for (int i = 0; i <= this->replacedTokensPtr; i++) {
				if (this->replacedTokenUsed[i]) {
					newReplacedTokensPtr++;
					this->replacedTokens[newReplacedTokensPtr] = this->replacedTokens[i];
					this->replacedTokensStart[newReplacedTokensPtr] = this->replacedTokensStart[i];
					this->replacedTokensEnd[newReplacedTokensPtr] = this->replacedTokensEnd[i];
					this->replacedTokenUsed[newReplacedTokensPtr] = this->replacedTokenUsed[i];
				}
			}
			this->replacedTokensPtr = newReplacedTokensPtr;
		}
		if (this->removedTokensStart.size() != 0) {
			int newRemovedTokensPtr = -1;
			for (int i = 0; i <= this->removedTokensPtr; i++) {
				if (this->removedTokenUsed[i]) {
					newRemovedTokensPtr++;
					this->removedTokensStart[newRemovedTokensPtr] = this->removedTokensStart[i];
					this->removedTokensEnd[newRemovedTokensPtr] = this->removedTokensEnd[i];
					this->removedTokenUsed[newRemovedTokensPtr] = this->removedTokenUsed[i];
				}
			}
			this->removedTokensPtr = newRemovedTokensPtr;
		}

		return this;
	}
	bool isEmpty()const
	{
		return removedTokensStart.size() && replacedTokens.size() && insertedTokens.size();
	}
 };



