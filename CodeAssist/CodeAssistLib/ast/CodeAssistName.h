#ifndef Jikes_CodeAssist_AssistAstName_INCLUDED
#define Jikes_CodeAssist_AssistAstName_INCLUDED

#include "AssistExpression.h"
#include <JCDT_Lib/internal/lex/Token.h>
#include <string>
#include <vector>

namespace Jikes { // Open namespace Jikes block

 namespace CodeAssist{
				  //
				  // Simple and qualified names.
				  //
	class  AssistAstName : public AssistExpression
	{
	public:
		AssistAstName* base_opt = nullptr;
		wstring identifier_token;
		 AssistAstName()
			: AssistExpression(NAME)
		{}
		~AssistAstName() {}

	   static	std::wstring  GetCoumpoundName(AssistAstName* nameAst, wchar_t separator)
		 {
		   std::wstring _ret;
			auto cur = nameAst;
			int counter = 0;
			while(cur){
				if(!counter++)
					_ret = cur->identifier_token + _ret;
				else{
					auto temp = cur->identifier_token;
					temp.push_back(separator);
					temp += _ret;
					_ret.swap(temp);
				}
				cur = cur->base_opt;
			}
			return _ret;
		 };
	   static void  GetAllNames(AssistAstName* nameAst, std::vector< std::wstring >& names)
	   {
		   std::wstring _ret;
		   auto cur = nameAst;
		   int counter = 0;
		   while (cur) {
			   counter++;
			   names.push_back(cur->identifier_token);
			   cur = cur->base_opt;
		   }
		   std::reverse(names.begin(),names.end());
	   };
	};


 }

} // Close namespace Jikes block


#endif // Jikes_CodeAssist_AssistAstName_INCLUDED
