#ifndef jcdt_class_CPFloatInfo_INCLUDED
#define jcdt_class_CPFloatInfo_INCLUDED
#include "CPInfo.inl"
#include <JCDT_Lib/internal/impl/double.h>


namespace Jikes { // Open namespace Jikes block


//
// Describes a float literal. JVMS 4.4.4.
//
class CPFloatInfo : public CPInfo
{
    // u1 tag; // inherited from CPInfo
    const IEEEfloat value; // u4 bytes

public:
    CPFloatInfo(u4 bytes)
        : CPInfo(CONSTANT_Float)
        , value(bytes)
    {}
    CPFloatInfo(ClassFile&);
    virtual ~CPFloatInfo() {}

    virtual void Put(OutputBuffer& out) const
    {
        CPInfo::Put(out);
        out.PutU4(value.Word());
    }

    const IEEEfloat& Value() const { return value; }

#ifdef JIKES_DEBUG
    virtual void Print(Ostream& os,const ConstantPool&) const
    {
        os << "Float: bytes: 0x"
                << IntToString(value.Word(), 8).String() << " ("
                << FloatToString(value).String() << "F)" << endl;
    }

    virtual void Describe(Ostream& os,const ConstantPool&) const
    {
        os << FloatToString(value).String() << 'F';
    }
#endif // JIKES_DEBUG
};


} // Close namespace Jikes block


#endif // _INCLUDED
