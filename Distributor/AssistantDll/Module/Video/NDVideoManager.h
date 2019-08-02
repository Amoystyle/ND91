#pragma once

namespace ND91Assistant
{
	//��èӰ��������
	class CLASSINDLL_CLASS_DECL NDVideoManager
	{
	public:
		NDVideoManager(const DeviceData* pDevice);
		~NDVideoManager(void);

		//************************************
		// Method:    InitEnvironment ��ʼ������������ÿ���ӿ�ǰ�����ȵ��ã��Է����û�ȱʧ�Ļ�������
		// FullName:  ND91Assistant::NDVideoManager::InitEnvironment
		// Access:    public 
		// Returns:   ND91Assistant::eVideoInitCode ������ȱʧ������
		// Qualifier:
		//************************************
		eVideoInitCode InitEnvironment();

		//************************************
		// Method:    GetSupportVideoType ����ȡ֧�ֵ�Ӱ����ʽ
		// FullName:  ND91Assistant::NDVideoManager::GetSupportVideoType
		// Access:    public 
		// Returns:   std::wstring ��Ӱ����ʽ��
		// Qualifier:
		//************************************
		wstring GetSupportVideoType();

		//************************************
		// Method:    Load ����������Ӱ���б�ÿ�ε��ö������֮ǰ������
		// FullName:  ND91Assistant::NDVideoManager::Load
		// Access:    public 
		// Returns:   void
		// Qualifier:
		//************************************
		void Load();

		//************************************
		// Method:    LoadAlbumImage ����ȡר�������ͼ
		// FullName:  ND91Assistant::NDVideoManager::LoadAlbumImage
		// Access:    public 
		// Returns:   bool ���ɹ�����TRUE
		// Qualifier:
		// Parameter: PlayAlbumData * pPlayAlbumData ��ר����Ϣ
		//************************************
		bool LoadAlbumImage(PlayAlbumData* pPlayAlbumData);

		//************************************
		// Method:    LoadVideoImage ����ȡ��ƵԤ��ͼ
		// FullName:  ND91Assistant::NDVideoManager::LoadVideoImage
		// Access:    public 
		// Returns:   bool
		// Qualifier:
		// Parameter: VideoData * pVideoData ����Ƶ��Ϣ
		//************************************
		bool LoadVideoImage(VideoData* pVideoData);

		//************************************
		// Method:    AddMyVideo ����ӱ���Ӱ�����ֻ�������èӰ��Ŀ¼
		// FullName:  ND91Assistant::NDVideoManager::AddMyVideo
		// Access:    public 
		// Returns:   VideoData*
		// Qualifier:
		// Parameter: wstring strLocalFullName �������ļ�������·����
		// Parameter: ProgressSubject & subject �����ȷ���SUBJECT
		//************************************
		VideoData* AddMyVideo(wstring strLocalFullName, ProgressObserver* pObserver = NULL);

		//************************************
		// Method:    AddPandaPlayerVideo ����ӱ���Ӱ������èӰ��Ŀ¼
		// FullName:  ND91Assistant::NDVideoManager::AddPandaPlayerVideo
		// Access:    public 
		// Returns:   VideoData*
		// Parameter: wstring strLocalFullName �������ļ�������·����
		// Parameter: ProgressSubject & subject �����ȷ���SUBJECT
		//************************************
		VideoData* AddPandaPlayerVideo(wstring strLocalFullName, ProgressObserver* pObserver = NULL);		

		//************************************
		// Method:    DeleteVideo ��ɾ���ֻ��ϵ�Ӱ��
		// FullName:  ND91Assistant::NDVideoManager::DeleteVideo
		// Access:    public 
		// Returns:   bool
		// Qualifier:
		// Parameter: VideoData * pVideoData ��Ӱ����Ϣ
		//************************************
		bool DeleteVideo(VideoData* pVideoData);

		//************************************
		// Method:    RenameVideo ���������ֻ��ϵ�Ӱ��
		// FullName:  ND91Assistant::NDVideoManager::RenameVideo
		// Access:    public 
		// Returns:   bool
		// Qualifier:
		// Parameter: VideoData * pVideoData ��Ӱ����Ϣ
		// Parameter: wstring strNewName ������
		//************************************
		bool RenameVideo(VideoData* pVideoData, wstring strNewName);

		//************************************
		// Method:    GetAlbumList ����ȡ���غ��Ӱ���б�
		// FullName:  ND91Assistant::NDVideoManager::GetAlbumList
		// Access:    public 
		// Returns:   deque<PlayAlbumData*>
		// Qualifier:
		//************************************
		const deque<PlayAlbumData*>* GetAlbumList();

		//************************************
		// Method:    GetMyVideoAlbumData ����ȡ����èӰ������Ƶר��
		// FullName:  ND91Assistant::NDVideoManager::GetMyVideoAlbumData
		// Access:    public 
		// Returns:   PlayAlbumData* ������ר������ָ��
		// Qualifier:
		//************************************
		PlayAlbumData* GetMyVideoAlbumData();

