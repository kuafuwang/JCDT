
#ifndef ExplicitConstructorCall_Jikes_INCLUDED
#define ExplicitConstructorCall_Jikes_INCLUDED
#include "InvocationSite.h"


namespace Jikes { // Open namespace Jikes block

   class ExplicitConstructorCall: public InvocationSite {
   public:
	   bool isTypeAccess() override {
		   return  true;
	   }
   };
	

} // Close namespace Jikes block


#endif // _INCLUDED

