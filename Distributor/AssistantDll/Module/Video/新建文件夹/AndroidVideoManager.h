#pragma once
namespace ND91Assistant
{
    // 根据原VideoManager和NDVideoManager的设计重新整理的Android视频操作类
    class AndroidVideoManager
    {
    public:
        AndroidVideoManager(const DeviceData* pDevice);
        virtual ~AndroidVideoManager();

		//************************************
		// Method:    InitEnvironment ：初始化设备环境
		// FullName:  ND91Assistant::IosVideoManager::InitEnvironment
		// Access:    public 
		// Returns:   ND91Assistant::eVideoInitCode ：返回环境错误码
		// Qualifier:
		//************************************
		eVideoInitCode InitEnvironment();

		//************************************
		// Method:    GetSupportVideoType ：获取设备支持的视频类型
		// FullName:  ND91Assistant::IosVideoManager::GetSupportVideoType
		// Access:    public 
		// Returns:   std::wstring ：返回视频类型串
		// Qualifier:
		//************************************
        wstring GetSupportVideoType(){ return L"*.mp4;*.m4v;*.wmv;*.3gp;*.ndv"; }


		//************************************
		// Method:    LoadMyVideoList ：加载非熊猫影音的视频
		// FullName:  ND91Assistant::IosVideoManager::LoadMyVideoList
		// Access:    public 
		// Returns:   void
		// Qualifier:
		// Parameter: deque<VideoData * > & vtMyVideoList ：存储影音信息列表
		//************************************
        void Load(deque<VideoData*>& vtMyVideoList, 
                  deque<VideoData*>& vt91PlayerWithoutAlbumVideoList,
                  deque<PlayAlbumData*>& vtPlayAlbumList);

		//************************************
		// Method:    LoadAlbumImage ：获取专辑封面截图
		// FullName:  ND91Assistant::NDVideoManager::LoadAlbumImage
		// Access:    public 
		// Returns:   bool ：成功返回TRUE
		// Qualifier:
		// Parameter: PlayAlbumData * pPlayAlbumData ：专辑信息
		//************************************
		bool LoadAlbumImage(PlayAlbumData* pPlayAlbumData);
		//************************************
		// Method:    LoadVideoImage ：获取视频预览图
		// FullName:  ND91Assistant::NDVideoManager::LoadVideoImage
		// Access:    public 
		// Returns:   bool
		// Qualifier:
		// Parameter: VideoData * pVideoData ：视频信息
		//************************************
		bool LoadVideoImage(VideoData* pVideoData);

		//************************************
		// Method:    AddMyVideo ：添加本地影音到手机，非熊猫影音目录
		// FullName:  ND91Assistant::NDVideoManager::AddMyVideo
		// Access:    public 
		// Returns:   VideoData*
		// Qualifier:
		// Parameter: wstring strLocalFullName ：本地文件名（含路径）
		// Parameter: ProgressSubject & subject ：进度反馈SUBJECT
		//************************************
		VideoData* AddMyVideo(wstring strLocalFullName, ProgressObserver* pObserver = NULL);
		//************************************
		// Method:    AddPandaPlayerVideo ：添加本地影音到熊猫影音目录
		// FullName:  ND91Assistant::NDVideoManager::AddPandaPlayerVideo
		// Access:    public 
		// Returns:   VideoData*
		// Qualifier:
		// Parameter: wstring strLocalFullName ：本地文件名（含路径）
		// Parameter: ProgressSubject & subject ：进度反馈SUBJECT
		//************************************
		VideoData* AddPandaPlayerVideo(wstring strLocalFullName, ProgressObserver* pObserver = NULL);		
		//************************************
		// Method:    AddNetVideo ：添加WEB上下载的视频
		// FullName:  ND91Assistant::NDVideoManager::AddNetVideo
		// Access:    public 
		// Returns:   VideoData*
		// Qualifier:
		// Parameter: PlayerAlbumItem * pPlayerAlbumItem ：视频在播放列表中的信息，有外层自行解析并申请和释放空间
		// Parameter: PlayAlbumData * pPlayAlbumData ：通过ParsePlayerAlbumPlist获取的PlayAlbumData，或者自行解析的PlayAlbumData
		// Parameter: ProgressSubject & subject ：进度反馈
		//************************************
		VideoData* AddNetVideo(PlayerAlbumItem* pPlayerAlbumItem, 
            PlayAlbumData* pPlayAlbumData, 
            deque<PlayAlbumData*>& vtPlayAlbumList,
            ProgressObserver* pObserver = NULL);

		//************************************
		// Method:    DeleteVideo ：删除手机上的影音
		// FullName:  ND91Assistant::NDVideoManager::DeleteVideo
		// Access:    public 
		// Returns:   bool
		// Qualifier:
		// Parameter: VideoData * pVideoData ：影音信息
		//************************************
		bool DeleteVideo(VideoData* pVideoData, deque<PlayAlbumData*>& vtAlbumDataList);

		//************************************
		// Method:    RenameVideo ：重命名手机上的影音
		// FullName:  ND91Assistant::NDVideoManager::RenameVideo
		// Access:    public 
		// Returns:   bool
		// Qualifier:
		// Parameter: VideoData * pVideoData ：影音信息
		// Parameter: wstring strNewName ：新名
		//************************************
		bool RenameVideo(VideoData* pVideoData, wstring strNewName);

		//************************************
		// Method:    DownLoadVideo ：下载视频文件至本地
		// FullName:  ND91Assistant::NDVideoManager::DownLoadVideo
		// Access:    public 
		// Returns:   bool ：成功返回TRUE
		// Qualifier:
		// Parameter: VideoData * pVideoData ：视频文件信息
		// Parameter: wstring strLocalSaveFullName ：视频文件存储的本地文件全名
		//************************************
		wstring DownLoadVideo(VideoData* pVideoData, wstring destDir, ProgressObserver* pObserver = NULL);

		//************************************
		// Method:    DeletePlayAlbum ：删除专辑
		// FullName:  ND91Assistant::NDVideoManager::DeletePlayAlbum
		// Access:    public 
		// Returns:   bool ：成功返回TRUE
		// Qualifier:
		// Parameter: PlayAlbumData * pPlayAlbumData ：专辑信息
		//************************************
		bool DeletePlayAlbum(PlayAlbumData* pPlayAlbumData);

        //获取影音根目录
        wstring GetVideoRootPath();
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
        const DeviceData* _pDevice;		// 设备信息指针
        wstring           _sdcardPath;  // SD卡路径
    };
}