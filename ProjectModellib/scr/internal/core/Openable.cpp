#include "stdafx.h"
#include <JavaModelLib/internal/core/JavaElement.h>
#include <JavaModelLib/internal/core/IOpenable.h>
#include <JCDT_Lib/internal/core/IProgressMonitor.hpp>
#include <JavaModelLib/internal/core/Openable.h>
#include <JavaModelLib/internal/core/IJavaProject.h>


namespace Jikes { // Open namespace Jikes block


	namespace JavaModel {


		void Openable::buildStructure(bool no_lay_load, IProgressMonitor* monitor)
		{
			if (monitor&& monitor->isCanceled())
				return;
			
			setIsStructureKnown(false);
			if (fLEType != IJavaElementType::JAVA_MODEL)
			{
				IJavaProject* proj = nullptr;
				if (!is_Clone) 
				{
					proj = getJavaProject();
					if(proj)
					{
						proj->beginToOccupy();
						if (fIsStructureKnown) {
							proj->endToOccupy();
							return;
						}
					}
				}

				removeInfo();
				
				setIsStructureKnown(generateInfos(monitor));
				if (proj) {
					proj->endToOccupy();
				}
			}
			else
			{
				removeInfo();
				
				setIsStructureKnown(generateInfos(monitor));
			}
			
		}
	
	Openable::Openable(IJavaElement* parent,
		IJavaElementType _type,  wstring name)
		: JavaElement(parent,_type, name), IOpenable()
	{

	}

	Openable::Openable(Openable& o): JavaElement(o), IOpenable(o)
	{
	}

	

	
	void Openable::makeConsistent(IProgressMonitor* progress)
	{
		buildStructure(false,progress);
	}
	void Openable::openWhenClosed(IProgressMonitor* pm)
	{
		// 1) Parent must be open - open the parent if necessary
		// 2) create the new element info and open a buffer if needed
		// 3) build the structure of the openable
		buildStructure(false,pm);

	}

	
}// Close namespace JavaModel block
	

} // Close namespace Jikes block

