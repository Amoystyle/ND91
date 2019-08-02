#include "pch_module.h"

#ifdef MODULE_MESSAGE

#include "Module/Message/SendMessageCommand.h"
#include "Module/Message/MessageData.h"
#include "Module/Message/MessageReceiptData.h"

#include "Device/NoDeamonInterface/NoDeamonInterface.h"
#include "Device/IosFileIO.h"
#include "Monitor/MonitorThread.h"
#include "Device/IosDaemon.h"
#include "Common/Path.h"
#include "Module/AndroidDefine.h"

SendMessageCommand::SendMessageCommand(const DeviceData* pDevice)
: BaseCommand(pDevice)
{
    _success           = false;
    _dataList          = NULL;
    _pMessageData      = NULL;

    _nAndroidChannelID = PIM;
    _nAndroidCommandID = SEND_SMS;
    _sendSuccessIds.clear();
}

SendMessageCommand::~SendMessageCommand()
{
}

std::string SendMessageCommand::GetAndroidBodyData()
{
    BinaryBufferWriter writer;
    writer.Clear();
    writer.WriteInt(_nAndroidChannelID);
    writer.WriteInt(_nAndroidCommandID);

    // д��Ҫ���͵Ķ��ţ�ÿ��ֻ�ܷ���һ��
    writer.WriteInt(_pMessageData->GetId());
    writer.WriteString(_pMessageData->GetStrNumber());
    writer.WriteTime(_pMessageData->GetTime()*1000);
    writer.WriteInt(_pMessageData->GetCategory());
    writer.WriteStringAsUTF8(_pMessageData->GetText());
    writer.WriteBool(_pMessageData->GetRead());

    return writer.GetBuffer();
}

// ���淵�����ݰ�
void SendMessageCommand::SetAndroidReturnData(std::string data)
{
    BinaryBufferReader reader(data.c_str(), data.length());
    if (!parseAndroidPackageHead(&reader))
    {
        _success = false;
        return;
    }

    int id = 0;
    time_t time = 0;
    reader.ReadInt32(id);
    reader.ReadTime32(time);
    _pMessageData->SetId(id);
    _pMessageData->SetTime(time);

    _success = true;
}

bool SendMessageCommand::Success()
{
    return _success;
}

void SendMessageCommand::SetDataList( const std::vector<MessageData*>* DataList)
{
    _dataList = DataList;
}

void SendMessageCommand::Execute()
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

