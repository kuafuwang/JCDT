#include <JCDT_Lib/internal/ast/AstMethodDeclaration.h>
#include <JCDT_Lib/internal/ast/AstConstructorDeclaration.h>
#include <JCDT_Lib/internal/ast/AstInitializerDeclaration.h>
#include <JCDT_Lib/internal/ast/AstClassDeclaration.h>
#include <JCDT_Lib/internal/ast/AstEnumDeclaration.h>
#include <JCDT_Lib/internal/ast/AstInterfaceDeclaration.h>
#include <JCDT_Lib/internal/ast/AstAnnotationDeclaration.h>
#include <JCDT_Lib/internal/ast/AstFieldDeclaration.h>
#include <JCDT_Lib/internal/ast/AstEmptyDeclaration.h>
#ifndef AstClassBody_def_INCLUDED
#define AstClassBody_def_INCLUDED
#include "JCDT_Lib/internal/ast/AstClassBody.h"
#include <JCDT_Lib/internal/lex/LexStream.h>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>
#include <JCDT_Lib/internal/ast/AstStoragePool.h>
namespace Jikes { // Open namespace Jikes block

    void AstClassBody::Unparse(Ostream& os, LexStream* lex_stream, bool enum_body)
    {
        if (debug_unparse)
            os << "/*AstClassBody:#" << id << "*/";
        if (enum_body)
            os << ';' << endl;
        else os << '{' << endl;
        for (unsigned i = 0; i < NumClassBodyDeclarations(); i++)
            ClassBodyDeclaration(i) -> Unparse(os, lex_stream);
        os << '}' << endl;
        if (debug_unparse)
            os << "/*:AstClassBody#" << id << "*/";
    }

	Ast* AstClassBody::Clone(AstStoragePool* ast_pool)
	{
		AstClassBody* clone = ast_pool->GenClassBody();
		clone->identifier_token = identifier_token;
		clone->left_brace_token = left_brace_token;
		clone->AllocateClassBodyDeclarations(NumClassBodyDeclarations());
		clone->AllocateInstanceVariables(NumInstanceVariables());
		clone->AllocateClassVariables(NumClassVariables());
		clone->AllocateMethods(NumMethods());
		clone->AllocateConstructors(NumConstructors());
		clone->AllocateStaticInitializers(NumStaticInitializers());
		clone->AllocateInstanceInitializers(NumInstanceInitializers());
		clone->AllocateNestedClasses(NumNestedClasses());
		clone->AllocateNestedEnums(NumNestedEnums());
		clone->AllocateNestedInterfaces(NumNestedInterfaces());
		clone->AllocateNestedAnnotations(NumNestedAnnotations());
		clone->AllocateEmptyDeclarations(NumEmptyDeclarations());
		for (unsigned i = 0; i < NumClassBodyDeclarations(); i++)
			clone->AddClassBodyDeclaration((TypeDeclaration*)
				ClassBodyDeclaration(i) ->
				Clone(ast_pool));
		clone->right_brace_token = right_brace_token;
		return clone;
	}

	

	void AstClassBody::AddClassBodyDeclaration(Ast* member)
	{
		assert(class_body_declarations);
		FieldDeclaration* field_declaration = FieldDeclaration::FieldDeclarationCast(member);
		MethodDeclaration* method_declaration =
			MethodDeclaration::MethodDeclarationCast(member);
		ConstructorDeclaration* constructor_declaration =
			ConstructorDeclaration::ConstructorDeclarationCast(member);
		Initializer* initializer =
			Initializer::InitializerDeclarationCast(member);
		ClassTypeDeclaration* class_declaration = ClassTypeDeclaration::ClassDeclarationCast(member);
		EnumTypeDeclaration* enum_declaration = EnumTypeDeclaration::EnumDeclarationCast(member);
		InterfaceTypeDeclaration* interface_declaration =
			InterfaceTypeDeclaration::InterfaceDeclarationCast(member);
		AnnotationTypeDeclaration* annotation_declaration =
			AnnotationTypeDeclaration::AnnotationDeclarationCast(member);

		class_body_declarations->Next() = member;
		if (field_declaration)
		{
			if (FieldDeclaration::StaticFieldCast(field_declaration))
				AddClassVariable(field_declaration);
			else AddInstanceVariable(field_declaration);
		}
		else if (method_declaration)
			AddMethod(method_declaration);
		else if (constructor_declaration)
			AddConstructor(constructor_declaration);
		else if (initializer)
		{
			if (Initializer::StaticInitializerCast(initializer))
				AddStaticInitializer(initializer);
			else AddInstanceInitializer(initializer);
		}
		else if (class_declaration)
			AddNestedClass(class_declaration);
		else if (enum_declaration)
			AddNestedEnum(enum_declaration);
		else if (interface_declaration)
			AddNestedInterface(interface_declaration);
		else if (annotation_declaration)
			AddNestedAnnotation(annotation_declaration);
		else {
			
			AddEmptyDeclaration(EmptyTypeDeclaration::EmptyTypeDeclarationCast(member));
			
		}
	}

