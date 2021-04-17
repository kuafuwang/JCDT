
#ifndef FileBase_Jikes_INCLUDED
#define FileBase_Jikes_INCLUDED
#include "JCDT_Lib/internal/env/platform.h"


namespace Jikes { // Open namespace Jikes block
	class Zip;
	class LexStream;
	class FileBase
	{
	public:
		enum FileKind
		{
			JAVA,
			CLASS,
			CLASS_ONLY
		};
		FileKind kind;
		void SetJava()
		{
			kind = JAVA;
		}
		void SetClass() { kind = CLASS; }
		void SetClassOnly() { kind = CLASS_ONLY; }

		bool IsJava() const
		{
			return kind == JAVA;
		}
		bool IsClass() const
		{
			return kind >= CLASS;
		}
		bool IsClassOnly() const
		{
			return kind == CLASS_ONLY;
		}
		virtual ~FileBase(){}
		virtual Zip* Zipfile() = 0;

		virtual  wstring GetUnicodeFileName() = 0;
		virtual  const char* FileName() = 0;
		virtual unsigned FileNameLength() = 0;
		// These fields are used for files in zip packages.
		u4 uncompressed_size;
		u4 date_time;
		long offset;
		LexStream* lex_stream ;

		FileBase();
		virtual bool IsZip() = 0;

		// This field holds the time of last data modification for a non-zip file
		time_t mtime;

		bool virtualFile = false;
		bool MarkVirtualFile() const
		{
			return virtualFile;
		}
		bool IsVirtualFile() const { return virtualFile; }

		static const char* java_suffix;
		static unsigned java_suffix_length;
		static const char* class_suffix;
		static unsigned class_suffix_length;
		static bool IsJavaSuffix(char* ptr);
		static bool IsClassSuffix(char* ptr);

	};
	

} // Close namespace Jikes block


#endif // _INCLUDED

