#include "JCDT_Lib/internal/lex/LexStream.h"
#include "JCDT_Lib/internal/util/zip/zip.h"
#include "JCDT_Lib/internal/env/LookupEnvironment.h"
#include  "JCDT_Lib/internal/semantic/semantic.h"
#include "JCDT_Lib/internal/impl/option.h"
#include "JCDT_Lib/internal/util/tab.h"
#include <JCDT_Lib/internal/problem/ErrorString.h>
#include "JCDT_Lib/compiler/Compiler.h"
#include <JCDT_Lib/internal/lookup/FileSymbol.h>
#include <JCDT_Lib/internal/util/FileLoader.h>

namespace Jikes { // Open namespace Jikes block


// Class BaseLexStream

	LexStream::LexStream(Option* option_, FileBase* file_symbol_)
		: BaseLexStream(option_), file_symbol(file_symbol_)
	{

	}
BaseLexStream::BaseLexStream(Option* option_)
	: 
#ifdef JIKES_DEBUG
	file_read(false),
#endif
	  comment_stream(),
      index(0),
      locations(NULL),
      line_location(12, 8),
      package(0),
      initial_reading_of_input(true),
      comment_buffer(NULL),
	  option(option_)
{
	token_stream.reserve(16);
    StreamError::emacs_style_report = ! option->errors;
	pool = new boost::object_pool<Token>();
}

BaseLexStream::~BaseLexStream()
{
#ifdef JIKES_DEBUG
    if (file_read)
		LookupEnvironment::line_count += (line_location.Length() - 3);
#endif
	if (pool) {
		
		delete pool;
		pool = nullptr;
	}
	size_t _size = bad_token_stream.size();
	for (size_t i = 0; i < _size; ++i)
		delete bad_token_stream[i];
	
	_size = comment_stream.size();
	auto data_comment_stream = comment_stream.data();
	for (auto i = 0; i < _size; ++i)
		delete data_comment_stream[i];
	
    DestroyInput();
}


	void BaseLexStream::GetText(unsigned start_loc, unsigned end_loc, wstring& output){

		if (start_loc < 0)
			return;
		if(end_loc > input_buffer_length)
			return;
		auto begin = input_buffer + start_loc;
		auto end = input_buffer + end_loc;
		output.assign(begin, end);
	}

