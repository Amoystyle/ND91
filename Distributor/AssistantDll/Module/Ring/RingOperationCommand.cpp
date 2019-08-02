#include "pch_module.h"

#ifdef MODULE_RING

#include "RingOperationCommand.h"
#include "Module/System/SystemManager.h"
#include "Common/Path.h"
#include "Module/AndroidDefine.h"

RingOperationCommand::RingOperationCommand(const DeviceData* pDevice)
:BaseCommand(pDevice)
{
	_success	= false;
	_ring		= NULL;
	_kind		= RingData::Null;
	_title		= L"";
	_type		= ENone;
	_ringid		= L"";

	_pDirCmd	= NULL;
	_pUpload	= NULL;
	_pDownCmd	= NULL;
	_nAndroidChannelID=MEDIA;

	_strIosRingList			= _pDevice->GetIosRingPath()->GetIosRingListPath();

	_strTempRingList		= _pDevice->GetIosRingPath()->GetLocalRingListPath();

	_strIosRingSetList		= _pDevice->GetIosRingPath()->GetIosRingSetListPath();

	_strTempRingSet			= _pDevice->GetIosRingPath()->GetLocalRingSetListPath();

	_strIosRingWallerSet	= _pDevice->GetIosPicturePath()->GetIosRingWallerSet();

	_strTempRingWallerSet	= _pDevice->GetIosPicturePath()->GetLocalRingWallerSet();

}

RingOperationCommand::~RingOperationCommand(void)
{
	SAFE_DELETE(_pDirCmd);
	SAFE_DELETE(_pDownCmd);
	SAFE_DELETE(_pUpload);
}

void RingOperationCommand::setParam(RingData* ring,
                                    RingData::MediaFileKind kind /*= RingData::Null*/,
                                    wstring tittle /*= L""*/ )
{
	_ring = ring;
	_kind = kind;
	_title = tittle;
}


void RingOperationCommand::Execute()
{
   _success = false;

   if ( _pDevice->GetDevInfo()->_deviceType == Ios)
   {
	   if (_ring == NULL && _type != EGetCurrentRing)
		   return;

	   //if ( !GetDirCmd()->Exist(_strTempRingList) )
       if (!CFileOperation::IsFileExist(_strTempRingList.c_str()))
		   GetDownCmd()->Download(_strTempRingList, _strIosRingList);

	   switch(_type)
	   {
	   case ERemoveRing:
		   {
			   //if (_kind == NULL)
			   if (_kind == 0)
			   {
				   _success = DeleteRing();
			   }
		   }	   
		   break;
	   case ESetRing:
		   _success = SetIosRing();
		   break;
	   case ERenameRing:
		   _success = ChangeRingname();
		   break;
	   case EReplaceRing:
		   _success = ReplaceRing();
		   break;
	   case EGetCurrentRing:
		   _ringid	= GetIosRing();
		   break;
	   case EDownLoadRing:
		   _success = DownLoadRing();
		   break;
	   default:
		   break;
	   }
   }
   else if (_pDevice->GetDevInfo()->_deviceType == Android)
   {
	   DefaultExecute();
   }

}

std::string RingOperationCommand::GetAndroidBodyData()
{
    BinaryBufferWriter  writer;
	writer.WriteInt(MEDIA);
	writer.WriteChar((char)_type);

	switch(_type)
	{
		case ERemoveRing:
			writer.WriteInt(_ring->_Id);
			writer.WriteChar(0);
			break;
		case ESetRing:
			writer.WriteInt(_ring->_Id);
			writer.WriteBool(_ring->_IsSystemRing);
			writer.WriteInt(_kind);
			break;
		case ERenameRing:
			writer.WriteInt(_ring->_Id);
			writer.WriteString(CCodeOperation::UnicodeToUTF_8(_title));
			break;
		case EChangeRingType:
			writer.WriteInt(_ring->_Id);
			writer.WriteInt(_kind);
			break;
		default:
			break;
	}
	return writer.GetBuffer();
}




