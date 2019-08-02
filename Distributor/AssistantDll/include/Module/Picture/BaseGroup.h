#pragma once
#pragma warning (disable : 4251)

namespace ND91Assistant
{
	enum PICTURE_TYPE
	{
		PICTURE_TYPE_NORMAL		= 0,  // 
		PICTURE_TYPE_PHOTO		= 1,  // ��Ƭ 
		PICTURE_TYPE_WALLPAPER	= 2,  // ��ֽ
		PICTURE_TYPE_SCREENSHOT = 3,  // ��Ļ��ͼ 
		PICTURE_TYPE_ALL        = 128,// ȫ��
		PICTURE_TYPE_UNSET		= 255,// δ����
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
        string    _id;          // ����ID
        wstring   _name;	    // ��������
        wstring   _path;        // ·��
        bool      _visible;     // �Ƿ�ɼ�(��׿ר��)

        const DeviceData*	_pDevice;
        PICTURE_TYPE        _type;      // ����
    };
}
