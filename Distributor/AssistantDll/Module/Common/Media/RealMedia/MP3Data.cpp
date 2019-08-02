#include "stdafx.h"
#include "MP3Data.h"

MP3Data::MP3Data(wstring strFileFullName)
{
	_strLocalFileName = strFileFullName;		//本地文件全名（含路径）
	_strDevFileName = L"";		//手机文件全名（含路径）

	_nFileSize = 0;				//文件大小
	_nPlayTime = 0;				//时长(ms)
	_nSampleRate = 0;				//采样速率
	_niBitSpeed = 0;				//位速		
	_strVolumeAdjustment = 0;	//音量调整

	_bSelectWorks = false;		//加入选集


	_strID3Ver = L"";				//ID3标记
	_strFormat = L"";				//音频格式
	_strChannel = L"";			//音频通道
	_strMusicName = L"";			//名称
	_strMusicPerformer = L"";		//表演者
	_strAlbumPerformer = L"";		//专辑表演者
	_strComposer = L"";			//作曲者
	_strAlbumName = L"";			//专辑名
	_strLyrics = L"";				//歌词
	_strFrontCover = L"";			//封面
	_strTrackNumber = L"";		//轨道号码
	_strCDNumber = L"";			//光盘号码
	_strYear = L"";				//年份
	_strNotes = L"";				//注释
	_strClassify = L"";			//归类
	_strStyle = L"";				//风格
	_strDescription = L"";		//描述
	_strBPM = L"";				//BPM
	_strNameSort = L"";			//名称排序
	_strAlbumSort = L"";			//专辑排序
	_strPerformerSort = L"";		//表演者排序
	_strAlbumPerformerSort = L"";	//专辑表演者排序
	_strComposerSort = L"";		//作曲者排序
	_strMediaTypes_Prodcast = L"";//媒体种类podcast
	_strMediaTypes_iTunesU = L"";	//媒体种类iTunes U	
    _hMP3Handle = NULL;
	_ArtWork = "";
	//_ArtWork.buff=NULL;
	//_ArtWork.size=0;

	LoadMp3DataInfo(_strLocalFileName);

}

MP3Data::~MP3Data(void)
{
	ND_ReleaseMP3Info(_hMP3Handle);
	//SAFE_DELETE(_ArtWork.buff);
}

bool MP3Data::IsValidMp3(wstring strFileFullName)
{
	RETURN_FALSE_IF(!CFileOperation::IsFileExist(strFileFullName.c_str()));

	return (ND_ValidMP3Cap(CCodeOperation::WstringToString(strFileFullName).c_str()) == /*TRUE*/1)?true:false;
}

