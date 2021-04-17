// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#if _MSC_VER <= 1500  // vc2008

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
using boost::shared_ptr;
using boost::make_shared;
using namespace std;
#else

#include <memory>
#include <utility>
using namespace std;
using std::shared_ptr;
using std::make_shared;

#endif





#if defined(UNICODE) || defined(_UNICODE)


typedef  std::wostringstream   Tostringstream;

#else

typedef  std::ostringstream     Tostringstream;


#endif


#include <string>
using std::wstring;
#include <list>
#include <set>
#include <vector>
#include <map>
#include <unordered_map>

using std::vector;



typedef unsigned char byte;

#include <tchar.h>
#include <sstream>

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
