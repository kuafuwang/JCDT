#include "stdafx.h"
#include "JavaModelUtil.h"
namespace Jikes { // Open namespace Jikes block


	namespace Correction {
		JavaModel::IImportDeclaration* JavaModelUtil::findImport(JavaModel::LayerPosInJavaModel* layer_pos, const std::wstring& simpleName)
		{
			/*IImportDeclaration[]
			existing = cu.getImports();
			for (int i = 0; i < existing.length; i++)
			{
				String curr = existing[i].getElementName();
				if (curr.endsWith(simpleName))
				{
					int dotPos = curr.length() - simpleName.length() - 1;
					if ((dotPos == -1) || (dotPos > 0 && curr.charAt(dotPos) == '.'))
					{
						return existing[i];
					}
				}
			}
			return null;*/
			return NULL;
		}
	} // Close namespace Correction block

} // Close namespace Jikes block

