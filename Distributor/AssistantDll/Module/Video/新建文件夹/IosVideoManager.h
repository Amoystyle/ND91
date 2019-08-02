#pragma once

namespace ND91Assistant
{
#define IOS_PXL_91PLAYER_PATH			L"/Applications/91Player.app"
#define IOS_PXL_PANDAPLAYER_PATH		L"/Applications/PandaPlayer.app"
#define IOS_HD_PXL_91PLAYER_PATH		L"/Applications/PandaPlayerHD.app"
#define IOS_IPA_91PLAYER_PATH			L"/Applications/91Player.app"
#define IOS_IPA_91PLAYER_IDENTIFIER		L"com.91.sj.iphone.PandaPlayerHD"
#define IOS_PXL_91PLAYER_IDENTIFIER		L"com.91.sj.iphone.91Player"

	//��èӰ������
	typedef enum eNDPlayerType
	{
		eNDPlayerType_IPA,
		eNDPlayerType_PXL,
		eNDPlayerType_NONE
	};

	//IOS�豸����Ƶ������
	class IosVideoManager
	{
	public:
		IosVideoManager(const DeviceData* pDevice);
		virtual ~IosVideoManager(void);

		//************************************
		// Method:    InitEnvironment ����ʼ���豸����
		// FullName:  ND91Assistant::IosVideoManager::InitEnvironment
		// Access:    public 
		// Returns:   ND91Assistant::eVideoInitCode �����ػ���������
		// Qualifier:
		//************************************
		eVideoInitCode InitEnvironment();

		//************************************
		// Method:    GetSupportVideoType ����ȡ�豸֧�ֵ���Ƶ����
		// FullName:  ND91Assistant::IosVideoManager::GetSupportVideoType
		// Access:    public 
		// Returns:   std::wstring ��������Ƶ���ʹ�
		// Qualifier:
		//************************************
		wstring GetSupportVideoType() {//return L"*.mp3;*.mp4;*.mpv;*.3gp;*.mov;*.m4r;*.wav;*.aac;*.ndv";
                                        return L"*.mp4;*.mpv;*.3gp;*.mov;*.ndv";}

		//************************************
		// Method:    LoadMyVideoList �����ط���èӰ������Ƶ
		// FullName:  ND91Assistant::IosVideoManager::LoadMyVideoList
		// Access:    public 
		// Returns:   void
		// Qualifier:
		// Parameter: deque<VideoData * > & vtMyVideoList ���洢Ӱ����Ϣ�б�
		//************************************
		void LoadMyVideoList(deque<VideoData*>& vtMyVideoList);

		//************************************
		// Method:    Load91PlayerVideoWithoutAlbum ��������èӰ����û��ר����Ϣ����Ƶ
		// FullName:  ND91Assistant::IosVideoManager::Load91PlayerVideoWithoutAlbum
		// Access:    public 
		// Returns:   void
		// Qualifier:
		// Parameter: deque<VideoData * > & vtVideoList ���洢Ӱ����Ϣ�б�
		//************************************
		void Load91PlayerVideoWithoutAlbum(deque<VideoData*>& vtVideoList);

		//************************************
		// Method:    Load91PlayerAlumData ��������èӰ������ר����Ϣ����Ƶ
		// FullName:  ND91Assistant::IosVideoManager::Load91PlayerAlumData
		// Access:    public 
		// Returns:   void
		// Qualifier:
		// Parameter: deque<PlayAlbumData * > & vtPlayAlbumList ���洢ר����Ϣ������Ӱ����Ϣ
		//************************************
		void Load91PlayerAlumData(deque<PlayAlbumData*>& vtPlayAlbumList);

		//************************************
		// Method:    AddMyVideo ����ӱ�����Ƶ�ļ�
		// FullName:  ND91Assistant::IosVideoManager::AddMyVideo
		// Access:    public 
		// Returns:   std::wstring ��������Ӻ����Ƶ���ֻ��ϵ�ȫ�������򷵻�L""
		// Qualifier:
		// Parameter: wstring strLocalFullName ����Ƶ�ļ�ȫ��
		// Parameter: ePlayAlbumType eType ���洢���ͣ��Ƿ�Ϊ��èӰ��
		// Parameter: ProgressSubject & subject �����ȷ���SUBJECT
		//************************************
		wstring AddMyVideo(wstring strLocalFullName, ePlayAlbumType eType, ProgressObserver* pObserver = NULL);

		//************************************
		// Method:    DeleteVideoFile ��ɾ����Ƶ
		// FullName:  ND91Assistant::IosVideoManager::DeleteVideoFile
		// Access:    public 
		// Returns:   bool ���ɹ�����TRUE
		// Qualifier:
		// Parameter: VideoData * pVideoData ����Ƶ�ļ���Ϣ
		//************************************
		bool DeleteVideoFile(VideoData* pVideoData);

