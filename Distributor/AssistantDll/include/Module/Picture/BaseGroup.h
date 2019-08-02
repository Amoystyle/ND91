#pragma once
#pragma warning (disable : 4251)

namespace ND91Assistant
{
	enum PICTURE_TYPE
	{
		PICTURE_TYPE_NORMAL		= 0,  // 
		PICTURE_TYPE_PHOTO		= 1,  // 照片 
		PICTURE_TYPE_WALLPAPER	= 2,  // 壁纸
		PICTURE_TYPE_SCREENSHOT = 3,  // 屏幕截图 
		PICTURE_TYPE_ALL        = 128,// 全部
		PICTURE_TYPE_UNSET		= 255,// 未设置
	};

    class DeviceData;
    class PictureData;

    class CLASSINDLL_CLASS_DECL BaseGroup
    {
		friend class IosPictureManager;
		friend class AndroidPictureManager;
        friend class PictureManager;
    public:
        BaseGroup(const DeviceData*	pDevice);
        virtual ~BaseGroup() {};

        virtual int     Size() const = 0;

        string          GetId  () const { return _id;   }
        wstring         GetName() const { return _name; }
		void			SetName( wstring name ) { _name = name; }
        wstring         GetPath() const { return _path; }
        PICTURE_TYPE    GetType() const { return _type; }

    protected:
        virtual void    InsertPicture(const PictureData* pPicture) = 0;
        virtual void    RemovePictureFromList(const PictureData* pic) = 0;

        const PictureData* AndroidAddPicture(wstring filePath) const;

    protected:
        string    _id;          // 分类ID
        wstring   _name;	    // 分类名称
        wstring   _path;        // 路径
        bool      _visible;     // 是否可见(安卓专用)

        const DeviceData*	_pDevice;
        PICTURE_TYPE        _type;      // 类型
    };
}