		//************************************
		// Method:    GetWithoutAlbumPandaVideoAlbumData ����ȡ��èӰ������ר����Ϣ��ר��
		// FullName:  ND91Assistant::NDVideoManager::GetWithoutAlbumPandaVideoAlbumData
		// Access:    public 
		// Returns:   PlayAlbumData* ������ר������ָ��
		// Qualifier:
		//************************************
		PlayAlbumData* GetWithoutAlbumPandaVideoAlbumData();

		//************************************
		// Method:    ParsePlayerAlbumPlist ������ר����Ϣ
		// FullName:  ND91Assistant::NDVideoManager::ParsePlayerAlbumPlist
		// Access:    public 
		// Returns:   bool
		// Qualifier:
		// Parameter: wstring strPlistFileName ��ר����Ϣ��PLIST�ļ�
		// Parameter: PlayAlbumData * pPlayAlbumData ��ר����Ϣָ�룬Ϊ��Ӧ֮��WEBЭ��ĸ��ģ��ʴ˶��������������ͷ�
		//************************************
		bool ParsePlayerAlbumPlist(wstring strPlistFileName, PlayAlbumData* pPlayAlbumData);

		//************************************
		// Method:    AddNetVideo �����WEB�����ص���Ƶ
		// FullName:  ND91Assistant::NDVideoManager::AddNetVideo
		// Access:    public 
		// Returns:   VideoData*
		// Qualifier:
		// Parameter: PlayerAlbumItem * pPlayerAlbumItem ����Ƶ�ڲ����б��е���Ϣ����������н�����������ͷſռ�
		// Parameter: PlayAlbumData * pPlayAlbumData ��ͨ��ParsePlayerAlbumPlist��ȡ��PlayAlbumData���������н�����PlayAlbumData
		// Parameter: ProgressSubject & subject �����ȷ���
		//************************************
		VideoData* AddNetVideo(PlayerAlbumItem* pPlayerAlbumItem, PlayAlbumData* pPlayAlbumData, ProgressObserver* pObserver = NULL);

		//************************************
		// Method:    DownLoadVideo ��������Ƶ�ļ�������
		// FullName:  ND91Assistant::NDVideoManager::DownLoadVideo
		// Access:    public 
		// Returns:   bool ���ɹ�������Ƶ�ļ��洢�ı����ļ�ȫ��,���򷵻�L""
		// Qualifier:
		// Parameter: VideoData * pVideoData ����Ƶ�ļ���Ϣ
		// Parameter: wstring strLocalSaveFullName ����Ƶ�ļ��洢�ı���·��
		//************************************
		wstring DownLoadVideo(VideoData* pVideoData, wstring strLocalSaveDir, ProgressObserver* pObserver = NULL);

		//************************************
		// Method:    DeletePlayAlbum ��ɾ��ר��
		// FullName:  ND91Assistant::NDVideoManager::DeletePlayAlbum
		// Access:    public 
		// Returns:   bool ���ɹ�����TRUE
		// Qualifier:
		// Parameter: PlayAlbumData * pPlayAlbumData ��ר����Ϣ
		//************************************
		bool DeletePlayAlbum(PlayAlbumData* pPlayAlbumData);

        //��ȡӰ����Ŀ¼
        wstring GetVideoRootPath();
	private:
		//��ӱ���Ӱ��
		VideoData* AddLocalVideo(wstring strLocalFullName, ePlayAlbumType eType, ProgressObserver* pObserver);
		//����ר�����ʹ���ר������
		PlayAlbumData* CreatePlayAlbumData(ePlayAlbumType eType);	
		//��մ洢��Ӱ����Ƶ��Ϣ
		void ClearList();
		//���û��ר����Ϣ����Ƶ�ļ���Ϣ
		void AddWithoutAlbumInfoVideo(deque<VideoData*> videoList, ePlayAlbumType eType);
		//����û��ר����Ϣ��ר����û���ҵ������Զ�����
		//PlayAlbumData* FindWithoutAlbumInfoPlayAlbumData(ePlayAlbumType eType);
		//���ר����Ϣ�б�
		void AddAlbumDatas(deque<PlayAlbumData*> vtPlayAlbumList);
		//������ר����Ϣ��ר��
		PlayAlbumData* FindWithAlbumInfoAlbumData(PlayAlbumData* pPlayAlbumData);
        void RemoveAlbumData(PlayAlbumData* pPlayAlbumData);

	private:
		IosVideoManager*        _pIosVideoManager; //IOSӰ��������
		AndroidVideoManager*    _pAndroidVideoManager; //ANDROIDӰ��������
		deque<PlayAlbumData*>   _playAlbumList;   // ��èӰ����Ƶר��
		PlayAlbumData*          _myVideos;
		PlayAlbumData*          _other91PlayerVideos;
		const DeviceData*		_pDevice; //�豸ָ��
		wstring					_strLocalAlbumPath; //ר��ͼƬ�洢·��
		wstring					_strLocalVideoPath; //��ƵͼƬ�洢·��
	};
}
