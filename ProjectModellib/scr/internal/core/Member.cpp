#include "stdafx.h"

#include <JavaModelLib/internal/core/Member.h>
#include <JavaModelLib/internal/core/IPackageFragment.h>
#include <JavaModelLib/internal/core/IType.h>
#include <JavaModelLib/internal/core/ISourceRange.h>

namespace Jikes { // Open namespace Jikes block


	namespace JavaModel {
	
		 Member::Member(Member& o) : JavaElement(o),
			IMember(o)
		{

		

		}

		void Member::figureRange()
		{
			
			if(isBinary()){
				if(fParent)
					reinterpret_cast<Member*>(fParent)->figureRange();
			}
		}

		Member::~Member()
		{
		}

		IType* Member::getDeclaringType(){
			if (fParent){
				auto temp = reinterpret_cast<Member*>(fParent);
				return temp->getDeclaringType();
			}	
			
			return nullptr;
		}

		void  Member::setSourceRange(ISourceRange* _rng) {
			auto _temp = getElementInfo();
			if(_temp)
				_temp->setSourceRange(_rng);
		 }
		void  Member::setNameRange(ISourceRange* _rng) {
			auto _temp = getElementInfo();
			if (_temp)
				_temp->setNameRange(_rng);
		}
		void Member::setAccessFlags(unsigned flag)
		{
			auto info = getElementInfo();
			if(info)
				info->access.SetFlags(flag);
		}

		const std::vector<std::wstring>* const Member::getModfiers()
		{
			auto info = getElementInfo();
			if(info)
				return info->getModfiers();
			else
			{
				return nullptr;
			}
		}

		wstring Member::getModifersString(wchar_t seperator)
		{
			auto info = getElementInfo();
			if (info)
				return info->getModifersString(seperator);
			else
				return{};
		}

		AccessFlags Member::getAccessFlags()
		{
			auto info = getElementInfo();
			if(info)
				return info->access;
			else
			{
				return AccessFlags();
			}
		}

		ISourceRange* Member::getSourceRange()
		 {
			 auto _info = getElementInfo();
			 if (!_info)
				 return nullptr;

			 auto ___rang = _info->getSourceRange();
			 if (___rang && ___rang->m_nStartLine != ISourceRange::UNKNOWN_FLAGE)
				return ___rang;
			 if (isBinary()){
				 figureRange();
			 }
			 return _info->getSourceRange();
		 }
		ISourceRange* Member::getNameRange()
		{
			auto _info = getElementInfo();
			if (!_info)
				return nullptr;
			auto ___rang = _info->getNameRange();
			if (___rang && ___rang->m_nStartLine != ISourceRange::UNKNOWN_FLAGE)
				return ___rang;
			if (isBinary()) {
				figureRange();
			}
			return _info->getNameRange();

		}
}// Close namespace JavaModel block


} // Close namespace Jikes block
