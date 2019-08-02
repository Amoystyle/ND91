
#include "MP3Manager.h"
#include "Common/FileOperation.h"
#include "Common/CodeOperation.h"
#include "Common/Path.h"

using namespace ND91Assistant;

MP3Manager::MP3Manager(wstring strFileFullName)
{
	_strFileFullName = strFileFullName;	
	_pMP3Data = NULL;
	_hMP3Handle = NULL;
}

MP3Manager::~MP3Manager(void)
{
	SAFE_DELETE(_pMP3Data);
	ND_ReleaseMP3Info(_hMP3Handle);
	_mapSetInfo.clear();
}

bool MP3Manager::IsValidMp3(wstring strFileFullName)
{
	RETURN_FALSE_IF(!CFileOperation::IsFileExist(strFileFullName.c_str()));

	return (ND_ValidMP3Cap(CCodeOperation::UnicodeToGB2312(strFileFullName).c_str()) == TRUE)?true:false;
}

const MP3Data* MP3Manager::GetMp3DataInfo()
{
	if (!_pMP3Data && IsValidMp3(_strFileFullName))
	{
		_hMP3Handle = ND_ParseMP3Info(CCodeOperation::UnicodeToGB2312(_strFileFullName).c_str());
		if (_hMP3Handle)
		{
			_pMP3Data = new MP3Data();
			_pMP3Data->_strLocalFileName = _strFileFullName;

			_pMP3Data->_nFileSize = CFileOperation::GetFileSizeW(_strFileFullName);				//文件大小
			_pMP3Data->_nPlayTime = GetUintByType(ENUM_MP3_INFO_PARA_PLAYTIME);				//时长(ms)
			_pMP3Data->_nSampleRate = GetUintByType(ENUM_MP3_INFO_PARA_SAMPLERATE);				//采样速率
			_pMP3Data->_niBitSpeed = GetUintByType(ENUM_MP3_INFO_PARA_BITRATE);				//位速		
			UINT nID3Ver = GetUintByType(ENUM_MP3_INFO_PARA_ID3VER);					//ID3标记
			switch (nID3Ver)
			{
			case ENUM_ID3VER_2_3_0:
				_pMP3Data->_strID3Ver = L"v2.3";
				break;
			case ENUM_ID3VER_2_4_0:
				_pMP3Data->_strID3Ver = L"v2.4";
				break;
			default:
				break;
			}
			_pMP3Data->_strVolumeAdjustment = GetUintByType(ENUM_MP3_INFO_PARA_RVAD);	//音量调整

			wstring strSelectWorks = GetWStringByType(ENUM_MP3_INFO_PARA_TCMP);
			if (strSelectWorks.find(L"1") == 0)
				_pMP3Data->_bSelectWorks = true;		//加入选集

			_pMP3Data->_strFormat = GetWStringByType(ENUM_MP3_INFO_PARA_FORMAT);				//音频格式
			_pMP3Data->_strChannel = GetWStringByType(ENUM_MP3_INFO_PARA_CHANNEL);			//音频通道
			_pMP3Data->_strMusicName = GetWStringByType(ENUM_MP3_INFO_PARA_TIT2);			//名称
			_pMP3Data->_strMusicPerformer = GetWStringByType(ENUM_MP3_INFO_PARA_TPE1);		//表演者
			_pMP3Data->_strAlbumPerformer = GetWStringByType(ENUM_MP3_INFO_PARA_TPE2);		//专辑表演者
			_pMP3Data->_strComposer = GetWStringByType(ENUM_MP3_INFO_PARA_TCOM);			//作曲者
			_pMP3Data->_strAlbumName = GetWStringByType(ENUM_MP3_INFO_PARA_TALB);			//专辑名
			_pMP3Data->_strLyrics = GetWStringByType(ENUM_MP3_INFO_PARA_USLT);				//歌词
			_pMP3Data->_strFrontCover = GetWStringByType(ENUM_MP3_INFO_PARA_APIC);			//封面
			_pMP3Data->_strTrackNumber = GetWStringByType(ENUM_MP3_INFO_PARA_TRCK);		//轨道号码
			_pMP3Data->_strCDNumber = GetWStringByType(ENUM_MP3_INFO_PARA_TPOS);			//光盘号码
			_pMP3Data->_strYear = GetWStringByType(ENUM_MP3_INFO_PARA_TYER);				//年份
			_pMP3Data->_strNotes = GetWStringByType(ENUM_MP3_INFO_PARA_COMM);				//注释
			_pMP3Data->_strClassify = GetWStringByType(ENUM_MP3_INFO_PARA_TIT1);			//归类
			_pMP3Data->_strStyle = GetWStringByType(ENUM_MP3_INFO_PARA_TCON);				//风格
			_pMP3Data->_strDescription = GetWStringByType(ENUM_MP3_INFO_PARA_TIT3);		//描述
			_pMP3Data->_strBPM = GetWStringByType(ENUM_MP3_INFO_PARA_TBPM);				//BPM
			_pMP3Data->_strNameSort = GetWStringByType(ENUM_MP3_INFO_PARA_TSOT);			//名称排序
			_pMP3Data->_strAlbumSort = GetWStringByType(ENUM_MP3_INFO_PARA_TSOA);			//专辑排序
			_pMP3Data->_strPerformerSort = GetWStringByType(ENUM_MP3_INFO_PARA_TSOP);		//表演者排序
			_pMP3Data->_strAlbumPerformerSort = GetWStringByType(ENUM_MP3_INFO_PARA_TSO2);	//专辑表演者排序
			_pMP3Data->_strComposerSort = GetWStringByType(ENUM_MP3_INFO_PARA_TSOC);		//作曲者排序
			_pMP3Data->_strMediaTypes_Prodcast = GetWStringByType(ENUM_MP3_INFO_PARA_PCST);//媒体种类podcast
			_pMP3Data->_strMediaTypes_iTunesU = GetWStringByType(ENUM_MP3_INFO_PARA_ITNU);	//媒体种类iTunes U	

			string strTempFileFullName = CCodeOperation::UnicodeToGB2312(Path::GlobalTemp()) + CFileOperation::GetGuid() + ".mp3";
			ND_SetMP3Info(_hMP3Handle, ENUM_MP3_INFO_PARA_PATH, (void*)strTempFileFullName.c_str(), strTempFileFullName.length()+1);
		}		
	}

	return _pMP3Data;
}

