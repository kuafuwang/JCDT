#include "stdafx.h"
#include "AssistInfo.h"

#include <JCDT_Lib/internal/util/CharOperation.h>
namespace Jikes { // Open namespace Jikes block


	namespace CodeAssist {
		MiniError::MiniError(int _left_line_no, int _left_column_no, int _right_line_no,
			int _right_column_no, const wstring& _error_string,
			Problem::Severity _severity):severity(_severity),
			left_line_no(_left_line_no),
			left_column_no(_left_column_no),
			right_line_no(_right_line_no),
			right_column_no(_right_column_no),
			error_string(_error_string)

		{
		}


		MiniSemanticError::MiniSemanticError(int _msg_code, int _left_line_no, 
			int _left_column_no,int _right_line_no, int _right_column_no, 
			const wstring& _error_string, Problem::Severity _severity)
			: MiniError(_left_line_no, _left_column_no, _right_line_no, _right_column_no,
				_error_string, _severity), msg_code(_msg_code)
		{

		}

		MiniSemanticError::~MiniSemanticError()
		{
			
		}

		ExtraInfoForError::~ExtraInfoForError()
		{
		   for(auto it : errorInfos)
		   {
			   delete it;
		   }

		}

		ExtraInfoForError::ExtraInfoForError():semanticError(nullptr)
		{

		}

		AssistInfo::AssistInfo(const wstring& _path, int _line, int _column, WhatFor _what_for, unsigned _start_pos, unsigned _end_pos)
			: what_for(_what_for), doc_path(_path), line(_line), column(_column), start_pos(_start_pos), end_pos(_end_pos),
			  extraData(nullptr)
		{
		}

		AssistInfo::~AssistInfo()
		{
			delete extraData;
		}

		
	} // Close namespace Jikes block

} // Close namespace CodeAssist block

