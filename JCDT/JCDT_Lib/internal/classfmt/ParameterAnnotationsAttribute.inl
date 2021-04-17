
#ifndef jcdt_class_ParameterAnnotationsAttribute_INCLUDED
#define jcdt_class_ParameterAnnotationsAttribute_INCLUDED
#include "AttributeInfo.inl"
#include "Annotation.inl"


namespace Jikes { // Open namespace Jikes block


//
// Valid on all methods, this lists the RuntimeVisible and RuntimeInvisible
// annotations tied to each parameter. JSR 175, section IX.
//
class ParameterAnnotationsAttribute : public AttributeInfo
{
    // u2 attribute_name_index; // inherited from AttributeInfo
    // u4 attribute_length; // inherited from AttributeInfo
    const u1 num_parameters;
    // u2 num_annotations; // computed as parameters[i].Length()
    // annotation annotations[num_annotations]; // computed as parameters[i]
    // parameter_annotations[num_parameters]
    Tuple<Annotation*>* parameters;

public:
    ParameterAnnotationsAttribute(u2 _name_index, bool visible, u1 params)
        : AttributeInfo((visible ? ATTRIBUTE_RuntimeVisibleParameterAnnotations
                         : ATTRIBUTE_RuntimeInvisibleParameterAnnotations),
                        _name_index, 1 + 2 * params)
        // +1 num_parameters, +2 num_annotations (num_parameters times)
        , num_parameters(params)
        , parameters(NULL)
    {
        if (params)
            parameters = new Tuple<Annotation*>[params];
    }
    ParameterAnnotationsAttribute(ClassFile&, bool visible);
    virtual ~ParameterAnnotationsAttribute()
    {
        for (unsigned i = num_parameters; i--; )
            for (unsigned j = parameters[i].Length(); j--; )
                delete parameters[i][j];
        delete [] parameters;
    }

    u1 NumParameters() const { return num_parameters; }
    u2 NumAnnotations(u1 param) const
    {
        assert(param < num_parameters);
        return parameters[param].Length();
    }

    void AddAnnotation(Annotation* annotation, u1 param)
    {
        assert(param < num_parameters);
        parameters[param].Next() = annotation;
        attribute_length += annotation -> Length();
    }

    virtual void Put(OutputBuffer& out) const
    {
        AttributeInfo::Put(out);
        out.PutU1(num_parameters);
        for (unsigned i = 0; i < num_parameters; i++)
        {
            out.PutU2(parameters[i].Length());
            for (unsigned j = 0; j < parameters[i].Length(); j++)
                parameters[i][j] -> Put(out);
        }
    }

#ifdef JIKES_DEBUG
    virtual void Print(Ostream& os,const ConstantPool& constant_pool, int fill = 0) 
    {
        assert(! fill);
        PrintPrefix(os,(Tag() == ATTRIBUTE_RuntimeVisibleParameterAnnotations
                     ? "RuntimeVisibleParameterAnnotations"
                     : "RuntimeInvisibleParameterAnnotations"),
                    constant_pool, 0);
        os << ", num_parameters: " << (unsigned) num_parameters << endl;
        for (unsigned i = 0; i < num_parameters; i++)
        {
            os << " param " << i << ": num_annotations: "
                    << (unsigned) parameters[i].Length() << endl;
            for (unsigned j = 0; j < parameters[i].Length(); j++)
            {
                os << "  ";
                parameters[i][j] -> Print(os,constant_pool);
                os << endl;
            }
        }
    }
#endif // JIKES_DEBUG
};


	

} // Close namespace Jikes block


#endif // _INCLUDED

