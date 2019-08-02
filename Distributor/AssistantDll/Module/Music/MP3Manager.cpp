
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

			_pMP3Data->_nFileSize = CFileOperation::GetFileSizeW(_strFileFullName);				//�ļ���С
			_pMP3Data->_nPlayTime = GetUintByType(ENUM_MP3_INFO_PARA_PLAYTIME);				//ʱ��(ms)
			_pMP3Data->_nSampleRate = GetUintByType(ENUM_MP3_INFO_PARA_SAMPLERATE);				//��������
			_pMP3Data->_niBitSpeed = GetUintByType(ENUM_MP3_INFO_PARA_BITRATE);				//λ��		
			UINT nID3Ver = GetUintByType(ENUM_MP3_INFO_PARA_ID3VER);					//ID3���
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
			_pMP3Data->_strVolumeAdjustment = GetUintByType(ENUM_MP3_INFO_PARA_RVAD);	//��������

			wstring strSelectWorks = GetWStringByType(ENUM_MP3_INFO_PARA_TCMP);
			if (strSelectWorks.find(L"1") == 0)
				_pMP3Data->_bSelectWorks = true;		//����ѡ��

			_pMP3Data->_strFormat = GetWStringByType(ENUM_MP3_INFO_PARA_FORMAT);				//��Ƶ��ʽ
			_pMP3Data->_strChannel = GetWStringByType(ENUM_MP3_INFO_PARA_CHANNEL);			//��Ƶͨ��
			_pMP3Data->_strMusicName = GetWStringByType(ENUM_MP3_INFO_PARA_TIT2);			//����
			_pMP3Data->_strMusicPerformer = GetWStringByType(ENUM_MP3_INFO_PARA_TPE1);		//������
			_pMP3Data->_strAlbumPerformer = GetWStringByType(ENUM_MP3_INFO_PARA_TPE2);		//ר��������
			_pMP3Data->_strComposer = GetWStringByType(ENUM_MP3_INFO_PARA_TCOM);			//������
			_pMP3Data->_strAlbumName = GetWStringByType(ENUM_MP3_INFO_PARA_TALB);			//ר����
			_pMP3Data->_strLyrics = GetWStringByType(ENUM_MP3_INFO_PARA_USLT);				//���
			_pMP3Data->_strFrontCover = GetWStringByType(ENUM_MP3_INFO_PARA_APIC);			//����
			_pMP3Data->_strTrackNumber = GetWStringByType(ENUM_MP3_INFO_PARA_TRCK);		//�������
			_pMP3Data->_strCDNumber = GetWStringByType(ENUM_MP3_INFO_PARA_TPOS);			//���̺���
			_pMP3Data->_strYear = GetWStringByType(ENUM_MP3_INFO_PARA_TYER);				//���
			_pMP3Data->_strNotes = GetWStringByType(ENUM_MP3_INFO_PARA_COMM);				//ע��
			_pMP3Data->_strClassify = GetWStringByType(ENUM_MP3_INFO_PARA_TIT1);			//����
			_pMP3Data->_strStyle = GetWStringByType(ENUM_MP3_INFO_PARA_TCON);				//���
			_pMP3Data->_strDescription = GetWStringByType(ENUM_MP3_INFO_PARA_TIT3);		//����
			_pMP3Data->_strBPM = GetWStringByType(ENUM_MP3_INFO_PARA_TBPM);				//BPM
			_pMP3Data->_strNameSort = GetWStringByType(ENUM_MP3_INFO_PARA_TSOT);			//��������
			_pMP3Data->_strAlbumSort = GetWStringByType(ENUM_MP3_INFO_PARA_TSOA);			//ר������
			_pMP3Data->_strPerformerSort = GetWStringByType(ENUM_MP3_INFO_PARA_TSOP);		//����������
			_pMP3Data->_strAlbumPerformerSort = GetWStringByType(ENUM_MP3_INFO_PARA_TSO2);	//ר������������
			_pMP3Data->_strComposerSort = GetWStringByType(ENUM_MP3_INFO_PARA_TSOC);		//����������
			_pMP3Data->_strMediaTypes_Prodcast = GetWStringByType(ENUM_MP3_INFO_PARA_PCST);//ý������podcast
			_pMP3Data->_strMediaTypes_iTunesU = GetWStringByType(ENUM_MP3_INFO_PARA_ITNU);	//ý������iTunes U	

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
	case ENUM_MP3_INFO_PARA_BITRATE://λ��
	case ENUM_MP3_INFO_PARA_SAMPLERATE://��������
	case ENUM_MP3_INFO_PARA_ID3VER://ID3���
	case ENUM_MP3_INFO_PARA_PLAYTIME://ʱ��(ms)
	case ENUM_MP3_INFO_PARA_RVAD:						//��������
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

