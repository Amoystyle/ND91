#pragma once
#include "Module/BaseCommand.h"
#include <deque>
using namespace std;
namespace ND91Assistant
{
// 获取情景模式命令
class ProfileData;
class GetProfilesCommand :public BaseCommand
{
public:
	GetProfilesCommand(const DeviceData* pDevice);
	virtual ~GetProfilesCommand(void);

	virtual void Execute();

	deque<ProfileData*> GetProfiles(){return _profilelist;}

protected:

#pragma region 这6个方法是Command的主要内容，处理令格式与结果格式

	// 生成Android数据体，在GetAndroidDatagram()方法中调用本方法
	virtual std::string GetAndroidBodyData();

	// 解析返回数据包
	virtual void SetAndroidReturnData(std::string data);

	// 命令传送到IOS设备上的文件名（全路径, unicode）
	virtual std::wstring GetIosCommandFilePath() {return L""; }

	// 从IOS上获取的命令结果文件名（全路径, unicode）
	virtual std::wstring GetIosResultFilePath(){return L"";}

	// 生成ios文件。返回文件名（全路径, unicode），调用者负责删除文件
	virtual std::wstring GetIosCommandFileContent(){return L"";}

	// 解析ios设备的返回文件（全路径, unicode）并删除文件
	virtual void SetIosReturnFile(std::wstring filename){}
private:

	deque<ProfileData*>   _profilelist;
};

}
