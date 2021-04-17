#pragma once

#include <jni.h>
#include <string>

namespace Jikes { // Open namespace Jikes block


	namespace JavaModel {

	class JDTCompiler
		{
		public:
			HINSTANCE hInstance;
			jclass   cls;
			jmethodID   m_CompilerFilesMid;
			jmethodID   m_AddUnitMid;
			jmethodID   m_ProcessMid;
			JavaVM* jvm;
			JNIEnv* env;
			//jobject compilerMainObject;
			std::wstring  dllPath;
			std::wstring jdtJarPath;
			JDTCompiler(const std::wstring& _path ,const std::wstring& _jdtJarPath);

			~JDTCompiler();
			bool Valid();
			int LinkToJNI();
			bool CompilerFiles(const std::wstring& args, std::wstring& result);
			void Clear();
			bool AddRamUnit(const char* utf8_buf, const std::wstring& fileName, const std::wstring& destPath);
			bool Process(const std::wstring& args, std::wstring& result);
		};

	} // Close namespace JavaModel block

} // Close namespace Jikes block

