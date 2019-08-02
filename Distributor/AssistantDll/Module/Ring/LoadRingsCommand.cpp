#include "pch_module.h"

#ifdef MODULE_RING

#include "Module/Ring/LoadRingsCommand.h"

#include "Module/Ring/AndroidRingData.h"
#include "Module/Ring/IosUserRingData.h"
#include "Module/Ring/IosSystemRingData.h"
#include "Core/International.h"
#include "Common/Path.h"
#include "Module/AndroidDefine.h"

#define RINGPOSTFIX L".caf"
LoadRingsCommand::LoadRingsCommand(const DeviceData* pDevice)
:TableQueryCommand(pDevice)
{
	_strIosRingList		= _pDevice->GetIosRingPath()->GetIosRingListPath();
	_strTempRingList	= _pDevice->GetIosRingPath()->GetLocalRingListPath();

	_strIosUserRing		= _pDevice->GetIosRingPath()->GetIosUserRingPath();

	// android
	_nAndroidChannelID = MEDIA;

	_success = false;

    // 如果不存在目录，创建各种存放临时数据的目录
    RETURN_IF(!CFileOperation::MakeLocalDir(_pDevice->GetPath()->GetWorkFolder()));
}

LoadRingsCommand::~LoadRingsCommand(void)
{
	for (deque<RingData*>::iterator it = _Ringlist.begin(); it != _Ringlist.end(); it++)
	{
		RingData* pData = *it;
		SAFE_DELETE(pData);
	}
	_Ringlist.clear();
}


#define LOAD_RING_COLUMNS "_id,_data,title,duration,artist,album,is_ringtone,is_music,is_alarm,is_notification,_size"

void ND91Assistant::LoadRingsCommand::SetAudioType( AudioType type )
{
	_audioType = type;

	if(_pDevice->GetDevInfo()->_deviceType != Android)
		return;
	string tablename;
	switch(_audioType)
	{
	case SYSTEM_RING:
		tablename = "sysaudio";
		break;
	case CUSTOM_RING:
		tablename = "cusaudio";
		break;
	}

	SetParam(tablename, LOAD_RING_COLUMNS, "", "title");
}


void ND91Assistant::LoadRingsCommand::Execute()
{

	if ( _pDevice->GetDevInfo()->_deviceType == Ios)
	{
		if (_audioType == SYSTEM_RING)
		{
			_success = GetSystemRingList();
		}
		else if (_audioType == CUSTOM_RING)
		{
			LoadRingsByPath(_strIosRingList, _strTempRingList);
		}
	}
	else if (_pDevice->GetDevInfo()->_deviceType == Android)
	{
		DefaultExecute();
	}
}


void ND91Assistant::LoadRingsCommand::SetAndroidReturnData( std::string data )
{
	BinaryBufferReader  reader(data.c_str(), data.length());
	if (!parseAndroidPackageHead(&reader))
		return;
	int count = 0;
	reader.ReadInt32(count);
	for(int i = 0; i < count; i++)
	{

		AndroidRingData* ring = new AndroidRingData();
		ring->ParseAndroidData(reader);
		if(SYSTEM_RING == _audioType)
			ring->_IsSystemRing = true;
		else
		{
			//型号为qsd8250_surf的机子可为music
			string strModelList(RINGASMUSIC_MODEL);
			if((int)strModelList.find(CCodeOperation::UnicodeToGB2312(_pDevice->GetDevInfo()->_deviceHWModel)) >= 0 && ring->_ringType == RingData::Music)
			{
				delete ring;
				continue;
			}
		}
		_Ringlist.push_back(ring);
	}
}

int ND91Assistant::LoadRingsCommand::GetRingCount()
{
	std::string strname;
	int tag = 0;

	if (!CFileOperation::IsFileExist(_strTempRingList.c_str()))
	{
		DownloadFileCommand downCmd(_pDevice);
		downCmd.Download(_strTempRingList, _strIosRingList);
	}

	RETURN_VALUE_IF ( !CFileOperation::IsFileExist(_strTempRingList.c_str()), 0 );
	if ( MyFileOperation::IsIosPlist(_strTempRingList) )
		IosPlistConverter::GetXmlFromPlist(_strTempRingList, _strTempRingList);          //.Plist转XML

	file<> fdoc( CCodeOperation::UnicodeToUTF_8(_strTempRingList).c_str() );
	xml_document<char> doc;
	doc.parse<0>(fdoc.data());
	RETURN_FALSE_IF(fdoc.data() == NULL);

	rapidxml::xml_node<char> * root = doc.first_node();            //xml文件的根节点
	rapidxml::xml_node<char> * scnode = root->first_node();        //根节点的子节点
	rapidxml::xml_node<char> * chnode = scnode->last_node();       //子节点最后一个节点
	for (rapidxml::xml_node<char> * listnode = chnode->first_node(); listnode!=NULL; listnode = listnode->next_sibling())
	{
		strname = listnode->name();
		if ( STRKEY == strname )
		{
			++tag;								//获取实际存放铃声文件的名字
		}			
	}
	return tag + 28;							//系统铃声暂时默认总数为 28
}

