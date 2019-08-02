 #pragma once
#include "Module/BaseCommand.h"
#include <deque>

using namespace std;
namespace ND91Assistant
{

class VideoData;
// 对视频操作的一些命令
class VideoOperationCommand :public BaseCommand
{
public:
	VideoOperationCommand(const DeviceData* pDevice);
	virtual ~VideoOperationCommand(void);

     // Summary  :添加视频
     // Returns	: VideoData*
     // Parameter: wstring localpath本地路径
     // Parameter: wstring remotepath手机端路径
     VideoData* AddVideo(wstring localpath, wstring remotepath);

	 // Summary  :移除视频
	 // Returns	: bool
	 // Parameter: VideoData * pVideo
	 bool  RemoveVideo(VideoData* pVideo);

	 // Summary  :重命名视频
	 // Returns	: bool
	 // Parameter: VideoData * pVideo
	 // Parameter: wstring newName
	 bool  RenameVideo(VideoData* pVideo, wstring newName);

	 // Summary  :获得视频封面图片
	 // Returns	: void
	 // Parameter: const deque<VideoData * > * pVideoList
	 void  GetVideoImages(const deque<VideoData*>* pVideoList);

	 // Summary  :获得空间大小
	 // Returns	: __int64
	 __int64 GetTotalSize();

protected:

	// 执行命令
	virtual void Execute();

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

    
private:

	wstring GetVideoMime(wstring localpath, int& duration);
    

	enum VideoOperateType
	{ 
		VIDEO_ADD = 11,          // 添加视频
		VIDEO_REMOVE = 12,       // 移除视频
		VIDEO_RENAME = 13,       // 重命名
		VIDEO_GET_IMAGE = 101,   // 获得专辑封面
		VIDEO_GET_TOTAL_SIZE = 24// 获得空间大小
	};

    VideoOperateType   _type;		// 操作类型

    VideoData*         _pVideo;		//参数，被操作的视频
    wstring            _newName;	//参数，重命名传入的新名称

    wstring            _localpath;	//参数，添加视频时的本地路径
    wstring            _remotePath;	//参数，添加视频时已上传到手机端的路径

	const deque<VideoData*>* _pVideoList;//参数，获取视频封面传入的视频列表


    VideoData*         _pReturnVideo;// 返回值，添加的视频
    __int64            _size;        // 返回值，空间大小
    bool               _success;     // 返回值，成功失败
};

}
