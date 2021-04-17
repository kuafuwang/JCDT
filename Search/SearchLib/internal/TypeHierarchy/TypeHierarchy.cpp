#include "stdafx.h"
#include "TypeHierarchy.h"
#include <JavaModelLib/internal/lookup/NameLookup.h>
#include <JavaModelLib/internal/core/IPackageFragment.h>
#include <JavaModelLib/internal/core/ClassFile.h>
#include <PathModelLib/SourcePath/CompilationUnitTypInfo.h>
#include <PathModelLib/JarPath/JarPackage.h>
#include <PathModelLib/SourcePath/UnitManager.h>
#include <JavaModelLib/internal/core/IType.h>
#include <JavaModelLib/internal/core/JavaModel.h>
#include <JavaModelLib/internal/core/PackageFragment.h>
#include <JavaModelLib/internal/core/ICompilationUnit.h>
#include <JavaModelLib/internal/core/IMethod.h>
#include <CodeAssistLib/select/SelectionEngine.h>
#include <boost/algorithm/string.hpp>
#include <JCDT_Lib/internal/impl/option.h>
#include <JCDT_Lib/internal/env/INameEnvironment.h>
#include <JavaModelLib/internal/core/OriginalJavaProject.h>
#include <ProjectModelLib/Builder/NameEnvironment.h>
#include <CodeAssistLib/select/SelecctResult.h>
#include <PathModelLib/MosesPath/MosesPath.h>
#include <JavaModelLib/internal/core/OriginalJavaProject.h>
using namespace Jikes::JavaModel;
using namespace Jikes::PathModel;
namespace Jikes { // Open namespace Jikes block

namespace Search {
	const int TypeHierarchy::acceptFlags = JavaModel::NameLookup::ACCEPT_CLASSES | JavaModel::NameLookup::ACCEPT_INTERFACES;

	InternalTypeHierarchyLayer::InternalTypeHierarchyLayer(): type(nullptr)
	{
	}

	InternalTypeHierarchyLayer::~InternalTypeHierarchyLayer()
	{
		for(auto it :chilren)
		{
			delete it;
		}
		chilren.clear();
	}

	DeviceMethodInfo::DeviceMethodInfo(JavaModel::IType* _type, JavaModel::IMethod* _method,
		const std::wstring& _path,  JavaModel::ISourceRange* _rng)
		:type(_type),method(_method),attachSoucePath(_path), rang(_rng)
	{

	}
	DeviceMethodInfo::~DeviceMethodInfo()
	{
		delete type;
		type = nullptr;

		delete method;
		method = nullptr;

		delete rang;
		rang = nullptr;

	}

	PathModel::CompilationUnitTypInfo* TypeHierarchy::GetUnitTypeInfo(const wstring& unit_path)
	{
		if(!fileInfos){
			fileInfos =new  boost::unordered_map<wstring, PathModel::CompilationUnitTypInfo* >();
			_manager->beginToOccupy();
			for(auto it : _manager->units)
			{
				for(auto projIt: it.second)
				{
					fileInfos->insert(projIt);
				}
			}	
		}

		auto find_it = fileInfos->find(unit_path);
		if (find_it == fileInfos->end()) {
			return nullptr;
		}
		return  find_it->second;
	}

	TypeHierarchyLayer::TypeHierarchyLayer(JavaModel::IType* _type, const wstring &_path)
		: type(_type),attachSoucePath(_path)
	{
	}

	TypeHierarchyLayer::~TypeHierarchyLayer()
	{
		for (auto it : chilren)
		{
			delete it;
		}
		chilren.clear();
		delete type;
		type = nullptr;
	
	}

	TypeHierarchy::TypeHierarchy(JavaModel::JavaModel* pJM,const wstring& projectName): fileInfos(nullptr) {
		
		_manager =new PathModel::UnitManager();
		auto project = pJM->getJavaProject(projectName);
		if(project)
		{
			nameLookUp = project->getNameLookup();
			nameLookUp->beginToOccupy();
		}
		else
		{
			nameLookUp = nullptr;
		}
	}

