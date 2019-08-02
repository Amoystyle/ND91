#pragma once

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
	class IosVideoManager
	{
	public:
		IosVideoManager(const DeviceData* pDevice);
		virtual ~IosVideoManager(void);

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
		wstring GetSupportVideoType() {//return L"*.mp3;*.mp4;*.mpv;*.3gp;*.mov;*.m4r;*.wav;*.aac;*.ndv";
                                        return L"*.mp4;*.mpv;*.3gp;*.mov;*.ndv";}

		//************************************
		// Method:    LoadMyVideoList ：加载非熊猫影音的视频
		// FullName:  ND91Assistant::IosVideoManager::LoadMyVideoList
		// Access:    public 
		// Returns:   void
		// Qualifier:
		// Parameter: deque<VideoData * > & vtMyVideoList ：存储影音信息列表
		//************************************
		void LoadMyVideoList(deque<VideoData*>& vtMyVideoList);

		//************************************
		// Method:    Load91PlayerVideoWithoutAlbum ：加载熊猫影音中没有专辑信息的视频
		// FullName:  ND91Assistant::IosVideoManager::Load91PlayerVideoWithoutAlbum
		// Access:    public 
		// Returns:   void
		// Qualifier:
		// Parameter: deque<VideoData * > & vtVideoList ：存储影音信息列表
		//************************************
		void Load91PlayerVideoWithoutAlbum(deque<VideoData*>& vtVideoList);

		//************************************
		// Method:    Load91PlayerAlumData ：加载熊猫影音中有专辑信息的视频
		// FullName:  ND91Assistant::IosVideoManager::Load91PlayerAlumData
		// Access:    public 
		// Returns:   void
		// Qualifier:
		// Parameter: deque<PlayAlbumData * > & vtPlayAlbumList ：存储专辑信息，包含影音信息
		//************************************
		void Load91PlayerAlumData(deque<PlayAlbumData*>& vtPlayAlbumList);

		//************************************
		// Method:    AddMyVideo ：添加本地视频文件
		// FullName:  ND91Assistant::IosVideoManager::AddMyVideo
		// Access:    public 
		// Returns:   std::wstring ：返回添加后的视频在手机上的全名，否则返回L""
		// Qualifier:
		// Parameter: wstring strLocalFullName ：视频文件全名
		// Parameter: ePlayAlbumType eType ：存储类型，是否为熊猫影音
		// Parameter: ProgressSubject & subject ：进度反馈SUBJECT
		//************************************
		wstring AddMyVideo(wstring strLocalFullName, ePlayAlbumType eType, ProgressObserver* pObserver = NULL);

		//************************************
		// Method:    DeleteVideoFile ：删除视频
		// FullName:  ND91Assistant::IosVideoManager::DeleteVideoFile
		// Access:    public 
		// Returns:   bool ：成功返回TRUE
		// Qualifier:
		// Parameter: VideoData * pVideoData ：视频文件信息
		//************************************
		bool DeleteVideoFile(VideoData* pVideoData);

		//************************************
		// Method:    RenameVideoFile ：重命名视频文件
		// FullName:  ND91Assistant::IosVideoManager::RenameVideoFile
		// Access:    public 
		// Returns:   bool ：成功返回TRUE
		// Qualifier:
		// Parameter: VideoData * pVideoData ：视频文件信息
		// Parameter: wstring strNewName ：新的视频文件名
		//************************************
		bool RenameVideoFile(VideoData* pVideoData, wstring strNewName);

		//************************************
		// Method:    AddPandaVideo ：添加有专辑信息的熊猫影音文件
		// FullName:  ND91Assistant::IosVideoManager::AddPandaVideo
		// Access:    public 
		// Returns:   std::wstring ：成功返回添加后的视频在手机上的全名，否则返回L""
		// Qualifier:
		// Parameter: wstring strLocalVideoFullName ：视频文件全名（PC上）
		// Parameter: wstring strVideoNameOnPhone ：视频文件名（设备上）
		// Parameter: wstring strLocalAlbumImageFullName ：专辑图片全名（PC上）
		// Parameter: wstring strAlbumName ：专辑名
		// Parameter: ProgressSubject & subject ：进度反馈
		//************************************
		wstring AddPandaVideo(wstring strLocalVideoFullName, wstring strVideoNameOnPhone, wstring strLocalAlbumImageFullName, 
			wstring strAlbumName, ProgressObserver* pObserver = NULL);

		//************************************
		// Method:    LoadAlbumImage ：加载专辑封面图片
		// FullName:  ND91Assistant::IosVideoManager::LoadAlbumImage
		// Access:    public 
		// Returns:   bool ：成功返回TRUE
		// Qualifier:
		// Parameter: wstring strLocalFullName ：本地存储路径
		// Parameter: wstring strAlbumName ：专辑名
		//************************************
		bool LoadAlbumImage(wstring strLocalFullName, wstring strAlbumName);

		//************************************
		// Method:    DownLoadFile ：从设备端下载文件
		// FullName:  ND91Assistant::IosVideoManager::DownLoadFile
		// Access:    public 
		// Returns:   bool ：成功返回TRUE
		// Qualifier:
		// Parameter: wstring strFileOnDev ：设备上的文件全名
		// Parameter: wstring strFileOnPC ：PC上的文件全名
		//************************************
		bool DownLoadFile(wstring strFileOnDev, wstring strFileOnPC, ProgressObserver* pObserver = NULL);

		//************************************
		// Method:    DeleteFolder ：删除文件夹
		// FullName:  ND91Assistant::IosVideoManager::DeleteFolder
		// Access:    public 
		// Returns:   bool ：成功返回TRUE
		// Qualifier:
		// Parameter: wstring strFolderOnDev ：文件夹全名
		//************************************
		bool DeleteAlbum(wstring strAlbumName);

        //获取影音根目录
        wstring GetVideoRootPath();
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
		const DeviceData* _pDevice;		// 设备信息指针
		eNDPlayerType _ePlayerType;		// 熊猫影音类型，IPA或者PXL
		IIosFileSystem* _pFileStream;	// 文件操作指针
	};
}
