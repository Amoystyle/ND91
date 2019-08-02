#pragma once
#include "Module/BaseCommand.h"

namespace ND91Assistant
{
// 图片和图库分类操作
class PictureData;
class AndroidPhotoBucket;
class PictureOperationCommand : public BaseCommand
{
public:
	PictureOperationCommand(const DeviceData* pDevice);
	virtual ~PictureOperationCommand(void);

	// Summary  : 重命名分类
	// Returns  : 是否成功
	// Parameter: string id        分类ID
	// Parameter: wstring path     分类路径
	// Parameter: wstring name     分类名称
	// Parameter: wstring newName  分类新名称
	bool RenamePhotoBucket(string id, wstring path, wstring name, wstring newName);

	// Summary  : 移除分类
    // Returns  : 是否成功
    // Parameter: string id        分类ID
    // Parameter: wstring path     分类路径
    // Parameter: wstring name     分类名称
	bool RemovePhotoBucket(string id, wstring path, wstring name);

	// Summary  : 移动图片到另一个分类
    // Returns  : 是否成功
    // Parameter: int imageId       图片ID
    // Parameter: string bucketId   新分类ID
    // Parameter: wstring name      新分类名称
	bool MovePicToCategory(int imageId, string bucketId, wstring name);

	// Summary  : 删除图片
    // Returns  : 是否成功
	// Parameter: int id 图片ID
	bool DeletePicture(int id);

	// Summary  :
	// Returns	: bool
	bool NotifyScanPhoto();

	// Summary  : 成功失败标志
	// Returns	: bool
	bool  GetSuccess(){return _success;}


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
	enum OperationType
	{
		Operation_None,
		Rename_Bucket,	// 重命名
		Remove_Bucket,	// 移除分类
		Move_Picture,	// 移动图片到指定分类
		Add_Picture,    // 添加图片到指定分类
		Delete_Picture,  // 删除图片
		NotifyScan  = 10
	};

    OperationType   _type;      // 操作类型

	int             _imageId;	// 参数，被操作的图片
    string          _bucketId;  // 参数: 分类ID
    wstring         _path;      // 参数: 分类路径
    wstring         _name;      // 参数: 分类名称
	wstring         _newName;	// 参数: 分类新名称

    bool            _success;   // 结果, 成功失败标志
};

}
