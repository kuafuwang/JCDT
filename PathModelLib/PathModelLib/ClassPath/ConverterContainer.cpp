#include "stdafx.h"
#include "ClasspathContainer.h"
#include <Utillib/zlib/ZipFile.h>
#include <boost/algorithm/string.hpp>
#include "ConverterContainer.h"
using  boost::filesystem::path;


namespace Jikes { // Open namespace Jikes block


	namespace PathModel {

	
	    const wchar_t* szConverter_ft222 = L"ft222";
	    const wchar_t* szConverter_ft305 = L"ft305";
		
		const wchar_t* szConverter_jc222 = L"jc222";
		const wchar_t* szConverter_jc305 = L"jc305";
		const wchar_t* szDefaultConveter_2_x_x_Paramer = L"com.sun.javacard.converter.Converter";

		const wchar_t* szDefaultConveter_3_x_x_Paramer = L"com.sun.javacard.converter.Main";
	
		DefaultConverterContainer::DefaultConverterContainer(const boost::filesystem::path& _path,
			const std::wstring& _name,const wstring& _v, const wstring& parameter)
		{

			m_path = _path;
			name = _name;
			m_paramters = parameter;
			version = _v;
			
		
		
			EnumEntries();
			
			
		}

		void  DefaultConverterContainer::EnumEntries() 
		{
			using namespace boost::filesystem;



			if (!exists(m_path))
			{
				exist = false;
				return;
			}
			directory_iterator end;
			vector<wstring> classpaths;
			
			for (directory_iterator pos(m_path); pos != end; ++pos)
			{
				if (is_directory(*pos))
				{
					continue;
				}
				
				path file(*pos);
				if (file.extension().wstring() != L".jar")
				{
					continue;;
				}
				auto filename = file.filename().wstring();
				boost::to_lower(filename);
				
				converter_jars.push_back(file);
				
			}
			exist = true;
			
		}

		JcopConverterContainer::JcopConverterContainer(const boost::filesystem::path& _path, const std::wstring& _name,
			const std::wstring& _v, const wstring& parameter):DefaultConverterContainer(_path,_name,_v,parameter)
		{
		}

	    FTConverterContainer::FTConverterContainer(const boost::filesystem::path& _path, const std::wstring& _name,
		    const std::wstring& _v, const wstring& parameter) : DefaultConverterContainer(_path, _name, _v, parameter)
	    {
			
	    }

	    const wstring & DefaultConverterContainer::Parameter()
		{
		
			return m_paramters;
		}
	}// Close namespace PathModel block


} // Close namespace Jikes block
