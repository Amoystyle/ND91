#pragma once

namespace ND91Assistant
{
class AndroidPhotoBucket;
// 添加图库分类
class AddPhotoBucketCommand :public BaseCommand
{
public:
	AddPhotoBucketCommand(const DeviceData* pDevice);
	virtual ~AddPhotoBucketCommand(void);

	// Summary  : 设置命令参数
	// Returns	: void
	// Parameter: AndroidPhotoBucket * PhotoBucket要添加的分类
	void SetParam(AndroidPhotoBucket* PhotoBucket);

	// 执行命令
	virtual void Execute();

	// Summary  : 成功失败标志
	// Returns	: bool
	bool  Success(){return _success;}

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

	AndroidPhotoBucket*     _pPhotoBucket;  // 参数，要添加的图库分类
    bool                 _success;       // 成功失败标志
};

}
