#ifndef LocatorCompiler_JIKES_SERACH_INCLUDE
#define LocatorCompiler_JIKES_SERACH_INCLUDE
#include <boost/smart_ptr/shared_ptr.hpp>
#include <JCDT_Lib/compiler/Compiler.h>
namespace Jikes { // Open namespace Jikes block
	class FileSymbol;
	class LookupEnvironment;
	namespace JavaModel
	{
		class IJavaElement;
		class ClassFile;
		class Openable;
		class JavaModel;
		class NameLookup;
		class CompilationUnit;
		class OriginalJavaProject;
	}
namespace Search{
	
	class MatchSet;
	class MatchLocator;
	class BinaryMatchSet;
	class SearchPattern;

	class LocatorCompiler :public Compiler
	{
	public:

		boost::shared_ptr<JavaModel::OriginalJavaProject> original_project;
		LocatorCompiler(INameEnvironment* _name_env, Option* jikes_option,
			boost::shared_ptr<JavaModel::OriginalJavaProject> _original_project);

		JavaModel::NameLookup* getNameLookup();
		
		~LocatorCompiler();
	private:

		JavaModel::NameLookup* _lookup;
	
	};


}// Close namespace Search block
	

} // Close namespace Jikes block

#endif