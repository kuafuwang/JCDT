#include "stdafx.h"
#include "SearchEngine.h"
#include <SearchLib/internal/pattern/SearchPatternFactory.h>
#include <SearchLib/internal/matching/MatchLocator.h>
#include <SearchLib/internal/pattern/SearchPattern.h>
#include <JCDT_Lib/internal/core/IProgressMonitor.hpp>
#include "IJavaSearchResultCollector.h"

namespace Jikes { // Open namespace Jikes block


	
namespace Search{


	void SearchEngine::search(JavaModel::JavaModel* workspace, const std::wstring& patternString,
		int searchFor, int limitTo, IJavaSearchScope* scope, IJavaSearchResultCollector* resultCollector)
	{

		search(workspace, SearchPatternFactory::createSearchPattern(patternString, searchFor, limitTo, true), 
			scope, resultCollector);
	}

	void SearchEngine::search(JavaModel::JavaModel* workspace, JavaModel::IJavaElement* element,
		int limitTo, IJavaSearchScope* scope, IJavaSearchResultCollector* resultCollector)
	{

		search(workspace,
			SearchPatternFactory::createPattern(element, limitTo),
			scope, resultCollector);
	}

	void SearchEngine::search(JavaModel::JavaModel* workspace, ISearchPattern* searchPattern,
		IJavaSearchScope* scope, IJavaSearchResultCollector* resultCollector)
	{

			

		/* search is starting */
		resultCollector->aboutToStart();

		
		if (searchPattern == nullptr) return;

		/* initialize progress monitor */
		IProgressMonitor* progressMonitor = resultCollector->getProgressMonitor();
		if (progressMonitor != nullptr) {
			progressMonitor->beginTask("engine.searching", 100); //$NON-NLS-1$
		}

		
		int detailLevel = IInfoConstants::PathInfo | IInfoConstants::PositionInfo;
		MatchLocator* matchLocator = new MatchLocator(
			reinterpret_cast<SearchPattern*>(searchPattern), detailLevel,
			resultCollector, scope);

		
		/* eliminating false matches and locating them */
		if (progressMonitor != nullptr && progressMonitor->isCanceled())
			return;

		matchLocator->locateMatches(
			workspace,
			progressMonitor 
		);


		if (progressMonitor != nullptr && progressMonitor->isCanceled())
			return;

		if (progressMonitor != nullptr) {
			progressMonitor->done(nullptr);
		}

	//	matchLocator->locatePackageDeclarations(workspace);
		
	
		/* search has ended */
	resultCollector->done();
		
	}
}


} // Close namespace Jikes block


