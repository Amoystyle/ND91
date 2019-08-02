#pragma once

#include <string>

using namespace std;


namespace ND91Assistant
{
	class IiPodMediaParser
	{ 
	public:
		// ��ȡý���ļ�����ʱ��
		virtual bool IsVideo()=0;
		virtual void OnDestroy() = 0;
		virtual long GetMediaDuration() = 0;
		virtual bool IsValid() = 0;		//��ʶ���ļ�
		virtual std::wstring GetDevFileName() = 0;			//�豸�ϵ��ļ���
		virtual std::wstring GetLocalFileName() = 0;		//�����ļ���

		//ֻ������
		virtual /*UINT*/unsigned int GetFileSize() = 0;						 //�ļ���С
		virtual /*UINT*/unsigned int GetPlayTime() = 0;						 //����ʱ��
		virtual /*UINT*/unsigned int GetiBitSpeed() = 0;					 //����
		virtual /*UINT*/unsigned int GetSampleRate() = 0;					 //������
		virtual /*UINT*/unsigned int GetVolumeAdjustment() = 0;				 //��������
		virtual bool GetSelectWorks() = 0;					 // ��ѡ��Ʒ
		virtual std::wstring GetID3Ver() = 0;				 //ID3�汾
		virtual std::wstring GetChannel() = 0;				 //ͨ����
		virtual std::wstring GetFormat() = 0;				 //��Ƶ��ʽ

		//��д��Ϣ����
		virtual std::wstring PropertyGetMusicName() = 0;				//��ȡ���ֱ���
		virtual bool PropertySetMusicName(std::wstring val)  = 0;		//�������ֱ���
		virtual std::wstring PropertyGetMusicPerformer()  = 0;			//��ȡ������
		virtual bool PropertySetMusicPerformer(std::wstring val)  = 0;	//���ñ�����
		virtual std::wstring PropertyGetAlbumPerformer()  = 0;			//��ȡר��������
		virtual bool PropertySetAlbumPerformer(std::wstring val)  = 0;	//����ר��������
		virtual std::wstring PropertyGetComposer() = 0;			//��ȡ������
		virtual bool PropertySetComposer(std::wstring val)  = 0;		//����������
		virtual std::wstring PropertyGetAlbumName()  = 0;				//ȡר����
		virtual bool PropertySetAlbumName(std::wstring val)  = 0;		//����ר����
		virtual std::wstring PropertyGetLyrics() = 0;				//���
		virtual std::wstring PropertyGetTrackNumber()  = 0;				//�����
		virtual std::wstring PropertyGetCDNumber() = 0;					//cd��
		virtual std::wstring PropertyGetYear()  = 0;					//���		
		virtual std::wstring PropertyGetNotes()  = 0;					//ע��	
		virtual std::wstring PropertyGetClassify()  = 0;				//����
		virtual std::wstring PropertyGetStyle()  = 0;					//���
		virtual std::wstring PropertyGetDescription() = 0;				//����
		virtual std::wstring PropertyGetBPM()  = 0;						//BMP
		virtual std::wstring PropertyGetNameSort()  = 0;				//��������
		virtual std::wstring PropertyGetAlbumSort()  = 0;				//ר��������
		virtual std::wstring PropertyGetPerformerSort() = 0;			//����������
		virtual std::wstring PropertyGetAlbumPerformerSort()  = 0;		//ר������������
		virtual std::wstring PropertyGetComposerSort()  = 0;			//����������
		virtual std::wstring PropertyGetMediaTypes_Prodcast()  = 0;		//Prodcast ���ļ�����
		virtual std::wstring PropertyGetMediaTypes_iTunesU() = 0;		//iTunesU���ļ�����
		virtual /*UINT*/unsigned int PropertyGetVolumeAdjustment() = 0;					//��������
		virtual bool PropertyGetSelectWorks() = 0;						//�Ƿ���ѡר��
		virtual bool PropertySetSelectWorks(bool val) = 0;				//�����Ƿ���ѡר��
		virtual std::string PropertyGetArtWork() = 0;					//ȡ��������
		virtual bool PropertySetArtWork(std::string val) = 0;			//���÷�������
	};
}
