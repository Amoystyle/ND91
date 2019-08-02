#pragma once
#define MODULE_VIDEO

namespace ND91Assistant
{
    class DeviceData;
    class VideoData;
    class DirData;
    class PlayAlbumData;
    class PlayerAlbumItem;
    
    // 视频管理类
    class CLASSINDLL_CLASS_DECL VideoManager
    {
    public:
        VideoManager(const DeviceData* pDevice);
        virtual ~VideoManager();

        // Summary  :加载数据
        // Returns	: void
        // Parameter: 增加是否取文件大小选项，因为去文件大小很耗时间,文件大小另取
        void Load();

        // Summary  : 检测是否需要安装熊猫影音
        // Returns	: bool
        bool IsNeedInstall91Player();

        // Summary  : 获取视频封面

        void GetVideoImages(const deque<VideoData*>* videoList);

		// Summary  : 获得专辑图
		// Returns	: std::wstring
		// Parameter: PlayAlbumData * pAlbum
		wstring GetAlbumImage(PlayAlbumData* pAlbum);

        // Summary  : 添加我的视频
        // Returns	: void
        // Parameter: const deque<wstring> localVideoPaths
        void AddMyVideos(const deque<wstring> localVideoPaths, ProgressObserver* pObserver = NULL);

        // Summary  : 添加熊猫影音视频
        // Returns	: void
        // Parameter: const deque<wstring> localVideoPaths
        void Add91PlayerVideos(const deque<wstring> localVideoPaths, ProgressObserver* pObserver = NULL);


		// Summary  : 解析网络下载的专辑plist文件
		// Returns	: PlayAlbumData*
		// Parameter: wstring localPlistXml
		PlayAlbumData* ParseAlbumPlist( wstring localPlistXml);

        // Summary  : 用于添加网站上下载下来的视频
        // Returns	: void
        // Parameter: PlayerAlbumItem * albumItem 专辑播放列表项，从url解析得到
        // Parameter: wstring localPlistXml 下载下来的专辑信息配置文件
        void Add91PlayerAlbumVideo(PlayerAlbumItem* albumItem, PlayAlbumData* albumData, ProgressObserver* pObserver = NULL);

        // Summary  : 删除视频
        // Returns	: void
        // Parameter: const deque<VideoData * > * videoList
        void DeleteVideo(const deque<VideoData*>* videoList);

        // Summary  : 我的视频
        // Returns	: const deque<VideoData*>
        const deque<VideoData*>* GetMyVideoList(){return &_pMyVideoList;}

        // Summary  : 91影音视频
        // Returns	: const deque<VideoData*>
        const deque<VideoData*>* Get91PlayerVideoList(){return &_p91playerVideoList;}

        // Summary  : 91影音专辑列表
        // Returns	: const deque<PlayAlbumData*>
        const deque<PlayAlbumData*>* Get91PlayerAlbumList(){return &_playAlbumList;}

		VideoData* Find91PlayerVideoByRemotePath(wstring remotePath);

        // 释放
        void  Release();

    private:

        // Summary  :加载全部视频
        // Returns	: void
        void LoadAllVideos();

        // Summary  : 加载熊猫影音专辑
        // Returns	: void
        void Load91PlayerAlbum();

        // Summary  : 关联熊猫影音视频和专辑
        // Returns	: void
        void Init91PlayerAlbum();

        // Summary  : 加载熊猫影音专辑，递归用
        // Returns	: void
        // Parameter: DirData * pDir
        void Load91PlayerAlbum(DirData* pDir);

        // Summary  : 调命令添加图片
        // Returns	: bool
        // Parameter: const wstring localVideoPath视频本地路径
        // Parameter: wstring remotePath传到手机端的路径，不包含文件名
        bool AddVideo( const wstring localVideoPath, wstring remotePath, deque<VideoData*>& toList, ProgressObserver* pObserver = NULL );

        // Summary  : 插入视频到列表
        // Returns	: bool 存在相同路径的
        // Parameter: VideoData * video 
        // Parameter: deque<VideoData * > & ToList要插入的列表
        // Parameter: bool del 是否删除相同的
        bool InsertOneVideoToList(VideoData* video, deque<VideoData*>& ToList, bool del=true);

        // Summary  : 从列表中移除视频
        // Returns	: void
        // Parameter: VideoData * video
        // Parameter: deque<VideoData * >* fromList
        bool  RemoveVideoFromList(VideoData* video, deque<VideoData*>& fromList);

        // Summary  : 通过ID找到专辑
        // Returns	: PlayAlbumData*
        // Parameter: wstring id
        PlayAlbumData* FindPlayAlbumById(wstring id);

        // Summary  : 下载专辑图片并上传到手机
        // Returns	: void
        // Parameter: wstring localSavePath本地存储路径
        // Parameter: wstring url 下载地址url
        // Parameter: wstring toUploadRemotePath 要上传到的手机端路径
        void DownLoadUrlImage(wstring localSavePath,  wstring  url, wstring toUploadRemotePath);


        // Summary  : 获得sd卡路径
        // Returns	: std::wstring
        wstring GetSdcardPath();

		wstring GetAlbumRootPath();

    private:
        deque<VideoData*>   _pMyVideoList;       // 手机上视频

        deque<VideoData*>   _p91playerVideoList; // 熊猫影音视频

        deque<PlayAlbumData*>  _playAlbumList;   // 熊猫影音视频专辑

        const DeviceData*   _pDevice;

        wstring             _sdcardPath;         // SD卡路径
        bool                _noSdcard;
    };

}