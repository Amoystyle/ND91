#include "pch_module.h"

#ifdef MODULE_NOTE

#include "Module/Note/IosNoteManager.h"
#include "Module/Note/NoteSynchronizer.h"

IosNoteManager::IosNoteManager( const DeviceData* pDevice )
: NoteManager (pDevice)
{
	_pNoteSynchronizer = NULL;
}

IosNoteManager::~IosNoteManager()
{
	SAFE_DELETE(_pNoteSynchronizer);
}

bool ND91Assistant::IosNoteManager::IsSupport()
{
    return true;
}

CommandExecuteResult ND91Assistant::IosNoteManager::Load()
{
    GetNoteSynchronizer()->LoadAllData();
    vector<NoteData*>* pNotes = GetNoteSynchronizer()->GetCacheNotes();

    //Release();
	_datas.clear();

    for (vector<NoteData*>::iterator it = pNotes->begin(); it != pNotes->end(); it++)
    {
        _datas.push_back(*it);
    }

    return COMMAND_EXECUTE_SUCCESS;
}

CommandExecuteResult ND91Assistant::IosNoteManager::DelNote(NoteData* note)
{
    GetNoteSynchronizer()->DelNote(note);
    return COMMAND_EXECUTE_SUCCESS;
}

NoteData* ND91Assistant::IosNoteManager::AddNote(wstring subject, wstring content)
{
    return GetNoteSynchronizer()->AddNote(subject, content);
}

NoteData* ND91Assistant::IosNoteManager::ImportNote(NoteData* importnote)
{
	return GetNoteSynchronizer()->AddNote(importnote);
}

NoteData* ND91Assistant::IosNoteManager::EditNote(NoteData* note, wstring subject, wstring content)
{
    return GetNoteSynchronizer()->EditNote(note, subject, content);
}

CommandExecuteResult ND91Assistant::IosNoteManager::Update()
{
    return GetNoteSynchronizer()->SaveChangesNotes() ? COMMAND_EXECUTE_SUCCESS : COMMAND_EXECUTE_FAIL;
}

NoteSynchronizer* ND91Assistant::IosNoteManager::GetNoteSynchronizer()
{
    if(!_pNoteSynchronizer)
        _pNoteSynchronizer = new NoteSynchronizer(_pDevice);
    return _pNoteSynchronizer;
}

#endif 