
#ifndef IOpenable_java_model_INCLUDED_INCLUDED
#define IOpenable_java_model_INCLUDED_INCLUDED

#include  "JCDT_Lib/internal/core/IProgressMonitor.hpp"

namespace Jikes { // Open namespace Jikes block


namespace JavaModel{

	class  IOpenable 
	{
	public:
		virtual ~IOpenable()
		{
		}
		IOpenable(IOpenable& o):is_Clone(false),fIsStructureKnown(false)
		{
			
		}
		IOpenable() :is_Clone(false), fIsStructureKnown(false)
		{
			
		}
		inline void LazyLoad()
		{
			if (!fIsStructureKnown) {
				buildStructure(false, nullptr);
			}
		}
		virtual bool isOpen() = 0;
	

		bool isStructureKnown() const
		{
			return fIsStructureKnown;
		} ;
	    virtual	void buildStructure(bool no_lay_load, IProgressMonitor* monitor) = 0 ;
		
		void setIsStructureKnown(bool newIsStructureKnown)
		{
			fIsStructureKnown = newIsStructureKnown;
		}


		void MarkClone() { is_Clone = true; }
		bool isClone() const {
			return is_Clone;
		}
		bool is_Clone;

		virtual void removeInfo() = 0;


		virtual bool generateInfos(IProgressMonitor* pm) = 0;

	
		virtual void makeConsistent(IProgressMonitor* progress )= 0;

		virtual void openWhenClosed(IProgressMonitor* pm) = 0;
		


	protected:
		bool fIsStructureKnown = false;


	};

}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

