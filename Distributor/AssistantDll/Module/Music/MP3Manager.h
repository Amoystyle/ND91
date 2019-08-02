#pragma once
#include "Command/Common/Media/RealMedia/MP3Data.h"
#include "Command/Common/Media/RealMedia/MP3Decoder.h"
#include <map>

using namespace std;

namespace ND91Assistant
{
	/*typedef enum eAvailableSetProperty
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
	};		*/

	//MP3�ļ���Ϣ������
	class MP3Manager
	{
	public:
		MP3Manager(wstring strFileFullName);
		~MP3Manager(void);

		//************************************
		// Method:    IsValidMp3 ���ж��Ƿ��ǿ�ʶ���MP3�ļ�
		// FullName:  ND91Assistant::MP3Manager::IsValidMp3
		// Access:    public static 
		// Returns:   bool �����򷵻�TRUE�����򷵻�FALSE������FALSE�򲻿����ô˹�������в���
		// Qualifier:
		// Parameter: wstring strFileFullName ��MP3�ļ��ı����ļ�������·����
		//************************************
		static bool IsValidMp3(wstring strFileFullName);

		//************************************
		// Method:    GetMp3DataInfo ����ȡMP3����Ϣ
		// FullName:  ND91Assistant::MP3Manager::GetMp3DataInfo
		// Access:    public 
		// Returns:   const MP3Data* ������MP3��Ϣ���������ⲿ�����޸�
		// Qualifier:
		//************************************
		const MP3Data* GetMp3DataInfo();

		//************************************
		// Method:    ModifyProperty ���޸�MP3������Ϣ����������Ӧ�ã�DoModify����Ӧ�ã�CancelModify��ȡ������δӦ�õ��޸�
		// FullName:  ND91Assistant::MP3Manager::ModifyProperty
		// Access:    public 
		// Returns:   void
		// Qualifier:
		// Parameter: eAvailableSetProperty eProperty �����޸����Ե�����ö��ֵ
		// Parameter: wstring strNewValue ���޸ĵ�MP3������Ϣ����ֵ
		//************************************
		void ModifyProperty(eAvailableSetProperty eProperty, wstring strNewValue);

		//************************************
		// Method:    CancelModify ��ȡ������δӦ�õ��޸�
		// FullName:  ND91Assistant::MP3Manager::CancelModify
		// Access:    public 
		// Returns:   void
		// Qualifier:
		//************************************
		void CancelModify();

		//************************************
		// Method:    DoModify ��Ӧ�������޸ĵ�����
		// FullName:  ND91Assistant::MP3Manager::DoModify
		// Access:    public 
		// Returns:   void
		// Qualifier:
		//************************************
		bool DoModify();

	private:
		UINT GetUintByType(ENUM_MP3_INFO_PARA eType);
		wstring GetWStringByType(ENUM_MP3_INFO_PARA eType);
		ENUM_MP3_INFO_PARA GetAvailableSetMp3InfoPara(eAvailableSetProperty eType);
		void SetAvailableSetMp3Info(eAvailableSetProperty eType, wstring strValue);

	private:
		MP3Data* _pMP3Data; //MP3��������Ϣ
		wstring _strFileFullName; //MP3�ļ���
		HANDLE _hMP3Handle; //��MP3Decoder�νӵĲ���MP3�ļ���HANDLE
 		map<eAvailableSetProperty, wstring> _mapSetInfo; //����޸ĵ����Լ���Ӧֵ
	};
}
