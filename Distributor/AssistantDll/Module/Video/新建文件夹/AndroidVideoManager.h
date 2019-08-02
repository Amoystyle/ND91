#pragma once
namespace ND91Assistant
{
    // ����ԭVideoManager��NDVideoManager��������������Android��Ƶ������
    class AndroidVideoManager
    {
    public:
        AndroidVideoManager(const DeviceData* pDevice);
        virtual ~AndroidVideoManager();

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
        wstring GetSupportVideoType(){ return L"*.mp4;*.m4v;*.wmv;*.3gp;*.ndv"; }


		//************************************
		// Method:    LoadMyVideoList �����ط���èӰ������Ƶ
		// FullName:  ND91Assistant::IosVideoManager::LoadMyVideoList
		// Access:    public 
		// Returns:   void
		// Qualifier:
		// Parameter: deque<VideoData * > & vtMyVideoList ���洢Ӱ����Ϣ�б�
		//************************************
        void Load(deque<VideoData*>& vtMyVideoList, 
                  deque<VideoData*>& vt91PlayerWithoutAlbumVideoList,
                  deque<PlayAlbumData*>& vtPlayAlbumList);

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
		// Qualifier:
		// Parameter: wstring strLocalFullName �������ļ�������·����
		// Parameter: ProgressSubject & subject �����ȷ���SUBJECT
		//************************************
		VideoData* AddPandaPlayerVideo(wstring strLocalFullName, ProgressObserver* pObserver = NULL);		
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
		VideoData* AddNetVideo(PlayerAlbumItem* pPlayerAlbumItem, 
            PlayAlbumData* pPlayAlbumData, 
            deque<PlayAlbumData*>& vtPlayAlbumList,
            ProgressObserver* pObserver = NULL);

		//************************************
		// Method:    DeleteVideo ��ɾ���ֻ��ϵ�Ӱ��
		// FullName:  ND91Assistant::NDVideoManager::DeleteVideo
		// Access:    public 
		// Returns:   bool
		// Qualifier:
		// Parameter: VideoData * pVideoData ��Ӱ����Ϣ
		//************************************
		bool DeleteVideo(VideoData* pVideoData, deque<PlayAlbumData*>& vtAlbumDataList);

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
		// Method:    DownLoadVideo ��������Ƶ�ļ�������
		// FullName:  ND91Assistant::NDVideoManager::DownLoadVideo
		// Access:    public 
		// Returns:   bool ���ɹ�����TRUE
		// Qualifier:
		// Parameter: VideoData * pVideoData ����Ƶ�ļ���Ϣ
		// Parameter: wstring strLocalSaveFullName ����Ƶ�ļ��洢�ı����ļ�ȫ��
		//************************************
		wstring DownLoadVideo(VideoData* pVideoData, wstring destDir, ProgressObserver* pObserver = NULL);

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
    protected:
        // Summary  : ���sd��·��
        // Returns	: std::wstring
        wstring GetSdcardPath();
        // Summary  : ������Ƶ���б�
        // Returns	: bool ������ͬ·����
        // Parameter: VideoData * video 
        // Parameter: deque<VideoData * > & ToListҪ������б�
        // Parameter: bool del �Ƿ�ɾ����ͬ��
        bool InsertOneVideoToList(VideoData* video, deque<VideoData*>& vtList, bool del=true);
        // Summary  : ������èӰ��ר��
        // Returns	: void
        void Load91PlayerAlbum(deque<PlayAlbumData*>& vtAlbumList);
        // Summary  : ������èӰ��ר�����ݹ���
        // Returns	: void
        // Parameter: DirData * pDir
        void Load91PlayerAlbum(deque<PlayAlbumData*>& vtAlbumList, DirData* pDir);
        // Summary  : ���������ͼƬ
        // Returns	: bool
        // Parameter: const wstring localVideoPath��Ƶ����·��
        // Parameter: wstring remotePath�����ֻ��˵�·�����������ļ���
        VideoData* AddVideo( const wstring localVideoPath, wstring remotePath, ProgressObserver* pObserver = NULL);

        PlayAlbumData* InsertAlbumData(PlayAlbumData* pPlayAlbumData, deque<PlayAlbumData*>& vtPlayAlbumList);
        PlayerAlbumItem* InsertAlbumItem(PlayAlbumData* pPlayAlbumData, PlayerAlbumItem* pPlayAlbumItem);
        PlayAlbumData* FindAlbumData(wstring strAlbumID, deque<PlayAlbumData*>& vtPlayAlbumList);
        

        wstring GetAlbumRootPath();
    private:
        const DeviceData* _pDevice;		// �豸��Ϣָ��
        wstring           _sdcardPath;  // SD��·��
    };
}