	TypeHierarchy::~TypeHierarchy()
	{
		if (nameLookUp)
			nameLookUp->endToOccupy();
		if (fileInfos)
		{
			delete fileInfos;
			_manager->endToOccupy();
		}
	}

	
	InternalTypeHierarchyLayer* TypeHierarchy::FindDeriveClass(const wstring& qualifiedPackage,
		const wstring& typeName, bool binary)
	{
		auto type =  nameLookUp->findType(typeName, qualifiedPackage, false, acceptFlags);
		if(!type){
			return nullptr;
		}
		InternalTypeHierarchyLayer* layer = new InternalTypeHierarchyLayer();
		layer->type = type;
		
		for(auto it : nameLookUp->fPackageFragments)
		{
			vector<IPackageFragment*>& pkgs = it.second;
		
			for (auto itPkg : pkgs) {
				if (!itPkg)
					continue;

				if (itPkg->getKind() == IPackageFragmentRoot::K_BINARY) {
					PackageFragment* actualPkg = reinterpret_cast<PackageFragment*>(itPkg);
					auto childrenClass = actualPkg->getInternalClassFiles();
					if(!childrenClass)
						continue;
					for(auto  _classIt : *childrenClass)
					{
						auto _classFile = reinterpret_cast<JavaModel::ClassFile*> (_classIt.second);
						if (!_classFile)continue;

						PathModel::ClassFile* _class_in_jar = _classFile->_class_in_jar;
						if (!_class_in_jar)
							continue;
						bool getOutOfThisClass = false;
						auto superName = _class_in_jar->getSuperName();
						if (superName && superName->first == qualifiedPackage)
						{
							if(superName->second == typeName)
							{
								bool superIsBinary = false;;
								auto child = FindDeriveClass(_class_in_jar->parent->name, _class_in_jar->name, superIsBinary);
								if (child)
									layer->chilren.push_back(child);

								getOutOfThisClass = true;
							}
						}
						
						if (getOutOfThisClass)
							continue;
						auto intefaceInfo = _class_in_jar->getInterfaceNames();
						if (!intefaceInfo)
							continue;
						for (auto _info : *intefaceInfo)
						{
							if (_info.first == qualifiedPackage)
							{
								if(_info.second == typeName)
								{
									bool superIsBinary = false;;
									auto child = FindDeriveClass(_class_in_jar->parent->name, _class_in_jar->name, superIsBinary);
									if (child)
										layer->chilren.push_back(child);
									break;
								}
							}
						
						}
					}				
				}
				else
				{			
					PackageFragment* actualPkg = reinterpret_cast<PackageFragment*>(itPkg);
					auto childrenUnits = actualPkg->getInternalCompilationUnits();
					if (!childrenUnits) continue;
			
					for(auto itUnit : *childrenUnits)
					{
						auto unit = reinterpret_cast<JavaModel::ICompilationUnit*> (itUnit.second);
						if (!unit)continue;
						auto unit_path = unit->getPath();
						// 获取到  找到父类
						CompilationUnitTypInfo* unitInf = GetUnitTypeInfo(unit_path);
						if (!unitInf) {
							continue;
						}

						for (auto typeInfo : unitInf->typesInfo)
						{
							if (!typeInfo)
								continue;

							bool getOutLoopTypeInfo = false;
							auto superName = typeInfo->superName;
							if (superName && superName->first == qualifiedPackage)
							{
								if (superName->second == typeName)
								{
									bool superIsBinary = false;;
									auto child = FindDeriveClass(typeInfo->qualifiedPackage, typeInfo->typeName, superIsBinary);
									if (child)
										layer->chilren.push_back(child);
									getOutLoopTypeInfo = true;
								}
							}
							
							if (getOutLoopTypeInfo)
								break;

							auto intefaceInfo = typeInfo->interfaceNames;
							if (!intefaceInfo)
								continue;

							for (auto _info : *intefaceInfo)
							{
								if (_info.first == qualifiedPackage )
								{
									if(_info.second == typeName)
									{
										bool superIsBinary = false;;
										auto child = FindDeriveClass(typeInfo->qualifiedPackage, typeInfo->typeName, superIsBinary);
										getOutLoopTypeInfo = true;
										if (child)
											layer->chilren.push_back(child);

										break;
									}
								}
							
							}
							if (getOutLoopTypeInfo)
								break;
						}
					}
				
				}
			}
		}
		return layer;
    
	}
	InternalTypeHierarchyLayer* TypeHierarchy::FindSuperClass(
		const wstring& qualifiedPackage, 
		const wstring& typeName,bool binary, InternalTypeHierarchyLayer* layer)
	{
		auto it = nameLookUp->fPackageFragments.find(qualifiedPackage);
		if(it == nameLookUp->fPackageFragments.end())
		{
			return nullptr;
		}

		if (typeName == L"Object" && qualifiedPackage == L"java.lang") {
			return layer;
		}

		vector<IPackageFragment*>& pkgs = it->second;
		
		for (auto itPkg : pkgs){

			if (itPkg && itPkg->getKind() == IPackageFragmentRoot::K_BINARY && binary){
				auto _classFile = reinterpret_cast<JavaModel::ClassFile*> (itPkg->getClassFile(typeName));
				if(!_classFile)continue;
				PathModel::ClassFile* _class_in_jar = _classFile->_class_in_jar;
				if(!_class_in_jar)
					continue;
				auto superName = _class_in_jar->getSuperName();
				if(superName){
					bool superIsBinary = false;
					
					auto type = nameLookUp->findType(superName->second, superName->first, false, acceptFlags);
					if (!type) {
						continue;
					}
					InternalTypeHierarchyLayer* parent = new InternalTypeHierarchyLayer();
					parent->type = type;

					parent->chilren.push_back(layer);

					auto older = FindSuperClass(superName->first, superName->second, superIsBinary, parent);
					
					if (older)
						return older;
					else
						return parent;
				}
			}
			else
			{
				auto unit = reinterpret_cast<JavaModel::ClassFile*> (itPkg->getCompilationUnit(typeName));
				if (!unit)continue;
			
				auto unit_path = unit->getPath();
			

				// 获取到  找到父类
				 CompilationUnitTypInfo* unitInf = GetUnitTypeInfo(unit_path);
				if(!unitInf){
				   continue;
				}
				for(auto typeInfo : unitInf->typesInfo)
				{
					if(typeInfo && typeInfo->qualifiedPackage == qualifiedPackage
						&& typeInfo->typeName == typeName)
					{
						if(!typeInfo->superName)
							continue;

						bool superIsBinary = false;
						InternalTypeHierarchyLayer* parent = new InternalTypeHierarchyLayer();
						auto type = nameLookUp->findType(typeInfo->superName->second, typeInfo->superName->first, false, acceptFlags);
						if (!type) {
							continue;
						}
						parent->type = type;
						parent->chilren.push_back(layer);
						auto older =  FindSuperClass(typeInfo->superName->first, typeInfo->superName->second,
							superIsBinary, parent);
						if (older)
							return older;
						else
							return parent;
					}
				}
			}
		}
		return layer;
	
	}

	
	

