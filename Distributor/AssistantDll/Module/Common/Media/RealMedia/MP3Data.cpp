#include "stdafx.h"
#include "MP3Data.h"

MP3Data::MP3Data(wstring strFileFullName)
{
	_strLocalFileName = strFileFullName;		//�����ļ�ȫ������·����
	_strDevFileName = L"";		//�ֻ��ļ�ȫ������·����

	_nFileSize = 0;				//�ļ���С
	_nPlayTime = 0;				//ʱ��(ms)
	_nSampleRate = 0;				//��������
	_niBitSpeed = 0;				//λ��		
	_strVolumeAdjustment = 0;	//��������

	_bSelectWorks = false;		//����ѡ��


	_strID3Ver = L"";				//ID3���
	_strFormat = L"";				//��Ƶ��ʽ
	_strChannel = L"";			//��Ƶͨ��
	_strMusicName = L"";			//����
	_strMusicPerformer = L"";		//������
	_strAlbumPerformer = L"";		//ר��������
	_strComposer = L"";			//������
	_strAlbumName = L"";			//ר����
	_strLyrics = L"";				//���
	_strFrontCover = L"";			//����
	_strTrackNumber = L"";		//�������
	_strCDNumber = L"";			//���̺���
	_strYear = L"";				//���
	_strNotes = L"";				//ע��
	_strClassify = L"";			//����
	_strStyle = L"";				//���
	_strDescription = L"";		//����
	_strBPM = L"";				//BPM
	_strNameSort = L"";			//��������
	_strAlbumSort = L"";			//ר������
	_strPerformerSort = L"";		//����������
	_strAlbumPerformerSort = L"";	//ר������������
	_strComposerSort = L"";		//����������
	_strMediaTypes_Prodcast = L"";//ý������podcast
	_strMediaTypes_iTunesU = L"";	//ý������iTunes U	
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
			_nFileSize = CFileOperation::GetFileSizeW(_strLocalFileName);				//�ļ���С
			_nPlayTime = GetUintByType(ENUM_MP3_INFO_PARA_PLAYTIME);				//ʱ��(ms)
			_nSampleRate = GetUintByType(ENUM_MP3_INFO_PARA_SAMPLERATE);				//��������
			_niBitSpeed = GetUintByType(ENUM_MP3_INFO_PARA_BITRATE);				//λ��		
			/*UINT*/unsigned int nID3Ver = GetUintByType(ENUM_MP3_INFO_PARA_ID3VER);					//ID3���
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
			_strVolumeAdjustment = GetUintByType(ENUM_MP3_INFO_PARA_RVAD);	//��������

			wstring strSelectWorks = GetWStringByType(ENUM_MP3_INFO_PARA_TCMP);
			if (strSelectWorks.find(L"1") == 0)
				_bSelectWorks = true;		//����ѡ��

			_strFormat = GetWStringByType(ENUM_MP3_INFO_PARA_FORMAT);				//��Ƶ��ʽ
			_strChannel = GetWStringByType(ENUM_MP3_INFO_PARA_CHANNEL);			//��Ƶͨ��
			_strMusicName = GetWStringByType(ENUM_MP3_INFO_PARA_TIT2);			//����
			_strMusicPerformer = GetWStringByType(ENUM_MP3_INFO_PARA_TPE1);		//������
			_strAlbumPerformer = GetWStringByType(ENUM_MP3_INFO_PARA_TPE2);		//ר��������
			_strComposer = GetWStringByType(ENUM_MP3_INFO_PARA_TCOM);			//������
			_strAlbumName = GetWStringByType(ENUM_MP3_INFO_PARA_TALB);			//ר����
			_strLyrics = GetWStringByType(ENUM_MP3_INFO_PARA_USLT);				//���
			_strFrontCover = GetWStringByType(ENUM_MP3_INFO_PARA_APIC);			//����
			_strTrackNumber = GetWStringByType(ENUM_MP3_INFO_PARA_TRCK);		//�������
			_strCDNumber = GetWStringByType(ENUM_MP3_INFO_PARA_TPOS);			//���̺���
			_strYear = GetWStringByType(ENUM_MP3_INFO_PARA_TYER);				//���
			_strNotes = GetWStringByType(ENUM_MP3_INFO_PARA_COMM);				//ע��
			_strClassify = GetWStringByType(ENUM_MP3_INFO_PARA_TIT1);			//����
			_strStyle = GetWStringByType(ENUM_MP3_INFO_PARA_TCON);				//���
			_strDescription = GetWStringByType(ENUM_MP3_INFO_PARA_TIT3);		//����
			_strBPM = GetWStringByType(ENUM_MP3_INFO_PARA_TBPM);				//BPM
			_strNameSort = GetWStringByType(ENUM_MP3_INFO_PARA_TSOT);			//��������
			_strAlbumSort = GetWStringByType(ENUM_MP3_INFO_PARA_TSOA);			//ר������
			_strPerformerSort = GetWStringByType(ENUM_MP3_INFO_PARA_TSOP);		//����������
			_strAlbumPerformerSort = GetWStringByType(ENUM_MP3_INFO_PARA_TSO2);	//ר������������
			_strComposerSort = GetWStringByType(ENUM_MP3_INFO_PARA_TSOC);		//����������
			_strMediaTypes_Prodcast = GetWStringByType(ENUM_MP3_INFO_PARA_PCST);//ý������podcast
			_strMediaTypes_iTunesU = GetWStringByType(ENUM_MP3_INFO_PARA_ITNU);	//ý������iTunes U	

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
	case ENUM_MP3_INFO_PARA_BITRATE://λ��
	case ENUM_MP3_INFO_PARA_SAMPLERATE://��������
	case ENUM_MP3_INFO_PARA_ID3VER://ID3���
	case ENUM_MP3_INFO_PARA_PLAYTIME://ʱ��(ms)
	case ENUM_MP3_INFO_PARA_RVAD:						//��������
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
	//����	
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
	case ENUM_MP3_INFO_PARA_FORMAT:						//��Ƶ��ʽ
	case ENUM_MP3_INFO_PARA_CHANNEL:					//ͨ��
	case ENUM_MP3_INFO_PARA_TIT2:						//����
	case ENUM_MP3_INFO_PARA_TPE1:						//������
	case ENUM_MP3_INFO_PARA_TPE2:						//ר��������
	case ENUM_MP3_INFO_PARA_TCOM:						//������
	case ENUM_MP3_INFO_PARA_TALB:						//ר��
	case ENUM_MP3_INFO_PARA_USLT:						//���
	case ENUM_MP3_INFO_PARA_APIC:						//����
	case ENUM_MP3_INFO_PARA_TRCK:						//�������
	case ENUM_MP3_INFO_PARA_TPOS:						//���̺���
	case ENUM_MP3_INFO_PARA_TYER:						//���
	case ENUM_MP3_INFO_PARA_COMM:						//ע��
	case ENUM_MP3_INFO_PARA_TIT1:						//����
	case ENUM_MP3_INFO_PARA_TCON:						//���
	case ENUM_MP3_INFO_PARA_TIT3:						//����
	case ENUM_MP3_INFO_PARA_TBPM:						//BPM
	case ENUM_MP3_INFO_PARA_TCMP:						//����ѡ��
	case ENUM_MP3_INFO_PARA_TSOT:						//��������
	case ENUM_MP3_INFO_PARA_TSOA:						//ר������
	case ENUM_MP3_INFO_PARA_TSOP:						//����������
	case ENUM_MP3_INFO_PARA_TSO2:						//ר������������
	case ENUM_MP3_INFO_PARA_TSOC:						//����������
	case ENUM_MP3_INFO_PARA_PCST:						//ý������podcast
	case ENUM_MP3_INFO_PARA_ITNU:						//ý������iTunes U	
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
		ENUM_MP3_INFO_PARA_TIT2,						//����
		ENUM_MP3_INFO_PARA_TPE1,						//������
		ENUM_MP3_INFO_PARA_TPE2,						//ר��������
		ENUM_MP3_INFO_PARA_TCOM,						//������
		ENUM_MP3_INFO_PARA_TALB,						//ר��
		ENUM_MP3_INFO_PARA_USLT,						//���
		ENUM_MP3_INFO_PARA_APIC,						//����
		ENUM_MP3_INFO_PARA_TRCK,						//�������
		ENUM_MP3_INFO_PARA_TPOS,						//���̺���
		ENUM_MP3_INFO_PARA_TYER,						//���
		ENUM_MP3_INFO_PARA_COMM,						//ע��
		ENUM_MP3_INFO_PARA_TIT1,						//����
		ENUM_MP3_INFO_PARA_TCON,						//���
		ENUM_MP3_INFO_PARA_TIT3,						//����
		ENUM_MP3_INFO_PARA_TBPM,						//BPM
		ENUM_MP3_INFO_PARA_TCMP,						//����ѡ��
		ENUM_MP3_INFO_PARA_TSOT,						//��������
		ENUM_MP3_INFO_PARA_TSOA,						//ר������
		ENUM_MP3_INFO_PARA_TSOP,						//����������
		ENUM_MP3_INFO_PARA_TSO2,						//ר������������
		ENUM_MP3_INFO_PARA_TSOC,						//����������
		ENUM_MP3_INFO_PARA_RVAD,						//��������
		ENUM_MP3_INFO_PARA_PCST,						//ý������podcast
		ENUM_MP3_INFO_PARA_ITNU,						//ý������iTunes U	
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
	case eAvailableSetProperty_MusicName:						//����
		_strMusicName = val;
		break;
	case eAvailableSetProperty_MusicPerformer:					//������
		_strMusicPerformer = val;		
		break;
	case eAvailableSetProperty_AlbumPerformer:					//ר��������
		_strAlbumPerformer = val;		
		break;
	case eAvailableSetProperty_Composer:							//������
		_strComposer = val;			
		break;
	case eAvailableSetProperty_AlbumName:						//ר��
		_strAlbumName = val;			
		break;
	case eAvailableSetProperty_Lyrics:							//���
		_strLyrics = val;				
		break;
	case eAvailableSetProperty_FrontCover:						//����
		_strFrontCover = val;			
		break;
	case eAvailableSetProperty_TrackNumber:						//�������
		_strTrackNumber = val;		
		break;
	case eAvailableSetProperty_CDNumber:							//���̺���
		_strCDNumber = val;			
		break;
	case eAvailableSetProperty_Year:								//���
		_strYear = val;				
		break;
	case eAvailableSetProperty_Notes:							//ע��
		_strNotes = val;				
		break;
	case eAvailableSetProperty_Classify:							//����
		_strClassify = val;			
		break;
	case eAvailableSetProperty_Style:							//���
		_strStyle = val;				
		break;
	case eAvailableSetProperty_Description:						//����
		_strDescription = val;		
		break;
	case eAvailableSetProperty_BPM:								//BPM
		_strBPM = val;				
		break;
	case eAvailableSetProperty_SelectWorks:						//����ѡ��
		if (val == L"1")
			_bSelectWorks = true;	
		else
			_bSelectWorks = false;
		break;
	case eAvailableSetProperty_NameSort:							//��������
		_strNameSort = val;			
		break;
	case eAvailableSetProperty_AlbumSort:						//ר������
		_strAlbumSort = val;			
		break;
	case eAvailableSetProperty_PerformerSort:					//����������
		_strPerformerSort = val;		
		break;
	case eAvailableSetProperty_AlbumPerformerSort:				//ר������������
		_strAlbumPerformerSort = val;	
		break;
	case eAvailableSetProperty_ComposerSort:						//����������
		_strComposerSort = val;		
		break;
	case eAvailableSetProperty_VolumeAdjustment:					//��������
		//_strVolumeAdjustment = _wtoi(val.c_str());
		_strVolumeAdjustment = atoi(CCodeOperation::WstringToString(val).c_str());
		break;
	case eAvailableSetProperty_MediaTypes_Prodcast:				//ý������podcast
		_strMediaTypes_Prodcast = val;
		break;
	case eAvailableSetProperty_MediaTypes_iTunesU:				//ý������iTunes U	
		_strMediaTypes_iTunesU = val;
		break;
	default:
		break;
	}

	return true;
}