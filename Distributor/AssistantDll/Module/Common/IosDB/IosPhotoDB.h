#pragma  once

#include "IosPdbData.h"
#include "IthmbDB.h"

#include <string>
#include <map>
#include <list>

using namespace std;

namespace Gdiplus
{
    class Bitmap;
}

namespace ND91Assistant
{
    // ios设备的photo database文件读写类
    class IosPhotoDB
    {
    public:
        // Summary  : 构造函数
        // Parameter: wstring path 本地硬盘的photo database文件路径
        // Parameter: const vector<int> & thumbTypes 需要支持的THUMB文件类型
        // Parameter: wstring thumbFileFolder ithmb文件所在目录
        //              为空时设定为photo database文件所在目录的THUMBS子目录
        IosPhotoDB(wstring path, const vector<int>& thumbTypes, wstring thumbFolder = L"");
        ~IosPhotoDB(void);

        // Summary  : 取缩略图文件信息
        // Returns  : map<文件ID, 图像大小>
        map<int, int> GetThumbIdAndSize();

        // Summary  : 返回所有图片id
        // Returns  : vector<图片ID>, 调用者负责delete
        vector<int>* Load();

        // Summary  : 返回所有图片数量
        int size();

        // Summary  : 将当前DB内容写入文件
        // Parameter: wstring destFile 写入的目标文件名
        void Write(wstring destFile);

        // Summary  : 将图片添加到图库(不添加原图文件, write时才添加)
        // Returns  : 图片id: >0表示成功, -1表示失败
        // Parameter: Image * pImage 图片指针
        int AddImage(Bitmap* pImage);

        // Summary  : 从图库中删除图片(不删除原图文件)
        // Returns  : 成功返回true, 没找到返回false
        // Parameter: int imageId 图片id
		// Parameter: 是否从所有分类中删除
        bool DelImage(int imageId, bool deleteAll = true);

        // Summary  : 修改图片内容(同时修改原图文件)
        // Returns  : 成功返回true, 没找到返回false
        // Parameter: int imageId 图片id
        // Parameter: Bitmap * pBitmap 图片内容
        bool UpdateImage(int imageId, Bitmap* pBitmap);

		// Summary  : 取图片内容
		// Returns  : Bitmap* 图片内容，调用者负责删除(找不到返回NULL)
		// Parameter: int id 图片id
		// Parameter: IOS_THUMB_TYPE type 缩略图类型
		Bitmap* GetImage(int id, IOS_THUMB_TYPE type);

        // Summary  : 取原图路径(ios 3.2以上版本)
        // Returns  : 图片路径, 找不到返回空字符串
        // Parameter: id 图片ID
        // Parameter: delimiter 目录分隔符, 只能是\或/
        wstring GetImageFilePath(int id, char delimiter);

        // Summary  : 取所有需要上传的文件名
        // Returns  : map<图片id, 图片路径>
        list<int> GetUploadFiles();

        // Summary  : 取所有需要删除的文件名
        // Returns  : vector<图片路径>
        vector<wstring> GetDeleteFiles();

#pragma region 图库分类访问

        // Summary  : 加载图库所有分类
        // Returns  : map<分类ID, 分类名称>的指针, 调用者负责删除
        map<int, wstring>* GetAllCategory();

        // Summary  : 取某个分类下的所有图片
        // Returns  : vector<图片id>的指针, 调用者负责删除
        // Parameter: int id 分类ID
        vector<int>* GetCategoryImages(int id);

        // Summary  : 分类改名
        // Parameter: int id 分类id
        // Parameter: wstring newName 新名
        void RenameCategory(int id, wstring newName);

        // Summary  : 增加分类
        // Returns  : 分类ID, -1表示重名失败
        // Parameter: wstring name 分类名
        int AddCategory(wstring name);

        // Summary  : 删除分类
        // Parameter: int id 分类id
        void DelCategory(int id);

        // Summary  : 将图片从一个分类移动到另一个分类
        // Return   : 0 成功 1 原分类不存在 2 新分类不存在 3 新分类已有此图 4 原分类无此图
        // Parameter: int imageId 图片id
        // Parameter: int originalCategoryId 原分类id
        // Parameter: int newCategoryId 新分类id
        int MoveImageBetweenCategory(int imageId, int originalCategoryId, int newCategoryId);

        // Summary  : 将已有的图片添加到分类(需先添加到图库)
        // Return   : 0 成功 1 分类不存在 2 分类中已有此图片
        // Parameter: int imageId 图片id
        // Parameter: int categoryId 分类id
        int AddImageToCategory(int imageId, int categoryId);

        // Summary  : 从分类中移除图片(移除后若所有分类中均无, 此图依然存在于"全部"中)
        // Returns  : 成功返回true, 没找到返回false
        // Parameter: int imageId 图片id
        // Parameter: int categoryId 分类id
        bool RemoveFromCategory(int imageId, int categoryId);

#pragma endregion

        vector<wstring> GetThumbFileName();

        void SetNeedMHAF(bool val) { _needMHAF = val; }

    private:

        void Read();

        void InitThumbInfo(const vector<int>& thumbTypes);

        IthmbDB* GetIthmb( IOS_THUMB_TYPE type );

    private:

        wstring         _filePath;      // 文件路径
        wstring         _workFolder;    // 存放临时文件的工作目录
        wstring         _thumbFolder;   // ithmb文件所在的目录

        PdbFileHead         _head;      // 文件头
        PdbImageList*       _images;    // 图片数据
        PdbCategoryList*    _category;  // 分类数据
        PdbFileList*        _files;     // 文件数据

        THUMB_FILES         _thumbs;    // 每种缩略图的读写类实例

        bool            _needMHAF;      // 搞不清楚是否需要，只好让调用者设置了
    };
}