bool SendMessageCommand::AndroidExecute()
{
    // �ж��û��Ƿ�ȡ������ȡ������������
    ProgressObserver* pObserver = NULL;
    if (!_observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(_observers[0]);

    // �ִ�ִ��_dataList�������
    for (size_t i = 0; i < _dataList->size(); i++)
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
    return true;
}

bool SendMessageCommand::IosExecute()
{
    if (_dataList->size() == 0)
        return false;

    // ����iPhone�� ֱ�ӷ���false
    RETURN_FALSE_IF((int)(_pDevice->GetDevInfo()->GetDevModelName().find(L"iPhone")) < 0);
    // ���û��sim�������Ҳ���cdma���ֻ�����false
    RETURN_FALSE_IF(!_pDevice->GetDevInfo()->_ios_isSimInserted && 
                    (int)(_pDevice->GetDevInfo()->GetDevTypeText().find(L"CDMA")) < 0);

    // �ж��û��Ƿ�ȡ������ȡ������������
    ProgressObserver* pObserver = NULL;
    if (!_observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(_observers[0]);

    if (_pDevice->GetDevInfo()->_connectType == USB 
        && _pDevice->GetDevInfo()->_ios_bJailBreaked)
        return IosNoDeamonExecut();
    else
        return IosDeamonExecut();

    return true;
}

bool SendMessageCommand::IosDeamonExecut()
{
    //��ȡ���봮
    //string sendTo = GetPhoneNumberStr();

    _strIosCmdFileName = L"send." + GetSerial(_pDevice);
    // ���ػ���û��Ҫ��
    if (_pDevice->GetIosDeamonMode() == 0 ||
        ! _pDevice->GetDevInfo()->_ios_bJailBreaked)
        return false;

    wstring filepath = GetIO()->SendIosFile(GetIosCommandFilePath(),
        GetIosResultFilePath(), GetIosCommandFileContent(), false);

    WaiteForResult();

    return true;
}

bool SendMessageCommand::IosNoDeamonExecut()
{
    for (size_t i = 0; i < _dataList->size(); i++)
    {
        MessageData* pData = _dataList->at(i);
        int res = NoDeamonInterface::SendSmsSingle(_pDevice->GetDevInfo()->_ios_devptr, 
                                                   (wchar_t*)CCodeOperation::UTF_8ToUnicode(pData->GetStrNumber()).c_str(), 
                                                   (wchar_t*)pData->GetText().c_str());

        ReceiptMessage(0, pData->GetStrNumber(), (res == 0));

        IncreaseTaskNumberComplete();
        SendProgress();
        SetCurrentTaskSum(1);
        SetCurrentTaskName(CCodeOperation::UTF_8ToUnicode(pData->GetStrNumber()));
        AddTaskProgress(1);
        SendProgress();
    }

    return true;
}

// ��ȡ�绰�����ַ���
string SendMessageCommand::GetPhoneNumberStr()
{
    string sendTo = "";
    for (size_t i = 0; i < _dataList->size(); i++)
    {
        string phoneNumber = _dataList->at(i)->GetStrNumber();
        sendTo += phoneNumber;
        sendTo += ",";
        _phoneVec.push_back(phoneNumber);
    }
    return CStrOperation::trimRight(sendTo, ",");
}

void SendMessageCommand::WaiteForResult()
{
	unsigned long timeout = 10000*_dataList->size(); //�ȴ�ʱ��
	unsigned long lFirstTick = GetTickCount();
	unsigned long lNextTick = 0;	
	IosFileIO* pFileIO = dynamic_cast<IosFileIO*>(GetIO());
	if ( ! pFileIO )
	{
		THROW_ASSISTANT_EXCEPTION(INVALID_IO_OBJECT);
	}
	IIosFileSystem* pFileStream = pFileIO->GetIosFileStream();

	DirData* dir = new DirData(_pDevice);
	dir->_name = CStrOperation::trimRightW(Path::IosMessageFolder(),L"/"); 
	map<string, int> sendNumbers;
	do 
	{
		Sleep(1000);
		dir->Refresh();
		deque<NodeData*> nodes = dir->GetChildrens();
		for (size_t i = 0; i < nodes.size(); i++)
		{
			if(!CStrOperation::startsWith(nodes[i]->_name, _strIosCmdFileName+L"."))
				continue;
			wstring strname = nodes[i]->_name;
			strname = strname.substr(_strIosCmdFileName.length()+1);
			vector<string> vecReturn = CStrOperation::parseStrings(CCodeOperation::UnicodeToUTF_8(strname), '.');
			if (CStrOperation::endsWithW(strname, L".P"))// ���ڷ�����...
			{
				continue;
			}
			else if (CStrOperation::endsWithW(strname, L".S"))//���ŷ��ͳɹ�
			{
				int id = atoi(vecReturn[1].c_str());
				sendNumbers[vecReturn[0]] = id;
				ReceiptMessage(id, vecReturn[0], true);
			}
			else if (CStrOperation::endsWithW(strname, L".F"))//���ŷ���ʧ��
			{
				sendNumbers[vecReturn[0]] = 0;
				ReceiptMessage(0, vecReturn[0], false);
			}
			pFileStream->Ios_DeleteFile(nodes[i]->GetFullPath());

			IncreaseTaskNumberComplete();
			SendProgress();
			SetCurrentTaskSum(1);
			SetCurrentTaskName(CCodeOperation::UTF_8ToUnicode(vecReturn[0]));
			AddTaskProgress(1);
			SendProgress();
		}
		lNextTick = GetTickCount();
	} while((lNextTick-lFirstTick) < timeout);

	for (size_t i = 0; i < _dataList->size(); i++)
	{
		if(sendNumbers.find(_dataList->at(i)->GetStrNumber()) == sendNumbers.end())
		{
			ReceiptMessage(0, _dataList->at(i)->GetStrNumber(), false);
		}
	}
	SAFE_DELETE(dir);
}


// ���ն��Ż�ִ
void SendMessageCommand::ReceiptMessage(int id, string phoneNumber, bool success)
{
    MonitorThread* pMonitorThread =  _pDevice->GetMonitorThread();

    MessageReceiptData messageReceipt;
    messageReceipt._id = id;
    messageReceipt._ios_number = phoneNumber;
    messageReceipt._success = success;
    pMonitorThread->MessageReceipt(&messageReceipt);

    if (success)
        if (find(_sendSuccessIds.begin(),_sendSuccessIds.end(),id) == _sendSuccessIds.end())
            _sendSuccessIds.push_back(id);      
}

wstring SendMessageCommand::GetSerial(const DeviceData * pDevice)
{
    wstring strSerial = L"";

    if (pDevice->GetDevInfo() != NULL && pDevice->GetDevInfo()->_ios_bAFCConn)
        strSerial = pDevice->GetDevInfo()->_ios_deviceIdentifier;

    if (strSerial == L"")
        strSerial = CFileOperation::GetGuidW();//Ĭ�����кţ��������guid������40λ������0����

    return strSerial;
}

std::wstring ND91Assistant::SendMessageCommand::GetIosCommandFileContent()
{
	string number = "<to>" + GetPhoneNumberStr() + "</to>" + "\r\n\r\n";
	wstring content = _dataList->at(0)->GetText() + L"\r\n";
	return CCodeOperation::UTF_8ToUnicode(number)+content;
}

void ND91Assistant::SendMessageCommand::SetIosReturnFile( std::wstring filename )
{
}

std::wstring SendMessageCommand::GetIosCommandFilePath()
{
	//����IOS�豸�ϵ������ļ�
	IosDaemon daemon(NULL, L"");
	wstring strFileName_OnPhone = daemon.CreatDaemonCypher(_strIosCmdFileName, 
															GetDeviceInfo()->_ios_devicebTurnCypher, 
															GetDeviceInfo()->_ios_deviceszDaemonCypherBytes);

	return Path::IosMessageFolder() + strFileName_OnPhone;
}

std::wstring SendMessageCommand::GetIosResultFilePath()
{
	return Path::IosMessageFolder() + _strIosCmdFileName + L".done";
}


#endif