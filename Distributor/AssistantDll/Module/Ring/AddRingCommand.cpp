#include "pch_module.h"

#ifdef MODULE_RING

#include "Module/Ring/AddRingCommand.h"
#include "Module/Ring/AndroidRingData.h"
#include "Module/Ring/IosUserRingData.h"

#include "Module/Common/Media/MediaBase.h"
#include "Module/Common/Media/RealMedia/MP3Data.h"
#include "Module/Common/Media/RealMedia/WmvParser.h"
#include "Module/Common/Media/RealMedia/Mp4Parser.h"
#include "Module/Common/Media/RealMedia/OggParser.h"
#include "Common/Path.h"
#include "Module/AndroidDefine.h"

#define FIRSTRING "0001 .m4r"
AddRingCommand::AddRingCommand(const DeviceData* pDevice)
:BaseCommand(pDevice)
{
	_retRing = NULL;
	_strTempRingList = _pDevice->GetIosRingPath()->GetLocalRingListPath();

	//android
	_nAndroidChannelID = MEDIA;
	_nAndroidCommandID = ADD_RING;


	_success = false;
}

AddRingCommand::~AddRingCommand(void)
{
	SAFE_DELETE(_retRing);
}

void ND91Assistant::AddRingCommand::SetParam( int kind, wstring localpath, wstring remotepath,MediaFileType type, wstring tittle )
{
	_kind		= kind;
	_localpath  = localpath;
	_remotepath	= remotepath;
	_title		= tittle;
	_type		= type;
	if(_title == L"")
		_title = CFileOperation::GetFileNameNoExt(_remotepath);
}

void ND91Assistant::AddRingCommand::Execute()
{
	SAFE_DELETE(_retRing);
	if ( _pDevice->GetDevInfo()->_deviceType == Ios)
	{
		wstring strIosRingList = _pDevice->GetIosRingPath()->GetIosRingListPath();   
		//铃声的名字
		wstring strRingname = CFileOperation::GetFileNameNoExt(_localpath);

		if(_localpath.length()<1 || _remotepath.length()<1)
			return;

		DirectoryOperationCommand	dirCmd(_pDevice);
		DownloadFileCommand			downCmd(_pDevice);
		UploadFileCommand			uploadCmd(_pDevice);

		if (!CFileOperation::IsFileExist(_strTempRingList.c_str()))
		{
			if ( !dirCmd.Exist(strIosRingList) )
			{
				RETURN_IF ( !CFileOperation::CreateEmptyFile(_strTempRingList.c_str()) );
			
				string XmlValue = "<plist version=\"1.0\"> \n <dict> \n <key>Ringtones</key> \n <dict/> \n </dict> \n </plist>";	
				fstream fs(CCodeOperation::WstringToString(_strTempRingList).c_str(), ios::in|ios::out|ios::binary);
				fs.write(XmlValue.c_str(), XmlValue.length());
				fs.close();
			}
			else
			{
				RETURN_IF ( !downCmd.Download(_strTempRingList, strIosRingList) );
			}
		} 
		RETURN_IF ( !CreateRingtonesList(CCodeOperation::UnicodeToUTF_8(strRingname)) );         //创建铃声列表XML文件
		
		IosPlistConverter::GetPlistFromXml(_strTempRingList, _strTempRingList);          //XML转.Plist
		RETURN_IF ( !dirCmd.DeleteFileLink(strIosRingList) );
		RETURN_IF ( !uploadCmd.Upload(_strTempRingList, strIosRingList) );   //上传铃声XML列表

		_success = uploadCmd.Upload(_localpath, _retRing->_remotePath);

// 		int nVersion = IosPublicFunc::GetVerNum(_pDevice->GetDevInfo()->_deviceProductVersion);
// 		if ( nVersion < 400 )
// 		{
// 			DeviceData* pdata = (DeviceData*)_pDevice;
// 			RETURN_IF (!pdata->GetSystemManager()->LogoffDev());
// 		}


	}
	else if (_pDevice->GetDevInfo()->_deviceType == Android)
	{
		DefaultExecute();
	}
}

std::string ND91Assistant::AddRingCommand::GetAndroidBodyData()
{
	BinaryBufferWriter  writer;

	writer.WriteInt(_nAndroidChannelID);
	writer.WriteChar((char)_nAndroidCommandID);
	writer.WriteString(CCodeOperation::UnicodeToUTF_8(_remotepath));
	writer.WriteString(CCodeOperation::UnicodeToUTF_8(_title));//tittle
	writer.WriteString("");//Artist,传空
	writer.WriteString("");//Album, 传空

	writer.WriteString("");//专辑封面,传空

	writer.WriteInt64(CFileOperation::GetFileSizeW(_localpath));    // filesize
	writer.WriteString(GetMimeType(_localpath)); // mime

	writer.WriteInt(GetDuration()*1000);    //duration时长
	writer.WriteChar(0);   //TrackNumber
	writer.WriteString("");//年份,传空
	writer.WriteInt(_kind);//类型

	return writer.GetBuffer();

}

void ND91Assistant::AddRingCommand::SetAndroidReturnData( std::string data )
{
     BinaryBufferReader reader(data.c_str(), data.length());

	 if(!parseAndroidPackageHead(&reader))
		 return;

	 if (reader.GetOffset() >= (int)data.length())
		 return;

	 if((int)data.find("not connected to MediaScannerService")>-1)
		 return;

	 _retRing = new AndroidRingData();
	 dynamic_cast<AndroidRingData*>(_retRing)->ParseAndroidData(reader);
}

