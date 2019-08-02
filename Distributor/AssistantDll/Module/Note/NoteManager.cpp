#include "pch_module.h"

#ifdef MODULE_NOTE

#include "Module/Note/NoteData.h"
#include "Module/Note/NoteManager.h"

NoteManager::NoteManager( const DeviceData* pDevice )
: IManager (pDevice)
{
}

NoteManager::~NoteManager()
{
    Release();
}

bool ND91Assistant::NoteManager::IsSupport()
{
    return false;
}

CommandExecuteResult ND91Assistant::NoteManager::Load()
{
    return COMMAND_EXECUTE_UNSUPPORT;
}

void ND91Assistant::NoteManager::Release()
{
    for (vector<NoteData*>::iterator it = _datas.begin(); it != _datas.end(); it++)
        SAFE_DELETE(*it);
    _datas.clear();
}

const vector<NoteData*>* ND91Assistant::NoteManager::GetDatas()
{
    return &_datas;
}

CommandExecuteResult ND91Assistant::NoteManager::DelNote(NoteData* note)
{
    return COMMAND_EXECUTE_UNSUPPORT;
}

NoteData* ND91Assistant::NoteManager::AddNote(wstring subject, wstring content)
{
    return NULL;
}

NoteData* ND91Assistant::NoteManager::EditNote(NoteData* note, wstring subject, wstring content)
{
    return NULL;
}

CommandExecuteResult ND91Assistant::NoteManager::Update()
{
    return COMMAND_EXECUTE_UNSUPPORT;
}

#endif 