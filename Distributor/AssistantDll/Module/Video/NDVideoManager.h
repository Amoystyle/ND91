#pragma once

namespace ND91Assistant
{
	//熊猫影音操作类
	class CLASSINDLL_CLASS_DECL NDVideoManager
	{
	public:
		NDVideoManager(const DeviceData* pDevice);
		~NDVideoManager(void);

		//************************************
		// Method:    InitEnvironment 初始化环境，调用每个接口前都需先调用，以反馈用户缺失的环境配置
		// FullName:  ND91Assistant::NDVideoManager::InitEnvironment
		// Access:    public 
		// Returns:   ND91Assistant::eVideoInitCode ：环境缺失错误码
		// Qualifier:
		//************************************
		eVideoInitCode InitEnvironment();

		//************************************
		// Method:    GetSupportVideoType ：获取支持的影音格式
		// FullName:  ND91Assistant::NDVideoManager::GetSupportVideoType
		// Access:    public 
		// Returns:   std::wstring ：影音格式串
		// Qualifier:
		//************************************
		wstring GetSupportVideoType();

		//************************************
		// Method:    Load ：加载所有影音列表，每次调用都会清空之前的数据
		// FullName:  ND91Assistant::NDVideoManager::Load
		// Access:    public 
		// Returns:   void
		// Qualifier:
		//************************************
		void Load();

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
		// Parameter: wstring strLocalFullName ：本地文件名（含路径）
		// Parameter: ProgressSubject & subject ：进度反馈SUBJECT
		//************************************
		VideoData* AddPandaPlayerVideo(wstring strLocalFullName, ProgressObserver* pObserver = NULL);		

		//************************************
		// Method:    DeleteVideo ：删除手机上的影音
		// FullName:  ND91Assistant::NDVideoManager::DeleteVideo
		// Access:    public 
		// Returns:   bool
		// Qualifier:
		// Parameter: VideoData * pVideoData ：影音信息
		//************************************
		bool DeleteVideo(VideoData* pVideoData);

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
		// Method:    GetAlbumList ：获取加载后的影音列表
		// FullName:  ND91Assistant::NDVideoManager::GetAlbumList
		// Access:    public 
		// Returns:   deque<PlayAlbumData*>
		// Qualifier:
		//************************************
		const deque<PlayAlbumData*>* GetAlbumList();

		//************************************
		// Method:    GetMyVideoAlbumData ：获取非熊猫影音的视频专辑
		// FullName:  ND91Assistant::NDVideoManager::GetMyVideoAlbumData
		// Access:    public 
		// Returns:   PlayAlbumData* ：返回专辑数据指针
		// Qualifier:
		//************************************
		PlayAlbumData* GetMyVideoAlbumData();

		//************************************
		// Method:    GetWithoutAlbumPandaVideoAlbumData ：获取熊猫影音中无专辑信息的专辑
		// FullName:  ND91Assistant::NDVideoManager::GetWithoutAlbumPandaVideoAlbumData
		// Access:    public 
		// Returns:   PlayAlbumData* ：返回专辑数据指针
		// Qualifier:
		//************************************
		PlayAlbumData* GetWithoutAlbumPandaVideoAlbumData();

		//************************************
		// Method:    ParsePlayerAlbumPlist ：解析专辑信息
		// FullName:  ND91Assistant::NDVideoManager::ParsePlayerAlbumPlist
		// Access:    public 
		// Returns:   bool
		// Qualifier:
		// Parameter: wstring strPlistFileName ：专辑信息的PLIST文件
		// Parameter: PlayAlbumData * pPlayAlbumData ：专辑信息指针，为适应之后WEB协议的更改，故此对象由外层申请和释放
		//************************************
		bool ParsePlayerAlbumPlist(wstring strPlistFileName, PlayAlbumData* pPlayAlbumData);

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
		VideoData* AddNetVideo(PlayerAlbumItem* pPlayerAlbumItem, PlayAlbumData* pPlayAlbumData, ProgressObserver* pObserver = NULL);

		//************************************
		// Method:    DownLoadVideo ：下载视频文件至本地
		// FullName:  ND91Assistant::NDVideoManager::DownLoadVideo
		// Access:    public 
		// Returns:   bool ：成功返回视频文件存储的本地文件全名,否则返回L""
		// Qualifier:
		// Parameter: VideoData * pVideoData ：视频文件信息
		// Parameter: wstring strLocalSaveFullName ：视频文件存储的本地路径
		//************************************
		wstring DownLoadVideo(VideoData* pVideoData, wstring strLocalSaveDir, ProgressObserver* pObserver = NULL);

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
	private:
		//添加本地影音
		VideoData* AddLocalVideo(wstring strLocalFullName, ePlayAlbumType eType, ProgressObserver* pObserver);
		//根据专辑类型创建专辑对象
		PlayAlbumData* CreatePlayAlbumData(ePlayAlbumType eType);	
		//清空存储的影音视频信息
		void ClearList();
		//添加没有专辑信息的视频文件信息
		void AddWithoutAlbumInfoVideo(deque<VideoData*> videoList, ePlayAlbumType eType);
		//查找没有专辑信息的专辑，没有找到，则自动创建
		//PlayAlbumData* FindWithoutAlbumInfoPlayAlbumData(ePlayAlbumType eType);
		//添加专辑信息列表
		void AddAlbumDatas(deque<PlayAlbumData*> vtPlayAlbumList);
		//查找有专辑信息的专辑
		PlayAlbumData* FindWithAlbumInfoAlbumData(PlayAlbumData* pPlayAlbumData);
        void RemoveAlbumData(PlayAlbumData* pPlayAlbumData);

	private:
		IosVideoManager*        _pIosVideoManager; //IOS影音操作类
		AndroidVideoManager*    _pAndroidVideoManager; //ANDROID影音操作类
		deque<PlayAlbumData*>   _playAlbumList;   // 熊猫影音视频专辑
		PlayAlbumData*          _myVideos;
		PlayAlbumData*          _other91PlayerVideos;
		const DeviceData*		_pDevice; //设备指针
		wstring					_strLocalAlbumPath; //专辑图片存储路径
		wstring					_strLocalVideoPath; //视频图片存储路径
	};
}
