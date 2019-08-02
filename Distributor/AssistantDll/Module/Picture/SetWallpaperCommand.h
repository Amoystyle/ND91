#pragma once

namespace ND91Assistant
{

// 设为壁纸
class SetWallpaperCommand :public BaseCommand
{
public:
	SetWallpaperCommand(const DeviceData* pDevice);
	virtual ~SetWallpaperCommand(void);

	// Summary  : 本地图片设为壁纸
	// Returns	: bool
	// Parameter: wstring localPath
	bool SetLocalImageAsWallPaper(wstring localPath);

	// Summary  : 手机端图片设为壁纸
	// Returns	: bool
	// Parameter: wstring remotePath
	bool SetRemoteImageAsWallPaper(wstring remotePath);

protected:

	// 执行命令
	virtual void Execute();

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

	wstring GetVisibleName(wstring remote);

	enum  SetType
	{
		SetType_RemotePath,// 按文件
		SetType_ImageData  // 按内容
	};

	SetType     _type;		// 类型
	wstring     _localpath;	// 参数，本地路径
	wstring     _remotepath;// 参数，手机端路径

	bool        _success;	// 返回结果，成功失败

};

}
