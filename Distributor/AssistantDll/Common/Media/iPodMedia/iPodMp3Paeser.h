#pragma once

#include <string>
#include <fstream>
#include <vector>
#include "IiPodMediaParser.h"
#include "Core/MP3Data.h"

using namespace std;

namespace ND91Assistant
{
	//mp3���͵�ipodý�����ݲ�����
	class ciPodMP3Paeser : public IiPodMediaParser//,public MP3Data
	{ 
		public:
			ciPodMP3Paeser(wstring path);
			virtual void OnDestroy();
			virtual bool IsVideo(){return false;}
			virtual long GetMediaDuration(){return _Mp3Data->GetMediaDuration();}
			virtual bool IsValid(){return _Mp3Data->IsValid();}	//��ʶ���ļ�
			virtual std::wstring GetDevFileName() {return _Mp3Data->GetDevFileName();}			//�豸�ϵ��ļ���
			virtual std::wstring GetLocalFileName(){return _Mp3Data->GetLocalFileName();}		//�����ļ���

			//ֻ������
			virtual UINT GetFileSize(){return _Mp3Data->GetFileSize();}					 //�ļ���С
			virtual UINT GetPlayTime() {return _Mp3Data->GetPlayTime();}						 //����ʱ��
			virtual UINT GetiBitSpeed() {return _Mp3Data->GetiBitSpeed();}					 //����
			virtual UINT GetSampleRate(){return _Mp3Data->GetSampleRate();}						 //������
			virtual UINT GetVolumeAdjustment(){return _Mp3Data->GetVolumeAdjustment();}					 //��������
			virtual bool GetSelectWorks(){return _Mp3Data->GetSelectWorks();}						 // ��ѡ��Ʒ
			virtual std::wstring GetID3Ver(){return _Mp3Data->GetID3Ver();}					 //ID3�汾
			virtual std::wstring GetChannel(){return _Mp3Data->GetChannel();}					 //ͨ����
			virtual std::wstring GetFormat(){return _Mp3Data->GetFormat();}					 //��Ƶ��ʽ

			//��д��Ϣ����
			virtual std::wstring PropertyGetMusicName(){return _Mp3Data->PropertyGetMusicName();}					//��ȡ���ֱ���
			virtual bool PropertySetMusicName(std::wstring val){return _Mp3Data->PropertySetMusicName(val);}			//�������ֱ���
			virtual std::wstring PropertyGetMusicPerformer(){return _Mp3Data->PropertyGetMusicPerformer();}				//��ȡ������
			virtual bool PropertySetMusicPerformer(std::wstring val){return _Mp3Data->PropertySetMusicPerformer(val);}		//���ñ�����
			virtual std::wstring PropertyGetAlbumPerformer(){return _Mp3Data->PropertyGetAlbumPerformer();}				//��ȡר��������
			virtual bool PropertySetAlbumPerformer(std::wstring val){return _Mp3Data->PropertySetAlbumPerformer(val);}		//����ר��������
			virtual std::wstring PropertyGetComposer(){return _Mp3Data->PropertyGetComposer();}				//��ȡ������
			virtual bool PropertySetComposer(std::wstring val){return _Mp3Data->PropertySetComposer(val);}			//����������
			virtual std::wstring PropertyGetAlbumName(){return _Mp3Data->PropertyGetAlbumName();}					//ȡר����
			virtual bool PropertySetAlbumName(std::wstring val){return _Mp3Data->PropertySetAlbumName(val);}			//����ר����
			virtual std::wstring PropertyGetLyrics(){return _Mp3Data->PropertyGetLyrics();}					//���
			virtual std::wstring PropertyGetTrackNumber(){return _Mp3Data->PropertyGetTrackNumber();}					//�����
			virtual std::wstring PropertyGetCDNumber() {return _Mp3Data->PropertyGetCDNumber();}						//cd��
			virtual std::wstring PropertyGetYear(){return _Mp3Data->PropertyGetYear();}						//���		
			virtual std::wstring PropertyGetNotes(){return _Mp3Data->PropertyGetNotes();}						//ע��	
			virtual std::wstring PropertyGetClassify(){return _Mp3Data->PropertyGetClassify();}					//����
			virtual std::wstring PropertyGetStyle(){return _Mp3Data->PropertyGetStyle();}						//���
			virtual std::wstring PropertyGetDescription(){return _Mp3Data->PropertyGetDescription();}					//����
			virtual std::wstring PropertyGetBPM(){return _Mp3Data->PropertyGetBPM();}							//BMP
			virtual std::wstring PropertyGetNameSort(){return _Mp3Data->PropertyGetNameSort();}					//��������
			virtual std::wstring PropertyGetAlbumSort(){return _Mp3Data->PropertyGetAlbumSort();}					//ר��������
			virtual std::wstring PropertyGetPerformerSort(){return _Mp3Data->PropertyGetPerformerSort();}				//����������
			virtual std::wstring PropertyGetAlbumPerformerSort() {return _Mp3Data->PropertyGetAlbumPerformerSort();}			//ר������������
			virtual std::wstring PropertyGetComposerSort() {return _Mp3Data->PropertyGetComposerSort();}				//����������
			virtual std::wstring PropertyGetMediaTypes_Prodcast() {return _Mp3Data->PropertyGetMediaTypes_Prodcast();}			//Prodcast ���ļ�����
			virtual std::wstring PropertyGetMediaTypes_iTunesU() {return _Mp3Data->PropertyGetMediaTypes_iTunesU();}			//iTunesU���ļ�����
			virtual UINT PropertyGetVolumeAdjustment() {return _Mp3Data->PropertyGetVolumeAdjustment();}						//��������
			virtual bool PropertyGetSelectWorks(){return _Mp3Data->PropertyGetSelectWorks();}							//�Ƿ���ѡר��
			virtual bool PropertySetSelectWorks(bool val) {return _Mp3Data->PropertySetSelectWorks(val);}					//�����Ƿ���ѡר��
			virtual std::string PropertyGetArtWork(){return _Mp3Data->PropertyGetArtWork();}						//ȡ��������
			virtual bool PropertySetArtWork(std::string val) {return _Mp3Data->PropertySetArtWork(val);}				//���÷�������
	private:
		MP3Data* _Mp3Data;
	};
}