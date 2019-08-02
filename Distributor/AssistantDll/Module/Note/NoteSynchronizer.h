#pragma once

#include "Module/Common/Synchronizer/SynchronizerBase.h"

namespace ND91Assistant
{
    class NoteData;

    // 备忘录同步器
    class NoteSynchronizer : public SynchronizerBase
    {
    public:
        NoteSynchronizer(const DeviceData* pDevice);
        ~NoteSynchronizer();

        vector<NoteData*>* GetCacheNotes() { return &_cacheNotes; }

        // Summary  : 是否支持同步功能
        // Returns  : bool
        bool IsSupportSynchronizer();

        // Summary  : 删除备忘录
        // Returns  : void
        // Parameter: Note * note 要删除的备忘录
        void DelNote(NoteData* note);

        // Summary  : 添加备忘录
        // Returns  : Note* 新添加的备忘录
        // Parameter: wstring subject 标题
        // Parameter: wstring content 内容
        NoteData* AddNote(wstring subject, wstring content);

        // Summary  : 编辑备忘录
        // Returns  : Note* 编辑的备忘录
        // Parameter: Note * note 编辑的备忘录
        // Parameter: wstring subject 标题
        // Parameter: wstring content 内容
        NoteData* EditNote(NoteData* note, wstring subject, wstring content);

        // Summary  : 保存数据对外接口
        // Returns  : bool
        bool SaveChangesNotes();

		// Summary  : 添加备忘录
		// Returns	: NoteData*
		// Parameter: NoteData * importnote
		NoteData* AddNote(NoteData* importnote );

        static string NOTE_ENTITY_NAME;

    protected:
        virtual void DoSaveChanges();                   // 保存数据
        virtual void ClearCachedData();                 // 清理缓存数据
        virtual void ParsePlistEntities();        // 解析实体数据

    private:
        void RemapEntitiyIDs();                         // 重新映射数据(在添加时返回数据)

        // Summary  : 打包实体数据
        // Returns  : bool
        // Parameter: bool isHaveRecord 是否还有要打包的数据
        // Parameter: bool linkAllRecordsOfPulledEntityTypeSentKey 是否需要返回重新映射的ID集合
        bool PackPlistEntities(bool isHaveRecord,bool linkAllRecordsOfPulledEntityTypeSentKey);   
        
    private:
        vector<NoteData*>       _cacheNotes;        // 书签集合
        map<string,string>  _remappedIDs;       // 重新映射的ID集合
    };
}
