#include "stdafx.h"

#include "LocatorCompiler.h"
#include <JavaModelLib/internal/core/OriginalJavaProject.h>
#include <JavaModelLib/internal/lookup/NameLookup.h>
#include <JCDT_Lib/internal/env/LookupEnvironment.h>
using namespace Jikes::JavaModel;

namespace Jikes { // Open namespace Jikes block

namespace Search{

	LocatorCompiler::LocatorCompiler(
		INameEnvironment* _name_env,
		Option* jikes_option,
		boost::shared_ptr<OriginalJavaProject> _original_project)
		:Compiler(_name_env, jikes_option), original_project(_original_project), _lookup(nullptr)
	{

	}
	
	JavaModel::NameLookup* LocatorCompiler::getNameLookup() {
		if (_lookup)
			return _lookup;
		_lookup = original_project->getNameLookup();
		if (!_lookup)
		{
			assert(false);
			return nullptr;
		}
		

		_lookup->beginToOccupy();
		return  _lookup;
	}

	

	LocatorCompiler::~LocatorCompiler()
	{
		if (_lookup) {
			_lookup->endToOccupy();
		}
	}
}


} // Close namespace Jikes block


