#ifndef  SearchEngine_JIKES_SERACH_INCLUDE
#define SearchEngine_JIKES_SERACH_INCLUDE
#include <string>

namespace Jikes{
	namespace JavaModel{
		class JavaModel;
		class IJavaElement;
	}
}

namespace Jikes { // Open namespace Jikes block

namespace Search{
	class IJavaSearchResultCollector;
	class IJavaSearchScope;
	class ISearchPattern;

	enum IInfoConstants
	{
		/* granularity of search results */
		NameInfo = 1,
		PathInfo = 2,
		PositionInfo = 4,
		DeclarationInfo = 8
	};


	class SearchEngine
	{
	public:
		 SearchEngine() {
		}
		 /**
		 * Searches for the Java element determined by the given signature. The signature
		 * can be incomplete. For example, a call like
		 * <code>search(ws, "run()", METHOD,REFERENCES, col)</code>
		 * searches for all references to the method <code>run</code>.
		 *
		 * Note that by default the pattern will be case insensitive. For specifying case s
		 * sensitive search, use <code>search(workspace, createSearchPattern(patternString, searchFor, limitTo, true), scope, resultCollector);</code>
		 *
		 * @param workspace the workspace
		 * @param pattern the pattern to be searched for
		 * @param searchFor a hint what kind of Java element the string pattern represents.
		 *  Look into <code>IJavaSearchConstants</code> for valid values
		 * @param limitTo one of the following values:
		 *	<ul>
		 *	  <li><code>IJavaSearchConstants.DECLARATIONS</code>: search
		 *		  for declarations only </li>
		 *	  <li><code>IJavaSearchConstants.REFERENCES</code>: search
		 *		  for all references </li>
		 *	  <li><code>IJavaSearchConstants.ALL_OCCURENCES</code>: search
		 *		  for both declarations and all references </li>
		 *	  <li><code>IJavaSearchConstants.IMPLEMENTORS</code>: search for
		 *		  all implementors of an interface; the value is only valid if
		 *		  the Java element represents an interface
		 * 	</ul>
		 * @param scope the search result has to be limited to the given scope
		 * @param resultCollector a callback object to which each match is reported
		 * @exception JavaModelException if the search failed. Reasons include:
		 *	<ul>
		 *		<li>the classpath is incorrectly set
		 *	</ul>
		 */
		void search(JavaModel::JavaModel* workspace, const std::wstring& patternString, int searchFor,
		            int limitTo, IJavaSearchScope* scope, IJavaSearchResultCollector* resultCollector);

		/**
		* Searches for the given Java element.
		*
		* @param workspace the workspace
		* @param element the Java element to be searched for
		* @param limitTo one of the following values:
		*	<ul>
		*	  <li><code>IJavaSearchConstants.DECLARATIONS</code>: search
		*		  for declarations only </li>
		*	  <li><code>IJavaSearchConstants.REFERENCES</code>: search
		*		  for all references </li>
		*	  <li><code>IJavaSearchConstants.ALL_OCCURENCES</code>: search
		*		  for both declarations and all references </li>
		*	  <li><code>IJavaSearchConstants.IMPLEMENTORS</code>: search for
		*		  all implementors of an interface; the value is only valid if
		*		  the Java element represents an interface
		* 	</ul>
		* @param scope the search result has to be limited to the given scope
		* @param resultCollector a callback object to which each match is reported
		* @exception JavaModelException if the search failed. Reasons include:
		*	<ul>
		*		<li>the element doesn't exist
		*		<li>the classpath is incorrectly set
		*	</ul>
		*/
		void search(JavaModel::JavaModel* workspace, JavaModel::IJavaElement* element,
		            int limitTo, IJavaSearchScope* scope,
		            IJavaSearchResultCollector* resultCollector);

		/**
		* Searches for matches of a given search pattern. Search patterns can be created using helper
		* methods (from a const wstring& pattern or a Java element) and encapsulate the description of what is
		* being searched (e.g. search method declarations in a case sensitive way).
		*
		* @param workspace the workspace
		* @param searchPattern the pattern to be searched for
		* @param scope the search result has to be limited to the given scope
		* @param resultCollector a callback object to which each match is reported
		* @exception JavaModelException if the search failed. Reasons include:
		*	<ul>
		*		<li>the classpath is incorrectly set
		*	</ul>
		*/
		void search(JavaModel::JavaModel* workspace, ISearchPattern* searchPattern, IJavaSearchScope* scope,
		            IJavaSearchResultCollector* resultCollector);
		/**
		* Searches for all top-level types and member types in the given scope.
		* The search can be selecting specific types (given a package or a type name
		* prefix and match modes).
		*
		* @param workspace the workspace to search in
		* @param packageName the full name of the package of the searched types, or a prefix for this
		*						package, or a wild-carded string for this package.
		* @param typeName the dot-separated qualified name of the searched type (the qualification include
		*					the enclosing types if the searched type is a member type), or a prefix
		*					for this type, or a wild-carded string for this type.
		* @param matchMode one of
		* <ul>
		*		<li><code>IJavaSearchConstants.EXACT_MATCH</code> if the package name and type name are the full names
		*			of the searched types.
		*		<li><code>IJavaSearchConstants.PREFIX_MATCH</code> if the package name and type name are prefixes of the names
		*			of the searched types.
		*		<li><code>IJavaSearchConstants.PATTERN_MATCH</code> if the package name and type name contain wild-cards.
		* </ul>
		* @param isCaseSensitive whether the search should be case sensitive
		* @param searchFor one of
		* <ul>
		* 		<li><code>IJavaSearchConstants.CLASS</code> if searching for classes only
		* 		<li><code>IJavaSearchConstants.INTERFACE</code> if searching for interfaces only
		* 		<li><code>IJavaSearchConstants.TYPE</code> if searching for both classes and interfaces
		* </ul>
		* @param scope the scope to search in
		* @param nameRequestor the requestor that collects the results of the search
		* @param waitingPolicy one of
		* <ul>
		*		<li><code>IJavaSearchConstants.FORCE_IMMEDIATE_SEARCH</code> if the search should start immediately
		*		<li><code>IJavaSearchConstants.CANCEL_IF_NOT_READY_TO_SEARCH</code> if the search should be cancelled if the
		*			underlying indexer has not finished indexing the workspace
		*		<li><code>IJavaSearchConstants.WAIT_UNTIL_READY_TO_SEARCH</code> if the search should wait for the
		*			underlying indexer to finish indexing the workspace
		* </ul>
		* @param progressMonitor the progress monitor to report progress to, or <code>nullptr</code> if no progress
		*							monitor is provided
		* @exception JavaModelException if the search failed. Reasons include:
		*	<ul>
		*		<li>the classpath is incorrectly set
		*	</ul>
		*/
	};
}// Close namespace Search block
	

} // Close namespace Jikes block

#endif