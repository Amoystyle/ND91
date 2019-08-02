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

	// 获取指定设备上的文件信息
    class GetFileInfoCommand :public BaseCommand
    {
    public:
	    GetFileInfoCommand(const DeviceData* pDevice);
	    virtual ~GetFileInfoCommand(void);

        void SetRemotePath(wstring remotePath) { _remotePath = remotePath; }
        AndroidFileInfo GetAndroidFileInfo() { return _fileInfo; }

	    // 执行命令
	    virtual void Execute();

    protected:

    #pragma region 这6个方法是Command的主要内容，处理令格式与结果格式

	    // 生成Android数据体，在GetAndroidDatagram()方法中调用本方法
	    virtual std::string GetAndroidBodyData();

	    // 解析返回数据包
	    virtual void SetAndroidReturnData(std::string data);

	    // 命令传送到IOS设备上的文件名（全路径, unicode）
	    virtual std::wstring GetIosCommandFilePath(){return L"";}

	    // 从IOS上获取的命令结果文件名（全路径, unicode）
	    virtual std::wstring GetIosResultFilePath() {return L"";}

	    // 生成ios文件。返回文件名（全路径, unicode），调用者负责删除文件
	    virtual std::wstring GetIosCommandFileContent(){return L"";}

	    // 解析ios设备的返回文件（全路径, unicode）并删除文件
	    virtual void SetIosReturnFile(std::wstring filename) { }

    private:
        AndroidFileInfo _fileInfo;
        wstring         _remotePath; // 设备上的文件路径

    };

}
