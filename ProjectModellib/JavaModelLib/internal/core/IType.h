
#ifndef IType_java_model_INCLUDED_INCLUDED
#define IType_java_model_INCLUDED_INCLUDED
#include "Member.h"


using std::wstring;
namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	
	class IField;
	class IMethod;
	class IPackageFragment;

	class IClassFile;
	class ICompilationUnit;
	class BaseJavaFile;
	class IInitializer;

	class IType
		: public Member
		 
	{
	public:
		
		virtual void SortOutLineNameIdentity() = 0;

		IType(IJavaElement* parent, const wstring& name);

		IType(IType& o);

		IType* getDeclaringType() override;

		virtual bool isClass() = 0;

		virtual const std::vector<IType*>* getTypes() = 0;

		virtual IMethod* getMethod(const wstring& name,
			const vector<wstring>& parameterTypes) = 0;
	
		virtual IField* getField(const wstring& name) = 0;

		
		virtual IInitializer* getInitializer(int start_pos) = 0;

		//virtual IMethod* createIndepentMethod(const wstring& name,
		//	const vector<wstring>& parameterTypes) = 0;

		//virtual IField* creatIndependtField(const wstring& name) = 0;



		virtual const std::vector<IField*> *getFields() = 0;

		virtual const std::vector<IMethod*>* getMethods() = 0;

		virtual IType* getType(const wstring& name) = 0;

		//virtual IType* createIndependentType(const wstring& name) = 0;

		IPackageFragment* getPackageFragment();
		PathModel::MosesPath* getMosesPath() override;
		wstring getTypeQualifiedName(wchar_t enclosingTypeSeparator);

		virtual IType* DeepCopyButNoChildren(IJavaElement* parent) = 0;
	};

}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

