#pragma once

#include "ScreenImageData.h"

namespace ND91Assistant
{
    class ScreenImageConvert
    {
    public:
#ifdef WIN32    	
        static Bitmap* ConvertScreenImage(string binarydata, ScreenInfo info);
#endif        

    private:
#ifdef WIN32    	
        static void AnalyzeImage(Bitmap* pImage, ScreenInfo info, string binarydata);
#endif        
        static bool NoColorInfo(const ScreenInfo &info);
    };

}