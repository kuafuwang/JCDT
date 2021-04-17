#ifndef  FieldDeclarationPattern_JIKES_SERACH_INCLUDE
#define FieldDeclarationPattern_JIKES_SERACH_INCLUDE
#include "SearchPattern.h"

namespace Jikes { // Open namespace Jikes block

namespace Search{
	class FieldDeclarationPattern:public SearchPattern
	{
	public:
		bool isLocal;
		// selector	
		 std::wstring name;

		// declaring type
		 std::wstring declaringQualification;
		 std::wstring declaringSimpleName;

		// type
		 std::wstring typeQualification;
		 std::wstring typeSimpleName;

		
		FieldDeclarationPattern(
			std::wstring _name,
			int _matchMode,
			bool _isCaseSensitive,
			std::wstring _declaringQualification,
			std::wstring _declaringSimpleName,
			std::wstring _typeQualification,
			std::wstring _typeSimpleName);


		/**
		* @see SearchPattern#matchContainer()
		*/
		 int matchContainer() {
			return CLASS;
		}
		/**
		* @see SearchPattern#matchesBinary(Object, Object)
		*/
		bool matchesBinary(std::pair<void*, BinaryInfoType>&  binaryInfo, 
			std::pair<const ConstantPool*, CPClassInfo*>& class_data);
		
		/**
		* Returns whether a method declaration or message send will need to be resolved to
		* find out if this method pattern matches it.
		*/
		 bool needsResolve() {

			// declaring type
			if (declaringSimpleName.empty() != true || declaringQualification.empty() != true)
				return true;

			// return type
			if (typeSimpleName.empty() != true || typeQualification.empty() != true) return true;

			return false;
		}
	
		/**
		* @see SearchPattern#matchLevel(AstNode, bool)
		*/
		int matchLevel(Ast* node, bool resolve);

		/**
		* @see SearchPattern#matchLevel(Binding)
		*/
		int matchLevel(Symbol* binding);
		bool matchesBinaryPackge(const std::wstring& compoundPackageName) override;
		bool matchesBinaryClass(const std::wstring& mainTypeName) override;

		std::wstring indexEntryPrefix() override {
			return  name;
		}
	};
}// Close namespace Search block
	

} // Close namespace Jikes block

#endif