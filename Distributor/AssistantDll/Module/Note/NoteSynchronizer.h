#pragma once

#include "Module/Common/Synchronizer/SynchronizerBase.h"

namespace ND91Assistant
{
    class NoteData;

    // ����¼ͬ����
    class NoteSynchronizer : public SynchronizerBase
    {
    public:
        NoteSynchronizer(const DeviceData* pDevice);
        ~NoteSynchronizer();

        vector<NoteData*>* GetCacheNotes() { return &_cacheNotes; }

        // Summary  : �Ƿ�֧��ͬ������
        // Returns  : bool
        bool IsSupportSynchronizer();

        // Summary  : ɾ������¼
        // Returns  : void
        // Parameter: Note * note Ҫɾ���ı���¼
        void DelNote(NoteData* note);

        // Summary  : ��ӱ���¼
        // Returns  : Note* ����ӵı���¼
        // Parameter: wstring subject ����
        // Parameter: wstring content ����
        NoteData* AddNote(wstring subject, wstring content);

        // Summary  : �༭����¼
        // Returns  : Note* �༭�ı���¼
        // Parameter: Note * note �༭�ı���¼
        // Parameter: wstring subject ����
        // Parameter: wstring content ����
        NoteData* EditNote(NoteData* note, wstring subject, wstring content);

        // Summary  : �������ݶ���ӿ�
        // Returns  : bool
        bool SaveChangesNotes();

		// Summary  : ��ӱ���¼
		// Returns	: NoteData*
		// Parameter: NoteData * importnote
		NoteData* AddNote(NoteData* importnote );

        static string NOTE_ENTITY_NAME;

    protected:
        virtual void DoSaveChanges();                   // ��������
        virtual void ClearCachedData();                 // ����������
        virtual void ParsePlistEntities();        // ����ʵ������

    private:
        void RemapEntitiyIDs();                         // ����ӳ������(�����ʱ��������)

        // Summary  : ���ʵ������
        // Returns  : bool
        // Parameter: bool isHaveRecord �Ƿ���Ҫ���������
        // Parameter: bool linkAllRecordsOfPulledEntityTypeSentKey �Ƿ���Ҫ��������ӳ���ID����
        bool PackPlistEntities(bool isHaveRecord,bool linkAllRecordsOfPulledEntityTypeSentKey);   
        
    private:
        vector<NoteData*>       _cacheNotes;        // ��ǩ����
        map<string,string>  _remappedIDs;       // ����ӳ���ID����
    };
}
