#pragma once

#include "AdbRemoteCommand.h"
#include "Monitor/ProgressSubject.h"
#include "Module/AndroidDefine.h"

using namespace std;
namespace ND91Assistant
{
    //下载前获取的手机端文件属性
    struct  TFileState
    {
        int _mReadMode;//可读模式
        int _mSize;    //文件大小
        int _mModified;//可修改
    };

    class AdbDownloadFileCommand : public AdbRemoteCommand
    {
    public:
        AdbDownloadFileCommand(wstring sn,wstring id, ProgressSubject* pSubject = NULL);

        void SetParam(wstring strPCPath, wstring strPhonePath);

        virtual ~AdbDownloadFileCommand(void);

        // 执行命令
        // 子类实现时，若无特殊处理只需调用CommonExecute即可

        TransferResult GetDownloadResult(){return _success;}

    protected:

        // 解析返回数据
        // 返回值：true 表示成功，false 表示失败
        virtual bool ParseReturnData();

    private:
        virtual void InternalExecute();

        // 生成获取远程文件属性命令数据包
        string CreateFileStateReqPkg(const string& afilepath);

        // 生成接收远程文件数据请求包
        string CreateReceiveRegPkg();

        // 解析远程文件属性
        TFileState ParseFileStateResult(const string& result);


        wstring _FileToPc;     // PC端文件
        wstring _FilefromPhone;// 手机端文件

        TransferResult   _success;

        ProgressSubject* _pSubject;

        NullProgressSubject _nullSubject;
    };
}
