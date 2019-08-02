#pragma once

#include "Module/Ring/RingData.h"

namespace ND91Assistant
{
	// 添加铃声
class AddRingCommand :
	public BaseCommand
{
public:
	AddRingCommand(const DeviceData* pDevice);
	virtual ~AddRingCommand(void);

	void SetParam(int kind, wstring localpath, wstring remotepath, MediaFileType type, wstring tittle=L"");

	// 执行命令
	virtual void Execute();

	const RingData* GetAddedRing(){return _retRing;}

	bool  DoneSuccess(){return _success;}

protected:

#pragma region 这6个方法是Command的主要内容，处理令格式与结果格式

	// 生成Android数据体，在GetAndroidDatagram()方法中调用本方法
	virtual std::string GetAndroidBodyData();

	// 解析返回数据包
	virtual void SetAndroidReturnData(std::string data);

	bool parseAndroidPackageHead(BinaryBufferReader* pReader);

	// 命令传送到IOS设备上的文件名（全路径, unicode）
	virtual std::wstring GetIosCommandFilePath(){return L"";}

	// 从IOS上获取的命令结果文件名（全路径, unicode）
	virtual std::wstring GetIosResultFilePath() {return L"";}

	// 生成ios文件。返回文件名（全路径, unicode），调用者负责删除文件
	virtual std::wstring GetIosCommandFileContent(){return L"";}

	// 解析ios设备的返回文件（全路径, unicode）并删除文件
	virtual void SetIosReturnFile(std::wstring filename) { }

private:
	bool CreateRingtonesList(std::string ringname);
	string GetMimeType(const wstring path);
	int GetDuration();
	int			_kind;
	wstring		_localpath;
	wstring		_remotepath;
	wstring		_title;
	MediaFileType  _type;
	bool		_success;            //操作结果

	wstring		_strTempRingList;    //临时文件夹下的RingList文件

	RingData*   _retRing;


};

}
