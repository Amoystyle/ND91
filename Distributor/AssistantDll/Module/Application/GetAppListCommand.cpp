#include "pch_module.h"

#if defined( MODULE_APP )

#include "Module/Application/GetAppListCommand.h"
#include "Module/Application/AppData.h"

#include "Device/AdbPMListCommand.h"
#include "IosApp/IosAppOperationFactory.h"
#include "Module/AndroidDefine.h"
#include "Module/File/GetFileInfoCommand.h"

GetAppListCommand::GetAppListCommand(const DeviceData* pDevice, APP_TYPE appType, APP_OWNER appOwner)
: BaseCommand(pDevice)
{
	_appType = appType;
	_appOwner = appOwner;

	//Android
	_nAndroidChannelID = APPLICATION;
    if (appOwner == APP_OWNER_ANY)
        _nAndroidCommandID = GET_ALLAPPLIST;
    else
	    _nAndroidCommandID = GET_APPLIST;
}

GetAppListCommand::~GetAppListCommand()
{
	std::vector<AppData*>::iterator iter;
	for(iter = _DataList.begin(); iter != _DataList.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
	_DataList.clear();
}

// void GetAppListCommand::GetAndroidAppSize()
// {
//     map<wstring, long> appMap;
//     GetAndroidSystemAppMap(appMap);
//     GetAndroidUserAppMap(appMap);
// 
//     for (size_t i = 0; i < _DataList.size(); i++)
//     {
//         AppData* pApp = _DataList.at(i);
//         vector<wstring> splits = CStrOperation::parseStringsW(pApp->_strAppPhonePath, L"/");
//         if (splits.size() < 3)
//             continue;
// 
//         wstring name = splits.at(2);
//         if (!CStrOperation::endsWithW(name, L".apk"))
//             name += L".apk";
// 
//         pApp->_staticDiskUsage = appMap[name];
//     }
// }
// 
// void GetAppListCommand::GetAndroidSystemAppMap(map<wstring, long>& appMap)
// {
//     GetAndroidAppMap(appMap, L"/system/app/");
//     GetAndroidAppMap(appMap, L"/system/framework/");
// }
// 
// void GetAppListCommand::GetAndroidUserAppMap(map<wstring, long>& appMap)
// {
//     GetAndroidAppMap(appMap, L"/data/app/");
//     GetAndroidAppMap(appMap, L"/mnt/asec/");
// }
// 
// void GetAppListCommand::GetAndroidAppMap(map<wstring, long>& appMap, wstring path)
// {
//     DirData appFolder(_pDevice);
//     appFolder._name = path;
//     deque<NodeData*> appPhonePaths = appFolder.GetChildrens();
//     for (size_t i = 0; i < appPhonePaths.size(); i++)
//         if (appPhonePaths.at(i)->_type == NodeData::FILE && 
//             (CStrOperation::endsWithW(appPhonePaths.at(i)->_name, L".apk")
//             || CStrOperation::endsWithW(appPhonePaths.at(i)->_name, L".npk")))
//             appMap[appPhonePaths.at(i)->_name] = appPhonePaths.at(i)->_size;
//         else if (appPhonePaths.at(i)->_type == NodeData::DIR)
//         {
//             DirData subAppFolder(_pDevice);
//             subAppFolder._name = path+appPhonePaths.at(i)->_name+L"/";
//             deque<NodeData*> subAppPhonePaths = subAppFolder.GetChildrens();
//             if (!subAppPhonePaths.empty())
//                 appMap[appPhonePaths.at(i)->_name+L".apk"] = subAppPhonePaths.at(0)->_size;
//         }
// }

void GetAppListCommand::GetAndroidUserAppSize()
{
    GetFileInfoCommand* command = new GetFileInfoCommand(_pDevice);

    for (size_t i = 0; i < _DataList.size(); i++)
    {
        AppData* pApp = _DataList.at(i);
        if (pApp->_eAppOwner == APP_OWNER_SYS)
            continue;

        command->SetRemotePath(pApp->_strAppPhonePath);
        command->Execute();
        pApp->_staticDiskUsage = command->GetAndroidFileInfo()._size;
    }

    SAFE_DELETE(command);
}

// 执行命令
void GetAppListCommand::Execute()
{
	if (!_pDevice || !_pDevice->GetDevInfo())
		return ;

	_DataList.clear();
	if ( _pDevice->GetDevInfo()->_deviceType == Android )
	{
		if (_pDevice->GetAndroidDeamonMode())
			DefaultExecute();
		else
		{
			AdbPMListCommand cmd(_pDevice->GetDevInfo()->_strSerialNumber);
			cmd.Execute();
            _success = cmd.Success();
			vector<AppData*> appList = cmd.getDataList();
			for (vector<AppData*>::iterator it = appList.begin(); it != appList.end(); it++)
				_DataList.push_back(new AppData(*it));
		}
        //GetAndroidUserAppSize();
	}
	else if ( _pDevice->GetDevInfo()->_deviceType == Ios )
	{
		if (!_pDevice->GetDevInfo()->_ios_bAFCConn)//是否连接上设备
			return ;

		_DataList.clear();
		if (_appType == APP_TYPE_ALL)
		{
			APP_TYPE eTypeArray[] = {
				APP_TYPE_PXL,
				APP_TYPE_IPA,
				APP_TYPE_DEB,	
// 				APP_TYPE_INSTALLER,
// 				APP_TYPE_PANDA
            };

            int count = sizeof(eTypeArray) / sizeof(APP_TYPE);
			for (int i = 0; i < count; i++)
            {
                vector<AppData*> vtDataTemp;
                GetIosDataList(eTypeArray[i], vtDataTemp);
                LOG->WriteDebug(L"获得appdata类型:"+CStrOperation::IntToWString(eTypeArray[i])
                                           +L"数量:"+CStrOperation::IntToWString(vtDataTemp.size()));

                _DataList.insert(_DataList.begin(), vtDataTemp.begin(), vtDataTemp.end());
                vtDataTemp.clear();
            }
		}
		else
			GetIosDataList(_appType, _DataList);

        _success = true;
	}
}

const vector<AppData*> GetAppListCommand::GetDataList()
{
	vector<AppData*> vtGet;

	//并非通过守护的数据
	//if (_pDevice->IsActivate())
	{
		vector<AppData*>::iterator it;
		for (it=_DataList.begin(); it!=_DataList.end(); it++)
		{
			AppData* pTemp = *it;
			if (pTemp && ((_appOwner == APP_OWNER_ANY) ||
				(_appOwner == APP_OWNER_USER && pTemp->_eAppOwner == APP_OWNER_USER) ||
				(_appOwner == APP_OWNER_SYS && pTemp->_eAppOwner == APP_OWNER_SYS)))
			{
				vtGet.push_back(pTemp);
			}
		}
	}	

	return vtGet;
}

std::string ND91Assistant::GetAppListCommand::GetAndroidBodyData()
{
	BinaryBufferWriter writer;

	writer.WriteInt(_nAndroidChannelID);
	writer.WriteChar((char)_nAndroidCommandID);
	return writer.GetBuffer();
}

void ND91Assistant::GetAppListCommand::SetAndroidReturnData( std::string data )
{
	BinaryBufferReader reader(data.c_str(), data.length());
	if (!parseAndroidPackageHead_Top16(&reader))
		return;
	int nAllCount = 0;		// 总个数
	int nCurCount = 0;		// 当前读到的软件个数
	//读取个数
	reader.ReadInt32(nAllCount);
	//读取用户软件
	reader.ReadInt32(nCurCount); 
	const char* pData = data.c_str() + reader.GetOffset();
	for (int i=0; i<nCurCount; i++)
	{
		AppData* pApp = new AppData();
		int nLeftDataSize = data.length() - (pData - data.c_str());
		pData += pApp->ParseAndroidData(pData, nLeftDataSize);
// 		if (_appOwner == APP_OWNER_USER) // 这里不需要再给pApp->_eAppOwner赋值了吧，ParseAndroidData里已经赋值了
// 			pApp->_eAppOwner = APP_OWNER_USER;
// 		else if ((int)(pApp->_strAppPhonePath.find(L"/system/")) >= 0)
// 			pApp->_eAppOwner = APP_OWNER_SYS;
// 		else
// 			pApp->_eAppOwner = APP_OWNER_USER;
        pApp->_eAppType = APP_TYPE_APK;
		_DataList.push_back(pApp);
	}
}

void ND91Assistant::GetAppListCommand::SortData()
{
    vector<AppData*> dataList;
    vector<AppData*>::iterator it = _DataList.begin();
    while (it != _DataList.end())
    {
        if ((*it)->_eAppOwner != APP_OWNER_USER)
        {
            dataList.push_back(*it);
            it = _DataList.erase(it);
        }
        else
        {
            it ++;
        }
    }
    _DataList.insert(_DataList.end(), dataList.begin(), dataList.end());
}

void GetAppListCommand::GetIosDataList(APP_TYPE appType, vector<AppData*>& vtData)
{
	if (!_pDevice->IsActivate())
		return ;

	vtData.clear();
	IIosAppOperation* pOperation = IosAppOperationFactory::
        GetAppOperation(_pDevice, appType, *this);

	if (pOperation)
	{
		pOperation->GetAppInfos(vtData);

		delete pOperation;
		pOperation = NULL;
	}
}

int GetAppListCommand::GetIosAppCount()
{
	int nCount = 0;
	APP_TYPE eTypeArray[] = {
		APP_TYPE_PXL,
		APP_TYPE_IPA,
		APP_TYPE_DEB,
// 		APP_TYPE_INSTALLER,
// 		APP_TYPE_PANDA
	};

    int count = sizeof(eTypeArray) / sizeof(APP_TYPE);
	for (int i=0; i< count; i++)
	{
		IIosAppOperation* pOperation = IosAppOperationFactory::
            GetAppOperation(_pDevice, eTypeArray[i], *this);

		if (pOperation)
		{
			nCount += pOperation->GetAppCount(_appOwner);

			delete pOperation;
			pOperation = NULL;
		}
	}	

	return nCount;
}

#endif