		//************************************
		// Method:    RenameVideoFile ����������Ƶ�ļ�
		// FullName:  ND91Assistant::IosVideoManager::RenameVideoFile
		// Access:    public 
		// Returns:   bool ���ɹ�����TRUE
		// Qualifier:
		// Parameter: VideoData * pVideoData ����Ƶ�ļ���Ϣ
		// Parameter: wstring strNewName ���µ���Ƶ�ļ���
		//************************************
		bool RenameVideoFile(VideoData* pVideoData, wstring strNewName);

		//************************************
		// Method:    AddPandaVideo �������ר����Ϣ����èӰ���ļ�
		// FullName:  ND91Assistant::IosVideoManager::AddPandaVideo
		// Access:    public 
		// Returns:   std::wstring ���ɹ�������Ӻ����Ƶ���ֻ��ϵ�ȫ�������򷵻�L""
		// Qualifier:
		// Parameter: wstring strLocalVideoFullName ����Ƶ�ļ�ȫ����PC�ϣ�
		// Parameter: wstring strVideoNameOnPhone ����Ƶ�ļ������豸�ϣ�
		// Parameter: wstring strLocalAlbumImageFullName ��ר��ͼƬȫ����PC�ϣ�
		// Parameter: wstring strAlbumName ��ר����
		// Parameter: ProgressSubject & subject �����ȷ���
		//************************************
		wstring AddPandaVideo(wstring strLocalVideoFullName, wstring strVideoNameOnPhone, wstring strLocalAlbumImageFullName, 
			wstring strAlbumName, ProgressObserver* pObserver = NULL);

		//************************************
		// Method:    LoadAlbumImage ������ר������ͼƬ
		// FullName:  ND91Assistant::IosVideoManager::LoadAlbumImage
		// Access:    public 
		// Returns:   bool ���ɹ�����TRUE
		// Qualifier:
		// Parameter: wstring strLocalFullName �����ش洢·��
		// Parameter: wstring strAlbumName ��ר����
		//************************************
		bool LoadAlbumImage(wstring strLocalFullName, wstring strAlbumName);

		//************************************
		// Method:    DownLoadFile �����豸�������ļ�
		// FullName:  ND91Assistant::IosVideoManager::DownLoadFile
		// Access:    public 
		// Returns:   bool ���ɹ�����TRUE
		// Qualifier:
		// Parameter: wstring strFileOnDev ���豸�ϵ��ļ�ȫ��
		// Parameter: wstring strFileOnPC ��PC�ϵ��ļ�ȫ��
		//************************************
		bool DownLoadFile(wstring strFileOnDev, wstring strFileOnPC, ProgressObserver* pObserver = NULL);

		//************************************
		// Method:    DeleteFolder ��ɾ���ļ���
		// FullName:  ND91Assistant::IosVideoManager::DeleteFolder
		// Access:    public 
		// Returns:   bool ���ɹ�����TRUE
		// Qualifier:
		// Parameter: wstring strFolderOnDev ���ļ���ȫ��
		//************************************
		bool DeleteAlbum(wstring strAlbumName);

        //��ȡӰ����Ŀ¼
        wstring GetVideoRootPath();
	private:		
		//��ȡ��èӰ�����ͣ�PXL��IPA
		eNDPlayerType GetPlayerType();

		//������èӰ�����ͻ�ȡ�ļ�����ָ��
		IIosFileSystem* GetFileStream(eNDPlayerType ePlayerType);

		//��ȡ��èӰ���洢��Ƶ��Ŀ¼
		wstring GetVideoBookingPath();
		//��ȡ����èӰ���洢��Ƶ��Ŀ¼
		wstring GetVideoNoBookingPath();

		//ͨ���豸�ϵ�·������Ӱ����Ϣ�б�
		void GetVideosFromPath(deque<VideoData*>& vtMyVideoList, wstring strPath, bool bIncludeDir = true, PlayAlbumData* pAlbumData = NULL);

		//ͨ���ļ���Ϣ��ר����Ϣ��ȡӰ������Ϣ
		VideoData* GetVideoData(NodeData* pFile, PlayAlbumData* pAlbumData);

		//�ж��Ƿ�Ϊ��Ƶ�ļ�
		bool IsValidVideo(wstring strName);

	private:
		const DeviceData* _pDevice;		// �豸��Ϣָ��
		eNDPlayerType _ePlayerType;		// ��èӰ�����ͣ�IPA����PXL
		IIosFileSystem* _pFileStream;	// �ļ�����ָ��
	};
}
