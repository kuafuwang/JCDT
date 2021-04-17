#ifndef  SuperTypeNamesCollector_JIKES_SERACH_INCLUDE
#define SuperTypeNamesCollector_JIKES_SERACH_INCLUDE
#include <JCDT_Lib/internal/impl/ITypeRequestor.h>
#include <JCDT_Lib/internal/core/IProgressMonitor.hpp>
#include <vector>
#include <JCDT_Lib/internal/lookup/ASTVisitor.h>

namespace Jikes { // Open namespace Jikes block
	class TypeSymbol;

namespace Search{
	class MethodReferencePattern;
	class MatchSet;
	class SuperTypeNamesCollector  
	{
	public:
		class TypeDeclarationVisitor :public ASTVisitor
		{
		public:
			SuperTypeNamesCollector* super_type_names_collector_;
			TypeDeclarationVisitor(SuperTypeNamesCollector* _collector);
			bool visit(ClassTypeDeclaration * node, AstNodeScope* scope)override;
			bool visit(InterfaceTypeDeclaration * node, AstNodeScope* scope)override;
			bool visit(AnnotationTypeDeclaration * node, AstNodeScope* scope)override;
			bool visit(EnumTypeDeclaration * node, AstNodeScope* scope) override;
		};

		SuperTypeNamesCollector(MethodReferencePattern* _p);
		MethodReferencePattern* pattern;
		void collect();
		std::set< std::wstring  > allSuperDeclaringTypeNames;
		bool matches(TypeSymbol* type);
		bool  matches(const vector<wstring>& compoundName) const;
		void collectSuperTypeNames(TypeSymbol* type);
	};
}// Close namespace Search block
	

} // Close namespace Jikes block

#endif