
#ifndef JavaElementRequestor_lookup_java_model_INCLUDED_INCLUDED
#define JavaElementRequestor_lookup_java_model_INCLUDED_INCLUDED
#include "IJavaElementRequestor.hpp"
#include <vector>
#include <JavaModelLib/internal/core/IPackageFragment.h>
using std::vector;
namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	
	/**
	* @see IJavaElementRequestor
	*/

	 class JavaElementRequestor :public
		 IJavaElementRequestor {
		/**
		* True if this requestor no longer wants to receive
		* results from its <code>IRequestorNameLookup</code>.
		*/
		 bool fCanceled = false;

		/**
		* A collection of the resulting fields, or <code>nullptr</code>
		* if no field results have been received.
		*/
		 vector< IField*> fFields;

		/**
		* A collection of the resulting initializers, or <code>nullptr</code>
		* if no initializer results have been received.
		*/
		 vector<IInitializer *> fInitializers;

		/**
		* A collection of the resulting member types, or <code>nullptr</code>
		* if no member type results have been received.
		*/
		 vector<IType *> fMemberTypes;

		/**
		* A collection of the resulting methods, or <code>nullptr</code>
		* if no method results have been received.
		*/
		 vector<IMethod *> fMethods;

		/**
		* A collection of the resulting package fragments, or <code>nullptr</code>
		* if no package fragment results have been received.
		*/
		 vector<IPackageFragment *> fPackageFragments;

		/**
		* A collection of the resulting types, or <code>nullptr</code>
		* if no type results have been received.
		*/
		 vector<IType *> fTypes;

		/**
		* Empty arrays used for efficiency
		*/

	 public:

		/**
		* @see IJavaElementRequestor
		*/
		 void acceptField(IField* field) {
			
			fFields.push_back(field);
		}
		/**
		* @see IJavaElementRequestor
		*/
		 void acceptInitializer(IInitializer* initializer) {
		
			fInitializers.push_back(initializer);
		}
		/**
		* @see IJavaElementRequestor
		*/
		 void acceptMemberType(IType* type) {
			
			fMemberTypes.push_back(type);
		}
		/**
		* @see IJavaElementRequestor
		*/
		 void acceptMethod(IMethod* method) {
		
			fMethods.push_back(method);
		}
		/**
		* @see IJavaElementRequestor
		*/
		 void acceptPackageFragment(IPackageFragment* packageFragment) {
			fPackageFragments.push_back(packageFragment);
		}
		/**
		* @see IJavaElementRequestor
		*/
		 void acceptType(IType* type) {
			fTypes.push_back(type);
		}
		/**
		* @see IJavaElementRequestor
		*/
		 const vector<IField*>& getFields() {
			return fFields;
		}
		/**
		* @see IJavaElementRequestor
		*/
		const vector< IInitializer*> & getInitializers() {
			return fInitializers;

		}
		/**
		* @see IJavaElementRequestor
		*/
		const vector<IType*>& getMemberTypes() {
			return fMemberTypes;
			
		}
		/**
		* @see IJavaElementRequestor
		*/
		 const vector<IMethod*>& getMethods() {	
			 return fMethods;
		}
		/**
		* @see IJavaElementRequestor
		*/
		 const vector<IPackageFragment*>& getPackageFragments() {		
			 return fPackageFragments;	
		}
		/**
		* @see IJavaElementRequestor
		*/
		 const vector<IType*>&  getTypes() {
			 return fTypes;	
		}
		/**
		* @see IJavaElementRequestor
		*/
		 bool isCanceled() {
			return fCanceled;
		}
		/**
		* Reset the state of this requestor.
		*/
		 void reset() {
			fCanceled = false;
			fFields.clear();
			fInitializers.clear();;
			fMemberTypes.clear();
			fMethods.clear();
			fPackageFragments.clear();
			fTypes.clear();
		}
		/**
		* Sets the #isCanceled state of this requestor to true or false.
		*/
		 void setCanceled(bool b) {
			fCanceled = b;
		}
	};
}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

