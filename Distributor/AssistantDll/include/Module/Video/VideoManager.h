#pragma once

#include "Module/Video/VideoData.h"
#include "Module/Video/PlayAlbumData.h"

namespace ND91Assistant
{
	//��èӰ��������
	class CLASSINDLL_CLASS_DECL VideoManager
	{
	public:
		VideoManager(const DeviceData* pDevice);
		virtual ~VideoManager(void);

        //��ʼ������
		virtual eVideoInitCode InitEnvironment() = 0;

        //��������Ӱ���б�ÿ�ε��ö������֮ǰ������
        virtual void Load() = 0;

        virtual int GetVideoCount();

        // ��ȡ�豸֧�ֵ���Ƶ����
        virtual wstring GetSupportVideoType() = 0;

        // ��ȡר�������ͼ
        virtual bool LoadAlbumImage(PlayAlbumData* pPlayAlbumData) = 0;

        // ��ȡ��ƵԤ��ͼ
        virtual bool LoadVideoImage(VideoData* pVideoData) = 0;

        // ��ӱ���Ӱ�����ֻ�������èӰ��Ŀ¼
        virtual VideoData* AddMyVideo(wstring strLocalFullName, ProgressObserver* pObserver = NULL) = 0;

        // ��ӱ���Ӱ������èӰ��Ŀ¼
        virtual VideoData* AddPandaPlayerVideo(wstring strLocalFullName, ProgressObserver* pObserver = NULL) = 0;

        // ���WEB�����ص���Ƶ
        virtual VideoData* AddNetVideo(PlayerAlbumItem* pPlayerAlbumItem, PlayAlbumData* pPlayAlbumData, ProgressObserver* pObserver = NULL) = 0;

        // ������Ƶ�ļ�������
        virtual wstring DownLoadVideo(VideoData* pVideoData, wstring strLocalSaveDir, ProgressObserver* pObserver = NULL) = 0;

        //��ȡӰ����Ŀ¼
        virtual wstring GetVideoRootPath() = 0;

        // �ͷ��Ѽ��ص�����
        void Release();

        // ɾ��ר��
        bool DeletePlayAlbum(PlayAlbumData* pPlayAlbumData);

        // ɾ���ֻ��ϵ�Ӱ��
		bool DeleteVideo(VideoData* pVideoData);

        // �������ֻ��ϵ�Ӱ��
		bool RenameVideo(VideoData* pVideoData, wstring strNewName);

        // ��ȡ���غ��Ӱ���б�
		const deque<PlayAlbumData*>* GetAlbumList();

        // ��ȡ����èӰ������Ƶר��
		PlayAlbumData* GetMyVideoAlbumData();

		// ��ȡ��èӰ������ר����Ϣ��ר��
		PlayAlbumData* GetWithoutAlbumPandaVideoAlbumData();

		// ����ר����Ϣ
		bool ParsePlayerAlbumPlist(wstring strPlistFileName, PlayAlbumData* pPlayAlbumData);

    protected:
        //���û��ר����Ϣ����Ƶ�ļ���Ϣ
        void AddWithoutAlbumInfoVideo(deque<VideoData*> videoList, ePlayAlbumType eType);

        //���ר����Ϣ�б�
        void AddAlbumDatas(deque<PlayAlbumData*> vtPlayAlbumList);

        // ���Net��Ƶ AddNetVideo ����
        void CheckNetVideo(PlayerAlbumItem* pPlayerAlbumItem, PlayAlbumData* pPlayAlbumData, VideoData* pVideoData, wstring strAlbumImageremotePath);

        virtual bool DeletePlayAlbum_Action(PlayAlbumData* pPlayAlbumData) = 0;
        virtual bool DeleteVideo_Action(VideoData* pVideoData) = 0;
        virtual bool RenameVideo_Action(VideoData* pVideoData, wstring strNewName) = 0;

        //����ר�����ʹ���ר������
        PlayAlbumData* CreatePlayAlbumData(ePlayAlbumType eType);	

		//��ӱ���Ӱ��
		VideoData* AddLocalVideo(wstring strLocalFullName, ePlayAlbumType eType, ProgressObserver* pObserver);

		//����û��ר����Ϣ��ר����û���ҵ������Զ�����
		//PlayAlbumData* FindWithoutAlbumInfoPlayAlbumData(ePlayAlbumType eType);
		//������ר����Ϣ��ר��
		PlayAlbumData* FindWithAlbumInfoAlbumData(PlayAlbumData* pPlayAlbumData);
        void RemoveAlbumData(PlayAlbumData* pPlayAlbumData);

	protected:
        const DeviceData*		_pDevice;               //�豸ָ��
        deque<PlayAlbumData*>   _playAlbumList;         //��èӰ����Ƶר��
        PlayAlbumData*          _myVideos;              //�ҵ���Ƶ��������èӰ������ģ�
        PlayAlbumData*          _other91PlayerVideos;   //������èӰ����Ƶ(û��ר������)

	    wstring					_strLocalAlbumPath;     //ר��ͼƬ�洢·��
		wstring					_strLocalVideoPath;     //��ƵͼƬ�洢·��
	};
}
