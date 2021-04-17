#ifndef  JavaSearchScope_JIKES_SERACH_INCLUDE
#define JavaSearchScope_JIKES_SERACH_INCLUDE

#include <boost/shared_ptr.hpp>
#include "IJavaSearchScope.h"
#include <set>
namespace Jikes { // Open namespace Jikes block
	namespace JavaModel
	{
		class OriginalJavaProject;
		class JavaModel;
	}
namespace Search{
	class JavaSearchScope:public IJavaSearchScope
	{
	public:
		void setLimitTo(int, bool) override;
		const std::vector<GroupInfo*>& getPaths() override;
		JavaSearchScope();
		~JavaSearchScope();
		
		static const wchar_t* NO_BELONG_TO_ANY_PROJECT;
		bool IsLimitTo(int limit) override;
		void setLimitTo(std::map<int, bool>&) override;
		std::map<int, bool>& getLimits() override;
		bool encloses(const wstring& resourcePath) override;

		void add(JavaModel::JavaModel* pJm);

		void add(const wstring& project, JavaModel::JavaModel* pJm);

		void add(std::vector<wstring>& files);
		void add(GroupInfo& files);

	protected:
		std::map<int, bool> limits;
		std::vector<GroupInfo*> _paths;
		std::vector<  std::pair< wstring, bool> > _record;

		bool IsDuplicate(const wstring& fileCandidate);
		void ProcessProjectPath(std::set<wstring>& project, JavaModel::JavaModel* pJm);

		GroupInfo* FindGroupOrInGroup(const wstring& groupName);
	};
}// Close namespace Search block
	

} // Close namespace Jikes block

#endif