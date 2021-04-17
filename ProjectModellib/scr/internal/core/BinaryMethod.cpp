#include "stdafx.h"
#include <JavaModelLib/internal/core/PackageFragment.h>
#include  <JCDT_Lib/internal/codegen/Signature.h>
#include <JavaModelLib/internal/core/BinaryMethod.h>
#include <JavaModelLib/internal/core/IType.h>
#include <JCDT_Lib/internal/codegen/code.h>
#include <JCDT_Lib/internal/util/CharOperation.h>


namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	

	
	IJavaElement* BinaryMethod::ShallowCopy(IJavaElement* parent)
	{
		auto clone = new BinaryMethod(*this);
		clone->fParent = parent;
		return clone;
	}

	BinaryMethod::BinaryMethod(IType* parent, 
		const wstring& name_symbol_, const vector<wstring>& parameterTypes)
		: BaseMethod(parent, name_symbol_, parameterTypes)

	{

	}

	

	BinaryMethod::BinaryMethod(BinaryMethod&o): BaseMethod(o)
	{
	}

	void RetrieveNestedTypes(vector<wstring>&compudeNames, const wchar_t* signature)
	{
		int len;
		for (len = 0;
			signature[len] != U_NULL && signature[len] != U_DOLLAR; len++)
			;

		compudeNames.push_back(wstring(signature, signature + len));
		if (signature[len] == U_DOLLAR)
			RetrieveNestedTypes(compudeNames, &signature[len + 1]);

	}

	

	vector<wstring> ReadTypeFromSignature(const char* utf8_signature, int length)
	{
		vector<wstring> compudeNames;
		wchar_t* signature = new wchar_t[length + 1];
		int convert_len = CharOperation::ConvertUtf8ToUnicode(signature, utf8_signature, length);

		int total_length;
		for (total_length = 0;
		signature[total_length] != U_NULL &&
			signature[total_length] != U_DOLLAR; total_length++)
		{

		}

		if (length == total_length) {
			compudeNames.push_back(wstring(signature, signature + convert_len));
			return compudeNames;
		}


		if (signature[total_length] != U_NULL &&
			Code::IsDecimalDigit(signature[total_length + 1]))
		{
			// an anonymous or a local type?
			for (total_length += 2;
			Code::IsDecimalDigit(signature[total_length]); total_length++)
				// will stop at next '$' or '\0' !!!
				;
			if (signature[total_length] != U_NULL)
			{
				// not an anonymous type? then scan local type name
				for (total_length++;
				signature[total_length] != U_NULL &&
					signature[total_length] != U_DOLLAR; total_length++)
					;
			}
		}

		int len;
		for (len = total_length - 1;
		len >= 0 && signature[len] != U_SLASH; len--)
			;

		wchar_t* name = &(signature[len + 1]);



		compudeNames.push_back(wstring(name, name + total_length - (len + 1)));

		if (signature[total_length] == U_DOLLAR)
			RetrieveNestedTypes(compudeNames, &signature[total_length + 1]);

		delete[] signature;

		return compudeNames;
	}

	//inline void BinaryMethod::AddThrowsSignature(const char* signature_, unsigned length)
	//{
	//	char* signature = new char[length + 1];
	//	strncpy(signature, signature_, length);
	//	signature[length] = U_NULL;

	//	if (!throws_signatures)
	//		throws_signatures = new Tuple<char*>(8);
	//	throws_signatures->Next() = signature;
	//}

	//string signature;


	//void BinaryMethod::ProcessMethodSignature()
	//{

	//	auto  _ret = Signature::parseSignature(signature);
	//	formal_parameters = _ret.first;
	//	return_type = _ret.second;
	//}


	//void BinaryMethod::ProcessMethodThrows(Tuple<char*>* throws_signatures)
	//{
	//	if (throws_signatures) {
	//		// Process throws clause
	//		for (unsigned i = 0; i < throws_signatures->Length(); i++)
	//		{
	//			wstring temp;
	//			auto _throw = (*throws_signatures)[i];
	//			auto ret = ReadTypeFromSignature(_throw, strlen(_throw));

	//			for (size_t k = 0; k < ret.size(); ++k)
	//			{
	//				temp += (ret[k]);
	//				temp += L".";
	//			}
	//			m_throws.push_back(temp);
	//			delete[]_throw;

	//		}

	//		delete throws_signatures;
	//		throws_signatures = NULL;
	//	}
	//}



	
	BinaryMethod::~BinaryMethod()
	{
	
	}

}// Close namespace JavaModel block
	

} // Close namespace Jikes block




