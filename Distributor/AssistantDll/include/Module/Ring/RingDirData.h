#pragma once

#include "RingData.h"
#include <deque>
using namespace std;

namespace ND91Assistant
{
	enum RingGroupType
	{
		CALL_RING = 1, // ��������
		SMS_RING  =2,  // ��������
		ALARM_RING =4 // ��������
	};

class DirData;
class DeviceData;
class AndroidRingData;
class CLASSINDLL_CLASS_DECL RingDirData : public BaseData
{
public:
	RingDirData(const DeviceData* pDevice, RingGroupType type, wstring  fullpath);

	virtual ~RingDirData(void);

	// Summary  : �������
	// Returns  : ������������������ָ�롣�����������ʧ�ܡ�����false
	// Parameter: const RingData * pRing Ҫ��ӵ���������
	RingData* Insert(const RingData* pRing);

	// Summary  : �����������ԭ��
	// Returns	: RingData* �ɹ���ӵ�����
	// Parameter: wstring localPath PC��·��
	// Parameter: wstring remotePath ԭʼ�ֻ���·��
	// Parameter: int mediaKind ԭʼ��������
	RingData* ImportRing(wstring localPath, ProgressObserver* pObserver=NULL, wstring remotePath = L"", 
							int mediaKind = RingData::Null, wstring title = L"" );

    // Summary  : �������
    // Returns  : int ʵ����ӵ���������
    // Parameter: RingData::MediaFileKind kind ��������
    // Parameter: deque<wstring> localringPaths �����ļ��ڱ��ص�·������
    int AddRings(deque<wstring> localringPaths, ProgressObserver* pObserver = NULL, RingData::MediaFileKind kind = RingData::Null);

    // ֹͣ����
	void StopRing();

    // �Ƴ�����
	bool RemoveRing(RingData* pRing); 

	// ��������
	bool DownLoadRing(RingData* pRing, wstring destPath);

	
	enum TypeReturnRings
	{
		RETURN_ALL,   // ����ȫ��
		RETURN_SYSTEM,// ����ϵͳ����
		RETURN_CUSTOM // �����Զ�������
	};

    // ���������������
	deque<RingData*> GetRings(TypeReturnRings type);

	deque<AndroidRingData*> GetAndroidRings(TypeReturnRings type);

    // ����
	RingGroupType GetRingGroupType(){return _type;}

	void SetCurrentRing(wstring ringUri);

	void SetCurrentAndroidRing(int ringId, bool isSystem);

	// ��ȡandroid��ǰ������������
	RingData* GetAndroidCurrentRing(){return _pCurRing;}

	// ��ȡios��ǰ������������
	wstring	  GetIosCurrentRing(){ return _iosCurrentRingTitle;}	

private:

	RingData* FindRing(wstring remotePath, wstring title);

    // ��鱾���ļ��Ƿ�Ϊ�ֻ�֧�ֵ�ý������
	MediaFileType IsSurpportedMediaFormat(wstring path);

	RingGroupType _type;

	DirData*  _pDir;

	RingData*  _pCurRing;

	wstring	   _iosCurrentRingTitle;     // ios��ǰ������������
};

}
