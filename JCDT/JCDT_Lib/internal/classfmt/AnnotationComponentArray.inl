
#ifndef jcdt_class_AnnotationComponentArray_INCLUDED
#define jcdt_class_AnnotationComponentArray_INCLUDED
#include "AnnotationComponentValue.inl"


namespace Jikes { // Open namespace Jikes block


	//
// Represents array annotation values.
//
class AnnotationComponentArray : public AnnotationComponentValue
{
    // u1 tag; // inherited from AnnotationComponentValue
    // u2 num_values; // computed as values.Length()
    Tuple<AnnotationComponentValue*> values; // values[num_values]

    u2 len; // cache the length

public:
    AnnotationComponentArray()
        : AnnotationComponentValue(COMPONENT_array)
        , values(6, 16)
        , len(3) // +1 tag, +2 num_values
    {}
    AnnotationComponentArray(ClassFile&);
    virtual ~AnnotationComponentArray()
    {
        unsigned i = values.Length();
        while (i--)
            delete values[i];
    }

    u2 NumValues() const { return values.Length(); }

    void AddValue(AnnotationComponentValue* value)
    {
        values.Next() = value;
        len += value -> Length();
    }

    virtual u2 Length() const { return len; }

    virtual void Put(OutputBuffer& out) const
    {
        AnnotationComponentValue::Put(out);
        out.PutU2(values.Length());
        for (unsigned i = 0; i < values.Length(); i++)
            values[i] -> Put(out);
    }

#ifdef JIKES_DEBUG
    virtual void Print(Ostream& os,const ConstantPool& pool) const
    {
        os << '{';
        unsigned count = values.Length();
        if (count)
            values[0] -> Print(os,pool);
        for (unsigned i = 1; i < count; i++)
        {
            os << ", ";
            values[i] -> Print(os,pool);
        }
        os << '}';
    }
#endif // JIKES_DEBUG
};



} // Close namespace Jikes block


#endif // _INCLUDED