void MP3Data::LoadMp3DataInfo(wstring Localpath)
{
	_bIsValid =IsValidMp3(_strLocalFileName);
	if (_bIsValid)
	{
        _hMP3Handle = ND_ParseMP3Info(CCodeOperation::WstringToString(_strLocalFileName).c_str());
		if (_hMP3Handle)
		{
			_ArtWork = GetArtWorkData();
			_nFileSize = CFileOperation::GetFileSizeW(_strLocalFileName);				//文件大小
			_nPlayTime = GetUintByType(ENUM_MP3_INFO_PARA_PLAYTIME);				//时长(ms)
			_nSampleRate = GetUintByType(ENUM_MP3_INFO_PARA_SAMPLERATE);				//采样速率
			_niBitSpeed = GetUintByType(ENUM_MP3_INFO_PARA_BITRATE);				//位速		
			/*UINT*/unsigned int nID3Ver = GetUintByType(ENUM_MP3_INFO_PARA_ID3VER);					//ID3标记
			switch (nID3Ver)
			{
			case ENUM_ID3VER_2_3_0:
				_strID3Ver = L"v2.3";
				break;
			case ENUM_ID3VER_2_4_0:
				_strID3Ver = L"v2.4";
				break;
			default:
				break;
			}
			_strVolumeAdjustment = GetUintByType(ENUM_MP3_INFO_PARA_RVAD);	//音量调整

			wstring strSelectWorks = GetWStringByType(ENUM_MP3_INFO_PARA_TCMP);
			if (strSelectWorks.find(L"1") == 0)
				_bSelectWorks = true;		//加入选集

			_strFormat = GetWStringByType(ENUM_MP3_INFO_PARA_FORMAT);				//音频格式
			_strChannel = GetWStringByType(ENUM_MP3_INFO_PARA_CHANNEL);			//音频通道
			_strMusicName = GetWStringByType(ENUM_MP3_INFO_PARA_TIT2);			//名称
			_strMusicPerformer = GetWStringByType(ENUM_MP3_INFO_PARA_TPE1);		//表演者
			_strAlbumPerformer = GetWStringByType(ENUM_MP3_INFO_PARA_TPE2);		//专辑表演者
			_strComposer = GetWStringByType(ENUM_MP3_INFO_PARA_TCOM);			//作曲者
			_strAlbumName = GetWStringByType(ENUM_MP3_INFO_PARA_TALB);			//专辑名
			_strLyrics = GetWStringByType(ENUM_MP3_INFO_PARA_USLT);				//歌词
			_strFrontCover = GetWStringByType(ENUM_MP3_INFO_PARA_APIC);			//封面
			_strTrackNumber = GetWStringByType(ENUM_MP3_INFO_PARA_TRCK);		//轨道号码
			_strCDNumber = GetWStringByType(ENUM_MP3_INFO_PARA_TPOS);			//光盘号码
			_strYear = GetWStringByType(ENUM_MP3_INFO_PARA_TYER);				//年份
			_strNotes = GetWStringByType(ENUM_MP3_INFO_PARA_COMM);				//注释
			_strClassify = GetWStringByType(ENUM_MP3_INFO_PARA_TIT1);			//归类
			_strStyle = GetWStringByType(ENUM_MP3_INFO_PARA_TCON);				//风格
			_strDescription = GetWStringByType(ENUM_MP3_INFO_PARA_TIT3);		//描述
			_strBPM = GetWStringByType(ENUM_MP3_INFO_PARA_TBPM);				//BPM
			_strNameSort = GetWStringByType(ENUM_MP3_INFO_PARA_TSOT);			//名称排序
			_strAlbumSort = GetWStringByType(ENUM_MP3_INFO_PARA_TSOA);			//专辑排序
			_strPerformerSort = GetWStringByType(ENUM_MP3_INFO_PARA_TSOP);		//表演者排序
			_strAlbumPerformerSort = GetWStringByType(ENUM_MP3_INFO_PARA_TSO2);	//专辑表演者排序
			_strComposerSort = GetWStringByType(ENUM_MP3_INFO_PARA_TSOC);		//作曲者排序
			_strMediaTypes_Prodcast = GetWStringByType(ENUM_MP3_INFO_PARA_PCST);//媒体种类podcast
			_strMediaTypes_iTunesU = GetWStringByType(ENUM_MP3_INFO_PARA_ITNU);	//媒体种类iTunes U	

			string strTempFileFullName = CCodeOperation::WstringToString(Localpath) + CFileOperation::GetGuid() + ".mp3";
			ND_SetMP3Info(_hMP3Handle, ENUM_MP3_INFO_PARA_PATH, (void*)strTempFileFullName.c_str(), strTempFileFullName.length()+1);
		}		
	}

}

/*UINT*/unsigned int MP3Data::GetUintByType(ENUM_MP3_INFO_PARA eType)
{
	if (!_hMP3Handle) return 0;

	/*UINT*/unsigned int nValue = 0;
	HRESULT hr = E_NOTIMPL;
	switch (eType)
	{
	case ENUM_MP3_INFO_PARA_BITRATE://位速
	case ENUM_MP3_INFO_PARA_SAMPLERATE://采样速率
	case ENUM_MP3_INFO_PARA_ID3VER://ID3标记
	case ENUM_MP3_INFO_PARA_PLAYTIME://时长(ms)
	case ENUM_MP3_INFO_PARA_RVAD:						//音量调整
		hr = ND_GetMP3Info(_hMP3Handle, eType, &nValue, sizeof(/*UINT*/unsigned int));
		break;
	}	

	return (hr==sizeof(/*UINT*/unsigned int))?nValue:0;
}
string MP3Data::GetArtWorkData()
{
	if (!_hMP3Handle) return "";
	int BmpLen=0;
	char* szValue = NULL;
	HRESULT hr = E_NOTIMPL;
	//封面	
	hr = ND_GetMP3Info(_hMP3Handle, ENUM_MP3_INFO_PARA_APIC, NULL, -1);	
	if (hr!=E_NOTIMPL && hr!=E_INVALIDARG && hr>0)
	{
		szValue = new char[hr];
		//ZeroMemory(szValue,hr);
		memset(szValue,0,hr);
		ND_GetMP3Info(_hMP3Handle, ENUM_MP3_INFO_PARA_APIC, szValue, hr);	
	}
	else
	{
		return "";
	}
	string retstr(szValue+14,hr-14);
	SAFE_DELETE(szValue);
	return retstr;
}
wstring MP3Data::GetWStringByType(ENUM_MP3_INFO_PARA eType)
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

	if (hr && hr!=E_NOTIMPL && hr!=E_INVALIDARG && hr>0)
	{
		szValue = new char[hr*2];
		//ZeroMemory(szValue, hr);
		memset(szValue,0,sizeof(szValue));
		ND_GetMP3Info(_hMP3Handle, eType, szValue, hr);
		if (eType == ENUM_MP3_INFO_PARA_USLT ||
			eType == ENUM_MP3_INFO_PARA_COMM)
		{
			char* szRealValue = NULL;
			ND_UINT32 nRealSize = 0;
			if (strlen(szValue) > 0 && ND_GetEngString(szValue, hr, (ND_UCHAR**)&szRealValue, &nRealSize) == S_OK)
			{		
				if(szRealValue || szRealValue[nRealSize-1] == 0)
					nRealSize--;
				string strValueGB2312(szRealValue, nRealSize);
                strValue = CCodeOperation::StringToWstring(strValueGB2312);
			}
			free(szRealValue);//szRealValue?
			
		}
		else
		{
			int strlen=0;
			if(szValue[hr-1] == 0)
			{
				strlen =hr-1;
			}
			else
			{
				strlen= hr;
			}
			string strValueGB2312(szValue, strlen);

			strValue = CCodeOperation::StringToWstring(strValueGB2312);
		//
		}	
		delete szValue;
		
	}

	return strValue;
}

