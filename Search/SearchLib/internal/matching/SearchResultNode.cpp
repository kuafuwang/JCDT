#include "stdafx.h"
#include "SearchResultNode.h"
#include <JavaModelLib/internal/core/ISourceRange.h>
#include <JavaModelLib/internal/core/IJavaElement.h>
#include <PathModelLib/MosesPath/MosesPath.h>

namespace Jikes { // Open namespace Jikes block


	
namespace Search{
	FileResultElement::FileResultElement():resouce(nullptr), count(0)
	{
	}

	FileResultElement::~FileResultElement()
	{
		for(auto it  : result_in_file)
		{
			delete it;
		}
		result_in_file.clear();
		delete resouce;
		resouce = nullptr;
	}

	MemberInFileElemnt::~MemberInFileElemnt()
	{
		delete element;
		element = nullptr;
		for(auto it : result_in_member)
		{
			delete it;
		}
		result_in_member.clear();
	}

	MemberInFileElemnt::MemberInFileElemnt():element(nullptr), result_count(0)
	{
		

	}

	ReferenceFileResultElement::ReferenceFileResultElement(): resouce(nullptr), count(0)
	{

	}

	ReferenceFileResultElement::~ReferenceFileResultElement()
	{
		for (auto it : result_in_file)
		{
			for(auto secondIt : it.second)
			{
				delete secondIt;
			}
		}
		result_in_file.clear();
		delete resouce;
	}

	ReferenceMemberInFileElemnt::~ReferenceMemberInFileElemnt()
	{
	}

	ReferenceMemberInFileElemnt::ReferenceMemberInFileElemnt(): result_count(0), element(nullptr)
	{
	}

	SearchMatch::SearchMatch(JavaModel::ISourceRange* _rng, int _accuracy, std::wstring&& _strContext,int  _extra_info):
		rng(_rng), accuracy(_accuracy), extra_info(_extra_info)
	{
		m_strContext.swap(_strContext);
	}

	SearchMatch::~SearchMatch()
	{
		delete rng;
		rng = nullptr;
	}
}


} // Close namespace Jikes block


