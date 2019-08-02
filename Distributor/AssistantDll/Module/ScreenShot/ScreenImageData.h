#pragma once
#include <GdiPlus.h>
#include "Module/BaseData.h"
using namespace Gdiplus;
namespace ND91Assistant
{
enum  NDRotateFlipType // 旋转类型，顺时针
{
	RotateNoneFlip,
	Rotate90Flip,
	Rotate180Flip,
	Rotate270Flip
};

struct ScreenInfo
{
	int _width;
	int _height;
	int _bpp;
	int _red_offset;
	int _red_length;
	int _blue_offset;
	int _blue_length;
	int _green_offset;
	int _green_length;
	int _alpha_offset;
	int _alpha_length;
	ScreenInfo()
	{
		_width  = 320;
		_height = 480;
		_bpp    = 0;
		_red_offset = 0;
		_red_length = 0;
		_blue_offset = 0;
		_blue_length = 0;
		_green_offset = 0;
		_green_length = 0;
		_alpha_offset = 0;
		_alpha_length = 0;
	}

};

class BinaryBufferReader;
class DeviceData;
class ScreenImageData :public BaseData
{
	friend class GetScreenImageInfoCommand;
public:
	ScreenImageData(void);
	virtual ~ScreenImageData(void);

	// Summary  : 设置屏幕图片二进制数据
	// Returns	: bool
	// Parameter: string data
	bool SetScreenBinaryData(string data, bool fresh = true);

	// Summary  : 设置旋转类型, 已处理旋转
	// Returns	: bool
	// Parameter: RotateFlipType type
	bool SetRotateFlipType(NDRotateFlipType type);

	// Summary  : 设置缩放比例，已处理缩放
	// Returns	: bool
	// Parameter: float scale
	bool SetScale(float  scale);

#ifdef WIN32
	// Summary  : 获得屏幕图片
	// Returns	: Bitmap*
	Bitmap* GetScreenImage(){return _pImage;}
#endif	

	// Summary  : 获得屏幕宽
	// Returns	: int
	int  GetWidth(){return _ScreenInfo._width;}

	// Summary  : 获得屏幕高
	// Returns	: int
	int  GetHeight(){return _ScreenInfo._height;}

	// Summary  : 获得旋转类型, 界面处理旋转
	// Returns	: RotateFlipType
	NDRotateFlipType  GetRotateFlipType(){return _rotateType;}

	// Summary  : 获得缩放比例，缩放也由界面处理
	// Returns	: float
	//float GetScale(){return _scale;}

	// Summary  : 保存图片到指定路径
	// Returns	: void
	// Parameter: wstring path
	void SaveToFile(wstring path, const DeviceData* pDevice);

	// Summary  : 设为不缩放不旋转
	// Returns	: void
	void ResetShowType();

private:

	// Summary  : 刷新图片
	// Returns	: void
	void FreshBitmap();

	// Summary  : 解析图片信息
	// Returns	: bool
	// Parameter: BinaryBufferReader & reader
	// Parameter: bool IsUsb
	bool ParseAndroidData(BinaryBufferReader& reader, bool IsUsb);

private:

	NDRotateFlipType	_rotateType	;  // 旋转类型

	float               _scale		;  // 缩放比例,图片要显示的大小乘以下面的宽高

	string	  _ScreenImageBinaryData;  // 屏幕图片二进制数据

	ScreenInfo          _ScreenInfo ;  // 屏幕图片信息

#ifdef WIN32
	Bitmap*             _pImage		;  // 屏幕图片
#endif	
};

}
