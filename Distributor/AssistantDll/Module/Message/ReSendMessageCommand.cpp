#include "pch_module.h"

#ifdef MODULE_MESSAGE

#include "Module/Message/ReSendMessageCommand.h"
#include "Module/Message/IosMessageDB.h"
#include "Module/Message/SendMessageCommand.h"
#include "Module/Message/MessageData.h"

#include "Module/Common/IosDB/IosDBPath.h"
#include "Module/AndroidDefine.h"

ReSendMessageCommand::ReSendMessageCommand(const DeviceData* pDevice,time_t startTime,time_t endTime)
: BaseCommand(pDevice)
{
    _success           = false;
    _startTime         = startTime;
    _endTime           = endTime;

    _nAndroidChannelID = PIM;
    _nAndroidCommandID = RESEND_SMS;
	_delmsgIds         = NULL;
	_curMessageId      = 0;
}

ReSendMessageCommand::~ReSendMessageCommand()
{
}

std::string ReSendMessageCommand::GetAndroidBodyData()
{
    BinaryBufferWriter writer;
    writer.Clear();
    writer.WriteInt(_nAndroidChannelID);
    writer.WriteInt(_nAndroidCommandID);

    // д��Ҫ���͵Ķ��ţ�ÿ��ֻ�ܷ���һ��
    writer.WriteInt(_curMessageId);

    return writer.GetBuffer();
}

// ���淵�����ݰ�
void ReSendMessageCommand::SetAndroidReturnData(std::string data)
{
    BinaryBufferReader reader(data.c_str(), data.length());
    if (!parseAndroidPackageHead(&reader))
    {
        _success = false;
        return;
    }

    reader.ReadBool(_success);
}

bool ReSendMessageCommand::Success()
{
    return _success;
}


void ReSendMessageCommand::SetDataList(vector<int>* delmsgIds)
{
    _delmsgIds = delmsgIds;
}

void ReSendMessageCommand::Execute()
{
    if ( _delmsgIds->size()<1)
    {
        _success = true;
        Complete();
        return;
    }

    // ����������������
    SetAllTaskSum(_delmsgIds->size());
    // ������������
    SetTaskNumberSum(_delmsgIds->size());

    if ( _pDevice->GetDevInfo()->_deviceType == Android )
        _success = AndroidExecute();
    else if ( _pDevice->GetDevInfo()->_deviceType == Ios )
        _success = IosExecute();
}

bool ReSendMessageCommand::AndroidExecute()
{
    // �ж��û��Ƿ�ȡ������ȡ������������
    ProgressObserver* pObserver = NULL;
    if (!_observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(_observers[0]);

    for (size_t i = 0; i < _delmsgIds->size(); i++)
    {
        IF_CANCEL_BREAK(pObserver)
        _curMessageId = _delmsgIds->at(i);
        DefaultExecute();
        RETURN_FALSE_IF(!_success)

        IncreaseTaskNumberComplete();
        SendProgress();

        SetCurrentTaskSum(1);
		SetCurrentTaskName(CStrOperation::IntToWString(_curMessageId));

        AddTaskProgress(1);
        SendProgress();
    }
    return true;
}

bool ReSendMessageCommand::IosExecute()
{
    // û�������豸�򷵻�
    if (_pDevice->GetDevInfo() == NULL || !_pDevice->GetDevInfo()->_ios_bAFCConn)
        return false;

    IosDBPath dbPath(_pDevice,ISMESSAGEDB);
    IosMessageDB messageDB(dbPath);
    // �������ݿ�
    messageDB.Reload(_pDevice);
    // �����ݿ�
    messageDB.OpenDB();
    // ��ȡ����ʧ�ܵĶ�������
	// ��Ҫ�ط��Ķ���Ϣ
	vector<MessageData*> dataList;
    messageDB.GetMessageSendFail(&dataList,_startTime,_endTime);
    // ɾ������ʧ�ܵ�����
	for(size_t i = 0; i < dataList.size(); i++)
	{
		messageDB.DeleteMessage(dataList[i]);
	}
    // �ر����ݿ�
    messageDB.Close();
    messageDB.SyncToIos(_pDevice);// ͬ����IOS�豸

	// �ж��û��Ƿ�ȡ������ȡ������������
	ProgressObserver* pObserver = NULL;
	if (!_observers.empty())
		pObserver = dynamic_cast<ProgressObserver*>(_observers[0]);

	// ����������������
	SetAllTaskSum(dataList.size());
	// ������������
	SetTaskNumberSum(dataList.size());

    SendMessageCommand cmd(_pDevice);
    cmd.Attach(pObserver);
	for (size_t i =0; i < dataList.size(); i++)
	{
		vector<MessageData*> tmplist;
		tmplist.push_back(dataList[i]);
		cmd.SetDataList(&tmplist);
		cmd.Execute();

		IncreaseTaskNumberComplete();
		SendProgress();
		SetCurrentTaskSum(1);
		SetCurrentTaskName(CCodeOperation::UTF_8ToUnicode(dataList[i]->GetStrNumber()));

		AddTaskProgress(1);
		SendProgress();
	}
	for(size_t i = 0; i < dataList.size(); i++)
	{
		SAFE_DELETE(dataList[i]);
	}
	dataList.clear();
    return cmd.Success();
}


#endif