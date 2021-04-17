#ifndef jcdt_class_CPClassInfo_INCLUDED
#define jcdt_class_CPClassInfo_INCLUDED
#include "CPInfo.inl"
#include "CPUtf8Info.inl"
#include "ConstantPool.inl"


namespace Jikes { // Open namespace Jikes block
	class TypeSymbol;

//
// Describes a class or interface name. JVMS 4.4.1.
//
class CPClassInfo : public CPInfo
{
    // u1 tag; // inherited from CPInfo
    const u2 name_index;

    TypeSymbol* type; // the associated type

public:
    CPClassInfo(u2 _name_index)
        : CPInfo(CONSTANT_Class)
        , name_index(_name_index)
        , type(NULL)
    {}
    CPClassInfo(ClassFile&);
    virtual ~CPClassInfo() {}

    virtual void Put(OutputBuffer& out) const
    {
        CPInfo::Put(out);
        out.PutU2(name_index);
    }

    virtual bool Check(const ConstantPool& constant_pool) const
    {
        return constant_pool[name_index] -> Tag() == CONSTANT_Utf8;
    }

    const char* TypeName(const ConstantPool& constant_pool) const
    {
        assert(constant_pool[name_index] -> Tag() == CONSTANT_Utf8);
        return ((const CPUtf8Info*) constant_pool[name_index]) -> Bytes();
    }
    u2 TypeNameLength(const ConstantPool& constant_pool) const
    {
        assert(constant_pool[name_index] -> Tag() == CONSTANT_Utf8);
        return ((const CPUtf8Info*) constant_pool[name_index]) -> Length();
    }

    void SetType(TypeSymbol* t)
    {
        assert(! type);
        type = t;
    }
    TypeSymbol* Type() { return type; }
    std::string getName(ConstantPool& constant_pool);
#ifdef JIKES_DEBUG
    virtual void Print(Ostream& os,const ConstantPool& constant_pool) const
    {
        os << "Class: name: " << (unsigned) name_index;
        if (constant_pool[name_index] -> Tag() == CONSTANT_Utf8)
        {
            os << "=\"";
            constant_pool[name_index] ->  Describe(os,constant_pool);
            os << '"';
        }
        else os << "(invalid)";
        os << endl;
    }

    virtual void Describe(Ostream& os,const ConstantPool& constant_pool) const
    {
        if (constant_pool[name_index] -> Tag() == CONSTANT_Utf8)
            constant_pool[name_index] ->  Describe(os,constant_pool);
        else os << "(invalid)";
    }
#endif // JIKES_DEBUG
};

} // Close namespace Jikes block


#endif // _INCLUDED
