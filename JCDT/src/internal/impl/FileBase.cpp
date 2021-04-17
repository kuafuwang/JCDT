
#include <JCDT_Lib/internal/impl/FileBase.h>
#include <JCDT_Lib/internal/util/case.h>

namespace Jikes { // Open namespace Jikes block

	const char* FileBase::java_suffix = StringConstant::U8S_DO_java;
	unsigned FileBase::java_suffix_length = strlen(java_suffix);
	const char* FileBase::class_suffix = StringConstant::U8S_DO_class;
	unsigned FileBase::class_suffix_length = strlen(class_suffix);

#ifdef UNIX_FILE_SYSTEM
	bool FileBase::IsClassSuffix(char* suffix)
	{
		return strncmp(suffix, class_suffix, class_suffix_length) == 0;
	}

	bool FileBase::IsJavaSuffix(char* suffix)
	{
		return strncmp(suffix, java_suffix, java_suffix_length) == 0;
	}
#elif defined(WIN32_FILE_SYSTEM)
	bool FileBase::IsClassSuffix(char* suffix)
	{
		return Case::StringSegmentEqual(suffix, class_suffix, class_suffix_length);
	}

	FileBase::FileBase(): kind(JAVA), uncompressed_size(0), date_time(0), offset(0), lex_stream(nullptr), mtime(0)
	{
			
	}

	bool FileBase::IsJavaSuffix(char* suffix)
	{
		return Case::StringSegmentEqual(suffix, java_suffix, java_suffix_length);
	}
#endif // WIN32_FILE_SYSTEM


} // Close namespace Jikes block



