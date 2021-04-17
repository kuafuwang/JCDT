
#ifndef SourceField_java_model_INCLUDED_INCLUDED
#define SourceField_java_model_INCLUDED_INCLUDED

#include "IField.h"
namespace Jikes { // Open namespace Jikes block
	

namespace JavaModel{
	
	class SourceField:
		 public IField 
	{
	public:
		IField* ShallowCopy(IJavaElement* parent) override;

		SourceField(IJavaElement* parent, const wstring& name, IJavaElementType _type);
		SourceField(SourceField&o);
		
	};

	
}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

