#pragma once

#include "RingData.h"
#include <deque>
using namespace std;

namespace ND91Assistant
{
	enum RingGroupType
	{
		CALL_RING = 1, // 来电铃声
		SMS_RING  =2,  // 短信铃声
		ALARM_RING =4 // 闹钟铃声
	};

class DirData;
class DeviceData;
class AndroidRingData;
class CLASSINDLL_CLASS_DECL RingDirData : public BaseData
{
public:
	RingDirData(const DeviceData* pDevice, RingGroupType type, wstring  fullpath);

	virtual ~RingDirData(void);

	// Summary  : 添加铃声
	// Returns  : 返回新增的铃声数据指针。若重名则添加失败、返回false
	// Parameter: const RingData * pRing 要添加的铃声数据
	RingData* Insert(const RingData* pRing);

	// Summary  : 添加铃声，还原用
	// Returns	: RingData* 成功添加的铃声
	// Parameter: wstring localPath PC端路径
	// Parameter: wstring remotePath 原始手机端路径
	// Parameter: int mediaKind 原始铃声类型
	RingData* ImportRing(wstring localPath, ProgressObserver* pObserver=NULL, wstring remotePath = L"", 
							int mediaKind = RingData::Null, wstring title = L"" );

    // Summary  : 添加铃声
    // Returns  : int 实际添加的铃声数量
    // Parameter: RingData::MediaFileKind kind 铃声类型
    // Parameter: deque<wstring> localringPaths 铃声文件在本地的路径集合
    int AddRings(deque<wstring> localringPaths, ProgressObserver* pObserver = NULL, RingData::MediaFileKind kind = RingData::Null);

    // 停止播放
	void StopRing();

    // 移除铃声
	bool RemoveRing(RingData* pRing); 

	// 下载铃声
	bool DownLoadRing(RingData* pRing, wstring destPath);

	
	enum TypeReturnRings
	{
		RETURN_ALL,   // 返回全部
		RETURN_SYSTEM,// 返回系统铃声
		RETURN_CUSTOM // 返回自定义铃声
	};

    // 获得其下所有铃声
	deque<RingData*> GetRings(TypeReturnRings type);

	deque<AndroidRingData*> GetAndroidRings(TypeReturnRings type);

    // 标题
	RingGroupType GetRingGroupType(){return _type;}

	void SetCurrentRing(wstring ringUri);

	void SetCurrentAndroidRing(int ringId, bool isSystem);

	// 获取android当前来电铃声标题
	RingData* GetAndroidCurrentRing(){return _pCurRing;}

	// 获取ios当前来电铃声标题
	wstring	  GetIosCurrentRing(){ return _iosCurrentRingTitle;}	

private:

	RingData* FindRing(wstring remotePath, wstring title);

    // 检查本地文件是否为手机支持的媒体类型
	MediaFileType IsSurpportedMediaFormat(wstring path);

	RingGroupType _type;

	DirData*  _pDir;

	RingData*  _pCurRing;

	wstring	   _iosCurrentRingTitle;     // ios当前来电铃声名字
};

}
