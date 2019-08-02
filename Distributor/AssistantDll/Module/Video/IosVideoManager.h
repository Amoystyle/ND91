#pragma once

#include "Module/Video/VideoManager.h"

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
	class IosVideoManager : public VideoManager
	{
	public:
		IosVideoManager(const DeviceData* pDevice);
		virtual ~IosVideoManager(void);

        //��ʼ������
        virtual eVideoInitCode InitEnvironment();

        //��������Ӱ���б�ÿ�ε��ö������֮ǰ������
        virtual void Load();

        // ��ȡ�豸֧�ֵ���Ƶ����
        wstring GetSupportVideoType();

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
        virtual wstring DownLoadVideo(VideoData* pVideoData, wstring strLocalSaveDir, ProgressObserver* pObserver = NULL);

        //��ȡӰ����Ŀ¼
        virtual wstring GetVideoRootPath();
    
    protected:
        virtual bool DeletePlayAlbum_Action(PlayAlbumData* pPlayAlbumData);
        virtual bool DeleteVideo_Action(VideoData* pVideoData);
        virtual bool RenameVideo_Action(VideoData* pVideoData, wstring strNewName);
    private:
		// ���ط���èӰ������Ƶ
		void LoadMyVideoList(deque<VideoData*>& vtMyVideoList);

		// ������èӰ����û��ר����Ϣ����Ƶ
        void Load91PlayerVideoWithoutAlbum(deque<VideoData*>& vtVideoList);

		// ������èӰ������ר����Ϣ����Ƶ
		void Load91PlayerAlumData(deque<PlayAlbumData*>& vtPlayAlbumList);

        // ��ӱ�����Ƶ�ļ�
        wstring AddMyVideo(wstring strLocalFullName, ePlayAlbumType eType, ProgressObserver* pObserver = NULL);

        // �����ר����Ϣ����èӰ���ļ�
        wstring AddPandaVideo(wstring strLocalVideoFullName, wstring strVideoNameOnPhone, wstring strLocalAlbumImageFullName, 
                                wstring strAlbumName, ProgressObserver* pObserver = NULL);
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
		eNDPlayerType _ePlayerType;		// ��èӰ�����ͣ�IPA����PXL
		IIosFileSystem* _pFileStream;	// �ļ�����ָ��
	};
}
