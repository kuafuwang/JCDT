
#ifndef JavaFileBase_java_model_INCLUDED_INCLUDED
#define JavaFileBase_java_model_INCLUDED_INCLUDED
#include <JCDT_Lib/internal/impl/FileBase.h>
#include "Openable.h"

#include "IPackageFragment.h"
#include "IPackageDeclaration.h"
#include <boost/smart_ptr/shared_ptr.hpp>
#include <JCDT_Lib/internal/util/CharOperation.h>

namespace Jikes { // Open namespace Jikes block
	class ClassFile;

namespace JavaModel{
	class IType;
	class LayerPosInJavaModel;
	class BuildProblem;
	class IImportDeclaration;

	class BaseJavaFile 
		:public Openable
		
	{
	public:
		wstring getPath() override;
		IJavaElement* DeepCopy(IJavaElement*) override;
		wstring getAttachSourcePath();
		std::vector<IJavaElement*>* getChildrenOfType(IJavaElementType _type) override;
		bool hasChildren() override;
		PathModel::MosesPath* getMosesPath();
		void addChild(IJavaElement* child) override;
	
		virtual ~BaseJavaFile();

	
		BaseJavaFile(IJavaElementType _type, IPackageFragment* p,const wstring& name);

		BaseJavaFile(BaseJavaFile&);
	
		void removeInfo() override;
		
		bool IsZip();

		virtual std::vector<IJavaElement*>* getChildren() override;

		IPackageFragment* ContainingPackage() const{
			return package;
		}
		const vector<IType*>* getAllTypes();
		IType* getTypeByElementName(wstring& elementName);
		BaseJavaFile* GetNoLazyLoadAndSeparateFromParentClone();
		LayerPosInJavaModel* GetPosition() const{
			return _pos;
		}
		vector<wstring>& getImportPackages();
		unsigned long  m_iDeclsVersion;
		inline unsigned long GetDeclVersion() const
		{
			return m_iDeclsVersion;
		}
		void SortOutLineNameIdentity() ;

		void CalCulatePos();
		
	protected:
		LayerPosInJavaModel* _pos;
	
		vector< IType* > * types = nullptr;
		vector<wstring> import_pakcages;
		vector< IPackageDeclaration* > * packageDecls = nullptr;
		vector< IImportDeclaration* >* importDecls;
		IPackageFragment* package = nullptr;

		void HelpToDeepCopy(BaseJavaFile& o);
	
		bool CopyChildFromOriginalWhenIsClone();


	};

	
	

}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