UINT MP3Manager::GetUintByType(ENUM_MP3_INFO_PARA eType)
{
	if (!_hMP3Handle) return 0;

	UINT nValue = 0;
	HRESULT hr = E_NOTIMPL;
	switch (eType)
	{
	case ENUM_MP3_INFO_PARA_BITRATE://位速
	case ENUM_MP3_INFO_PARA_SAMPLERATE://采样速率
	case ENUM_MP3_INFO_PARA_ID3VER://ID3标记
	case ENUM_MP3_INFO_PARA_PLAYTIME://时长(ms)
	case ENUM_MP3_INFO_PARA_RVAD:						//音量调整
		hr = ND_GetMP3Info(_hMP3Handle, eType, &nValue, sizeof(UINT));
		break;
	}	

	return (hr==sizeof(UINT))?nValue:0;
}

wstring MP3Manager::GetWStringByType(ENUM_MP3_INFO_PARA eType)
{
	if (!_hMP3Handle) return L"";

	wstring strValue = L"";
	char* szValue = NULL;
	HRESULT hr = E_NOTIMPL;
	switch (eType)
	{
	case ENUM_MP3_INFO_PARA_FORMAT:						//音频格式
	case ENUM_MP3_INFO_PARA_CHANNEL:					//通道
	case ENUM_MP3_INFO_PARA_TIT2:						//名称
	case ENUM_MP3_INFO_PARA_TPE1:						//表演者
	case ENUM_MP3_INFO_PARA_TPE2:						//专辑表演者
	case ENUM_MP3_INFO_PARA_TCOM:						//作曲者
	case ENUM_MP3_INFO_PARA_TALB:						//专辑
	case ENUM_MP3_INFO_PARA_USLT:						//歌词
	case ENUM_MP3_INFO_PARA_APIC:						//封面
	case ENUM_MP3_INFO_PARA_TRCK:						//轨道号码
	case ENUM_MP3_INFO_PARA_TPOS:						//光盘号码
	case ENUM_MP3_INFO_PARA_TYER:						//年份
	case ENUM_MP3_INFO_PARA_COMM:						//注释
	case ENUM_MP3_INFO_PARA_TIT1:						//归类
	case ENUM_MP3_INFO_PARA_TCON:						//风格
	case ENUM_MP3_INFO_PARA_TIT3:						//描述
	case ENUM_MP3_INFO_PARA_TBPM:						//BPM
	case ENUM_MP3_INFO_PARA_TCMP:						//加入选集
	case ENUM_MP3_INFO_PARA_TSOT:						//名称排序
	case ENUM_MP3_INFO_PARA_TSOA:						//专辑排序
	case ENUM_MP3_INFO_PARA_TSOP:						//表演者排序
	case ENUM_MP3_INFO_PARA_TSO2:						//专辑表演者排序
	case ENUM_MP3_INFO_PARA_TSOC:						//作曲者排序
	case ENUM_MP3_INFO_PARA_PCST:						//媒体种类podcast
	case ENUM_MP3_INFO_PARA_ITNU:						//媒体种类iTunes U	
		hr = ND_GetMP3Info(_hMP3Handle, eType, NULL, -1);
		break;
	}

	if (hr!=E_NOTIMPL && hr!=E_INVALIDARG && hr>0)
	{
		szValue = new char[hr];
		ZeroMemory(szValue, hr);
		ND_GetMP3Info(_hMP3Handle, eType, szValue, hr);
		if (eType == ENUM_MP3_INFO_PARA_USLT ||
			eType == ENUM_MP3_INFO_PARA_COMM)
		{
			char* szRealValue = NULL;
			ND_UINT32 nRealSize = 0;
			if (ND_GetEngString(szValue, hr, (ND_UCHAR**)&szRealValue, &nRealSize) == S_OK)
			{
				string strValueGB2312(szRealValue, nRealSize);
				strValue = CCodeOperation::GB2312ToUnicode(strValueGB2312);
			}
			free(szRealValue);
		}
		else
		{
			string strValueGB2312(szValue, hr);
			strValue = CCodeOperation::GB2312ToUnicode(strValueGB2312);
		}		
	}

	return strValue;
}

