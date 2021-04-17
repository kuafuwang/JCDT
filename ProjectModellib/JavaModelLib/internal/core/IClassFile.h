
#ifndef IClassFile_java_model_INCLUDED_INCLUDED
#define IClassFile_java_model_INCLUDED_INCLUDED
#include "BaseJavaFile.h"


namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	class IType;
	class IPackageFragment;
	class IClassFile:
		  	public BaseJavaFile
	{
	public:
		IClassFile(IJavaElementType _type, IPackageFragment* p, const wstring& name)
			: BaseJavaFile(_type, p, name)
		{
		}

		 IClassFile(BaseJavaFile& base_java_file)
			: BaseJavaFile(base_java_file)
		{
		}

		
		virtual IType* getType() = 0;
		//virtual IType* createIndependentType() = 0;
	
	};

}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

