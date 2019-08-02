#pragma once
#include "Module/BaseCommand.h"

namespace ND91Assistant
{
// 非OMS获取联络人图片
class GetContactPhotoCommand :public BaseCommand
{
public:
	GetContactPhotoCommand(const DeviceData* pDevice, int contactId);
	virtual ~GetContactPhotoCommand(void);

	// 执行命令
	virtual void Execute();

	// Summary  : 获取头像图片保存到本地的路径
	// Returns	: std::wstring
	wstring  GetPhotoPath(){return _contantPhotoLocalPath;}

private:
	// 生成Android数据体，在GetAndroidDatagram()方法中调用本方法
	virtual std::string GetAndroidBodyData();

	// 解析返回数据包
	virtual void SetAndroidReturnData(std::string data);

	// 命令传送到IOS设备上的文件名（全路径）
	virtual std::wstring GetIosCommandFilePath(){ return L""; }

	// 从IOS上获取的命令结果文件名（全路径）
	virtual std::wstring GetIosResultFilePath(){ return L""; }

	// 生成ios文件
	virtual std::wstring GetIosCommandFileContent(){ return L""; }

	// 解析ios设备的返回文件，解析后删除文件
	virtual void SetIosReturnFile(std::wstring filename){}

private:

	int			_contactId;					// 参数，联络人id

	wstring		_contantPhotoLocalPath;		// 保存获取到的头像到本地的路径

};

}
