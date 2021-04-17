
#ifndef AnnotationComponentEnumAnnotationComponentEnum_INCLUDED
#define AnnotationComponentEnumAnnotationComponentEnum_INCLUDED
#include "AnnotationComponentValue.inl"
#include "ConstantPool.inl"


namespace Jikes { // Open namespace Jikes block


//
// Represents enum constant annotation values.
//
class AnnotationComponentEnum : public AnnotationComponentValue
{
    // u1 tag; // inherited from AnnotationComponentValue
    const u2 type_name_index;
    const u2 const_name_index;

public:
    AnnotationComponentEnum(u2 type, u2 name)
        : AnnotationComponentValue(COMPONENT_enum)
        , type_name_index(type)
        , const_name_index(name)
    {}
    AnnotationComponentEnum(ClassFile&);
    virtual ~AnnotationComponentEnum() {}

    virtual u2 Length() const { return 5; } // +1 tag, +2 type, +2 name

    virtual void Put(OutputBuffer& out) const
    {
        AnnotationComponentValue::Put(out);
        out.PutU2(type_name_index);
        out.PutU2(const_name_index);
    }

#ifdef JIKES_DEBUG
    virtual void Print(Ostream& os,const ConstantPool& pool) const
    {
        os << (unsigned) type_name_index << '.'
                << (unsigned) const_name_index << '=';
        pool[type_name_index] -> Describe(os,pool);
        if (pool[type_name_index] -> Tag() != CPInfo::CONSTANT_Class)
            os << "(invalid)";
        os << '.';
        pool[const_name_index] -> Describe(os,pool);
        if (pool[type_name_index] -> Tag() != CPInfo::CONSTANT_Utf8)
            os << "(invalid)";
    }
#endif // JIKES_DEBUG
};


	

} // Close namespace Jikes block


#endif // _INCLUDED