void MP3Manager::SetAvailableSetMp3Info(eAvailableSetProperty eType, wstring strValue)
{
	switch (eType)
	{
	case eAvailableSetProperty_MusicName:						//����
		_pMP3Data->_strMusicName = strValue;			
		break;
	case eAvailableSetProperty_MusicPerformer:					//������
		_pMP3Data->_strMusicPerformer = strValue;		
		break;
	case eAvailableSetProperty_AlbumPerformer:					//ר��������
		_pMP3Data->_strAlbumPerformer = strValue;		
		break;
	case eAvailableSetProperty_Composer:							//������
		_pMP3Data->_strComposer = strValue;			
		break;
	case eAvailableSetProperty_AlbumName:						//ר��
		_pMP3Data->_strAlbumName = strValue;			
		break;
	case eAvailableSetProperty_Lyrics:							//���
		_pMP3Data->_strLyrics = strValue;				
		break;
	case eAvailableSetProperty_FrontCover:						//����
		_pMP3Data->_strFrontCover = strValue;			
		break;
	case eAvailableSetProperty_TrackNumber:						//�������
		_pMP3Data->_strTrackNumber = strValue;		
		break;
	case eAvailableSetProperty_CDNumber:							//���̺���
		_pMP3Data->_strCDNumber = strValue;			
		break;
	case eAvailableSetProperty_Year:								//���
		_pMP3Data->_strYear = strValue;				
		break;
	case eAvailableSetProperty_Notes:							//ע��
		_pMP3Data->_strNotes = strValue;				
		break;
	case eAvailableSetProperty_Classify:							//����
		_pMP3Data->_strClassify = strValue;			
		break;
	case eAvailableSetProperty_Style:							//���
		_pMP3Data->_strStyle = strValue;				
		break;
	case eAvailableSetProperty_Description:						//����
		_pMP3Data->_strDescription = strValue;		
		break;
	case eAvailableSetProperty_BPM:								//BPM
		_pMP3Data->_strBPM = strValue;				
		break;
	case eAvailableSetProperty_SelectWorks:						//����ѡ��
		if (strValue == L"1")
			_pMP3Data->_bSelectWorks = true;		
		break;
	case eAvailableSetProperty_NameSort:							//��������
		_pMP3Data->_strNameSort = strValue;			
		break;
	case eAvailableSetProperty_AlbumSort:						//ר������
		_pMP3Data->_strAlbumSort = strValue;			
		break;
	case eAvailableSetProperty_PerformerSort:					//����������
		_pMP3Data->_strPerformerSort = strValue;		
		break;
	case eAvailableSetProperty_AlbumPerformerSort:				//ר������������
		_pMP3Data->_strAlbumPerformerSort = strValue;	
		break;
	case eAvailableSetProperty_ComposerSort:						//����������
		_pMP3Data->_strComposerSort = strValue;		
		break;
	case eAvailableSetProperty_VolumeAdjustment:					//��������
		_pMP3Data->_strVolumeAdjustment = _wtoi(strValue.c_str());	
		break;
	case eAvailableSetProperty_MediaTypes_Prodcast:				//ý������podcast
		_pMP3Data->_strMediaTypes_Prodcast = strValue;
		break;
	case eAvailableSetProperty_MediaTypes_iTunesU:				//ý������iTunes U	
		_pMP3Data->_strMediaTypes_iTunesU = strValue;
		break;
	default:
		break;
	}
}