ENUM_MP3_INFO_PARA MP3Data::GetAvailableSetMp3InfoPara(eAvailableSetProperty eType)
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

bool MP3Data::ModifyMp3ArtWork(string val)
{
	char temchr[14];
	memset(temchr,0,sizeof(temchr));
	string temp(temchr);
	temp+=val;
	HRESULT hr = ND_SetMP3Info(_hMP3Handle, ENUM_MP3_INFO_PARA_APIC, (void*)temp.c_str(), temp.size());
	RETURN_FALSE_IF(hr != S_OK);
	_ArtWork  = val;
	return true;
}
bool MP3Data::ModifyMp3Info(eAvailableSetProperty eType,wstring val)
{
	ENUM_MP3_INFO_PARA eMP3InfoPara = GetAvailableSetMp3InfoPara(eType);

	string strValue = CCodeOperation::WstringToString(val);

	HRESULT hr = ND_SetMP3Info(_hMP3Handle, eMP3InfoPara, (void*)strValue.c_str(), strValue.length()+1);
	RETURN_FALSE_IF(hr != S_OK);

	switch (eType)
	{
	case eAvailableSetProperty_MusicName:						//名称
		_strMusicName = val;
		break;
	case eAvailableSetProperty_MusicPerformer:					//表演者
		_strMusicPerformer = val;		
		break;
	case eAvailableSetProperty_AlbumPerformer:					//专辑表演者
		_strAlbumPerformer = val;		
		break;
	case eAvailableSetProperty_Composer:							//作曲者
		_strComposer = val;			
		break;
	case eAvailableSetProperty_AlbumName:						//专辑
		_strAlbumName = val;			
		break;
	case eAvailableSetProperty_Lyrics:							//歌词
		_strLyrics = val;				
		break;
	case eAvailableSetProperty_FrontCover:						//封面
		_strFrontCover = val;			
		break;
	case eAvailableSetProperty_TrackNumber:						//轨道号码
		_strTrackNumber = val;		
		break;
	case eAvailableSetProperty_CDNumber:							//光盘号码
		_strCDNumber = val;			
		break;
	case eAvailableSetProperty_Year:								//年份
		_strYear = val;				
		break;
	case eAvailableSetProperty_Notes:							//注释
		_strNotes = val;				
		break;
	case eAvailableSetProperty_Classify:							//归类
		_strClassify = val;			
		break;
	case eAvailableSetProperty_Style:							//风格
		_strStyle = val;				
		break;
	case eAvailableSetProperty_Description:						//描述
		_strDescription = val;		
		break;
	case eAvailableSetProperty_BPM:								//BPM
		_strBPM = val;				
		break;
	case eAvailableSetProperty_SelectWorks:						//加入选集
		if (val == L"1")
			_bSelectWorks = true;	
		else
			_bSelectWorks = false;
		break;
	case eAvailableSetProperty_NameSort:							//名称排序
		_strNameSort = val;			
		break;
	case eAvailableSetProperty_AlbumSort:						//专辑排序
		_strAlbumSort = val;			
		break;
	case eAvailableSetProperty_PerformerSort:					//表演者排序
		_strPerformerSort = val;		
		break;
	case eAvailableSetProperty_AlbumPerformerSort:				//专辑表演者排序
		_strAlbumPerformerSort = val;	
		break;
	case eAvailableSetProperty_ComposerSort:						//作曲者排序
		_strComposerSort = val;		
		break;
	case eAvailableSetProperty_VolumeAdjustment:					//音量调整
		//_strVolumeAdjustment = _wtoi(val.c_str());
		_strVolumeAdjustment = atoi(CCodeOperation::WstringToString(val).c_str());
		break;
	case eAvailableSetProperty_MediaTypes_Prodcast:				//媒体种类podcast
		_strMediaTypes_Prodcast = val;
		break;
	case eAvailableSetProperty_MediaTypes_iTunesU:				//媒体种类iTunes U	
		_strMediaTypes_iTunesU = val;
		break;
	default:
		break;
	}

	return true;
}