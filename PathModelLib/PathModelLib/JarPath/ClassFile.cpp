#include "stdafx.h"
#include "ClassFile.h"
#include "JarPackageRoot.h"
#include <JCDT_Lib/internal/util/zip/zip.h>
#include <JCDT_Lib/internal/classfmt/class.h>
#include <JCDT_Lib/internal/util/CharOperation.h>
#include <JCDT_Lib/internal/codegen/Signature.h>
#include <boost/algorithm/string.hpp>
namespace Jikes { // Open namespace Jikes block


	namespace PathModel {
		void ClassFile::buildStructure(IProgressMonitor* monitor)
		{
			root->beginToOccupy();

			if (fIsStructureKnown) {
				root->endToOccupy();
				return;
			}

			Jikes::ClassFile* class_data = nullptr;
			ZipFile* zipfile = new ZipFile(root->Zipfile(), uncompressed_size, offset);
			auto buffer = zipfile->Buffer();
			if (buffer) {
				class_data = new Jikes::ClassFile(buffer, uncompressed_size);
				if (!class_data->Valid()) {
					delete class_data;
					class_data = nullptr;
				}
			}
			delete zipfile;
			if (class_data && class_data->Valid()) {
				// See if we read the expected type.
				const ConstantPool& pool = class_data->Pool();
				auto superClassInfo = class_data->SuperClass();
				const char* str = nullptr;
				u2 length = 0;
				string signature;
				wstring qualifedName;
				size_t pos = wstring::npos;
				if(superClassInfo){

					str  = superClassInfo->TypeName(pool);
					length = superClassInfo->TypeNameLength(pool);
					
					if (str)
						signature.assign(str, str + length);
					if (*str == U_LEFT_BRACKET) {
						int i = 0;
						qualifedName = Signature::parseType(str, i, 0);
					}
					else
					{
						qualifedName = CharOperation::ConvertUtf8ToUnicode(signature);
					}
					pos = qualifedName.rfind('/');
					superName = new std::pair<wstring, wstring>();
					if (pos != wstring::npos) {
						superName->first = boost::replace_all_copy(qualifedName.substr(0, pos), L"/", L".");
						auto typeName = qualifedName.substr(pos + 1, qualifedName.size());
						boost::replace_all(typeName, L"$", L".");
						superName->second = typeName;
					}
					else
					{
						boost::replace_all(qualifedName, L"$", L".");
						superName->second = qualifedName;
					}
				}
				

				auto   interfaceCount =  class_data->InterfacesCount();
				if(interfaceCount)
				{
					interfaceNames = new std::vector<std::pair<wstring, wstring> >();
					for (auto i = interfaceCount -1; i >= 0; i--)
					{
						auto _info = class_data->Interface(i);
						if (!_info)
							continue;
						 str = _info->TypeName(pool);
						 length = _info->TypeNameLength(pool);
						 signature.clear();
						 if (str)
							signature.assign(str, str + length);
						 qualifedName.clear();
						 if (*str == U_LEFT_BRACKET) {
							 i = 0;
							 qualifedName = Signature::parseType(str, i, 0);
						 }
						 else
						 {
							 qualifedName = CharOperation::ConvertUtf8ToUnicode(signature);
						 }
						 pos = qualifedName.rfind('/');
						 std::pair<wstring, wstring> item;
						 if (pos != wstring::npos) {
							 item.first = boost::replace_all_copy(qualifedName.substr(0, pos), L"/", L".");
							 auto typeName = qualifedName.substr(pos + 1, qualifedName.size());
							 boost::replace_all(typeName, L"$", L".");
							 item.second = typeName;
						 }
						 else
						 {
							 boost::replace_all(qualifedName, L"$", L".");
							 item.second = qualifedName;
						 }
						 interfaceNames->push_back(item);
					}
				}
			}
			
			data = class_data;
			fIsStructureKnown = true;
			root->endToOccupy();
		}

		Jikes::ClassFile* ClassFile::getData()
		{
			LazyLoad();
			return data;
		}

		ClassFile::ClassFile(std::wstring& _name, JarPackageRoot* _root, JarPackage* _parent, u4 _uncompressed_size, u4 _offset)
			: name(_name), root(_root), parent(_parent), data(nullptr),
			superName(nullptr), interfaceNames(nullptr), uncompressed_size(_uncompressed_size),offset(_offset)
		{

		}

		ClassFile::~ClassFile()
		{
			delete data;
			delete superName;
			delete interfaceNames;
		}

		const std::pair<wstring, wstring>* ClassFile::getSuperName()
		{

			LazyLoad();
			return  superName;
		}

		const std::vector<std::pair<wstring, wstring>>* ClassFile::getInterfaceNames()
		{
			LazyLoad();
			return  interfaceNames;
		}
	} // Close namespace JavaModel block

} // Close namespace Jikes block

