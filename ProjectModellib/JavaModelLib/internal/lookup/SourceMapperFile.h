
#ifndef SourceMapperFile_java_model_INCLUDED_INCLUDED
#define SourceMapperFile_java_model_INCLUDED_INCLUDED
#include <JCDT_Lib/internal/impl/FileBase.h>


namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	
	class SourceMapperFile :public FileBase
	{
	public:
		wstring GetUnicodeFileName() override;

		~SourceMapperFile() override;

		SourceMapperFile(const string& _path);

		Zip* Zipfile() override {
			return nullptr;
		};
		const char* FileName() override {
			return fileName.c_str();
		};
		unsigned FileNameLength() override {
			return fileName.size();
		};
		bool IsZip() override {
			return false;
		};

		string fileName;
	};

}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

