///////////////////////////////////////////////////////////
//  Implementation of the Class VideoData
//  Created on:      01-六月-2011 9:43:04
//  Original author: michael
///////////////////////////////////////////////////////////

#if !defined(EA_D9BA3307_D527_4ad8_B508_876A55AC1566__INCLUDED_)
#define EA_D9BA3307_D527_4ad8_B508_876A55AC1566__INCLUDED_

#include "Module/File/FileData.h"

namespace ND91Assistant
{
	// 视频
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

		// from FileData,返回完整路径,用于获取文件信息
		virtual wstring GetFullPath() const;

		// 获取视频文件大小创建时间等信息
		bool LoadVideoFileInfo(const DeviceData* pDevice);
        
        // Summary  : 重命名
        // Returns	: bool
        // Parameter: const DeviceData * pDevice
        // Parameter: wstring newName  新标题
        bool Rename(const DeviceData* pDevice, wstring newName);

		// Summary  : 下载视频文件
		// Returns	: bool
		// Parameter: const DeviceData * pDevice
		// Parameter: wstring localSavePath  本地存储路径
		bool DownLoadVideoFile(const DeviceData* pDevice, wstring localSavePath);

		int		 _id;				// 视频Id

		int		 _duration;			// 时长

		wstring  _remotepath;		// 手机端全路径

		wstring  _artist;			// 艺术家

		wstring  _album;			// 专辑

		wstring  _title;			// 标题
 
		wstring  _localpath;		// 下载后本地存储路径

		wstring  _imageLocalPath;	// 获取到的视频图片存储到本地的路径

		PlayAlbumData*  _pOwnAlbum;	// 所属专辑

	private:
        // 解析命令返回的数据
		bool ParseAndroidData(BinaryBufferReader& reader);
		void InitParams();

    };
}

#endif // !defined(EA_D9BA3307_D527_4ad8_B508_876A55AC1566__INCLUDED_)
