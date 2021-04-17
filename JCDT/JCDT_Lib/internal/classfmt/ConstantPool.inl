#ifndef jcdt_class_ConstantPool_INCLUDED
#define jcdt_class_ConstantPool_INCLUDED
#include "CPInfo.inl"


namespace Jikes { // Open namespace Jikes block


//
// Specialization of Tuple to more gracefully handle invalid indices. This
// is because slot 0 and slots following a long or double entry are invalid.
//

class ConstantPool 
{
	class CPInvalid : public CPInfo
	{
	public:
		CPInvalid() : CPInfo((ConstantPoolTag)0) {}
		~CPInvalid() {}
		virtual void Put(OutputBuffer&) const
		{
			assert(false && "trying to put invalid ConstantPool entry");
		}
		virtual bool Check(const ConstantPool&) const { return false; }

#ifdef JIKES_DEBUG
		virtual void Print(Ostream& os,const ConstantPool&) const
		{
			os << "invalid index" << endl;
		}
		virtual void Describe(Ostream& os,const ConstantPool&) const
		{
			os << "(invalid)";
		}
#endif // JIKES_DEBUG
	} invalid;

public:
	ConstantPool(unsigned estimate = 0)
		
	{
		pool.reserve(estimate);
		pool.push_back( NULL); // The 0th entry is unused.
	}
	ConstantPool(unsigned log_blksize, unsigned base_increment)
		
	{
		//Next() = NULL; // The 0th entry is unused.
		pool.reserve(base_increment + log_blksize);
		pool.push_back(NULL); // The 0th entry is unused.
	}
	~ConstantPool() {}

	// Inherit only the methods we know are safe.
	//using Tuple<CPInfo*>::Length;
	//using Tuple<CPInfo*>::SpaceAllocated;
	//using Tuple<CPInfo*>::SpaceUsed;

	//
	// Note that unlike in Tuple<>, ConstantPool[] does not return an lvalue;
	// use SetNext(CPInfo*) instead.
	//
	inline const CPInfo* operator[](const unsigned i) const
	{
		if (i >= pool.size())
			return &invalid;
		const CPInfo* result = pool[i];
		return result ? result : &invalid;
	}

	inline void Reset()
	{
		//Tuple<CPInfo*>::Reset();
		pool.clear();
	}
	inline unsigned Length() const
	{
		return pool.size();
	}
	void SetNext(CPInfo*);

	inline bool Valid(const unsigned i)
	{
		return i < pool.size() && pool[i];
	}
	bool Check() const;

private:
	vector<CPInfo*> pool;
};


} // Close namespace Jikes block


#endif // _INCLUDED
