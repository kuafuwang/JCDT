#include "stdafx.h"
#include <JavaModelLib/internal/info/SourceTypeElementInfo.h>
#include <JavaModelLib/internal/core/IJavaElement.h>
#include <JavaModelLib/internal/core/IField.h>
#include <JavaModelLib/internal/core/IMethod.h>
#include <JavaModelLib/internal/core/BaseJavaType.h>
#include <JavaModelLib/internal/core/Initializer.h>
#include <JavaModelLib/internal/core/IType.h>
namespace Jikes { // Open namespace Jikes block


	namespace JavaModel {

		
		
		
		void SourceTypeElementInfo::HelpToDeepCopy(SourceTypeElementInfo& o, BaseJavaType* parent)
		{
			MemberElementInfo::HelpToDeepCopy(o);
			if ( o.all_children.size())
			{
				if (o.fileds) {
					for (auto it : *(o.fileds))
					{
						if (!it)
							continue;
						auto temp = (IField*)it->DeepCopy(parent);
						InsertField(temp);
					}
				}
				if (o.methods) {

					for (auto it : *(o.methods))
					{
						if (!it)
							continue;
						auto temp = (IMethod*)it->DeepCopy(parent);
						InsertMethodSymbol(temp);
					}
				}
				if (o.nested_types) {

					for (auto it : *(o.nested_types)) {
						if (!it)
							continue;
						auto temp = (BaseJavaType*)it->DeepCopy(parent);
						InsertNestedTypeSymbol(temp);
					}
				}
				if (o.anonymous_types) {
					anonymous_types = new vector<BaseJavaType*>();
					for (auto it : *(o.anonymous_types)) {
						if (!it)
							continue;
						auto temp = (BaseJavaType*)it->DeepCopy(parent);
						InsertAnonymousTypeSymbol(temp);
					}
				}
				for (auto it : o.initializers)
				{
					if (!it)
						continue;
					auto clone = (Initializer*)it->DeepCopy(parent);
					InsertInitializer(clone);
				}
			}

			fIsStructureKnown = o.fIsStructureKnown;

		}

	

		std::vector<IJavaElement*>* SourceTypeElementInfo::getChildrenOfType(
			IJavaElementType _type)
		{


			std::vector<IJavaElement*>* ret = nullptr;
			if (_type == IJavaElementType::TYPE)
			{

				if (nested_types && nested_types->size()) {
					return new vector<IJavaElement*>(nested_types->begin(), nested_types->end());
				}
			}
			else if (_type == IJavaElementType::METHOD)
			{
				if (methods && methods->size()) {
					return new vector<IJavaElement*>(methods->begin(), methods->end());
				}
			}
			else if (_type == IJavaElementType::FIELD)
			{

				if (fileds && fileds->size()) {
					return new vector<IJavaElement*>(fileds->begin(), fileds->end());
				}
			}

			return ret;
		};

		SourceTypeElementInfo::~SourceTypeElementInfo()
		{
			if ( all_children.size())
			{
				for (auto it : all_children) {
					delete it;
				}
				

				if (fileds)
					delete fileds;
				fileds = nullptr;

				if (methods)
					delete methods;
				methods = nullptr;

				if (nested_types)
					delete nested_types;
				nested_types = nullptr;

				// The anonymous types that were declared in this type.
				if (anonymous_types)
					delete anonymous_types;
				anonymous_types = nullptr;

				initializers.clear();

			}
		}
	



		

		

		IMethod* SourceTypeElementInfo::getMethod(const wstring& name,
		const vector<wstring>& parameterTypeSignatures)
		{
			if (methods) {
				auto temp_methods = *methods;
				for (size_t i = 0; i < temp_methods.size(); ++i)
				{
					auto md = temp_methods[i];
					if (md && md->getElementName() == name)
					{
						if (md->getParameterTypes() == parameterTypeSignatures)
						{
							return  md;
						}
					}
				}
			}
			return nullptr;
		}
		IField* SourceTypeElementInfo::getField(const wstring& name) {

			if (fileds) {
				auto _temp_f = *fileds;
				for (size_t i = 0; i < _temp_f.size(); ++i) {
					auto _f = _temp_f[i];
					if (_f && _f->getElementName() == name)
					{
						return  _f;
					}
				}
			}
			return nullptr;
		};


