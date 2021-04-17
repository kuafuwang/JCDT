#ifndef AstClassBody_INCLUDED
#define AstClassBody_INCLUDED
#include "JCDT_Lib/internal/ast/ast_Pre_declaration.h"
#include "JCDT_Lib/internal/ast/AstNode.h"
#include <JCDT_Lib/internal/impl/ReferenceContext.h>
#include "AstNodeScope.h"

namespace Jikes { // Open namespace Jikes block


	//
	// Represents the class body of the following: ClassTypeDeclaration,
	// EnumTypeDeclaration, InterfaceTypeDeclaration, AnnotationTypeDeclaration,
	// AstEnumConstant, and AstClassCreationExpression.  Not all uses can legally
	// have all class body members, so some filtering is in order in the semantic
	// pass.
	//
	class AstClassBody : public Ast, public  ReferenceContext
	{
	public:
		friend class Parser;
		AstStoragePool* pool;
		AstArray<Ast*>* class_body_declarations;

		AstArray<FieldDeclaration*>* instance_variables;
		AstArray<FieldDeclaration*>* class_variables;
		AstArray<MethodDeclaration*>* methods;
		AstArray<ConstructorDeclaration*>* constructors;
		AstArray<Initializer*>* static_initializers;
		AstArray<Initializer*>* instance_initializers;
		AstArray<ClassTypeDeclaration*>* inner_classes;
		AstArray<EnumTypeDeclaration*>* inner_enums;
		AstArray<InterfaceTypeDeclaration*>* inner_interfaces;
		AstArray<AnnotationTypeDeclaration*>* inner_annotations;
		AstArray<EmptyTypeDeclaration*>* empty_declarations;


		enum ClassBodyTag
		{
			NONE,
			UNPARSED
		};
		static AstClassBody* ClassBodyCast(Ast* node)
		{
			return DYNAMIC_CAST<AstClassBody*>(node->kind == CLASS_BODY ? node : NULL);
		}
		bool initializerParseBad = false;
		bool bodyParseBad = false;
		SemanticEnvironment* semantic_environment;
		ConstructorDeclaration* default_constructor;
		static AstClassBody* UnparsedClassBodyCast(Ast* node)
		{
			return DYNAMIC_CAST<AstClassBody*>
				(node->kind == CLASS_BODY &&
					node->other_tag == AstClassBody::UNPARSED ? node : NULL);
		}
		//
		// Filled in by the owning ClassTypeDeclaration, EnumTypeDeclaration,
		// InterfaceTypeDeclaration, or AnnotationTypeDeclaration to allow nicer
		// error messages. Note that owner is null for anonymous classes,
		// including enum constants.
		//
		TypeDeclaration* owner;
		Token* identifier_token;

		//
		// The actual delimiters of the class body.
		//
		Token* left_brace_token;
		Token* right_brace_token;

		AstClassBody(AstStoragePool* p);

		~AstClassBody() {}

		 void MarkUnparsed() { other_tag = UNPARSED; }
		 void MarkParsed() { other_tag = NONE; }

		 Ast*& ClassBodyDeclaration(unsigned i);

		 unsigned NumClassBodyDeclarations();
		 void AllocateClassBodyDeclarations(unsigned estimate = 1);
		void AddClassBodyDeclaration(Ast*);

		 FieldDeclaration*& InstanceVariable(unsigned i);

		 unsigned NumInstanceVariables();
		 void AllocateInstanceVariables(unsigned estimate = 1);
		 void AddInstanceVariable(FieldDeclaration*);

		 FieldDeclaration*& ClassVariable(unsigned i);

		 unsigned NumClassVariables();
		 void AllocateClassVariables(unsigned estimate = 1);
		 void AddClassVariable(FieldDeclaration*);

		 MethodDeclaration*& Method(unsigned i);

		 unsigned NumMethods();
		 void AllocateMethods(unsigned estimate = 1);
		 void AddMethod(MethodDeclaration*);

		 ConstructorDeclaration*& Constructor(unsigned i);

		 unsigned NumConstructors();
		 void AllocateConstructors(unsigned estimate = 1);
		 void AddConstructor(ConstructorDeclaration*);

		 Initializer*& StaticInitializer(unsigned i);

		 unsigned NumStaticInitializers();
		 void AllocateStaticInitializers(unsigned estimate = 1);
		 void AddStaticInitializer(Initializer*);

		 Initializer*& InstanceInitializer(unsigned i);

		 unsigned NumInstanceInitializers();
		 void AllocateInstanceInitializers(unsigned estimate = 1);
		 void AddInstanceInitializer(Initializer*);

		 ClassTypeDeclaration*& NestedClass(unsigned i);

		 unsigned NumNestedClasses();
		 void AllocateNestedClasses(unsigned estimate = 1);
		 void AddNestedClass(ClassTypeDeclaration*);

		 EnumTypeDeclaration*& NestedEnum(unsigned i);

		 unsigned NumNestedEnums();
		 void AllocateNestedEnums(unsigned estimate = 1);
		 void AddNestedEnum(EnumTypeDeclaration*);

		 InterfaceTypeDeclaration*& NestedInterface(unsigned i);

		 unsigned NumNestedInterfaces();
		 void AllocateNestedInterfaces(unsigned estimate = 1);
		 void AddNestedInterface(InterfaceTypeDeclaration*);

		 AnnotationTypeDeclaration*& NestedAnnotation(unsigned i);

		 unsigned NumNestedAnnotations();
		 void AllocateNestedAnnotations(unsigned estimate = 1);
		 void AddNestedAnnotation(AnnotationTypeDeclaration*);

		 EmptyTypeDeclaration*& EmptyDeclaration(unsigned i);

		 unsigned NumEmptyDeclarations();
		 void AllocateEmptyDeclarations(unsigned estimate = 1);
		 void AddEmptyDeclaration(EmptyTypeDeclaration*);

#ifdef JIKES_DEBUG
		void Print(Ostream&,LexStream&) override;
		virtual void Unparse(Ostream& o, LexStream* l) { Unparse(o, l, false); }
		void Unparse(Ostream&, LexStream*, bool);
#endif // JIKES_DEBUG

		virtual Ast* Clone(AstStoragePool*);

		 virtual void  traverse(ASTVisitor* visitor,AstNodeScope* scope) override;
		 ;

		virtual Token* LeftToken() { return left_brace_token; }
		virtual Token* RightToken() { return right_brace_token; }


		public:
			virtual void abort(int abortLevel, int problem)override
			{

			};
			virtual CompilationResult* compilationResult()override
			{
				return nullptr;
			};
			virtual bool hasErrors() override
			{
				return false;
			};
			virtual void tagAsHavingErrors()override
			{

			}

		




	};


	
} // Close namespace Jikes block


#endif // AstClassBody_INCLUDED
