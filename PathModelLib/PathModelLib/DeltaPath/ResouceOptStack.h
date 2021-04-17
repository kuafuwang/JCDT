
#ifndef ResouceOptStack_javamodel_INCLUDED_INCLUDED
#define ResouceOptStack_javamodel_INCLUDED_INCLUDED
#include <vector>
#include <string>
#include <set>
#include "NameLookUpNotify.h"
#include <windows.h>
using std::map;
using std::vector;
using std::set;

namespace Jikes { // Open namespace Jikes block
	namespace PathModel {
		
		struct NotifyResult
		{
			typedef  map<ResourceOptKind, map<wstring, NameLookUpNotify*> > RootT;

			typedef map<wstring, map<ResourceOptKind, 
				map<wstring, NameLookUpNotify*> > > PkgT;
				
			typedef map<wstring, map<wstring, map<ResourceOptKind,
				map<wstring, NameLookUpNotify*> > >  > FileT;

			
			CompilerAct act = INCREMENT;
			RootT  _roots;
			PkgT	_pkgs;
			FileT _files;
			virtual ~NotifyResult(){	
			}
		};

		struct NotifyResult_For_Origianal:public NotifyResult
		{

			map<FileMsg*,wstring> add_file_set;
			map<FileMsg*, wstring> chang_file_set;
			map<FileMsg*, wstring> remove_file_set;

			vector<NameLookUpNotify*> opt_stack;
			vector<NameLookUpNotify*>* all_notify = nullptr;
			virtual ~NotifyResult_For_Origianal()
			{
				if (all_notify) {
					delete all_notify;
				}
				auto _size = opt_stack.size();
				for (size_t i = 0; i < _size; ++i) {
					delete opt_stack[i];
				}
				opt_stack.clear();
			}
		};
		class ResouceOptStack
		{
		public:
			void PushNotify(vector<NameLookUpNotify*>& _notifys);
			void PushNotify(NameLookUpNotify* _notisfy);
			static NotifyResult* GetClassifyNotify(vector<NameLookUpNotify*>& stack, bool for_orginal);
			void Clear();
			~ResouceOptStack();
			
			bool empty() const;
			void handOutStack(vector<NameLookUpNotify*>& _stack);

			vector<NameLookUpNotify*> stack;
		};


		inline bool ResouceOptStack::empty() const
		{
			 return  stack.empty();
		}
		inline void  ResouceOptStack::handOutStack(vector<NameLookUpNotify*>& _stack)
		{
			_stack.swap(stack);
			Clear();
		}
		inline void ResouceOptStack::Clear() {
			auto _size = stack.size();
			for (size_t i = 0; i < _size; ++i)
				delete stack[i];
			stack.clear();
		}

		inline void ResouceOptStack::PushNotify(std::vector<NameLookUpNotify*>& _notifys){
			for (auto it : _notifys) {
				stack.push_back(it->Clone());
			}	
		}

		inline void ResouceOptStack::PushNotify(NameLookUpNotify* _notisfy) {
			stack.push_back(_notisfy);	
		}


	
	}
	

} // Close namespace Jikes block


#endif // _INCLUDED