	 FieldDeclaration*& AstClassBody::InstanceVariable(unsigned i)
	{
		return (*instance_variables)[i];
	}

	 unsigned AstClassBody::NumInstanceVariables()
	{
		return instance_variables ? instance_variables->Length() : 0;
	}

	 FieldDeclaration*& AstClassBody::ClassVariable(unsigned i)
	{
		return (*class_variables)[i];
	}

	 unsigned AstClassBody::NumClassVariables()
	{
		return class_variables ? class_variables->Length() : 0;
	}

	 MethodDeclaration*& AstClassBody::Method(unsigned i)
	{ return (*methods)[i]; }

	 unsigned AstClassBody::NumMethods()
	{
		return methods ? methods->Length() : 0;
	}

	 ConstructorDeclaration*& AstClassBody::Constructor(unsigned i)
	{
		return (*constructors)[i];
	}

	 unsigned AstClassBody::NumConstructors()
	{
		return constructors ? constructors->Length() : 0;
	}

	 Initializer*& AstClassBody::StaticInitializer(unsigned i)
	{
		return (*static_initializers)[i];
	}

	 unsigned AstClassBody::NumStaticInitializers()
	{
		return static_initializers ? static_initializers->Length() : 0;
	}

	 Initializer*& AstClassBody::InstanceInitializer(unsigned i)
	{
		return (*instance_initializers)[i];
	}

	 unsigned AstClassBody::NumInstanceInitializers()
	{
		return instance_initializers ? instance_initializers->Length() : 0;
	}

	 ClassTypeDeclaration*& AstClassBody::NestedClass(unsigned i)
	{
		return (*inner_classes)[i];
	}

	 unsigned AstClassBody::NumNestedClasses()
	{
		return inner_classes ? inner_classes->Length() : 0;
	}

	 EnumTypeDeclaration*& AstClassBody::NestedEnum(unsigned i)
	{
		return (*inner_enums)[i];
	}

	 unsigned AstClassBody::NumNestedEnums()
	{
		return inner_enums ? inner_enums->Length() : 0;
	}

	 InterfaceTypeDeclaration*& AstClassBody::NestedInterface(unsigned i)
	{
		return (*inner_interfaces)[i];
	}

	 unsigned AstClassBody::NumNestedInterfaces()
	{
		return inner_interfaces ? inner_interfaces->Length() : 0;
	}

	 AnnotationTypeDeclaration*& AstClassBody::NestedAnnotation(unsigned i)
	{
		return (*inner_annotations)[i];
	}

	 unsigned AstClassBody::NumNestedAnnotations()
	{
		return inner_annotations ? inner_annotations->Length() : 0;
	}

	void AstClassBody::Print(Ostream& os,LexStream& lex_stream)
{
    unsigned i;
    os << '#' << id << " (ClassBody):  "
            << endl << "    {";
    for (i = 0; i < NumClassBodyDeclarations(); i++)
    {
        if (i % 10 == 0)
            os << endl << "       ";
        os << " #" << ClassBodyDeclaration(i) -> id;
    }
    os << endl << "    }" << endl;

    for (i = 0; i < NumClassBodyDeclarations(); i++)
        ClassBodyDeclaration(i) -> Print(os, lex_stream);
}
void  AstClassBody::traverse(ASTVisitor* visitor,AstNodeScope* scope)
{
	if (visitor->visit(this,scope)) {
		unsigned i;
		unsigned num = NumClassBodyDeclarations();
		for (i = 0; i < num; i++)
			(*class_body_declarations)[i]->traverse(visitor, scope);
	}
	visitor->endVisit(this,scope);
}

 void AstClassBody::AllocateClassBodyDeclarations(unsigned estimate)
{
	assert(!class_body_declarations);
	class_body_declarations =
		new (pool) AstArray<Ast*>(pool, estimate);
}

 void AstClassBody::AllocateInstanceVariables(unsigned estimate)
{
	assert(!instance_variables);
	instance_variables =
		new (pool) AstArray<FieldDeclaration*>(pool, estimate);
}

 void AstClassBody::AddInstanceVariable(FieldDeclaration* field_declaration)
{
	assert(instance_variables);
	instance_variables->Next() = field_declaration;
}

 void AstClassBody::AllocateClassVariables(unsigned estimate)
{
	assert(!class_variables);
	class_variables =
		new (pool) AstArray<FieldDeclaration*>(pool, estimate);
}

 void AstClassBody::AddClassVariable(FieldDeclaration* field_declaration)
{
	assert(class_variables);
	class_variables->Next() = field_declaration;
}

 void AstClassBody::AllocateMethods(unsigned estimate)
{
	assert(!methods);
	methods = new (pool) AstArray<MethodDeclaration*>(pool, estimate);
}

