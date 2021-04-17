
#ifndef Jikes_CodeAssist_SymbolInfo_INCLUDED
#define Jikes_CodeAssist_SymbolInfo_INCLUDED
#include <string>
#include <JCDT_Lib/internal/lookup/LiteralValue.h>
#include <JCDT_Lib/internal/util/AccessFlagsUtil.h>
using std::wstring;
namespace Jikes { // Open namespace Jikes block


namespace CodeAssist{
	
	struct SymbolInfo
	{

		enum SymbolKind
		{
			PACKAGE,
			TYPE, // class or interface
			VARIABLE,
			CONSTANT,
			
		};
		SymbolKind kind;
		virtual ~SymbolInfo() {
		}
		SymbolInfo(SymbolKind _kind,const wstring& _name):kind(_kind), name(_name){}
		wstring name;
		const wstring& Name() const { return name; }
		virtual wstring getQualifiedName()
		{
			return name;
		}
	
	};

	struct AssistPackageInfo: public SymbolInfo
	{
		AssistPackageInfo(const wstring& _name)
			:SymbolInfo(PACKAGE,_name)
		{
			
		}
	};
	struct AssistType :public SymbolInfo
	{
		wstring pkgName;
		AssistType(const wstring& _pkgName,const wstring& _name)
			: SymbolInfo(PACKAGE, _name), pkgName(_pkgName)
		{
			
		}
		 wstring getQualifiedName() override
		{
			return  pkgName + L"/" + name;
		}
	};
	struct AssistVariable :public SymbolInfo,public AccessFlags
	{
		
		wstring m_strTypeDesc;
		wstring claName;
		wstring pkgName;
	
	

		unsigned const_val;
		AssistVariable(const wstring& _name  ) 
			: SymbolInfo(VARIABLE, _name), AccessFlags(),const_val(0)

		{
		}
		wstring getQualifiedName() override
		{
			return  pkgName + L"/" + claName + L"/" + name;
		}
	};

	struct AssistConstantInfo :public SymbolInfo
	{
	
		AssistConstantInfo(const wstring& _name)
			:SymbolInfo(CONSTANT, _name)
		{
		}
		~AssistConstantInfo(){
		
		}
	};
}// Close namespace Jikes block
	

} // Close namespace CodeAssist block


#endif // _INCLUDED

