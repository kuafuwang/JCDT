
#include <JCDT_Lib/internal/classfmt/StackMapAttribute.inl>
#include <JCDT_Lib/internal/classfmt/ClassFile.inl>

namespace Jikes { // Open namespace Jikes block


	void StackMapAttribute::StackMapFrame::VerificationTypeInfo::Read(ClassFile& buffer)
	{
		int type = buffer.GetU1();
		tag = (VerificationTypeInfoTag)type;
		if (type < TYPE_Top || type > TYPE_Uninitialized)
		{
			type = TYPE_Top;
			buffer.MarkInvalid("bad stack map type");
		}
		if (type >= TYPE_Object)
		{
			info = buffer.GetU2();
			if (type == TYPE_Object &&
				buffer.Pool()[info]->Tag() != CPInfo::CONSTANT_Class)
			{
				buffer.MarkInvalid("bad stack map info");
			}
		}
	}

	StackMapAttribute::StackMapFrame::StackMapFrame(ClassFile& buffer)
		: offset(buffer.GetU2())
		, locals(6, 16)
		, stack(6, 16)
		, frame_size(6)
		// +2 for offset, +2 for locals_size, +2 for stack_size
	{
		unsigned count = buffer.GetU2();
		while (count--)
		{
			unsigned index = locals.NextIndex();
			locals[index].Read(buffer);
			frame_size += locals[index].Size();
		}
		count = buffer.GetU2();
		while (count--)
		{
			unsigned index = stack.NextIndex();
			stack[index].Read(buffer);
			frame_size += stack[index].Size();
		}
	}

	StackMapAttribute::StackMapAttribute(ClassFile& buffer)
		: AttributeInfo(ATTRIBUTE_StackMap, buffer)
		, frames(6, 16)
	{
		unsigned remaining = attribute_length - 2; // -2 for frame_count
		unsigned count = buffer.GetU2();
		while (count--)
		{
			unsigned index = frames.NextIndex();
			frames[index] = new StackMapFrame(buffer);
			remaining -= frames[index]->FrameSize();
		}
		if (remaining)
			buffer.MarkInvalid("bytes remaining at end of stack map attribute");
	}


	

} // Close namespace Jikes block



