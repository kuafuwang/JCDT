
#ifndef SemanticEnvironmentStack_INCLUDED
#define SemanticEnvironmentStack_INCLUDED
#include "JCDT_Lib/internal/env/platform.h"
#include "JCDT_Lib/internal/util/tuple.h"


namespace Jikes { // Open namespace Jikes block


//
// A stack of semantic environments.
//
template <typename T>
class SemanticEnvironmentStack
{
public:
	void Push(T env) { info.Next() = env; }

	void Pop()
	{
		assert(info.Length());
		info.Reset(info.Length() - 1);
	}

	int Size() { return info.Length(); }

	T Top()
	{
		assert(info.Length());
		return info[info.Length() - 1];
	}

	T operator[](const int i) { return info[i]; }

private:
	Tuple<T> info;
};


//class SemanticEnvironmentStack
//{
//public:
//	void Push(SemanticEnvironment* env) { info.Next() = env; }
//
//	void Pop()
//	{
//		assert(info.Length());
//		info.Reset(info.Length() - 1);
//	}
//
//	int Size() { return info.Length(); }
//
//	SemanticEnvironment* Top()
//	{
//		assert(info.Length());
//		return info[info.Length() - 1];
//	}
//
//	SemanticEnvironment* operator[](const int i) { return info[i]; }
//
//private:
//	Tuple<SemanticEnvironment*> info;
//};
//

} // Close namespace Jikes block


#endif // _INCLUDED

