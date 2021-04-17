
#ifndef InvocationSite_Jikes_INCLUDED
#define InvocationSite_Jikes_INCLUDED


namespace Jikes { // Open namespace Jikes block


	class InvocationSite
	{
		
	public:
		virtual ~InvocationSite()
		{
		}

		virtual	bool isSuperAccess() = 0;
	   virtual	bool isTypeAccess() = 0;
	};

} // Close namespace Jikes block


#endif // _INCLUDED

