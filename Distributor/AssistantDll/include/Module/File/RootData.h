#pragma once

#include "AssistantDll.h"
#include "Module/BaseData.h"
#include "Module/File/NodeData.h"
#include "Core/DeviceInfo.h"

#include <string>
#include <deque>
#include <vector>
using namespace std;

namespace ND91Assistant
{
	struct SpaceInfo
	{
		wstring     _path;
		int64_t    _totalsize; //��λKB
		int64_t    _usedsize;
		int64_t    _Available;
        int64_t    _applicationSize;     //����ռ�ÿռ�
        // ���±�ʾΪϵͳ�ؼ���Ϣ��Ϊ�������λ�ȡ�˷�ʱ�䣬���Լ�������
        int64_t    _systemSize;         //ϵͳ�ռ�
        int64_t    _systemAvailable;    //ϵͳ���ÿռ�
        int64_t    _systemUsed;         //ϵͳ���ÿռ�

        int64_t    _nAmountDataReserved;
        int64_t    _nAmountDataAvailable;
        SpaceInfo()
        {
            _totalsize = 0; //��λ
            _usedsize = 0;
            _Available = 0;
            _applicationSize = 0; 
            _systemSize = 0;      
            _systemAvailable = 0; 
            _systemUsed = 0;      
            _nAmountDataReserved = 0;
            _nAmountDataAvailable = 0;
        };
	};

    class DirData;
    class DeviceData;

    // ��Ŀ¼��Ϣ�ࣺ�ɽ�����һ��Ŀ¼�趨Ϊ��Ŀ¼
    class CLASSINDLL_CLASS_DECL RootData : private BaseData
    {
    public:

		enum ROOT_TYPE
		{
			ROOT,			//��Ŀ¼
			PHONEDATA,		//�û�Ŀ¼
            SYSTEM,         //ϵͳ
			SDCARD,			//�洢��Ŀ¼
			DOCUMENT,		//�ĵ�
			PHOTO,			//��ƬĿ¼
			WALLPAPER,		//ǽֽĿ¼
			RINGRONES,		//����Ŀ¼
			MUSIC,			//����Ŀ¼
			PANDAREADER,	//��è����
			UNKNOWN,		//δ֪
		};
    public:
        // parameter: DeviceData* pDevice   �豸ָ��
        // parameter: std::wstring caption  ����
        // parameter: std::wstring path     ·��
        RootData(const DeviceData* pDevice);

        // �ļ�����ʱ��/��/sdcard����ʱ���ô˹��캯������/sdcard
        // parameter: std::wstring caption  ����
        // parameter: DirData * pDir        ������Ŀ¼������ָ��
        // RootData(DirData* pDir);
        virtual ~RootData();

		DirData* GetDir() { return _pDir; };

		// ��ȡ�ض�Ŀ¼��DirData
		DirData* GetPath(ROOT_TYPE nType, DirData* pSDCardDir = NULL, bool reLoad = false);	
		
		// ��ȡ�ռ���Ϣ
		bool GetSpaceInfo(ROOT_TYPE nType, SpaceInfo* pInfo);

        // Summary  : ��ȡָ��·�������ļ�ϵͳ�Ŀռ���Ϣ������׿��
        // Returns  : �Ƿ�ɹ�
        // Parameter: wstring path     ָ��·��
        // Parameter: int64_t& used       ���ÿռ䣨�����������λ B��
        // Parameter: int64_t& available  ���ÿռ䣨�����������λ B��
        bool GetAndroidSpaceInfo(wstring path, int64_t& used, int64_t& available);

        // ��ȡָ��·����Ŀ¼
        DirData* GetPath(wstring path, DirData* pParentDir);

        // ͳ��ָ��Ŀ¼��ָ����չ���ļ����ܿռ�ռ��
        int GetFolderTotalSize(std::vector<wstring> paths, wstring ExtName = L"");

        // ������Ƶ,��Ƶ,���,ͼ��,iBook�ռ�ռ��
        int64_t GetIosAudioUsage();
        int64_t GetIosVideoUsage();
        int64_t GetIosCamaraUsage();
        int64_t GetIosPhotoUsage();
        int64_t GetIosBookUsage();

    private:

		// ��ȡ��è�������鼮��·��
		wstring	GetPandaReaderPath();

		// Summary  : ��ȡ��ͬ�汾��è������installation.plist��Ӧ�ļ�ֵ
		// Returns  : std::wstring
		// Parameter: const DeviceInfo * pInfo
		void GetRealKey( const DeviceInfo* pInfo, vector<string>& verKeys );  

        DirData*        _pDir;          // Ŀ¼���ݣ������洢FileData

		const DeviceData* _pDeviceData;

    };
}