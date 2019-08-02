#include "pch_module.h"

#ifdef MODULE_NOTE

#include "Module/Note/NoteSynchronizer.h"
#include "Module/Note/NoteData.h"

#include "Module/Common/Synchronizer/DeviceSyncClient.h"
#include "Device/CFConverter.h"
#include "Device/IosUsbConnection.h"

#include <algorithm>
#define DATACLASSVERSION   102  // 数据类型在PC上的版本号

string NoteSynchronizer::NOTE_ENTITY_NAME = "com.apple.notes.Note";
NoteSynchronizer::NoteSynchronizer(const DeviceData* pDevice) : SynchronizerBase(pDevice)
{
    _cacheNotes.clear();
    _remappedIDs.clear();

    _shortDataClass     = "Notes";
    _dataClass          = "com.apple.Notes";
    _dataClassAnchor    = "Notes-Device-Anchor";
    _dataClassVersion   = DATACLASSVERSION;
}

NoteSynchronizer::~NoteSynchronizer()
{
    ClearCachedData();
}

bool NoteSynchronizer::SaveChangesNotes()
{
    //_cacheNotes = notes;
    if (_cacheNotes.empty())
        return true;

    return SaveChanges();
}

void NoteSynchronizer::DoSaveChanges()
{
    if (!PackPlistEntities(false, true))
        return ;

    if (!_syncClient->SendProcessChangesToDevice(_sendInfo))
       return ;

    if (!_syncClient->GetRemapIdentifiersFromDevice(_remappedIDs))
        return ;

    RemapEntitiyIDs();
}

void NoteSynchronizer::RemapEntitiyIDs()
{
    for (size_t i = 0; i < _cacheNotes.size(); i++)
    {
        NoteData* n = _cacheNotes.at(i);
        map<string,string>::iterator iter = _remappedIDs.find(n->GetKey());
        if (iter == _remappedIDs.end())
            continue;

        n->SetKey(iter->second);
    }
}

void NoteSynchronizer::ClearCachedData()
{
    for (size_t i = 0; i < _cacheNotes.size(); i++)
        SAFE_DELETE(_cacheNotes.at(i));
    _cacheNotes.clear();
}

void NoteSynchronizer::ParsePlistEntities()
{
    for (map<string, map<string,void*>>::iterator it = _mapEntities.begin(); it != _mapEntities.end(); it++)
    {
        NoteData* note = new NoteData(it->second);
        note->SetKey(it->first);
        _cacheNotes.push_back(note);
    }
}

bool NoteSynchronizer::PackPlistEntities(bool isHaveRecord, bool linkAllRecordsOfPulledEntityTypeSentKey)
{
    if (_cacheNotes.empty())
        return false;

    ReleaseSendInfo();

    // 开始写数组内容
    _sendInfo.push_back(CCFConverter::StringtoCFString("SDMessageProcessChanges"));
    _sendInfo.push_back(CCFConverter::StringtoCFString(_dataClass));

    // 写实体内容
    map<string, string> dict;
    dict.clear();

    if (_cacheNotes.empty())
    {
        _sendInfo.push_back(CCFConverter::DictionarytoCFDictionary(dict));
    }
    else
    {
        //map<string,map<string,void*>> temp;
        map<string, void*> temp;
        temp.clear();
        for (size_t i = 0; i < _cacheNotes.size(); i++)
        {
            NoteData* note = _cacheNotes.at(i);
            if (note->GetStatus() == BaseData::NO_CHANGE)
                continue;

            map<string, void*> mapInfo;
            mapInfo.clear();
            note->ConvertToDictInfos(mapInfo);
            if (mapInfo.empty())// 删除
            {
                temp.insert(pair<string, void*>(note->GetKey(), CCFConverter::StringtoCFString(DeviceSyncClient::EMPTY_PARAMETER_STRING)));
                //RemoveFromList(note);
            }
            else // 添加或修改
            {
                temp.insert(pair<string, void*>(note->GetKey(),CCFConverter::DictionarytoCFDictionary(mapInfo)));
            }
        }
        _sendInfo.push_back(CCFConverter::DictionarytoCFDictionary(temp));
    }
    
    // 是否还有数据
    _sendInfo.push_back(CCFConverter::BoolToCFBool(!isHaveRecord));

    // sync actions
    map<string, vector<void*>> dict1;
    dict1.clear();

    vector<void*> linkEntityNamesKey;
    linkEntityNamesKey.clear();
    linkEntityNamesKey.push_back(CCFConverter::StringtoCFString(NOTE_ENTITY_NAME));
    dict1.insert(pair<string,vector<void*>>("SyncDeviceLinkEntityNamesKey",linkEntityNamesKey));

    vector<void*> aaa;
    aaa.clear();
    aaa.push_back(CCFConverter::BoolToCFBool(linkAllRecordsOfPulledEntityTypeSentKey));
    dict1.insert(pair<string,vector<void*>>("SyncDeviceLinkAllRecordsOfPulledEntityTypeSentKey",aaa));

    _sendInfo.push_back(CCFConverter::ConvertStringVectorMap(dict1));
    
    return true;
}

void NoteSynchronizer::DelNote(NoteData* note)
{
    note->SetStatus(BaseData::DEL);
}

NoteData* NoteSynchronizer::AddNote(NoteData* importnote )
{
	NoteData* note = new NoteData();
	note->SetSubject(importnote->GetSubject());
	note->SetContent(importnote->GetContent());
	note->SetContentType(importnote->GetContentType());
	note->SetAuthor(importnote->GetAuthor());
	note->SetDateCreated(importnote->GetDateCreated());
	note->SetDateModified(importnote->GetDateModified());
	note->SetKey(importnote->GetKey());
	_cacheNotes.push_back(note);
	return note;
}

NoteData* NoteSynchronizer::AddNote(wstring subject, wstring content)
{
    NoteData* note = new NoteData();
    note->SetSubject(subject);
    note->SetContent(content);
    note->SetContentType(NoteData::Html);
    _cacheNotes.push_back(note);
    return note;
}

NoteData* NoteSynchronizer::EditNote(NoteData* note, wstring subject, wstring content)
{
    if (find(_cacheNotes.begin(), _cacheNotes.end(), note) == _cacheNotes.end())
        return NULL;

    note->SetStatus(BaseData::UPDATE);
    note->SetSubject(subject);
    note->SetContent(content);
    note->SetContentType(NoteData::Html);
    return note;
}

bool NoteSynchronizer::IsSupportSynchronizer()
{
    if (_device->GetDeviceInfo()->_connectType == WiFi)
    {
        return false;// NoSupportWIFIConnect
    }
    else if (_device->GetDeviceInfo()->_connectType == USB)
    {
        if (IsDisableSync(true))
            return false;// FailToSyncWithDevice
    }

    return true;
}


#endif