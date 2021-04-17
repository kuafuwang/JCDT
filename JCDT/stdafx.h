// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
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

// TODO: 在此处引用程序需要的其他头文件
