// $Id: option.h,v 1.43 2004/10/10 23:17:44 elliott-oss Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1998, 1999, 2000, 2001, 2002 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#ifndef LiteralValue_INCLUDED
#define LiteralValue_INCLUDED
#include <JCDT_Lib/internal/env/platform.h>
#include "JCDT_Lib/internal/impl/long.h"
#include "JCDT_Lib/internal/impl/double.h"


namespace Jikes { // Open namespace Jikes block


	class LiteralValue
	{
	public:
		enum Kind
		{
			INT_KIND,
			LONG_KIND,
			FLOAT_KIND,
			DOUBLE_KIND,
			UTF8_KIND,
			UNDEFINED
		};
		virtual Kind getKind(){
			return UNDEFINED;
		}
		LiteralValue* next;
		int index;

		virtual ~LiteralValue() {}
		virtual LiteralValue* Clone()
		{
			return nullptr;
		};
	};


	class IntLiteralValue : public LiteralValue
	{
	public:
		i4 value;
		virtual Kind getKind() {
			return INT_KIND;
		}
		virtual ~IntLiteralValue() {}
		void Initialize(int value_)
		{
			value = value_;
			index = 0;
		}
		void Initialize(int value_, int index_)
		{
			value = value_;
			index = index_;
		}
		virtual LiteralValue* Clone()
		{
			auto clone = new IntLiteralValue();
			clone->value = value;
			clone->index = index;
			return clone;
		}
	};


	class LongLiteralValue : public LiteralValue
	{
	public:
		virtual Kind getKind() {
			return LONG_KIND;
		}
		LongInt value;

		virtual ~LongLiteralValue() {}

		void Initialize(LongInt value_, int index_)
		{
			value = value_;
			index = index_;
		}
		virtual LiteralValue* Clone()
		{
			auto clone = new LongLiteralValue();
			clone->value = value;
			clone->index = index;
			return clone;
		}
	};


	class FloatLiteralValue : public LiteralValue
	{
	public:
		virtual Kind getKind() {
			return FLOAT_KIND;
		}
		IEEEfloat value;

		virtual ~FloatLiteralValue() {}

		void Initialize(IEEEfloat value_, int index_)
		{
			value = value_;
			index = index_;
		}
		virtual LiteralValue* Clone()
		{
			auto clone = new FloatLiteralValue();
			clone->value = value;
			clone->index = index;
			return clone;
		}
	};


	class DoubleLiteralValue : public LiteralValue
	{
	public:
		virtual Kind getKind() {
			return DOUBLE_KIND;
		}
		IEEEdouble value;

		virtual ~DoubleLiteralValue() {}

		void Initialize(IEEEdouble value_, int index_)
		{
			value = value_;
			index = index_;
		}
		virtual LiteralValue* Clone()
		{
			auto clone = new DoubleLiteralValue();
			clone->value = value;
			clone->index = index;
			return clone;
		}
	};


	class Utf8LiteralValue : public LiteralValue
	{
	public:
		virtual Kind getKind() {
			return UTF8_KIND;
		}
		char* value;
		int  length;
		string getString()const
		{
			if (value)
			{
				return string(value, value + length);
			}
			else
				return{};
		}
		Utf8LiteralValue() : value(NULL)
		{}

		virtual ~Utf8LiteralValue()
		{
			delete[] value;
		}

		void Initialize(const char* value_,
			int length_, 
			unsigned hash_address_,
			int index_)
		{
			length = length_;
			value = new char[length + 1];
			memcpy(value, value_, length * sizeof(char));
			value[length] = U_NULL;

			hash_address = hash_address_;
			index = index_;
		}
		virtual LiteralValue* Clone()
		{
			auto clone = new Utf8LiteralValue();
			clone->Initialize(value, length, hash_address, index);
			return clone;
		}
	private:

		friend class Utf8LiteralTable;

		unsigned hash_address;
	};

	

} // Close namespace Jikes block


#endif // _INCLUDED

