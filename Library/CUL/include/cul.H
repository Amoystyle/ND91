#pragma once

#ifdef CUL_EXPORTS 
#define CUL_CLASS_DECL     __declspec(dllexport) 
#else 
#define CUL_CLASS_DECL //    __declspec(dllimport) 
#endif 

#define SAFE_DELETE(p)          if ( p ) { delete p; p=NULL; }
#define SAFE_DELETE_ARRAY(p)    if ( p ) { delete[] p; p=NULL; }

#define RETURN_IF(x)            if ( x ) { return; }
#define RETURN_VALUE_IF(x, y)   if ( x ) { return y; }
#define RETURN_FALSE_IF(x)      if ( x ) { return false; }
#define RETURN_TRUE_IF(x)       if ( x ) { return true; }
#define RETURN_NULL_IF(x)       if ( x ) { return NULL; }