bool ND91Assistant::LoadRingsCommand::GetRingtonesList(wstring filePath)
{
	std::string strVal, strGuid;
	wstring strTitle;
	int tag = 0;
	std::vector<string> PlistUserRing;     //用户自定义铃声文件名

	RETURN_VALUE_IF ( !CFileOperation::IsFileExist(filePath.c_str()), 0 );
	if ( MyFileOperation::IsIosPlist(filePath) )
		IosPlistConverter::GetXmlFromPlist(filePath, filePath);          //.Plist转XML

	file<> fdoc( CCodeOperation::UnicodeToUTF_8(filePath).c_str() );
	xml_document<char> doc;
	doc.parse<0>(fdoc.data());
	RETURN_FALSE_IF(fdoc.data() == NULL);

	rapidxml::xml_node<char> * root = doc.first_node();            //xml文件的根节点
	RETURN_FALSE_IF (!root);
	rapidxml::xml_node<char> * scnode = root->first_node();        //根节点的子节点
	RETURN_FALSE_IF (!scnode);
	rapidxml::xml_node<char> * chnode = scnode->first_node();       //子节点最后一个节点
	RETURN_FALSE_IF ( !chnode );
	std::string name = chnode->name();
	std::string val = chnode->value();
	RETURN_FALSE_IF ( STRKEY != name || CStrOperation::toLower(val) != "ringtones" )
	
	chnode = chnode->next_sibling();
	RETURN_FALSE_IF( !chnode );
	for (rapidxml::xml_node<char> * listnode = chnode->first_node(); listnode!=NULL; listnode = listnode->next_sibling())
	{
		if ( !listnode ) continue;
		
		std::string strname = listnode->name();
		if ( STRKEY == strname )
		{
			PlistUserRing.push_back(listnode->value());                             //获取实际存放铃声文件的名字
		}			
		if ( DICT == strname )
		{
			for (rapidxml::xml_node<char> * node = listnode->first_node(); node!=NULL; node = node->next_sibling())
			{
				if ( !node ) continue;

				std::string strfile = node->name();
				if ( STRING == strfile)
				{
					switch(tag)
					{
					case 1: 
						{ 
							strVal = node->value();
							strGuid = strVal;				        //获取铃声文件的唯一ID
						}
						break;
					case 3:
						{
							strVal = node->value();
							strTitle = CCodeOperation::UTF_8ToUnicode(strVal.c_str());			//获取Ringtones.plist的铃声名字				
						}
						break;
					}				
				}
				tag++;
 			}
			
			RETURN_FALSE_IF( 0 == PlistUserRing.size() );

			DirectoryOperationCommand dirCmd(_pDevice);

			IosUserRingData* ring = new IosUserRingData();
			wstring strRingName = CCodeOperation::UTF_8ToUnicode(PlistUserRing.at(PlistUserRing.size()-1));
			ring->_Guid			= strGuid;
			ring->_name			= strRingName;
			ring->_title		= strTitle;
			ring->_remotePath	= _strIosUserRing + strRingName;
			ring->_size			= dirCmd.IosGetFileSize(ring->_remotePath);
			ring->_IsSystemRing = false;
			ring->_duration		= 0;

			_Ringlist.push_back(ring);
			tag = 0;
		}
	}
	PlistUserRing.clear();

	return true;
}

