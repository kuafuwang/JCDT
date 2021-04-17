
#ifndef SymbolCast_jikes_INCLUDED
#define SymbolCast_jikes_INCLUDED
#include "lookup_Pre_declaration.h"

namespace Jikes { // Open namespace Jikes block



class SymbolCast
{
public:
	static PackageSymbol* PackageCast(Symbol*);
	static const PackageSymbol* PackageCast(const Symbol*) ;

	static TypeSymbol* TypeCast( Symbol*);
	static const TypeSymbol* TypeCast(const Symbol*);


	static MethodSymbol* MethodCast( Symbol*);
	static const MethodSymbol* MethodCast(const Symbol*);


	static BlockSymbol* BlockCast( Symbol*);
	static const BlockSymbol* BlockCast(const Symbol*);


	static VariableSymbol* VariableCast( Symbol*);
	static const VariableSymbol* VariableCast(const Symbol*);

	static LabelSymbol* LabelCast( Symbol*);
	static const LabelSymbol* LabelCast(const Symbol*);

	static LiteralSymbol* LiteralCast( Symbol*);
	static const LiteralSymbol* LiteralCast(const Symbol*);


	/*static NameSymbol* NameCast( Symbol*);
	static const NameSymbol* NameCast(const Symbol*);
*/
	static PathSymbol* PathCast(Symbol*);
	static const  PathSymbol* PathCast(const Symbol*)  ;


	static DirectorySymbol* DirectoryCast(Symbol*);

	static const DirectorySymbol* DirectoryCast(const Symbol*) ;

	static FileSymbol* FileCast(Symbol*);
	static const FileSymbol* FileCast(const Symbol*);
};

	

} // Close namespace Jikes block


#endif // _INCLUDED

