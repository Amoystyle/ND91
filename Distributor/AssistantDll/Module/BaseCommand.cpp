#include "pch_module.h"
#include "BaseCommand.h"
#include "AndroidDefine.h"

#include "Device/IosDaemon.h"
#include "Device/IosFileIO.h"
#include "Common/Path.h"
#include "Device/AndroidTcpIO.h"


BaseCommand::BaseCommand(const DeviceData* pDevice)
: _pDevice((DeviceData*)pDevice)
{
    //assert(pDevice);
	_pBaseIO = NULL;

    // �������������ֵ����ֵ��ͬ��������������
    _pHead = ANDROID_PACKAGE_NDPC;
    _nFlag = Client | NeedPostBack;

    _nAndroidChannelID = 0;
    _nAndroidCommandID = 0;

    memset(_pReturnHead, 0, 4);
    _nReturnFlag = 0;
    _nReturnLongConnectSign = 0;
    _nReturnChannelID = 0;
    _nReturnCommonType = 0;
    _nReturnBodySize = 0;

    _lastAction = NONE;

	_strIosCmdFileName = L"";

	_success = true;
    _result = COMMAND_EXECUTE_SUCCESS;
	_executesuccess = false;
}

BaseCommand::~BaseCommand()
{
    SAFE_DELETE( _pBaseIO );
}

const DeviceInfo* BaseCommand::GetDeviceInfo()
{
    return _pDevice->GetDevInfo();
}

std::string BaseCommand::GetAndroidDatagram()
{
    BinaryBufferWriter writer;	//����Android�����Writer

    // ����ͷ������
    writer.WriteBuffer(_pHead, 4);
    writer.WriteShort(_nFlag);
    writer.WriteShort(_pDevice->GetDevInfo()->_nLongConnectSign);

    // ȡ���������ݲ�д��
    writer.WriteString(GetAndroidBodyData());


    return writer.GetBuffer();
}

bool BaseCommand::parseAndroidPackageHead( BinaryBufferReader* pReader )
{
    pReader->ReadBuffer(_pReturnHead, 4);
    pReader->ReadShort(_nReturnFlag);
    pReader->ReadShort(_nReturnLongConnectSign);
    pReader->ReadInt32(_nReturnBodySize);
    pReader->ReadInt32(_nReturnChannelID);
    pReader->ReadInt32(_nReturnCommonType);
	//������ص����ݰ���Ϣ�����ϱ�Command����Ϣ��������
	if (_nReturnChannelID != _nAndroidChannelID || _nReturnCommonType != _nAndroidCommandID)
		return false;
	_executesuccess = true;
    return true;
}

bool BaseCommand::parseAndroidPackageHead_Top16( BinaryBufferReader* pReader )
{
	pReader->ReadBuffer(_pReturnHead, 4);
	pReader->ReadShort(_nReturnFlag);
	pReader->ReadShort(_nReturnLongConnectSign);
	pReader->ReadInt32(_nReturnBodySize);
	pReader->ReadInt32(_nReturnChannelID);
	if (_nReturnChannelID != _nAndroidChannelID)
		return false;
	_executesuccess = true;
	return true;
}

bool BaseCommand::parseAndroidPackageHead_Top12( BinaryBufferReader* pReader )
{
	pReader->ReadBuffer(_pReturnHead, 4);
	pReader->ReadShort(_nReturnFlag);
	pReader->ReadShort(_nReturnLongConnectSign);
	pReader->ReadInt32(_nReturnBodySize);
	if (memcmp(_pReturnHead, "NDPC", 4) != 0)
	{
		LOG->WriteDebug(L"PackageHead not have NDPC");
		return false;
	}
	_executesuccess = true;
	return true;
}

void BaseCommand::DefaultExecute()
{
 	//if (_pDevice->GetDevInfo()->_deviceType == Android)
  //   {
  //       //SetCSLock(_pDevice->_csAndroidCommandLock);
  //       EnterCriticalSection(&_pDevice->_csAndroidCommandLock);
  //   }

try{

	if (!_pDevice || !_pDevice->GetDevInfo())
		goto END ;

    switch( _pDevice->GetDevInfo()->_deviceType )
    {
    case Android:		
        {
			wstring strCommandMsg = CStrOperation::IntToWString(_nAndroidChannelID) + L"-" + CStrOperation::IntToWString(_nAndroidCommandID);
			if (_pDevice->GetAndroidDeamonMode() == 0) // ���ػ�ģʽ
			{
				LOG->WriteDebug(L"���ػ�ģʽ");
				break;
			}

			string cmd = "init";   // ������ֵ������log���ж��Ƿ��Ѿ�����ֵ
			string ret = "init";   // ������ֵ������log���ж��Ƿ��Ѿ�����ֵ
			int outtimecount = 0;
			try
			{
				cmd = GetAndroidDatagram();
				while( ret == "init" || ret.empty())
				{           
					AndroidTcpIO* pIO = dynamic_cast<AndroidTcpIO*>(GetIO());
					if (!pIO)
					{
						LOG->WriteDebug(L"�ػ�TCP����ʧ��");
						goto END;
					}
					ret = pIO->SendAndroidDatagram(cmd);
					SetAndroidReturnData(ret);

					LOG->WriteDebug(L"ִ��BaseCommand " + strCommandMsg);

					outtimecount++;
					if(outtimecount>10) //10�볬ʱ
					{
						LOG->WriteDebug(L"�ػ����ӳ�ʱ");
						_executesuccess = false;
						break;
					}
				}
			}
			catch (...)
			{
				// �������ϸ��
				LOG->WriteDebug(L"ִ��BaseCommand Error" + strCommandMsg);
				LOG->WriteDebug(L"GetAndroidDatagram return:");
				LOG_BINARY(Log::LOG_DEBUG, cmd);
				LOG->WriteDebug(L"SendAndroidDatagram return:");
				LOG_BINARY(Log::LOG_DEBUG, ret);
				LOG->WriteDebug(CCodeOperation::StringToWstring("BaseCommand::DefaultExecute ʧ��, ͨѶ����û����Ӧ"));
				goto END;
			}
        }
        break;
    case Ios:
        {
            // ���ػ���û��Ҫ��
			if (_pDevice->GetIosDeamonMode() == 0 ||
                ! _pDevice->GetDevInfo()->_ios_bJailBreaked ||
                ! _pDevice->GetDevInfo()->_ios_daemonInstalled )
				break;
            SetIosReturnFile(GetIO()->SendIosFile(GetIosCommandFilePath(),
                GetIosResultFilePath(), GetIosCommandFileContent()));
        }
        break;

    default:
        THROW_ASSISTANT_EXCEPTION(UNKNOWN_DEVICE_OR_CONNECTION);
        break;
    }
  //  LOG->WriteDebug(L"BaseCommand::DefaultExecute End");
}
catch(AssistantException e)
{/*
     if (_pDevice->GetDevInfo()->_deviceType == Android)
     {
         LeaveCriticalSection(&_pDevice->_csAndroidCommandLock);
     }*/
    throw e;
}
catch (...)
{

}
END:
   /*  if (_pDevice->GetDevInfo()->_deviceType == Android)
     {
         LeaveCriticalSection(&_pDevice->_csAndroidCommandLock);
     }*/
	return;
}

