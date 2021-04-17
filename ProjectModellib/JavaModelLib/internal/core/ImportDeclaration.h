#pragma once

#ifndef ImportDeclaration_java_model_INCLUDED_INCLUDED
#define ImportDeclaration_java_model_INCLUDED_INCLUDED

#include "IImportDeclaration.h"
namespace Jikes { // Open namespace Jikes block


	namespace JavaModel {

		class ImportDeclaration
			:public IImportDeclaration
		{
		public:

			ImportDeclaration(IJavaElement* parent, const wstring& name)
				: IImportDeclaration(parent, name)
			{
			}

			ImportDeclaration(IImportDeclaration& o)
				: IImportDeclaration(o)
			{
			}

			IJavaElement* ShallowCopy(IJavaElement* parent) override {
				return new ImportDeclaration(parent, fName);
			}
		};

	}// Close namespace JavaModel block


} // Close namespace Jikes block



#endif // _INCLUDED

