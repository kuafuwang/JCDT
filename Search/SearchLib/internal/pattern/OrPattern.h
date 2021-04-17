#ifndef  OrPattern_JIKES_SERACH_INCLUDE
#define OrPattern_JIKES_SERACH_INCLUDE
#include "SearchPattern.h"

namespace Jikes { // Open namespace Jikes block

namespace Search{
	class OrPattern:public SearchPattern
	{
	public:
	
		SearchPattern** patterns;
		~OrPattern();
		int num;
		int _container = 0;
		OrPattern(SearchPattern* _leftPattern, SearchPattern* _rightPattern);

		OrPattern(SearchPattern** _patterns, int _num);

		int matchContainer() override;

		std::wstring indexEntryPrefix() override;

		int matchLevel(Ast* node, bool resolve) override;

		int matchLevel(Symbol* binding) override;

		bool matchesBinary(std::pair<void*, BinaryInfoType>& binaryInfo, 
			std::pair<const ConstantPool*, CPClassInfo*>& class_data);
		virtual  bool IsMayInTheFile(BaseLexStream*) override;
		bool matchesBinaryPackge(const std::wstring& compoundPackageName) override;
		bool matchesBinaryClass(const std::wstring& mainTypeName) override;
	};
}// Close namespace Search block
	

} // Close namespace Jikes block

#endif