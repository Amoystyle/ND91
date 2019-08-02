#pragma once

namespace ND91Assistant
{
    class IBaseCommand : public ProgressSubject
    {
    public:
        IBaseCommand(const DeviceData* pDevice);
        ~IBaseCommand();

        // ִ������
        virtual void Execute() = 0;

        // ȡ���豸��Ϣ
        const DeviceInfo* GetDeviceInfo();

        // �ļ��Ѵ��ڣ�ѯ���û���δ���
        EXIST_ACTION AskUser(std::wstring destName, bool isDir = false);
        void SetRenameCopyAllAction() { _lastAction = RENAME_COPY_ALL; }

        // ȡIO��ָ�루���Ƕ���ҪIO�࣬��Ҫʱ�ٴ�����
        BaseIO* GetIO();

        CommandExecuteResult Result() { return _result; };

    protected:
        const DeviceData* _pDevice;
        CommandExecuteResult _result;
        BaseIO*	_pBaseIO;		// IO�࣬������ͨ��GetIO()��ȡ
        EXIST_ACTION    _lastAction;    // �ļ��Ѵ���ʱ���û��ϴ�ѡ��Ķ���
    };
}