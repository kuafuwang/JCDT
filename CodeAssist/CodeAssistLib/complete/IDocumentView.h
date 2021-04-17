
#ifndef IDocumentView_Jikes_CodeAssist_INCLUDED_INCLUDED
#define IDocumentView_Jikes_CodeAssist_INCLUDED_INCLUDED
#include <string>
using  std::wstring;
namespace Jikes { // Open namespace Jikes block


namespace CodeAssist{
	
	class IDocumentView
	{
	public:
	 virtual ~IDocumentView(){}
		/* --------------- text access and manipulation --------------------------- */
		/**
		* Returns the character at the given document offset in this document.
		*
		* @param offset a document offset
		* @return the character at the offset
		* @exception BadLocationException if the offset is invalid in this document
		*/
	 virtual	char getChar(int offset) = 0 ;

		/**
		* Returns the number of characters in this document.
		*
		* @return the number of characters in this document
		*/
	 virtual	int getLength() = 0;

		/**
		* Returns this document's complete text.
		*
		* @return the document's complete text
		*/
	 virtual	const char* get() = 0;

		/**
		* Returns this document's text for the specified range.
		*
		* @param offset the document offset
		* @param length the length of the specified range
		* @return the document's text for the specified range
		* @exception BadLocationException if the range is invalid in this document
		*/
	 virtual wstring get(int offset, int length)  =0 ;



		/**
		* Subsitutes the given text for the specified document range.
		* Sends a <code>DocumentEvent</code> to all registered <code>IDocumentListener</code>.
		*
		* @param offset the document offset
		* @param length the length of the specified range
		* @param text the substitution text
		* @exception BadLocationException if the offset is invalid in this document
		*
		* @see DocumentEvent
		* @see IDocumentListener
		*/
	 virtual 	void replace(int offset, int length, const wstring& text) = 0;




		/* ---------------------- line information -------------------------------- */

		/**
		* Returns the length of the given line including the line's delimiter.
		*
		* @param line the line of interest
		* @return the length of the line
		* @exception BadLocationException if the line number is invalid in this document
		*/
	 virtual int getLineLength(int line) = 0  ;

		/**
		* Returns the number of the line at which the character of the specified position is located.
		* The first line has the line number 0. A new line starts directly after a line
		* delimiter. <code>(pos == document length)</code> is a valid argument also there is no
		* corresponding character.
		*
		* @param offset the document offset
		* @return the number of the line
		* @exception BadLocationException if the offset is invalid in this document
		*/
	 virtual 	int getLineOfOffset(int offset)  =0  ;

		/**
		* Determines the offset of the first character of the given line.
		*
		* @param line the line of interest
		* @return the document offset
		* @exception BadLocationException if the line number is invalid in this document
		*/
	 virtual 	int getLineOffset(int line) = 0;


	};
}// Close namespace Jikes block
	

} // Close namespace CodeAssist block


#endif // _INCLUDED

