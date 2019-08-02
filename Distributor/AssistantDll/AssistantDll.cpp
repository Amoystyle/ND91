// This is the main DLL file.

#include "AssistantDll.h"
#ifndef _CSHARPDLL
#ifdef MODULE_360
#include "Core/NDMobileCommu.h"
 
using namespace ND91Assistant;

    #ifdef WIN32
        extern "C" __declspec(dllexport) NDMobileSrvCommu* CreateInstanceSrvCommu()
        {
            return new NDMobileSrvCommu();
        };
         
        extern "C" __declspec(dllexport) NDMobileMgrCommu* CreateInstanceMgrCommu()
        {
            return new NDMobileMgrCommu();
        };
    #else
        extern "C" NDMobileSrvCommu* CreateInstanceSrvCommu()
        {
            return new NDMobileSrvCommu();
        };

        extern "C" NDMobileMgrCommu* CreateInstanceMgrCommu()
        {
            return new NDMobileMgrCommu();
        };
    #endif

#endif
#else
	using namespace ND91Assistant;
#endif
