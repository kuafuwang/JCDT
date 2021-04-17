
#ifndef JavaModelZip_jikes_INCLUDED_INCLUDED
#define JavaModelZip_jikes_INCLUDED_INCLUDED

#include <JCDT_Lib/internal/util/zip/zip.h>


namespace Jikes {// Open namespace Jikes block


	class ClassFile;
	class ClassFileReader {
	public:
		static ClassFile* read(wstring& path_in_zip, wstring& zip_paht_in_os);
	};



	
} // Close namespace Jikes block


#endif // zip_INCLUDED

