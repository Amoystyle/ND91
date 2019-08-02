#pragma once
#include "AssistantDll.h"
#include "Module/Common/Media/IMediaParser.h"
#include "Common/CodeOperation.h"
#include "Common/StrOperation.h"
#include "MP3Decoder.h"

namespace ND91Assistant
{
	//id3��ǩ����
	typedef enum eAvailableSetProperty
	{
		eAvailableSetProperty_MusicName,						//����
		eAvailableSetProperty_MusicPerformer,					//������
		eAvailableSetProperty_AlbumPerformer,					//ר��������
		eAvailableSetProperty_Composer,							//������
		eAvailableSetProperty_AlbumName,						//ר��
		eAvailableSetProperty_Lyrics,							//���
		eAvailableSetProperty_FrontCover,						//����
		eAvailableSetProperty_TrackNumber,						//�������
		eAvailableSetProperty_CDNumber,							//���̺���
		eAvailableSetProperty_Year,								//���
		eAvailableSetProperty_Notes,							//ע��
		eAvailableSetProperty_Classify,							//����
		eAvailableSetProperty_Style,							//���
		eAvailableSetProperty_Description,						//����
		eAvailableSetProperty_BPM,								//BPM
		eAvailableSetProperty_SelectWorks,						//����ѡ��
		eAvailableSetProperty_NameSort,							//��������
		eAvailableSetProperty_AlbumSort,						//ר������
		eAvailableSetProperty_PerformerSort,					//����������
		eAvailableSetProperty_AlbumPerformerSort,				//ר������������
		eAvailableSetProperty_ComposerSort,						//����������
		eAvailableSetProperty_VolumeAdjustment,					//��������
		eAvailableSetProperty_MediaTypes_Prodcast,				//ý������podcast
		eAvailableSetProperty_MediaTypes_iTunesU,				//ý������iTunes U	
	};		

	class MP3Data  : public IMediaParser
	{
	public:
		MP3Data(wstring strFileFullName);
		~MP3Data(void);

		// ��ȡý���ļ�����ʱ��
		virtual long GetMediaDuration()	{ return _nPlayTime/1000; } 

	public:


		//************************************
		// Method:    IsValidMp3 ���ж��Ƿ��ǿ�ʶ���MP3�ļ�
		// Access:    public static 
		// Returns:   bool �����򷵻�TRUE�����򷵻�FALSE������FALSE�򲻿����ô˹�������в���
		// Qualifier:
		// Parameter: wstring strFileFullName ��MP3�ļ��ı����ļ�������·����
		//************************************
		static bool IsValidMp3(wstring strFileFullName);

		//��ʶ���ļ�
		bool IsValid(){return _bIsValid;}
		//�豸�ϵ��ļ���
		std::wstring GetDevFileName() const { return _strDevFileName; }
		void SetDevFileName(std::wstring val) { _strDevFileName = val; }
		//�����ļ���
		std::wstring GetLocalFileName() const { return _strLocalFileName; }

		//Mp3�ļ�ֻ������
		/*UINT*/unsigned int GetFileSize() const { return _nFileSize; }
		/*UINT*/unsigned int GetPlayTime() const { return _nPlayTime; }
		/*UINT*/unsigned int GetiBitSpeed() const { return _niBitSpeed; }
		/*UINT*/unsigned int GetSampleRate() const { return _nSampleRate; }
		/*UINT*/unsigned int GetVolumeAdjustment() const { return _strVolumeAdjustment; }
		bool GetSelectWorks() const { return _bSelectWorks; }
		std::wstring GetID3Ver() const { return _strID3Ver; }
		std::wstring GetChannel() const { return _strChannel; }
		std::wstring GetFormat() const { return _strFormat; }
		

		//Mp3��Ϣ���Զ�д
		std::wstring PropertyGetMusicName() const { return CCodeOperation::StrFilter(_strMusicName); }
		bool PropertySetMusicName(std::wstring val) { return ModifyMp3Info(eAvailableSetProperty_MusicName,val);}

		std::wstring PropertyGetMusicPerformer() const { return CCodeOperation::StrFilter(_strMusicPerformer); }
		bool PropertySetMusicPerformer(std::wstring val) {return ModifyMp3Info(eAvailableSetProperty_MusicPerformer,val); }

		std::wstring PropertyGetAlbumPerformer() const { return CCodeOperation::StrFilter(_strAlbumPerformer); }
		bool PropertySetAlbumPerformer(std::wstring val) {  return ModifyMp3Info(eAvailableSetProperty_AlbumPerformer,val);}

