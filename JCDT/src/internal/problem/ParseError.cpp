// $Id: option.h,v 1.43 2004/10/10 23:17:44 elliott-oss Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1998, 1999, 2000, 2001, 2002 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//
#include "JCDT_Lib/internal/problem/ParseError.h"
#include "JCDT_Lib/internal/impl/option.h"
#include <JCDT_Lib/internal/parser/LPG/lpginput.h>
#include <JCDT_Lib/internal/lex/LexStream.h>
#include "JCDT_Lib/compiler/Compiler.h"
#include "JCDT_Lib/internal/problem/ParseErrorInfo.h"
namespace Jikes { // Open namespace Jikes block



	ParseError::ParseError(Option* option_)
		: option(option_),
		  errors()
	{
		ParseErrorInfo::emacs_style_report = !option->errors;
	}

	//
	// This procedure uses a  quick sort algorithm to sort the ERRORS
	// by the left_line_no and left_column_no fields.
	//
	void ParseError::SortMessages()
	{
		int lower,
			upper,
			lostack[32],
			histack[32];

		int top,
			i,
			j;
		ParseErrorInfo* pivot;
		ParseErrorInfo* temp;

		top = 0;
		lostack[top] = 0;
		histack[top] = errors.size() - 1;

		while (top >= 0)
		{
			lower = lostack[top];
			upper = histack[top];
			top--;

			while (upper > lower)
			{
				/*********************************************************/
				/* The array is most-likely almost sorted. Therefore,    */
				/* we use the middle element as the pivot element.       */
				/*********************************************************/
				i = (lower + upper) / 2;
				pivot = errors[i];
				errors[i] = errors[lower];

				/*********************************************************/
				/* Split the array section indicated by LOWER and UPPER  */
				/* using ARRAY(LOWER) as the pivot->                      */
				/*********************************************************/
				i = lower;
				for (j = lower + 1; j <= upper; j++)
					if ((errors[j]->left_token < pivot->left_token) ||
						/*****************************************************/
						/* When two error messages start in the same location*/
						/* and one is nested inside the other, the outer one */
						/* is placed first so that it can be printed last.   */
						/* Recall that its right-span location is reached    */
						/* after the inner one has been completely processed.*/
						/*****************************************************/
						(errors[j]->left_token == pivot->left_token &&
							errors[j]->right_token > pivot->right_token) ||
						/*****************************************************/
						/* When two error messages are at the same location  */
						/* span, check the NUM field to keep the sort stable.*/
						/* When the location spans only a single symbol,     */
						/* the one with the lowest "num" is placed first.    */
						/*****************************************************/
						(errors[j]->left_token == pivot->left_token  &&
							errors[j]->right_token == pivot->right_token &&
							pivot->left_token == pivot->right_token     &&
							errors[j]->num < pivot->num) ||
						/*****************************************************/
						/* When two error messages are at the same location  */
						/* which spans more than one symbol in the source,   */
						/* the first message is treated as been nested into  */
						/* the second message and (just like the nested case */
						/* above) it is placed last in the sorted sequence.  */
						/*****************************************************/
						(errors[j]->left_token == pivot->left_token  &&
							errors[j]->right_token == pivot->right_token &&
							pivot->left_token < pivot->right_token      &&
							errors[j]->num > pivot->num))
					{
						temp = errors[++i];
						errors[i] = errors[j];
						errors[j] = temp;
					}
				errors[lower] = errors[i];
				errors[i] = pivot;

				top++;
				if ((i - lower) < (upper - i))
				{
					lostack[top] = i + 1;
					histack[top] = upper;
					upper = i - 1;
				}
				else
				{
					histack[top] = i - 1;
					lostack[top] = lower;
					lower = i + 1;
				}
			}
		}
	}


