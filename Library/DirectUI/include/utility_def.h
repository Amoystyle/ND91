
#ifndef _UTILITY_DEF_H
#define _UTILITY_DEF_H

#include <tchar.h>
#include <string>

#ifdef UNICODE
typedef std::wstring tstring;
typedef std::wstringstream tstringstream;
#else
typedef std::string tstring;
typedef std::stringstream tstringstream;
#endif // UNICODE 

#define _DUI_BEGIN	namespace dui{

#define _DUI_END	}

#define _DUI dui











#endif //_UTILITY_DEF_H
