#pragma once

namespace ND91Assistant
{
class AndroidPhotoBucket;
class PictureData;
// 添加图片 
class AddPictureCommand :public BaseCommand
{
public:
	AddPictureCommand(const DeviceData* pDevice);
	virtual ~AddPictureCommand(void);

	// Summary  : 设置命令参数
	// Returns	: void
	// Parameter: wstring remotePath 手机端路径
	// Parameter: wstring localPath 图片本地路径
	// Parameter: string groupId    分类ID
	// Parameter: wstring groupName 分类名称
	void SetParam(wstring remotePath, wstring localPath, string groupId, wstring groupName);

	// 执行命令
	virtual void Execute();

	// Summary  : 获取添加的图片
	// Returns	: PictureData* 
	const PictureData* GetRetPicture(){return _pRetPicture;}


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

	wstring          _remotePath;		  // 参数， 手机端路径
	wstring          _localPath;		  // 参数， 图片在PC端路径
	
    string          _groupId;       // 参数: 分类ID
    wstring         _groupName;     // 参数: 分类名称

	PictureData*     _pRetPicture;		  // 返回结果， 新添加的图片信息

};

}
