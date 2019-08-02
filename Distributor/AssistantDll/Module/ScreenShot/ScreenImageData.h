#pragma once
#include <GdiPlus.h>
#include "Module/BaseData.h"
using namespace Gdiplus;
namespace ND91Assistant
{
enum  NDRotateFlipType // ��ת���ͣ�˳ʱ��
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

	// Summary  : ������ĻͼƬ����������
	// Returns	: bool
	// Parameter: string data
	bool SetScreenBinaryData(string data, bool fresh = true);

	// Summary  : ������ת����, �Ѵ�����ת
	// Returns	: bool
	// Parameter: RotateFlipType type
	bool SetRotateFlipType(NDRotateFlipType type);

	// Summary  : �������ű������Ѵ�������
	// Returns	: bool
	// Parameter: float scale
	bool SetScale(float  scale);

#ifdef WIN32
	// Summary  : �����ĻͼƬ
	// Returns	: Bitmap*
	Bitmap* GetScreenImage(){return _pImage;}
#endif	

	// Summary  : �����Ļ��
	// Returns	: int
	int  GetWidth(){return _ScreenInfo._width;}

	// Summary  : �����Ļ��
	// Returns	: int
	int  GetHeight(){return _ScreenInfo._height;}

	// Summary  : �����ת����, ���洦����ת
	// Returns	: RotateFlipType
	NDRotateFlipType  GetRotateFlipType(){return _rotateType;}

	// Summary  : ������ű���������Ҳ�ɽ��洦��
	// Returns	: float
	//float GetScale(){return _scale;}

	// Summary  : ����ͼƬ��ָ��·��
	// Returns	: void
	// Parameter: wstring path
	void SaveToFile(wstring path, const DeviceData* pDevice);

	// Summary  : ��Ϊ�����Ų���ת
	// Returns	: void
	void ResetShowType();

private:

	// Summary  : ˢ��ͼƬ
	// Returns	: void
	void FreshBitmap();

	// Summary  : ����ͼƬ��Ϣ
	// Returns	: bool
	// Parameter: BinaryBufferReader & reader
	// Parameter: bool IsUsb
	bool ParseAndroidData(BinaryBufferReader& reader, bool IsUsb);

private:

	NDRotateFlipType	_rotateType	;  // ��ת����

	float               _scale		;  // ���ű���,ͼƬҪ��ʾ�Ĵ�С��������Ŀ��

	string	  _ScreenImageBinaryData;  // ��ĻͼƬ����������

	ScreenInfo          _ScreenInfo ;  // ��ĻͼƬ��Ϣ

#ifdef WIN32
	Bitmap*             _pImage		;  // ��ĻͼƬ
#endif	
};

}
