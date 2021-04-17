#pragma once

#ifndef Jikes_JavaModel_SearchableEnvironmentRequestor_INCLUDED
#define Jikes_JavaModel_SearchableEnvironmentRequestor_INCLUDED

#include "JavaElementRequestor.h"
namespace Jikes { // Open namespace Jikes block


	namespace JavaModel {
		class ISearchRequestor;
		class ICompilationUnit;

		class SearchableEnvironmentRequestor :public  JavaElementRequestor , public IJavaElementRequestor
		{
		public:
			/**
			 * The <code>ISearchRequestor</code> this JavaElementRequestor wraps
			 * and forwards results to.
			 */
			 ISearchRequestor* fRequestor;
			/**
			 * The <code>ICompilationUNit</code> this JavaElementRequestor will not
			 * accept types within.
			 */
			 ICompilationUnit* fUnitToSkip;
			/**
			 * Constructs a SearchableEnvironmentRequestor that wraps the
			 * given SearchRequestor.
			 */
			SearchableEnvironmentRequestor(ISearchRequestor* requestor);

			/**
			 * Constructs a SearchableEnvironmentRequestor that wraps the
			 * given SearchRequestor.  The requestor will not accept types in
			 * the <code>unitToSkip</code>.
			 */
			SearchableEnvironmentRequestor(ISearchRequestor* requestor, ICompilationUnit* unitToSkip);

			/**
			 * Do nothing, a SearchRequestor does not accept initializers
			 * so there is no need to forward this results.
			 *
			 * @see IJavaElementRequestor
			 */
			 void acceptInitializer(IInitializer* initializer) {

			}
			/**
			 * @see IJavaElementRequestor
			 */
			void acceptPackageFragment(IPackageFragment* packageFragment);

			/**
			 * @see IJavaElementRequestor
			 */
			void acceptType(IType* type);
		public:
			void acceptField(IField* field) override;
			void acceptMemberType(IType* type) override;
			void acceptMethod(IMethod* method) override;
			bool isCanceled() override;
		};
	}
}

#endif 

