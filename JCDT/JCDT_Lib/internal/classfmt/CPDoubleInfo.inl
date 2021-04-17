#ifndef jcdt_class_CPDoubleInfo_INCLUDED
#define jcdt_class_CPDoubleInfo_INCLUDED
#include "CPInfo.inl"
#include <JCDT_Lib/internal/impl/double.h>


namespace Jikes { // Open namespace Jikes block


//
// Describes a double literal. JVMS 4.4.5.
//
class CPDoubleInfo : public CPInfo
{
    // u1 tag; // inherited from CPInfo
    const IEEEdouble value; // u4 high_bytes, low_bytes;

public:
    CPDoubleInfo(u4 high_bytes, u4 low_bytes)
        : CPInfo(CONSTANT_Double)
        , value(high_bytes, low_bytes)
    {}
    CPDoubleInfo(ClassFile&);
    virtual ~CPDoubleInfo() {}

    virtual void Put(OutputBuffer& out) const
    {
        CPInfo::Put(out);
        out.PutU4(value.HighWord());
        out.PutU4(value.LowWord());
    }

    const IEEEdouble& Value() const { return value; }

#ifdef JIKES_DEBUG
    virtual void Print(Ostream& os,const ConstantPool&) const
    {
        os << "Double: high: 0x"
                << IntToString(value.HighWord(), 8).String()
                << ", low: 0x" << IntToString(value.LowWord(), 8).String()
                << " (" << DoubleToString(value).String() << ')' << endl;
    }

    virtual void Describe(Ostream& os,const ConstantPool&) const
    {
        os << DoubleToString(value).String();
    }
#endif // JIKES_DEBUG
};


} // Close namespace Jikes block


#endif // _INCLUDED
