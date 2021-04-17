
#ifndef ClassFile_jikes_PathModel_INCLUDED_INCLUDED
#define ClassFile_jikes_PathModel_INCLUDED_INCLUDED
#include "IOpenable.h"
#include <JCDT_Lib/internal/env/platform.h>
#include <vector>
namespace Jikes { // Open namespace Jikes block
	class ClassFile;

namespace PathModel{
	class JarPackageRoot;
	class JarPackage;
	class ClassFile:public IOpenable
	{
	public:
		void buildStructure(IProgressMonitor* monitor) override;
	
	public:
		Jikes::ClassFile* getData();
		ClassFile(std::wstring& _name, JarPackageRoot* _root, JarPackage* _parent, u4 _uncompressed_size,u4 _offset);
		~ClassFile();
		std::wstring name;
		const  std::pair<wstring, wstring>* getSuperName();
	
		const  std::vector<std::pair<wstring, wstring> >* getInterfaceNames();
		JarPackageRoot* root;
		JarPackage* parent;
	private:
		Jikes::ClassFile* data;
		
		std::pair<wstring, wstring>* superName;
		std::vector<std::pair<wstring, wstring> >* interfaceNames;

		u4 uncompressed_size;
		u4 offset;
	};

}// Close namespace PathModel block
	

} // Close namespace Jikes block


#endif // _INCLUDED

