
#ifndef IField_java_model_INCLUDED_INCLUDED
#define IField_java_model_INCLUDED_INCLUDED
#include "Member.h"
#include "IJavaElementType.h"
#include <JCDT_Lib/internal/lookup/LiteralValue.h>
#include <JavaModelLib/internal/info/SourceFieldElementInfo.h>

namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	
	class IField : public Member
	{
	public:
		PathModel::MosesPath* getMosesPath() override;
		void GetIndendentInfoAndSepareteFromParent() override;
		IJavaElement* DeepCopy(IJavaElement*) override;
		wstring getOutLineName() override;
		wstring getFullName() override;
		bool IsLocal() const
		{
			return fLEType == LOCAL_FIELD;
		}

		IField(IJavaElement* parent, const wstring& name, IJavaElementType _type);

		virtual ~IField();

		IField(IField& o);
		
		void SetType(const wstring& type)
		{
			auto _info = reinterpret_cast<SourceFieldElementInfo*>( getElementInfo() );
			_info->_type = type;
		}
	
	protected:
		virtual void HelpToDeepCopy(IField&);
	
		SourceFieldElementInfo* info;
	public:
		MemberElementInfo* getElementInfo() override{
			if (!info){
				info = new SourceFieldElementInfo();
			}
			return info;
		}


		std::wstring getJavaCardDocUrlFragment() override;
	};

	
}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

