#pragma once
#include "Monitor/ISubject.h"
#include "Core/DeviceData.h"
#include <string>

using namespace std;

namespace ND91Assistant
{
enum  ScaleType
{
	Scale_None,//不缩放
	Scale_Half,//缩小一半
	Scale_Twice, // 2倍
	Scale_Fix  // 缩放到指定大小
};

class ScreenImageData;
class ScreenShotSubject :public ISubject
{
public:
	ScreenShotSubject(const DeviceData* pDevice);
	virtual ~ScreenShotSubject(void);

	// Summary  : 发送刷屏消息
	// Returns	: void
	void  SendFreshScreen();

	// Summary  : 获得屏幕图片
	// Returns	: ScreenImageData*
	ScreenImageData* GetScreenImage(){return _pScreenImage;}

	// Summary  : 设置屏幕图片数据
	// Returns	: void
	// Parameter: string data
	void  SetScreenImageBinaryData(string data);

	// Summary  : 旋转到下个状态
	// Returns	: void
	void  NextRotate();

	// Summary  : 设置缩放类型
	// Returns	: void
	// Parameter: int width  指定宽
	// Parameter: int height 指定高
	// Parameter: ScaleType type 缩放类型
	void  SetScaleType(ScaleType type, int width=0, int height=0);

protected:

	// Summary  : 退出全屏时设为不旋转不缩放状态
	// Returns	: void
	void  ResetScreenImageShow();

	const DeviceData*  _pDevice;

private:

	ScreenImageData*   _pScreenImage; // 当前屏幕图片数据
};

}
