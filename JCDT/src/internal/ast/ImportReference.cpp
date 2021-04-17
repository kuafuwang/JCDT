
#include "JCDT_Lib/internal/ast/ImportReference.h"
#include <JCDT_Lib/internal/ast/AstName.h>
#include <list>
#include <JCDT_Lib/internal/lex/Token.h>

namespace Jikes { // Open namespace Jikes block


	ImportReference::ImportReference(AstKind k, AstTag t):Ast(k, t), name(nullptr), semicolon_token(nullptr)
	{

	}

	vector<wstring >  ImportReference::getNames() const
	{
	
		list<wstring> name_list;
		AstName* cur = name;
		do{
			name_list.push_front(cur->identifier_token->getName());
			cur = cur->base_opt;
		} while (cur);
	
		return vector<wstring >(name_list.begin(), name_list.end());
	}

	wstring ImportReference::getImportName(wchar_t separator) const
	{
		list<wstring> name_list;
		AstName* cur = name;
		while (cur)
		{
			name_list.push_front(cur->identifier_token->getName());
			cur = cur->base_opt;	
		}
		wstring ret;
		for (auto str : name_list) {
			if (ret.size())
				ret.push_back(separator);
			ret.append(str);
		}
		return ret;
		}


} // Close namespace Jikes block