int AddRingCommand::GetDuration()
{
	int duration = 0;
	MediaBase* base = NULL;
	switch(_type)
	{
	case MEDIA_AUDIO_M4A:
		base = new Mp4Parser(_localpath);
		break;
	case MEDIA_AUDIO_MP3:
		{
			MP3Data* data = new MP3Data(_localpath);
			duration = (int)(data->GetMediaDuration());
			SAFE_DELETE(data);
		}
		break;
	case MEDIA_AUDIO_OGG:
		base = new OggParser(_localpath);
		break;
	case MEDIA_AUDIO_WMA:
		base = new WmvParser(_localpath);
		break;
	default:
		break;
	}
	if(base)
		duration = base->GetMediaDuration();
	SAFE_DELETE(base);
	return duration;
}

 string AddRingCommand::GetMimeType(const wstring path)
{
	wstring format=L"";
	int pos = path.find_last_of(L".");
	if(pos>0)
		format = CStrOperation::toLowerW(path.substr(pos+1, path.length()-pos-1));
	if(format.empty())
	{
		switch(_type)
		{
		case MEDIA_AUDIO_M4A:
			return "audio/mp4";
		case MEDIA_AUDIO_MP3:
			return "audio/mpeg";
		case MEDIA_AUDIO_OGG:
			return "application/ogg";
		case MEDIA_AUDIO_WMA:
			return "audio/x-ms-wma";
		default:
			return "audio/mp3";
		}
	}
	if (format==L"mp3")
		return "audio/mpeg";
	else if (format==L"m4a")
		return "audio/mp4";
	else if (format==L"wma")
		return "audio/x-ms-wma";
	else if (format==L"ogg")
		return "application/ogg";
	else if (format==L"wav")
		return "audio/x-wav";
	else if (format==L"amr")
		return "audio/amr";
	else if (format==L"wma")
		return "audio/x-ms-wma";
	else
		return "audio/mp3";
	/*else if (format==L"mp4" || format==L"m4v")
		return "video/mp4";
	else if (format==L"3gp" || format==L"3gpp")
		return "video/3gpp";
	else if (format==L"3g2" || format==L"3gpp2")
		return "video/3gpp2";
	else if (format==L"wmv")
		return "video/x-ms-wmv";*/
}



bool ND91Assistant::AddRingCommand::CreateRingtonesList(std::string ringname)
{
	std::string lastKey, title;
	char szL[6] = {0};

	RETURN_FALSE_IF ( ringname.empty() );
	title = _title.empty() ? ringname : CCodeOperation::UnicodeToUTF_8(_title);

	RETURN_FALSE_IF ( !CFileOperation::IsFileExist(_strTempRingList.c_str()) );
	if ( MyFileOperation::IsIosPlist(_strTempRingList) )
		IosPlistConverter::GetXmlFromPlist(_strTempRingList, _strTempRingList);          //.Plist转XML

	file<> fdoc( CCodeOperation::UnicodeToUTF_8(_strTempRingList).c_str() );
	xml_document<char> doc;
	doc.parse<0>(fdoc.data());

	std::set<int> keys;
	set<int>::reverse_iterator it;
	keys.clear();

	rapidxml::xml_node<char> * root = doc.first_node();            //xml文件的根节点
	rapidxml::xml_node<char> * scnode = root->first_node();        //根节点的子节点
	rapidxml::xml_node<char> * chnode = scnode->last_node();       //子节点最后一个节点
	for (rapidxml::xml_node<char> * node = chnode->first_node(); node!=NULL; node = node->next_sibling())
	{
		std::string strname = node->name();
		if ( STRKEY == strname )
		{                               
			keys.insert( atoi(node->value()) );		//获取最大一个key的值
		}
	}

	if ( keys.empty() )
	{
		lastKey = FIRSTRING;
	}
	else
	{
		it = keys.rbegin();
		sprintf(szL, "%04d", *it+1);
		lastKey = szL;
		lastKey = lastKey + " .m4r";									//数据的处理
	}



	std::string guid = CCodeOperation::GB2312ToUTF_8(CFileOperation::GetGuid());

	_retRing = new IosUserRingData;
	_retRing->_Guid			= guid;
	_retRing->_name			= CCodeOperation::UTF_8ToUnicode(lastKey);
	_retRing->_remotePath	= _pDevice->GetIosRingPath()->GetIosUserRingPath()+_retRing->_name;
	_retRing->_size			= CFileOperation::GetFileSizeW(_localpath);								//RingData数据打包
	_retRing->_title		= CCodeOperation::UTF_8ToUnicode(title);
	_retRing->_duration		= 0;

	xml_node<>* key = doc.allocate_node(node_element,"key",lastKey.c_str());
	chnode->append_node(key); 
	xml_node<>* dict = doc.allocate_node(node_element,"dict",NULL);
	dict->append_node(doc.allocate_node(node_element,"key","GUID"));
	dict->append_node(doc.allocate_node(node_element,"string",guid.c_str()));  //插入XML节点
	dict->append_node(doc.allocate_node(node_element,"key","Name"));
	dict->append_node(doc.allocate_node(node_element,"string",title.c_str()));
	chnode->append_node(dict);                                                

	std::string text;  
	rapidxml::print(std::back_inserter(text), doc, 0);
	text = XML_PLIST_Base + text; 
	RETURN_FALSE_IF ( !CFileOperation::WriteBinFile(_strTempRingList, text.c_str(), text.length()) );  //写入.plist文件

	return true;

}

bool ND91Assistant::AddRingCommand::parseAndroidPackageHead( BinaryBufferReader* pReader )
{
	pReader->ReadBuffer(_pReturnHead, 4);
	pReader->ReadShort(_nReturnFlag);
	pReader->ReadShort(_nReturnLongConnectSign);
	pReader->ReadInt32(_nReturnBodySize);
	pReader->ReadInt32(_nReturnChannelID);
	if(_nReturnChannelID != MEDIA )
		return false;
	return true;
}


#endif