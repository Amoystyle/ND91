#include "pch_module.h"

#ifdef MODULE_MESSAGE

#include "Module/Message/SetMessageReadCommand.h"
#include "Module/Message/MessageData.h"
#include "Module/Message/IosMessageDB.h"

#include "Module/Common/IosDB/IosDBPath.h"
#include "Device/NoDeamonInterface/NoDeamonInterface.h"
#include "Module/AndroidDefine.h"

SetMessageReadCommand::SetMessageReadCommand(const DeviceData* pDevice)
: BaseCommand(pDevice)
{
    _success           = false;
    _dataList          = NULL;
    _pMessageData      = NULL;
    
    _nAndroidChannelID = PIM;
    _nAndroidCommandID = SETREAD_SMS;
    _executeCount      = 0;
}

SetMessageReadCommand::~SetMessageReadCommand()
{
    _pMessageData = NULL;
}

// �ο���PimProvider::DeleteSmsItems
std::string SetMessageReadCommand::GetAndroidBodyData()
{
    BinaryBufferWriter  writer;
    writer.Clear();
    writer.WriteInt(_nAndroidChannelID);
    writer.WriteInt(_nAndroidCommandID);    // ��������

    // д��Ҫ�޸ĵĶ��ţ�ÿ��ֻ���޸�һ��
    writer.WriteInt(_pMessageData->GetId());
    writer.WriteBool(!_pMessageData->GetRead());

    return writer.GetBuffer();
}

// ���淵�����ݰ�
void SetMessageReadCommand::SetAndroidReturnData(std::string data)
{
    BinaryBufferReader reader(data.c_str(), data.length());
    if (!parseAndroidPackageHead(&reader))
    {
        _success = false;
        return;
    }
    reader.ReadBool(_success);
}

bool SetMessageReadCommand::Success()
{
    return _success;
}

void SetMessageReadCommand::SetDataList( const vector<const MessageData*>* DataList )
{
    _dataList = DataList;
}

void SetMessageReadCommand::Execute()
{
    if ( _dataList == NULL )
    {
        _success = true;
        Complete();
        return;
    }

    // ����������������
    SetAllTaskSum(_dataList->size());
    // ������������
    SetTaskNumberSum(_dataList->size());

    if ( _pDevice->GetDevInfo()->_deviceType == Android )
        _success = AndroidExecute();
    else if ( _pDevice->GetDevInfo()->_deviceType == Ios )
        _success = IosExecute();
}

bool SetMessageReadCommand::AndroidExecute()
{
    // �ж��û��Ƿ�ȡ������ȡ������������
    ProgressObserver* pObserver = NULL;
    if (!_observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(_observers[0]);

    size_t i = 0;
    for (i = 0; i < _dataList->size(); i++)
    {
        IF_CANCEL_BREAK(pObserver)

        _pMessageData = _dataList->at(i);
        DefaultExecute();
        RETURN_FALSE_IF(!_success)

        IncreaseTaskNumberComplete();
        SendProgress();

        SetCurrentTaskSum(1);
        SetCurrentTaskName(CCodeOperation::UTF_8ToUnicode(_pMessageData->GetStrNumber()));

        AddTaskProgress(1);
        SendProgress();
    }
    _executeCount = (int)i;

    return true;
}

bool SetMessageReadCommand::IosExecute()
{
//     if (_pDevice->GetDevInfo()->_connectType == USB 
//         && _pDevice->GetDevInfo()->_ios_bJailBreaked)
//         return IosNoDeamonExecut();
//     else
        return IosDeamonExecut();

    return false;
}

bool SetMessageReadCommand::IosDeamonExecut()
{
    bool result = false;

    // û�������豸�򷵻�
    if (_pDevice->GetDevInfo() == NULL || !_pDevice->GetDevInfo()->_ios_bAFCConn)
        return result;

    // �޸����ݿ��ļ�
    IosDBPath dbPath(_pDevice,ISMESSAGEDB);
    IosMessageDB messageDB(dbPath);
    messageDB.OpenDB();
    messageDB.BeginTransaction();
    try
    {
        _executeCount = messageDB.UpdateMessageByIsRead(_dataList,(ProgressSubject*)this);
        messageDB.Commit();
        messageDB.CloseDB();
        return messageDB.SyncToIos(_pDevice);// ͬ����IOS�豸
    }
    catch (CppSQLite3Exception& e)
    {
        int iError = e.ErrorCode();
        const char* szError = e.ErrorMessage();
        messageDB.Rollback();
    }
    messageDB.CloseDB();

    return result;
}

bool SetMessageReadCommand::IosNoDeamonExecut()
{
    // �ж��û��Ƿ�ȡ������ȡ������������
    ProgressObserver* pObserver = NULL;
    if (!_observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(_observers[0]);

    size_t i = 0;
    for (i = 0; i < _dataList->size(); i++)
    {
        IF_CANCEL_BREAK(pObserver)

        _pMessageData = _dataList->at(i);
        int flag = (_pMessageData->GetRead() == true ? 0 : 2);
        _success = (NoDeamonInterface::SetSmsFlag(_pDevice->GetDevInfo()->_ios_devptr, 
                                                  (wchar_t*)CStrOperation::IntToWString(_pMessageData->GetId()).c_str(), 
                                                  (wchar_t*)CStrOperation::IntToWString(flag).c_str()) 
                                                  == 0);
        RETURN_FALSE_IF(!_success);

        IncreaseTaskNumberComplete();
        SendProgress();

        SetCurrentTaskSum(1);
        SetCurrentTaskName(CStrOperation::IntToWString(_pMessageData->GetId()));

        AddTaskProgress(1);
        SendProgress();
    }
    _executeCount = (int)i;

    return true;
}


#endif