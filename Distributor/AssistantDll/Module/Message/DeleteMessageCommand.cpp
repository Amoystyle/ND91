#include "pch_module.h"

#ifdef MODULE_MESSAGE

#include "Module/Message/DeleteMessageCommand.h"
#include "Module/Message/MessageData.h"
#include "Module/Message/IosMessageDB.h"

#include "Module/Common/IosDB/IosDBPath.h"
#include "Module/AndroidDefine.h"

DeleteMessageCommand::DeleteMessageCommand(const DeviceData* pDevice)
: BaseCommand(pDevice)
{
    _success           = false;
    _dataList          = NULL;
    _nAndroidChannelID = PIM;
    _nAndroidCommandID = DEL_SMS;
    _executeCount      = 0;
}

DeleteMessageCommand::~DeleteMessageCommand()
{
}

// 参考：PimProvider::DeleteSmsItems
std::string DeleteMessageCommand::GetAndroidBodyData()
{
    // 判断用户是否取消，若取消则立即返回
    ProgressObserver* pObserver = NULL;
    if (!_observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(_observers[0]);

    BinaryBufferWriter  writer;
    writer.Clear();
    writer.WriteInt(_nAndroidChannelID);
    writer.WriteInt(_nAndroidCommandID);    // 命令类型
    writer.WriteInt(_dataList->size());

    IF_CANCEL(pObserver)
        return writer.GetBuffer();

    size_t i = 0;
    for (i = 0; i < _dataList->size(); i++)
    {
        IF_CANCEL_BREAK(pObserver)
        const MessageData* pData = _dataList->at(i);
        writer.WriteInt(pData->GetId()); // 写入每个短信

        IncreaseTaskNumberComplete();
        SendProgress();

        SetCurrentTaskSum(1);
        SetCurrentTaskName(CCodeOperation::UTF_8ToUnicode(pData->GetStrNumber()));

        AddTaskProgress(1);
        SendProgress();
    }
    _executeCount = (int)i;
    if (_executeCount != (int)_dataList->size())
        writer.ReplaceInt(_executeCount,8);

    return writer.GetBuffer();
}

// 保存返回数据包
void DeleteMessageCommand::SetAndroidReturnData(std::string data)
{
    BinaryBufferReader reader(data.c_str(), data.length());
    if (!parseAndroidPackageHead(&reader))
    {
        _success = false;
        return;
    }
    reader.ReadBool(_success);
}

bool DeleteMessageCommand::Success()
{
    return _success;
}

void DeleteMessageCommand::SetDataList( const std::vector<const MessageData*>* DataList )
{
    _dataList = DataList;
}

void DeleteMessageCommand::Execute()
{
    if ( _dataList == NULL )
    {
        _success = true;
        Complete();
        return;
    }

    // 设置所有任务总量
    SetAllTaskSum(_dataList->size());
    // 设置任务总数
    SetTaskNumberSum(_dataList->size());

    if ( _pDevice->GetDevInfo()->_deviceType == Android )
        DefaultExecute();
    else if ( _pDevice->GetDevInfo()->_deviceType == Ios )
        _success = IosExecute();

}

bool DeleteMessageCommand::IosExecute()
{
    // 没连接上设备则返回
    if (_pDevice->GetDevInfo() == NULL || !_pDevice->GetDevInfo()->_ios_bAFCConn)
        return false;

    // 修改数据库文件
    IosDBPath dbPath(_pDevice,ISMESSAGEDB);
    IosMessageDB messageDB(dbPath);
    messageDB.OpenDB();
    messageDB.BeginTransaction();
    try
    {
		ProgressObserver* pObserver =_observers.empty()?NULL:dynamic_cast<ProgressObserver*>(_observers[0]);

        // 取消还没删除的
		for (size_t i = 0; i < _dataList->size();i++)
		{
			IF_CANCEL_BREAK(pObserver);
			const MessageData* pMsg = _dataList->at(i);
			if(pMsg->GetMessageType() == MessageData::EIMS)
				messageDB.DeleteMadridMessage(pMsg,_pDevice);
			else
			{
				messageDB.DeleteMessage(pMsg);
				DeleteMMSFile(pMsg);
			}
			_executeCount++;
			IncreaseTaskNumberComplete();
			SendProgress();

			SetCurrentTaskSum(1);
			SetCurrentTaskName(CStrOperation::IntToWString(pMsg->GetId()));

			AddTaskProgress(1);
			SendProgress();
		}
        messageDB.Commit();
        messageDB.CloseDB();
        return messageDB.SyncToIos(_pDevice);// 同步到IOS设备
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

void DeleteMessageCommand::DeleteMMSFile(const MessageData* pMessageData)
{
	if (pMessageData->GetMessageType() != MessageData::EMMS)
		return;
	IosDBPath dbPath(_pDevice,ISMESSAGEDB);
	wstring remotepath = dbPath.GetDBFolder()+L"/Parts/";
	wstring foldername = MyFileOperation::ConvertMMSItemFolder(pMessageData->GetId(), pMessageData->GetGroupId());
	foldername = CStrOperation::trimRightW(foldername,L"\\");
	wstring locpath = dbPath.GetTempFolder()+L"sms\\Parts\\"+foldername;
	CFileOperation::DeleteDirectory(locpath);

	remotepath += CStrOperation::ReplaceW(foldername,L"\\",L"/");
	DirData* dir = new DirData(_pDevice);
	dir->_name = remotepath;
	deque<NodeData*> nodes = dir->GetChildrens();
	vector<NodeData*> delnodes;
	for (size_t j =0; j < nodes.size(); j++)
	{
		if(nodes[j]->_name.find(CStrOperation::IntToWString(pMessageData->GetId()))==0)
		{
			delnodes.push_back(nodes[j]);
		}
	}
	FileHelper::BatchDelete(&delnodes,NULL);
	delete dir;
}


#endif