	void getAllTypeInLayer(vector<IType*>& types, InternalTypeHierarchyLayer* layer)
	{
		for(auto it : layer->chilren){
			types.emplace_back(it->type);
			getAllTypeInLayer(types,it);
		}
	}

	void TypeHierarchy::getDeviceMethod(JavaModel::IMethod* method, 
		std::vector<DeviceMethodInfo* >& deriveMethods)
	{
		if (!nameLookUp || !method)
			return;
		auto   mose = reinterpret_cast<PathModel::MemberInFileMoses*> (method->getMosesPath());
		InternalTypeHierarchyLayer* layer =FindDeriveClass(mose->pkg, boost::join(mose->enclosing_types, L"."), true);
		vector<IType*> types;
		wstring methodName = method->getElementName();
		auto& paramentTypes = method->getParameterTypes();
		getAllTypeInLayer(types,layer);
		for(auto it :types)
		{

			if (!it)
				continue;

			auto  find_it = it->getMethod(methodName, paramentTypes);
			if(find_it)
			{
				wstring _path;
				auto _contain_type = it->getDeclaringType();
				if (_contain_type) {
					auto _contain_file = reinterpret_cast<JavaModel::BaseJavaFile*>(_contain_type->getParent());
					if (_contain_file) {
						_path = _contain_file->getAttachSourcePath();
					}
				}
				auto rng =   find_it->getNameRange();
				if (rng)
					rng = rng->Clone();

				auto type = it->DeepCopyButNoChildren(it->getParent());
				if (!type)
					continue;

				type->GetIndendentInfoAndSepareteFromParent();
				auto cloneOne = find_it->DeepCopy(it->getParent());
				cloneOne->GetIndendentInfoAndSepareteFromParent();
				deriveMethods.emplace_back(new DeviceMethodInfo(type, reinterpret_cast<IMethod*> (cloneOne), _path,rng ));
			}
		}
		delete layer;
	}

