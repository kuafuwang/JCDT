
#ifndef SingleTypeRequestor_lookup_java_model_INCLUDED_INCLUDED
#define SingleTypeRequestor_lookup_java_model_INCLUDED_INCLUDED
#include "IJavaElementRequestor.hpp"
#include <JavaModelLib/internal/core/IPackageFragment.h>

namespace Jikes { // Open namespace Jikes block


namespace JavaModel{

	/**
	* The SingleTypeRequestor is an IJavaElementRequestor that
	* only accepts one result element and then cancels.
	*/
	/* package */
	class SingleTypeRequestor :public IJavaElementRequestor {
	public:
		/**
		* The single accepted element
		*/
		IType* fElement = nullptr;
		/**
		* @see IJavaElementRequestor
		*/
		void acceptField(IField* field) {
		}
		/**
		* @see IJavaElementRequestor
		*/
		void acceptInitializer(IInitializer* initializer) {
		}
		/**
		* @see IJavaElementRequestor
		*/
		void acceptMemberType(IType* type) {
			fElement = type;
		}
		/**
		* @see IJavaElementRequestor
		*/
		void acceptMethod(IMethod* method) {
		}
		/**
		* @see IJavaElementRequestor
		*/
		void acceptPackageFragment(IPackageFragment* packageFragment) {
		}
		/**
		* @see IJavaElementRequestor
		*/
		void acceptType(IType* type) {
			fElement = type;
		}
		/**
		* Returns the type accepted by this requestor, or <code>null</code>
		* if no type has been accepted.
		*/
		IType* getType() {
			return fElement;
		}
		/**
		* @see IJavaElementRequestor
		*/
		bool isCanceled() {
			return fElement != nullptr;
		}
		/**
		* Reset the state of this requestor
		*/
		void reset() {
			fElement = nullptr;
		}
	};
}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

