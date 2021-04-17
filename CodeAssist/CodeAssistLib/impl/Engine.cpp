#include "stdafx.h"
#include "JCDT_Lib/internal/env/platform.h"
#include "JCDT_Lib/internal/env/LookupEnvironment.h"
#include "JCDT_Lib/internal/impl/option.h"
#include "JCDT_Lib/internal/problem/JikesError.h"
#include "JCDT_Lib/internal/impl/JikesOption.h"
#include "JCDT_Lib/internal/impl/ArgumentExpander.h"
#include "JCDT_Lib/internal/impl/OptionError.h"
#include "JCDT_Lib/internal/util/FileUtil.h"
#include "JCDT_Lib/internal/env/INameEnvironment.h"
#include <JCDT_Lib/internal/lookup/FileSymbol.h>
// Note: Engine classes only use the Jikes namespace, they
// are never defined in the Jikes namespace. The use of the Jikes
// namespace is a compile time option and jikesapi.h can not
// include build files like platform.h or config.h. Only
// the Default* classes in this file can live in the Jikes namespace.
#include "Engine.h"
#include "JCDT_Lib/internal/lex/LexStream.h"

namespace Jikes {
	namespace CodeAssist {


		void ExcludeUselessMethod(TypeDeclaration* type, unsigned pos)
		{

		
			if(!type || !type->class_body)
			{
				return;
			}
			auto class_body = type->class_body;
			unsigned i, num;
			for (i = 0, num = class_body->NumNestedClasses(); i < num; i++) {
				ClassTypeDeclaration* memberType = class_body->NestedClass(i);
				ExcludeUselessMethod(memberType, pos);
			}

			for (i = 0, num = class_body->NumNestedEnums(); i < num; i++) {
				EnumTypeDeclaration* memberType = class_body->NestedEnum(i);
				ExcludeUselessMethod(memberType, pos);
			}
			for (i = 0, num = class_body->NumNestedInterfaces(); i < num; i++) {
				InterfaceTypeDeclaration* memberType = class_body->NestedInterface(i);
				ExcludeUselessMethod(memberType, pos);
			}

			for (i = 0, num = class_body->NumNestedAnnotations(); i < num; i++) {
				AnnotationTypeDeclaration* memberType = class_body->NestedAnnotation(i);
				ExcludeUselessMethod(memberType, pos);
			}
			for (i = 0, num = class_body->NumMethods(); i < num; i++) {
				auto method = class_body->Method(i);
				if ( !method || !method->method_body_opt)
					continue;

				auto node = method->method_body_opt;
				
				auto start = node->LeftToken()->start_location;
				if (pos < start) {
					if (method->method_body_opt) {
						method->method_body_opt->Reset();
						method->body_parse = true;
					}
				}
				auto end = node->RightToken()->EndLocation();
				if (pos > end) {
					if (method->method_body_opt){
						method->method_body_opt->Reset();
						method->body_parse = true;
					}

				}
			
			}

			for (i = 0, num = class_body->NumConstructors(); i < num; i++) {
				auto method = class_body->Constructor(i);

				if (!method || !method->constructor_body)
					continue;

				auto node = method->constructor_body;
				auto start = node->LeftToken()->start_location;
				if (pos < start) {
					method->constructor_body->Reset();
					method->body_parse = true;
				}
				auto end = node->RightToken()->EndLocation();
				if (pos > end) {
					method->constructor_body->Reset();
					method->body_parse = true;
				}
				
			}
		}



		Engine::Engine(INameEnvironment* name_env, Option* option_)
			:Compiler(name_env,option_), cunit(nullptr)
		{
	
		}
		FileSymbol* Engine::getUnit(const wstring& fileName) 
		{
			vector<FileSymbol*>&  input_files = lookupEnvironment->input_files;
			auto _size = input_files.size();
			for (size_t i = 0; i < _size; ++i)
			{
				if (!input_files[i])
					continue;

				wstring tempFileName;
				auto file_location = FileLocation(input_files[i]);
				if (file_location.location) {
					tempFileName = file_location.location;
				}
				if (fileName == tempFileName) {
					return input_files[i];
				}

			}
			return nullptr;
		}

		bool Engine::checkSelection(LexStream* lex, unsigned int position)
		{
			//lex->Reset();
			//lex->Dump();
			if (!lex)
				return  false;

			int idx = lex->FindBadTokenIndex(position);
			if (idx != PositionNode::NO_FOUND) {
				return false;
			}
			idx = lex->FindCommentIndex(position);
			if (idx != PositionNode::NO_FOUND) {

				return false;
			}
			return true;
		}

		Engine::~Engine()
		{
			
		}

		void Engine::clear()
		{
			lookupEnvironment->cleanUp();
		}
	}//namespace CodeAssist
} // Close namespace Jikes block