	TypeHierarchyLayer* TypeHierarchy::getTypeHierarchy(JavaModel::IType* type)
	{
		if (!nameLookUp || !type)
			return nullptr;

		auto mose = reinterpret_cast<PathModel::MemberInFileMoses*> (type->getMosesPath());
		if (!mose)
			return nullptr;

		vector<wstring> typeNames(mose->enclosing_types);
		typeNames.push_back(mose->name);
		wstring qualifiedTypeName = boost::join(typeNames, L".");
	
	

		InternalTypeHierarchyLayer* layMySelf = FindDeriveClass(mose->pkg, qualifiedTypeName, true);
		if (!layMySelf)
			return nullptr;
		InternalTypeHierarchyLayer* superLayer = FindSuperClass(mose->pkg, qualifiedTypeName, true, layMySelf);
		auto result =  	ChangeInternalTypeHierarchyLayerToTypeHierarchyLayer(superLayer);
		delete superLayer;

		return result;
	}

	TypeHierarchyLayer* TypeHierarchy::getTypeHierarchyFromMethod(JavaModel::IMethod* method)
	{
		if (!nameLookUp || !method)
			return nullptr;


		auto   mose = reinterpret_cast<PathModel::MemberInFileMoses*> (method->getMosesPath());
		if (!mose)
			return nullptr;

		auto qualifiedTypeName = boost::join(mose->enclosing_types, L".");

		InternalTypeHierarchyLayer* layMySelf = FindDeriveClass(mose->pkg, qualifiedTypeName, true);
		if (!layMySelf)
			return nullptr;
		InternalTypeHierarchyLayer* superLayer = FindSuperClass(mose->pkg, qualifiedTypeName, true, layMySelf);
		auto result = ChangeInternalTypeHierarchyLayerToTypeHierarchyLayer(superLayer);
		delete superLayer;

		return result;


	}

	void TypeHierarchy::getDeviceType(JavaModel::IType* type, std::vector< std::pair< JavaModel::IType* ,wstring > >& deviceTyps)
	{
		if (!nameLookUp || !type)
			return ;
		auto   mose = reinterpret_cast<PathModel::MemberInFileMoses*> (type->getMosesPath());
		if (!mose)
			return ;
		vector<wstring> typeNames(mose->enclosing_types);
		typeNames.push_back(mose->name);
		wstring qualifiedTypeName = boost::join(typeNames, L".");



		InternalTypeHierarchyLayer* layMySelf = FindDeriveClass(mose->pkg, qualifiedTypeName, true);
		if (!layMySelf)
			return ;
		vector<IType*> tempTypes;
		getAllTypeInLayer(tempTypes, layMySelf);
		for(auto it : tempTypes)
		{
			wstring _path;
			it->getNameRange();
			auto _contain_type = it->getDeclaringType();
			if (_contain_type) {
				auto _contain_file = reinterpret_cast<JavaModel::BaseJavaFile*>(_contain_type->getParent());
				if (_contain_file) {
					_path = _contain_file->getAttachSourcePath();
				}
			}
			auto cloneType = reinterpret_cast<JavaModel::IType*>(it->DeepCopyButNoChildren(it->getParent()));
			deviceTyps.push_back(make_pair(cloneType, _path));
		}
		delete layMySelf;
	}

	TypeHierarchyLayer* TypeHierarchy::ChangeInternalTypeHierarchyLayerToTypeHierarchyLayer(InternalTypeHierarchyLayer* internal)
	{
		if (!internal || !internal->type)
			return nullptr;

		internal->type->getNameRange();

		auto type = reinterpret_cast<JavaModel::IType*>( internal->type->DeepCopy(internal->type->getParent()));
		if (!type )
			return nullptr;

		wstring _path;
		auto _contain_type = internal->type->getDeclaringType();
		if (_contain_type){
			auto _contain_file = reinterpret_cast<JavaModel::BaseJavaFile*>(_contain_type->getParent());
			if (_contain_file){
				_path =  _contain_file->getAttachSourcePath();
			}
		}
		type->GetIndendentInfoAndSepareteFromParent();
		auto resut = new TypeHierarchyLayer(type , _path);
		for(auto it  : internal->chilren)
		{
			auto child = ChangeInternalTypeHierarchyLayerToTypeHierarchyLayer(it);
			resut->chilren.emplace_back(child);
		}
		return resut;
	}
}




} // Close namespace Jikes block


