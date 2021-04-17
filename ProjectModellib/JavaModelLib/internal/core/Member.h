
#ifndef Member_java_model_INCLUDED_INCLUDED
#define Member_java_model_INCLUDED_INCLUDED

#include "IMember.h"
#include "JavaElement.h"

namespace Jikes { // Open namespace Jikes block


namespace JavaModel{

	class IPackageFragment;
	class MemberElementInfo;
	class Member 
		:  public JavaElement, public IMember, public IJavaCardDocType
	{
	public:
		void GetIndendentInfoAndSepareteFromParent() override{
			if (!fParent)return;

			getMosesPath();
			if (isBinary()) {
				getSourceRange();
			}
			fParent = nullptr;
		}
		wstring getOutLineName() override {
			return fName;
		}
		wstring getOutLineIdentity()  override {
			wchar_t temp[8];
			int idx = wsprintfW(temp, L"%d", fOccurrenceCount);
			temp[idx] = 0x00;
			return fName + temp;
		}
		Member(IJavaElement* parent, IJavaElementType i_java_element_type, const wstring& name)
			: JavaElement(parent, i_java_element_type, name)
		{
		}

		Member(Member& o);
		void figureRange() override;
		virtual ~Member();
		IType* getDeclaringType() override;

		
		ISourceRange* getSourceRange() override;
		virtual  void  setSourceRange(ISourceRange*) override;
	
		ISourceRange* getNameRange() override;
		virtual  void  setNameRange(ISourceRange*) override;

	


		void setAccessFlags(unsigned flag) override;
		const std::vector<std::wstring>* const getModfiers() override;
		wstring getModifersString(wchar_t seperator = ' ') override;
		AccessFlags getAccessFlags() override;
		std::wstring getJavaCardDocUrlFragment() override{
			return L"";
		};
	};

	
}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

