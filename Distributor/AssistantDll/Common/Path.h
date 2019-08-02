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
        wstring _workFolder;        // ��ʱ����Ŀ¼

    };

    // ��ʱ����Ŀ¼����
    // ��ʱ����Ŀ¼��������: %TEMP%\Assistant_���̺�_�豸��
    // ԭ��֧�ֶ���̣�ÿ��������Ҫһ��������Ŀ¼����Ϊÿ�������˳�ʱ��ɾ����ʱĿ¼��
    //        ֧�ֶ��豸��ÿ���豸��Ҫһ��������Ŀ¼
    // ��ע�⡿����������������·����, ����ȡ·���ķ���, ����ֵȫ����'\'��'/'��β
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

#pragma region ĳЩģ��ʹ�õ�Ŀ¼��һ����, �����ⶨ����, ֱ���ڴ��ṩ

        wstring GetLocalIconPath() const;           // ͼ��
        wstring GetLocalIpaPath() const;            // ipa��װ
        wstring GetLocalAppPath() const;            // Ӧ��
        wstring GetLocalAlbumPath() const;          // ר������
        wstring GetLocalAudioPath() const;          // ��Ƶ
        wstring GetLocalPimPath() const;            // ��ϵ��
        wstring GetLocalPimPhotoPath() const;       // ��ϵ��ͷ��
        wstring GetLocalSmsPath() const;            // ����
        wstring GetLocalCallDataPath() const;       // ͨ����¼
        wstring GetLocalVideoPath() const;          // ��Ƶ�ļ�
        wstring GetLocalVideoImagePath() const;     // ��Ƶ����
        wstring GetLocalScreensnapPath() const;     // ����ͼƬ
		

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

        // IOS�ϻ�ȡ�Ľ����������ļ�����ȫ·����
        static  wstring IosScreensnapFile()
        {
            return L"/tmp/foo_0.png";
        }

        // // IOS�豸�϶����ļ���ŵ�Ŀ¼
        static wstring IosMessageFolder()
        {
            return L"/private/var/root/Media/ndDaemon/sms/";
        }

        static wstring GlobalTemp();
        static wstring GlobalTempFile();
        static wstring GetRandomName();
    private:

        wstring				_workFolder;        // ��ʱ����Ŀ¼
        const DeviceInfo*	_pInfo;				// �豸��Ϣ
		int					_version;

		IosRecordingPath*	_pIosRecordingPath;
        IosPicturePath*		_pIosPicturePath;
        IosRingPath*		_pIosRingPath;
        AndroidPath*		_pAndroidPath;
        ThemePath*			_pThemePath;
    };
}
