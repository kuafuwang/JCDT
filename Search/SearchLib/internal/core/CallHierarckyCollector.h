#ifndef  CallHierarckyCollector_JIKES_SERACH_INCLUDE
#define CallHierarckyCollector_JIKES_SERACH_INCLUDE
#include "IJavaSearchResultCollector.h"
#include <vector>

namespace Jikes { // Open namespace Jikes block

namespace Search{
	class FileResultElement;
	class CallHierarckyCollector :
		public IJavaSearchResultCollector
	{
	public:
		CallHierarckyCollector();
	
		JavaModel::IJavaElement* search_element;
		std::vector<  FileResultElement*  > all_result;
		~CallHierarckyCollector();

		void aboutToStart() override
		{
		};
		int  getResultCount() {
			return resultCount;
		};

		virtual void accept(FileResultElement* elemt) override;

		void done() override {

		}
		IProgressMonitor* getProgressMonitor() override {
			return nullptr;
		}

		bool IsCopyChildren() override{
			return  false;
		};
	private :
		int resultCount;
	};
}// Close namespace Search block
	

} // Close namespace Jikes block

#endif