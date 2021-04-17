
#ifndef ITypeRequestor_INCLUDED
#define ITypeRequestor_INCLUDED


namespace Jikes {// Open namespace Jikes block

	class FileSymbol;
	class AstPackageDeclaration;
	class AstStoragePool;
	class ITypeRequestor
	{
	public:
		virtual ~ITypeRequestor()
		{
		}
		virtual bool HeadParse(FileSymbol*) =0;

		virtual bool accept(FileSymbol*) = 0;
		virtual AstPackageDeclaration* ParseForPackage(FileSymbol*, AstStoragePool*) = 0;
	};
	

} // Close namespace Jikes block


#endif // _INCLUDED