void RingOperationCommand::SetAndroidReturnData( std::string data )
{
	BinaryBufferReader  reader(data.c_str(), data.length());
	if(!parseAndroidPackageHead(&reader) || _nReturnChannelID != MEDIA)
		return;

	switch(_type)
	{
		case ERemoveRing:
		case ESetRing:
		case ERenameRing:
		case EChangeRingType:
			{
				char* pOK = new char[5];
				reader.ReadBuffer(pOK,4);
				if(!strncmp(pOK,"OKAY",4))
				{
                    _success = true;
				}
				delete []pOK;
			}
			break;
		default:
			break;
	}

}

bool RingOperationCommand::SettingRing( RingData* ring, RingData::MediaFileKind kind )
{
	_type = ESetRing;
	setParam(ring, kind);
	Execute();
	return _success;
}

bool RingOperationCommand::RenameRing( RingData* ring, wstring tittle )
{
	_type = ERenameRing;
	setParam(ring,RingData::Null,tittle);
	Execute();
	return _success;
}

bool RingOperationCommand::ReplaceIosRing( RingData* ring, wstring filepath )
{
	_type = EReplaceRing;
	setParam(ring,RingData::Null,filepath);
	Execute();
	return _success;
}

wstring RingOperationCommand::GetIosCurrentRing()
{
	_type = EGetCurrentRing;
	Execute();
	return _ringid;
}

bool RingOperationCommand::RemoveRing( RingData* ring )
{
	_type = ERemoveRing;
	setParam(ring);
	Execute();
	return _success;
}

bool RingOperationCommand::DownLoadIosRing( RingData* ring, wstring destpath )
{
	_type = EDownLoadRing;
	setParam(ring,RingData::Null,destpath);
	Execute();
	return _success;
}

bool RingOperationCommand::ChangeRingType( RingData* ring, RingData::MediaFileKind kind )
{
	_type = EChangeRingType;
	setParam(ring,kind);
	Execute();
	return _success;
}

bool RingOperationCommand::parseAndroidPackageHead( BinaryBufferReader* pReader )
{
	pReader->ReadBuffer(_pReturnHead, 4);
	pReader->ReadShort(_nReturnFlag);
	pReader->ReadShort(_nReturnLongConnectSign);
	pReader->ReadInt32(_nReturnBodySize);
	pReader->ReadInt32(_nReturnChannelID);
	return true;
}


bool RingOperationCommand::DeleteRing()
{
	std::string strKey, strVal, strname;

	if (NULL == _ring)
		return false;

	RETURN_FALSE_IF ( !CFileOperation::IsFileExist(_strTempRingList.c_str()) );
	if ( MyFileOperation::IsIosPlist(_strTempRingList) )
		IosPlistConverter::GetXmlFromPlist(_strTempRingList, _strTempRingList);          //.Plist转XML

	file<> fdoc( CCodeOperation::UnicodeToUTF_8(_strTempRingList).c_str() );
	xml_document<char> doc;
	doc.parse<0>(fdoc.data());

	rapidxml::xml_node<char> * root = doc.first_node();            //xml文件的根节点
	rapidxml::xml_node<char> * scnode = root->first_node();        //根节点的子节点
	rapidxml::xml_node<char> * chnode = scnode->last_node();       //子节点最后一个节点
	for (rapidxml::xml_node<char> * listnode = chnode->first_node();
        listnode!=NULL; listnode = listnode->next_sibling())
	{
		strname = listnode->name();
		if ( STRKEY ==strname )
		{
			strKey = listnode->value();                             //获取实际存放铃声文件的名字
			if (CCodeOperation::UTF_8ToUnicode(strKey) == _ring->_name)
			{
				chnode->remove_node(listnode);                      //删除真实文件名节点
				break;
			}
		}
	}
	for (rapidxml::xml_node<char> * listnode = chnode->first_node();
        listnode!=NULL; listnode = listnode->next_sibling())
	{
		strname = listnode->name();
		if ( DICT == strname )
		{				
			for (rapidxml::xml_node<char> * node = listnode->first_node();
                node!=NULL; node = node->next_sibling())
			{
				strVal = node->name();
				if ( STRING == strVal)
				{
					strVal = node->value();
					if (_ring->_Guid == strVal)				//Ringtones.plist里面的铃声Name或者GUID
					{
						chnode->remove_node(listnode);
						goto SYNC;
					}
				}
			}			
		}
	}
SYNC:
	std::string text;  
	rapidxml::print(std::back_inserter(text), doc, 0);
	text = XML_PLIST_Base + text; 
	CFileOperation::WriteBinFile(_strTempRingList, text.c_str(), text.length());  //写入.plist文件	

	IosPlistConverter::GetPlistFromXml(_strTempRingList, _strTempRingList);          //XML转.Plist

	RETURN_FALSE_IF( ! GetDirCmd()->DeleteFileLink(_strIosRingList) );

	RETURN_FALSE_IF( ! GetUpLoadCmd()->Upload(_strTempRingList, _strIosRingList) );
		
	return true;
}

