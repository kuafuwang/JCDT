#pragma once
#include <cassert>
#include "JCDT_Lib/internal/codegen/code.h"
#include <JCDT_Lib/internal/lookup/LiteralSymbol.h>
#include <JCDT_Lib/internal/lookup/PositionInterface.h>
#include <JCDT_Lib/internal/parser/LPG/javasym.h>
#include "JCDT_Lib/internal/impl/NameSymbol.h"

namespace Jikes { // Open namespace Jikes block

class Token :public PositionNode
{

	//
	// It is expected that a location will be set for every token.
	// Therefore, as we are setting the location, we also reset the
	// deprecated bit to 0. If it is subsequently discovered that the
	// token is followed by one or more deprecated tags then the bit is
	// set to 1 by an invocation of the function SetDeprecated. Note that
	// a better way to resetting all the bits in "info" is to use the
	// function ResetInfoAndSetLocation defined below, instead of using
	// SetLocation.
	//
public:
	enum VirtualType
	{
		NORMAL,
		INSERT,
		REPLACE

	};
	inline void SetLocation(unsigned location)
	{
		assert(location <= 0x00FFFFFF);
		start_location = location;
	}
inline bool IsInRange(unsigned pos)
	{
		if (start_location <= pos && pos <= EndLocation())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	virtual int Compare(unsigned int pos) override {
		unsigned int start_pos = start_location;
		
		if(start_pos > pos ){
			return LEFT_HAND_LARGE;
		}
		unsigned int end_pos = start_pos + NameStringLength() -1 ;
		if( end_pos < pos){
			return LEFT_HAND_LESS;
		}

		return EQUAL;
	}
	enum TokenType
	{
		UNKNOWN,
		NAMESTRING,
		LITERAL,
		RIGHT_BRACE,
		
	};
	TokenIndex MatchingBrace()
	{
		return additional_info.right_brace->index;
	}
	TokenIndex index = 0;
	unsigned _kind = 0;
	unsigned start_location = 0;
	unsigned info;
	TokenType _token_type = UNKNOWN;
	union
	{
		Symbol* symbol;
		NameSymbol* name_symbol;
		Token* right_brace;
	} additional_info;

public:
	unsigned NameStringLength()
	{
		if (GetNameSymbol())
			return  additional_info.name_symbol->NameLength();
		else if(GetLiteralSymbol()){
			return  additional_info.symbol->NameLength();
		}
		else
			return wcslen(KeywordName(_kind));
	}

	wstring getName(){
		auto temp = NameString();
		return wstring(temp, temp + NameStringLength());
	}
	const wchar_t* NameString()
	{
		if (GetNameSymbol())
			return additional_info.name_symbol->Name();
		else if( GetLiteralSymbol())
			return additional_info.symbol->Name();
		else
			return Token::KeywordName(_kind);
	}


	LiteralSymbol* GetLiteralSymbol()
	{	
		Symbol* symbol = additional_info.symbol;
		if (symbol && _kind != TK_LBRACE && _token_type == LITERAL)
			return LiteralSymbol::LiteralCast(symbol);
		else
			return  NULL;
	}

	NameSymbol* GetNameSymbol()
	{
	
		auto symbol =additional_info.name_symbol;
		if (symbol && _token_type == NAMESTRING)
			return  dynamic_cast<NameSymbol*>(symbol);
		else
			return NULL;
	}

explicit Token(unsigned _location);
	~Token();
	//
	// To just reset the info, this function should be invoked with a
	// location value of 0.
	//
	inline void ResetInfoAndSetLocation(unsigned _location)
	{
		assert(_location <= 0x00FFFFFF);
		start_location = _location;
		additional_info.symbol = NULL;
		_kind = 0;
		info = 0;
		_token_type = UNKNOWN;
	}
	
	inline unsigned StartLocation() { return start_location; }
	inline void SetKind(unsigned kind){
		assert(kind <= 0x0000007F);
		_kind = kind;

	}

	inline unsigned EndLocation()
	{
		if (!end_location){
			end_location=  start_location + NameStringLength() -1;
		}
		
		return	end_location;
	}
	inline unsigned Kind() { return _kind; }

	inline void SetVirtualType(unsigned _type){
		assert(_type <= 0x0000007F);
		info = (info & 0xFFFFFF80) | _type;
	}
	inline unsigned VirtualType() { return info & 0x0000007F; }


	inline void ResetDeprecated() { info &= ~0x00000080; }
	inline void SetDeprecated() { info |= 0x00000080; }
	inline bool Deprecated() { return (info & 0x00000080) != 0; }

	inline void SetSymbol(Symbol* symbol){
		_token_type = LITERAL;
		additional_info.symbol = symbol;
		
	}
	inline void SetNameSymbol(NameSymbol* symbol)
	{
		_token_type = NAMESTRING;
		additional_info.name_symbol = symbol;
	}
	inline void SetRightBrace(Token* rbrace)
	{
		
		_token_type = RIGHT_BRACE;
		additional_info.right_brace = rbrace;
		
	
	}
	void Copy(Token& cpy) const
	{
		cpy.index = index;
		cpy.additional_info = additional_info;
		cpy.end_location = end_location;
		cpy.start_location = start_location;
		cpy.info = info;
		cpy._token_type = _token_type;
		cpy._kind = _kind;
	}
	static const wchar_t* KeywordName(int kind)
	{
		switch (kind)
		{
		case TK_abstract: return StringConstant::US_abstract;
		case TK_assert: return StringConstant::US_assert;
		case TK_boolean: return StringConstant::US_boolean;
		case TK_break: return StringConstant::US_break;
		case TK_byte: return StringConstant::US_byte;
		case TK_case: return StringConstant::US_case;
		case TK_catch: return StringConstant::US_catch;
		case TK_char: return StringConstant::US_char;
		case TK_class: return StringConstant::US_class;
		case TK_const: return StringConstant::US_const;
		case TK_continue: return StringConstant::US_continue;
		case TK_default: return StringConstant::US_default;
		case TK_do: return StringConstant::US_do;
		case TK_double: return StringConstant::US_double;
		case TK_else: return StringConstant::US_else;
		case TK_enum: return StringConstant::US_enum;
		case TK_extends: return StringConstant::US_extends;
		case TK_false: return StringConstant::US_false;
		case TK_final: return StringConstant::US_final;
		case TK_finally: return StringConstant::US_finally;
		case TK_float: return StringConstant::US_float;
		case TK_for: return StringConstant::US_for;
		case TK_goto: return StringConstant::US_goto;
		case TK_if: return StringConstant::US_if;
		case TK_implements: return StringConstant::US_implements;
		case TK_import: return StringConstant::US_import;
		case TK_instanceof: return StringConstant::US_instanceof;
		case TK_int: return StringConstant::US_int;
		case TK_interface: return StringConstant::US_interface;
		case TK_long: return StringConstant::US_long;
		case TK_native: return StringConstant::US_native;
		case TK_new: return StringConstant::US_new;
		case TK_null: return StringConstant::US_null;
		case TK_package: return StringConstant::US_package;
		case TK_private: return StringConstant::US_private;
		case TK_protected: return StringConstant::US_protected;
		case TK_public: return StringConstant::US_public;
		case TK_return: return StringConstant::US_return;
		case TK_short: return StringConstant::US_short;
		case TK_static: return StringConstant::US_static;
		case TK_strictfp: return StringConstant::US_strictfp;
		case TK_super: return StringConstant::US_super;
		case TK_switch: return StringConstant::US_switch;
		case TK_synchronized: return StringConstant::US_synchronized;
		case TK_this: return StringConstant::US_this;
		case TK_throw: return StringConstant::US_throw;
		case TK_throws: return StringConstant::US_throws;
		case TK_transient: return StringConstant::US_transient;
		case TK_true: return StringConstant::US_true;
		case TK_try: return StringConstant::US_try;
		case TK_void: return StringConstant::US_void;
		case TK_volatile: return StringConstant::US_volatile;
		case TK_while: return StringConstant::US_while;

		case TK_PLUS_PLUS: return StringConstant::US_PLUS_PLUS;
		case TK_MINUS_MINUS: return StringConstant::US_MINUS_MINUS;
		case TK_EQUAL_EQUAL: return StringConstant::US_EQUAL_EQUAL;
		case TK_LESS_EQUAL: return StringConstant::US_LESS_EQUAL;
		case TK_GREATER_EQUAL: return StringConstant::US_GREATER_EQUAL;
		case TK_NOT_EQUAL: return StringConstant::US_NOT_EQUAL;
		case TK_LEFT_SHIFT: return StringConstant::US_LEFT_SHIFT;
		case TK_RIGHT_SHIFT: return StringConstant::US_RIGHT_SHIFT;
		case TK_UNSIGNED_RIGHT_SHIFT:
			return StringConstant::US_UNSIGNED_RIGHT_SHIFT;
		case TK_PLUS_EQUAL: return StringConstant::US_PLUS_EQUAL;
		case TK_MINUS_EQUAL: return StringConstant::US_MINUS_EQUAL;
		case TK_MULTIPLY_EQUAL: return StringConstant::US_MULTIPLY_EQUAL;
		case TK_DIVIDE_EQUAL: return StringConstant::US_DIVIDE_EQUAL;
		case TK_AND_EQUAL: return StringConstant::US_AND_EQUAL;
		case TK_OR_EQUAL: return StringConstant::US_OR_EQUAL;
		case TK_XOR_EQUAL: return StringConstant::US_XOR_EQUAL;
		case TK_REMAINDER_EQUAL: return StringConstant::US_REMAINDER_EQUAL;
		case TK_LEFT_SHIFT_EQUAL: return StringConstant::US_LEFT_SHIFT_EQUAL;
		case TK_RIGHT_SHIFT_EQUAL:
			return StringConstant::US_RIGHT_SHIFT_EQUAL;
		case TK_UNSIGNED_RIGHT_SHIFT_EQUAL:
			return StringConstant::US_UNSIGNED_RIGHT_SHIFT_EQUAL;
		case TK_OR_OR: return StringConstant::US_OR_OR;
		case TK_AND_AND: return StringConstant::US_AND_AND;

		case TK_PLUS: return StringConstant::US_PLUS;
		case TK_MINUS: return StringConstant::US_MINUS;
		case TK_NOT: return StringConstant::US_NOT;
		case TK_REMAINDER: return StringConstant::US_REMAINDER;
		case TK_XOR: return StringConstant::US_XOR;
		case TK_AND: return StringConstant::US_AND;
		case TK_MULTIPLY: return StringConstant::US_MULTIPLY;
		case TK_OR: return StringConstant::US_OR;
		case TK_TWIDDLE: return StringConstant::US_TWIDDLE;
		case TK_DIVIDE: return StringConstant::US_DIVIDE;
		case TK_GREATER: return StringConstant::US_GREATER;
		case TK_LESS: return StringConstant::US_LESS;
		case TK_LPAREN: return StringConstant::US_LPAREN;
		case TK_RPAREN: return StringConstant::US_RPAREN;
		case TK_LBRACE: return StringConstant::US_LBRACE;
		case TK_RBRACE: return StringConstant::US_RBRACE;
		case TK_LBRACKET: return StringConstant::US_LBRACKET;
		case TK_RBRACKET: return StringConstant::US_RBRACKET;
		case TK_SEMICOLON: return StringConstant::US_SEMICOLON;
		case TK_QUESTION: return StringConstant::US_QUESTION;
		case TK_COLON: return StringConstant::US_COLON;
		case TK_COMMA: return StringConstant::US_COMMA;
		case TK_DOT: return StringConstant::US_DOT;
		case TK_ELLIPSIS: return StringConstant::US_DOT_DOT_DOT;
		case TK_AT: return StringConstant::US_AT;
		case TK_EQUAL: return StringConstant::US_EQUAL;
		case TK_EOF: return StringConstant::US_EOF;
		default: break;
		}
		return StringConstant::US_EMPTY;
	}
	private:
		unsigned int end_location = 0;
};


} // Close namespace Jikes block