	//
// Outputs a line of source code, flattening literal TABs into spaces for
// uniform output spacing.
//
void BaseLexStream::OutputLine(unsigned line_no, ErrorString& s)
{
    assert(line_no);
    unsigned line_end = LineEnd(line_no);
    bool expand = Coutput.ExpandWchar();
    for (unsigned i = LineStart(line_no), offset = 0; i <= line_end;
         i++, offset++)
    {
        wchar_t ch = input_buffer[i];
        if (ch == U_CARRIAGE_RETURN || ch == U_LINE_FEED)
            s << (wchar_t) U_LINE_FEED;
        else if (ch == U_HORIZONTAL_TAB)
        {
            s.width(Tab::TabSize() - offset % Tab::TabSize());
            //s << (wchar_t) U_SPACE;
            offset = Tab::TabSize() - 1;
        }
        else if (ch == U_NULL)
        {
            s << (expand ? "\\u0000" : "?");
        }
        else
        {
            if (expand && (ch < U_SPACE || ch >= 0x0ff))
                offset += 5;
            s << ch;
        }
    }
}


//
// Outputs the section of source code which is in error.
//
void BaseLexStream::OutputSource(JikesError* err, ErrorString& s)
{
    int left_line_no = err -> getLeftLineNo();
    int left_column_no = err -> getLeftColumnNo();
    int right_line_no = err -> getRightLineNo();
    int right_column_no = err -> getRightColumnNo();
    if (left_line_no == 0)
        s << endl;

	else if (left_line_no >= right_line_no)
	{
		s << endl << endl;
		s.width(6);
		s << left_line_no << ". ";
		OutputLine(left_line_no, s);

		s.width(left_column_no + 8);
		s << '^';
		if (left_column_no < right_column_no)
		{
			s.width(right_column_no - left_column_no);
			s.fill('-');
			s << "^";
			s.fill(' ');
		}
	}
	else // multi-line
	{
		s << endl << endl;
		s.width(left_column_no + 8);
		s << "<";
		s.width(LineLength(left_line_no) - left_column_no);
		s.fill('-');
		s << "" << endl;
		s.fill(' ');

		s.width(6);
		s << left_line_no << ". ";
		OutputLine(left_line_no, s);
		if (right_line_no > left_line_no + 1)
			s << "   . . ." << endl;
		s.width(6);
		s << right_line_no << ". ";
		OutputLine(right_line_no, s);

		s.width(8);
		s << "";
		s.width(right_column_no);
		s.fill('-');
		s << ">";
		s.fill(' ');
	}
}


//
// Find and return the index of the first comment that immediately follows
// tok. Return 0 if there is not a comment that immediately follows tok.
//
BaseLexStream::CommentIndex BaseLexStream::FirstComment(TokenIndex tok)
{
    unsigned location = Location(tok);
    int lo = 0;
    int hi = comment_stream.size() - 1;
    unsigned i = 0;
    if (lo < hi)
    {
        do
        {
            int mid = (lo + hi) / 2;
            if (comment_stream[mid]->location < location)
                lo = mid + 1;
            else hi = mid - 1;
        } while (lo < hi);
        i = comment_stream[lo]->location > location ? lo : lo + 1;
    }
    return i < (unsigned) comment_stream.size() &&
        comment_stream[i]->previous_token == tok ? i : 0;
}


unsigned BaseLexStream::FindLine(unsigned location)
{
    int lo = 0;
    int hi = line_location.Length() - 1;
    assert(locations);
    // we can place the exit test at the bottom of the loop
    // since the line_location array will always contain at least
    // one element.
    do
    {
        int mid = (lo + hi) / 2;
        if (locations[mid] == location)
            return mid;
        if (locations[mid] < location)
             lo = mid + 1;
        else hi = mid - 1;
    } while (lo < hi);
    return locations[lo] > location ? lo - 1 : lo;
}


