
#include <JCDT_Lib/internal/classfmt/ClassFile.inl>

namespace Jikes { // Open namespace Jikes block

	SignatureAttribute::SignatureAttribute(ClassFile& buffer)
		: AttributeInfo(ATTRIBUTE_Signature, buffer)
		, signature_index(buffer.GetU2())
	{
		if (attribute_length != 2)
			buffer.MarkInvalid("bad signature attribute length");
		if (buffer.Pool()[signature_index]->Tag() != CPInfo::CONSTANT_Utf8)
			buffer.MarkInvalid("bad type for signature attribute");
	}

	void SignatureAttribute::Print(Ostream& os, const ConstantPool& constant_pool, int fill)
	{
		assert(! fill);
		PrintPrefix(os, "Signature", constant_pool, 0);
		os << endl << " signature: " << (unsigned)signature_index;
		if (constant_pool[signature_index]->Tag() == CPInfo::CONSTANT_Utf8)
		{
			os << '=';
			constant_pool[signature_index]->Describe(os, constant_pool);
		}
		else os << "(invalid)";
		os << endl;
	}
} // Close namespace Jikes block



