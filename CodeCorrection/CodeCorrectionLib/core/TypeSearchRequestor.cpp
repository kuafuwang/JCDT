#include "stdafx.h"
#include "TypeSearchRequestor.h"
#include "SimilarElementsRequestor.h"
#include <JavaModelLib/internal/lookup/SearchableEnvironmentRequestor.h>

namespace Jikes { // Open namespace Jikes block


	namespace Correction {
		TypeSearchRequestor::TypeSearchRequestor(SimilarElementsRequestor* _requestor)
		{
			assert(_requestor);
			requestor = _requestor;
			nameLookupRequestor = new JavaModel::SearchableEnvironmentRequestor(this, nullptr);
		}

		TypeSearchRequestor::~TypeSearchRequestor()
		{

		}

		void TypeSearchRequestor::acceptClass(const std::wstring& packageName, const std::wstring& typeName, int modifiers)
		{
			requestor->acceptClass(
				packageName,
				typeName,
				typeName,
				modifiers,
				nullptr,
				0);
		}

		void TypeSearchRequestor::acceptInterface(const std::wstring& packageName, const std::wstring& typeName, int modifiers)
		{
			requestor->acceptInterface(
				packageName,
				typeName,
				typeName,
				modifiers,
				nullptr,
				0);
		}

		void TypeSearchRequestor::acceptPackage(const std::wstring& packageName)
		{
		}

		void TypeSearchRequestor::acceptType(const std::wstring& packageName, const std::wstring& typeName, int modifiers)
		{
			requestor->acceptType(
				packageName,
				typeName,
				typeName,
				nullptr,
				0);
		}
	} // Close namespace Correction block

} // Close namespace Jikes block

