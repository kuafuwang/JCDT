#include "JCDT_Lib/internal/env/platform.h"
#include <JCDT_Lib/internal/lookup/LiteralValue.h>
#include "JCDT_Lib/internal/ast/ast.h"
#include "JCDT_Lib/internal/lookup/Utf8LiteralTable.h"
#include <JCDT_Lib/internal/lookup/LiteralSymbol.h>
#include <JCDT_Lib/internal/lookup/TypeSymbol.h>

namespace Jikes { // Open namespace Jikes block


	LiteralValue* Utf8LiteralTable::FindOrInsertString(LiteralSymbol* literal)
	{
		const wchar_t* name = literal->Name() + 1;
		int literal_length = literal->NameLength() - 2; // discard ""

														// Big enough for the worst case: 3 bytes/char + \0.
		char* value = new char[literal_length * 3 + 1];
		int len = 0;
		int i = -1;

		while (++i < literal_length)
		{
			int ch = name[i];
			if (ch == U_BACKSLASH)
			{
				ch = 0;
				switch (name[++i])
				{
				case U_b:
					ch = U_BACKSPACE;
					break;
				case U_f:
					ch = U_FORM_FEED;
					break;
				case U_n:
					ch = U_LINE_FEED;
					break;
				case U_r:
					ch = U_CARRIAGE_RETURN;
					break;
				case U_t:
					ch = U_HORIZONTAL_TAB;
					break;
				case U_DOUBLE_QUOTE:
				case U_SINGLE_QUOTE:
				case U_BACKSLASH:
					ch = name[i];
					break;
				case U_0:
				case U_1:
				case U_2:
				case U_3:
					ch = name[i] - U_0;
					if (!Code::IsOctalDigit(name[i + 1]))
						break;
					i++;
					// fallthrough
				case U_4:
				case U_5:
				case U_6:
				case U_7:
					ch = ch * 8 + name[i] - U_0;
					if (!Code::IsOctalDigit(name[i + 1]))
						break;
					ch = ch * 8 + name[++i] - U_0;
					break;
				default:
					ch = -1;
				}
			}
			else if (Code::IsNewline(ch))
				ch = -1;

			if (ch < 0)
				break;
			else if (ch == 0)
			{
				value[len++] = (char)0xC0;
				value[len++] = (char)0x80;
			}
			else if (ch <= 0x007F)
				value[len++] = (char)ch;
			else if (ch <= 0x07FF)
			{
				value[len++] = (char)(0x0C0 | ((ch >> 6) & 0x01F));
				value[len++] = (char)(0x080 | (ch & 0x03F));
			}
			else
			{
				value[len++] = (char)(0x0E0 | ((ch >> 12) & 0x0F));
				value[len++] = (char)(0x080 | ((ch >> 6) & 0x03F));
				value[len++] = (char)(0x080 | (ch & 0x03F));
			}
		}

		value[len] = U_NULL;
		literal->value = (i < literal_length ? bad_value
			: FindOrInsert(value, len));

		delete[] value;
		return literal->value;
	}


	Utf8LiteralValue* Utf8LiteralTable::FindOrInsert(wchar_t ch)
	{
		int len = 0;
		char str[4];

		if (ch == 0)
		{
			str[len++] = (char)0xC0;
			str[len++] = (char)0x80;
		}
		else if (ch <= 0x007F)
			str[len++] = (char)ch;
		else if (ch <= 0x07FF)
		{
			str[len++] = (char)(0x0C0 | ((ch >> 6) & 0x01F));
			str[len++] = (char)(0x080 | (ch & 0x03F));
		}
		else
		{
			str[len++] = (char)(0x0E0 | (char)((ch >> 12) & 0x0F));
			str[len++] = (char)(0x080 | (char)((ch >> 6) & 0x03F));
			str[len++] = (char)(0x080 | (char)(ch & 0x03F));
		}

		str[len] = U_NULL;
		return FindOrInsert(str, len);
	}


