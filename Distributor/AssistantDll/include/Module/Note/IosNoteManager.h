#pragma once

#include "NoteManager.h"

namespace ND91Assistant
{
    class CLASSINDLL_CLASS_DECL IosNoteManager : public NoteManager
    {
    public:
        IosNoteManager(const DeviceData* pDevice);
        virtual ~IosNoteManager();
 
        // �Ƿ�֧��
        virtual bool IsSupport();

        // ����
        virtual CommandExecuteResult Load();

        // ɾ��
        virtual CommandExecuteResult DelNote(NoteData* note);

        // ���
        virtual NoteData* AddNote(wstring subject, wstring content);

		virtual NoteData* ImportNote(NoteData* importnote);

        // �޸�
        virtual NoteData* EditNote(NoteData* note, wstring subject, wstring content);

        // �ύ�޸�
        virtual CommandExecuteResult Update();

        NoteSynchronizer* GetNoteSynchronizer();          // ����¼ͬ����

    protected:
        NoteSynchronizer* _pNoteSynchronizer;
    };
}