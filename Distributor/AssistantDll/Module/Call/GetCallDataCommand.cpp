#include "pch_module.h"

#ifdef MODULE_CALL

#include "GetCallDataCommand.h"
#include "IosCallDataDB.h"

#include "Module/Common/IosDB/IosDBPath.h"
#include "Module/AndroidDefine.h"

GetCallDataCommand::GetCallDataCommand(const DeviceData* pDevice)
: BaseCommand(pDevice)
{
    _nAndroidChannelID = PIM;
    _nAndroidCommandID = GET_CALL;
    _onlySize          = false;
    _size              = 0;
}

GetCallDataCommand::~GetCallDataCommand()
{
    std::vector<CallData*>::iterator iter;
    for(iter = _dataList.begin(); iter != _dataList.end(); iter++)
    {
		SAFE_DELETE(*iter);
    }
    _dataList.clear();
}

std::string GetCallDataCommand::GetAndroidBodyData()
{
    BinaryBufferWriter writer;

	writer.WriteInt(_nAndroidChannelID);
	writer.WriteInt(_nAndroidCommandID);

	return writer.GetBuffer();
}

void ND91Assistant::GetCallDataCommand::SetAndroidReturnData(std::string data)
{
	BinaryBufferReader reader(data.c_str(), data.length());
	if (!parseAndroidPackageHead(&reader))
    {
        _result = COMMAND_EXECUTE_FAIL;
		return;
    }
	int nCount = 0;		//nCount 标识接下来记录的条数
	reader.ReadInt32(nCount);

    const char* pData = data.c_str() + reader.GetOffset();
	for (int i=0; i<nCount; i++)
	{
		CallData* pCallData = new CallData();
        int nLeftDataSize = data.length() - (pData - data.c_str());
		pData += pCallData->ParseAndroidData(pData, nLeftDataSize);
		_dataList.push_back(pCallData);
	}
}

void GetCallDataCommand::SetOnlySize(bool onlySize)
{
    _onlySize = onlySize;
}

void GetCallDataCommand::Execute()
{
    _result = COMMAND_EXECUTE_SUCCESS;
    if ( _pDevice->GetDevInfo()->_deviceType == Android )
        DefaultExecute();
    else if ( _pDevice->GetDevInfo()->_deviceType == Ios )
    {
        if (!IosExecute())
            _result = COMMAND_EXECUTE_FAIL;
    }
}

const vector<CallData*>* GetCallDataCommand::GetDataList()
{
    return &_dataList;
}

int GetCallDataCommand::GetSize()
{
    return _size;
}

bool GetCallDataCommand::IosExecute()
{
    bool result = false;
    // 没连接上设备则返回
    if (_pDevice->GetDevInfo() == NULL || !_pDevice->GetDevInfo()->_ios_bAFCConn)
        return result;

    IosDBPath dbPath(_pDevice,ISCALLDATADB);
    IosCallDataDB callDataDB(dbPath);
    // 加载数据库
    if (!callDataDB.Reload(_pDevice))
        return result;

    // 打开数据库
    if (!callDataDB.OpenDB())
        return result;

    if (_onlySize)
    {
        // 获取短信数量
        if (callDataDB.GetCallsSize(&_size))
            result = true;
        else
            result = false;
    }
    else
    {
        // 获取所有通话记录
        if (callDataDB.GetCalls(&_dataList))
            result = true;
        else
            result = false;
    }

    // 关闭数据库
    callDataDB.Close();

    return result;
}


#endif