#ifndef  MatchLocator_JIKES_SERACH_INCLUDE
#define MatchLocator_JIKES_SERACH_INCLUDE
#include <string>

#include <JCDT_Lib/compiler/Compiler.h>
#include <JavaModelLib/internal/core/IMethod.h>
class SolutionModel;

namespace Jikes {
	class IProgressMonitor;

	// Open namespace Jikes block
	namespace JavaModel
	{
		class IMember;
		class IJavaElement;
		class NameLookup;
		class JavaModel;
		class ClassFile;
		class IType;
		class ISourceRange;
	}
	namespace PathModel
	{
		class MosesPath;
	}
namespace Search{
	class SearchPattern;
	class IJavaSearchScope;
	class IJavaSearchResultCollector;
	class MatchLocator
	{
	public:
		 SearchPattern* pattern;
		 int detailLevel;
		 IJavaSearchResultCollector* collector;
		 IJavaSearchScope* scope;

		static std::wstring EMPTY_FILE_NAME;

		MatchLocator(
			SearchPattern* _pattern,
			int _detailLevel,
			IJavaSearchResultCollector* _collector,
			IJavaSearchScope* _scope);
		void locateMatchesInClassFile(Jikes::JavaModel::ClassFile* openable);

		void locateMatches(
			JavaModel::JavaModel* workspace,
			IProgressMonitor* progressMonitor);
		~MatchLocator();

		
	};



}// Close namespace Search block
	

} // Close namespace Jikes block

#endif