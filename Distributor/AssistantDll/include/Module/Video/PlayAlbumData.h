#pragma once
#include "Module/BaseData.h"
#include <deque>
using namespace std;

#pragma warning(disable : 4251)

namespace ND91Assistant
{
	typedef enum eVideoInitCode
	{
		eVideoInitCode_Success, //成功
		eVideoInitCode_UnConnected, //手机未连接
		eVideoInitCode_Ios_UnJailBreaked, //未越狱
		eVideoInitCode_UnInstall_91Player, // 未装熊猫影音
		eVideoInitCode_Cannot_Use_With_Wifi, //WIFI连接无法使用
		eVideoInitCode_Error, //其他错误
	};

    class DeviceData;
    class VideoData;

    // 视频专辑项
    class CLASSINDLL_CLASS_DECL PlayerAlbumItem:public BaseData
    {
    public:
        PlayerAlbumItem();
        virtual ~PlayerAlbumItem();

        PlayerAlbumItem(const PlayerAlbumItem* albumItem);

        //int      _videoId;           // 对应视频Id
        VideoData*  _pVideo;           // 对应视频

        wstring		_fileId;           // 文件id, id为空代表下面数据均无效
        wstring		_fileDisplayName;  // 文件在播放列表中显示的名字
        wstring		_fileUrl;          // 截图url路径
        wstring		_filePath;         // 文件在手机端路径
        __int64		_fileSize;         // 文件大小
        int			_fileOrder;        // 文件排序
        wstring		_fileType;         // 文件类型
        wstring		_downUrl;          // web地址url路径
        wstring		_fileLocalPath;    // 文件下载到端路径

		wstring     _urlImageLocalPath;

        wstring		_albumId;          // 专辑id,从url获得
        wstring		_albumName;        // 专辑名,从url获得

	private:
		void InitParams();
    };

	typedef enum ePlayAlbumType
	{
		ePlayAlbumType_91Player_With_Album, //有专辑信息的91影音专辑
		ePlayAlbumType_91Player_Without_Album, //无专辑信息的91影音专辑
		ePlayAlbumType_NO_91Player, //非91影音的专辑
	};

    // 视频专辑
    class CLASSINDLL_CLASS_DECL PlayAlbumData :public BaseData
    {
    public:
        PlayAlbumData(void);
        virtual ~PlayAlbumData(void);
		PlayAlbumData(PlayAlbumData* pAlbum);
		PlayAlbumData(const DeviceData* pDevice, wstring path);

        void UpdateInfo(PlayAlbumData* pAlbum);
        // Summary  : 根据路径找到专辑项
        // Returns	: PlayerAlbumItem*
        // Parameter: wstring remotepath
        PlayerAlbumItem* FindItemByPath(wstring  remotepath);

        // Summary  : 根据路径移除专辑项
        // Returns	: void
        // Parameter: wstring remotepath
        void RemoveItemFromListByPath(wstring  remotepath);

        // Summary  : 专辑有修改，重写回xml文件
        // Returns	: void
        // Parameter: const DeviceData * pDevice
        void ReWritePlayAlbumXml(const DeviceData* pDevice);

		ePlayAlbumType _ePlayAlbumType;

		wstring GetAlbumPath();

        wstring   _albumId;     // 专辑id
        wstring   _albumName;   // 专辑名
        wstring	  _count;		// 总共集数
        wstring	  _updata;      // 更新至第几集
        int       _type;        // 类型，1电影，2电视剧，3综艺节目，6
        wstring   _showingTime; // 上映时间
        wstring   _director;	// 导演
        wstring   _actor;		// 演员
        wstring   _introduce;	// 专辑简介
        wstring   _albumUrl;    // 专辑图片url

        wstring   _xmlremotePath;//专辑xml文件
		wstring   _albumImageremotePath;//专辑图片全名 设备

		wstring   _AlbumImagelocalSavePath; //专辑图片全名 PC

        deque<PlayerAlbumItem*>  _playItemList; //专辑列表项

	private:
		void InitParams();

	private:
		void Init();
		bool IsValidVideo(wstring strName);
		DirData* _pdir;
    };
}
