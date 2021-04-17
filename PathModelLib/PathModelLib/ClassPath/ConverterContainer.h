#pragma once

#include <string>
#include <vector>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include "IConverterContainer.h"

using namespace std;


namespace Jikes { // Open namespace Jikes block


	namespace PathModel {

		extern    const wchar_t* szConverter_ft222;
		extern    const wchar_t* szConverter_ft305;
		extern    const wchar_t* szConverter_jc222;
		extern    const wchar_t* szConverter_jc305;
		extern const wchar_t* szDefaultConveter_2_x_x_Paramer;
		extern const wchar_t* szDefaultConveter_3_x_x_Paramer;

		class DefaultConverterContainer :public IConverterContainer
		{
		private:
			wstring m_paramters;
			std::wstring version;
	
			std::vector< boost::filesystem::path > converter_jars;
		public:
			
			std::vector< boost::filesystem::path >& GetConverterJars()
			{
				return converter_jars;
			}
			const wstring& Parameter();
			wstring GetVersion()
			{
				return  version;
			}
			virtual ~DefaultConverterContainer() {};

			DefaultConverterContainer(const boost::filesystem::path& _path,
				const std::wstring& _name, const std::wstring& _v, const wstring& parameter);
			void EnumEntries( );
			unsigned long long getKind() override
			{
				return  K_ORACALE_CONVERTER;
			}
		};

		class JcopConverterContainer :public  DefaultConverterContainer
		{
		public:
			JcopConverterContainer(const boost::filesystem::path& _path,
				const std::wstring& _name, const std::wstring& _v, const wstring& parameter);
		
			unsigned long long getKind() override
			{
				return  K_JCOP_CONVERTER;
			}
		};
		class FTConverterContainer :public  DefaultConverterContainer
		{
		public:
			FTConverterContainer(const boost::filesystem::path& _path,
				const std::wstring& _name, const std::wstring& _v, const wstring& parameter);

			unsigned long long getKind() override
			{
				return  K_FTSAFE_CONVERTER;
			}
		};
	}// Close namespace PathModel block


} // Close namespace Jikes block


