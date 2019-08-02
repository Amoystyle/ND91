#pragma once

#include "Module/BaseCommand.h"

using namespace  std;

namespace ND91Assistant
{
	// 获取SD卡状态
class GetSDCardStateCommand :
	public BaseCommand
{
public:
	GetSDCardStateCommand(const DeviceData* pDevice);
	virtual ~GetSDCardStateCommand(void);

	// Summary  : SD卡是否存在
	// Returns	: bool
	// Parameter: const DeviceData * pDevice
	static bool IsSDCardExist(const DeviceData* pDevice);

	SDCardState GetSDCardState(){return _SDCardState;}

	// 执行命令
	virtual void Execute();

protected:

#pragma region 这6个方法是Command的主要内容，处理令格式与结果格式

	// 生成Android数据体，在GetAndroidDatagram()方法中调用本方法
	virtual std::string GetAndroidBodyData();

	// 解析返回数据包
	virtual void SetAndroidReturnData(std::string data);

	bool parseAndroidPackageHead( BinaryBufferReader* pReader );

	// 命令传送到IOS设备上的文件名（全路径, unicode）
	virtual std::wstring GetIosCommandFilePath(){return L"";}

	// 从IOS上获取的命令结果文件名（全路径, unicode）
	virtual std::wstring GetIosResultFilePath() {return L"";}

	// 生成ios文件。返回文件名（全路径, unicode），调用者负责删除文件
	virtual std::wstring GetIosCommandFileContent(){return L"";}

	// 解析ios设备的返回文件（全路径, unicode）并删除文件
	virtual void SetIosReturnFile(std::wstring filename) { }

	void  GetSdcardPathState();

private:

	SDCardState   _SDCardState; //SD卡状态， MOUNTED表示存在


};

}
