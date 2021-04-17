#ifndef  SearchResultNode_JIKES_SERACH_INCLUDE
#define SearchResultNode_JIKES_SERACH_INCLUDE
#include <vector>
#include <map>
#include <JCDT_Lib/internal/lookup/Symbol.h>

namespace Jikes { // Open namespace Jikes block
	namespace PathModel
	{
		class MosesPath;
	}
	namespace  JavaModel
	{
		class IJavaElement;
		class ISourceRange;
	}
namespace Search{
	class MemberInFileElemnt;
	class SearchMatch;

	class FileResultElement {
	public:
		PathModel::MosesPath* resouce;
		std::vector<MemberInFileElemnt*> result_in_file;
		int count;
		FileResultElement();
		~FileResultElement();
	};

	class MemberInFileElemnt{
	public:
		~MemberInFileElemnt();
		MemberInFileElemnt();

		int result_count;
		JavaModel::IJavaElement* element;
		
		std::vector<SearchMatch*> result_in_member;
	};

	class ReferenceMemberInFileElemnt;

	class ReferenceFileResultElement 	
	{
	public:
		PathModel::MosesPath* resouce;

		std::map<Symbol::SymbolKind, vector< ReferenceMemberInFileElemnt* > > result_in_file;
		int count;
		ReferenceFileResultElement();
		~ReferenceFileResultElement();
	};

	class ReferenceMemberInFileElemnt 
		{
	public:
		~ReferenceMemberInFileElemnt();
		ReferenceMemberInFileElemnt();

		int result_count;
		JavaModel::IJavaElement* element;
	};


	enum  FieldReferenceAttribute
	{
		WRITE_VALUE = 5,
		READ_VALUE = 6,
	};
	class  SearchMatch
	{
	public:
		SearchMatch(JavaModel::ISourceRange* _rng, int _accuracy, std::wstring&& _strContext,int  _extra_info);
		~SearchMatch();
		JavaModel::ISourceRange* rng;

		int accuracy;
		//此行的内容
		std::wstring m_strContext;
		int  extra_info;

	};

}// Close namespace Search block
	

} // Close namespace Jikes block

#endif