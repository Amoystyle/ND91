#pragma once

#include "NoteManager.h"

namespace ND91Assistant
{
    class CLASSINDLL_CLASS_DECL IosNoteManager : public NoteManager
    {
    public:
        IosNoteManager(const DeviceData* pDevice);
        virtual ~IosNoteManager();
 
        // 是否支持
        virtual bool IsSupport();

        // 加载
        virtual CommandExecuteResult Load();

        // 删除
        virtual CommandExecuteResult DelNote(NoteData* note);

        // 添加
        virtual NoteData* AddNote(wstring subject, wstring content);

		virtual NoteData* ImportNote(NoteData* importnote);

        // 修改
        virtual NoteData* EditNote(NoteData* note, wstring subject, wstring content);

        // 提交修改
        virtual CommandExecuteResult Update();

        NoteSynchronizer* GetNoteSynchronizer();          // 备忘录同步器

    protected:
        NoteSynchronizer* _pNoteSynchronizer;
    };
}