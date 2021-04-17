
#include <JCDT_Lib/internal/classfmt/AnnotationComponentConstant.inl>
#include <JCDT_Lib/internal/classfmt/ClassFile.inl>
#include <JCDT_Lib/internal/classfmt/CPIntegerInfo.inl>

namespace Jikes { // Open namespace Jikes block


	AnnotationComponentConstant::AnnotationComponentConstant(AnnotationComponentValueTag _tag, u2 _index): AnnotationComponentValue(_tag)
	                                                                                                       , index(_index)
	{
		assert(tag != COMPONENT_annotation && tag != COMPONENT_array &&
			tag != COMPONENT_enum);
	}
	inline void AnnotationComponentConstant::Print(Ostream& os,const ConstantPool& pool) const
	{
		os << (unsigned)index << '=';
		pool[index]->Describe(os,pool);
		switch (tag)
		{
		case COMPONENT_boolean:
			if (pool[index]->Tag() == CPInfo::CONSTANT_Integer)
			{
				i4 value = ((CPIntegerInfo*)pool[index])->Value();
				if (value == 0)
					os << "(false)";
				else if (value == 0)
					os << "(true)";
				else os << "(invalid)";
			}
			else os << "(invalid)";
			break;
		case COMPONENT_char:
			if (pool[index]->Tag() == CPInfo::CONSTANT_Integer)
			{
				i4 value = ((CPIntegerInfo*)pool[index])->Value();
				if (value == '\n')
					os << "('\\n')";
				else if (value == '\r')
					os << "('\\r')";
				else if (value == '\'')
					os << "('\\'')";
				else if (value == '\\')
					os << "('\\\\')";
				else if (value > 0x1f && value < 0x7f)
					os << "('" << (char)value << "')";
				else if (value >= 0 && value <= 0xffff)
				{
					os << "('\\u" << IntToString(value, 4).String()
						<< "')";
				}
				else os << "(invalid)";
			}
			else os << "(invalid)";
			break;
		case COMPONENT_byte:
			if (pool[index]->Tag() == CPInfo::CONSTANT_Integer)
			{
				i4 value = ((CPIntegerInfo*)pool[index])->Value();
				if (value < -128 || value > 127)
					os << "(invalid)";
			}
			else os << "(invalid)";
			break;
		case COMPONENT_short:
			if (pool[index]->Tag() == CPInfo::CONSTANT_Integer)
			{
				i4 value = ((CPIntegerInfo*)pool[index])->Value();
				if (value < -32768 || value > 32767)
					os << "(invalid)";
			}
			else os << "(invalid)";
			break;
		case COMPONENT_int:
			if (pool[index]->Tag() != CPInfo::CONSTANT_Integer)
				os << "(invalid)";
			break;
		case COMPONENT_long:
			if (pool[index]->Tag() != CPInfo::CONSTANT_Long)
				os << "(invalid)";
			break;
		case COMPONENT_float:
			if (pool[index]->Tag() != CPInfo::CONSTANT_Float)
				os << "(invalid)";
			break;
		case COMPONENT_double:
			if (pool[index]->Tag() != CPInfo::CONSTANT_Double)
				os << "(invalid)";
			break;
		case COMPONENT_string:
			if (pool[index]->Tag() != CPInfo::CONSTANT_String)
				os << "(invalid)";
			break;
		case COMPONENT_class:
			if (pool[index]->Tag() != CPInfo::CONSTANT_Class)
				os << "(invalid)";
			break;
		default:
			assert(false && "invalid constant-valued attribute");
		}
	}
	AnnotationComponentConstant::AnnotationComponentConstant(ClassFile& buffer,
		AnnotationComponentValueTag _tag)
		: AnnotationComponentValue(_tag)
		, index(buffer.GetU2())
	{
		switch (tag)
		{
		case COMPONENT_boolean: case COMPONENT_byte: case COMPONENT_char:
		case COMPONENT_short: case COMPONENT_int:
			if (buffer.Pool()[index]->Tag() != CPInfo::CONSTANT_Integer)
				buffer.MarkInvalid("bad int-like annotation constant");
			break;
		case COMPONENT_long:
			if (buffer.Pool()[index]->Tag() != CPInfo::CONSTANT_Long)
				buffer.MarkInvalid("bad long annotation constant");
			break;
		case COMPONENT_float:
			if (buffer.Pool()[index]->Tag() != CPInfo::CONSTANT_Float)
				buffer.MarkInvalid("bad float annotation constant");
			break;
		case COMPONENT_double:
			if (buffer.Pool()[index]->Tag() != CPInfo::CONSTANT_Double)
				buffer.MarkInvalid("bad double annotation constant");
			break;
		case COMPONENT_string:
			if (buffer.Pool()[index]->Tag() != CPInfo::CONSTANT_String)
				buffer.MarkInvalid("bad string annotation constant");
			break;
		case COMPONENT_class:
			if (buffer.Pool()[index]->Tag() != CPInfo::CONSTANT_Class)
				buffer.MarkInvalid("bad class annotation constant");
			break;
		default:
			assert(false && "invalid annotation constant");
		}
	}


} // Close namespace Jikes block



