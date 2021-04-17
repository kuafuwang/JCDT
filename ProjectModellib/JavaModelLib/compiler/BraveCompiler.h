
#ifndef BraveCompiler_java_model_INCLUDED_INCLUDED
#define BraveCompiler_java_model_INCLUDED_INCLUDED
#include <JCDT_Lib/compiler/Compiler.h>
#include <JavaModelLib/impl/config.h>
#include "BuildMsgInfo.h"
#include <JCDT_Lib/internal/env/LookupEnvironment.h>

namespace Jikes { // Open namespace Jikes block
	class INameEnvironment;

namespace JavaModel{
	
	class BraveCompiler :public Compiler
	{
	public:
		BraveCompiler(INameEnvironment* i_name_environment, Option* jikes_option);
		~BraveCompiler();
		void ParseStage();
		void BindingStage();
		void Compile() ;
	
		void Clear();
	
	
		const vector<FileSymbol*>& getAffectFileSymbol() const;
		
		Semantic*  getSymstemError();
		
		set<wstring> add_file_set;
		set<wstring> chang_file_set;
		set<wstring> remove_file_set;
		set<wstring> file_has_error_set;

		void CalculateIsNeedToBuld()
		{
			need_to_build = false;
			if (!add_file_set.empty())
				need_to_build = true;
			if (!chang_file_set.empty())
				need_to_build = true;
			if (!remove_file_set.empty())
				need_to_build = true;

			
		}
		bool IsNeedToBuild() const
		{
			return need_to_build;
		}
	private:
		
		bool need_to_build = false;

	};




	inline const vector<FileSymbol*>& BraveCompiler::getAffectFileSymbol() const
	{
		return lookupEnvironment->input_files;
	}
}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

