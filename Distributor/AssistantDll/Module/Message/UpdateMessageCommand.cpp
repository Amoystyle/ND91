#include "pch_module.h"

#ifdef MODULE_MESSAGE

#include "Module/Message/UpdateMessageCommand.h"
#include "Module/Message/MessageData.h"
#include "Module/Message/IosMessageDB.h"

#include "Module/Common/IosDB/IosDBPath.h"
#include "Module/AndroidDefine.h"

UpdateMessageCommand::UpdateMessageCommand(const DeviceData* pDevice)
: BaseCommand(pDevice)
{
    _success           = false;
    _dataList          = NULL;
    _pMessageData      = NULL;

    _nAndroidChannelID = PIM;
    _nAndroidCommandID = UPDATE_SMS;
    _executeCount      = 0;
}

UpdateMessageCommand::~UpdateMessageCommand()
{
    _pMessageData = NULL;
}

// �ο���PimProvider::DeleteSmsItems
std::string UpdateMessageCommand::GetAndroidBodyData()
{
    BinaryBufferWriter  writer;
    writer.Clear();
    writer.WriteInt(_nAndroidChannelID);
    writer.WriteInt(_nAndroidCommandID);    // ��������
    
    // д��Ҫ�޸ĵĶ��ţ�ÿ��ֻ���޸�һ��
    writer.WriteInt(_pMessageData->GetId());
    writer.WriteString(_pMessageData->GetStrNumber());
    writer.WriteTime(_pMessageData->GetTime());
    writer.WriteInt(_pMessageData->GetCategory());
    writer.WriteStringAsUTF8(_pMessageData->GetText());
    writer.WriteBool(_pMessageData->GetRead());

    return writer.GetBuffer();
}

// ���淵�����ݰ�
void UpdateMessageCommand::SetAndroidReturnData(std::string data)
{
    BinaryBufferReader reader(data.c_str(), data.length());
    if (!parseAndroidPackageHead(&reader))
    {
        _success = false;
        return;
    }
    reader.ReadBool(_success);
}

bool UpdateMessageCommand::Success()
{
    return _success;
}

void UpdateMessageCommand::SetDataList( const std::vector<const MessageData*>* DataList )
{
    _dataList = DataList;
}

void UpdateMessageCommand::Execute()
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

bool UpdateMessageCommand::AndroidExecute()
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

bool UpdateMessageCommand::IosExecute()
{
    // û�������豸�򷵻�
    if (_pDevice->GetDevInfo() == NULL || !_pDevice->GetDevInfo()->_ios_bAFCConn)
        return false;

    // �޸����ݿ��ļ�
    IosDBPath dbPath(_pDevice,ISMESSAGEDB);
    IosMessageDB messageDB(dbPath);
    messageDB.OpenDB();
    messageDB.BeginTransaction();
    try
    {
        _executeCount = messageDB.UpdateMessageByContent(_dataList,(ProgressSubject*)this);
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

    return false;
}


#endif