#pragma once

#include "Module/Ring/RingData.h"

namespace ND91Assistant
{
//对铃声的各种操作
class RingOperationCommand :
	 public BaseCommand
{
public:
	RingOperationCommand(const DeviceData* pDevice);
	virtual ~RingOperationCommand(void);

	// 设置铃声
	bool SettingRing(RingData* ring, RingData::MediaFileKind kind);

	// 重命名铃声
	bool RenameRing(RingData* ring, wstring tittle);

	// 移除铃声
	bool RemoveRing(RingData* ring);

	// 修改铃声类型
	bool ChangeRingType(RingData* ring, RingData::MediaFileKind kind);

	// 替换系统铃声
	bool ReplaceIosRing(RingData* ring, wstring filepath);

	// 下载铃声
	bool DownLoadIosRing(RingData* ring, wstring destpath);	

	// 获取IOS当前来电铃声的GUID
	wstring GetIosCurrentRing();

	bool  DoneSuccess(){return _success;}

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
	//EStopRing  无须输入其他参数，其他均需设置RingData
	//ERenameRing/EReplaceRing 需输入title参数
	//ESetRing和EChangeRingType需输入MediaFileKind参数
	void setParam(RingData* ring, RingData::MediaFileKind kind = RingData::Null, wstring title = L"");

	bool DeleteRing();
	bool ChangeRingname();
	bool SetIosRing();
	bool SetIosRingWallerFile();
	bool ReplaceRing();
	bool DownLoadRing();
	wstring GetIosRing();

	DirectoryOperationCommand*  GetDirCmd();
	UploadFileCommand*			GetUpLoadCmd();
	DownloadFileCommand*		GetDownCmd();

	enum RingOpareType
	{
		ENone,
		ERemoveRing		= 1,				//移除铃声
		ESetRing		= 2,				//设置铃声
		ERenameRing		= 3,				//重命名铃声
		EReplaceRing	= 4,        		//替换铃声
		EDownLoadRing	= 5,				//下载铃声
		EGetCurrentRing	= 6,				//获取IOS当前来电铃声
		EChangeRingType = 16				//修改铃声类型
	};

	RingData*			_ring;	 		//铃声
    int					_kind;	 		//铃声 类型，设置铃声和修改铃声类型时用
	wstring				_title;			//标题，rename/replace用
	wstring				_ringid;		//ios来电铃声的GUID	
	RingOpareType       _type;			//操作类型

	DirectoryOperationCommand*	_pDirCmd;
	UploadFileCommand*			_pUpload;
	DownloadFileCommand*		_pDownCmd;

	bool                _success;		//成功失败

	wstring   _strTempRingList;       	//PC Suite下的RingList文件
	wstring   _strIosRingList;        	//手机端上存放RingList文件的目录
	wstring   _strTempRingSet;        	//PC Suite下铃声设置List文件
	wstring   _strIosRingSetList;     	//手机端存放铃声设置List文件
	wstring   _strTempRingWallerSet;  	//PC Suite下铃声壁纸设置ini文件
	wstring   _strIosRingWallerSet;   	//手机端铃声壁纸设置ini文件
	wstring   _strTempSystemRingList; 	//PC Suite下系统铃声设置List文件
	wstring   _strIosSystemRingList;  	//手机端系统铃声设置List文件
};

}
