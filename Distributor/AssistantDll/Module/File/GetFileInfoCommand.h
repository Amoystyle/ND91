#pragma once

#include "Module/BaseCommand.h"

using namespace  std;

namespace ND91Assistant
{
    struct AndroidFileInfo
    {
        wstring     _path;
        wstring     _name;
        bool        _isDir;
        bool        _isLink;
        wstring     _linkTo;
        wstring     _power;
        wstring     _owner;
        wstring     _group;
        int64_t     _size;
        time_t      _date;
        bool        _hasSubFolders;
        bool        _canWrite;
        AndroidFileInfo()
        {
            _isDir          = false;
            _isLink         = false;
            _size           = 0;
            _date           = 0;
            _hasSubFolders  = false;
            _canWrite       = true;
        }
    };

	// ��ȡָ���豸�ϵ��ļ���Ϣ
    class GetFileInfoCommand :public BaseCommand
    {
    public:
	    GetFileInfoCommand(const DeviceData* pDevice);
	    virtual ~GetFileInfoCommand(void);

        void SetRemotePath(wstring remotePath) { _remotePath = remotePath; }
        AndroidFileInfo GetAndroidFileInfo() { return _fileInfo; }

	    // ִ������
	    virtual void Execute();

    protected:

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
        AndroidFileInfo _fileInfo;
        wstring         _remotePath; // �豸�ϵ��ļ�·��

    };

}
