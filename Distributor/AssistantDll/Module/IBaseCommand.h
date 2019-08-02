#pragma once

namespace ND91Assistant
{
    class IBaseCommand : public ProgressSubject
    {
    public:
        IBaseCommand(const DeviceData* pDevice);
        ~IBaseCommand();

        // 执行命令
        virtual void Execute() = 0;

        // 取得设备信息
        const DeviceInfo* GetDeviceInfo();

        // 文件已存在，询问用户如何处理
        EXIST_ACTION AskUser(std::wstring destName, bool isDir = false);
        void SetRenameCopyAllAction() { _lastAction = RENAME_COPY_ALL; }

        // 取IO类指针（并非都需要IO类，需要时再创建）
        BaseIO* GetIO();

        CommandExecuteResult Result() { return _result; };

    protected:
        const DeviceData* _pDevice;
        CommandExecuteResult _result;
        BaseIO*	_pBaseIO;		// IO类，子类须通过GetIO()获取
        EXIST_ACTION    _lastAction;    // 文件已存在时，用户上次选择的动作
    };
}