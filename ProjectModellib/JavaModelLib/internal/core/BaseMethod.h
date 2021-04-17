
#ifndef BaseMethod_java_model_INCLUDED_INCLUDED
#define BaseMethod_java_model_INCLUDED_INCLUDED
#include "IMethod.h"
#include <vector>
#include <map>
#include <JavaModelLib/internal/info/SourceMethodElementInfo.h>


using std::vector;
namespace Jikes { // Open namespace Jikes block


namespace JavaModel{

	class IType;

	class BaseMethod
		:public IMethod
	{
	public:
		vector<wstring> formal_parameterTypes;
		 bool equals(IJavaElement* o) override;
		void MarkConstructor(bool) override;
		bool isConstructor() override;
		PathModel::MosesPath* getMosesPath() override;
		const wstring getMethodNameWithParameterNames() override;
		void HelpToDeepCopy(BaseMethod& o);
		IJavaElement* DeepCopy(IJavaElement*) override;
		unsigned getNumParameterTypes() override;
		IField* getLocalField(const wstring& name, const wstring& declareType) override;

		wstring getFullName() override;

		void addChild(IJavaElement* child) override;
		~BaseMethod();
		BaseMethod(IJavaElement* p,const wstring& name, const vector<wstring>& parameterTypes);
		 BaseMethod(BaseMethod& o);
	

		unsigned char status;
		bool _isConstructor;
		// The return type of methods, and the containing type of constructors.
		
			wstring& getReturnType() override;

		void  SetReturnType(const wstring& _type);
	
		const vector<wstring>& getParameterTypes() override;

		const vector<wstring>& getParameterNames() override;

		
		void SetParamenterNames(vector<wstring>& names)
		{
			auto _info = reinterpret_cast<SourceMethodElementInfo*>(getElementInfo());
			_info->paramenterNames = names;
		}

		MemberElementInfo* getElementInfo() override
		{
			if (!info){
				// ªÒ»°info
				info = new  SourceMethodElementInfo();
			}
			return info;
		}


		std::wstring getJavaCardDocUrlFragment() override;
	protected:
		SourceMethodElementInfo* info;
	};


	inline void BaseMethod::SetReturnType(const wstring& _type)
	{
		auto _info = reinterpret_cast<SourceMethodElementInfo*>(getElementInfo());
		_info->return_type = _type;
	}


	

}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

