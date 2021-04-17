
#ifndef SourceMapper_lookup_java_model_INCLUDED_INCLUDED
#define SourceMapper_lookup_java_model_INCLUDED_INCLUDED
#include <JavaModelLib/internal/core/BinaryType.h>


namespace Jikes { // Open namespace Jikes block

	class JikesError;
	class AstStoragePool;
	class Option;
	class Scanner;
	class Parser;
	class LexStream;
	class INameEnvironment;
namespace JavaModel{
	class BinaryType;
   class SourceMapper{
   public:
	   SourceMapper();
	   ~SourceMapper();
	   void MapperSource(BinaryType* type);
	   static void BuildSourceNameRange(BinaryType* _type, TypeDeclaration*, LexStream*);
   private:
	   INameEnvironment* name_env;
	   Option* option;
	   char** parsedOptions;
	   Parser* parser;
	   Scanner* scanner;
   };

	
}	

} // Close namespace Jikes block


#endif // _INCLUDED

