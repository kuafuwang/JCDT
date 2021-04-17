
#ifndef SourceType_java_model_INCLUDED_INCLUDED
#define SourceType_java_model_INCLUDED_INCLUDED

#include "BaseJavaType.h"
namespace Jikes {// Open namespace Jikes block
	class AstClassBody;
	class AstMethodDeclarator;
	class FieldDeclaration;
	class TypeDeclaration;
	class TypeSymbol;
	class LexStream;
namespace JavaModel{
	
	class SourceType:public BaseJavaType
	{
	public:
		IJavaElement* DeepCopy(IJavaElement*) override;

		IJavaElement* ShallowCopy(IJavaElement* ) override;

		SourceType(IJavaElement* parent,const wstring& name);

		SourceType(SourceType& o);

	
		static  void BuildStructure(SourceType* _type, SourceTypeElementInfo*type_element_info, 
			TypeDeclaration*, LexStream*);
		

	};

	
}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