bool ND91Assistant::LoadRingsCommand::GetSystemRingList()
{
	std::map<wstring, wstring> ringNameTitle;
	ringNameTitle[L"lock.caf"]			= International::GetString(PHONE_LOCK_RING_NAME    );
	ringNameTitle[L"unlock.caf"]		= International::GetString(PHONE_UNLOCK_RING_NAME  );
	ringNameTitle[L"bee-bee.caf"]		= International::GetString(CHARGING_RING_NAME      );
	ringNameTitle[L"low_power.caf"]		= International::GetString(LOW_ENERGY_RING_NAME    );
	ringNameTitle[L"alarm.caf"]			= International::GetString(CALENDER_ALARM_RING_NAME);
	ringNameTitle[L"Tock.caf"]			= International::GetString(KEY_PRESS_RING_NAME     );
	ringNameTitle[L"photoShutter.caf"]	= International::GetString(TAKE_PHOTO_RING_NAME    );
	RingListToRingData(&ringNameTitle, RingData::Alarm);

	ringNameTitle.clear();
	ringNameTitle[L"sms-received1.caf"]	= International::GetString(ALTERNATE_1);
	ringNameTitle[L"sms-received2.caf"]	= International::GetString(ALTERNATE_2);
	ringNameTitle[L"sms-received3.caf"]	= International::GetString(ALTERNATE_3);
	ringNameTitle[L"sms-received4.caf"]	= International::GetString(ALTERNATE_4);
	ringNameTitle[L"sms-received5.caf"]	= International::GetString(ALTERNATE_5);
	ringNameTitle[L"sms-received6.caf"]	= International::GetString(ALTERNATE_6);
	RingListToRingData(&ringNameTitle, RingData::Mail);

	ringNameTitle.clear();
	ringNameTitle[L"dtmf-0.caf"]		= International::GetString(DIAL_0_RING_NAME);				         
	ringNameTitle[L"dtmf-1.caf"]		= International::GetString(DIAL_1_RING_NAME);				        
	ringNameTitle[L"dtmf-2.caf"]		= International::GetString(DIAL_2_RING_NAME);				         
	ringNameTitle[L"dtmf-3.caf"]		= International::GetString(DIAL_3_RING_NAME);				        
	ringNameTitle[L"dtmf-4.caf"]		= International::GetString(DIAL_4_RING_NAME);				         
	ringNameTitle[L"dtmf-5.caf"]		= International::GetString(DIAL_5_RING_NAME);				        
	ringNameTitle[L"dtmf-6.caf"]		= International::GetString(DIAL_6_RING_NAME);				         
	ringNameTitle[L"dtmf-7.caf"]		= International::GetString(DIAL_7_RING_NAME);				        
	ringNameTitle[L"dtmf-8.caf"]		= International::GetString(DIAL_8_RING_NAME);				         
	ringNameTitle[L"dtmf-9.caf"]		= International::GetString(DIAL_9_RING_NAME);				        
	ringNameTitle[L"dtmf-pound.caf"]	= International::GetString(DIAL_POUND_RING_NAME);				         
	ringNameTitle[L"dtmf-star.caf"]		= International::GetString(DIAL_STAR_RING_NAME);				        
	ringNameTitle[L"Voicemail.caf"]		= International::GetString(VOICEMAIL_RING_NAME);				         
	RingListToRingData(&ringNameTitle, RingData::Calendar);

	ringNameTitle.clear();
	ringNameTitle[L"new-mail.caf"]		= International::GetString(NEW_MAIL_RING_NAME);				        
	ringNameTitle[L"mail-sent.caf"]		= International::GetString(SEND_MAIL_RING_NAME);				    
	RingListToRingData(&ringNameTitle, RingData::Mail);

	return true;

}

void ND91Assistant::LoadRingsCommand::RingListToRingData(const map<wstring, wstring>* mapList, RingData::MediaFileKind kind)
{
	DirectoryOperationCommand dirCmd(_pDevice);

	map<wstring, wstring>::const_iterator it;
	for (it=mapList->begin();it!=mapList->end();it++)
	{
		IosSystemRingData* ring = new IosSystemRingData();
		ring->_name			= it->first;
		ring->_title		= it->second;                               //加载IOS系统铃声数据
		ring->_IsSystemRing = true;
		ring->_ringType		= kind;
		ring->_remotePath	= _pDevice->GetIosRingPath()->GetIosSyetemRingPath() + ring->_name;
		ring->_size			= dirCmd.IosGetFileSize(ring->_remotePath);
		ring->_duration		= 0;
		_Ringlist.push_back(ring);
	}
}

void ND91Assistant::LoadRingsCommand::LoadRingsByPath( wstring remotefilePath, wstring localpath)
{
	wstring path = localpath.empty() ? _pDevice->GetIosRingPath()->GetLocalRingWorkPath()+CFileOperation::GetFileName(remotefilePath)
									 : localpath;

	if (CFileOperation::IsFileExist(path.c_str()))
	{	
        CFileOperation::DeleteFile(path);
	}
	DownloadFileCommand downCmd(_pDevice);
	if(downCmd.Download(path, remotefilePath))
	{
		GetRingtonesList(path);
	}
}

const deque<RingData*>* ND91Assistant::LoadRingsCommand::LoadAlarmRings()
{
	LoadRingsByPath(L"/var/mobile/Media/iTunes_Control/iTunes/Ringtones.plist");
	LoadRingsByPath(L"/var/root/Media/iTunes_Control/iTunes/Ringtones.plist");
	DirData* dir = new DirData(_pDevice);
	dir->_name = CStrOperation::trimRightW(_pDevice->GetIosRingPath()->GetIosRingLibraryFolder(), L"/");
	deque<NodeData*> nodelist = dir->GetChildrens();
	for (size_t i = 0; i < nodelist.size(); i++)
	{
		if(nodelist[i]->_type == NodeData::FILE)
		{
			FileData* ringfile = dynamic_cast<FileData*>(nodelist[i]);
			int pos = ringfile->_name.find_last_of(L".");
			wstring ext = pos > -1?ringfile->_name.substr(pos):L"";
			ext = CStrOperation::toLowerW(ext);
			if(ext == L".m4r"|| ext == L"m4a" || ext == L"mp3")
			{
				IosSystemRingData* ring = new IosSystemRingData();
				ring->_title		= ringfile->_name.substr(0, pos);
				ring->_IsSystemRing = true;
				ring->_Guid		= CCodeOperation::UnicodeToUTF_8(L"system:"+ring->_title);
				ring->_remotePath	= ringfile->GetFullPath();
				_Ringlist.push_back(ring);
			}
		}
	}
	SAFE_DELETE(dir);
	return &_Ringlist;
}


#endif