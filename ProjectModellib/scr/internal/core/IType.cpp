#include "stdafx.h"
#include <JavaModelLib/internal/core/IType.h>
#include <JavaModelLib/internal/core/IPackageFragment.h>
#include <PathModelLib/MosesPath/MosesPath.h>

namespace Jikes { // Open namespace Jikes block


	namespace JavaModel {
	

		IType::IType(IJavaElement* parent, const wstring& name): Member(parent, IJavaElementType::TYPE, name)
		{
		}

		IType::IType(IType& o): Member(o)
		{
		}

		IType* IType::getDeclaringType()
		{
			if (!fParent)
				return nullptr;

			switch (fParent->fLEType)
			{
			case JAR_CLASS_FILE:
			case CLASS_FILE:
			case COMPILATION_UNIT:
			case JAR_COMPILATION_UNIT:
				return this;
				
			default:
				return ((Member*)fParent)->getDeclaringType();
			}


		}

		IPackageFragment* IType::getPackageFragment()
		{
			auto type = getDeclaringType();
			if (!type)
				return nullptr;
			auto _file = type->getParent();
			if (!_file)
				return nullptr;

			return reinterpret_cast<IPackageFragment*>(_file->getParent());
		}
		PathModel::MosesPath* IType::getMosesPath()
		{
			if (moses)
				return moses;
			if (!fParent || fParent->fLEType != TYPE)
				return nullptr;

			auto parent_moses = reinterpret_cast<PathModel::MemberInFileMoses*>(fParent->getMosesPath());
			if (!parent_moses)
				return nullptr;

			vector<wstring> enclosing_types(parent_moses->enclosing_types);
			enclosing_types.push_back(parent_moses->name);
			return moses = new PathModel::MemberInFileMoses(
				parent_moses->proj,
				parent_moses->root,
				parent_moses->pkg,
				parent_moses->file,
				enclosing_types,
				fName,
				MosesPath::FIELD, isBinary());

		}
		wstring IType::getTypeQualifiedName(wchar_t enclosingTypeSeparator)
		{
			if (!fParent)
				return wstring();

			switch (fParent->fLEType)
			{
			case JAR_CLASS_FILE:
			case CLASS_FILE:
			case COMPILATION_UNIT:
			case JAR_COMPILATION_UNIT:
				return fName;

			case TYPE:
			{
				auto temp = reinterpret_cast<IType*>(fParent);
				return temp->getTypeQualifiedName(enclosingTypeSeparator)
					+ enclosingTypeSeparator + getElementName();
			}
			
			default:
				return wstring();
			}
		}
	} // Close namespace JavaModel block

} // Close namespace Jikes block

