
#ifndef AccessFlagsUtil_Jikes_INCLUDED
#define AccessFlagsUtil_Jikes_INCLUDED
#include <JCDT_Lib/internal/impl/access.h>
#include <JCDT_Lib/internal/ast/ast.h>
#include <JCDT_Lib/internal/lex/LexStream.h>

namespace Jikes { // Open namespace Jikes block


	class AccessFlagsUtil
	{
	public:

		// Process modifiers, returning the AccessFlags they represent.
		//
		// Diagnostics are emitted in the following cases:
		// . Invalid modifiers based on context.
		// . Duplicated mutually exclusive modifiers (public, protected, private).
		// . Duplicated general modifiers.
		// . Modifier combinations not legal (such as final abstract).
		// . Modifiers not in the recommended order (controlled by +Pmodifier-order).
		// . Explicit specification of implicit modifier (controlled by
		//   +Predundant-modifiers).
		//
		static AccessFlags ProcessModifiers(AstModifiers* modifiers, u2 implicit_flags)
		{
			if (!modifiers)
				return AccessFlags(implicit_flags);
			AccessFlags access_flags;
			for (unsigned i = 0; i < modifiers->NumModifiers(); i++)
			{
				Ast* mod = modifiers->Modifier(i);
				AstModifierKeyword* keyword = AstModifierKeyword::ModifierKeywordCast(mod);
				if (!keyword){
					continue;
				}
				u2 flag = 0x00;
				switch (keyword->modifier_token->Kind())
				{
				case TK_abstract: flag = AccessFlags::ACCESS_ABSTRACT; break;
				case TK_final: flag = AccessFlags::ACCESS_FINAL; break;
				case TK_native: flag = AccessFlags::ACCESS_NATIVE; break;
				case TK_public: flag = AccessFlags::ACCESS_PUBLIC; break;
				case TK_protected: flag = AccessFlags::ACCESS_PROTECTED; break;
				case TK_private: flag = AccessFlags::ACCESS_PRIVATE; break;
				case TK_static: flag = AccessFlags::ACCESS_STATIC; break;
				case TK_strictfp: flag = AccessFlags::ACCESS_STRICTFP; break;
				case TK_synchronized: flag = AccessFlags::ACCESS_SYNCHRONIZED; break;
				case TK_transient: flag = AccessFlags::ACCESS_TRANSIENT; break;
				case TK_volatile: flag = AccessFlags::ACCESS_VOLATILE; break;
				default:
					assert(false && "token was not modifier");
				}
				access_flags.SetFlags(flag);		
			}
			access_flags.SetFlags(implicit_flags);
			return access_flags;
		}

		// Process modifiers of top-level types.
		static AccessFlags ProcessTopLevelTypeModifiers(TypeDeclaration* decl)
		{
			ClassTypeDeclaration* class_decl = ClassTypeDeclaration::ClassDeclarationCast(decl);
			EnumTypeDeclaration* enum_decl = EnumTypeDeclaration::EnumDeclarationCast(decl);
			InterfaceTypeDeclaration* interface_decl =
				InterfaceTypeDeclaration::InterfaceDeclarationCast(decl);
			AnnotationTypeDeclaration* annotation_decl =
				AnnotationTypeDeclaration::AnnotationDeclarationCast(decl);
			u2 implicit_flags = 0;
			 if (enum_decl){
				implicit_flags = AccessFlags::ACCESS_FINAL;
				for (unsigned i = 0; i < enum_decl->NumEnumConstants(); i++){
					if (enum_decl->EnumConstant(i)->class_body_opt){
						implicit_flags = 0;
						break;
					}
				}	
			}
			else{
				implicit_flags = AccessFlags::ACCESS_INTERFACE |
					AccessFlags::ACCESS_ABSTRACT;
				if (!interface_decl){
					implicit_flags |= AccessFlags::ACCESS_ANNOTATION;
				}
			}
			return ProcessModifiers(decl->modifiers_opt,implicit_flags);
		}

		static void GetModifer(vector<wstring>& out, AstModifiers* modifiers, LexStream* lex)
		{
			if (modifiers && lex)
			{
				for (unsigned i = 0; i < modifiers->NumModifiers(); i++)
				{
					Ast* mod = modifiers->Modifier(i);
					AstAnnotation* annotation = AstAnnotation::AnnotationCast(mod);
					AstModifierKeyword* keyword = AstModifierKeyword::ModifierKeywordCast(mod);
					if (annotation)
					{
						continue;
					}
					assert(keyword);
					
					switch (lex->Kind(keyword->modifier_token->index))
					{
					case TK_abstract:
						out.emplace_back(L"abstract"); break;
					case TK_final: 
						out.emplace_back(L"final"); break;
					case TK_native: 
						out.emplace_back(L"native"); break;
					case TK_public:
						out.emplace_back(L"public"); break;
					case TK_protected:
						out.emplace_back(L"protected"); break;
					case TK_private:
						out.emplace_back(L"private"); break;
					case TK_static: 
						out.emplace_back(L"static"); break;
					case TK_strictfp:
						out.emplace_back(L"strictfp"); break;
					case TK_synchronized:
						out.emplace_back(L"synchronized"); break;
					case TK_transient:
						out.emplace_back(L"transient"); break;
					case TK_volatile:
						out.emplace_back(L"volatile"); break;
					default:
						break;
					}
				}
			}
		}
	};

} // Close namespace Jikes block


#endif // _INCLUDED