 //
 // Name of input file where the token appeared.
 //
 const char* LexStream::FileName() { return file_symbol->FileName(); }
 unsigned LexStream::FileNameLength()
 {
	 return file_symbol->FileNameLength();
 }
 //
void LexStream::ReadInput()
{
    if (file_symbol -> IsZip())
    {
		Zip *zip = file_symbol->Zipfile();
		u4 offset = file_symbol->offset;
		u4 uncompressed_size = file_symbol->uncompressed_size;

        ZipFile* zipfile = new ZipFile(zip, uncompressed_size, offset);

        if (zipfile -> Buffer() == NULL)
        {
            fprintf(stderr, "chaos: Don\'t know how to process compressed "
                    "(\".java\") source in a zip file\n");
            assert(false);
        }
        else if (! file_symbol -> lex_stream)
        {
            // Once the zip file is loaded, it never changes. So, we only read
            // it the first time
            file_symbol -> lex_stream = this;
            ProcessInput(zipfile -> Buffer(),
                         file_symbol -> uncompressed_size);
        }
        delete zipfile;
    }
    else
    {
		if(file_symbol->IsVirtualFile())
		{
			file_symbol->lex_stream = this;
			FileReader* file =
				FileHelper::getInstance()->GetVirutalFile();
			if (file){
				ProcessInput(file->getBuffer(), file->getBufferSize());
				delete file;
			}
		}
		else {
			time_t st_mtime = 0;
			FileHelper::getInstance()->LastModifiedTime(FileName(), st_mtime);

			file_symbol->mtime = st_mtime; // actual time stamp of file read
			file_symbol->lex_stream = this;


			FileReader* file =
				FileHelper::getInstance()->read(FileName());
			if (file)
			{
				ProcessInput(file->getBuffer(), file->getBufferSize());
				delete file;
			}
		}
    }

    initial_reading_of_input = false;
}

void LexStream::RereadInput()
{
    if (input_buffer) // if input already available, do nothing
        return ;
	if (file_symbol->IsVirtualFile())
	{
		file_symbol->lex_stream = this;
		FileReader* file =
			FileHelper::getInstance()->GetVirutalFile();
		if (file) {
			ProcessInput(file->getBuffer(), file->getBufferSize());
			delete file;
		}
	}
    else if (file_symbol -> IsZip())
    {
		Zip *zip = file_symbol->Zipfile();
		u4 offset = file_symbol->offset;
		u4 uncompressed_size = file_symbol->uncompressed_size;

		ZipFile* zipfile = new ZipFile(zip, uncompressed_size, offset);

        if (zipfile -> Buffer() == NULL)
        {
            fprintf(stderr, "chaos: Don\'t know how to process compressed "
                    "(\".java\") source in a zip file\n");
            assert(false);
        }
        else ProcessInput(zipfile -> Buffer(),
                          file_symbol -> uncompressed_size);
        delete zipfile;
    }
    else
    {
		time_t st_mtime = 0;
		FileHelper::getInstance()->LastModifiedTime(FileName(), st_mtime);

		if (st_mtime == file_symbol->mtime)
		{
			FileReader* file =
				FileHelper::getInstance()->read(FileName());
			if (file)
			{
				ProcessInput(file->getBuffer(), file->getBufferSize());
				delete file;
			}
		}
		else
		{
			// TODO: File has changed !!!
		}
		
    }
}


int BaseLexStream::hexvalue(wchar_t ch)
{
    switch (ch)
    {
    case U_a: case U_A:
        return 10;
    case U_b: case U_B:
        return 11;
    case U_c: case U_C:
        return 12;
    case U_d: case U_D:
        return 13;
    case U_e: case U_E:
        return 14;
    case U_f: case U_F:
        return 15;
    default:
        return ch - U_0;
    }
}

	Token* BaseLexStream::GetNextTokenAndReturnToken(unsigned location)
	{
		Token* _tokon = pool->construct(location);
		TokenIndex index = token_stream.size();
		token_stream.push_back(_tokon);
		_tokon->index = index;
		return _tokon;
	}

	TokenIndex BaseLexStream::GetNextToken(unsigned location)
	{
		Token* _tokon = pool->construct(location);
		TokenIndex index = token_stream.size();
		token_stream.push_back(_tokon);
		_tokon->index = index;
		return index;
	}

	void BaseLexStream::TransferError(vector<JikesError*>& errer_owner)
	{
		vector<StreamError*> temp;
		temp.swap(bad_token_stream);
		errer_owner.insert(errer_owner.end(), temp.begin(), temp.end());	
	}

