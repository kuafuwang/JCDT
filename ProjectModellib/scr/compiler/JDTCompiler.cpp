#include "stdafx.h"
#include "JavaModelLib/compiler/JDTCompiler.h"
#include <Utillib/common/Util.h>

using namespace  std;
namespace Jikes { // Open namespace Jikes block


	namespace JavaModel {
		string  JStringToAsciiString(JNIEnv* env, jstring str)// (jstring str, LPTSTR desc, int desc_len)
		{
			if (str == NULL)
			{
				return "";
			}
			//在VC中wchar_t是用来存储宽字节字符(UNICODE)的数据类型
			int len = env->GetStringLength(str);
			wchar_t* w_buffer = new wchar_t[len + 1];
			char* c_buffer = new char[2 * len + 1];
			ZeroMemory(w_buffer, (len + 1) * sizeof(wchar_t));
			//使用GetStringChars而不是GetStringUTFChars
			const jchar* jcharString = env->GetStringChars(str, 0);
			wcscpy(w_buffer, (wchar_t*)jcharString);
			env->ReleaseStringChars(str, jcharString);
			ZeroMemory(c_buffer, (2 * len + 1) * sizeof(char));

			len = WideCharToMultiByte(CP_ACP, 0, w_buffer, len, c_buffer, 2 * len, NULL, NULL);
			string cstr = c_buffer;
			delete[] w_buffer;
			delete[] c_buffer;

			return cstr;

		}



		wstring  JStringToWString(JNIEnv* env, jstring str)// (jstring str, LPTSTR desc, int desc_len)
		{
			if (str == NULL)
			{
				return L"";
			}
			//在VC中wchar_t是用来存储宽字节字符(UNICODE)的数据类型
			int len = env->GetStringLength(str);
			wstring  wString(len + 1, 0);

			auto w_buffer = &wString[0];

			//	ZeroMemory(w_buffer, (len + 1) * sizeof(wchar_t));//使用GetStringChars而不是GetStringUTFChars
			const jchar* jcharString = env->GetStringChars(str, 0);
			wcscpy(w_buffer, (wchar_t*)jcharString);
			env->ReleaseStringChars(str, jcharString);
			return wString;


		}

		/*	jstring NewJString(JNIEnv* env, const char* str)
			{
				if (!env || !str)
				{
					return 0;
				}
				int slen = strlen(str);
				jchar* buffer = new jchar[slen];
				int len = MultiByteToWideChar(CP_ACP, 0, str, strlen(str), (wchar_t*)buffer, slen);
				if (len > 0 && len < slen)
				{
					buffer[len] = 0;
				}
				jstring js = env->NewString(buffer, len);
				delete[] buffer;
				return js;

			}*/
		jstring NewJString(JNIEnv* env, const std::wstring& str)
		{
			jstring js = env->NewString((const jchar*)str.c_str(), str.size());

			return js;
		}

	
		
		JDTCompiler::JDTCompiler(const std::wstring& _path, const std::wstring& _jdtJarPath): hInstance(nullptr), cls(nullptr), m_CompilerFilesMid(nullptr), jvm(nullptr),
		                            env(nullptr),dllPath(_path),jdtJarPath(_jdtJarPath)
		{
		}

		JDTCompiler::~JDTCompiler()
		{
			Clear();
		}

		bool JDTCompiler::Valid()
		{
			if (!hInstance)return false;
			if (!cls)return false;
			if (!m_CompilerFilesMid)return false;
			if (!jvm)return false;
			if (!env)return false;
			return true;
		}

