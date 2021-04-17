
#ifndef IndependentBinaryType_java_model_INCLUDED_INCLUDED
#define IndependentBinaryType_java_model_INCLUDED_INCLUDED

#include <JavaModelLib/internal/core/BinaryType.h>


namespace Jikes {
	class AstClassBody;
	class AstMethodDeclarator;
	// Open namespace Jikes block
	class TypeDeclaration;
	class LexStream;
	class ClassFile;

namespace JavaModel{
	class PackageFragment;
	class JavaFileElement;
	class ClassFile;
	class IndependentBinaryType:
		public BinaryType
	{
	public:

		IJavaElement* ShallowCopy(IJavaElement* parent) override;
		IndependentBinaryType(IJavaElement* parent, const wstring& name);
		IndependentBinaryType(IndependentBinaryType&);
		virtual ~IndependentBinaryType();
	

		MemberElementInfo* getElementInfo() override;;
	};

}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

