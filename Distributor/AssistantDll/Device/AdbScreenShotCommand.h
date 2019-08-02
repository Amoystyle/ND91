#pragma once
#include "Device/AdbRemoteCommand.h"

namespace ND91Assistant
{
// usb 截屏命令
class AdbScreenShotCommand :public AdbRemoteCommand
{
public:
	AdbScreenShotCommand(const wstring sn,wstring id);
	virtual ~AdbScreenShotCommand(void);

	// Summary  : 获得屏幕信息
	// Returns	: string 返回要解析的数据
	// Parameter: int & bpp     pixelformat
	string  GetScreenImageInfoData(int& bpp);

	// Summary  : 获得屏幕图片二进制数据
	// Returns	: string
	string  GetScreenImageData();

protected:

	// 子类实现时，若无特殊处理只需调用CommonExecute即可
	virtual void InternalExecute();

	// 解析返回数据
	// 返回值：true 表示成功，false 表示失败
	virtual bool ParseReturnData()
	{
		return true;
	}

	bool  _IsInfo;

	int   _bpp;
};

}