	//
	// This procedure is invoked by a JIKES PARSER or a semantic
	// routine to process an error message.  The JIKES parser always
	// passes the value 0 to msg_level to indicate an error.
	// This routine simply stores all necessary information about
	// the message into an array: error.
	//
	void ParseError::Report(BaseLexStream* lex_stream,int msg_level, ParseErrorCode msg_code,
		int name_index, TokenIndex left_token,
		TokenIndex right_token, int scope_name_index)
	{
		errors.push_back(new ParseErrorInfo());
		int i = errors.size() - 1;

		errors[i]->msg_level = msg_level;
		errors[i]->msg_code = msg_code;
		errors[i]->name_index = name_index;
		errors[i]->num = i;
		errors[i]->left_token = (left_token > Loc(right_token) ? Loc(right_token)
			: left_token);
		errors[i]->right_token = Loc(right_token);
		errors[i]->scope_name_index = scope_name_index;
		if(lex_stream)
		{
			lex_stream->RereadInput();
			errors[i]->Initialize(lex_stream);
		}
		

	}
	/*****************************************************************/
	/* This procedure is invoked to print JIKES error messages that  */
	/* have been stored in the array: error.                         */
	/*****************************************************************/
	void ParseError::PrintMessages(BaseLexStream* lex_stream)
	{
		if (errors.size() == 0)
		{
			Coutput.flush();
			return;
		}
		if (option->errors){
			Coutput << endl << "Found " << errors.size() << " syntax error"
				<< (errors.size() == 1 ? "" : "s") << " in \""
				<< lex_stream->FileName() << "\":";
		}

		lex_stream->RereadInput();

		if (!lex_stream->InputBuffer())
		{
			/*const char* file_name = file_symbol->lex_stream->FileName();
			int length = file_symbol->lex_stream->FileNameLength();
			wchar_t* name = new wchar_t[length + 1];
			for (int i = 0; i < length; i++)
				name[i] = file_name[i];
			name[length] = U_NULL;

			file_symbol->semantic ->ReportSemErrorSimple(
					SemanticError::CANNOT_REOPEN_FILE,
					BAD_TOKEN, 
					name);

				delete[] name;*/
			return;
		}

		SortMessages();

		int stack_top = -1;
		int* error_stack = new int[errors.size()];

		for (unsigned k = 0; k < errors.size(); k++)
		{
			for (;  stack_top >= 0 &&
					(errors[error_stack[stack_top]]->right_token <=errors[k]->left_token);
					stack_top--)
			{
				if (stack_top == 0)
				{
					PrintMessage(lex_stream,error_stack[stack_top]);
				}
				else
				{
					int i = error_stack[stack_top],
						j = error_stack[stack_top - 1];

					if ((errors[i]->msg_code != SECONDARY_CODE &&
						errors[i]->msg_code != MISPLACED_CODE &&
						errors[i]->msg_code != DELETION_CODE) ||
						(errors[j]->msg_code != DELETION_CODE  &&
							errors[j]->msg_code != MISPLACED_CODE &&
							errors[j]->msg_code != SECONDARY_CODE))
					{
						PrintMessage(lex_stream,i);
					}
				}
			}

			if (errors[k]->left_token < errors[k]->right_token)
			{
				stack_top++;
				error_stack[stack_top] = k;
			}
			else if (!(stack_top >= 0 &&
				(errors[error_stack[stack_top]]->msg_code == SECONDARY_CODE ||
					errors[error_stack[stack_top]]->msg_code == DELETION_CODE) &&
				(errors[k]->left_token == errors[error_stack[stack_top]]->left_token ||
					errors[k]->right_token == errors[error_stack[stack_top]]->right_token)))
			{
				/* NOTE that input_line already contains a '\n' character */
				PrintMessage(lex_stream, k);
			}
		}

		for (; stack_top > 0; stack_top--)
		{
			int i = error_stack[stack_top],
				j = error_stack[stack_top - 1];

			if ((errors[i]->msg_code != SECONDARY_CODE &&
				errors[i]->msg_code != DELETION_CODE) ||
				(errors[j]->msg_code != DELETION_CODE  &&
					errors[j]->msg_code != SECONDARY_CODE))
			{
				PrintMessage(lex_stream,i);
			}
		}
		if (stack_top == 0)
			PrintMessage(lex_stream,error_stack[stack_top]);

		Coutput.flush();

		delete[] error_stack;

	}
	void  ParseError::InitAllMessages(BaseLexStream* lex_stream)
	{
		if (errors.size() == 0){	
			return;
		}
		lex_stream->RereadInput();
		if (!lex_stream->InputBuffer())
		{
			/*const char* file_name = lex_stream->FileName();
			int length = lex_stream->FileNameLength();
			wchar_t* name = new wchar_t[length + 1];
			for (int i = 0; i < length; i++)
				name[i] = file_name[i];
			name[length] = U_NULL;

			file_symbol->semantic->ReportSemErrorSimple(
				SemanticError::CANNOT_REOPEN_FILE,
				BAD_TOKEN,
				name);

			delete[] name;*/
			return;
		}

		SortMessages();
		int stack_top = -1;
		int* error_stack = new int[errors.size()];

		for (unsigned k = 0; k < errors.size(); k++)
		{
			for (;  stack_top >= 0 &&
				(errors[error_stack[stack_top]]->right_token <= errors[k]->left_token);
				stack_top--)
			{
				if (stack_top == 0)
				{
					errors[error_stack[stack_top]]->Initialize(lex_stream);
				}
				else
				{
					int i = error_stack[stack_top],
						j = error_stack[stack_top - 1];

					if ((errors[i]->msg_code != SECONDARY_CODE &&
						errors[i]->msg_code != MISPLACED_CODE &&
						errors[i]->msg_code != DELETION_CODE) ||
						(errors[j]->msg_code != DELETION_CODE  &&
							errors[j]->msg_code != MISPLACED_CODE &&
							errors[j]->msg_code != SECONDARY_CODE))
					{
						errors[i]->Initialize(lex_stream);
					}
				}
			}

			if (errors[k]->left_token < errors[k]->right_token){
				stack_top++;
				error_stack[stack_top] = k;
			}
			else if (!(stack_top >= 0 &&
				(errors[error_stack[stack_top]]->msg_code == SECONDARY_CODE ||
					errors[error_stack[stack_top]]->msg_code == DELETION_CODE) &&
				(errors[k]->left_token == errors[error_stack[stack_top]]->left_token ||
					errors[k]->right_token == errors[error_stack[stack_top]]->right_token)))
			{
				/* NOTE that input_line already contains a '\n' character */
				errors[k]->Initialize(lex_stream);
			}
		}

		for (; stack_top > 0; stack_top--)
		{
			int i = error_stack[stack_top],
				j = error_stack[stack_top - 1];

			if ((errors[i]->msg_code != SECONDARY_CODE &&
				errors[i]->msg_code != DELETION_CODE) ||
				(errors[j]->msg_code != DELETION_CODE  &&
					errors[j]->msg_code != SECONDARY_CODE))
			{
				errors[i]->Initialize(lex_stream);
			}
		}
		if (stack_top == 0)
			errors[ error_stack[stack_top] ]->Initialize(lex_stream);
		delete[] error_stack;
	}
	void ParseError::PrintMessage(BaseLexStream* lex_stream,int k)
	{
		errors[k]->Initialize(lex_stream);
		Compiler::reportError(errors[k]);
	}
	void ParseError::Clear()
	{
		auto _size = errors.size();
		for (size_t i = 0; i < _size; ++i)
		{
			delete errors[i];
		}
		errors.clear();
	}

	void ParseError::TransferError(vector<JikesError*>& errer_owner, BaseLexStream* lex_stream)
	{
		InitAllMessages(lex_stream);
		vector<ParseErrorInfo*> temp;
		temp.swap(errors);

		errer_owner.insert(errer_owner.end() , temp.begin(), temp.end());
		
	}

	ParseError::~ParseError()
	{
		size_t size = errors.size();
		for(size_t i = 0 ; i < size; ++ i)
		{
			delete errors[i];
		}
	}
} // Close namespace Jikes block



