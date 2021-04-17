#include "stdafx.h"
#include "JarPackageRoot.h"
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/algorithm/string.hpp>
#include "JavaZip.h"
#include "JarPathManager.h"
#include "JarPackage.h"

namespace Jikes { // Open namespace Jikes block


	namespace PathModel {
		void JarPackageRoot::buildStructure(IProgressMonitor* monitor)
		{
			auto cs_generate = mgr->cs_generate;
			EnterCriticalSection(cs_generate);
			if (fIsStructureKnown) {
				LeaveCriticalSection(cs_generate);
				return;
			}
			computeChildren();
			LeaveCriticalSection(cs_generate);
		}

		void JarPackageRoot::beginToOccupy()
		{
			if (!usage_reference) {
				auto cs_generate = mgr->cs_generate;
				EnterCriticalSection(cs_generate);
				if (!usage_reference) {
					auto  temp = new CRITICAL_SECTION();
					InitializeCriticalSectionAndSpinCount(temp, 2000);
					usage_reference = temp;
				}
				LeaveCriticalSection(cs_generate);
			}

			EnterCriticalSection(usage_reference);
		}

		void JarPackageRoot::endToOccupy()
		{

			if (usage_reference) {
				LeaveCriticalSection(usage_reference);
			}
		}

		JarPackageRoot::JarPackageRoot(const JarPathManager* _mgr, const std::wstring& _path): zipfile(nullptr),
		                                                                           mgr(_mgr),resoucePath(_path), usage_reference(nullptr)
		{

		}

		JarPackageRoot::~JarPackageRoot()
		{
			if (zipfile) {
				delete zipfile;
				zipfile = nullptr;
			}
			for(auto it : children)
			{
				delete it.second;
			}
			if (usage_reference) {
				DeleteCriticalSection(usage_reference);
				delete usage_reference;
			}
		}
		void JarPackageRoot::computeChildren()
		{

			namespace fs = boost::filesystem;
			fs::path jarPath(resoucePath);
			if (!fs::exists(jarPath)) {
				return;
			}
			if (fs::is_directory(jarPath)) {
				return;
			}
			auto ext = jarPath.extension().wstring();
			boost::to_lower(ext);
			if (ext != L".jar" && ext != L".zip") {
				return;
			}
			if (zipfile) {
				delete zipfile;
				zipfile = nullptr;
			}
			zipfile = new JavaZip(this);
			zipfile->ReadZipContent(resoucePath.c_str());
		
		}

		Zip* JarPackageRoot::Zipfile()
		{
			LazyLoad();
			return zipfile;
		}

		JarPackage* JarPackageRoot::FindOrInsertPackageFragment(const std::wstring& pkgName)
		{
			auto find_it = children.find(pkgName);
			if(find_it != children.end())
			{
				return find_it->second;
			}
			else
			{
				JarPackage* jar_package = new JarPackage(pkgName);
				children.insert(std::make_pair(pkgName, jar_package));
				return jar_package;
			}
			
		}
	} // Close namespace JavaModel block

} // Close namespace Jikes block