		int JDTCompiler::LinkToJNI()
		{

			typedef   jint(JNICALL * PFunCreateJavaVM)(JavaVM * *, void**, void*);

			hInstance = ::LoadLibrary(dllPath.c_str());
			if (hInstance == NULL) {
				return 1;
			}

			//取得里面的JNI_CreateJavaVM函数指针
			PFunCreateJavaVM funCreateJavaVM = (PFunCreateJavaVM)::GetProcAddress(hInstance, "JNI_CreateJavaVM");
			if (funCreateJavaVM == NULL) {
				return 2;
			}
			{
				JavaVMInitArgs vm_args;
				JavaVMOption options[3];
				//设置初始化参数
				//disable JIT，这是JNI文档中的解释，具体意义不是很清楚 ，能取哪些值也不清楚。
				//从JNI文档里给的示例代码中搬过来的
				options[0].optionString = (char*)"-Djava.compiler=NONE";
				//设置classpath，如果程序用到了第三方的JAR包，也可以在这里面包含进来
				string classpath = "-Djava.class.path=.;";
				classpath += Util::UnicodeToANSI(jdtJarPath);
				options[1].optionString = (char*)classpath.c_str();

				//设置显示消息的类型，取值有gc、class和jni，如果一次取多个的话值之间用逗号格开，如-verbose:gc,class
				//该参数可以用来观察C++调用JAVA的过程，设置该参数后，程序会在标准输出设备上打印调用的相关信息
				options[2].optionString = (char*)"-verbose:NONE";

				//设置版本号，版本号有JNI_VERSION_1_1，JNI_VERSION_1_2和JNI_VERSION_1_4
				//选择一个根你安装的JRE版本最近的版本号即可，不过你的JRE版本一定要等于或者高于指定的版本号
				vm_args.version = JNI_VERSION_1_6;
				vm_args.nOptions = 3;
				vm_args.options = options;
				//该参数指定是否忽略非标准的参数，如果填JNI_FLASE，当遇到非标准参数时，JNI_CreateJavaVM会返回JNI_ERR
				vm_args.ignoreUnrecognized = JNI_TRUE;
				auto res = (*funCreateJavaVM)(&(jvm), (void**) & (env), &vm_args);
				if (res < 0)
				{
					return 3;
				}
				cls = env->FindClass("com/ftsafe/compiler/Compiler");
				if (!cls)
				{
					return 4;
				}

			
				m_CompilerFilesMid = env->GetStaticMethodID(cls, "CompilerFiles", "(Ljava/lang/String;)Ljava/lang/String;");
				if (!m_CompilerFilesMid)
				{
					return 5;
				}
				m_AddUnitMid = env->GetStaticMethodID(cls, "addUnitInRAM", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
				if (!m_AddUnitMid)
				{
					return 6;
				}
				m_ProcessMid = env->GetStaticMethodID(cls, "Process", "(Ljava/lang/String;)Ljava/lang/String;");
				if (!m_ProcessMid)
				{
					return 7;
				}
				//jclass global_class = (jclass)env->NewGlobalRef(cls);
				 //compilerMainObject = env->AllocObject(cls);//分配新 Java 对象而不调用该对象的任何构造函数。返回该对象的引用。
				 //if (!compilerMainObject)
				 //{
					// return 8;
				 //}
				
			}
			return 0;
		}
		bool JDTCompiler::CompilerFiles(const std::wstring& args, std::wstring& result)
		{
			if(!Valid())
			{
				return  false;
			}
			
			jstring arg = NewJString(env, args);
			jstring msg = (jstring)env->CallStaticObjectMethod(cls, m_CompilerFilesMid, arg);
			result=  JStringToWString(env, msg);
			return true;
		}

		void JDTCompiler::Clear()
		{
			if (jvm)
			{
				jvm->DestroyJavaVM();
				jvm = nullptr;
			}
		}

		bool JDTCompiler::AddRamUnit(const char* utf8_buf, const std::wstring& fileName, const std::wstring& destPath)
		{
			if(!utf8_buf || fileName.empty() || destPath.empty())
			{
				return  false;
			}
			if (!Valid())
			{
				return  false;
			}
			
			jstring  context = env->NewStringUTF(utf8_buf);
			jstring  fileNameJni = NewJString(env, fileName);
			jstring  destPathJni = NewJString(env, destPath);
			env->CallStaticObjectMethod(cls, m_AddUnitMid, context, fileNameJni, destPathJni);
			return true;
	
		}

		bool JDTCompiler::Process(const std::wstring& args, std::wstring& result)
		{
			if (args.empty())
			{
				return  false;
			}
			if (!Valid())
			{
				return  false;
			}
			
			jstring arg = NewJString(env, args);
			jstring msg = (jstring)env->CallStaticObjectMethod(cls, m_ProcessMid, arg);
			result = JStringToWString(env, msg);
			return  true;
		}
	} // Close namespace JavaModel block

} // Close namespace Jikes block

