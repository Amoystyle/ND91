#pragma once
#include "Module/BaseData.h"
#include <deque>
using namespace std;

#pragma warning(disable : 4251)

namespace ND91Assistant
{
	typedef enum eVideoInitCode
	{
		eVideoInitCode_Success, //�ɹ�
		eVideoInitCode_UnConnected, //�ֻ�δ����
		eVideoInitCode_Ios_UnJailBreaked, //δԽ��
		eVideoInitCode_UnInstall_91Player, // δװ��èӰ��
		eVideoInitCode_Cannot_Use_With_Wifi, //WIFI�����޷�ʹ��
		eVideoInitCode_Error, //��������
	};

    class DeviceData;
    class VideoData;

    // ��Ƶר����
    class CLASSINDLL_CLASS_DECL PlayerAlbumItem:public BaseData
    {
    public:
        PlayerAlbumItem();
        virtual ~PlayerAlbumItem();

        PlayerAlbumItem(const PlayerAlbumItem* albumItem);

        //int      _videoId;           // ��Ӧ��ƵId
        VideoData*  _pVideo;           // ��Ӧ��Ƶ

        wstring		_fileId;           // �ļ�id, idΪ�մ����������ݾ���Ч
        wstring		_fileDisplayName;  // �ļ��ڲ����б�����ʾ������
        wstring		_fileUrl;          // ��ͼurl·��
        wstring		_filePath;         // �ļ����ֻ���·��
        __int64		_fileSize;         // �ļ���С
        int			_fileOrder;        // �ļ�����
        wstring		_fileType;         // �ļ�����
        wstring		_downUrl;          // web��ַurl·��
        wstring		_fileLocalPath;    // �ļ����ص���·��

		wstring     _urlImageLocalPath;

        wstring		_albumId;          // ר��id,��url���
        wstring		_albumName;        // ר����,��url���

	private:
		void InitParams();
    };

	typedef enum ePlayAlbumType
	{
		ePlayAlbumType_91Player_With_Album, //��ר����Ϣ��91Ӱ��ר��
		ePlayAlbumType_91Player_Without_Album, //��ר����Ϣ��91Ӱ��ר��
		ePlayAlbumType_NO_91Player, //��91Ӱ����ר��
	};

    // ��Ƶר��
    class CLASSINDLL_CLASS_DECL PlayAlbumData :public BaseData
    {
    public:
        PlayAlbumData(void);
        virtual ~PlayAlbumData(void);
		PlayAlbumData(PlayAlbumData* pAlbum);
		PlayAlbumData(const DeviceData* pDevice, wstring path);

        void UpdateInfo(PlayAlbumData* pAlbum);
        // Summary  : ����·���ҵ�ר����
        // Returns	: PlayerAlbumItem*
        // Parameter: wstring remotepath
        PlayerAlbumItem* FindItemByPath(wstring  remotepath);

        // Summary  : ����·���Ƴ�ר����
        // Returns	: void
        // Parameter: wstring remotepath
        void RemoveItemFromListByPath(wstring  remotepath);

        // Summary  : ר�����޸ģ���д��xml�ļ�
        // Returns	: void
        // Parameter: const DeviceData * pDevice
        void ReWritePlayAlbumXml(const DeviceData* pDevice);

		ePlayAlbumType _ePlayAlbumType;

		wstring GetAlbumPath();

        wstring   _albumId;     // ר��id
        wstring   _albumName;   // ר����
        wstring	  _count;		// �ܹ�����
        wstring	  _updata;      // �������ڼ���
        int       _type;        // ���ͣ�1��Ӱ��2���Ӿ磬3���ս�Ŀ��6
        wstring   _showingTime; // ��ӳʱ��
        wstring   _director;	// ����
        wstring   _actor;		// ��Ա
        wstring   _introduce;	// ר�����
        wstring   _albumUrl;    // ר��ͼƬurl

        wstring   _xmlremotePath;//ר��xml�ļ�
		wstring   _albumImageremotePath;//ר��ͼƬȫ�� �豸

		wstring   _AlbumImagelocalSavePath; //ר��ͼƬȫ�� PC

        deque<PlayerAlbumItem*>  _playItemList; //ר���б���

	private:
		void InitParams();

	private:
		void Init();
		bool IsValidVideo(wstring strName);
		DirData* _pdir;
    };
}
