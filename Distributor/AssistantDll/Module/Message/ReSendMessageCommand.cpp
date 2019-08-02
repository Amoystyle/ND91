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

    // 写入要发送的短信，每次只能发送一条
    writer.WriteInt(_curMessageId);

    return writer.GetBuffer();
}

// 保存返回数据包
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

    // 设置所有任务总量
    SetAllTaskSum(_delmsgIds->size());
    // 设置任务总数
    SetTaskNumberSum(_delmsgIds->size());

    if ( _pDevice->GetDevInfo()->_deviceType == Android )
        _success = AndroidExecute();
    else if ( _pDevice->GetDevInfo()->_deviceType == Ios )
        _success = IosExecute();
}

bool ReSendMessageCommand::AndroidExecute()
{
    // 判断用户是否取消，若取消则立即返回
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
    // 没连接上设备则返回
    if (_pDevice->GetDevInfo() == NULL || !_pDevice->GetDevInfo()->_ios_bAFCConn)
        return false;

    IosDBPath dbPath(_pDevice,ISMESSAGEDB);
    IosMessageDB messageDB(dbPath);
    // 加载数据库
    messageDB.Reload(_pDevice);
    // 打开数据库
    messageDB.OpenDB();
    // 获取发送失败的短信数据
	// 需要重发的短信息
	vector<MessageData*> dataList;
    messageDB.GetMessageSendFail(&dataList,_startTime,_endTime);
    // 删除发送失败的数据
	for(size_t i = 0; i < dataList.size(); i++)
	{
		messageDB.DeleteMessage(dataList[i]);
	}
    // 关闭数据库
    messageDB.Close();
    messageDB.SyncToIos(_pDevice);// 同步到IOS设备

	// 判断用户是否取消，若取消则立即返回
	ProgressObserver* pObserver = NULL;
	if (!_observers.empty())
		pObserver = dynamic_cast<ProgressObserver*>(_observers[0]);

	// 设置所有任务总量
	SetAllTaskSum(dataList.size());
	// 设置任务总数
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