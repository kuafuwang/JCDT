
#ifndef JavaModelZip_java_model_INCLUDED_INCLUDED
#define JavaModelZip_java_model_INCLUDED_INCLUDED

#include <JCDT_Lib/internal/util/zip/zip.h>

namespace Jikes {// Open namespace Jikes block
namespace PathModel {
	class JarPackage;
	class JarPackageRoot;


	class JavaZip:public Zip
	{
	public:
		JavaZip(JarPackageRoot*);
		~JavaZip();

	protected:
   
   
		JarPackageRoot* pkgFrgRoot;

		void ReadDirectory() override;


		JarPackage *ProcessSubdirectoryEntries(char *, int) const;
	
		void ProcessDirectoryEntry();
	
	};

}// Close namespace PathModel block
	
} // Close namespace Jikes block


#endif // zip_INCLUDED

