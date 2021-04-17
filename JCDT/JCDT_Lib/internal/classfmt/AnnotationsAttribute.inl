
#ifndef jcdt_class_AnnotationsAttribute_INCLUDED
#define jcdt_class_AnnotationsAttribute_INCLUDED
#include "AttributeInfo.inl"
#include "Annotation.inl"


namespace Jikes { // Open namespace Jikes block


//
// Valid on all classes, fields, and methods, this lists the RuntimeVisible
// and RuntimeInvisible annotations tied to the item. JSR 175, section IX.
//
class AnnotationsAttribute : public AttributeInfo
{
    // u2 attribute_name_index; // inherited from AttributeInfo
    // u4 attribute_length; // inherited from AttributeInfo
    // u2 num_annotations; // computed as annotations.Length()
    Tuple<Annotation*> annotations; // annotations[num_annotations]

public:
    AnnotationsAttribute(u2 _name_index, bool visible)
        : AttributeInfo((visible ? ATTRIBUTE_RuntimeVisibleAnnotations
                         : ATTRIBUTE_RuntimeInvisibleAnnotations),
                        _name_index, 2)
        // +2 for num_annotations
        , annotations(6, 16)
    {}
    AnnotationsAttribute(ClassFile&, bool visible);
    virtual ~AnnotationsAttribute()
    {
        unsigned i = annotations.Length();
        while (i--)
            delete annotations[i];
    }

    u2 NumAnnotations() const { return annotations.Length(); }

    void AddAnnotation(Annotation* annotation)
    {
        annotations.Next() = annotation;
        attribute_length += annotation -> Length();
    }

    virtual void Put(OutputBuffer& out) const
    {
        AttributeInfo::Put(out);
        out.PutU2(annotations.Length());
        for (unsigned i = 0; i < annotations.Length(); i++)
            annotations[i] -> Put(out);
    }

#ifdef JIKES_DEBUG
    virtual void Print(Ostream& os,const ConstantPool& constant_pool, int fill = 0) 
    {
        assert(! fill);
        PrintPrefix(os,(Tag() == ATTRIBUTE_RuntimeVisibleAnnotations
                     ? "RuntimeVisibleAnnotations"
                     : "RuntimeInvisibleAnnotations"), constant_pool, 0);
        os << ", num_annotations: " << (unsigned) annotations.Length()
                << endl;
        for (unsigned i = 0; i < annotations.Length(); i++)
        {
            os << "  ";
            annotations[i] -> Print(os,constant_pool);
            os << endl;
        }
    }
#endif // JIKES_DEBUG
};


	

} // Close namespace Jikes block


#endif // _INCLUDED

