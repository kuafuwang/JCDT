
#ifndef jcdt_class_StackMapAttribute_INCLUDED
#define jcdt_class_StackMapAttribute_INCLUDED
#include "AttributeInfo.inl"


namespace Jikes { // Open namespace Jikes block


//
// Valid for code, this speeds up verification by listing the types of all
// locals and stack elements at the start of each basic block. JSR 139
// appendix 1 section 2.1 (or JSR 30 section 5.3.1.2).
//
class StackMapAttribute : public AttributeInfo
{
public:
    class StackMapFrame
    {
        class VerificationTypeInfo
        {
        public:
            enum VerificationTypeInfoTag
            {
                TYPE_Top = 0, // also called Bogus
                TYPE_Integer = 1,
                TYPE_Float = 2,
                TYPE_Long = 4,
                TYPE_Double = 3,
                TYPE_Null = 5,
                TYPE_UninitializedThis = 6,
                TYPE_Object = 7,
                TYPE_Uninitialized = 8
            };

        private:
            VerificationTypeInfoTag tag;
            // u2 cpool_index for Object,
            // u2 offset for Unitialized
            // unused for remaining types
            u2 info;

        public:
            VerificationTypeInfo()
                : tag(TYPE_Top)
            {}
            void Read(ClassFile&);

            u2 Size() const { return tag >= TYPE_Object ? 3 : 1; }

            void Put(OutputBuffer& out) const
            {
                out.PutU1((u1) tag);
                if (tag >= TYPE_Object)
                    out.PutU2(info);
            }

#ifdef JIKES_DEBUG
            void Print(Ostream& os,const ConstantPool& constant_pool,
                       unsigned& index) 
            {
                os << index++ << ':';
                switch (tag)
                {
                case TYPE_Top:
                    os << "void";
                    break;
                case TYPE_Integer:
                    os << "int";
                    break;
                case TYPE_Float:
                    os << "float";
                    break;
                case TYPE_Long:
                    os << "long";
                    index++;
                    break;
                case TYPE_Double:
                    os << "double";
                    index++;
                    break;
                case TYPE_Null:
                    os << "null";
                    break;
                case TYPE_UninitializedThis:
                    os << "unitialized this";
                    break;
                case TYPE_Object:
                    os << (unsigned) info;
                    if (constant_pool[info] -> Tag() == CPInfo::CONSTANT_Class)
                    {
                        os << '=';
                        constant_pool[info] ->  Describe(os,constant_pool);
                    }
                    else os << "(invalid)";
                    break;
                case TYPE_Uninitialized:
                    os << "unitialized since " << (unsigned) info;
                    break;
                default:
                    assert(false && "invalid VerificationTypeInfoTag");
                }
            }
#endif // JIKES_DEBUG
        };

    private:
        u2 offset;
        // u2 locals_size; // computed as locals.Length()
        Tuple<VerificationTypeInfo> locals;
        // u2 stack_size; // computed as stack.Length()
        Tuple<VerificationTypeInfo> stack;

        u2 frame_size; // needed since VerificationTypeInfo is variable size

    public:
        StackMapFrame(u2 _offset)
            : offset(_offset)
            , locals(6, 16)
            , stack(6, 16)
            , frame_size(6)
            // +2 for offset, +2 for locals_size, +2 for stack_size
        {}
        StackMapFrame(ClassFile&);

        // Note that LocalsSize() + StackSize() + 6 <= FrameSize()
        u2 LocalsSize() const { return locals.Length(); }
        u2 StackSize() const { return stack.Length(); }
        u2 FrameSize() const { return frame_size; }

        void AddLocal(const VerificationTypeInfo& entry)
        {
            frame_size += entry.Size();
            locals.Next() = entry;
        }
        void AddStack(const VerificationTypeInfo& entry)
        {
            frame_size += entry.Size();
            stack.Next() = entry;
        }

        void Put(OutputBuffer& out) const
        {
            out.PutU2(offset);
            out.PutU2(locals.Length());
            for (unsigned i = 0; i < locals.Length(); i++)
                locals[i].Put(out);
            out.PutU2(stack.Length());
            for (unsigned j = 0; j < stack.Length(); j++)
                stack[j].Put(out);
        }

#ifdef JIKES_DEBUG
        void Print(Ostream& os, const ConstantPool& constant_pool) 
        {
            os << "    pc: " << (unsigned) offset << ", locals_size: "
                    << (unsigned) locals.Length() << ", stack_size: "
                    << (unsigned) stack.Length() << endl;
            unsigned index = 0;
            if (locals.Length())
            {
                os << "      locals: ";
                locals[0].Print(os,constant_pool, index);
                for (unsigned i = 1; i < locals.Length(); i++)
                {
                    os << ", ";
                    locals[i].Print(os,constant_pool, index);
                }
                os << endl;
            }
            index = 0;
            if (stack.Length())
            {
                os << "      stack: ";
                stack[0].Print(os,constant_pool, index);
                for (unsigned i = 1; i < stack.Length(); i++)
                {
                    os << ", ";
                    stack[i].Print(os,constant_pool, index);
                }
                os << endl;
            }
        }
#endif // JIKES_DEBUG
    };

private:
    // u2 attribute_name_index; // inherited from AttributeInfo
    // u4 attribute_length; // inherited from AttributeInfo
    // u2 frame_count; // computed as frames.Length();
    Tuple<StackMapFrame*> frames; // stack_map_frame[frame_count]

public:
    StackMapAttribute(u2 name)
        : AttributeInfo(ATTRIBUTE_StackMap, name, 2)
        // +2 for frame_count
        , frames(6, 16)
    {}
    StackMapAttribute(ClassFile&);
    virtual ~StackMapAttribute()
    {
        for (unsigned i = 0; i < frames.Length(); i++)
            delete frames[i];
    }

    u2 FrameCount() const { return frames.Length(); }

    void AddFrame(StackMapFrame* frame)
    {
        frames.Next() = frame;
        attribute_length += frame -> FrameSize();
    }

    virtual void Put(OutputBuffer& out) const
    {
        AttributeInfo::Put(out);
        out.PutU2(frames.Length());
        for (unsigned i = 0; i < frames.Length(); i++)
            frames[i] -> Put(out);
    }

#ifdef JIKES_DEBUG
    virtual void Print(Ostream& os,const ConstantPool& constant_pool, int fill = 2) 
    {
        assert(fill == 2);
        PrintPrefix(os,"StackMap", constant_pool, 2);
        os << ", frames: "
                << (unsigned) frames.Length() << endl;
        for (unsigned i = 0; i < frames.Length(); i++)
            frames[i] -> Print(os,constant_pool);
    }
#endif // JIKES_DEBUG
};


	

} // Close namespace Jikes block


#endif // _INCLUDED

