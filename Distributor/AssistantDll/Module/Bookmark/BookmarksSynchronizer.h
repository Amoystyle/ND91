#pragma once

#include "Module/Common/Synchronizer/SynchronizerBase.h"

namespace ND91Assistant
{
    class BookmarkData;
    class BookmarkFolder;

    // 书签同步器
    class BookmarksSynchronizer : public SynchronizerBase
    {
    public:
        BookmarksSynchronizer(const DeviceData* pDevice);
        ~BookmarksSynchronizer();

        vector<BookmarkData*>*		GetBookMarks() { return &_bookMarks; }
        BookmarkFolder*        GetBookMarkFolder() { return _folder; }


        // Summary  : 是否支持同步功能
        // Returns  : bool
        bool IsSupportSynchronizer();

        // Summary  : 递归的删除书签或文件夹
        // Returns  : void
        // Parameter: BookmarkData * bookMark 要删除的书签或文件夹
        void DelBookMark(BookmarkData* bookMark); 

        // Summary  : 添加书签或文件夹（parent为空则为在根节点添加）
        // Returns  : BookmarkData*
        // Parameter: wstring strName 名字
        // Parameter: wstring strUrl 网址（如果为空则为添加文件夹）
        // Parameter: wstring parentGuid 父节点GUID
        BookmarkData* AddBookMark(wstring strName, wstring strUrl, string parentGuid);

        // Summary  : 修改书签或文件夹（支持书签的上下移动和从一个文件夹移动到另一个文件夹）
        // Returns  : BookmarkData*
        // Parameter: BookmarkData* node 要修改的书签
        // Parameter: int iNewPos 新的位置 （为0则不上下移动）
        // Parameter: BookmarkData* newParent 新的父节点（为NULL则不移动到另一文件夹）
        BookmarkData* EditBookMark(BookmarkData* node, int iNewPos = 0, BookmarkData* newParent = NULL);
        
        // Summary  : 保存数据对外接口
        // Returns  : bool
        bool SaveChangesBookMarks();

        static string BOOKMARK_ENTITY_NAME;
        static string FOLDER_ENTITY_NAME;

    protected:
        virtual void DoSaveChanges();                   // 保存数据
        virtual void ClearCachedData();                 // 清理缓存数据
        virtual void ParsePlistEntities();              // 解析实体数据

    private:
        void RemapEntitiyIDs();                         // 重新映射数据(无数据返回)

        // Summary  : 打包实体数据
        // Returns  : bool
        // Parameter: bool isHaveRecord 是否还有要打包的数据
        // Parameter: bool linkAllRecordsOfPulledEntityTypeSentKey 是否需要返回重新映射的ID集合
        bool PackPlistEntities(bool isHaveRecord,bool linkAllRecordsOfPulledEntityTypeSentKey);
        
        // Summary  : 根据KEY获取节点
        // Returns  : BookmarkData* 节点指针
        // Parameter: wstring strKey KEY值
        BookmarkData* GetNodeByKey(string strKey);

        // Summary  : 根据父节点获取子节点集合
        // Returns  : vector<BookmarkData*> 子节点集合
        // Parameter: BookmarkData * parent 父节点
        vector<BookmarkData*> GetChildBookMarks(BookmarkData* parent);

        // Summary  : 将书签集合转为目录形式
        // Returns  : void
        void ConvertBookMarkNodesToBookMarkFolder();

        // Summary  : 根目录按照位置排序
        // Returns  : void
        void SortRootByPosition();

        // Summary  : 将书签放入对应的目录下
        // Returns  : void
        // Parameter: vector<BookmarkData* >& folders  所有的目录集合
        void PutBookMarksIntoFolders(vector<BookmarkData*>& folders);

        // Summary  : 子目录按照位置排序
        // Returns  : void
        // Parameter: vector<BookmarkData* >& folders 所有的目录集合
        void SortChildByPosition(vector<BookmarkData*>& folders);

    private:
        vector<BookmarkData*>		_bookMarks;			// 书签集合
        map<string,string>			_remappedIDs;		// 重新映射的ID集合
        BookmarkFolder*				_folder;			// 书签的目录形式
		//vector<BookmarkFolder*>*	_pVerFolders;		// 书签所有集合
    };
}
