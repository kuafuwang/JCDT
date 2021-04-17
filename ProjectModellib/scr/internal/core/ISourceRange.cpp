#include "stdafx.h"
#include <assert.h>
#include <JavaModelLib/internal/core/ISourceRange.h>
namespace Jikes { // Open namespace Jikes block


	namespace JavaModel {

Jikes::JavaModel::ISourceRange::ISourceRange(): m_nStartIndex(UNKNOWN_FLAGE), m_nStartLine(UNKNOWN_FLAGE),
m_nEndIndex(UNKNOWN_FLAGE), m_nEndLine(UNKNOWN_FLAGE)
{

}

		ISourceRange::ISourceRange(int nStartIndex,int nStartLine,int nEndIndex,int nEndLine):
	m_nStartIndex(nStartIndex),
	m_nStartLine(nStartLine),
	m_nEndIndex(nEndIndex), 
	m_nEndLine(nEndLine)

{
	
	return;
}

ISourceRange::ISourceRange(const ISourceRange & decl)
{
	if (this == &decl)
		return;

	m_nStartIndex = decl.m_nStartIndex;
	m_nStartLine  = decl.m_nStartLine;
	m_nEndIndex   = decl.m_nEndIndex;
	m_nEndLine    = decl.m_nEndLine;
}

ISourceRange &ISourceRange::operator=(const ISourceRange & decl)
{
	if (this == &decl)
		return *this;


	m_nStartIndex = decl.m_nStartIndex;
	m_nStartLine  = decl.m_nStartLine;
	m_nEndIndex   = decl.m_nEndIndex;
	m_nEndLine    = decl.m_nEndLine;

	return *this;
}

bool ISourceRange::operator>(ISourceRange declBound)
{
	if(m_nStartLine > declBound.m_nEndLine)
		return true;
	if(m_nStartLine == declBound.m_nEndLine)
		if(m_nStartIndex>declBound.m_nEndIndex)
			return true;
	return false;
}

bool ISourceRange::operator<(ISourceRange declBound)
{
	if(m_nEndLine < declBound.m_nStartLine)
		return true;
	if(m_nEndLine == declBound.m_nStartLine)
		if(m_nEndIndex<declBound.m_nStartIndex)
			return true;
	return false;
}


void ISourceRange::Set(int nStartIndex,int nStartLine,int nEndIndex,int nEndLine)
{
	m_nStartIndex = nStartIndex;m_nStartLine = nStartLine;
	m_nEndIndex   = nEndIndex;m_nEndLine = nEndLine;
}

bool ISourceRange::operator>(__DeclPoint pt)
{
	if (m_nStartLine > pt.m_nLine)return true;

	if ((m_nStartLine == pt.m_nLine) && (m_nStartIndex > pt.m_nIndex))return true;

	return false;
}

bool ISourceRange::operator==(ISourceRange declBound)
{
	if(m_nStartIndex == declBound.m_nStartIndex)
		if(m_nStartLine == declBound.m_nStartLine)
			if(m_nEndIndex == declBound.m_nEndIndex)
				if(m_nEndLine == declBound.m_nEndLine)
					return true;
	return false;
}


bool ISourceRange::operator<(__DeclPoint pt)
{
	if(m_nEndLine < pt.m_nLine)return true;

	if((m_nEndLine == pt.m_nLine) && (m_nEndIndex < pt.m_nIndex))return true;

	return false;
}

bool ISourceRange::PointInDeclBlock(__DeclPoint& declPt)
{
	assert(false);
	return false;
}

bool ISourceRange::PointInDecl(__DeclPoint& declPt)
{
	if(m_nStartLine == m_nEndLine)
	{
		if(m_nStartLine == declPt.m_nLine)
		{
			if((declPt.m_nIndex>=m_nStartIndex)&&(declPt.m_nIndex<=m_nEndIndex))
				return true;
			else
				if((declPt.m_nIndex == m_nStartIndex))//must be '{'or '}'
					return true;
		}
		else
			return false;
	}
	else
	{
		if((declPt.m_nLine>m_nStartLine)&&(declPt.m_nLine<m_nEndLine))
			return true;
		else
			if((declPt.m_nLine == m_nStartLine)&&(declPt.m_nIndex>=m_nStartIndex))
				return true;
			else
				if((declPt.m_nLine == m_nEndLine)&&(declPt.m_nIndex<=m_nEndIndex))
					return true;  
	}
	return false;
}

bool ISourceRange::BoundInDecl(ISourceRange& declBound)
{
	if(declBound.m_nEndLine == -1)//insert context
	{
		if(m_nStartLine == m_nEndLine)
		{
			if(m_nStartLine == declBound.m_nStartLine)
			{
				if((declBound.m_nStartIndex>=m_nStartIndex)&&(declBound.m_nStartIndex<=m_nEndIndex))
					return true;
			}
			else
				return false;
		}
		else
		{
			if(declBound.m_nStartLine>m_nStartLine&&declBound.m_nStartLine<m_nEndLine)
				return true;
			else
				if((declBound.m_nStartIndex>=m_nStartIndex)&&(declBound.m_nStartLine == m_nStartLine))
					return true;
				else
					if((declBound.m_nStartIndex<=m_nEndIndex)&&(declBound.m_nStartLine == m_nEndLine))
						return true;
		}
	}
	else              //delete context
	{
		//swallowed by multi-line delete
		if(
			((declBound.m_nStartLine < m_nStartLine)||
			((declBound.m_nStartLine == m_nStartLine)&&
			(declBound.m_nStartIndex < m_nStartIndex)))
			&&
			((declBound.m_nEndLine > m_nEndLine)||((declBound.m_nEndLine == m_nEndLine)
			&&(declBound.m_nEndIndex > m_nEndIndex)))
			)
			return true;
		__DeclPoint nPt(declBound.m_nStartIndex,declBound.m_nStartLine);
		__DeclPoint nePt(declBound.m_nEndIndex,declBound.m_nEndLine);
		return(PointInDecl(nPt)||PointInDecl(nePt));
	}return false;
}

bool ISourceRange::BoundInDecl(int nStartIndex,int nStartLine,int nEndIndex,int nEndLine)
{
	if(nEndLine == -1)//insert context
	{
		if(m_nStartLine == m_nEndLine)
		{
			if(m_nStartLine == nStartLine)
			{
				if((nStartIndex>=m_nStartIndex)&&(nStartIndex<=m_nEndIndex))
					return true;
			}
			else
				return false;
		}
		else
		{
			if(nStartLine>m_nStartLine&&nStartLine<m_nEndLine)
				return true;
			else
				if((nStartIndex>=m_nStartIndex)&&(nStartLine == m_nStartLine))
					return true;
				else
					if((nStartIndex<=m_nEndIndex)&&(nStartLine == m_nEndLine))
						return true;
		}
	}
	else              //delete context
	{
		//swallowed by multi-line delete
		if(
			((nStartLine < m_nStartLine)||
			((nStartLine == m_nStartLine)&&
			(nStartIndex < m_nStartIndex)))
			&&
			((nEndLine > m_nEndLine)||((nEndLine == m_nEndLine)
			&&(nEndIndex > m_nEndIndex)))
			)
			return true;
		__DeclPoint nPt(nStartIndex,nStartLine);
		__DeclPoint nePt(nEndIndex,nEndLine);
		return(PointInDecl(nPt)||PointInDecl(nePt));
	}
	return false;
}


}// Close namespace JavaModel block


} // Close namespace Jikes block
