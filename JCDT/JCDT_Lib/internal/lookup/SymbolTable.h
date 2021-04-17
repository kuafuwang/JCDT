
#ifndef SymbolTablesymbol_INCLUDED
#define SymbolTablesymbol_INCLUDED



namespace Jikes { // Open namespace Jikes block
	class Symbol;
	class SymbolTable
	{
	public:
		enum
		{
			DEFAULT_HASH_SIZE = 13,
			MAX_HASH_SIZE = 1021
		};





		SymbolTable(unsigned hash_size_ = DEFAULT_HASH_SIZE);
		virtual ~SymbolTable();

		//virtual void CompressSpace();

	protected:



		Symbol** base;
		unsigned hash_size;

		static unsigned primes[];
		int prime_index;

		virtual unsigned Size() = 0;
		virtual void Hash(Symbol* symbol);
		virtual void Rehash() = 0;

	};
}
#endif
