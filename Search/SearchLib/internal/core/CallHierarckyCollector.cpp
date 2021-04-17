#include "stdafx.h"
#include "CallHierarckyCollector.h"
#include <JavaModelLib/internal/core/CompilationUnit.h>
#include <SearchLib/internal/matching/SearchResultNode.h>

namespace Jikes { // Open namespace Jikes block


	
namespace Search{
	CallHierarckyCollector::CallHierarckyCollector(): search_element(nullptr), resultCount(0)
	{

	}

	CallHierarckyCollector::~CallHierarckyCollector(){
		for (auto it : all_result){
			delete it;
		}
		if(search_element){
			delete search_element;
		}
	}

	void CallHierarckyCollector::accept(FileResultElement* elemt){
		if (!elemt)
			return;

		all_result.push_back(elemt);
		resultCount += elemt->count;
	}
}


} // Close namespace Jikes block


