#ifndef jcdt_class_CPStringInfo_INCLUDED
#define jcdt_class_CPStringInfo_INCLUDED
#include "CPInfo.inl"
#include "CPUtf8Info.inl"
#include "ConstantPool.inl"


namespace Jikes { // Open namespace Jikes block


//
// Describes a String literal. JVMS 4.4.3.
//
class CPStringInfo : public CPInfo
{
    // u1 tag; // inherited from CPInfo
    const u2 string_index;

public:
    CPStringInfo(u2 _string_index)
        : CPInfo(CONSTANT_String)
        , string_index(_string_index)
    {}
    CPStringInfo(ClassFile&);
    virtual ~CPStringInfo() {}

    virtual void Put(OutputBuffer& out) const
    {
        CPInfo::Put(out);
        out.PutU2(string_index);
    }

    virtual bool Check(const ConstantPool& constant_pool) const
    {
        return constant_pool[string_index] -> Tag() == CONSTANT_Utf8;
    }

    const char* Bytes(const ConstantPool& constant_pool) const
    {
        assert(constant_pool[string_index] -> Tag() == CONSTANT_Utf8);
        return ((const CPUtf8Info*) constant_pool[string_index]) -> Bytes();
    }
    u2 Length(const ConstantPool& constant_pool) const
    {
        assert(constant_pool[string_index] -> Tag() == CONSTANT_Utf8);
        return ((const CPUtf8Info*) constant_pool[string_index]) -> Length();
    }

#ifdef JIKES_DEBUG
    virtual void Print(Ostream& os, const ConstantPool& constant_pool) const;

    virtual void Describe(Ostream& os, const ConstantPool& constant_pool) const;
#endif // JIKES_DEBUG
};

} // Close namespace Jikes block


#endif // _INCLUDED
