#ifndef jcdt_class_CPLongInfo_INCLUDED
#define jcdt_class_CPLongInfo_INCLUDED
#include "CPInfo.inl"
#include <JCDT_Lib/internal/impl/long.h>


namespace Jikes { // Open namespace Jikes block


//
// Describes a long literal. JVMS 4.4.5.
//
class CPLongInfo : public CPInfo
{
    // u1 tag; // inherited from CPInfo
    const LongInt value; // u4 high_bytes, low_bytes;

public:
    CPLongInfo(u4 high_bytes, u4 low_bytes)
        : CPInfo(CONSTANT_Long)
        , value(high_bytes, low_bytes)
    {}
    CPLongInfo(ClassFile&);
    virtual ~CPLongInfo() {}

    virtual void Put(OutputBuffer& out) const
    {
        CPInfo::Put(out);
        out.PutU4(value.HighWord());
        out.PutU4(value.LowWord());
    }

    const LongInt& Value() const { return value; }

#ifdef JIKES_DEBUG
    virtual void Print(Ostream& os,const ConstantPool&) const
    {
        os << "Long: high: 0x"
                << IntToString(value.HighWord(), 8).String()
                << ", low: 0x" << IntToString(value.LowWord(), 8).String()
                << " (" << LongToString(value).String() << "L)" << endl;
    }

    virtual void Describe(Ostream& os,const ConstantPool&) const
    {
        os << LongToString(value).String() << 'L';
    }
#endif // JIKES_DEBUG
};


} // Close namespace Jikes block


#endif // _INCLUDED
