#include "stdafx.h"
#include <JavaModelLib/internal/core/ClassFile.h>
#include <JavaModelLib/internal/core/JarPackageFragmentRoot.h>

#include <JavaModelLib/internal/core/IType.h>

#include <JavaModelLib/internal/core/JavaModel.h>
#include <JavaModelLib/internal/core/BinaryType.h>
#include <JavaModelLib/internal/core/PackageDeclaration.h>


namespace Jikes { // Open namespace Jikes block


	namespace JavaModel {
		
		JarPackageFragmentRoot* ClassFile::getPackagePackageFragmentRoot()
		{
			if (!fParent)
				return nullptr;
			auto parent = fParent->getParent();
			if (!parent)
				return nullptr ;

			return reinterpret_cast<JarPackageFragmentRoot*>(parent);
		}

		wstring ClassFile::getFullName()
		{
			return L"class file " + fName;
		}

		IJavaElement* ClassFile::ShallowCopy(IJavaElement* parent){
		
			auto clone = new ClassFile(*this);
			clone->MarkClone();
			clone->fParent = parent;
			
			return clone;
		}

		bool ClassFile::isOpen()
		{
			return true;
		}

		

		IType* ClassFile::getType()
		{
			LazyLoad();
			if (types && types->size())
			{
				return types->operator[](0);
			}
			else
			{
				return nullptr;
			}
		}
		
	
		ClassFile::ClassFile(IJavaElementType _type,
			IPackageFragment* parent,
			const wstring& name_symbol_ )
			: IClassFile(_type, parent,name_symbol_), _class_in_jar(nullptr)
		{
		}


		ClassFile::ClassFile(ClassFile&o): IClassFile(o)
			, _class_in_jar(o._class_in_jar){
			
		}
	/*	IType* ClassFile::createIndependentType()
		{
			auto temp = new IndependentBinaryType(this, fName);
			temp->getMosesPath();
			return temp;
		}*/
		bool ClassFile::generateInfos(IProgressMonitor* pm)
		{
			
			IPackageFragment* pkg_frgment = dynamic_cast<IPackageFragment*>(fParent);

			if (!pkg_frgment) {
				return true;
			}
			auto pkgName = pkg_frgment->getElementName();
			auto pkg = new PackageDeclaration(pkgName, this);
			if (!packageDecls)
				packageDecls = new vector< IPackageDeclaration* >();

			packageDecls->push_back(pkg);

			
			BinaryType* mainType = new BinaryType(this, fName);
			mainType->info = new  SourceTypeElementInfo();
			
			boost::split(mainType->info->qualifiedTypeNames, pkgName, boost::is_any_of(L"."));

			
			if(fName.find(L"$") != wstring::npos){

				vector<wstring> innerTypeNames;
				boost::split(innerTypeNames, pkgName, boost::is_any_of(L"$"));
				size_t i = 0, size =  innerTypeNames.size() - 1;
				for(; i < size; ++i){
					mainType->info->qualifiedTypeNames.emplace_back(innerTypeNames[i]);
				}
				mainType->info->name = (innerTypeNames[i]);
			}
			else
			{
				mainType->info->name = (fName);
			}
			

			mainType->file_symbol = this;

			if (!types) {
				types = new vector<IType*>();
			}
			types->push_back(mainType);
			InterlockedIncrement(&m_iDeclsVersion);


			return true;
		}

	

		
		std::vector<IJavaElement*>* ClassFile::getChildren()
		{
			LazyLoad();
			if (types && types->size())
				return new std::vector<IJavaElement*>(types->begin(), types->end());
			else
				return nullptr;
		}

		void ClassFile::addChild(IJavaElement* child)
		{
			if (child)
			{
				if (child->getElementType() == IJavaElementType::TYPE)
				{
					if (!types) {
						types = new vector<IType*>();
					}
					types->push_back(dynamic_cast<IType*>(child));
				}

			}
		}

	} // Close namespace JavaModel block

} // Close namespace Jikes block

