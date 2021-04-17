
#ifndef ISourceRange_java_model_INCLUDED_INCLUDED
#define ISourceRange_java_model_INCLUDED_INCLUDED

namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	

	class ISourceRange
	{

	public:
		static const int  UNKNOWN_FLAGE = -1;
		virtual ~ISourceRange()
		{
		}

		class __DeclPoint  
		{
		public:
			// ×Ö·ûÐòºÅ;
			int            m_nIndex;
			// ÐÐºÅ;
			int            m_nLine;

			__DeclPoint() {
				m_nIndex = -1;
				m_nLine = -1;
			}
			__DeclPoint(int nIndex, int nLineIndex) {
				m_nIndex = nIndex;
				m_nLine = nLineIndex;
			}

			__DeclPoint(const __DeclPoint & decl) {
				if (&decl == this)
					return;
				m_nIndex = decl.m_nIndex;
				m_nLine = decl.m_nLine;
			}
			__DeclPoint &operator=(const __DeclPoint & decl) {
				if (&decl == this)
					return *this;

				m_nIndex = decl.m_nIndex;
				m_nLine = decl.m_nLine;

				return *this;
			}

			void Set(int nIndex, int nLineIndex) {
				m_nIndex = nIndex;
				m_nLine = nLineIndex;
			}

			~__DeclPoint() {

			}

		
		};

	public:
	
		virtual ISourceRange* Clone()
		{
			auto  clone = new ISourceRange(m_nStartIndex, m_nStartLine
				, m_nEndIndex, m_nEndLine);
			return clone;
		}
		int            m_nStartIndex;
		int            m_nStartLine;
		int            m_nEndIndex;
		int            m_nEndLine;

		ISourceRange();
		ISourceRange(int nStartIndex, int nStartLine, int nEndIndex, int nEndLine);
		ISourceRange(const ISourceRange &);
		ISourceRange & operator=(const ISourceRange &);
		bool operator>(ISourceRange dBound);
		bool operator<(ISourceRange dBound);
		bool operator>(__DeclPoint dPt);
		bool operator<(__DeclPoint dPt);
		bool operator==(ISourceRange declBound);

		bool PointInDecl(__DeclPoint& declPt);
		bool BoundInDecl(ISourceRange& declBound);
		bool BoundInDecl(int nStartIndex, int nStartLine, int nEndIndex, int nEndLine);
		bool PointInDeclBlock(__DeclPoint& declPt);
		void Set(int nStartIndex, int nStartLine, int nEndIndex, int nEndLine);

		inline int  height() { return (m_nEndLine - m_nStartLine); }
		inline int  width() { return (m_nEndIndex - m_nStartIndex); }

	
	};
}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