 void AstClassBody::AddMethod(MethodDeclaration* method_declaration)
{
	assert(methods);
	methods->Next() = method_declaration;
}

 void AstClassBody::AllocateConstructors(unsigned estimate)
{
	assert(!constructors);
	constructors =
		new (pool) AstArray<ConstructorDeclaration*>(pool, estimate);
}

 void AstClassBody::AddConstructor(ConstructorDeclaration* constructor_declaration)
{
	assert(constructors);
	constructors->Next() = constructor_declaration;
}

 void AstClassBody::AllocateStaticInitializers(unsigned estimate)
{
	assert(!static_initializers);
	static_initializers =
		new (pool) AstArray<Initializer*>(pool, estimate);
}

 void AstClassBody::AddStaticInitializer(Initializer* initializer)
{
	assert(static_initializers);
	static_initializers->Next() = initializer;
}

 void AstClassBody::AllocateInstanceInitializers(unsigned estimate)
{
	assert(!instance_initializers);
	instance_initializers =
		new (pool) AstArray<Initializer*>(pool, estimate);
}

 void AstClassBody::AddInstanceInitializer(Initializer* initializer)
{
	assert(instance_initializers);
	instance_initializers->Next() = initializer;
}

 void AstClassBody::AllocateNestedClasses(unsigned estimate)
{
	assert(!inner_classes);
	inner_classes = new (pool) AstArray<ClassTypeDeclaration*>(pool, estimate);
}

 void AstClassBody::AddNestedClass(ClassTypeDeclaration* class_declaration)
{
	assert(inner_classes);
	inner_classes->Next() = class_declaration;
	class_declaration->MarkMemberLevel();
}

 void AstClassBody::AllocateNestedEnums(unsigned estimate)
{
	assert(!inner_enums);
	inner_enums = new (pool) AstArray<EnumTypeDeclaration*>(pool, estimate);

}

 void AstClassBody::AddNestedEnum(EnumTypeDeclaration* enum_declaration)
{
	assert(inner_enums);
	inner_enums->Next() = enum_declaration;
	enum_declaration->MarkMemberLevel();
}

 void AstClassBody::AllocateNestedInterfaces(unsigned estimate)
{
	assert(!inner_interfaces);
	inner_interfaces =
		new (pool) AstArray<InterfaceTypeDeclaration*>(pool, estimate);
}

 void AstClassBody::AddNestedInterface(InterfaceTypeDeclaration* interface_declaration)
{
	assert(inner_interfaces);
	inner_interfaces->Next() = interface_declaration;
	interface_declaration->MarkMemberLevel();
}

 void AstClassBody::AllocateNestedAnnotations(unsigned estimate)
{
	assert(!inner_annotations);
	inner_annotations =
		new (pool) AstArray<AnnotationTypeDeclaration*>(pool, estimate);
}

 void AstClassBody::AddNestedAnnotation(AnnotationTypeDeclaration* ann)
{
	assert(inner_annotations);
	inner_annotations->Next() = ann;
	ann->MarkMemberLevel();
}

 	EmptyTypeDeclaration*& AstClassBody::EmptyDeclaration(unsigned i)
	{
		return (*empty_declarations)[i];
	}

 	unsigned AstClassBody::NumEmptyDeclarations()
	{
		return empty_declarations ? empty_declarations->Length() : 0;
	}

	 void AstClassBody::AllocateEmptyDeclarations(unsigned estimate)
{
	assert(!empty_declarations);
	empty_declarations =
		new (pool) AstArray<EmptyTypeDeclaration*>(pool, estimate);
}

 void AstClassBody::AddEmptyDeclaration(EmptyTypeDeclaration* empty_declaration)
{
	assert(empty_declarations);
	empty_declarations->Next() = empty_declaration;
}

	AstClassBody::AstClassBody(AstStoragePool* p): Ast(CLASS_BODY)
	                                               , pool(p), class_body_declarations(nullptr), 
	                                               instance_variables(nullptr), 
	                                               class_variables(nullptr),
	                                               methods(nullptr), constructors(nullptr),
	                                               static_initializers(nullptr), instance_initializers(nullptr),
	                                               inner_classes(nullptr), inner_enums(nullptr), 
	                                               inner_interfaces(nullptr), inner_annotations(nullptr), 
	                                               empty_declarations(nullptr), semantic_environment(nullptr),
	                                               default_constructor(nullptr), owner(nullptr), 
	                                               identifier_token(nullptr), left_brace_token(nullptr),
	                                               right_brace_token(nullptr)
	{}

	Ast*& AstClassBody::ClassBodyDeclaration(unsigned i)
{
	return (*class_body_declarations)[i];
}

 unsigned AstClassBody::NumClassBodyDeclarations()
{
	return class_body_declarations
		? class_body_declarations->Length() : 0;
}

} // Close namespace Jikes block


#endif // AstClassBody_def_INCLUDED