bool RingOperationCommand::ChangeRingname()
{
	std::string strVal, strTitle;

	strTitle = CCodeOperation::UnicodeToUTF_8(_title);

	if (NULL == _ring)
		return false;

	RETURN_FALSE_IF ( !CFileOperation::IsFileExist(_strTempRingList.c_str()) );
	if ( MyFileOperation::IsIosPlist(_strTempRingList) )
		IosPlistConverter::GetXmlFromPlist(_strTempRingList, _strTempRingList);          //.Plist转XML

	file<> fdoc( CCodeOperation::UnicodeToUTF_8(_strTempRingList).c_str() );
	xml_document<char> doc;
	doc.parse<0>(fdoc.data());

	rapidxml::xml_node<char> * root = doc.first_node();            //xml文件的根节点
	rapidxml::xml_node<char> * scnode = root->first_node();        //根节点的子节点
	rapidxml::xml_node<char> * chnode = scnode->last_node();       //子节点最后一个节点
	for (rapidxml::xml_node<char> * listnode = chnode->first_node();
        listnode!=NULL; listnode = listnode->next_sibling())
	{
		std::string strname = listnode->name();

		if ( DICT == strname )
		{				
			for (rapidxml::xml_node<char> * node = listnode->first_node();
                node!=NULL; node = node->next_sibling())
			{
				strVal = node->name();
				if ( STRING == strVal)
				{
					strVal = node->value();
					if (_ring->_Guid == strVal)     //Ringtones.plist里面的铃声GUID
					{
						listnode->remove_last_node();                                   //修改节点数据
						listnode->append_node(doc.allocate_node(node_element,"string",strTitle.c_str()));                                       
						break;
					}
				}
			}			
		}
	}

	std::string text;  
	rapidxml::print(std::back_inserter(text), doc, 0);
	text = XML_PLIST_Base + text; 
	CFileOperation::WriteBinFile(_strTempRingList, text.c_str(), text.length());  //写入.plist文件

	IosPlistConverter::GetPlistFromXml(_strTempRingList, _strTempRingList);          //XML转.Plist

	RETURN_FALSE_IF ( !GetDirCmd()->DeleteFileLink(_strIosRingList) );

	RETURN_FALSE_IF ( !GetUpLoadCmd()->Upload(_strTempRingList, _strIosRingList) )			//替换掉IOS原来的Ringtones.plist

	return true;
}