	void Utf8LiteralTable::Rehash()
	{
		hash_size = primes[++prime_index];

		delete[] base;
		base = (Utf8LiteralValue**)memset(new Utf8LiteralValue*[hash_size], 0,
			hash_size * sizeof(Utf8LiteralValue*));

		//
		// Recall that the 0th element is unused.
		//
		for (unsigned i = 1; i < symbol_pool.Length(); i++)
		{
			Utf8LiteralValue* ulv = symbol_pool[i];
			int k = ulv->hash_address % hash_size;
			ulv->next = base[k];
			base[k] = ulv;
		}
	}


	unsigned Utf8LiteralTable::primes[] = {
		DEFAULT_HASH_SIZE, 8191, 16411, MAX_HASH_SIZE
	};

	Utf8LiteralTable::Utf8LiteralTable(LiteralValue* bad_value_)
		: symbol_pool(16384), utf8_literals(nullptr), 
		leftmost_constant_expr(nullptr),
		hash_size(primes[0]),
		prime_index(0),
		bad_value(bad_value_)
	{
		base = (Utf8LiteralValue**)memset(new Utf8LiteralValue*[hash_size], 0,
			hash_size * sizeof(Utf8LiteralValue*));
		symbol_pool.Next() = NULL; // do not use the 0th element
	}


	Utf8LiteralTable::~Utf8LiteralTable()
	{
		for (unsigned i = 0; i < symbol_pool.Length(); i++)
			delete symbol_pool[i];
		delete[] base;
	}


	Utf8LiteralValue* Utf8LiteralTable::FindOrInsert(const char* str, int len)
	{
		unsigned hash_address = Hash(str, len);
		int k = hash_address % hash_size;

		Utf8LiteralValue* lit;
		for (lit = base[k]; lit; lit = (Utf8LiteralValue*)lit->next)
		{
			if (hash_address == lit->hash_address &&
				len == lit->length &&
				memcmp(lit->value, str, len * sizeof(char)) == 0)
			{
				return lit;
			}
		}

		lit = new Utf8LiteralValue();
		lit->Initialize(str, len, hash_address, symbol_pool.Length());
		symbol_pool.Next() = lit;

		lit->next = base[k];
		base[k] = lit;

		//
		// If the number of unique elements in the hash table exceeds 2 times
		// the size of the base, and we have not yet reached the maximum
		// allowable size for a base, reallocate a larger base and rehash
		// the elements.
		//
		if (symbol_pool.Length() > (hash_size << 1) && hash_size < MAX_HASH_SIZE)
			Rehash();
		return lit;
	}


	//
	// Collapses all known strings in an expression chain into the leftmost one;
	// since the others in the chain have been set to "", this allows the emitter
	// to use a single call to StringBuffer.append() for the entire chain.
	//
	void Utf8LiteralTable::CollectStrings()
	{
		unsigned count = utf8_literals->Length();
		assert(count && leftmost_constant_expr);
		if (count == 1)
		{
			if (!NullLiteral::NullLiteralCast(leftmost_constant_expr))
				leftmost_constant_expr->value = (*utf8_literals)[0];
		}
		else
		{
			int length = 0;
			for (unsigned i = 0; i < count; i++)
				length += (*utf8_literals)[i]->length;
			char* str = new char[length + 1]; // +1 for '\0'

			int index = 0;
			for (unsigned k = 0; k < count; k++)
			{
				Utf8LiteralValue* literal = (*utf8_literals)[k];
				assert(literal->value);

				memcpy(&str[index], literal->value,
					literal->length * sizeof(char));
				index += literal->length;
			}
			str[length] = U_NULL;

			leftmost_constant_expr->value = FindOrInsert(str, length);

			delete[] str;
		}
		utf8_literals->Reset();
		leftmost_constant_expr = NULL;
	}