		void SourceTypeElementInfo::HelpToDeepCopyButNoChildren(SourceTypeElementInfo& o, BaseJavaType* parent){
			MemberElementInfo::HelpToDeepCopy(o);
			fIsStructureKnown = o.fIsStructureKnown;
		}

		void SourceTypeElementInfo::InsertField(IField* item)
		{

			if (!fileds)
				fileds = new vector<IField*>();
			fileds->push_back(item);
			all_children.push_back(item);
		}
		  void SourceTypeElementInfo::InsertInitializer(Initializer* item)
		{
			initializers.push_back(item);
			all_children.push_back(item);
		}


		  void SourceTypeElementInfo::InsertMethodSymbol(IMethod* item)
		{

			if (!methods)
				methods = new vector<IMethod*>();
			methods->push_back(item);
			all_children.push_back(item);
		}

		  void SourceTypeElementInfo::InsertNestedTypeSymbol(BaseJavaType* item)
		{

			if (!nested_types)
				nested_types = new vector<IType*>();
			nested_types->push_back(item);
			all_children.push_back(item);
		}
		  void SourceTypeElementInfo::InsertAnonymousTypeSymbol(BaseJavaType* item)
		{

			if (!anonymous_types)
				anonymous_types = new vector<BaseJavaType*>();
			anonymous_types->push_back(item);
			all_children.push_back(item);
		}

		void  SourceTypeElementInfo::SortOutLineNameIdentity()
		{
		
			if (has_sort_identity)
				return;
			has_sort_identity = true;

			if (fileds)
			{
				map<wstring, int> occurrents;
				auto end = occurrents.end();
				auto size = fileds->size();
				auto fileds_data = fileds->data();
				for (auto i = 0; i < size; ++i)
				{
					auto item = fileds_data[i];

					if (!item)
						continue;

					wstring& outlineName = item->fName;
					auto find_it = occurrents.find(outlineName);
					if (end != find_it)
					{
						find_it->second += 1;
						item->fOccurrenceCount = find_it->second;
					}
					else {
						occurrents.insert(make_pair(outlineName, 1));
					}

				}
			}
			if (nested_types)
			{
				map<wstring, int> occurrents;
				auto end = occurrents.end();
				auto size = nested_types->size();
				const auto nested_types_data = nested_types->data();
				for (auto i = 0; i < size; ++i)
				{
					auto item = nested_types_data[i];

					if (!item)
						continue;

					wstring& outlineName = item->fName;
					auto find_it = occurrents.find(outlineName);
					if (end != find_it)
					{
						find_it->second += 1;
						item->fOccurrenceCount = find_it->second;
					}
					else {
						occurrents.insert(make_pair(outlineName, 1));
					}
					item->SortOutLineNameIdentity();
				}
			}
			if (methods)
			{
				map<wstring, int> occurrents;
				auto end = occurrents.end();
				auto size = methods->size();
				const auto  methods_data = methods->data();
				for (auto i = 0; i < size; ++i)
				{
					auto item = methods_data[i];

					if (!item)
						continue;

					wstring& outlineName = item->fName;
					auto find_it = occurrents.find(outlineName);
					if (end != find_it)
					{
						find_it->second += 1;
						item->fOccurrenceCount = find_it->second;
					}
					else {
						occurrents.insert(make_pair(outlineName, 1));
					}

				}
			}
		}



	
		IType* SourceTypeElementInfo::getType(const wstring& name)
		{
			
			if (nested_types)
			{
				auto types_data = nested_types->data();
				size_t len = nested_types->size();
				for (size_t i = 0; i < len; ++i)
				{
					if (types_data[i] && types_data[i]->getElementName() == name)
					{
						return types_data[i];
					}
				}
			}
			return nullptr;

		};



	} // Close namespace JavaModel block

} // Close namespace Jikes block

