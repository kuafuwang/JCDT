#pragma  once 
#include "JCDT_Lib/internal/lex/LexStream.h"
#include "RecoveryScannerData.h"
#include <string>
#include <vector>



using std::vector;
using std::wstring;

namespace Jikes { // Open namespace Jikes block

class LookupEnvironment;

 class RecoveryScanner  {

	

	 TokenIndex curTokenIndex = 0;
	 RecoveryScannerData* data;
	 int currentPosition =0, startPosition =0;
	 vector<int> pendingTokens;
	 int pendingTokensPtr = -1;
	 wstring* fakeTokenSource = nullptr;
	 bool isInserted = true;
	 bool precededByRemoved = false;
	 int skipNextInsertedTokens = -1;
	 BaseLexStream&  lex;

	 //Jikes::Token* lexNextTokon;
	 bool record = true;
	 LookupEnvironment& lookup_env;
 public:
	 static wstring NO_CHAR;
	 static wstring FAKE_IDENTIFIER;

	explicit RecoveryScanner(BaseLexStream&  _lex,RecoveryScannerData* data, LookupEnvironment& _lookup_env)
	 :lex(_lex), lookup_env(_lookup_env){
		setData(data);
	 }

	 ~RecoveryScanner();
	 void insertToken(int token, int completedToken, int position);

	 vector<int>& reverse(vector<int>& tokens);

	 void insertTokens(vector<int>& tokens, int completedToken, int position);

	 void replaceTokens(int token, int start, TokenIndex end_relate_tokon_index);

	 void replaceTokens(vector<int>& tokens, int start, TokenIndex end_relate_tokon_index);

	 void InitBeforeGetNextToken();

	 void removeTokens(int start, TokenIndex end_relate_tokon_index) const;

	 Token* getNextToken();

	 void  ProcessInserOrReplaceLiteralToken(Token*, int tokenKid);
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
	 RecoveryScannerData* getData();

	 bool isFakeToken();

	  bool isInsertedToken();

	  bool isReplacedToken();



	  bool isPrecededByRemovedToken();

	 void setData(RecoveryScannerData* data);

	 void Reborn();
 };

 inline void RecoveryScanner::replaceTokens(int token, int start, TokenIndex end_relate_tokon_index) {
	 vector<int> tokons(1, token);
	 replaceTokens(tokons, start, end_relate_tokon_index);
 }

 inline void RecoveryScanner::setData(RecoveryScannerData* data)
 {
	 if (data == nullptr) {
		 this->data = new RecoveryScannerData();
	 }
	 else {
		 delete  this->data;
		 this->data = data;
	 }
 }

 inline void RecoveryScanner::insertToken(int token, int completedToken, int position) {
	 vector<int> tokons(1, token);
	 insertTokens(tokons, completedToken, position);
 }

 inline void RecoveryScanner::InitBeforeGetNextToken()
 {
	 lex.Reset();
 }
 inline RecoveryScannerData* RecoveryScanner::getData() {
	 return this->data;
 }

 inline bool RecoveryScanner::isFakeToken() {
	 return this->fakeTokenSource != nullptr;
 }

 inline bool RecoveryScanner::isInsertedToken() {
	 return this->fakeTokenSource != nullptr && this->isInserted;
 }

 inline bool RecoveryScanner::isReplacedToken() {
	 return this->fakeTokenSource != nullptr && !this->isInserted;
 }



 inline bool RecoveryScanner::isPrecededByRemovedToken() {
	 return this->precededByRemoved;
 }

} // Close namespace Jikes block
