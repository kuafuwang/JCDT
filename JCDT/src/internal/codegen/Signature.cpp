/*
JDecomqiler

Copyright (c) 2014 <Alexander Roper>

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

   3. This notice may not be removed or altered from any source
   distribution.
*/
#include "stdafx.h"
#include <JCDT_Lib/internal/codegen/Signature.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <JCDT_Lib/internal/lookup/ExpandedFieldTable.h>

using namespace std;

namespace Jikes { // Open namespace Jikes block
	
	std::wstring Signature::checkClassName(std::wstring classname)
	{
		std::replace(classname.begin(), classname.end(), '/', '.');
		size_t start_pos = classname.find(L"java.lang.");
		if (start_pos != std::string::npos)
		{
			classname.replace(start_pos, 10, std::wstring());
		}
		return classname;
	}


	std::pair<std::vector<wstring>, wstring> Signature::parseSignature(const std::string& signature)
	{
		std::vector<std::wstring> params;

		int i = 0;
		if (signature[i] == U_LEFT_PARENTHESIS)
		{
			i++;

			int next_is_array = 0;
			std::wstring type;
			while (signature[i] != ')')
			{
				if (signature[i] == '[')
				{
					next_is_array++;
				}
				else
				{
					type = parseType(signature, i);
				}

				if (type.length() > 0)
				{
				
					while (next_is_array > 0)
					{
						type += L"[]";
						next_is_array--;
					}
					params.push_back(type);
					type.clear();
				}
				
				i++;
			}
			i++; // skip the ')'
		}

		auto last_type = parseType(signature, i);
		
		return make_pair(params, last_type);
	}

	std::wstring Signature::parseType(const std::string& signature, int & i,int num_dimensions)
	{
		std::wstring tmp;
		switch (signature[i])
		{
		case U_B:
			tmp = L"byte";
			break;
		case U_C:
			tmp = L"char";
			break;
		case U_D:
			tmp = L"double";
			break;
		case U_F:
			tmp = L"float";
			break;
		case U_I:
			tmp = L"int";
			break;
		case U_J:
			tmp = L"long";
			break;
	
		case U_S:
			tmp = L"short";
			break;
		case U_Z:
			tmp = L"boolean";
			break;
		case U_V:
			if (num_dimensions)
				return L"";
			tmp = L"void";
			break;
		case 'L':
			do
			{
				i++;
				tmp += signature[i];
			} while (signature[i] != ';');
			tmp.pop_back();
			break;
		case U_T:
			assert(false && "generics not implemented yet");
		case U_NULL: // oops, already exceeded string
			i++;
			// fallthrough

		case '[':
			i++;
			tmp = parseType(signature, i);
			tmp += L"[]";
			break;
		default:
			return L"";
		}

		tmp = checkClassName(tmp);

		return tmp;
	}

	std::wstring Signature::parseType(const char* signature, int& i, int num_dimensions)
	{
		if (signature)
		{
			string temp = signature;
			return parseType(temp, i, num_dimensions);
		}
		return L"";
	}
}
