#pragma once

#include <string>
#include <fstream>
#include <vector>
using namespace std;
#include "IiPodMediaParser.h"
#include "Module/common/Media/RealMedia/Mp4Parser.h"
#include "Module/common/Media/MediaData.h"

namespace ND91Assistant
{
	//mp4���͵�ipodý�����ݲ�����
	class ciPodMP4Paeser : public IiPodMediaParser
	{ 
		public:
			ciPodMP4Paeser(wstring path);
			virtual void OnDestroy();
			virtual bool IsVideo(){return _Mp4Parser->GetMediaData()->_isVideo;}
			virtual long GetMediaDuration(){return _Mp4Parser->GetMediaDuration();}
			virtual bool IsValid(){return false;}	                    // ��ʶ���ļ�
            virtual std::wstring GetDevFileName() {return L"";}			// �豸�ϵ��ļ���
			virtual std::wstring GetLocalFileName(){return L"";}		// �����ļ���

			//ֻ������
			virtual /*UINT*/unsigned int GetFileSize(){return _media->_fileSize;}			                     // �ļ���С
			virtual /*UINT*/unsigned int GetPlayTime() {return _Mp4Parser->GetMediaDuration()*1000;}	                         // ����ʱ��
			virtual /*UINT*/unsigned int GetiBitSpeed() {return 0;}					                             // ����
			virtual /*UINT*/unsigned int GetSampleRate(){return _media->_nSampleRate;}		                     // ������
			virtual /*UINT*/unsigned int GetVolumeAdjustment(){return 0;}					                     // ��������
			virtual bool GetSelectWorks(){return false;}						                 // ��ѡ��Ʒ
			virtual std::wstring GetID3Ver(){return L"";}					                     // ID3�汾
			virtual std::wstring GetChannel(){return L"";}					                     // ͨ����
			virtual std::wstring GetFormat(){return L"";}					                     // ��Ƶ��ʽ

			//��д��Ϣ����
			virtual std::wstring PropertyGetMusicName(){return _media->_title;}	                    // ��ȡ���ֱ���
			virtual bool PropertySetMusicName(std::wstring val){return false;}			            // �������ֱ���
			virtual std::wstring PropertyGetMusicPerformer(){return _media->_artist;}               // ��ȡ������
			virtual bool PropertySetMusicPerformer(std::wstring val){return false;}		            // ���ñ�����
			virtual std::wstring PropertyGetAlbumPerformer(){return L"";}				            // ��ȡר��������
			virtual bool PropertySetAlbumPerformer(std::wstring val){return false;}		            // ����ר��������
			virtual std::wstring PropertyGetComposer(){return L"";}				                    // ��ȡ������
			virtual bool PropertySetComposer(std::wstring val){return false;}			            // ����������
			virtual std::wstring PropertyGetAlbumName(){return _media->_album;}	                    // ȡר����
			virtual bool PropertySetAlbumName(std::wstring val){return false;}			            // ����ר����
			virtual std::wstring PropertyGetLyrics(){return L"";}					                // ���
			virtual std::wstring PropertyGetTrackNumber(){return L"";}					            // �����
			virtual std::wstring PropertyGetCDNumber() {return L"";}						        // cd��
			virtual std::wstring PropertyGetYear(){return L"";}						                // ���		
			virtual std::wstring PropertyGetNotes(){return _media->_comment;}	                    // ע��	
			virtual std::wstring PropertyGetClassify(){return L"";}					                // ����
			virtual std::wstring PropertyGetStyle(){return L"";}						            // ���
			virtual std::wstring PropertyGetDescription(){return L"";}					            // ����
			virtual std::wstring PropertyGetBPM(){return L"";}							            // BMP
			virtual std::wstring PropertyGetNameSort(){return L"";}					                // ��������
			virtual std::wstring PropertyGetAlbumSort(){return L"";}					            // ר��������
			virtual std::wstring PropertyGetPerformerSort(){return L"";}				            // ����������
			virtual std::wstring PropertyGetAlbumPerformerSort() {return L"";}			            // ר������������
			virtual std::wstring PropertyGetComposerSort() {return L"";}				            // ����������
			virtual std::wstring PropertyGetMediaTypes_Prodcast() {return L"";}			            // Prodcast ���ļ�����
			virtual std::wstring PropertyGetMediaTypes_iTunesU() {return L"";}			            // iTunesU���ļ�����
			virtual /*UINT*/unsigned int PropertyGetVolumeAdjustment() {return 0;}						            // ��������
			virtual bool PropertyGetSelectWorks(){return false;}							        // �Ƿ���ѡר��
			virtual bool PropertySetSelectWorks(bool val) {return false;}					        // �����Ƿ���ѡר��
			virtual std::string PropertyGetArtWork(){return "";}						            // ȡ��������
			virtual bool PropertySetArtWork(std::string val) {return false;}				        // ���÷�������

    private:
        Mp4Parser*          _Mp4Parser;
        MediaData*			_media;				// ��Ŷ�ý���ļ�Ԫ�ص�������
	};
}