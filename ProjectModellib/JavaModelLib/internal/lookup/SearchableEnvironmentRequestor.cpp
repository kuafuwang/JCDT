#include "stdafx.h"
#include <JavaModelLib/internal/lookup/SearchableEnvironmentRequestor.h>
#include "ISearchRequestor.h"
#include <JavaModelLib/internal/core/IType.h>
#include <JavaModelLib/internal/core/ICompilationUnit.h>
namespace Jikes { // Open namespace Jikes block


	namespace JavaModel {
		SearchableEnvironmentRequestor::SearchableEnvironmentRequestor(ISearchRequestor* requestor)
		{
			fRequestor = requestor;
			fUnitToSkip = nullptr;
		}

		SearchableEnvironmentRequestor::SearchableEnvironmentRequestor(ISearchRequestor* requestor, ICompilationUnit* unitToSkip)
		{
			fRequestor = requestor;
			fUnitToSkip = unitToSkip;
		}

		void SearchableEnvironmentRequestor::acceptPackageFragment(IPackageFragment* packageFragment)
		{
			if (!packageFragment)
				return;
			fRequestor->acceptPackage(packageFragment->getElementName());
		}

		void SearchableEnvironmentRequestor::acceptType(IType* type)
		{
			if (!type)
				return;

			if (fUnitToSkip != nullptr && fUnitToSkip == type->getParent() )
			{
				return;
			}
			auto pkg_frag = type->getPackageFragment();
			if (!pkg_frag)
				return;

			if (type->isClass())
			{
				fRequestor->acceptClass(pkg_frag->getElementName(), type->getElementName(),
					type->getAccessFlags().Flags());
			}
			else
			{
				fRequestor->acceptInterface(pkg_frag->getElementName(), type->getElementName(),
					type->getAccessFlags().Flags());
			}
				
		}

		void SearchableEnvironmentRequestor::acceptField(IField* field)
		{
		}

		void SearchableEnvironmentRequestor::acceptMemberType(IType* type)
		{
		}

		void SearchableEnvironmentRequestor::acceptMethod(IMethod* method)
		{
		}

		bool SearchableEnvironmentRequestor::isCanceled()
		{
			return false;
		}
	} // Close namespace JavaModel block

} // Close namespace Jikes block

