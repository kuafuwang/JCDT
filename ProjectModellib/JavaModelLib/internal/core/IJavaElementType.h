
#ifndef IJavaElementType_java_model_INCLUDED_INCLUDED
#define IJavaElementType_java_model_INCLUDED_INCLUDED

namespace Jikes { // Open namespace Jikes block


namespace JavaModel{

	enum IJavaElementType
	{
		JAVA_MODEL = 1,

		JAVA_PROJECT = 2,

		PACKAGE_FRAGMENT_ROOT,

		JAR_PACKAGE_FRAGMENT_ROOT,

		JAR_PACKAGE_FRAGMENT,


		PACKAGE_FRAGMENT,

		COMPILATION_UNIT,

		JAR_COMPILATION_UNIT,



		CLASS_FILE,

		JAR_CLASS_FILE,

		TYPE ,

		FIELD ,

		METHOD ,

		INITIALIZER ,

		PACKAGE_DECLARATION ,

		IMPORT_CONTAINER ,

		IMPORT_DECLARATION ,

		LOCAL_FIELD
	};

}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

