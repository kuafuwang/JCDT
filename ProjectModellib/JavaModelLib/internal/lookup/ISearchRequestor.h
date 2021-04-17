#pragma once

/**
 * This is the internal requestor passed to the searchable name environment
 * so as to process the multiple search results as they are discovered.
 *
 * It is used to allow the code assist engine to add some more information
 * to the raw name environment results before answering them to the UI.
 */
#include <string>
namespace Jikes { // Open namespace Jikes block


namespace JavaModel
{
	class ISearchRequestor
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
		virtual ~ISearchRequestor()
		{
		}

		virtual void acceptClass(const std::wstring& packageName, const std::wstring& typeName, int modifiers) = 0;

			/**
			 * One result of the search consists of a new interface.
			 *
			 * NOTE - All package and type names are presented in their readable form:
			 *    Package names are in the form "a.b.c".
			 *    Nested type names are in the qualified form "A.I".
			 *    The default package is represented by an empty array.
			 */
			virtual void acceptInterface(const std::wstring& packageName, const std::wstring& typeName, int modifiers) = 0;

			/**
			 * One result of the search consists of a new package.
			 *
			 * NOTE - All package names are presented in their readable form:
			 *    Package names are in the form "a.b.c".
			 *    The default package is represented by an empty array.
			 */
			virtual void acceptPackage(const std::wstring& packageName) = 0;

			/**
			 * One result of the search consists of a new type.
			 *
			 * NOTE - All package and type names are presented in their readable form:
			 *    Package names are in the form "a.b.c".
			 *    Nested type names are in the qualified form "A.M".
			 *    The default package is represented by an empty array.
			 */
			virtual void acceptType(const std::wstring& packageName, const std::wstring& typeName, int modifiers) = 0;
		};

	}
}