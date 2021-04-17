
#ifndef Jikes_ParseErrorInfo_H_FLAG_
#define Jikes_ParseErrorInfo_H_FLAG_

#include "JikesError.h"
#include <string>
#include <JCDT_Lib/internal/env/typedef.h>
#include "ParseErrorCode.h"

namespace Jikes { // Open namespace Jikes block

	class BaseLexStream;


class ParseErrorInfo: public JikesError
{
	friend class ParseError;
public:
	ParseErrorInfo();
	virtual std::wstring getErrorMessage();
    virtual std::wstring getErrorReport();

    virtual Problem::Severity getSeverity();
    virtual const char* getFileName();

    virtual int getLeftLineNo();
    virtual int getLeftColumnNo();
    virtual int getRightLineNo();
    virtual int getRightColumnNo();

private:
    int left_line_no;
    int left_column_no;
    int right_line_no;
    int right_column_no;
	
    static bool emacs_style_report;
   
    void Initialize(BaseLexStream*);

    const wchar_t* regularErrorString();
    const wchar_t* emacsErrorString();

    TokenIndex left_token;
    TokenIndex right_token;

    int name_index;
    int num;
    unsigned char msg_level;
    ParseErrorCode msg_code;
    unsigned scope_name_index;

	wstring error_string;
	string fileName;
	bool initialized ;

private:
	void getemacsErrorString();
	void getregularErrorString(BaseLexStream*);
};



} // Close namespace Jikes block


#endif // ParseErrorInfo_H_FLAG_
