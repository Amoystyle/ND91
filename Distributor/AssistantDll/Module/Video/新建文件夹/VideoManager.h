#pragma once
#define MODULE_VIDEO

namespace ND91Assistant
{
    class DeviceData;
    class VideoData;
    class DirData;
    class PlayAlbumData;
    class PlayerAlbumItem;
    
    // ��Ƶ������
    class CLASSINDLL_CLASS_DECL VideoManager
    {
    public:
        VideoManager(const DeviceData* pDevice);
        virtual ~VideoManager();

        // Summary  :��������
        // Returns	: void
        // Parameter: �����Ƿ�ȡ�ļ���Сѡ���Ϊȥ�ļ���С�ܺ�ʱ��,�ļ���С��ȡ
        void Load();

        // Summary  : ����Ƿ���Ҫ��װ��èӰ��
        // Returns	: bool
        bool IsNeedInstall91Player();

        // Summary  : ��ȡ��Ƶ����

        void GetVideoImages(const deque<VideoData*>* videoList);

		// Summary  : ���ר��ͼ
		// Returns	: std::wstring
		// Parameter: PlayAlbumData * pAlbum
		wstring GetAlbumImage(PlayAlbumData* pAlbum);

        // Summary  : ����ҵ���Ƶ
        // Returns	: void
        // Parameter: const deque<wstring> localVideoPaths
        void AddMyVideos(const deque<wstring> localVideoPaths, ProgressObserver* pObserver = NULL);

        // Summary  : �����èӰ����Ƶ
        // Returns	: void
        // Parameter: const deque<wstring> localVideoPaths
        void Add91PlayerVideos(const deque<wstring> localVideoPaths, ProgressObserver* pObserver = NULL);


		// Summary  : �����������ص�ר��plist�ļ�
		// Returns	: PlayAlbumData*
		// Parameter: wstring localPlistXml
		PlayAlbumData* ParseAlbumPlist( wstring localPlistXml);

        // Summary  : ���������վ��������������Ƶ
        // Returns	: void
        // Parameter: PlayerAlbumItem * albumItem ר�������б����url�����õ�
        // Parameter: wstring localPlistXml ����������ר����Ϣ�����ļ�
        void Add91PlayerAlbumVideo(PlayerAlbumItem* albumItem, PlayAlbumData* albumData, ProgressObserver* pObserver = NULL);

        // Summary  : ɾ����Ƶ
        // Returns	: void
        // Parameter: const deque<VideoData * > * videoList
        void DeleteVideo(const deque<VideoData*>* videoList);

        // Summary  : �ҵ���Ƶ
        // Returns	: const deque<VideoData*>
        const deque<VideoData*>* GetMyVideoList(){return &_pMyVideoList;}

        // Summary  : 91Ӱ����Ƶ
        // Returns	: const deque<VideoData*>
        const deque<VideoData*>* Get91PlayerVideoList(){return &_p91playerVideoList;}

        // Summary  : 91Ӱ��ר���б�
        // Returns	: const deque<PlayAlbumData*>
        const deque<PlayAlbumData*>* Get91PlayerAlbumList(){return &_playAlbumList;}

		VideoData* Find91PlayerVideoByRemotePath(wstring remotePath);

        // �ͷ�
        void  Release();

    private:

        // Summary  :����ȫ����Ƶ
        // Returns	: void
        void LoadAllVideos();

        // Summary  : ������èӰ��ר��
        // Returns	: void
        void Load91PlayerAlbum();

        // Summary  : ������èӰ����Ƶ��ר��
        // Returns	: void
        void Init91PlayerAlbum();

        // Summary  : ������èӰ��ר�����ݹ���
        // Returns	: void
        // Parameter: DirData * pDir
        void Load91PlayerAlbum(DirData* pDir);

        // Summary  : ���������ͼƬ
        // Returns	: bool
        // Parameter: const wstring localVideoPath��Ƶ����·��
        // Parameter: wstring remotePath�����ֻ��˵�·�����������ļ���
        bool AddVideo( const wstring localVideoPath, wstring remotePath, deque<VideoData*>& toList, ProgressObserver* pObserver = NULL );

        // Summary  : ������Ƶ���б�
        // Returns	: bool ������ͬ·����
        // Parameter: VideoData * video 
        // Parameter: deque<VideoData * > & ToListҪ������б�
        // Parameter: bool del �Ƿ�ɾ����ͬ��
        bool InsertOneVideoToList(VideoData* video, deque<VideoData*>& ToList, bool del=true);

        // Summary  : ���б����Ƴ���Ƶ
        // Returns	: void
        // Parameter: VideoData * video
        // Parameter: deque<VideoData * >* fromList
        bool  RemoveVideoFromList(VideoData* video, deque<VideoData*>& fromList);

        // Summary  : ͨ��ID�ҵ�ר��
        // Returns	: PlayAlbumData*
        // Parameter: wstring id
        PlayAlbumData* FindPlayAlbumById(wstring id);

        // Summary  : ����ר��ͼƬ���ϴ����ֻ�
        // Returns	: void
        // Parameter: wstring localSavePath���ش洢·��
        // Parameter: wstring url ���ص�ַurl
        // Parameter: wstring toUploadRemotePath Ҫ�ϴ������ֻ���·��
        void DownLoadUrlImage(wstring localSavePath,  wstring  url, wstring toUploadRemotePath);


        // Summary  : ���sd��·��
        // Returns	: std::wstring
        wstring GetSdcardPath();

		wstring GetAlbumRootPath();

    private:
        deque<VideoData*>   _pMyVideoList;       // �ֻ�����Ƶ

        deque<VideoData*>   _p91playerVideoList; // ��èӰ����Ƶ

        deque<PlayAlbumData*>  _playAlbumList;   // ��èӰ����Ƶר��

        const DeviceData*   _pDevice;

        wstring             _sdcardPath;         // SD��·��
        bool                _noSdcard;
    };

}