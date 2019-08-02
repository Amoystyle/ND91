#pragma once

#include "Module/Common/Synchronizer/SynchronizerBase.h"

namespace ND91Assistant
{
    class BookmarkData;
    class BookmarkFolder;

    // ��ǩͬ����
    class BookmarksSynchronizer : public SynchronizerBase
    {
    public:
        BookmarksSynchronizer(const DeviceData* pDevice);
        ~BookmarksSynchronizer();

        vector<BookmarkData*>*		GetBookMarks() { return &_bookMarks; }
        BookmarkFolder*        GetBookMarkFolder() { return _folder; }


        // Summary  : �Ƿ�֧��ͬ������
        // Returns  : bool
        bool IsSupportSynchronizer();

        // Summary  : �ݹ��ɾ����ǩ���ļ���
        // Returns  : void
        // Parameter: BookmarkData * bookMark Ҫɾ������ǩ���ļ���
        void DelBookMark(BookmarkData* bookMark); 

        // Summary  : �����ǩ���ļ��У�parentΪ����Ϊ�ڸ��ڵ���ӣ�
        // Returns  : BookmarkData*
        // Parameter: wstring strName ����
        // Parameter: wstring strUrl ��ַ�����Ϊ����Ϊ����ļ��У�
        // Parameter: wstring parentGuid ���ڵ�GUID
        BookmarkData* AddBookMark(wstring strName, wstring strUrl, string parentGuid);

        // Summary  : �޸���ǩ���ļ��У�֧����ǩ�������ƶ��ʹ�һ���ļ����ƶ�����һ���ļ��У�
        // Returns  : BookmarkData*
        // Parameter: BookmarkData* node Ҫ�޸ĵ���ǩ
        // Parameter: int iNewPos �µ�λ�� ��Ϊ0�������ƶ���
        // Parameter: BookmarkData* newParent �µĸ��ڵ㣨ΪNULL���ƶ�����һ�ļ��У�
        BookmarkData* EditBookMark(BookmarkData* node, int iNewPos = 0, BookmarkData* newParent = NULL);
        
        // Summary  : �������ݶ���ӿ�
        // Returns  : bool
        bool SaveChangesBookMarks();

        static string BOOKMARK_ENTITY_NAME;
        static string FOLDER_ENTITY_NAME;

    protected:
        virtual void DoSaveChanges();                   // ��������
        virtual void ClearCachedData();                 // ����������
        virtual void ParsePlistEntities();              // ����ʵ������

    private:
        void RemapEntitiyIDs();                         // ����ӳ������(�����ݷ���)

        // Summary  : ���ʵ������
        // Returns  : bool
        // Parameter: bool isHaveRecord �Ƿ���Ҫ���������
        // Parameter: bool linkAllRecordsOfPulledEntityTypeSentKey �Ƿ���Ҫ��������ӳ���ID����
        bool PackPlistEntities(bool isHaveRecord,bool linkAllRecordsOfPulledEntityTypeSentKey);
        
        // Summary  : ����KEY��ȡ�ڵ�
        // Returns  : BookmarkData* �ڵ�ָ��
        // Parameter: wstring strKey KEYֵ
        BookmarkData* GetNodeByKey(string strKey);

        // Summary  : ���ݸ��ڵ��ȡ�ӽڵ㼯��
        // Returns  : vector<BookmarkData*> �ӽڵ㼯��
        // Parameter: BookmarkData * parent ���ڵ�
        vector<BookmarkData*> GetChildBookMarks(BookmarkData* parent);

        // Summary  : ����ǩ����תΪĿ¼��ʽ
        // Returns  : void
        void ConvertBookMarkNodesToBookMarkFolder();

        // Summary  : ��Ŀ¼����λ������
        // Returns  : void
        void SortRootByPosition();

        // Summary  : ����ǩ�����Ӧ��Ŀ¼��
        // Returns  : void
        // Parameter: vector<BookmarkData* >& folders  ���е�Ŀ¼����
        void PutBookMarksIntoFolders(vector<BookmarkData*>& folders);

        // Summary  : ��Ŀ¼����λ������
        // Returns  : void
        // Parameter: vector<BookmarkData* >& folders ���е�Ŀ¼����
        void SortChildByPosition(vector<BookmarkData*>& folders);

    private:
        vector<BookmarkData*>		_bookMarks;			// ��ǩ����
        map<string,string>			_remappedIDs;		// ����ӳ���ID����
        BookmarkFolder*				_folder;			// ��ǩ��Ŀ¼��ʽ
		//vector<BookmarkFolder*>*	_pVerFolders;		// ��ǩ���м���
    };
}
