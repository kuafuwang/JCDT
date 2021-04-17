#include "stdafx.h"

#include "JavaModelLib/internal/independent_member/IndependentMoses.h"
#include "ProjectModelLib/ProjectModel/ProjectSolution/SolutionModel.h"
#include <JavaModelLib/internal/core/JavaModel.h>
#include <JavaModelLib/internal/core/OriginalJavaProject.h>
#include <JavaModelLib/internal/core/IType.h>
#include <JavaModelLib/internal/core/BaseJavaType.h>
#include <JavaModelLib/internal/core/IField.h>
#include <JavaModelLib/internal/info/SourceTypeElementInfo.h>
#include <JavaModelLib/internal/info/SourceFieldElementInfo.h>

#include <JavaModelLib/internal/core/ICompilationUnit.h>
#include <JavaModelLib/internal/core/IMethod.h>
#include <JavaModelLib/internal/info/SourceMethodElementInfo.h>
#include <JavaModelLib/internal/core/BaseMethod.h>
#include <JavaModelLib/internal/core/CompilationUnit.h>

namespace Jikes { // Open namespace Jikes block


	namespace JavaModel {

		void copyTypeInfo(BaseJavaType* parent,
			boost::shared_ptr<OriginalJavaProject>& proj,
			PathModel::MemberInFileMoses* actual_point,
			SourceTypeElementInfo* info,
			bool isBinary)
		{

			auto pkg = proj->findPackageFragmentByName(actual_point->root, actual_point->pkg);
			if (!pkg){
				return;
			}

			if (isBinary){
				auto _file = pkg->getClassFile(actual_point->file);
				if (!_file){
					return;
				}
				auto _type = _file->getType();
				if (_type)
					return;

				auto type_info = reinterpret_cast<SourceTypeElementInfo* >(  _type->getElementInfo() );
				
				

				info->HelpToDeepCopy(*type_info, parent);
			}
			else
			{
				auto _file = pkg->getCompilationUnit(actual_point->file);
				if (!_file) {
					return;
				}

				IType* _type = nullptr;
				if (actual_point->enclosing_types.empty()){
					return;
				}
				int i = 0;
				_type = _file->getType(actual_point->enclosing_types[i++]);

				for (size_t size = actual_point->enclosing_types.size(); i < size; ++i) {
					if (!_type) {
						return;
					}
					_type = _type->getType(actual_point->enclosing_types[i]);
				}
				if (!_type) {
					return;
				}
			
				auto type_info = reinterpret_cast<SourceTypeElementInfo*>(_type->getElementInfo());
				info->HelpToDeepCopy(*type_info, parent);

			}

		}

		void IndependentMoses::copyTypeElementInfoWithMosesPointHelp(BaseJavaType* relate_type, PathModel::MosesPath* moses_point,
			SourceTypeElementInfo* info, bool isBinary)
		{

			auto fParent = relate_type->getParent();
			if (fParent ) {
				switch(fParent->fLEType)
				{
				case TYPE: {
					auto type_info = reinterpret_cast<SourceTypeElementInfo*>(reinterpret_cast<IType*>(fParent)->getElementInfo());
					auto original_type_ = type_info->getType(relate_type->getElementName());
					auto original_type_info = reinterpret_cast<SourceTypeElementInfo*>(original_type_->getElementInfo());
					info->HelpToDeepCopy(*original_type_info, relate_type);
					return;
				}
			    
				case COMPILATION_UNIT:
				case JAR_COMPILATION_UNIT:
				{
					auto _file = reinterpret_cast<CompilationUnit*>(fParent);
					auto original_type_ = _file->getType(relate_type->getElementName());
					auto original_type_info = reinterpret_cast<SourceTypeElementInfo*>(original_type_->getElementInfo());
					info->HelpToDeepCopy(*original_type_info, relate_type);
					return;
				}
			

				case JAR_CLASS_FILE:	
				case CLASS_FILE:
				{
					auto _file = reinterpret_cast<ClassFile*>(fParent);
					auto original_type_ = _file->getType();
					auto original_type_info = reinterpret_cast<SourceTypeElementInfo*>(original_type_->getElementInfo());
					info->HelpToDeepCopy(*original_type_info, relate_type);
					return;
				}
			
				default:
					break;;
				}
			
			
			}


			auto actual_point = reinterpret_cast<PathModel::MemberInFileMoses*>(moses_point);

			auto pjm = SolutionModel::GetInstance()->pJM;

			auto proj = pjm->getJavaProject(actual_point->proj);
			if (!proj) {
				return;
			}
			proj->beginToOccupy();
			copyTypeInfo(relate_type,proj, actual_point, info, isBinary);
			proj->endToOccupy();
		}


