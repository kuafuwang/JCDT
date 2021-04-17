
#ifndef AssistInfo_java_model_INCLUDED_INCLUDED
#define AssistInfo_java_model_INCLUDED_INCLUDED
#include <string>
#include <map>
#include <vector>
#include <boost/exception/exception.hpp>
//#include <JCDT_Lib/internal/problem/ProblemSeverities.h>

using std::wstring;
using std::map;
namespace Jikes {
	class IProgressMonitor;

	// Open namespace Jikes block

namespace Problem
{
	enum Severity;
}

namespace CodeAssist
{
	class MiniError
	{
	public:
		MiniError(int _left_line_no,
		int _left_column_no,
		int _right_line_no,
		int _right_column_no,
		const wstring& _error_string,
		Problem::Severity _severity
			);
		Problem::Severity severity;
		int left_line_no;
		int left_column_no;
		int right_line_no;
		int right_column_no;
		wstring error_string;
	};
	class MiniSemanticError:public MiniError
	{
	public:

		MiniSemanticError(
			int _msg_code,int _left_line_no,
			int _left_column_no, int _right_line_no, 
			int _right_column_no,const wstring& _error_string, 
			Problem::Severity _severit);


		~MiniSemanticError();

		int msg_code;
		
		
	};

	class ExtraInfoForError
	{
	public:
		std::vector<MiniError*> errorInfos;
		MiniSemanticError* semanticError;
		~ExtraInfoForError();
		ExtraInfoForError();
	};

	class  AssistInfo
	{
	public:
		enum WhatFor
		{
			SELECT_FOR_EDIT_GO_TO_DECL,
			SELECT_FOR_EDIT_TIPS,
			SELECT_FOR_CORRECTION_ERROR,
			SELECT_FOR_DEBUG,
			COMPILTE,
			SELECT_FOR_SCOPE,
			DO_NOTHING
		};

		explicit AssistInfo(const wstring& _path, int _line, int _column, WhatFor _what_for, unsigned start_pos, unsigned end_pos);
		WhatFor what_for;
		wstring doc_path;
		int line;
		int column;
		unsigned start_pos;
		unsigned end_pos;
		ExtraInfoForError* extraData;

		~AssistInfo();
	};

}// Close namespace CodeAssist block
	

} // Close namespace Jikes block


#endif // _INCLUDED