BaseIO* ND91Assistant::BaseCommand::GetIO()
{
	if ( ! _pBaseIO )
		_pBaseIO = _pDevice->CreateIO();

	return _pBaseIO;
}

EXIST_ACTION BaseCommand::AskUser(std::wstring destName, bool isDir)
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

std::wstring BaseCommand::GetIosCommandFilePath()
{
	//����IOS�豸�ϵ������ļ�
	IosDaemon daemon(NULL, L"");

	wstring strFileName_OnPhone = daemon.CreatDaemonCypher(_strIosCmdFileName, 
		GetDeviceInfo()->_ios_devicebTurnCypher, 
		GetDeviceInfo()->_ios_deviceszDaemonCypherBytes);

	return Path::IosDeviceCommandPath() + strFileName_OnPhone;
}

std::wstring BaseCommand::GetIosResultFilePath()
{
	return Path::IosDeviceCommandPath() + _strIosCmdFileName + L".done";
}

std::wstring BaseCommand::GetIosCommandFileContent()
{
	return L"";
}

std::wstring BaseCommand::GetIosDoneInfoFromFileName()
{
	IosFileIO* pFileIO = dynamic_cast<IosFileIO*>(GetIO());
	if ( ! pFileIO )
	{
		THROW_ASSISTANT_EXCEPTION(INVALID_IO_OBJECT);
	}

	IIosFileSystem* pFileStream = pFileIO->GetIosFileStream();
	if (!pFileStream)
	{
		THROW_ASSISTANT_EXCEPTION(INVALID_IO_OBJECT);
	}

	// �ȴ����ؽ���ļ�
	bool blnCompleted = false; 
	unsigned long timeout = 30000; //30��ȴ�ʱ��
	unsigned long lFirstTick = GetTickCount();
    //unsigned long lFirstTick = IosPublicFunc::MyGetTickCount();
	unsigned long lNextTick = 0;	
	wstring strResultFileFirstName = _strIosCmdFileName;
	wstring strInfo = L"";

	// ѭ���ȴ�����ķ��ؽ��
	do 
	{
		Sleep(100);
		if ( LookForResultFile(pFileStream, strResultFileFirstName, strInfo) )
			break;

		lNextTick = GetTickCount();
        //lNextTick = IosPublicFunc::MyGetTickCount();

	} while ((lNextTick-lFirstTick) < timeout);	

	return strInfo;
}

#define RETURN_FILE_SUFFIX  L".done"

bool BaseCommand::LookForResultFile( IIosFileSystem* pFileStream, wstring filePrefix, wstring& strInfo)
{
	strInfo = L"";
	// ���������Ŀ¼
	DirData resultDir(_pDevice);
	resultDir._name = Path::IosDeviceCommandPath();
	resultDir._name.erase(resultDir._name.size()-1); // ȥ����β��'/'

	// ȡĿ¼�µ��ļ�
	pFileStream->Ios_GetFileInfos(&resultDir, eFileGetType_Files);

	deque<NodeData*> childs = resultDir.GetChildrens();
	for ( size_t i=0; i < childs.size(); ++i )
	{
		NodeData* pFile = childs[i];
		wstring name = pFile->_name;

		// ����ļ����ǡ������ļ��� + XXXX + .done����ʽ���Ͷ���
		if (CStrOperation::startsWith(name, filePrefix) &&
			CStrOperation::endsWithW(name, RETURN_FILE_SUFFIX))
		{
			pFileStream->Ios_DeleteFile(pFile->GetFullPath());

			int left = filePrefix.length()+1;
			int right = wstring(RETURN_FILE_SUFFIX).length();
			strInfo = name.substr(left, name.length() - left - right);

			return true;
		}
	}

	return false;
}