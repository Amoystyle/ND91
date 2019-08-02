#pragma once

#include "Module/Video/VideoData.h"
#include "Module/Video/PlayAlbumData.h"

namespace ND91Assistant
{
	//熊猫影音操作类
	class CLASSINDLL_CLASS_DECL VideoManager
	{
	public:
		VideoManager(const DeviceData* pDevice);
		virtual ~VideoManager(void);

        //初始化环境
		virtual eVideoInitCode InitEnvironment() = 0;

        //加载所有影音列表，每次调用都会清空之前的数据
        virtual void Load() = 0;

        virtual int GetVideoCount();

        // 获取设备支持的视频类型
        virtual wstring GetSupportVideoType() = 0;

        // 获取专辑封面截图
        virtual bool LoadAlbumImage(PlayAlbumData* pPlayAlbumData) = 0;

        // 获取视频预览图
        virtual bool LoadVideoImage(VideoData* pVideoData) = 0;

        // 添加本地影音到手机，非熊猫影音目录
        virtual VideoData* AddMyVideo(wstring strLocalFullName, ProgressObserver* pObserver = NULL) = 0;

        // 添加本地影音到熊猫影音目录
        virtual VideoData* AddPandaPlayerVideo(wstring strLocalFullName, ProgressObserver* pObserver = NULL) = 0;

        // 添加WEB上下载的视频
        virtual VideoData* AddNetVideo(PlayerAlbumItem* pPlayerAlbumItem, PlayAlbumData* pPlayAlbumData, ProgressObserver* pObserver = NULL) = 0;

        // 下载视频文件至本地
        virtual wstring DownLoadVideo(VideoData* pVideoData, wstring strLocalSaveDir, ProgressObserver* pObserver = NULL) = 0;

        //获取影音根目录
        virtual wstring GetVideoRootPath() = 0;

        // 释放已加载的数据
        void Release();

        // 删除专辑
        bool DeletePlayAlbum(PlayAlbumData* pPlayAlbumData);

        // 删除手机上的影音
		bool DeleteVideo(VideoData* pVideoData);

        // 重命名手机上的影音
		bool RenameVideo(VideoData* pVideoData, wstring strNewName);

        // 获取加载后的影音列表
		const deque<PlayAlbumData*>* GetAlbumList();

        // 获取非熊猫影音的视频专辑
		PlayAlbumData* GetMyVideoAlbumData();

		// 获取熊猫影音中无专辑信息的专辑
		PlayAlbumData* GetWithoutAlbumPandaVideoAlbumData();

		// 解析专辑信息
		bool ParsePlayerAlbumPlist(wstring strPlistFileName, PlayAlbumData* pPlayAlbumData);

    protected:
        //添加没有专辑信息的视频文件信息
        void AddWithoutAlbumInfoVideo(deque<VideoData*> videoList, ePlayAlbumType eType);

        //添加专辑信息列表
        void AddAlbumDatas(deque<PlayAlbumData*> vtPlayAlbumList);

        // 添加Net视频 AddNetVideo 处理
        void CheckNetVideo(PlayerAlbumItem* pPlayerAlbumItem, PlayAlbumData* pPlayAlbumData, VideoData* pVideoData, wstring strAlbumImageremotePath);

        virtual bool DeletePlayAlbum_Action(PlayAlbumData* pPlayAlbumData) = 0;
        virtual bool DeleteVideo_Action(VideoData* pVideoData) = 0;
        virtual bool RenameVideo_Action(VideoData* pVideoData, wstring strNewName) = 0;

        //根据专辑类型创建专辑对象
        PlayAlbumData* CreatePlayAlbumData(ePlayAlbumType eType);	

		//添加本地影音
		VideoData* AddLocalVideo(wstring strLocalFullName, ePlayAlbumType eType, ProgressObserver* pObserver);

		//查找没有专辑信息的专辑，没有找到，则自动创建
		//PlayAlbumData* FindWithoutAlbumInfoPlayAlbumData(ePlayAlbumType eType);
		//查找有专辑信息的专辑
		PlayAlbumData* FindWithAlbumInfoAlbumData(PlayAlbumData* pPlayAlbumData);
        void RemoveAlbumData(PlayAlbumData* pPlayAlbumData);

	protected:
        const DeviceData*		_pDevice;               //设备指针
        deque<PlayAlbumData*>   _playAlbumList;         //熊猫影音视频专辑
        PlayAlbumData*          _myVideos;              //我的视频（不在熊猫影音里面的）
        PlayAlbumData*          _other91PlayerVideos;   //其他熊猫影音视频(没有专辑分类)

	    wstring					_strLocalAlbumPath;     //专辑图片存储路径
		wstring					_strLocalVideoPath;     //视频图片存储路径
	};
}