bool RingOperationCommand::SetIosRing()
{
	if (NULL == _ring)
		return false;

	if ( !GetDirCmd()->Exist(_strTempRingSet) )
	{
		GetDownCmd()->Download(_strTempRingSet, _strIosRingSetList);		
	}
	RETURN_FALSE_IF ( !CFileOperation::IsFileExist(_strTempRingList.c_str()) );
	if ( MyFileOperation::IsIosPlist(_strTempRingList) )
		IosPlistConverter::GetXmlFromPlist(_strTempRingList, _strTempRingList);          //.Plist转XML

	bool have = false;
	std::string strname, strVal, strTemp = "itunes:" + _ring->_Guid;

	file<> fdoc( CCodeOperation::UnicodeToUTF_8(_strTempRingSet).c_str() );
	xml_document<char> doc;
	doc.parse<0>(fdoc.data());

	rapidxml::xml_node<char> * root = doc.first_node();            //xml文件的根节点
	rapidxml::xml_node<char> * scnode = root->first_node();        //根节点的第一个子节点
	for (rapidxml::xml_node<char> * listnode = scnode->first_node();
        listnode!=NULL; listnode = listnode->next_sibling())
	{
		strname = listnode->name();
		strVal = listnode->value();
		if ( STRING == strname && strVal.substr(0,7) == ITUNES )
		{
			have = true;
			rapidxml::xml_node<>* string = doc.allocate_node(node_element,"string",strTemp.c_str());
			scnode->insert_node(listnode,string);             //修改List中铃声的GUID,先插再删
			scnode->remove_node(listnode);
			break;
		}
	}

	if ( !have )
	{
		scnode->append_node(doc.allocate_node(node_element,"key","ring-vibrate"));
		scnode->append_node(doc.allocate_node(node_element,"integer", "1"));
		scnode->append_node(doc.allocate_node(node_element,"key", "ringtone"));
		scnode->append_node(doc.allocate_node(node_element,"string", strTemp.c_str()));
		scnode->append_node(doc.allocate_node(node_element,"key","silent-vibrate"));
		scnode->append_node(doc.allocate_node(node_element,"integer","1"));
		scnode->append_node(doc.allocate_node(node_element,"key","sms-sound"));
		scnode->append_node(doc.allocate_node(node_element,"integer","1"));
	}


	std::string text;  
	rapidxml::print(std::back_inserter(text), doc, 0);
	text = XML_PLIST_Base + text; 
	CFileOperation::WriteBinFile(_strTempRingSet, text.c_str(), text.length());  //写入.plist文件

	IosPlistConverter::GetPlistFromXml(_strTempRingSet, _strTempRingSet);          //XML转.Plist

	RETURN_FALSE_IF ( !GetDirCmd()->DeleteFileLink(_strIosRingSetList) );

	if (GetUpLoadCmd()->Upload(_strTempRingSet, _strIosRingSetList))			//替换掉IOS原来的RingSetList
	{
		RETURN_FALSE_IF ( !SetIosRingWallerFile() )								//替换掉IOS原来的RingWallerSet.ini
	}

    //NoDeamonInterface::ChangeCallRingtone(_pDevice->GetDevInfo()->_ios_devptr);//有没这句没差别
	DeviceData* pdata = (DeviceData*)_pDevice;
	RETURN_FALSE_IF(!pdata->GetSystemManager()->LogoffDev());
	return true;
}

wstring RingOperationCommand::GetIosRing()
{	
	RETURN_VALUE_IF ( !_pDevice->GetDevInfo()->_ios_bJailBreaked, L"" );

    CFileOperation::DeleteFile(_strTempRingSet);
	GetDownCmd()->Download(_strTempRingSet, _strIosRingSetList);		

	RETURN_VALUE_IF ( !CFileOperation::IsFileExist(_strTempRingSet.c_str()), L"" );
	if ( MyFileOperation::IsIosPlist(_strTempRingSet) )
		IosPlistConverter::GetXmlFromPlist(_strTempRingSet, _strTempRingSet);          //.Plist转XML

	file<> fdoc( CCodeOperation::UnicodeToUTF_8(_strTempRingSet).c_str() );
	xml_document<char> doc;
	doc.parse<0>(fdoc.data());

	std::string strname, strVal;
	wstring strTemp = L"system:Marimba";

	rapidxml::xml_node<char> * root = doc.first_node();            //xml文件的根节点
	rapidxml::xml_node<char> * scnode = root->first_node();        //根节点的子节点
	for (rapidxml::xml_node<char> * listnode = scnode->first_node();
		listnode!=NULL; listnode = listnode->next_sibling())
	{
		strname = listnode->name();
		strVal = listnode->value();
		
		if ( STRKEY == strname && RINGTONE == strVal)
		{
			continue;
		}
		if ( STRING == strname && ((int)strVal.find(STRSYSTEM) >= 0 || (int)strVal.find(ITUNES) >= 0) )
		{
			strTemp = CCodeOperation::GB2312ToUnicode(strVal);
			break;
		}
	}
	return strTemp;	
}

