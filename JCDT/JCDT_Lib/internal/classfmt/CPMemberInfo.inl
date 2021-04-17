#ifndef jcdt_class_CPMemberInfo_INCLUDED
#define jcdt_class_CPMemberInfo_INCLUDED
#include <JCDT_Lib/internal/classfmt/CPInfo.inl>
#include "ConstantPool.inl"

namespace Jikes { // Open namespace Jikes block


//
// Describes a field or method's declaring class, name, and type. JVMS 4.4.2.
//
class CPMemberInfo : public CPInfo
{
    // u1 tag; // inherited from CPInfo
    const u2 class_index;
    const u2 name_and_type_index;

public:
    CPMemberInfo(ConstantPoolTag _tag, u2 _class_index,
                 u2 _name_and_type_index)
        : CPInfo(_tag)
        , class_index(_class_index)
        , name_and_type_index(_name_and_type_index)
    {}
    CPMemberInfo(ConstantPoolTag, ClassFile&);
    virtual ~CPMemberInfo() {}

    virtual void Put(OutputBuffer& out) const
    {
        CPInfo::Put(out);
        out.PutU2(class_index);
        out.PutU2(name_and_type_index);
    }

    virtual bool Check(const ConstantPool& constant_pool) const
    {
        return constant_pool[class_index] -> Tag() == CONSTANT_Class &&
            (constant_pool[name_and_type_index] -> Tag() ==
             CONSTANT_NameAndType);
    }

#ifdef JIKES_DEBUG
    virtual void Print(Ostream& os,const ConstantPool& constant_pool) const
    {
        switch (tag)
        {
        case CONSTANT_Fieldref:
            os << "Field";
            break;
        case CONSTANT_InterfaceMethodref:
            os << "Interface";
            // fallthrough
        case CONSTANT_Methodref:
            os << "Method";
            break;
        default:
            assert(false && "bad constant pool member tag");
        }
        os << ": class: " << (unsigned) class_index;
        if (constant_pool[class_index] -> Tag() == CONSTANT_Class)
        {
            os << '=';
            constant_pool[class_index] ->  Describe(os,constant_pool);
        }
        else os << "(invalid)";
        os << endl << "        name_and_type: "
                << (unsigned) name_and_type_index;
        if (constant_pool[name_and_type_index] -> Tag() ==
            CONSTANT_NameAndType)
        {
            os << "=<";
            constant_pool[name_and_type_index] ->  Describe(os,constant_pool);
            os << ">";
        }
        else os << "(invalid)";
        os << endl;
    }

    virtual void Describe(Ostream& os,const ConstantPool& constant_pool) const
    {
        if (constant_pool[class_index] -> Tag() == CONSTANT_Class)
            constant_pool[class_index] ->  Describe(os,constant_pool);
        else os << "(invalid)";
        os << ".";
        if (constant_pool[name_and_type_index] -> Tag() ==
            CONSTANT_NameAndType)
        {
            constant_pool[name_and_type_index] ->  Describe(os,constant_pool);
        }
        else os << "(invalid)";
    }
#endif // JIKES_DEBUG
};


} // Close namespace Jikes block


#endif // _INCLUDED
