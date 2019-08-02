#include "pch_module.h"

#ifdef MODULE_SCREENSHOT

#include "ScreenImageConvert.h"

#ifdef WIN32
Bitmap* ScreenImageConvert::ConvertScreenImage(string binarydata, ScreenInfo info)
{
	if(binarydata.empty())
		return NULL;

	Bitmap* pImage = NULL;
	switch(info._bpp)
	{
	case 16:
		pImage = new Bitmap(info._width, info._height,PixelFormat16bppRGB565);
		break;
	case 24:
		pImage = new Bitmap(info._width, info._height, PixelFormat24bppRGB);
		break;
	case 32:
		pImage = new Bitmap(info._width, info._height, PixelFormat32bppARGB);
		break;
	}
	AnalyzeImage(pImage, info, binarydata);
	return pImage;
}

void ScreenImageConvert::AnalyzeImage(Bitmap* pImage, ScreenInfo info, string binarydata)
{
	RETURN_IF(!pImage);
    RETURN_IF(binarydata.length() == 0);
	BitmapData pBmpData;
	pImage->LockBits(&Rect(0,0,info._width, info._height), ImageLockModeWrite,
        pImage->GetPixelFormat(), &pBmpData);

    if(pImage->GetPixelFormat() != PixelFormat32bppARGB || NoColorInfo(info) )
	{
		memcpy((char*)pBmpData.Scan0, binarydata.c_str(), binarydata.length());
		pImage->UnlockBits(&pBmpData);	
		return;
	}

    // 计算红蓝绿三色的顺序
    int rOffset = info._red_offset  /(info._red_length  > 0 ? info._red_length  :8);
    int gOffset = info._green_offset/(info._green_length> 0 ? info._green_length:8);
    int bOffset = info._blue_offset /(info._blue_length > 0 ? info._blue_length :8);

	const char* data = binarydata.c_str();
	for (int y = 0; y < info._height; y++)
	{
        unsigned char* pDest = (unsigned char*)pBmpData.Scan0 + pBmpData.Stride * y;
        const char* pSourceLine = data + info._width * (y << 2);
		for (int x = 0; x < info._width; x++)
		{
			const char* pSourcePixel =  pSourceLine + (x << 2);

			*pDest++ = pSourcePixel[bOffset];
			*pDest++ = pSourcePixel[gOffset];
			*pDest++ = pSourcePixel[rOffset];
			*pDest++ = 0xFF;
		}
	}
	pImage->UnlockBits(&pBmpData);	
}
#endif

bool ND91Assistant::ScreenImageConvert::NoColorInfo(const ScreenInfo &info)
{
    return info._red_offset == 0    && info._red_length == 0
        && info._blue_offset == 0   && info._blue_length == 0
        && info._green_offset == 0  && info._green_length == 0
        && info._alpha_offset == 0  && info._alpha_length == 0;
}


#endif