#pragma once


#include <string>

#include <vector>
#include <boost/filesystem/path.hpp>

namespace Jikes { // Open namespace Jikes block


	namespace PathModel {

		class IConverterContainer
		{

		protected:
			
			boost::filesystem::path m_path;
			
			std::wstring name;
			bool exist;

		public :


			// 
			static const unsigned __int64 K_ORACALE_CONVERTER = 0;

		
			static const unsigned __int64 K_JCOP_CONVERTER = 1;

			static const unsigned __int64 K_FTSAFE_CONVERTER = 2;


			IConverterContainer(const boost::filesystem::path& _path,const std::wstring& _name)
			:m_path(_path), name(_name), exist(true)
			{
				
			}
			IConverterContainer():exist(true) {

			}

			

			virtual unsigned __int64 getKind() = 0;

			virtual ~IConverterContainer()
			{
				
			}

			 boost::filesystem::path  getPath();
			 void setPath(const boost::filesystem::path & _path);

			 std::wstring getName();
			 void setName(const std::wstring& _name);
			 static unsigned __int64 findKind(std::wstring& kindName);
			 static std::wstring getKindName(int kind);

			
		};

		inline unsigned __int64 IConverterContainer::findKind(std::wstring& kindName)
		{
			
			 if (L"JCOP" == kindName)
			{
				return K_JCOP_CONVERTER;
			}
			 else if(L"FTSAFE" == kindName)
			 {
				 return K_FTSAFE_CONVERTER;
			 }
			return K_ORACALE_CONVERTER;		 
				 
		}
				
		inline std::wstring IConverterContainer::getKindName(int kind)
		{
			if (K_JCOP_CONVERTER == kind)
			{
				return   L"JCOP";
			}

			else if (K_ORACALE_CONVERTER == kind)
			{
				return L"ORACALE";
			}
			else
			{
				return   L"FTSAFE";
			}
		}
		inline  boost::filesystem::path  IConverterContainer::getPath()
		{
			return m_path;
		}

		inline void IConverterContainer::setPath(const boost::filesystem::path & _path)
		{
			m_path = _path;
		}

		inline  std::wstring IConverterContainer::getName()
		{
			return name;
		}
		inline void IConverterContainer::setName(const std::wstring& _name)
		{
			name = _name;
		}

}// Close namespace PathModel block


} // Close namespace Jikes block

