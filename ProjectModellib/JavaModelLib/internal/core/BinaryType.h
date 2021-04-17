
#ifndef BinaryType_java_model_INCLUDED_INCLUDED
#define BinaryType_java_model_INCLUDED_INCLUDED

#include "BaseJavaType.h"
#include <JCDT_Lib/internal/ast/AbstractMethodDeclaration.h>

namespace Jikes {
	class AstClassBody;
	class AstMethodDeclarator;
	// Open namespace Jikes block
	class TypeDeclaration;
	class LexStream;
	class ClassFile;
	class MethodInfo;
	class ConstantPool;
namespace JavaModel{
	class PackageFragment;
	class JavaFileElement;
	class ClassFile;
	class BinaryMethod;

	BinaryMethod* CreateBinaryMethod(const MethodInfo* method, const ConstantPool& pool);
	class BinaryType:
		public BaseJavaType
	{
	public:
	
		void figureRange() override;
		bool isBinary() override{
			return true;
		}
		IJavaElement* ShallowCopy(IJavaElement*) override;

		BinaryType(IJavaElement* parent, const wstring& name);
		BinaryType(BinaryType&);
		virtual ~BinaryType();

		static void ProcessClassFile(BinaryType* type, SourceTypeElementInfo* type_info, Jikes::ClassFile* class_data);
		static void ReadClassFile(BinaryType*, SourceTypeElementInfo* type_info);
		MemberElementInfo* getElementInfo() override;
	
		bool isClass() override;;
	};

}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

