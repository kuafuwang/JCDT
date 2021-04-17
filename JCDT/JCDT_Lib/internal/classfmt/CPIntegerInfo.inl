#ifndef jcdt_class_CPIntegerInfo_INCLUDED
#define jcdt_class_CPIntegerInfo_INCLUDED
#include "CPInfo.inl"


namespace Jikes { // Open namespace Jikes block


//
// Describes an int literal. JVMS 4.4.4.
//
class CPIntegerInfo : public CPInfo
{
    // u1 tag; // inherited from CPInfo
    const u4 bytes;

public:
    CPIntegerInfo(u4 _bytes)
        : CPInfo(CONSTANT_Integer)
        , bytes(_bytes)
    {}
    CPIntegerInfo(ClassFile&);
    virtual ~CPIntegerInfo() {}

    virtual void Put(OutputBuffer& out) const
    {
        CPInfo::Put(out);
        out.PutU4(bytes);
    }

    i4 Value() const { return (i4) bytes; }

#ifdef JIKES_DEBUG
    virtual void Print(Ostream& os,const ConstantPool&) const
    {
        os << "Integer: bytes: 0x"
                << IntToString(bytes, 8).String() << " (" << (int) bytes
                << ')' << endl;
    }

    virtual void Describe(Ostream& os,const ConstantPool&) const
    {
        os << (int) bytes;
    }
#endif // JIKES_DEBUG
};


} // Close namespace Jikes block


#endif // _INCLUDED
