
#ifndef jcdt_class_AnnotationComponentConstant_INCLUDED
#define jcdt_class_AnnotationComponentConstant_INCLUDED
#include "AnnotationComponentValue.inl"


namespace Jikes { // Open namespace Jikes block


	
//
// Represents boolean, byte, short, char, int, long, float, double, String,
// and Class annotation values.
//
class AnnotationComponentConstant : public AnnotationComponentValue
{
    // u1 tag; // inherited from AnnotationComponentValue
    // u2 const_value_index for boolean, byte, short, char, int, long, float,
    //    double, String,
    // u2 class_info_index for Class
    const u2 index;

public:
	AnnotationComponentConstant(AnnotationComponentValueTag _tag, u2 _index);
	AnnotationComponentConstant(ClassFile&, AnnotationComponentValueTag);
    virtual ~AnnotationComponentConstant() {}

    virtual u2 Length() const { return 3; } // +1 tag, +2 index

    virtual void Put(OutputBuffer& out) const
    {
        AnnotationComponentValue::Put(out);
        out.PutU2(index);
    }

#ifdef JIKES_DEBUG
	virtual void Print(Ostream& os,const ConstantPool& pool) const;
#endif // JIKES_DEBUG
};

	
} // Close namespace Jikes block


#endif // _INCLUDED

