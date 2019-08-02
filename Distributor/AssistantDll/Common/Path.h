#pragma once

#include <string>
#include "Common/IosPicturePath.h"
#include "Common/IosRingPath.h"
#include "Common/ThemePath.h"
#include "Common/IosRecordingPath.h"

#ifdef WIN32
    #define CHAR_PATH_SEPARATER '\\'
    #define WCHAR_PATH_SEPARATER L'\\'
    #define STRING_PATH_SEPARATER "\\"
    #define WSTRING_PATH_SEPARATER L"\\"
#else
    #define CHAR_PATH_SEPARATER '/'
    #define WCHAR_PATH_SEPARATER L'/'
    #define STRING_PATH_SEPARATER "/"
    #define WSTRING_PATH_SEPARATER L"/"
#endif

using namespace std;

namespace ND91Assistant
{
	class DeviceInfo;

    class AndroidPath
    {
    public:
        AndroidPath(wstring workFolder) : _workFolder(workFolder) {}

        wstring GetPicturePath() const { //return _workFolder + L"Android Picture\\"; 
                                           return _workFolder + L"Android Picture" + WSTRING_PATH_SEPARATER;}
		wstring GetWallPaperPath() const { //return _workFolder + L"Android wallpaper\\"; 
                                           return _workFolder + L"Android wallpaper" + WSTRING_PATH_SEPARATER;}

    private:
        wstring _workFolder;        // 临时工作目录

    };

    // 临时工作目录基类
    // 临时工作目录命名规则: %TEMP%\Assistant_进程号_设备号
    // 原因：支持多进程，每个进程需要一个独立的目录（因为每个进程退出时将删除临时目录）
    //        支持多设备，每个设备需要一个独立的目录
    // 【注意】本类和其中所有相关路径类, 所有取路径的方法, 返回值全部以'\'或'/'结尾
    class Path
    {
    public:
        Path(const DeviceInfo* pInfo);
        ~Path();

        wstring GetWorkFolder() const { return _workFolder; }

		const IosRecordingPath* GetPIosRecordingPath	() const { return _pIosRecordingPath;	}
        const IosPicturePath*   GetPIosPicturePath		() const { return _pIosPicturePath;		}
        const IosRingPath*      GetPIosRingPath			() const { return _pIosRingPath;		}
        const AndroidPath*      GetAndroidPath			() const { return _pAndroidPath;		}
        const ThemePath*        GetPThemePath			() const { return _pThemePath;			}

#pragma region 某些模块使用的目录仅一两个, 不另外定义类, 直接在此提供

        wstring GetLocalIconPath() const;           // 图标
        wstring GetLocalIpaPath() const;            // ipa安装
        wstring GetLocalAppPath() const;            // 应用
        wstring GetLocalAlbumPath() const;          // 专辑封面
        wstring GetLocalAudioPath() const;          // 音频
        wstring GetLocalPimPath() const;            // 联系人
        wstring GetLocalPimPhotoPath() const;       // 联系人头像
        wstring GetLocalSmsPath() const;            // 短信
        wstring GetLocalCallDataPath() const;       // 通话记录
        wstring GetLocalVideoPath() const;          // 视频文件
        wstring GetLocalVideoImagePath() const;     // 视频封面
        wstring GetLocalScreensnapPath() const;     // 截屏图片
		

		wstring GetIosClockPlistPath() const;
		wstring IosCheckPathByVersion(wstring path) const;

        wstring GetLocalDeamonFolder() const 
        {
            return _workFolder + L"DaemonFolderTemp";
        }
#pragma endregion

        static wstring IosDeviceCommandPath()
        {
            return L"/private/var/root/Media/ndDaemon/cmd/";
        }

        static wstring DeviceDeamonPath()
        {
            //return L"/usr/bin/TQServer";
            return L"/var/root/Library/NetDragon/ndDaemon";
        }

        static wstring IosSbShutdownCookie()
        { return L"/var/mobile/Library/Caches/SBShutdownCookie"; }

        // IOS上获取的截屏命令结果文件名（全路径）
        static  wstring IosScreensnapFile()
        {
            return L"/tmp/foo_0.png";
        }

        // // IOS设备上短信文件存放的目录
        static wstring IosMessageFolder()
        {
            return L"/private/var/root/Media/ndDaemon/sms/";
        }

        static wstring GlobalTemp();
        static wstring GlobalTempFile();
        static wstring GetRandomName();
    private:

        wstring				_workFolder;        // 临时工作目录
        const DeviceInfo*	_pInfo;				// 设备信息
		int					_version;

		IosRecordingPath*	_pIosRecordingPath;
        IosPicturePath*		_pIosPicturePath;
        IosRingPath*		_pIosRingPath;
        AndroidPath*		_pAndroidPath;
        ThemePath*			_pThemePath;
    };
}
