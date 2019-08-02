///////////////////////////////////////////////////////////
//  Implementation of the Class VideoData
//  Created on:      01-����-2011 9:43:04
//  Original author: michael
///////////////////////////////////////////////////////////

#if !defined(EA_D9BA3307_D527_4ad8_B508_876A55AC1566__INCLUDED_)
#define EA_D9BA3307_D527_4ad8_B508_876A55AC1566__INCLUDED_

#include "Module/File/FileData.h"

namespace ND91Assistant
{
	// ��Ƶ
	class DeviceData;
	class PlayAlbumData;
	class BinaryBufferReader;
    class CLASSINDLL_CLASS_DECL VideoData : public FileData
    {
     friend class LoadVideoCommand;
	 friend class VideoOperationCommand;
    public:
        VideoData();
        virtual ~VideoData();
		VideoData(VideoData* pVideo);
		VideoData(FileData* pFileData, PlayAlbumData* pAlbumData);
		VideoData(wstring strLocalFileName, wstring strPhoneFileName, PlayAlbumData* pAlbumData);

		// from FileData,��������·��,���ڻ�ȡ�ļ���Ϣ
		virtual wstring GetFullPath() const;

		// ��ȡ��Ƶ�ļ���С����ʱ�����Ϣ
		bool LoadVideoFileInfo(const DeviceData* pDevice);
        
        // Summary  : ������
        // Returns	: bool
        // Parameter: const DeviceData * pDevice
        // Parameter: wstring newName  �±���
        bool Rename(const DeviceData* pDevice, wstring newName);

		// Summary  : ������Ƶ�ļ�
		// Returns	: bool
		// Parameter: const DeviceData * pDevice
		// Parameter: wstring localSavePath  ���ش洢·��
		bool DownLoadVideoFile(const DeviceData* pDevice, wstring localSavePath);

		int		 _id;				// ��ƵId

		int		 _duration;			// ʱ��

		wstring  _remotepath;		// �ֻ���ȫ·��

		wstring  _artist;			// ������

		wstring  _album;			// ר��

		wstring  _title;			// ����
 
		wstring  _localpath;		// ���غ󱾵ش洢·��

		wstring  _imageLocalPath;	// ��ȡ������ƵͼƬ�洢�����ص�·��

		PlayAlbumData*  _pOwnAlbum;	// ����ר��

	private:
        // ��������ص�����
		bool ParseAndroidData(BinaryBufferReader& reader);
		void InitParams();

    };
}

#endif // !defined(EA_D9BA3307_D527_4ad8_B508_876A55AC1566__INCLUDED_)
