
#ifndef Initializer_java_model_INCLUDED_INCLUDED
#define Initializer_java_model_INCLUDED_INCLUDED
#include "IInitializer.h"
#include <JavaModelLib/internal/info/InitializerElementInfo.h>


namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	
	class IType;
	class Initializer: public IInitializer{
	public:
		IJavaElement* DeepCopy(IJavaElement*) override;
		IJavaElement* ShallowCopy(IJavaElement* parent) override;
		Initializer(IType* parent, int occurrenceCount);
		Initializer(Initializer& o);
		MemberElementInfo* getElementInfo() override {
			if(!info){
				info = new InitializerElementInfo();
			}
			return info;
		}
		void addChild(IJavaElement* child) override;
		IField* getLocalField(const wstring& name, const wstring& declareType) override;
		int start_pos;
	protected:
		InitializerElementInfo* info;
		
	};

	
}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

