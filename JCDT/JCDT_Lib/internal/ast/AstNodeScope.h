
#ifndef AstNodeScope_Jikes_INCLUDED
#define AstNodeScope_Jikes_INCLUDED


namespace Jikes { // Open namespace Jikes block
	class Symbol;
	class AstNodeScope
	{
	public:
		virtual ~AstNodeScope()
		{
		}
		AstNodeScope():parent_scope(nullptr)
		{
			
		}
		virtual Symbol* GetSymbol()
		{
			return nullptr;
		}
	
		static  const int BLOCK_SCOPE = 1;
		
		static const  int METHOD_SCOPE = 3;
		static const  int CLASS_SCOPE = 4;
		static const  int COMPILATION_UNIT_SCOPE = 5;
		virtual int GetScopeKind() = 0;
		AstNodeScope* parent_scope;
	};
	

} // Close namespace Jikes block


#endif // _INCLUDED

