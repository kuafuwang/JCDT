#pragma once

#include <string>
#include <JavaModelLib/internal/lookup/ISearchRequestor.h>

namespace Jikes { // Open namespace Jikes block
	namespace JavaModel
	{
		class IJavaElementRequestor;
	}

namespace Correction
{
	class SimilarElementsRequestor;

	class TypeSearchRequestor : public JavaModel::ISearchRequestor
{
		/**
			* One result of the search consists of a new class.
			*
			* NOTE - All package and type names are presented in their readable form:
			*    Package names are in the form "a.b.c".
			*    Nested type names are in the qualified form "A.M".
			*    The default package is represented by an empty array.
			*/
		public:
			TypeSearchRequestor(SimilarElementsRequestor*  _requestor);
		virtual ~TypeSearchRequestor();

		virtual void acceptClass(const std::wstring& packageName, const std::wstring& typeName, int modifiers);

			/**
			 * One result of the search consists of a new interface.
			 *
			 * NOTE - All package and type names are presented in their readable form:
			 *    Package names are in the form "a.b.c".
			 *    Nested type names are in the qualified form "A.I".
			 *    The default package is represented by an empty array.
			 */
		virtual void acceptInterface(const std::wstring& packageName, const std::wstring& typeName, int modifiers);

			/**
			 * One result of the search consists of a new package.
			 *
			 * NOTE - All package names are presented in their readable form:
			 *    Package names are in the form "a.b.c".
			 *    The default package is represented by an empty array.
			 */
		virtual void acceptPackage(const std::wstring& packageName);

			/**
			 * One result of the search consists of a new type.
			 *
			 * NOTE - All package and type names are presented in their readable form:
			 *    Package names are in the form "a.b.c".
			 *    Nested type names are in the qualified form "A.M".
			 *    The default package is represented by an empty array.
			 */
		virtual void acceptType(const std::wstring& packageName, const std::wstring& typeName, int modifiers);


		SimilarElementsRequestor*  requestor;
		JavaModel::IJavaElementRequestor* nameLookupRequestor;

		};

	}
}