bool RingOperationCommand::SetIosRingWallerFile()
{
	if (CFileOperation::IsFileExist(_strTempRingWallerSet.c_str()))	
        CFileOperation::DeleteFile(_strTempRingWallerSet);

    // 如果设备上存在文件_strIosRingWallerSet则下载
    // 如果设备上不存在文件_strIosRingWallerSet则需要创建（未实现）
	if ( GetDownCmd()->Download(_strTempRingWallerSet,_strIosRingWallerSet) )
	{
		char szDaytime[20] = {0};
		string strDayTime;
		time_t t = time(NULL);

		CDealTime dt(t);
		sprintf(szDaytime,"%d-%d-%d %d:%d:%d",
			dt.GetYear(),
			dt.GetMonth(),
			dt.GetDay(),
			dt.GetHour()+8,
			dt.GetMinute(),
			dt.GetSecond());
		strDayTime = szDaytime;

		//::WritePrivateProfileStringW(L"Info",L"RingHash", CCodeOperation::UTF_8ToUnicode(_ring->_Guid).c_str(), _strTempRingWallerSet.c_str());
		//::WritePrivateProfileStringW(L"Info",L"RingSetTime", strDayTime.c_str(), _strTempRingWallerSet.c_str());
        write_profile_string("Info", "RingHash", _ring->_Guid.c_str(), CCodeOperation::WstringToString(_strTempRingWallerSet).c_str());
        write_profile_string("Info", "RingSetTime", strDayTime.c_str(), CCodeOperation::WstringToString(_strTempRingWallerSet).c_str());
	}

	RETURN_FALSE_IF( ! GetDirCmd()->DeleteFileLink(_strIosRingWallerSet) );
	RETURN_FALSE_IF (!GetUpLoadCmd()->Upload(_strTempRingWallerSet, _strIosRingWallerSet));
	
	return true;
}

bool RingOperationCommand::ReplaceRing()
{
	if (GetDirCmd()->DeleteFileLink(_ring->_remotePath))
	{
		if (GetUpLoadCmd()->Upload(_title, _ring->_remotePath))
		{
			_ring->_size = GetDirCmd()->IosGetFileSize(_ring->_remotePath);
            DeviceData* pdata = (DeviceData*)_pDevice;
            RETURN_FALSE_IF(!pdata->GetSystemManager()->LogoffDev());
			return true;
		}
	}
	return false;
}

bool ND91Assistant::RingOperationCommand::DownLoadRing()
{
	return GetDownCmd()->Download(_ring->_remotePath, _title);
}

DirectoryOperationCommand* RingOperationCommand::GetDirCmd()
{
	if ( ! _pDirCmd )
	{
		_pDirCmd = new DirectoryOperationCommand(_pDevice);
	}
	return _pDirCmd;
}

UploadFileCommand* RingOperationCommand::GetUpLoadCmd()
{
	if ( ! _pUpload )
	{
		_pUpload = new UploadFileCommand(_pDevice);
	}
	return _pUpload;
}

DownloadFileCommand* RingOperationCommand::GetDownCmd()
{
	if ( ! _pDownCmd )
	{
		_pDownCmd = new DownloadFileCommand(_pDevice);
	}
	return _pDownCmd;
}



#endif