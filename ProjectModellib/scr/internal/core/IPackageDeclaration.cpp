#include "stdafx.h"
#include  "JavaModelLib/internal/core/IPackageDeclaration.h"
#include <JavaModelLib/internal/core/ISourceRange.h>
#include <boost/algorithm/string.hpp>
namespace Jikes { // Open namespace Jikes block


	namespace JavaModel {


		

		ISourceRange* IPackageDeclaration::getSourceRange()
		{
			
			return source_rang;
		}

		void IPackageDeclaration::setSourceRange(ISourceRange* _rng)
		{
			delete source_rang;
			source_rang = _rng;
		}

		void IPackageDeclaration::figureRange()
		{

		}

		IPackageDeclaration::~IPackageDeclaration()
		{
			if(source_rang)
				delete source_rang;
			if(name_rang)
				delete name_rang;
		}

		
		 IJavaElement* IPackageDeclaration::DeepCopy(IJavaElement* parent)
		{
			auto clone = reinterpret_cast<IPackageDeclaration*>(ShallowCopy(parent));
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

		ISourceRange* IPackageDeclaration::getNameRange()
		{
			return name_rang;
		}

		void IPackageDeclaration::setNameRange(ISourceRange* _rng)
		{
			delete name_rang;
			name_rang = _rng;
		}

		
	} // Close namespace JavaModel block

} // Close namespace Jikes block

