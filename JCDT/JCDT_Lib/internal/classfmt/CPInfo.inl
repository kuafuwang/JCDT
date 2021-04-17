#ifndef jcdt_class_CPInfo_INCLUDED
#define jcdt_class_CPInfo_INCLUDED
#include <JCDT_Lib/internal/util/tuple.h>


namespace Jikes {
	class ClassFile;
	class ConstantPool;
	// Open namespace Jikes block


//
// The prefix CP stands for Constant Pool. This is the base class of all
// constant pool information; subclasses give more detail. JVMS 4.4.
//

class CPInfo
{
public:
	enum ConstantPoolTag
	{
		CONSTANT_Class = 7,
		CONSTANT_Fieldref = 9,
		CONSTANT_Methodref = 10,
		CONSTANT_InterfaceMethodref = 11,
		CONSTANT_String = 8,
		CONSTANT_Integer = 3,
		CONSTANT_Float = 4,
		CONSTANT_Long = 5,
		CONSTANT_Double = 6,
		CONSTANT_NameAndType = 12,
		CONSTANT_Utf8 = 1
	};

protected:
	const ConstantPoolTag tag;

public:
	CPInfo(ConstantPoolTag _tag) : tag(_tag) {}
	virtual ~CPInfo() {}

	ConstantPoolTag Tag() const { return tag; }
	bool Large() const
	{
		return tag == CONSTANT_Long || tag == CONSTANT_Double;
	}
	bool Constant() const
	{
		return ((tag >= CPInfo::CONSTANT_Integer &&
			tag <= CPInfo::CONSTANT_Double) ||
			tag == CPInfo::CONSTANT_String);
	}

	//
	// This reads the next constant pool entry from buffer, advancing it to
	// the end of the entry. The user is responsible for deleting the result.
	//
	static CPInfo* AllocateCPInfo(ClassFile&);

	//
	// Subclasses should override, to write an entry to a .class file.
	//
	virtual void Put(OutputBuffer& out) const
	{
		out.PutU1((u1)tag);
	}

	//
	// Allows subclasses to perform data validation, once the entire constant
	// pool has been read. Return false on failure.
	//
	virtual bool Check(const ConstantPool&) const { return true; }

#ifdef JIKES_DEBUG
	//
	// Prints all information about the constant value, ending the line.
	//
	virtual void Print(Ostream& os, const ConstantPool&) const
	{
		os << "unknown tag: " << (unsigned)tag << endl;
	}

	//
	// Prints shortened information, no newline.
	//
	virtual void Describe(Ostream& os, const ConstantPool&) const
	{
		os << "unknown tag: " << (unsigned)tag;
	}
#endif // JIKES_DEBUG
};


} // Close namespace Jikes block


#endif // _INCLUDED
