#ifndef jcdt_class_CPNameAndTypeInfo_INCLUDED
#define jcdt_class_CPNameAndTypeInfo_INCLUDED
#include "CPInfo.inl"
#include "ConstantPool.inl"
#include "CPUtf8Info.inl"


namespace Jikes { // Open namespace Jikes block


//
// Describes a member name and its signature. JVMS 4.4.6.
//
class CPNameAndTypeInfo : public CPInfo
{
    // u1 tag; // inherited from CPInfo
    const u2 name_index;
    const u2 descriptor_index;

public:
    CPNameAndTypeInfo(u2 _name_index, u2 _descriptor_index)
        : CPInfo(CONSTANT_NameAndType)
        , name_index(_name_index)
        , descriptor_index(_descriptor_index)
    {}
    CPNameAndTypeInfo(ClassFile&);
    virtual ~CPNameAndTypeInfo() {}

    const char* Signature(const ConstantPool& constant_pool) const
    {
        assert(constant_pool[descriptor_index] -> Tag() ==
               CPInfo::CONSTANT_Utf8);
        return ((const CPUtf8Info*) constant_pool[descriptor_index]) ->
            Bytes();
    }
    u2 SignatureLength(const ConstantPool& constant_pool) const
    {
        assert(constant_pool[descriptor_index] -> Tag() ==
               CPInfo::CONSTANT_Utf8);
        return ((const CPUtf8Info*) constant_pool[descriptor_index]) ->
            Length();
    }

    virtual void Put(OutputBuffer& out) const
    {
        CPInfo::Put(out);
        out.PutU2(name_index);
        out.PutU2(descriptor_index);
    }

    virtual bool Check(const ConstantPool& constant_pool) const
    {
        return constant_pool[name_index] -> Tag() == CONSTANT_Utf8 &&
            constant_pool[descriptor_index] -> Tag() == CONSTANT_Utf8;
    }

#ifdef JIKES_DEBUG
    virtual void Print(Ostream& os, const ConstantPool& constant_pool) const;

    virtual void Describe(Ostream& os, const ConstantPool& constant_pool) const;
#endif // JIKES_DEBUG
};

} // Close namespace Jikes block


#endif // _INCLUDED
