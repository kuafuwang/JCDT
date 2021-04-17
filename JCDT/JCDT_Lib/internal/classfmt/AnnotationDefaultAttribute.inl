
#ifndef jcdt_class_AnnotationDefaultAttribute_INCLUDED
#define jcdt_class_AnnotationDefaultAttribute_INCLUDED
#include "AttributeInfo.inl"
#include "AnnotationComponentValue.inl"


namespace Jikes { // Open namespace Jikes block


//
// Valid only on method members of annotation types, this lists the default
// return value for the annotation method. JSR 175, section IX.
//
class AnnotationDefaultAttribute : public AttributeInfo
{
    // u2 attribute_name_index; // inherited from AttributeInfo
    // u4 attribute_length; // inherited from AttributeInfo
    AnnotationComponentValue* default_value;

public:
    AnnotationDefaultAttribute(u2 _name_index, AnnotationComponentValue* value)
        : AttributeInfo(ATTRIBUTE_AnnotationDefault, _name_index,
                        value -> Length())
        , default_value(value)
    {}
    AnnotationDefaultAttribute(ClassFile&);
    virtual ~AnnotationDefaultAttribute() { delete default_value; }

    const AnnotationComponentValue* DefaultValue() const
    {
        return default_value;
    }

    virtual void Put(OutputBuffer& out) const
    {
        AttributeInfo::Put(out);
        default_value -> Put(out);
    }

#ifdef JIKES_DEBUG
    virtual void Print(Ostream& os,const ConstantPool& constant_pool, int fill = 0) 
    {
        assert(! fill);
        PrintPrefix(os,"AnnotationDefault", constant_pool, 0);
        os << ", ";
        default_value -> Print(os,constant_pool);
        os << endl;
    }
#endif // JIKES_DEBUG
};


	

} // Close namespace Jikes block


#endif // _INCLUDED

