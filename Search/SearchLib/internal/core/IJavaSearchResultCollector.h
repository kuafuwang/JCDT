#ifndef  IJavaSearchResultCollector_JIKES_SERACH_INCLUDE
#define IJavaSearchResultCollector_JIKES_SERACH_INCLUDE

/**
 * A <code>IJavaSearchResultCollector</code> collects search results from a <code>search</code>
 * query to a <code>SearchEngine</code>. Clients must implement this interface and pass
 * an instance to the <code>search(...)</code> methods. When a search starts, the <code>aboutToStart()</code>
 * method is called, then 0 or more call to <code>accept(...)</code> are done, finally the
 * <code>done()</code> method is called.
 * <p>
 * Results provided to this collector may be accurate - in this case they have an <code>EXACT_MATCH</code> accuracy -
 * or they might be potential matches only - they have a <code>POTENTIAL_MATCH</code> accuracy. This last
 * case can occur when a problem prevented the <code>SearchEngine</code> from resolving the match.
 * </p>
 * <p>
 * The order of the results is unspecified. Clients must not rely on this order to display results,
 * but they should sort these results (e.g. in a syntactical order).
 * <p>
 * The <code>IJavaSearchResultCollector</code> is also used to provide a progress monitor to the
 * <code>SearchEngine</code>.
 * </p>
 * <p>
 * Clients may implement this interface.
 * </p>
 *
 * @see SearchEngine#search
 */

#include <string>
#include <vector>

namespace Jikes {
	namespace JavaModel {
		class IJavaElement;
	}
	namespace PathModel
	{
		class MosesPath;
	}

	// Open namespace Jikes block
	class IProgressMonitor;

	namespace Search {

		class FileResultElement;

		class  IJavaSearchResultCollector {
		public:
			virtual ~IJavaSearchResultCollector()
			{
			}

			/**
			 * The search result corresponds exactly to the search pattern.
			 */
			static const int EXACT_MATCH = 0;

			/**
			 * The search result is potentially a match for the search pattern,
			 * but a problem prevented the search engine from being more accurate
			 * (typically because of the classpath was not correctly set).
			 */
			static const   int POTENTIAL_MATCH = 1;

			/**
			 * Called before the actual search starts.
			 */
			virtual void aboutToStart() = 0;
			/**
			 * Accepts the given search result.
			 *
			 * @param resource the resource in which the match has been found
			 * @param start the start position of the match, -1 if it is unknown
			 * @param end the end position of the match, -1 if it is unknown;
			 *  the ending offset is exclusive, meaning that the actual range of characters
			 *  covered is <code>[start, end]</code>
			 * @param enclosingElement the Java element that contains the character range
			 *	<code>[start, end]</code>; the value can be <code>nullptr</code> indicating that
			 *	no enclosing Java element has been found
			 * @param accuracy the level of accuracy the search result has; either
			 *  <code>EXACT_MATCH</code> or <code>POTENTIAL_MATCH</code>
			 * @exception CoreException if this collector had a problem accepting the search result
			 */
			virtual void accept(
				FileResultElement*) = 0;
			/**
			 * Called when the search has ended.
			 */
			virtual void done() = 0;
			/**
			 * Returns the progress monitor used to report progress.
			 *
			 * @return a progress monitor or nullptr if no progress monitor is provided
			 */
			virtual IProgressMonitor* getProgressMonitor() = 0;


			virtual bool IsCopyChildren() = 0;
		};
	}// Close namespace Search block


} // Close namespace Jikes block

#endif