/**
 * This interface is used by IRequestorNameLookup. As results
 * are found by IRequestorNameLookup, they are reported to this
 * interface. An IJavaElementRequestor is able to cancel
 * at any time (i.e. stop receiving results), by responding
 * <code>true</code> to <code>#isCancelled</code>.
 */
#ifndef IJavaElementRequestor_lookup_java_model_INCLUDED_INCLUDED
#define IJavaElementRequestor_lookup_java_model_INCLUDED_INCLUDED
namespace Jikes
{ // Open namespace Jikes block


	namespace JavaModel
	{
		class IPackageFragment;
		class IMethod;
		class IType;
		class IInitializer;
		class IField;

		class IJavaElementRequestor
		{
		public:
			virtual ~IJavaElementRequestor()
			{
			}

			virtual void acceptField(IField* field) = 0;
			virtual void acceptInitializer(IInitializer* initializer) = 0;
			virtual void acceptMemberType(IType* type) = 0;
			virtual void acceptMethod(IMethod* method) = 0;
			virtual void acceptPackageFragment(IPackageFragment* packageFragment) = 0;
			virtual void acceptType(IType* type) = 0;
			/**
				* Returns <code>true</code> if this IJavaElementRequestor does
				* not want to receive any more results.
				*/
			virtual bool isCanceled() = 0;
		};
	}//  end of namespace JavaModel ;
}//  end of namespace Jikes ;


#endif

 