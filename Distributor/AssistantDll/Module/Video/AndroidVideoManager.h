#pragma once

#include "Module/Video/VideoManager.h"

namespace ND91Assistant
{
    // 根据原VideoManager的设计重新整理的Android视频操作类
    class AndroidVideoManager : public VideoManager
    {
    public:
        AndroidVideoManager(const DeviceData* pDevice);
        virtual ~AndroidVideoManager();

        //初始化环境
        virtual eVideoInitCode InitEnvironment();

        //加载所有影音列表，每次调用都会清空之前的数据
        virtual void Load();

        // 获取设备支持的视频类型
        virtual wstring GetSupportVideoType();

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
		virtual wstring DownLoadVideo(VideoData* pVideoData, wstring destDir, ProgressObserver* pObserver = NULL);

        //获取影音根目录
        virtual wstring GetVideoRootPath();

    protected:
        // Summary  : 获得sd卡路径
        // Returns	: std::wstring
        wstring GetSdcardPath();
        // Summary  : 插入视频到列表
        // Returns	: bool 存在相同路径的
        // Parameter: VideoData * video 
        // Parameter: deque<VideoData * > & ToList要插入的列表
        // Parameter: bool del 是否删除相同的
        bool InsertOneVideoToList(VideoData* video, deque<VideoData*>& vtList, bool del=true);
        // Summary  : 加载熊猫影音专辑
        // Returns	: void
        void Load91PlayerAlbum(deque<PlayAlbumData*>& vtAlbumList);
        // Summary  : 加载熊猫影音专辑，递归用
        // Returns	: void
        // Parameter: DirData * pDir
        void Load91PlayerAlbum(deque<PlayAlbumData*>& vtAlbumList, DirData* pDir);
        // Summary  : 调命令添加图片
        // Returns	: bool
        // Parameter: const wstring localVideoPath视频本地路径
        // Parameter: wstring remotePath传到手机端的路径，不包含文件名
        VideoData* AddVideo( const wstring localVideoPath, wstring remotePath, ProgressObserver* pObserver = NULL);

        PlayAlbumData* InsertAlbumData(PlayAlbumData* pPlayAlbumData, deque<PlayAlbumData*>& vtPlayAlbumList);
        PlayerAlbumItem* InsertAlbumItem(PlayAlbumData* pPlayAlbumData, PlayerAlbumItem* pPlayAlbumItem);
        PlayAlbumData* FindAlbumData(wstring strAlbumID, deque<PlayAlbumData*>& vtPlayAlbumList);
        

        wstring GetAlbumRootPath();
    private:
        wstring           _sdcardPath;  // SD卡路径


    protected:
        virtual bool DeletePlayAlbum_Action(PlayAlbumData* pPlayAlbumData);
        virtual bool DeleteVideo_Action(VideoData* pVideoData);
        virtual bool RenameVideo_Action(VideoData* pVideoData, wstring strNewName);
    };
}