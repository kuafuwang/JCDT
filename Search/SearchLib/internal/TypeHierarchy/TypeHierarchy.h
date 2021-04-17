#ifndef  TypeHierarchy_JIKES_TypeHierarchy_INCLUDE
#define TypeHierarchy_JIKES_TypeHierarchy_INCLUDE
#include <string>
#include <vector>
#include <boost/unordered/unordered_map.hpp>
using  std::wstring;
namespace Jikes { // Open namespace Jikes block
	namespace JavaModel
	{
		class ISourceRange;
		class JavaModel;
		class NameLookup;
		class IType;
		class IJavaElement;
		class IMethod;
	}
	namespace PathModel {
		
		class UnitManager;
		class CompilationUnitTypInfo;
	}
	namespace  CodeAssist
	{
		class AssistInfo;

	}
namespace Search {

	class TypeHierarchyLayer
	{
	public:
		TypeHierarchyLayer(JavaModel::IType* _type, const wstring &_path);
		~TypeHierarchyLayer();
		JavaModel::IType* type;
		std::wstring attachSoucePath;
		std::vector<TypeHierarchyLayer*> chilren;
	};

	class InternalTypeHierarchyLayer
	{
	public:
		InternalTypeHierarchyLayer();
		~InternalTypeHierarchyLayer();
		JavaModel::IType* type;
		std::vector<InternalTypeHierarchyLayer*> chilren;
	};
	class DeviceMethodInfo
	{
	public:
		DeviceMethodInfo(JavaModel::IType* _type, JavaModel::IMethod* _method, 
			const std::wstring& _path , JavaModel::ISourceRange* _rng);
		JavaModel::IType* type;
		JavaModel::IMethod* method;
		std::wstring attachSoucePath;
		JavaModel::ISourceRange* rang;
		~DeviceMethodInfo();
	};
	class TypeHierarchy
	{
	private:
		PathModel::CompilationUnitTypInfo* GetUnitTypeInfo(const wstring& unit_path);
		boost::unordered_map<wstring, PathModel::CompilationUnitTypInfo* >* fileInfos;
		InternalTypeHierarchyLayer* FindDeriveClass(const std::wstring& qualifiedPackage, const std::wstring& typeName, bool binary);
		InternalTypeHierarchyLayer* FindSuperClass(const std::wstring& qualifiedPackage, const std::wstring& typeName, bool binary, InternalTypeHierarchyLayer* layer);
		TypeHierarchyLayer* ChangeInternalTypeHierarchyLayerToTypeHierarchyLayer(InternalTypeHierarchyLayer*);

	public:

		TypeHierarchy(JavaModel::JavaModel* pJM, const std::wstring& projectName);
		~TypeHierarchy();
		static  const int acceptFlags;
		JavaModel::NameLookup* nameLookUp = nullptr;
		PathModel::UnitManager* _manager = nullptr;
	
		void getDeviceMethod(JavaModel::IMethod* method,
			std::vector<DeviceMethodInfo* >& deriveMethods);


		TypeHierarchyLayer*  getTypeHierarchy(JavaModel::IType* type);
		TypeHierarchyLayer*  getTypeHierarchyFromMethod(JavaModel::IMethod* method);

		void   getDeviceType(JavaModel::IType* type, std::vector< std::pair< JavaModel::IType*, wstring > >& );

	};
}// Close namespace TypeHierarchy block
	

} // Close namespace Jikes block

#endif