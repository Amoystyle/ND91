#include "pch_module.h"
#include "IBaseCommand.h"

IBaseCommand::IBaseCommand(const DeviceData* pDevice)
{
    _pDevice = pDevice;
    _pBaseIO = NULL;
    _result  = COMMAND_EXECUTE_UNSUPPORT;
    _lastAction = NONE;
}

IBaseCommand::~IBaseCommand()
{
    SAFE_DELETE( _pBaseIO );
}

const DeviceInfo* ND91Assistant::IBaseCommand::GetDeviceInfo()
{
    return _pDevice->GetDevInfo();
}

ND91Assistant::EXIST_ACTION ND91Assistant::IBaseCommand::AskUser( std::wstring destName, bool isDir /*= false*/ )
{
    if ( RENAME_ALL == _lastAction || OVERWRITE_ALL == _lastAction || RENAME_COPY_ALL == _lastAction)
    {
        return _lastAction;
    }

    // �޷�ȡ�ý�����Ӧ��ȡ����ǰ�ļ�

    // �����Ŀ¼���򸲸�
    RETURN_VALUE_IF ( _observers.empty() && isDir, OVERWRITE);
    RETURN_VALUE_IF ( _observers.empty(), CANCEL );

    ProgressObserver* pObserver = dynamic_cast<ProgressObserver*>(_observers[0]);
    RETURN_VALUE_IF ( ! pObserver, CANCEL );

    // �ļ��Ѵ��ڣ�ѯ�ʽ�����δ���
    _lastAction = pObserver->GetExistAction(destName);

    return _lastAction;
}

BaseIO* ND91Assistant::IBaseCommand::GetIO()
{
    if ( ! _pBaseIO )
        _pBaseIO = _pDevice->CreateIO();

    return _pBaseIO; 
}
