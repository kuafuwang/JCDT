
#include <JCDT_Lib/internal/impl/NameSymbol.h>
#include <JCDT_Lib/internal/codegen/code.h>


namespace Jikes { // Open namespace Jikes block


	bool NameSymbol::Contains(wchar_t character) const
	{
		for (wchar_t* ptr = name_; *ptr; ptr++)
		{
			if (*ptr == character)
				return true;
		}
		return false;
	}

	//
	// JLS2 6.8 describes the well-established Java naming conventions.
	// See also "Effective Java", item 38.
	//

	bool NameSymbol::IsBadStyleForClass() const
	{
		// JLS2 6.8.2
		return Code::IsAsciiLower(*name_) || Contains(U_UNDERSCORE);
	}

	bool NameSymbol::IsBadStyleForConstantField() const
	{
		// JLS2 6.8.5
		for (wchar_t* ptr = name_; *ptr; ptr++)
		{
			if (Code::IsAsciiLower(*ptr))
				return true;
		}
		return false;
	}

	bool NameSymbol::IsBadStyleForField() const
	{
		// JLS2 6.8.4
		return IsBadStyleForVariable();
	}

	bool NameSymbol::IsBadStyleForMethod() const
	{
		// JLS2 6.8.3
		return IsBadStyleForVariable();
	}

	bool NameSymbol::IsBadStyleForVariable() const
	{
		// JLS2 6.8.3, 6.8.4, 6.8.6
		return Code::IsAsciiUpper(*name_) || Contains(U_UNDERSCORE);
	}




} // Close namespace Jikes block




