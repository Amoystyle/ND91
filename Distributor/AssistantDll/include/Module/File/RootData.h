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
		int64_t    _totalsize; //单位KB
		int64_t    _usedsize;
		int64_t    _Available;
        int64_t    _applicationSize;     //程序占用空间
        // 以下表示为系统控件信息，为避免两次获取浪费时间，所以加在这里
        int64_t    _systemSize;         //系统空间
        int64_t    _systemAvailable;    //系统可用空间
        int64_t    _systemUsed;         //系统已用空间

        int64_t    _nAmountDataReserved;
        int64_t    _nAmountDataAvailable;
        SpaceInfo()
        {
            _totalsize = 0; //单位
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

    // 根目录信息类：可将任意一个目录设定为根目录
    class CLASSINDLL_CLASS_DECL RootData : private BaseData
    {
    public:

		enum ROOT_TYPE
		{
			ROOT,			//根目录
			PHONEDATA,		//用户目录
            SYSTEM,         //系统
			SDCARD,			//存储卡目录
			DOCUMENT,		//文档
			PHOTO,			//照片目录
			WALLPAPER,		//墙纸目录
			RINGRONES,		//铃声目录
			MUSIC,			//音乐目录
			PANDAREADER,	//熊猫看书
			UNKNOWN,		//未知
		};
    public:
        // parameter: DeviceData* pDevice   设备指针
        // parameter: std::wstring caption  标题
        // parameter: std::wstring path     路径
        RootData(const DeviceData* pDevice);

        // 文件管理时，/和/sdcard并存时，用此构造函数构造/sdcard
        // parameter: std::wstring caption  标题
        // parameter: DirData * pDir        从其他目录树来的指针
        // RootData(DirData* pDir);
        virtual ~RootData();

		DirData* GetDir() { return _pDir; };

		// 获取特定目录的DirData
		DirData* GetPath(ROOT_TYPE nType, DirData* pSDCardDir = NULL, bool reLoad = false);	
		
		// 获取空间信息
		bool GetSpaceInfo(ROOT_TYPE nType, SpaceInfo* pInfo);

        // Summary  : 获取指定路径所在文件系统的空间信息（仅安卓）
        // Returns  : 是否成功
        // Parameter: wstring path     指定路径
        // Parameter: int64_t& used       已用空间（输出参数，单位 B）
        // Parameter: int64_t& available  可用空间（输出参数，单位 B）
        bool GetAndroidSpaceInfo(wstring path, int64_t& used, int64_t& available);

        // 获取指定路径的目录
        DirData* GetPath(wstring path, DirData* pParentDir);

        // 统计指定目录中指定扩展名文件的总空间占用
        int GetFolderTotalSize(std::vector<wstring> paths, wstring ExtName = L"");

        // 计算音频,视频,相机,图库,iBook空间占用
        int64_t GetIosAudioUsage();
        int64_t GetIosVideoUsage();
        int64_t GetIosCamaraUsage();
        int64_t GetIosPhotoUsage();
        int64_t GetIosBookUsage();

    private:

		// 获取熊猫看书存放书籍的路径
		wstring	GetPandaReaderPath();

		// Summary  : 获取不同版本熊猫看书在installation.plist对应的键值
		// Returns  : std::wstring
		// Parameter: const DeviceInfo * pInfo
		void GetRealKey( const DeviceInfo* pInfo, vector<string>& verKeys );  

        DirData*        _pDir;          // 目录数据，用来存储FileData

		const DeviceData* _pDeviceData;

    };
}