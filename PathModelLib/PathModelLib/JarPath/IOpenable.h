
#ifndef IOpenable_jikes_PathModel_INCLUDED_INCLUDED
#define IOpenable_jikes_PathModel_INCLUDED_INCLUDED
#include <string>

namespace Jikes { // Open namespace Jikes block
	class IProgressMonitor;

namespace PathModel{
	class IOpenable
	{
	
	public:
		virtual ~IOpenable()
		{
		}
		
		IOpenable() : fIsStructureKnown(false)
		{

		}
		IOpenable(IOpenable& o) : fIsStructureKnown(false){

		}
		inline void LazyLoad(){
			if (!fIsStructureKnown) {
				buildStructure(nullptr);
			}
		}
	
		inline bool isStructureKnown() const{
			return fIsStructureKnown;
		};
		virtual	void buildStructure(IProgressMonitor* monitor) = 0;

		inline void setIsStructureKnown(bool newIsStructureKnown){
			fIsStructureKnown = newIsStructureKnown;
		}
	protected:
		bool fIsStructureKnown;
	
		
	};

}// Close namespace PathModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

