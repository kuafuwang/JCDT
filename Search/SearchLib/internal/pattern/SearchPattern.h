#ifndef  SearchPattern_JIKES_SERACH_INCLUDE
#define SearchPattern_JIKES_SERACH_INCLUDE

#include <string>
#include <SearchLib/internal/core/IJavaSearchConstants.h>
#include <vector>
#include <SearchLib/internal/core/ISearchPattern.h>
#include <set>
#include <JCDT_Lib/internal/util/stringex.h>
using std::vector;
using std::wstring;
namespace Jikes {
	
	class ConstantPool;
	class ClassFile;
	namespace JavaModel{
		class IJavaElement;
		class JarPackageFragmentRoot;
		class IPackageFragment;
		
	}
	class CPClassInfo;
	class ImportReference;
	// Open namespace Jikes block
	class BaseLexStream;
	class Ast;
	class Symbol;
	class TypeSymbol;

	namespace  PathModel {
		class PathInput;
	}

namespace Search{
	class MatchSet;
	class MatchLocator;
	class IIndexSearchRequestor;
	class IJavaSearchScope;


	class SearchPattern:public IJavaSearchConstants,public ISearchPattern
	{
	public:

		enum BinaryInfoType
		{
			CLASS_INFO,
			METHOD_INFO,
			FIELD_INFO
		};
		 int matchMode;
		 bool isCaseSensitive;
		 bool _needsResolve;

		/* match level */
		 enum  MATCH_LEVEL
		 {
			  IMPOSSIBLE_MATCH = 0,
			  POSSIBLE_MATCH = 1,
			  ACCURATE_MATCH = 2,
			  INACCURATE_MATCH = 3
		 };
	
		/* match container */
	
		static const  int COMPILATION_UNIT = 1;
		static const  int CLASS = 2;
		static const  int FIELD = 4;
		static const  int METHOD = 8;
		SearchPattern(int _matchMode, bool _isCaseSensitive);
	
		virtual std::wstring indexEntryPrefix() = 0;
		virtual  bool IsMayInTheFile(BaseLexStream*);
		/**
		* Check if the given ast node syntactically matches this pattern.
		* If it does, add it to the match set.
		*/
		virtual int matchCheck(Ast* node, MatchSet* set);


	


		/**
		* Returns the type of container of this pattern, i.e. is it in compilation unit,
		* in class declarations, field declarations, or in method declarations.
		*/
		virtual int matchContainer() = 0;
		/**
		* Finds out whether the given binary info matches this search pattern.
		* Default is to return false.
		*/
		virtual bool matchesBinary(std::pair<void*, BinaryInfoType>&  binaryInfo,
			std::pair<const ConstantPool*, CPClassInfo*>& class_data)
		{
			return false;
		}

		virtual bool matchesBinaryPackge(const std::wstring& compoundPackageName){
			return false;
		}

		virtual bool matchesBinaryClass(const std::wstring& mainTypeName){
			return false;
		}
		/**
		* Returns whether the given name matches the given pattern.
		*/
		bool matchesName(const wstring& pattern, const wstring& name);


		/**
		* Returns whether the given name matches the given pattern.
		*/
		bool matchesName(const wchar_t* pattern_data, size_t patten_len,
			const wchar_t* name_data,size_t name_len);
		/**
		* Returns whether the given type binding matches the given simple name pattern
		* and qualification pattern.
		*/
		bool matchesType(const wstring& simpleNamePattern,const wstring& qualificationPattern,
			const wstring& fullyQualifiedTypeName);
	
		/**
		* Report the match of the given import reference
		*/
		virtual void matchReportImportRef(ImportReference* importRef,
		                          Symbol* binding, JavaModel::IJavaElement* element,
		                          int accuracy, MatchSet* locator);
		/**
		* Reports the match of the given reference.
		*/
		virtual void matchReportReference(Ast* reference,JavaModel::IJavaElement* element,
			int accuracy, MatchSet* locator);




		/**
		* Initializes this search pattern so that polymorphic search can be performed.
		*/
		virtual void initializePolymorphicSearch(MatchSet* locator, IProgressMonitor*
			progressMonitor) {
			// default is to do nothing
		}

		/**
		* Finds out whether the given ast node matches this search pattern.
		* Returns IMPOSSIBLE_MATCH if it doesn't.
		* Returns POSSIBLE_MATCH if it potentially matches this search pattern
		* and it has not been reolved, and it needs to be resolved to get more information.
		* Returns ACCURATE_MATCH if it matches exactly this search pattern (ie.
		* it doesn't need to be resolved or it has already been resolved.)
		* Returns INACCURATE_MATCH if it potentially exactly this search pattern (ie.
		* it has already been resolved but resolving failed.)
		*/
		virtual int matchLevel(Ast* node, bool resolve) = 0;

		/**
		* Finds out whether the given binding matches this search pattern.
		* Returns ACCURATE_MATCH if it does.
		* Returns INACCURATE_MATCH if resolve failed but match is still possible.
		* Retunrs IMPOSSIBLE_MATCH otherwise.
		* Default is to return INACCURATE_MATCH.
		*/
		virtual int matchLevel(Symbol* binding) {
			return INACCURATE_MATCH;
		}

		/**
		* Returns whether the given reference type binding matches or is a subtype of a type
		* that matches the given simple name pattern and qualification pattern.
		* Returns ACCURATE_MATCH if it does.
		* Returns INACCURATE_MATCH if resolve fails
		* Returns IMPOSSIBLE_MATCH if it doesn't.
		*/
		int matchLevelAsSubtype(TypeSymbol* type, const wstring& simpleNamePattern,
			const wstring& qualificationPattern);

		/**
		* Returns whether one of the given declaring types is the given receiver type.
		* Returns ACCURATE_MATCH if it does.
		* Returns INACCURATE_MATCH if resolve failed.
		* Returns IMPOSSIBLE_MATCH if it doesn't.
		*/
		int matchLevelForType(const vector<wstring>& declaringTypes, TypeSymbol* receiverType);

		/**
		* Returns whether the given type binding matches the given simple name pattern
		* and qualification pattern.
		* Returns ACCURATE_MATCH if it does.
		* Returns INACCURATE_MATCH if resolve failed.
		* Returns IMPOSSIBLE_MATCH if it doesn't.
		*/
		int matchLevelForType(const wstring& simpleNamePattern, const wstring& qualificationPattern,
		                      TypeSymbol* type);



		/**
		* Add square brackets to the given simple name
		*/
		wstring toArrayName(wstring simpleName, int dimensions);
	};
}// Close namespace Search block
	

} // Close namespace Jikes block

#endif