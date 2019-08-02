#pragma once

#include "BaseGroup.h"


#define PICJPG			L".JPG"
#define THMPIC			L".THM"
#define BTHPIC			L".BTH"
#define SUFFIXMOV		L".MOV"

#define PICPNG			L".png"
#define PNG2X			L"@2x.png"
#define NEWPNG2X		L"@2x~iphone.png"
#define IPADPNG2X		L"@2x~ipad.png"
#define THUMBNAIL		L".thumbnail.png"
#define THUMBNAIL2X		L".thumbnail@2x.png"
#define NEWTHUMBNAIL2X	L".thumbnail@2x~iphone.png"
#define IPADTHUMBNAIL2X	L".thumbnail@2x~ipad.png"

#define	DIRMISC			L".MISC"
#define DIRALBUM		"DCIM/100APPLE"
#define DIRGALLERY		"PhotoData/Sync/100SYNCD"
#define IMGTAG			L"IMG_"
#define IPADTAG			L"_iPad"	
#define JPGTYPE			"public.jpeg"
#define PNGTYPE			"public.png"

#define CAMERAID		"camera"
#define	WALLPAPERID		"wallpaper"
#define SCREENSHOTID	"screenshot"

#define IOS_WALLPAPER_WIDTH  320
#define IOS_WALLPAPER_HEIGHT 480

#define IPAD_WALLPAPER_WIDTH  1024
#define IPAD_WALLPAPER_HEIGHT 1024

#define IOS_THUMB_WIDTH  75
#define IOS_THUMB_HEIGHT 75

#define IOS_THUMB2_WIDTH  150
#define IOS_THUMB2_HEIGHT 150

#define CAMERATYPE		3
#define GALLERYTYPE		256

#define ALL_IMAGE_GROUP_ID		"101"				// ios设备图库中【所有图片】分类的id
#define ALL_IMAGE_GROUP_NAME	L"所有图库图片"		// ios设备图库中【所有图片】分类的名称	
#define MOBILEUSER				L"501" 


#pragma warning(disable : 4251)

namespace ND91Assistant
{
	class BinaryBufferReader;
	class ThumbnailData;
	class DeviceData;
	class DownloadFileCommand;
    class IosPicturesImpl;

	// 图片
    class CLASSINDLL_CLASS_DECL PictureData
    {
        friend class PictureManager;
		friend class LoadPhotosCommand;
		friend class AddPictureCommand;
        friend class IosPicturesImpl4;
        friend class IosPicturesImpl3;

	public:
		PictureData();
        PictureData(wstring remoteFilePaths, PICTURE_TYPE type);
		~PictureData();
		PictureData(const PictureData* pic);

        int GetId() const { return _id; } 
        void SetId(int id) { _id = id; }

        string GetCategoryId() const { return _bucketId; }
        void SetCategoryId(string value) { _bucketId = value; }

        wstring GetCategoryName() const { return _categoryName; }
        void SetCategoryName(wstring value) { _categoryName = value; } 

        ND91Assistant::PICTURE_TYPE GetType() const { return _type; }
        void SetType(ND91Assistant::PICTURE_TYPE val) { _type = val; }

		// Summary  : 设置图片缩略图
		// Returns	: void
		// Parameter: const ThumbnailData * pThumbnail
		void SetThumbnail(const ThumbnailData*  pThumbnail);


		void Create( 
					wstring			name,                
					time_t			createTime,     
					time_t			modifyTime,     
					int				width,          
					int				height,         
					int				thumbnailIndex, 					
      
					int				id				=-1,
					int				size			=-1, 
					wstring			localPath		=L"",		
					wstring			remotePath		=L"",		
					wstring			thumbLocalPath	=L"",
					wstring			thumbRemotePath =L"",
					PICTURE_TYPE	type			=PICTURE_TYPE_UNSET
					);



    public:

        wstring _name;
        time_t  _modifyTime;
        time_t  _createTime;
        int     _size;
        int     _thumbnailIndex;    // 缩略图索引(从sqlite读取)

		int     _width;		    // 图片宽
		int     _height;		// 图片高
		wstring _remotePath;	// 图片手机端路径

        // 图片本地存储路径
        // Android: 空白标识尚未下载原图，非空表示已下载原图
        // ios: 空白表示无原图, 非空表示有源图
		mutable wstring _localPath;

        wstring _thumbLocalPath;	// 缩略图存储在本地的路径
        wstring _thumbRemotePath;	// 缩略图手机端路径
        int     _thumbWidth;		// 缩略图宽
        int     _thumbHeight;		// 缩略图高

	private:

        // Summary  : 从守护返回的数据生成对象
        // Returns  : 是否成功
        // Parameter: BinaryBufferReader & reader 二进制数据流
        // Parameter: const DeviceData * pDevice 设备指针
        bool ParseAndroidData(BinaryBufferReader& reader, const DeviceData* pDevice);

    private:

		int		        _id;			// 图片id
		string          _bucketId;		// 图片分类id
		wstring         _categoryName;  // 图片分类名
        PICTURE_TYPE    _type;          // 图片类型

    };

}