void MP3Manager::ModifyProperty(eAvailableSetProperty eProperty, wstring strNewValue)
{
	_mapSetInfo.insert(pair<eAvailableSetProperty, wstring>(eProperty, strNewValue));
}

void MP3Manager::CancelModify()
{
	_mapSetInfo.clear();
}

bool MP3Manager::DoModify()
{
	RETURN_FALSE_IF(!_hMP3Handle);

	map<eAvailableSetProperty, wstring>::iterator it;
	for (it=_mapSetInfo.begin(); it!=_mapSetInfo.end(); it++)
	{
		ENUM_MP3_INFO_PARA eMP3InfoPara = GetAvailableSetMp3InfoPara(it->first);
		string strValue = CCodeOperation::UnicodeToGB2312(it->second);

		HRESULT hr = ND_SetMP3Info(_hMP3Handle, eMP3InfoPara, (void*)strValue.c_str(), strValue.length()+1);
		RETURN_FALSE_IF(hr != S_OK);
	}

	if (ND_WriteMP3Info(_hMP3Handle) == S_OK)
	{
		for (it=_mapSetInfo.begin(); it!=_mapSetInfo.end(); it++)
		{
			SetAvailableSetMp3Info(it->first, it->second);
		}

		return true;
	}

	return false;
}

ENUM_MP3_INFO_PARA MP3Manager::GetAvailableSetMp3InfoPara(eAvailableSetProperty eType)
{
	ENUM_MP3_INFO_PARA AvailableSetMp3InfoParas[] = {
		ENUM_MP3_INFO_PARA_TIT2,						//名称
		ENUM_MP3_INFO_PARA_TPE1,						//表演者
		ENUM_MP3_INFO_PARA_TPE2,						//专辑表演者
		ENUM_MP3_INFO_PARA_TCOM,						//作曲者
		ENUM_MP3_INFO_PARA_TALB,						//专辑
		ENUM_MP3_INFO_PARA_USLT,						//歌词
		ENUM_MP3_INFO_PARA_APIC,						//封面
		ENUM_MP3_INFO_PARA_TRCK,						//轨道号码
		ENUM_MP3_INFO_PARA_TPOS,						//光盘号码
		ENUM_MP3_INFO_PARA_TYER,						//年份
		ENUM_MP3_INFO_PARA_COMM,						//注释
		ENUM_MP3_INFO_PARA_TIT1,						//归类
		ENUM_MP3_INFO_PARA_TCON,						//风格
		ENUM_MP3_INFO_PARA_TIT3,						//描述
		ENUM_MP3_INFO_PARA_TBPM,						//BPM
		ENUM_MP3_INFO_PARA_TCMP,						//加入选集
		ENUM_MP3_INFO_PARA_TSOT,						//名称排序
		ENUM_MP3_INFO_PARA_TSOA,						//专辑排序
		ENUM_MP3_INFO_PARA_TSOP,						//表演者排序
		ENUM_MP3_INFO_PARA_TSO2,						//专辑表演者排序
		ENUM_MP3_INFO_PARA_TSOC,						//作曲者排序
		ENUM_MP3_INFO_PARA_RVAD,						//音量调整
		ENUM_MP3_INFO_PARA_PCST,						//媒体种类podcast
		ENUM_MP3_INFO_PARA_ITNU,						//媒体种类iTunes U	
	};

	return AvailableSetMp3InfoParas[eType];
}

