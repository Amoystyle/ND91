#pragma once

#include "Module/Video/VideoManager.h"

namespace ND91Assistant
{
#define IOS_PXL_91PLAYER_PATH			L"/Applications/91Player.app"
#define IOS_PXL_PANDAPLAYER_PATH		L"/Applications/PandaPlayer.app"
#define IOS_HD_PXL_91PLAYER_PATH		L"/Applications/PandaPlayerHD.app"
#define IOS_IPA_91PLAYER_PATH			L"/Applications/91Player.app"
#define IOS_IPA_91PLAYER_IDENTIFIER		L"com.91.sj.iphone.PandaPlayerHD"
#define IOS_PXL_91PLAYER_IDENTIFIER		L"com.91.sj.iphone.91Player"

	//熊猫影音类型
	typedef enum eNDPlayerType
	{
		eNDPlayerType_IPA,
		eNDPlayerType_PXL,
		eNDPlayerType_NONE
	};

	//IOS设备的视频操作类
	class IosVideoManager : public VideoManager
	{
	public:
		IosVideoManager(const DeviceData* pDevice);
		virtual ~IosVideoManager(void);

        //初始化环境
        virtual eVideoInitCode InitEnvironment();

        //加载所有影音列表，每次调用都会清空之前的数据
        virtual void Load();

        // 获取设备支持的视频类型
        wstring GetSupportVideoType();

        // 获取专辑封面截图
        virtual bool LoadAlbumImage(PlayAlbumData* pPlayAlbumData);

        // 获取视频预览图
        virtual bool LoadVideoImage(VideoData* pVideoData);

        // 添加本地影音到手机，非熊猫影音目录
        virtual VideoData* AddMyVideo(wstring strLocalFullName, ProgressObserver* pObserver = NULL);

        // 添加本地影音到熊猫影音目录
        virtual VideoData* AddPandaPlayerVideo(wstring strLocalFullName, ProgressObserver* pObserver = NULL);		

        // 添加WEB上下载的视频
        virtual VideoData* AddNetVideo(PlayerAlbumItem* pPlayerAlbumItem, PlayAlbumData* pPlayAlbumData, ProgressObserver* pObserver = NULL);

        // 下载视频文件至本地
        virtual wstring DownLoadVideo(VideoData* pVideoData, wstring strLocalSaveDir, ProgressObserver* pObserver = NULL);

        //获取影音根目录
        virtual wstring GetVideoRootPath();
    
    protected:
        virtual bool DeletePlayAlbum_Action(PlayAlbumData* pPlayAlbumData);
        virtual bool DeleteVideo_Action(VideoData* pVideoData);
        virtual bool RenameVideo_Action(VideoData* pVideoData, wstring strNewName);
    private:
		// 加载非熊猫影音的视频
		void LoadMyVideoList(deque<VideoData*>& vtMyVideoList);

		// 加载熊猫影音中没有专辑信息的视频
        void Load91PlayerVideoWithoutAlbum(deque<VideoData*>& vtVideoList);

		// 加载熊猫影音中有专辑信息的视频
		void Load91PlayerAlumData(deque<PlayAlbumData*>& vtPlayAlbumList);

        // 添加本地视频文件
        wstring AddMyVideo(wstring strLocalFullName, ePlayAlbumType eType, ProgressObserver* pObserver = NULL);

        // 添加有专辑信息的熊猫影音文件
        wstring AddPandaVideo(wstring strLocalVideoFullName, wstring strVideoNameOnPhone, wstring strLocalAlbumImageFullName, 
                                wstring strAlbumName, ProgressObserver* pObserver = NULL);
	private:		
		//获取熊猫影音类型，PXL或IPA
		eNDPlayerType GetPlayerType();

		//根据熊猫影音类型获取文件操作指针
		IIosFileSystem* GetFileStream(eNDPlayerType ePlayerType);

		//获取熊猫影音存储视频的目录
		wstring GetVideoBookingPath();
		//获取非熊猫影音存储视频的目录
		wstring GetVideoNoBookingPath();

		//通过设备上的路径加载影音信息列表
		void GetVideosFromPath(deque<VideoData*>& vtMyVideoList, wstring strPath, bool bIncludeDir = true, PlayAlbumData* pAlbumData = NULL);

		//通过文件信息和专辑信息获取影音的信息
		VideoData* GetVideoData(NodeData* pFile, PlayAlbumData* pAlbumData);

		//判断是否为视频文件
		bool IsValidVideo(wstring strName);

	private:
		eNDPlayerType _ePlayerType;		// 熊猫影音类型，IPA或者PXL
		IIosFileSystem* _pFileStream;	// 文件操作指针
	};
}
