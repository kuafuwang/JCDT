

#include <JCDT_Lib/internal/classfmt/ConstantPool.inl>

namespace Jikes { // Open namespace Jikes block


	void ConstantPool::SetNext(CPInfo* constant)
	{
		pool.push_back(constant);
		if (constant->Large())
			pool.push_back(NULL); // entries following Long or Double are unused
	}

	bool ConstantPool::Check() const
	{
		for (unsigned i = 1; i < pool.size(); i++)
		{
			CPInfo* entry = pool[i];
			if (entry && !entry->Check(*this))
				return false;
		}
		return true;
	}

	

} // Close namespace Jikes block