	//
	// The return value is true iff leftmost_constant_expr != NULL; in other words,
	// if the current expression ends in a known String value which can be chained
	// to the next expression. As a side effect, if the expression is constant, it
	// is in the growing tuple of known strings seen so far; and if the expression
	// is not constant, all strings in the tuple are collected into the leftmost
	// constant of the previous chain.
	//
	bool Utf8LiteralTable::EndsInKnownString(Expression* expression)
	{
		if (expression->IsConstant())
		{
			//
			// CollectStrings only works with Utf8LiteralValue* types, which
			// previous code in expr.cpp has already calculated. Here, we replace
			// constants with blank strings, and later we replace the left-most
			// constant with the concatenated version, so that expressions like
			// (nonconst + "a") + "b"; become (nonconst + "ab") + "";.  The
			// bytecode emitter is then smart enough to ignore the "".
			//
			Utf8LiteralValue* literal =
				DYNAMIC_CAST<Utf8LiteralValue*>(expression->value);
			assert(literal->value);

			utf8_literals->Next() = literal;
			if (!leftmost_constant_expr)
				leftmost_constant_expr = expression;
			else
				expression->value = FindOrInsert("", 0);
			return true;
		}

		BinaryExpression* binary_expr = BinaryExpression::BinaryExpressionCast(expression);
		CastExpression* cast_expr = CastExpression::CastExpressionCast(expression);
		AstParenthesizedExpression* paren_expr =
			 AstParenthesizedExpression::ParenthesizedExpressionCast(expression);
		NullLiteral* null_expr = NullLiteral::NullLiteralCast(expression);
		if (binary_expr)
		{
			//
			// If either subexpression is a constant but not a String, we have
			// already assigned it a Utf8LiteralValue.  But if a subexpression
			// is of type String, we don't know if it is constant yet.  Therefore,
			// we recurse to append the constant String for a primitive
			// expression, as well as to check if a String expression is constant.
			// This relies on the fact that this binary expression is of type
			// String. Remember that the null literal is not constant.
			//
			Expression* left = binary_expr->left_expression;
			Expression* right = binary_expr->right_expression;
			if (left->IsConstant() ||
				left->Type() == expression->Type())
			{
				EndsInKnownString(left);
			}
			if ((right->IsConstant() ||
				right->Type() == expression->Type()) &&
				EndsInKnownString(right))
			{
				if (leftmost_constant_expr == left &&
					!NullLiteral::NullLiteralCast(left) && ! NullLiteral::NullLiteralCast(right))
				{
					leftmost_constant_expr = binary_expr;
				}
				else
					right->symbol = expression->Type();
				return true;
			}
		}
		else if (cast_expr && EndsInKnownString(cast_expr->expression))
		{
			//
			// If we get here, the subexpression is necessarily a constant String;
			// but this cast is constant only if it is to type String.
			//
			if (leftmost_constant_expr == cast_expr->expression &&
				cast_expr->expression->Type() == cast_expr->Type())
			{
				leftmost_constant_expr = cast_expr;
			}
			return true;
		}
		else if (paren_expr && EndsInKnownString(paren_expr->expression))
		{
			if (leftmost_constant_expr == paren_expr->expression &&
				!NullLiteral::NullLiteralCast(leftmost_constant_expr))
			{
				leftmost_constant_expr = paren_expr;
			}
			return true;
		}
		else if (null_expr)
		{
			//
			// We are careful that null is never given a string value unless it is
			// part of a chain of strings, as it is not a compile-time constant.
			//
			//ebb hack. This entire method probably belongs in Semantic, where
			// we have access to Compiler.
			static char null_literal[] = { U_n, U_u, U_l, U_l, U_NU };
			utf8_literals->Next() = FindOrInsert(null_literal, 4);
			if (!leftmost_constant_expr)
				leftmost_constant_expr = expression;
			else expression->value = FindOrInsert("", 0);
			return true;
		}

		if (leftmost_constant_expr)
			CollectStrings();
		return false; // Not a constant String expression
	}


	//
	// This method flattens all known String expressions in the tree into a minimal
	// number of utf8 literals. Note that it even flattens non-constant expressions
	// (such as (Object)"ab", or null), when there are no side effects which could
	// get in the way.  After this method, expression -> IsConstant() will return
	// the correct value, but some intermediate subexpressions may return a
	// harmless false negative.
	//
	void Utf8LiteralTable::CheckStringConstant(Expression* expression)
	{
		utf8_literals = new Tuple<Utf8LiteralValue*>(256);
		leftmost_constant_expr = NULL;

		if (EndsInKnownString(expression))
			CollectStrings();

		delete utf8_literals;
	}

	

} // Close namespace Jikes block