void MP3Manager::SetAvailableSetMp3Info(eAvailableSetProperty eType, wstring strValue)
{
	switch (eType)
	{
	case eAvailableSetProperty_MusicName:						//名称
		_pMP3Data->_strMusicName = strValue;			
		break;
	case eAvailableSetProperty_MusicPerformer:					//表演者
		_pMP3Data->_strMusicPerformer = strValue;		
		break;
	case eAvailableSetProperty_AlbumPerformer:					//专辑表演者
		_pMP3Data->_strAlbumPerformer = strValue;		
		break;
	case eAvailableSetProperty_Composer:							//作曲者
		_pMP3Data->_strComposer = strValue;			
		break;
	case eAvailableSetProperty_AlbumName:						//专辑
		_pMP3Data->_strAlbumName = strValue;			
		break;
	case eAvailableSetProperty_Lyrics:							//歌词
		_pMP3Data->_strLyrics = strValue;				
		break;
	case eAvailableSetProperty_FrontCover:						//封面
		_pMP3Data->_strFrontCover = strValue;			
		break;
	case eAvailableSetProperty_TrackNumber:						//轨道号码
		_pMP3Data->_strTrackNumber = strValue;		
		break;
	case eAvailableSetProperty_CDNumber:							//光盘号码
		_pMP3Data->_strCDNumber = strValue;			
		break;
	case eAvailableSetProperty_Year:								//年份
		_pMP3Data->_strYear = strValue;				
		break;
	case eAvailableSetProperty_Notes:							//注释
		_pMP3Data->_strNotes = strValue;				
		break;
	case eAvailableSetProperty_Classify:							//归类
		_pMP3Data->_strClassify = strValue;			
		break;
	case eAvailableSetProperty_Style:							//风格
		_pMP3Data->_strStyle = strValue;				
		break;
	case eAvailableSetProperty_Description:						//描述
		_pMP3Data->_strDescription = strValue;		
		break;
	case eAvailableSetProperty_BPM:								//BPM
		_pMP3Data->_strBPM = strValue;				
		break;
	case eAvailableSetProperty_SelectWorks:						//加入选集
		if (strValue == L"1")
			_pMP3Data->_bSelectWorks = true;		
		break;
	case eAvailableSetProperty_NameSort:							//名称排序
		_pMP3Data->_strNameSort = strValue;			
		break;
	case eAvailableSetProperty_AlbumSort:						//专辑排序
		_pMP3Data->_strAlbumSort = strValue;			
		break;
	case eAvailableSetProperty_PerformerSort:					//表演者排序
		_pMP3Data->_strPerformerSort = strValue;		
		break;
	case eAvailableSetProperty_AlbumPerformerSort:				//专辑表演者排序
		_pMP3Data->_strAlbumPerformerSort = strValue;	
		break;
	case eAvailableSetProperty_ComposerSort:						//作曲者排序
		_pMP3Data->_strComposerSort = strValue;		
		break;
	case eAvailableSetProperty_VolumeAdjustment:					//音量调整
		_pMP3Data->_strVolumeAdjustment = _wtoi(strValue.c_str());	
		break;
	case eAvailableSetProperty_MediaTypes_Prodcast:				//媒体种类podcast
		_pMP3Data->_strMediaTypes_Prodcast = strValue;
		break;
	case eAvailableSetProperty_MediaTypes_iTunesU:				//媒体种类iTunes U	
		_pMP3Data->_strMediaTypes_iTunesU = strValue;
		break;
	default:
		break;
	}
}