		std::wstring PropertyGetComposer() const { return CCodeOperation::StrFilter(_strComposer); }
		bool PropertySetComposer(std::wstring val) { return  ModifyMp3Info(eAvailableSetProperty_Composer,val);}

		std::wstring PropertyGetAlbumName() const { return CCodeOperation::StrFilter(_strAlbumName); }
		bool PropertySetAlbumName(std::wstring val) { return  ModifyMp3Info(eAvailableSetProperty_AlbumName,val);}

		std::wstring PropertyGetLyrics() const { return CCodeOperation::StrFilter(_strLyrics); }
		bool PropertySetLyrics(std::wstring val) { return ModifyMp3Info(eAvailableSetProperty_Lyrics,val); }

		std::wstring PropertyGetFrontCover() const { return CCodeOperation::StrFilter(_strFrontCover); }
		bool PropertySetFrontCover(std::wstring val) { return ModifyMp3Info(eAvailableSetProperty_FrontCover,val); }

		std::wstring PropertyGetTrackNumber() const { return _strTrackNumber; }
		bool PropertySetTrackNumber(std::wstring val) { return  ModifyMp3Info(eAvailableSetProperty_TrackNumber,val);}

		std::wstring PropertyGetCDNumber() const { return _strCDNumber; }
		bool PropertySetCDNumber(std::wstring val) { return ModifyMp3Info(eAvailableSetProperty_CDNumber,val);}

		std::wstring PropertyGetYear() const { return CCodeOperation::StrFilter(_strYear); }
		bool PropertySetYear(std::wstring val) {  return ModifyMp3Info(eAvailableSetProperty_Year,val);}

		std::wstring PropertyGetNotes() const { return CCodeOperation::StrFilter(_strNotes); }
		bool PropertySetNotes(std::wstring val) { return ModifyMp3Info(eAvailableSetProperty_Notes,val);}

		std::wstring PropertyGetClassify() const { return _strClassify; }
		bool PropertySetClassify(std::wstring val) { return ModifyMp3Info(eAvailableSetProperty_Classify,val);}

		std::wstring PropertyGetStyle() const { return _strStyle; }
		bool PropertySetStyle(std::wstring val) { return  ModifyMp3Info(eAvailableSetProperty_Style,val);}

		std::wstring PropertyGetDescription() const { return CCodeOperation::StrFilter(_strDescription); }
		bool PropertySetDescription(std::wstring val) {  return ModifyMp3Info(eAvailableSetProperty_Description,val);}

		std::wstring PropertyGetBPM() const { return _strBPM; }
		bool PropertySetBPM(std::wstring val) { return ModifyMp3Info(eAvailableSetProperty_BPM,val); }

		std::wstring PropertyGetNameSort() const { return CCodeOperation::StrFilter(_strNameSort); }
		bool PropertySetNameSort(std::wstring val) { return ModifyMp3Info(eAvailableSetProperty_NameSort,val); }

		std::wstring PropertyGetAlbumSort() const { return CCodeOperation::StrFilter(_strAlbumSort); }
		bool PropertySetAlbumSort(std::wstring val) { return ModifyMp3Info(eAvailableSetProperty_AlbumSort,val); }

		std::wstring PropertyGetPerformerSort() const { return CCodeOperation::StrFilter(_strPerformerSort); }
		bool PropertySetPerformerSort(std::wstring val) { return  ModifyMp3Info(eAvailableSetProperty_PerformerSort,val);}

		std::wstring PropertyGetAlbumPerformerSort() const { return CCodeOperation::StrFilter(_strAlbumPerformerSort); }
		bool PropertySetAlbumPerformerSort(std::wstring val) { return  ModifyMp3Info(eAvailableSetProperty_AlbumPerformerSort,val);}

		std::wstring PropertyGetComposerSort() const { return CCodeOperation::StrFilter(_strComposerSort); }
		bool PropertySetComposerSort(std::wstring val) { return ModifyMp3Info(eAvailableSetProperty_ComposerSort,val); }

		std::wstring PropertyGetMediaTypes_Prodcast() const { return _strMediaTypes_Prodcast; }
		bool PropertySetMediaTypes_Prodcast(std::wstring val) {  return ModifyMp3Info(eAvailableSetProperty_MediaTypes_Prodcast,val);}

		std::wstring PropertyGetMediaTypes_iTunesU() const { return _strMediaTypes_iTunesU; }
		bool PropertySetMediaTypes_iTunesU(std::wstring val) { return ModifyMp3Info(eAvailableSetProperty_MediaTypes_iTunesU,val); }

