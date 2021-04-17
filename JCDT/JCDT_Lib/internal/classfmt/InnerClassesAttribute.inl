
#ifndef jct_class_InnerClassesAttribute_INCLUDED
#define jct_class_InnerClassesAttribute_INCLUDED
#include "AttributeInfo.inl"
#include <JCDT_Lib/internal/util/AccessFlagsUtil.h>
#include "CPClassInfo.inl"


namespace Jikes { // Open namespace Jikes block
	
//
// Valid only on classes, this lists information about all nested classes
// mentioned in the constant pool. JVMS 4.7.5.
//
class InnerClassesAttribute : public AttributeInfo
{
    // u2 attribute_name_index; // inherited from AttributeInfo
    // u4 attribute_length; // inherited from AttributeInfo
    // u2 number_of_classes; // computed as classes.Length()
    struct InnerClassesElement
    {
        u2 inner_class_info_index;
        u2 outer_class_info_index;
        u2 inner_name_index;
        AccessFlags inner_class_access_flags; // u2 inner_class_access_flags;
    };
    Tuple<InnerClassesElement> classes; // classes[number_of_classes]

public:
    InnerClassesAttribute(u2 _name_index)
        : AttributeInfo(ATTRIBUTE_InnerClasses, _name_index, 2)
        // +2 for number_of_classes
        , classes(6, 16)
    {}
    InnerClassesAttribute(ClassFile&);
    virtual ~InnerClassesAttribute() {}

    u2 InnerClassesLength() const { return classes.Length();}

    void AddInnerClass(u2 inner_class_info_index, u2 outer_class_info_index,
                       u2 inner_name_index, AccessFlags inner_class_flags)
    {
        attribute_length += 8;
        InnerClassesElement& entry = classes.Next();
        entry.inner_class_info_index = inner_class_info_index;
        entry.outer_class_info_index = outer_class_info_index;
        entry.inner_name_index = inner_name_index;
        entry.inner_class_access_flags.SetFlags(inner_class_flags);
    }

    const CPClassInfo* Inner(u2 i, const ConstantPool& constant_pool) const
    {
        assert(constant_pool[classes[i].inner_class_info_index] -> Tag() ==
               CPInfo::CONSTANT_Class);
        return (const CPClassInfo*)
            constant_pool[classes[i].inner_class_info_index];
    }
    const CPClassInfo* Outer(u2 i, const ConstantPool& constant_pool) const
    {
        assert(! classes[i].outer_class_info_index ||
               (constant_pool[classes[i].outer_class_info_index] -> Tag() ==
                CPInfo::CONSTANT_Class));
        return classes[i].outer_class_info_index
            ? ((const CPClassInfo*)
               constant_pool[classes[i].outer_class_info_index])
            : (const CPClassInfo*) NULL;
    }
    const char* Name(u2 i, const ConstantPool& constant_pool) const
    {
        assert(! classes[i].inner_name_index ||
               (constant_pool[classes[i].inner_name_index] -> Tag() ==
                CPInfo::CONSTANT_Utf8));
        if (! classes[i].inner_name_index)
            return NULL;
        return ((const CPUtf8Info*)
                constant_pool[classes[i].inner_name_index]) -> Bytes();
    }
    u2 NameLength(u2 i, const ConstantPool& constant_pool) const
    {
        assert(! classes[i].inner_name_index ||
               (constant_pool[classes[i].inner_name_index] -> Tag() ==
                CPInfo::CONSTANT_Utf8));
        return classes[i].inner_name_index
            ? ((const CPUtf8Info*)
               constant_pool[classes[i].inner_name_index]) -> Length() : 0;
    }
    const AccessFlags& Flags(u2 i) const
    {
        return classes[i].inner_class_access_flags;
    }

    virtual void Put(OutputBuffer& out) const
    {
        AttributeInfo::Put(out);
        out.PutU2(classes.Length());
        for (unsigned i = 0; i < classes.Length(); i++)
        {
            out.PutU2(classes[i].inner_class_info_index);
            out.PutU2(classes[i].outer_class_info_index);
            out.PutU2(classes[i].inner_name_index);
            out.PutU2(classes[i].inner_class_access_flags.Flags());
        }
    }

#ifdef JIKES_DEBUG
    virtual void Print(Ostream& os,const ConstantPool& constant_pool, int fill = 0) 
    {
        assert(! fill);
        PrintPrefix(os,"InnerClasses", constant_pool, 0);
        os << ", count: " << (unsigned) classes.Length() << endl;
        for (unsigned i = 0; i < classes.Length(); i++)
        {
            os << "  inner: "
                    << (unsigned) classes[i].inner_class_info_index;
            if (constant_pool[classes[i].inner_class_info_index] -> Tag() ==
                CPInfo::CONSTANT_Class)
            {
                os << '=';
                constant_pool[classes[i].inner_class_info_index] ->
                     Describe(os,constant_pool);
            }
            else os << "(invalid)";
            os << ", outer: "
                    << (unsigned) classes[i].outer_class_info_index;
            if (! classes[i].outer_class_info_index)
                os << "(inner is local)";
            else if (constant_pool[classes[i].outer_class_info_index] ->
                     Tag() == CPInfo::CONSTANT_Class)
            {
                os << '=';
                constant_pool[classes[i].outer_class_info_index] ->
                     Describe(os,constant_pool);
            }
            else os << "(invalid)";
            os << endl << "   name: "
                    << (unsigned) classes[i].inner_name_index;
            if (! classes[i].inner_name_index)
                os << "(anonymous)";
            else if (constant_pool[classes[i].inner_name_index] -> Tag() ==
                     CPInfo::CONSTANT_Utf8)
            {
                os << '=';
                constant_pool[classes[i].inner_name_index] ->
                     Describe(os,constant_pool);
            }
            else os << "(invalid)";
            os << ", ";
            classes[i].inner_class_access_flags.
                Print(os,AccessFlags::ACCESS_TYPE);
        }
    }
#endif // JIKES_DEBUG
};


	

} // Close namespace Jikes block


#endif // _INCLUDED

