
#ifndef jcdt_class_AnnotationComponentAnnotation_INCLUDED
#define jcdt_class_AnnotationComponentAnnotation_INCLUDED
#include "AnnotationComponentValue.inl"



namespace Jikes { // Open namespace Jikes block

//
// Represents nested annotation values.
//
	class Annotation;
class AnnotationComponentAnnotation : public AnnotationComponentValue
{
    // u1 tag; // inherited from AnnotationComponentValue
    Annotation* attr_value;

public:
    AnnotationComponentAnnotation(Annotation* nested)
        : AnnotationComponentValue(COMPONENT_annotation)
        , attr_value(nested)
    {}
    AnnotationComponentAnnotation(ClassFile&);
    virtual ~AnnotationComponentAnnotation();

    virtual u2 Length() const;

    virtual void Put(OutputBuffer&) const;

#ifdef JIKES_DEBUG
    virtual void Print(Ostream&,const ConstantPool&) ;
#endif // JIKES_DEBUG

};


	

} // Close namespace Jikes block


#endif // _INCLUDED

