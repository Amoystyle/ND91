#pragma once
#include "Module/BaseCommand.h"

using namespace std;
namespace ND91Assistant
{

//音乐，铃声播放
class MusicPlayCommand :public BaseCommand
{
public:
	MusicPlayCommand(const DeviceData* pDevice);
	virtual ~MusicPlayCommand(void);

	// 播放
	bool Play(wstring musicPath);

	// 停止播放
	bool Stop();

protected:

	// 执行命令
	virtual void Execute();

	// 解析Android返回的数据包头 -- 20个字节
	bool parseAndroidPackageHead(BinaryBufferReader* pReader);


#pragma region 这6个方法是Command的主要内容，处理令格式与结果格式

	// 生成Android数据体，在GetAndroidDatagram()方法中调用本方法
	virtual std::string GetAndroidBodyData();

	// 解析返回数据包
	virtual void SetAndroidReturnData(std::string data);

	// 命令传送到IOS设备上的文件名（全路径, unicode）
	virtual std::wstring GetIosCommandFilePath(){return L"";}

	// 从IOS上获取的命令结果文件名（全路径, unicode）
	virtual std::wstring GetIosResultFilePath() {return L"";}

	// 生成ios文件。返回文件名（全路径, unicode），调用者负责删除文件
	virtual std::wstring GetIosCommandFileContent(){return L"";}

	// 解析ios设备的返回文件（全路径, unicode）并删除文件
	virtual void SetIosReturnFile(std::wstring filename) { }
#pragma  endregion


private:
	enum  CommandType
	{
		 EPlayMusic = 17,
		 EStop		= 19
	};

	CommandType  _type;

	wstring      _remotepath;

	bool         _success;
};

}
