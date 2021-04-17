#include "stdafx.h"
#include  "JavaModelLib/internal/core/IImportDeclaration.h"
#include <JavaModelLib/internal/core/ISourceRange.h>
#include <vector>
#include <string>
#include <boost/algorithm/string.hpp>
using  namespace std;

namespace Jikes { // Open namespace Jikes block


	namespace JavaModel {


		IImportDeclaration::IImportDeclaration(IJavaElement* parent , const wstring& name)
			: JavaElement(parent, IMPORT_DECLARATION, name), source_rang(nullptr), name_rang(nullptr)
		{

		}

		IImportDeclaration::IImportDeclaration(IImportDeclaration& o): JavaElement(o), source_rang(nullptr), name_rang(nullptr)
		{
		
		}

		ISourceRange* IImportDeclaration::getSourceRange()
		{
			
			return source_rang;
		}

		void IImportDeclaration::setSourceRange(ISourceRange* _rng)
		{
			delete source_rang;
			source_rang = _rng;
		}

		void IImportDeclaration::figureRange()
		{

		}

		IImportDeclaration::~IImportDeclaration()
		{
			if(source_rang)
				delete source_rang;
			if(name_rang)
				delete name_rang;
		}

		bool IImportDeclaration::isOnDemand()
		{
			auto size = fName.size();
			if (size > 2)
			{
				if(fName[size -1] != '*')
				{
					return false;
				}
				if (fName[size - 2] != '.')
				{
					return false;
				}
				return true;
			}
			else
			{
				return false;
			}
		}

		 IJavaElement* IImportDeclaration::DeepCopy(IJavaElement* parent)
		{
			auto clone = reinterpret_cast<IImportDeclaration*>(ShallowCopy(parent));
			if (!clone)
				return nullptr;

			if (source_rang) {
				clone->source_rang = source_rang->Clone();
			}
			if (name_rang) {
				clone->name_rang = name_rang->Clone();
			}
			return clone;
		}

		ISourceRange* IImportDeclaration::getNameRange()
		{
			return name_rang;
		}

		void IImportDeclaration::setNameRange(ISourceRange* _rng)
		{
			delete name_rang;
			name_rang = _rng;
		}

		
	} // Close namespace JavaModel block

} // Close namespace Jikes block

