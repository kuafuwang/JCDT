// $Id: class.cpp,v 1.17 2004/12/12 04:35:52 elliott-oss Exp $
//
// This software is subject to the terms of the IBM Jikes Compiler
#include "stdafx.h"

#include <JCDT_Lib/internal/classfmt/EnclosingMethodAttribute.inl>
#include <JCDT_Lib/internal/classfmt/ClassFile.inl>

namespace Jikes { // Open namespace Jikes block




	EnclosingMethodAttribute::EnclosingMethodAttribute(ClassFile& buffer)
		: AttributeInfo(ATTRIBUTE_EnclosingMethod, buffer)
		, class_index(buffer.GetU2())
		, name_and_type_index(buffer.GetU2())
	{
		if (attribute_length != 4)
			buffer.MarkInvalid("bad enclosing method attribute length");
		if (buffer.Pool()[class_index]->Tag() != CPInfo::CONSTANT_Class ||
			(name_and_type_index &&
			(buffer.Pool()[name_and_type_index]->Tag() !=
				CPInfo::CONSTANT_NameAndType)))
		{
			buffer.MarkInvalid("bad type for enclosing method attribute");
		}
	}

	void EnclosingMethodAttribute::Print(Ostream& os, const ConstantPool& constant_pool, int fill)
	{
		assert(! fill);
		PrintPrefix(os, "EnclosingMethod", constant_pool, 0);
		os << endl << " method: " << (unsigned)class_index << '.'
			<< (unsigned)name_and_type_index;
		if (constant_pool[class_index]->Tag() == CPInfo::CONSTANT_Class &&
			(! name_and_type_index ||
				(constant_pool[name_and_type_index]->Tag() ==
					CPInfo::CONSTANT_NameAndType)))
		{
			os << '=';
			constant_pool[class_index]->Describe(os, constant_pool);
			os << '.';
			if (name_and_type_index)
				constant_pool[name_and_type_index]->Describe(os, constant_pool);
			else os << "<initializer>";
		}
		else os << "(invalid)";
		os << endl;
	}
} // Close namespace Jikes block

