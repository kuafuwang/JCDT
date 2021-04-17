
#ifndef DefiniteFinalAssignmentStack_INCLUDED
#define DefiniteFinalAssignmentStack_INCLUDED

#include "JCDT_Lib/internal/env/platform.h"
#include "JCDT_Lib/internal/ast/ast.h"
#include "JCDT_Lib/internal/util/tuple.h"

namespace Jikes { // Open namespace Jikes block


				  //
				  // A stack of expressions where assignment occurred to a final variable.
				  //
	class DefiniteFinalAssignmentStack
	{
	public:
		void Push() { info.Next().Reset(); }
		void Pop()
		{
			if (info.Length())
				info.Reset(info.Length() - 1);
		}
		unsigned Size() { return info.Length(); }
		Tuple<Expression*>& Top()
		{
			assert(info.Length());
			return info[info.Length() - 1];
		}

	private:
		Tuple<Tuple<Expression*> > info;
	};

	

} // Close namespace Jikes block


#endif // _INCLUDED

