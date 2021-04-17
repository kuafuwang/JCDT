#ifndef  MatchingOpenable_JIKES_SERACH_INCLUDE
#define MatchingOpenable_JIKES_SERACH_INCLUDE


namespace Jikes { // Open namespace Jikes block
	class FileSymbol;
	class LookupEnvironment;
	class CompilationUnitDeclaration;
	namespace JavaModel
	{
		class IJavaElement;
		class ClassFile;
		class Openable;
		class JavaModel;
		class NameLookup;
		class CompilationUnit;
		class OriginalJavaProject;
		class Member;
	}
	namespace PathModel
	{
		class MosesPath;
	}
namespace Search{
	class MatchSet;
	class MatchLocator;
	class BinaryMatchSet;
	class SearchPattern;
	class LocatorCompiler;

	class MatchingOpenable
	{
	public :
		virtual ~MatchingOpenable();

		
		MatchingOpenable(LocatorCompiler& _data_center, SearchPattern* _patten,
			JavaModel::Openable* _openable)
			:matchSet(nullptr),
			shouldResolve(false),
			compiler(_data_center),
			pattern(_patten), 
			openable(_openable){
		  
		 }

		void CreateMatchSet();
		bool ShouldResovle();
		MatchSet* matchSet;
		bool shouldResolve;
	  LocatorCompiler& compiler;
	  SearchPattern* pattern;
	  JavaModel::Openable* openable;
	  bool copy_children;
	};


	class MatchingCompilationOpenable :public MatchingOpenable
	{
	public:
		
	
		 FileSymbol* file_symbol;
		 CompilationUnitDeclaration* ast_unit;
		 JavaModel::CompilationUnit* compilationUnit;
		MatchingCompilationOpenable(FileSymbol* _file_symbol, 
			LocatorCompiler& _data_center,
			JavaModel::CompilationUnit* _compilationUnit,
			 SearchPattern* _patten
			
		);
		~MatchingCompilationOpenable();

		FileSymbol* TransferFileSymbol()
		 {
			 auto temp = file_symbol;
			 file_symbol = nullptr;
			 return temp;
		 }
		
		 
		 void locateMatchesInCompilationUnitBeforResolve();

		 void locateMatchesInCompilationUnitAfterResolve();

	};
	class MatchingClassOpenable :public MatchingOpenable
	{
	public:
		JavaModel::ClassFile* openable;
		MatchingClassOpenable(JavaModel::ClassFile* _openable,
		                      LocatorCompiler& _data_center,
			SearchPattern* _patten);
		void locateMatchesInClassFiles();
	
	};

}// Close namespace Search block
	

} // Close namespace Jikes block

#endif