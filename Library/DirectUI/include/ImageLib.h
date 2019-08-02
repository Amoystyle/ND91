#ifndef _IMAGELIB_H
#define _IMAGELIB_H

////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _GDIPLUS_SUPPORT
#include <GdiPlus.h>
using namespace Gdiplus;
#pragma comment(lib,"gdiplus.lib")

#endif
class DUI_API CImageLib
{
// 图片导入导出
public:
	 // 图片导入成HBITMAP
	 static void LoadImage(LPCTSTR lpszPath, OUT HBITMAP* hBitmap);
	 static void LoadImage(LPVOID lpData, long nSize, OUT HBITMAP* hBitmap);
	 static void LoadImage(UINT uResID, LPCTSTR lpszType, OUT HBITMAP* hBitmap);

	 // 图像导入成像素
	 static void LoadImage(LPCTSTR lpszPath, OUT LPBYTE* lppDataOut,  OUT int* nWidth, OUT int* nHeight);
	 static void LoadImage(LPVOID lpData, long nSize, OUT LPBYTE* lppDataOut,  OUT int* nWidth, OUT int* nHeight);
	 static void LoadImage(UINT uResID, LPCTSTR lpszType, OUT LPBYTE* lppDataOut,  OUT int* nWidth, OUT int* nHeight);

	 // 保存图片
	 static bool SaveImage(HBITMAP hBitmap, LPCTSTR lpszPath);
	 static bool SaveImage(LPBYTE lpData, int nWidth, int nHeight, LPCTSTR lpszPath);

	 // 图片导入成Bitmap
	#ifdef _GDIPLUS_SUPPORT
	 static void LoadImage(LPCTSTR lpszPath, OUT Bitmap** pBitmap);
	 static void LoadImage(LPVOID lpData, long nSize, OUT Bitmap** pBitmap);
	 static void LoadImage(UINT uResID, LPCTSTR lpszType, OUT Bitmap** pBitmap);
	 static bool SaveImage(Image* bitmap, LPCTSTR lpszPath);
	 static HBITMAP BitmapToHBITMAP(Bitmap* bitmap);
	#endif

 // 图片处理
public:
	static void DoGray(HBITMAP hBitmap);
};
#endif