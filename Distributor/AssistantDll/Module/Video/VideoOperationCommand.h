 #pragma once
#include "Module/BaseCommand.h"
#include <deque>

using namespace std;
namespace ND91Assistant
{

class VideoData;
// ����Ƶ������һЩ����
class VideoOperationCommand :public BaseCommand
{
public:
	VideoOperationCommand(const DeviceData* pDevice);
	virtual ~VideoOperationCommand(void);

     // Summary  :�����Ƶ
     // Returns	: VideoData*
     // Parameter: wstring localpath����·��
     // Parameter: wstring remotepath�ֻ���·��
     VideoData* AddVideo(wstring localpath, wstring remotepath);

	 // Summary  :�Ƴ���Ƶ
	 // Returns	: bool
	 // Parameter: VideoData * pVideo
	 bool  RemoveVideo(VideoData* pVideo);

	 // Summary  :��������Ƶ
	 // Returns	: bool
	 // Parameter: VideoData * pVideo
	 // Parameter: wstring newName
	 bool  RenameVideo(VideoData* pVideo, wstring newName);

	 // Summary  :�����Ƶ����ͼƬ
	 // Returns	: void
	 // Parameter: const deque<VideoData * > * pVideoList
	 void  GetVideoImages(const deque<VideoData*>* pVideoList);

	 // Summary  :��ÿռ��С
	 // Returns	: __int64
	 __int64 GetTotalSize();

protected:

	// ִ������
	virtual void Execute();

#pragma region ��6��������Command����Ҫ���ݣ��������ʽ������ʽ

	// ����Android�����壬��GetAndroidDatagram()�����е��ñ�����
	virtual std::string GetAndroidBodyData();

	// �����������ݰ�
	virtual void SetAndroidReturnData(std::string data);

	// ����͵�IOS�豸�ϵ��ļ�����ȫ·��, unicode��
	virtual std::wstring GetIosCommandFilePath(){return L"";}

	// ��IOS�ϻ�ȡ���������ļ�����ȫ·��, unicode��
	virtual std::wstring GetIosResultFilePath() {return L"";}

	// ����ios�ļ��������ļ�����ȫ·��, unicode���������߸���ɾ���ļ�
	virtual std::wstring GetIosCommandFileContent(){return L"";}

	// ����ios�豸�ķ����ļ���ȫ·��, unicode����ɾ���ļ�
	virtual void SetIosReturnFile(std::wstring filename) { }

    
private:

	wstring GetVideoMime(wstring localpath, int& duration);
    

	enum VideoOperateType
	{ 
		VIDEO_ADD = 11,          // �����Ƶ
		VIDEO_REMOVE = 12,       // �Ƴ���Ƶ
		VIDEO_RENAME = 13,       // ������
		VIDEO_GET_IMAGE = 101,   // ���ר������
		VIDEO_GET_TOTAL_SIZE = 24// ��ÿռ��С
	};

    VideoOperateType   _type;		// ��������

    VideoData*         _pVideo;		//����������������Ƶ
    wstring            _newName;	//�����������������������

    wstring            _localpath;	//�����������Ƶʱ�ı���·��
    wstring            _remotePath;	//�����������Ƶʱ���ϴ����ֻ��˵�·��

	const deque<VideoData*>* _pVideoList;//��������ȡ��Ƶ���洫�����Ƶ�б�


    VideoData*         _pReturnVideo;// ����ֵ����ӵ���Ƶ
    __int64            _size;        // ����ֵ���ռ��С
    bool               _success;     // ����ֵ���ɹ�ʧ��
};

}
