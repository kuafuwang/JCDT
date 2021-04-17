
#ifndef jcdt_class_AnnotationComponentValue_INCLUDED
#define jcdt_class_AnnotationComponentValue_INCLUDED
#include <JCDT_Lib/internal/util/tuple.h>


namespace Jikes { // Open namespace Jikes block

//
// Describes an annotation value, as used inside AnnotationsAttribute,
// ParameterAnnotationsAttribute, and AnnotationDefaultAttribute. JSR 175
// section IX.
//
	class ConstantPool;
	class ClassFile;
class AnnotationComponentValue
{
public:
    enum AnnotationComponentValueTag
    {
        COMPONENT_byte = U_B,
        COMPONENT_char = U_C,
        COMPONENT_double = U_D,
        COMPONENT_float = U_F,
        COMPONENT_int = U_I,
        COMPONENT_long = U_J,
        COMPONENT_short = U_S,
        COMPONENT_boolean = U_Z,
        COMPONENT_string = U_s,
        COMPONENT_enum = U_e,
        COMPONENT_class = U_c,
        COMPONENT_annotation = U_AT,
        COMPONENT_array = U_LB
    };
protected:
    const AnnotationComponentValueTag tag;

public:
    AnnotationComponentValue(AnnotationComponentValueTag _tag)
        : tag(_tag)
    {}
    virtual ~AnnotationComponentValue() {}

    //
    // This reads the next component from the buffer, advancing it to the
    // end of the component. The user is responsible for deleting the result.
    //
    static AnnotationComponentValue* AllocateAnnotationComponentValue(ClassFile&);

    AnnotationComponentValueTag Tag() const { return tag; }

    // Subclasses must override.
    virtual u2 Length() const { return 1; } // +1 tag

    // Subclasses must override.
    virtual void Put(OutputBuffer& out) const
    {
        out.PutU1((u1) tag);
    }

#ifdef JIKES_DEBUG
    // Subclasses must override.
    virtual void Print(Ostream& os, const ConstantPool&) const
    {
        os << "<invalid tag:" << (unsigned) tag << '>';
    }
#endif // JIKES_DEBUG
};


	

} // Close namespace Jikes block


#endif // _INCLUDED

