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
			//��VC��wchar_t�������洢���ֽ��ַ�(UNICODE)����������
			int len = env->GetStringLength(str);
			wchar_t* w_buffer = new wchar_t[len + 1];
			char* c_buffer = new char[2 * len + 1];
			ZeroMemory(w_buffer, (len + 1) * sizeof(wchar_t));
			//ʹ��GetStringChars������GetStringUTFChars
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
			//��VC��wchar_t�������洢���ֽ��ַ�(UNICODE)����������
			int len = env->GetStringLength(str);
			wstring  wString(len + 1, 0);

			auto w_buffer = &wString[0];

			//	ZeroMemory(w_buffer, (len + 1) * sizeof(wchar_t));//ʹ��GetStringChars������GetStringUTFChars
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

			//ȡ�������JNI_CreateJavaVM����ָ��
			PFunCreateJavaVM funCreateJavaVM = (PFunCreateJavaVM)::GetProcAddress(hInstance, "JNI_CreateJavaVM");
			if (funCreateJavaVM == NULL) {
				return 2;
			}
			{
				JavaVMInitArgs vm_args;
				JavaVMOption options[3];
				//���ó�ʼ������
				//disable JIT������JNI�ĵ��еĽ��ͣ��������岻�Ǻ���� ����ȡ��ЩֵҲ�������
				//��JNI�ĵ������ʾ�������а������
				options[0].optionString = (char*)"-Djava.compiler=NONE";
				//����classpath����������õ��˵�������JAR����Ҳ�������������������
				string classpath = "-Djava.class.path=.;";
				classpath += Util::UnicodeToANSI(jdtJarPath);
				options[1].optionString = (char*)classpath.c_str();

				//������ʾ��Ϣ�����ͣ�ȡֵ��gc��class��jni�����һ��ȡ����Ļ�ֵ֮���ö��Ÿ񿪣���-verbose:gc,class
				//�ò������������۲�C++����JAVA�Ĺ��̣����øò����󣬳�����ڱ�׼����豸�ϴ�ӡ���õ������Ϣ
				options[2].optionString = (char*)"-verbose:NONE";

				//���ð汾�ţ��汾����JNI_VERSION_1_1��JNI_VERSION_1_2��JNI_VERSION_1_4
				//ѡ��һ�����㰲װ��JRE�汾����İ汾�ż��ɣ��������JRE�汾һ��Ҫ���ڻ��߸���ָ���İ汾��
				vm_args.version = JNI_VERSION_1_6;
				vm_args.nOptions = 3;
				vm_args.options = options;
				//�ò���ָ���Ƿ���ԷǱ�׼�Ĳ����������JNI_FLASE���������Ǳ�׼����ʱ��JNI_CreateJavaVM�᷵��JNI_ERR
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
				 //compilerMainObject = env->AllocObject(cls);//������ Java ����������øö�����κι��캯�������ظö�������á�
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

