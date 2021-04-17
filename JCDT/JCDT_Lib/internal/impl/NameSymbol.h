
#ifndef NameSymbol_INCLUDED
#define NameSymbol_INCLUDED
#include <JCDT_Lib/internal/lookup/LiteralValue.h>


namespace Jikes { // Open namespace Jikes block


	class NameSymbol  
	{
	public:
		NameSymbol* next;
		int index;
		Utf8LiteralValue* Utf8_literal;
		inline unsigned HashCode() 
		{
			return index;

		};

		virtual const wchar_t* Name() const
		{
			return name_;
		}
		virtual unsigned NameLength() const
		{
			return length;
		}
		virtual const NameSymbol* Identity() const
		{
			return this;
		}
		const char* Utf8Name() const
		{
			return Utf8_literal ? Utf8_literal->value : (char*)NULL;
		}
		unsigned Utf8NameLength() const
		{
			return Utf8_literal ? Utf8_literal->length : 0;
		}

		bool IsBadStyleForClass() const;
		bool IsBadStyleForConstantField() const;
		bool IsBadStyleForField() const;
		bool IsBadStyleForMethod() const;
		bool IsBadStyleForVariable() const;

		NameSymbol() : name_(NULL) {}
		virtual ~NameSymbol() { delete[] name_; }

		inline void Initialize(const wchar_t* str, unsigned length_,
			unsigned hash_address_, int index_)
		{
			//_kind = NAME;

			hash_address = hash_address_;
			index = index_;

			length = length_;
			name_ = new wchar_t[length + 1];
			memcpy(name_, str, length * sizeof(wchar_t));
			name_[length] = U_NULL;

			Utf8_literal = NULL;
		}

	private:
		bool Contains(wchar_t character) const;

		friend class NameLookupTable;

		wchar_t* name_;
		unsigned length;
		unsigned hash_address;
	};

	

} // Close namespace Jikes block


#endif // _INCLUDED

