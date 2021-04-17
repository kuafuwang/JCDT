
#ifndef SourceRefElementInfo_java_model_INCLUDED_INCLUDED
#define SourceRefElementInfo_java_model_INCLUDED_INCLUDED
#include "JavaElementInfo.h"
#include <JavaModelLib/internal/core/ISourceRange.h>

namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	class ISourceRange;
	class SourceRefElementInfo : public JavaElementInfo
	{
	public:

		~SourceRefElementInfo();
		SourceRefElementInfo();
		ISourceRange* getSourceRange(){
			if (!source_range)
				source_range =  new ISourceRange(-1, -1, -1, -1);
			return  source_range;
		}
		ISourceRange* getNameRange(){
			if(!name_range)
				 name_range = new  ISourceRange(-1, -1, -1, -1);
			return  name_range;
		}
		void HelpToDeepCopy(SourceRefElementInfo& o)
		{
			if(o.source_range){
				delete source_range;
				source_range = o.source_range->Clone();
			}
			if (o.name_range) {
				delete name_range;
				name_range = o.name_range->Clone();
			}
			
		};
		void  setSourceRange(ISourceRange* _rng) {
			delete source_range;
			source_range = _rng;

		}
		void  setNameRange(ISourceRange* _rng) {
			delete name_range;
			name_range = _rng;
		}
	private:
		ISourceRange* source_range;
		ISourceRange* name_range;
	};

	inline SourceRefElementInfo::~SourceRefElementInfo()
	{
		if(source_range)
		{
			delete source_range;
		}
		if(name_range)
		{
			delete name_range;
		}
	}

	inline SourceRefElementInfo::SourceRefElementInfo(): source_range(nullptr), name_range(nullptr)
	{
	
	}
}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

