
#ifndef Openable_java_model_INCLUDED_INCLUDED
#define Openable_java_model_INCLUDED_INCLUDED
#include "IOpenable.h"
#include "JavaElement.h"
#include  "JCDT_Lib/internal/core/IProgressMonitor.hpp"
#include "IMember.h"

namespace Jikes { // Open namespace Jikes block

	

namespace JavaModel{
	
	class Openable :public JavaElement, public IOpenable
	{
	public:
		void buildStructure(bool no_lay_load,
			IProgressMonitor* monitor) override;

	
		Openable(IJavaElement* parent,
			IJavaElementType _type, wstring name);
		Openable(Openable& o);
	
		bool isOpen()
		{
			return false;
		}
		void makeConsistent(IProgressMonitor* progress) override;
		
		void openWhenClosed(IProgressMonitor* pm) override;

		IOpenable* getOpenable() override{
				return this;
		};
	
	};

	
}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