	//
// Store/convert filesize bytes from a file in the input_buffer.
//


#if defined(HAVE_ENCODING)

void BaseLexStream::ProcessInputUnicode(const char* buffer, long filesize)
{
    //fprintf(stderr,"BaseLexStream::ProcessInputUnicode called.\n");
#ifdef JIKES_DEBUG
    file_read = true;
#endif

    wchar_t* input_ptr = AllocateInputBuffer(filesize);
    wchar_t* input_tail = input_ptr + filesize;
    *input_ptr = U_LINE_FEED; // add an initial '\n';

    if (buffer)
    {
        int escape_value = 0;
        wchar_t* escape_ptr = NULL;
        UnicodeLexerState saved_state = RAW;
        UnicodeLexerState state = START;
        bool oncemore = false;

        // If oncemore is true, ch holds the current character, otherwise
        // it is updated to the next character
        wchar_t ch = 0;
		int maxDetectSize;
		if (filesize > 516)
			maxDetectSize = 516;
		else
			maxDetectSize = filesize;

		auto 	detectResult = detectTextEncoding(buffer, filesize);

        if (option->encoding){
				// The encoding should have been validated by now
			bool encoding_set;
			   if(detectResult.second == 100 && maxDetectSize == 516)
				   encoding_set = SetEncoding(detectResult.first.c_str());
			   else 
				   encoding_set = SetEncoding(option->encoding);
				assert(encoding_set);		 
        }
		else{		
			if (detectResult.second > 70) {
				bool encoding_set = SetEncoding(detectResult.first.c_str());
			}
			else{
				const string & default_encoding = getDefaultEncoding();
				bool encoding_set = SetEncoding(default_encoding.c_str());
			}
		}

        // init data after setting the encoding
        InitializeDataBuffer(buffer, filesize);
        while (HasMoreData() || oncemore)
        {
            // On each iteration we advance input_ptr a maximum of 2 positions.
            // Here we check if we are close to the end of input_buffer.
            if (input_ptr >= input_tail)
            {
                // If this happens, reallocate it with some more space.
                // This is very rare case, which could happen if
                // one code page character is represented by several
                // unicode characters. One of exaples of such
                // situation is unicode "surrogates".
                //
                // If such reallocation will be required, it will indeed
                // slow down compilation a bit.
                size_t cursize = input_ptr - input_buffer;
                size_t newsize = cursize + cursize / 10 + 4; // add 10%
                wchar_t* tmp = new wchar_t[newsize];
                memcpy(tmp, input_buffer, cursize * sizeof(wchar_t));
                delete [] input_buffer;
                input_buffer = tmp;
                input_tail = input_buffer + newsize - 1;
                input_ptr  = input_buffer + cursize;
            }

            if (! oncemore)
            {
                ch = DecodeNextCharacter();
                if (ErrorDecodeNextCharacter())
                    break;
            }
            else oncemore = false;

            switch (state)
            {
            case QUOTE:
                *(++input_ptr) = U_BACKSLASH;
                if (ch == U_BACKSLASH)
                {
                    *(++input_ptr) = U_BACKSLASH;
                    state = RAW;
                }
                else if (ch == U_u)
                {
                    //
                    // We transfer all the characters of the escape sequence,
                    // in case it is invalid; but remember where it started
                    // for error reporting, and to back up on success.
                    //
                    escape_ptr = input_ptr;
                    *(++input_ptr) = U_u;
                    state = UNICODE_ESCAPE;
                }
                else
                {
                    state = RAW;
                    oncemore = true;
                }
                break;
            case UNICODE_ESCAPE:
                *(++input_ptr) = ch;
                if (Code::IsHexDigit(ch))
                {
                    state = UNICODE_ESCAPE_DIGIT_0;
                    escape_value = hexvalue(ch) << 12;
                }
                else if (ch != U_u)
                {
                    if (initial_reading_of_input)
                        ReportMessage(StreamError::INVALID_UNICODE_ESCAPE,
                                      (unsigned) (escape_ptr - input_buffer),
                                      ((unsigned) (input_ptr - input_buffer) -
                                       (Code::IsNewline(ch) ? 1 : 0)));
                    state = RAW;
                }
                break;
            case UNICODE_ESCAPE_DIGIT_0:
                *(++input_ptr) = ch;
                if (Code::IsHexDigit(ch))
                {
                    state = UNICODE_ESCAPE_DIGIT_1;
                    escape_value += hexvalue(ch) << 8;
                }
                else
                {
                    if (initial_reading_of_input)
                        ReportMessage(StreamError::INVALID_UNICODE_ESCAPE,
                                      (unsigned) (escape_ptr - input_buffer),
                                      ((unsigned) (input_ptr - input_buffer) -
                                       (Code::IsNewline(ch) ? 1 : 0)));
                    state = RAW;
                }
                break;
            case UNICODE_ESCAPE_DIGIT_1:
                *(++input_ptr) = ch;
                if (Code::IsHexDigit(ch))
                {
                    state = UNICODE_ESCAPE_DIGIT_2;
                    escape_value += hexvalue(ch) << 4;
                }
                else
                {
                    if (initial_reading_of_input)
                        ReportMessage(StreamError::INVALID_UNICODE_ESCAPE,
                                      (unsigned) (escape_ptr - input_buffer),
                                      ((unsigned) (input_ptr - input_buffer) -
                                       (Code::IsNewline(ch) ? 1 : 0)));
                    state = RAW;
                }
                break;
            case UNICODE_ESCAPE_DIGIT_2:
                if (Code::IsHexDigit(ch))
                {
                    ch = escape_value + hexvalue(ch);
                    state = saved_state;
                    input_ptr = escape_ptr - 1; // Back up - see case QUOTE.
                    oncemore = true;
                }
                else
                {
                    *(++input_ptr) = ch;
                    if (initial_reading_of_input)
                        ReportMessage(StreamError::INVALID_UNICODE_ESCAPE,
                                      (unsigned) (escape_ptr - input_buffer),
                                      ((unsigned) (input_ptr - input_buffer) -
                                       (Code::IsNewline(ch) ? 1 : 0)));
                    state = RAW;
                }
                saved_state = UNICODE_ESCAPE_DIGIT_2;
                break;
            case CR:
                if (ch == U_LINE_FEED)
                {
                    // skip line feed if it comes right after a CR.
                    state = RAW;
                }
                else if (ch == U_CARRIAGE_RETURN)
                {
                    // but if CR follows CR then the second CR starts a
                    // line feed too (and note that state=CR afterwards),
                    // so that CR-CR-LF will be handled correctly.
                    *(++input_ptr) = U_LINE_FEED;
                }
                else if (ch == U_BACKSLASH &&
                         saved_state != UNICODE_ESCAPE_DIGIT_2)
                {
                    state = QUOTE;
                }
                else
                {
                    state = RAW;
                    *(++input_ptr) = ch;
                }
                // clear saved_state == UNICODE_ESCAPE_DIGIT_2 status
                saved_state = CR;
                break;
            case START:
                // if for some reason converter produced or passed
                // byte order mark, it have to be ignored.
                state = RAW;
                if (ch == U_BOM || ch == U_REVERSE_BOM)
                    break; //ignore
                // fallthrough
            case RAW:
                if (ch == U_BACKSLASH && saved_state != UNICODE_ESCAPE_DIGIT_2)
                {
                    state = QUOTE;
                }
                else if (ch == U_CARRIAGE_RETURN)
                {
                    state = CR;
                    *(++input_ptr) = U_LINE_FEED;
                }
                else
                {
                    *(++input_ptr) = ch;
                }
                saved_state = RAW;
                break;
            }
        }
        if (state == QUOTE)
        {
            *(++input_ptr) = U_BACKSLASH;
        }
        else if (state >= UNICODE_ESCAPE)
        {
            if (initial_reading_of_input)
                ReportMessage(StreamError::INVALID_UNICODE_ESCAPE,
                              (unsigned) (escape_ptr - input_buffer),
                              (unsigned) (input_ptr - input_buffer));
        }
    }

    //
    // To aid the scanner, we artificially remove any U_CTL_Z ending the file,
    // and insert U_CARRIAGE_RETURN, U_NULL. This is because U_CTL_Z is legal
    // inside comments, but // comments must end on a newline; and it is safe
    // since the above pass converted all CR's to LF's.
    //
    if (*input_ptr == U_CTL_Z)
        input_ptr--;
    if (initial_reading_of_input && option->pedantic &&
        *input_ptr != U_LINE_FEED)
    {
        ReportMessage(StreamError::LAST_CHARACTER_NOT_NEWLINE,
                      (unsigned) (input_ptr - input_buffer),
                      (unsigned) (input_ptr - input_buffer));
    }
    *(++input_ptr) = U_CARRIAGE_RETURN;
    *(++input_ptr) = U_NULL;
    input_buffer_length = input_ptr - input_buffer;
}
#endif // defined(HAVE_ENCODING)

void BaseLexStream::ReportMessage(StreamError::StreamErrorKind kind,
                              unsigned start_location,
                              unsigned end_location)
{
    if (option->tolerance != JikesOption::NO_WARNINGS ||
        kind < StreamError::DEPRECATED_IDENTIFIER_ASSERT)
    {
		StreamError* e = new StreamError();
		e->Initialize(kind, start_location, end_location);
		bad_token_stream.push_back(e);
	}
}

//
// This procedure uses a  quick sort algorithm to sort the stream ERRORS
// by their locations.
//
void BaseLexStream::SortMessages()
{
     int lower,
         upper,
         lostack[32],
         histack[32];

     int top,
         i,
         j;
	 StreamError* pivot;
	 StreamError* temp;

     top = 0;
     lostack[top] = 0;
     histack[top] = bad_token_stream.size() - 1;

     while (top >= 0)
     {
         lower = lostack[top];
         upper = histack[top];
         top--;

         while (upper > lower)
         {
             //
             // The array is most-likely almost sorted. Therefore,
             // we use the middle element as the pivot element.
             //
             i = (lower + upper) / 2;
             pivot = bad_token_stream[i];
             bad_token_stream[i] = bad_token_stream[lower];

             //
             // Split the array section indicated by LOWER and UPPER
             // using ARRAY(LOWER) as the pivot.
             //
             i = lower;
             for (j = lower + 1; j <= upper; j++)
             {
                 if (bad_token_stream[j]->start_location < pivot->start_location)
                 {
                     temp = bad_token_stream[++i];
                     bad_token_stream[i] = bad_token_stream[j];
                     bad_token_stream[j] = temp;
                 }
             }
             bad_token_stream[lower] = bad_token_stream[i];
             bad_token_stream[i] = pivot;

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

void BaseLexStream::DestroyInput()
{
	Stream::DestroyInput();

	delete[] comment_buffer;
	comment_buffer = NULL;
}

void LexStream::PrintMessages()
{

        RereadInput();
        if (option->errors)
        {
           const char* file_name = FileName();

            int error_count = NumErrorTokens(),
                warning_count = NumWarnTokens();
            if (error_count)
            {
                Coutput << endl << "Found " << error_count << " lexical error"
                        << (error_count == 1 ? "" : "s");
            }
            if (warning_count)
            {
                if (error_count)
                    Coutput << "and issued ";
                else
                    Coutput << endl << "Issued ";
                Coutput << warning_count << " lexical warning"
                        << (warning_count == 1 ? "" : "s");
            }
            if (error_count || warning_count)
                Coutput << " in \"" << file_name << "\":";

            if (! input_buffer)
            {
                int length = FileNameLength();
                wchar_t* name = new wchar_t[length + 1];
                for (int i = 0; i < length; i++)
                    name[i] = file_name[i];
                name[length] = U_NULL;
               // control.system_semantic ->
                 //   ReportSemErrorSimple(SemanticError::CANNOT_REOPEN_FILE,
                 //                  BAD_TOKEN, name);
                delete [] name;
            }
            else
            {
                for (unsigned i = 0; i < bad_token_stream.size(); i++)
                    Compiler::reportError(bad_token_stream[i]);
            }
        }
        else
        {
            for (unsigned i = 0; i < bad_token_stream.size(); i++)
                Compiler::reportError(bad_token_stream[i]);
        }
        Coutput.flush();
    
}
void BaseLexStream::SetUpComments()
{
	if (comment_buffer)
		return;
	RereadInput();
	//
	// Calculate the length of the string required to save the comments.
	// Allocate the buffer, save the comments in the buffer and update
	// their respective "string" pointer.
	//
	unsigned length = 0;
	unsigned i;

	for (i = 1; i < comment_stream.size(); i++)
		length += (comment_stream[i]->length + 1);
	comment_buffer = new wchar_t[length];
	wchar_t* ptr = comment_buffer;
	for (i = 1; i < comment_stream.size(); i++)
	{
		memcpy(ptr, &(input_buffer[comment_stream[i]->location]),
			comment_stream[i]->length * sizeof(wchar_t));
		comment_stream[i]->string = ptr;
		ptr += comment_stream[i]->length;
		*ptr++ = U_NULL;
	}
}


} // Close namespace Jikes block


