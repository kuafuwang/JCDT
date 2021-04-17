
#ifndef SourceTypeElementInfo_java_model_INCLUDED_INCLUDED
#define SourceTypeElementInfo_java_model_INCLUDED_INCLUDED
#include "JavaElementInfo.h"
#include "MemberElementInfo.h"
#include <JavaModelLib/internal/core/IJavaElementType.h>
#include <JavaModelLib/internal/core/Initializer.h>


namespace Jikes { // Open namespace Jikes block


namespace JavaModel{
	class IField;
	class IMethod;
	class IType;
	class BaseJavaType;
	class IJavaElement;
	class Initializer;
	class IInitializer;
	class SourceTypeElementInfo : public MemberElementInfo
	{
	public:
		bool fIsStructureKnown;
		bool has_sort_identity;
	
		
		void AddChildren(IJavaElement* item);
		void HelpToDeepCopy(SourceTypeElementInfo& o, BaseJavaType* parent);
		void HelpToDeepCopyButNoChildren(SourceTypeElementInfo& o, BaseJavaType* parent);
		
		void InsertField(IField* item);
		void InsertInitializer(Initializer* item);
		void InsertMethodSymbol(IMethod* item);
		void InsertNestedTypeSymbol(BaseJavaType* item);
		void InsertAnonymousTypeSymbol(BaseJavaType* item);
		std::vector<IJavaElement*>* getChildrenOfType(IJavaElementType _type);
		~SourceTypeElementInfo();
		vector<IField*>* fileds;

		vector<IMethod*>* methods;

		vector<IType*>* nested_types;

		// The anonymous types that were declared in this type.
		vector<BaseJavaType*>* anonymous_types;
		vector<Initializer*> initializers;
		std::vector<IJavaElement*> all_children;
		

		const std::vector<IType*>* getTypes();
		IInitializer* getInitializer(int start_pos);
		IMethod* getMethod(const wstring& name,
		const	vector<wstring>& parameterTypeSignatures);

		IField* getField(const wstring& name) ;

		const std::vector<IField*>* getFields() ;
		bool hasChildren();
		std::vector<IJavaElement*>* getChildren();
		void SortOutLineNameIdentity();


		const vector<IMethod*>* getMethods() ;

		IType* getType(const wstring& name) ;
	};

	inline bool SourceTypeElementInfo::hasChildren() {
		return  all_children.size();
	}

	inline std::vector<IJavaElement*>* SourceTypeElementInfo::getChildren()
	{

		if (all_children.size()) {
			return new std::vector<IJavaElement*>(all_children);
		}
		else {
			return nullptr;
		}

	}
	inline const vector<IMethod*>* SourceTypeElementInfo::getMethods() {

		if (methods && methods->size())
			return  methods;
		return  nullptr;
	}

	inline IInitializer* SourceTypeElementInfo::getInitializer(int start_pos){
		for(size_t i =0, size = initializers.size();  i < size; ++i){
			auto it = initializers[i];
			if(it->start_pos == start_pos){
				return it;
			}
		}
		return nullptr;
	}

	inline const  std::vector<IType*>* SourceTypeElementInfo::getTypes() {

		if (nested_types && nested_types->size()) {
			return nested_types;
		}
		else {
			return nullptr;
		}
	}

	inline  void SourceTypeElementInfo::AddChildren(IJavaElement* item)
	{
		all_children.push_back(item);
	}

	

	inline const vector<IField*> *SourceTypeElementInfo::getFields()
	{

		if (fileds && fileds->size())
		{
			return fileds;
		}
		else
		{
			return nullptr;
		}
	}

}// Close namespace JavaModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

