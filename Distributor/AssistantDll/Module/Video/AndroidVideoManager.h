#pragma once

#include "Module/Video/VideoManager.h"

namespace ND91Assistant
{
    // ����ԭVideoManager��������������Android��Ƶ������
    class AndroidVideoManager : public VideoManager
    {
    public:
        AndroidVideoManager(const DeviceData* pDevice);
        virtual ~AndroidVideoManager();

        //��ʼ������
        virtual eVideoInitCode InitEnvironment();

        //��������Ӱ���б�ÿ�ε��ö������֮ǰ������
        virtual void Load();

        // ��ȡ�豸֧�ֵ���Ƶ����
        virtual wstring GetSupportVideoType();

        // ��ȡר�������ͼ
        virtual bool LoadAlbumImage(PlayAlbumData* pPlayAlbumData);

        // ��ȡ��ƵԤ��ͼ
        virtual bool LoadVideoImage(VideoData* pVideoData);

        // ��ӱ���Ӱ�����ֻ�������èӰ��Ŀ¼
        virtual VideoData* AddMyVideo(wstring strLocalFullName, ProgressObserver* pObserver = NULL);

        // ��ӱ���Ӱ������èӰ��Ŀ¼
        virtual VideoData* AddPandaPlayerVideo(wstring strLocalFullName, ProgressObserver* pObserver = NULL);		

        // ���WEB�����ص���Ƶ
        virtual VideoData* AddNetVideo(PlayerAlbumItem* pPlayerAlbumItem, PlayAlbumData* pPlayAlbumData, ProgressObserver* pObserver = NULL);

		// ������Ƶ�ļ�������
		virtual wstring DownLoadVideo(VideoData* pVideoData, wstring destDir, ProgressObserver* pObserver = NULL);

        //��ȡӰ����Ŀ¼
        virtual wstring GetVideoRootPath();

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
        wstring           _sdcardPath;  // SD��·��


    protected:
        virtual bool DeletePlayAlbum_Action(PlayAlbumData* pPlayAlbumData);
        virtual bool DeleteVideo_Action(VideoData* pVideoData);
        virtual bool RenameVideo_Action(VideoData* pVideoData, wstring strNewName);
    };
}