
/**
 * This interface defines the API that may be used to implement any
 * search-based tool (such as a CodeAssist, a Finder, ...).
 * It is mainly used to hide from the search tool the implementation
 * of the underlying environment and its constructions.
 */
#include <string>
using namespace Jikes::JavaModel;

namespace Jikes { // Open namespace Jikes block

	namespace JavaModel{
		class ISearchRequestor;
	}
	namespace CodeAssist {

		

		class ISearchableNameEnvironment
		{
		public:
			virtual ~ISearchableNameEnvironment()
			{
			}

			/**
			 * Find the packages that start with the given prefix.
			 * A valid prefix is a qualified name separated by periods
			 * (ex. java.util).
			 * The packages found are passed to:
			 *    ISearchRequestor.acceptPackage(char[][] packageName)
			 */
			virtual	void findPackages(const std::wstring& prefix, ISearchRequestor* requestor) = 0;

			/**
			 * Find the top-level types (classes and interfaces) that are defined
			 * in the current environment and whose name starts with the
			 * given prefix. The prefix is a qualified name separated by periods
			 * or a simple name (ex. java.util.V or V).
			 *
			 * The types found are passed to one of the following methods (if additional
			 * information is known about the types):
			 *    ISearchRequestor.acceptType(char[][] packageName, char[] typeName)
			 *    ISearchRequestor.acceptClass(char[][] packageName, char[] typeName, int modifiers)
			 *    ISearchRequestor.acceptInterface(char[][] packageName, char[] typeName, int modifiers)
			 *
			 * This method can not be used to find member types... member
			 * types are found relative to their enclosing type.
			 */
		  virtual void findTypes(const std::wstring& prefix, ISearchRequestor* requestor) = 0;
		};
	}
}