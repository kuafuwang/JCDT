
#ifndef SemanticStack_INCLUDED
#define SemanticStack_INCLUDED


namespace Jikes { // Open namespace Jikes block



//
// Maintains a stack of information used by Semantic.
//
	template <typename T>
	class SemanticStack
	{
	public:
		void Push(T next = T()) { info.Next() = next; }
		void Pop()
		{
			if (info.Length())
				info.Reset(info.Length() - 1);
		}
		unsigned Size() { return info.Length(); }
		T Top()
		{
			return info.Length() ? info[info.Length() - 1] : T();
		}
		T operator[](const unsigned i) { return info[i]; }

	private:
		Tuple<T> info;
	};


} // Close namespace Jikes block


#endif // _INCLUDED

