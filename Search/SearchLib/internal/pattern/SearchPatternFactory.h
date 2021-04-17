#ifndef  SearchPatternFactory_JIKES_SERACH_INCLUDE
#define SearchPatternFactory_JIKES_SERACH_INCLUDE
#include <JavaModelLib/internal/core/IType.h>

namespace Jikes { // Open namespace Jikes block
	namespace JavaModel
	{
		class IJavaElement;
		class IType;

	}
namespace Search{
	class ISearchPattern;
	class SearchPattern;
	class SearchPatternFactory
	{
	
	public:
		/**
		* Returns a search pattern that combines the given two patterns into a "or" pattern.
		* The search result will match either the left pattern or the right pattern.
		*
		* @param leftPattern the left pattern
		* @param rightPattern the right pattern
		* @return a "or" pattern
		*/
		static ISearchPattern* createOrSearchPattern(ISearchPattern* leftPattern, ISearchPattern* rightPattern);

		/**
		* Returns a search pattern based on a given string pattern. The string patterns support '*' wild-cards.
		* The remaining parameters are used to narrow down the type of expected results.
		*
		* <p>
		*	Examples:
		*	<ul>
		* 		<li>search for case insensitive references to <code>Object</code>:
		*			<code>createSearchPattern("Object", TYPE, REFERENCES, false);</code></li>
		*  	<li>search for case sensitive references to exact <code>Object()</code> constructor:
		*			<code>createSearchPattern("java.lang.Object()", CONSTRUCTOR, REFERENCES, true);</code></li>
		*  	<li>search for implementers of <code>java.lang.Runnable</code>:
		*			<code>createSearchPattern("java.lang.Runnable", TYPE, IMPLEMENTORS, true);</code></li>
		*  </ul>
		* @param searchFor determines the nature of the searched elements
		*	<ul>
		* 		<li><code>IJavaSearchConstants.CLASS</code>: only look for classes</li>
		*		<li><code>IJavaSearchConstants.INTERFACE</code>: only look for interfaces</li>
		* 		<li><code>IJavaSearchConstants.TYPE</code>: look for both classes and interfaces</li>
		*		<li><code>IJavaSearchConstants.FIELD</code>: look for fields</li>
		*		<li><code>IJavaSearchConstants.METHOD</code>: look for methods</li>
		*		<li><code>IJavaSearchConstants.CONSTRUCTOR</code>: look for constructors</li>
		*		<li><code>IJavaSearchConstants.PACKAGE</code>: look for packages</li>
		*	</ul>
		* @param limitTo determines the nature of the expected matches
		*	<ul>
		* 		<li><code>IJavaSearchConstants.DECLARATIONS</code>: will search declarations matching with the corresponding
		* 			element. In case the element is a method, declarations of matching methods in subtypes will also
		*  		be found, allowing to find declarations of abstract methods, etc.</li>
		*
		*		 <li><code>IJavaSearchConstants.REFERENCES</code>: will search references to the given element.</li>
		*
		*		 <li><code>IJavaSearchConstants.ALL_OCCURRENCES</code>: will search for either declarations or references as specified
		*  		above.</li>
		*
		*		 <li><code>IJavaSearchConstants.IMPLEMENTORS</code>: for interface, will find all types which implements a given interface.</li>
		*	</ul>
		*
		* @param isCaseSensitive indicates whether the search is case sensitive or not.
		* @return a search pattern on the given string pattern, or <code>nullptr</code> if the string pattern is ill-formed.
		*/
		static ISearchPattern* createSearchPattern(const wstring& stringPattern, int searchFor, int limitTo, bool isCaseSensitive);

	
	
	  /**
	  * Constructor pattern are formed by [declaringQualification.]type[(parameterTypes)]
	  * e.g. java.lang.Object()
	  *		Main(*)
	  */
		static SearchPattern* createConstructorPattern(std::wstring patternString, int limitTo, int matchMode, bool isCaseSensitive);

		/**
		* Field pattern are formed by [declaringType.]name[type]
		* e.g. java.lang.std::wstring.serialVersionUID long
		*		field*
		*/
		static SearchPattern* createFieldPattern(std::wstring patternString, int limitTo, int matchMode, bool isCaseSensitive);
		/**
		* Method pattern are formed by [declaringType.]selector[(parameterTypes)][returnType]
		* e.g. java.lang.Runnable.run() void
		*		main(*)
		*/
		static SearchPattern* createMethodPattern(std::wstring patternString, int limitTo, int matchMode, bool isCaseSensitive);

		static SearchPattern* createPackagePattern(std::wstring patternString, int limitTo, int matchMode, bool isCaseSensitive);

		static SearchPattern* createPattern(std::wstring patternString, int searchFor, int limitTo, int matchMode, bool isCaseSensitive);

		static SearchPattern* createPattern(JavaModel::IJavaElement* element, int limitTo);
		/*static std::vector<std::wstring> enclosingTypeNames(JavaModel::IType* type);*/
		static SearchPattern* createTypePattern(std::wstring simpleName, std::wstring packageName,
		                                        std::vector<std::wstring> enclosingTypeNames, int limitTo);

		/**
		* Type pattern are formed by [qualification.]type
		* e.g. java.lang.Object
		*		Runnable
		*
		*/
		static SearchPattern* createTypePattern(std::wstring patternString, int limitTo, 
			int matchMode, bool isCaseSensitive);


	private:
		SearchPatternFactory();
	};
}// Close namespace Search block
	

} // Close namespace Jikes block

#endif