		/*UINT*/unsigned int PropertyGetVolumeAdjustment() const { return _strVolumeAdjustment; }
		bool PropertySetVolumeAdjustment(/*UINT*/unsigned int val) { return ModifyMp3Info(eAvailableSetProperty_VolumeAdjustment,CStrOperation::IntToWString(val)); }

		bool PropertyGetSelectWorks() const { return _bSelectWorks; }
		bool PropertySetSelectWorks(bool val) {return ModifyMp3Info(eAvailableSetProperty_VolumeAdjustment,val==true?L"1":L"0"); }

		std::string PropertyGetArtWork() const { return _ArtWork; }
		bool PropertySetArtWork(std::string val) { return ModifyMp3ArtWork(val); }

	private:
		//************************************
		// Method:    ����Mp3������Ϣ
		// Returns:   void
		//************************************
		void LoadMp3DataInfo(wstring Localpath);

		//************************************
		// Method:    �༭mp3����
		// Returns:   void
		// Parameter: eAvailableSetProperty PropertyType  ����ID
		// Parameter: wstring val ����ֵ
		//************************************
		bool ModifyMp3Info(eAvailableSetProperty PropertyType,wstring val);

		//************************************
		// Method:    �����������ͻ��MP3���Է���ֵ
		// Returns:   UINT ����ֵ
		// Parameter: ENUM_MP3_INFO_PARA eType ����
		//************************************
		/*UINT*/unsigned int GetUintByType(ENUM_MP3_INFO_PARA eType);

		//************************************
		// Method:    �����������ͻ��MP3���Է���ֵ
		// Returns:   std::wstring ����ֵ
		// Parameter: ENUM_MP3_INFO_PARA eType ����
		//************************************
		wstring GetWStringByType(ENUM_MP3_INFO_PARA eType);

		//************************************
		// Method:    ������Ա�ǩ
		// Returns:   ENUM_MP3_INFO_PARA ��ǩ��
		// Parameter: eAvailableSetProperty eType �õ��ı�ǩ
		//************************************
		ENUM_MP3_INFO_PARA GetAvailableSetMp3InfoPara(eAvailableSetProperty eType);

		//���mp3����ͼƬ����		
		string GetArtWorkData();

		//����MP3����ͼƬ��Ϣ
		bool ModifyMp3ArtWork(string val);

	private:
		wstring _strLocalFileName;		//�����ļ�ȫ������·����
		wstring _strDevFileName;		//�ֻ��ļ�ȫ������·����
		
		bool _bIsValid;					//�Ƿ���Ч��mp3�ļ�
		/*UINT*/unsigned int _nFileSize;				//�ļ���С(��λ�ֽ�)
		/*UINT*/unsigned int _nPlayTime;				//ʱ��(��λms)		
		/*UINT*/unsigned int _nSampleRate;				//�������ʣ���λHz��		
		/*UINT*/unsigned int _niBitSpeed;				//λ��(BPS)		
		
			
		wstring _strID3Ver;				//ID3���		
		wstring _strFormat;				//��Ƶ��ʽ		
		wstring _strChannel;			//��Ƶͨ��		

		bool _bSelectWorks;		//�Ƿ����ѡ��	
		
		/*UINT*/unsigned int _strVolumeAdjustment;	//��������

		wstring _strMusicName;			//����
		wstring _strMusicPerformer;		//������		
		wstring _strAlbumPerformer;		//ר��������		
		wstring _strComposer;			//������		
		wstring _strAlbumName;			//ר����		
		wstring _strLyrics;				//���		
		wstring _strFrontCover;			//����		
		wstring _strTrackNumber;		//�������		
		wstring _strCDNumber;			//���̺���		
		wstring _strYear;				//���		
		wstring _strNotes;				//ע��		
		wstring _strClassify;			//����		
		wstring _strStyle;				//���(Ϊ���֣����庬��δ��Ӧ)		
		wstring _strDescription;		//����		
		wstring _strBPM;				//BPM		
		wstring _strNameSort;			//��������		
		wstring _strAlbumSort;			//ר������		
		wstring _strPerformerSort;		//����������		
		wstring _strAlbumPerformerSort;	//ר������������		
		wstring _strComposerSort;		//����������		
		wstring _strMediaTypes_Prodcast;//ý������podcast		
		wstring _strMediaTypes_iTunesU;	//ý������iTunes U	

		string _ArtWork;					//���������ȡ
		
		/*HANDLE*/void* _hMP3Handle; //��MP3Decoder�νӵĲ���MP3�ļ���HANDLE
		
	};
}