		void copyFieldInfo(IField* parent,
		                   boost::shared_ptr<OriginalJavaProject>& proj,
		                   PathModel::MemberInFileMoses* actual_point,
		                   SourceFieldElementInfo* info,
		                   bool isBinary)
		{

			auto pkg = proj->findPackageFragmentByName(actual_point->root, actual_point->pkg);
			if (!pkg) {
				return;
			}

			if (isBinary) {
				auto _file = pkg->getClassFile(actual_point->file);
				if (!_file) {
					return;
				}
				auto _type = _file->getType();
				if (_type)
					return;

				auto type_info = reinterpret_cast<SourceTypeElementInfo*>(_type->getElementInfo());

				auto field_ = type_info->getField(actual_point->name);
				auto field_info = reinterpret_cast<SourceFieldElementInfo*>(field_->getElementInfo());
				info->HelpToDeepCopy(*field_info, parent);

			}
			else
			{
				auto _file = pkg->getCompilationUnit(actual_point->file);
				if (!_file) {
					return;
				}

				IType* _type = nullptr;
				if (actual_point->enclosing_types.empty()) {
					return;
				}
				int i = 0;
				_type = _file->getType(actual_point->enclosing_types[i++]);

				for (size_t size = actual_point->enclosing_types.size(); i < size; ++i) {
					if (!_type) {
						return;
					}
					_type = _type->getType(actual_point->enclosing_types[i]);
				}
				if (!_type) {
					return;
				}

				auto type_info = reinterpret_cast<SourceTypeElementInfo*>(_type->getElementInfo());

				auto field_ = type_info->getField(actual_point->name);
				auto field_info = reinterpret_cast<SourceFieldElementInfo*>(field_->getElementInfo());
				info->HelpToDeepCopy(*field_info, parent);


			}

		}


		void IndependentMoses::copyFieldElementInfoWithMosesPointHelp(IField* relate_field, MosesPath* moses_point,
			SourceFieldElementInfo* info, bool isBinary)
		{

			auto fParent = relate_field->getParent();
			if (fParent && fParent->fLEType == TYPE) {
				auto type_info = reinterpret_cast<SourceTypeElementInfo*>(reinterpret_cast<IType*>(fParent)->getElementInfo());
				auto field_ = type_info->getField(relate_field->getElementName());
				auto field_info = reinterpret_cast<SourceFieldElementInfo*>(field_->getElementInfo());
				info->HelpToDeepCopy(*field_info, relate_field);
				return;
			}




			auto actual_point = reinterpret_cast<PathModel::MemberInFileMoses*>(moses_point);

			auto pjm = SolutionModel::GetInstance()->pJM;

			auto proj = pjm->getJavaProject(actual_point->proj);
			if (!proj) {
				return;
			}
			proj->beginToOccupy();
			copyFieldInfo(relate_field, proj, actual_point, info, isBinary);
			proj->endToOccupy();
		}


		void copyMethodInfo(BaseMethod* parent,
			boost::shared_ptr<OriginalJavaProject>& proj,
			PathModel::MemberInFileMoses* actual_point,
			SourceMethodElementInfo* info,
			bool isBinary,
			const vector<wstring>& formal_parameters)
		{

			auto pkg = proj->findPackageFragmentByName(actual_point->root, actual_point->pkg);
			if (!pkg) {
				return;
			}

			if (isBinary) {
				auto _file = pkg->getClassFile(actual_point->file);
				if (!_file) {
					return;
				}
				auto _type = _file->getType();
				if (_type)
					return;

				auto type_info = reinterpret_cast<SourceTypeElementInfo*>(_type->getElementInfo());

				auto method_ =type_info->getMethod(actual_point->name, formal_parameters);
				auto method_info = reinterpret_cast<SourceMethodElementInfo*>( method_->getElementInfo() );
				info->HelpToDeepCopy(*method_info, parent);

			}
			else
			{
				auto _file = pkg->getCompilationUnit(actual_point->file);
				if (!_file) {
					return;
				}

				IType* _type = nullptr;
				if (actual_point->enclosing_types.empty()) {
					return;
				}
				int i = 0;
				_type = _file->getType(actual_point->enclosing_types[i++]);

				for (size_t size = actual_point->enclosing_types.size(); i < size; ++i) {
					if (!_type) {
						return;
					}
					_type = _type->getType(actual_point->enclosing_types[i]);
				}
				if (!_type) {
					return;
				}

				auto type_info = reinterpret_cast<SourceTypeElementInfo*>(_type->getElementInfo());
				auto method_ = type_info->getMethod(actual_point->name, formal_parameters);
				auto method_info = reinterpret_cast<SourceMethodElementInfo*>(method_->getElementInfo());
				info->HelpToDeepCopy(*method_info, parent);


			}

		}



		void IndependentMoses::copyMethodElementInfoWithMosesPointHelp
		(BaseMethod* releate_method, MosesPath* moses_point,
			SourceMethodElementInfo* info, bool isBinary,
			const vector<wstring>& formal_parameters)
		{

			auto fParent = releate_method->getParent();
			if (fParent && fParent->fLEType == TYPE) {
				auto type_info = reinterpret_cast<SourceTypeElementInfo*>(reinterpret_cast<IType*>(fParent)->getElementInfo());
				auto method_ = type_info->getMethod(releate_method->getElementName(), formal_parameters);
				auto method_info = reinterpret_cast<SourceMethodElementInfo*>(method_->getElementInfo());
				info->HelpToDeepCopy(*method_info, releate_method);
				return;
			}


			auto actual_point = reinterpret_cast<PathModel::MemberInFileMoses*>(
				moses_point);

			auto pjm = SolutionModel::GetInstance()->pJM;

			auto proj = pjm->getJavaProject(actual_point->proj);
			if (!proj) {
				return;
			}
			proj->beginToOccupy();
			copyMethodInfo(releate_method, proj, actual_point, info, 
				isBinary, formal_parameters);
			proj->endToOccupy();
		}
	} // Close namespace JavaModel block

} // Close namespace Jikes block

