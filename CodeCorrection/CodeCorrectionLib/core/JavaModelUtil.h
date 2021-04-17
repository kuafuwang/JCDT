
#ifndef JavaModelUtil_Jikes_Correction_INCLUDED_INCLUDED
#define JavaModelUtil_Jikes_Correction_INCLUDED_INCLUDED
#include <string>

namespace Jikes { // Open namespace Jikes block
	namespace  JavaModel {
		class IImportDeclaration;
		class LayerPosInJavaModel;
	}

	namespace Correction {

		class JavaModelUtil
		{
			/**
			* Finds a qualified import for a type name.
			*/
		public:
			static JavaModel::IImportDeclaration* findImport(JavaModel::LayerPosInJavaModel* layer_pos, const std::wstring& simpleName);;
		};
	}// Close namespace Jikes block
	

} // Close namespace Correction block


#endif // _INCLUDED

