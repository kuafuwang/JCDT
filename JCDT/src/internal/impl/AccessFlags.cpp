#include "JCDT_Lib/internal/impl/access.h"
namespace Jikes { // Open namespace Jikes block


	bool AccessFlags::RecommendedOrder(const u2 flag)
	{
		u2 later_flags = 0;
		switch (flag)
		{
		case ACCESS_PUBLIC:
		case ACCESS_PRIVATE:
		case ACCESS_PROTECTED:
			later_flags = ACCESS_ABSTRACT | ACCESS_STATIC | ACCESS_FINAL |
				ACCESS_SYNCHRONIZED | ACCESS_TRANSIENT | ACCESS_VOLATILE |
				ACCESS_NATIVE | ACCESS_STRICTFP;
			break;
		case ACCESS_ABSTRACT:
			later_flags = ACCESS_STATIC | ACCESS_FINAL | ACCESS_SYNCHRONIZED |
				ACCESS_TRANSIENT | ACCESS_VOLATILE | ACCESS_NATIVE |
				ACCESS_STRICTFP;
			break;
		case ACCESS_STATIC:
			later_flags = ACCESS_FINAL | ACCESS_SYNCHRONIZED |
				ACCESS_TRANSIENT | ACCESS_VOLATILE | ACCESS_NATIVE |
				ACCESS_STRICTFP;
			break;
		case ACCESS_FINAL:
			later_flags = ACCESS_SYNCHRONIZED | ACCESS_TRANSIENT |
				ACCESS_VOLATILE | ACCESS_NATIVE | ACCESS_STRICTFP;
			break;
		case ACCESS_SYNCHRONIZED:
			later_flags = ACCESS_TRANSIENT | ACCESS_VOLATILE | ACCESS_NATIVE |
				ACCESS_STRICTFP;
			break;
		case ACCESS_TRANSIENT:
			later_flags = ACCESS_VOLATILE | ACCESS_NATIVE | ACCESS_STRICTFP;
			break;
		case ACCESS_VOLATILE:
			later_flags = ACCESS_NATIVE | ACCESS_STRICTFP;
			break;
		case ACCESS_NATIVE:
			later_flags = ACCESS_STRICTFP;
			break;
		case ACCESS_STRICTFP:
			break;
		default:
			assert(false && "invalid modifier");
		}
		return ! IsSet(later_flags);
	}

	void AccessFlags::Print(Ostream& os, AccessMetatype metatype) const
	{
		if (ACC_PUBLIC())
			os << "public ";
		if (ACC_PRIVATE())
			os << "private ";
		if (ACC_PROTECTED())
			os << "protected ";
		if (ACC_STATIC())
			os << "static ";
		if (ACC_FINAL())
			os << "final ";
		// The next bits are shared; the metatype determines the meaning.
		if (ACC_SYNCHRONIZED())
			os << (metatype == ACCESS_TYPE ? "super " : "synchronized ");
		if (ACC_VOLATILE())
			os << (metatype == ACCESS_METHOD ? "bridge " : "volatile ");
		if (ACC_TRANSIENT())
			os << (metatype == ACCESS_METHOD ? "varags " : "transient ");
		if (ACC_NATIVE())
			os << (metatype == ACCESS_VARIABLE ? "enum " : "native ");
		if (ACC_INTERFACE())
			os << "interface ";
		if (ACC_ABSTRACT())
			os << "abstract ";
		if (ACC_STRICTFP())
			os << "strictfp ";
		if (ACC_SYNTHETIC())
			os << "synthetic ";
		if (ACC_ANNOTATION())
			os << "annotation ";
	}

	 void AccessFlags::GetModifers(std::vector<std::wstring>& modifiers) const
	 {
		 if (ACC_PUBLIC())
			 modifiers.emplace_back(L"public");
		 if (ACC_PRIVATE())
			 modifiers.emplace_back(L"private");
		 if (ACC_PROTECTED())
			 modifiers.emplace_back(L"protected");

		 if (ACC_STATIC())
			 modifiers.emplace_back(L"static");

		 if (ACC_FINAL())
			 modifiers.emplace_back(L"final");

		 // The next bits are shared; the metatype determines the meaning.
		 if (ACC_SYNCHRONIZED())
			 modifiers.emplace_back(L"super");
		 if (ACC_VOLATILE())
			 modifiers.emplace_back(L"volatile");
		 if (ACC_TRANSIENT())
			 modifiers.emplace_back(L"transient");
		 if (ACC_NATIVE())
			 modifiers.emplace_back(L"native");
		 if (ACC_INTERFACE())
			 modifiers.emplace_back(L"interface");
		 if (ACC_ABSTRACT())
			 modifiers.emplace_back(L"abstract");
		 if (ACC_STRICTFP())
			 modifiers.emplace_back(L"strictfp");
		 if (ACC_SYNTHETIC())
			 modifiers.emplace_back(L"synthetic");
		 if (ACC_ANNOTATION())
			 modifiers.emplace_back(L"annotation");
	 }
} // Close namespace Jikes block



