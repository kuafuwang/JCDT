
#ifndef jcdt_class_Annotation_INCLUDED
#define jcdt_class_Annotation_INCLUDED
#include "AnnotationComponentValue.inl"
#include "ConstantPool.inl"


namespace Jikes { // Open namespace Jikes block

//
// Describes an annotation, as used inside AnnotationComponentValue,
// ParameterAnnotationsAttribute, and AnnotationDefaultAttribute. JSR 175
// section IX.
//
class Annotation
{
    u2 type_index;
    // u2 num_components; // computed as components.Length()
    struct Component
    {
        u2 component_name_index;
        AnnotationComponentValue* component_value;
    };
    Tuple<Component> components; // components[num_components]

public:
    Annotation(u2 index)
        : type_index(index)
        , components(6, 16)
    {}
    Annotation(ClassFile&);
    ~Annotation()
    {
        unsigned i = components.Length();
        while (i--)
            delete components[i].component_value;
    }

    u2 Length() const
    {
        u2 size = 4; // +2 type_index, +2 num_components
        unsigned i = components.Length();
        while (i--)
            // +2 member_name_index
            size += 2 + components[i].component_value -> Length();
        return size;
    }

    u1 NumComponents() const { return components.Length(); }

    void AddComponent(u2 name_index, AnnotationComponentValue* value)
    {
        Component& component = components.Next();
        component.component_name_index = name_index;
        component.component_value = value;
    }

    void Put(OutputBuffer& out) const
    {
        out.PutU2(type_index);
        unsigned i = components.Length();
        out.PutU1(i);
        while (i--)
        {
            out.PutU2(components[i].component_name_index);
            components[i].component_value -> Put(out);
        }
    }

#ifdef JIKES_DEBUG
    void Print(Ostream& os,const ConstantPool& pool) const
    {
        os << (unsigned) type_index << "=@";
        pool[type_index] -> Describe(os,pool);
        unsigned num_components = components.Length();
        os << ", length:" << num_components << '(';
        if (num_components)
        {
            os << (unsigned) components[0].component_name_index << ':';
            pool[components[0].component_name_index] -> Describe(os,pool);
            os << '=';
            components[0].component_value -> Print(os,pool);
        }
        for (unsigned i = 1; i < num_components; i++)
        {
            os << ", " << (unsigned) components[i].component_name_index
                    << ':';
            pool[components[i].component_name_index] -> Describe(os,pool);
            os << '=';
            components[i].component_value -> Print(os,pool);
        }
        os << ')';
    }
#endif // JIKES_DEBUG
};


	

} // Close namespace Jikes block


#endif // _INCLUDED

