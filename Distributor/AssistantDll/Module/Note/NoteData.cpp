#include "pch_module.h"

#ifdef MODULE_NOTE

#include "Module/Note/NoteData.h"
#include "Module/Note/NoteSynchronizer.h"

#include "Device/CFConverter.h"
#include "Module/Common/Synchronizer/DeviceSyncClient.h"

long NoteData::_idIncrementer  = 1;
NoteData::NoteData()
{
    char temp[125] = {0};
    sprintf_s(temp, "Note/ND%d", _idIncrementer++);
    _key = temp;

    SetStatus(NEW);
    _contentType = Plain;

    time_t nowTime = 0;
    _dateCreated = _dateModified = ::time(&nowTime);

    _author = L"";
    _subject = L"";
    _content = L"";
    _recordEntityName= "com.apple.notes.Note";
}

NoteData::NoteData(map<string,void*> mapEntities)
{
    for (map<string, void*>:: iterator it = mapEntities.begin(); it != mapEntities.end(); it++)
    {
        string str = "";
        if (CCFConverter::NDCFGetTypeID(it->second) == CCFConverter::NDCFStringGetTypeID())
            CCFConverter::GetIosData(it->second, str);

        if (it->first == "content")
            _content = FormatContentStr(str);
        else if (it->first == "contentType")
            _contentType = FormatContentType(str);
        else if (it->first == "dateCreated")
            CCFConverter::GetIosData(it->second, _dateCreated);
        else if (it->first == "subject")
            _subject = CCodeOperation::GB2312ToUnicode(str);
        else if (it->first == "author")
            _author = CCodeOperation::GB2312ToUnicode(str);
        else if (it->first == "dateModified")
            CCFConverter::GetIosData(it->second, _dateModified);
        else if (it->first == "com.apple.syncservices.RecordEntityName")
            _recordEntityName = str;
    }
    SetStatus(NO_CHANGE);
}

NoteData::~NoteData()
{
}

wstring NoteData::GetAuthor()
{
    return _author;
}

void NoteData::SetAuthor(wstring author)
{
    _author = author;
}

wstring NoteData::GetSubject()
{
    return _subject;
}

void NoteData::SetSubject(wstring subject)
{
    _subject = subject;
}

wstring NoteData::GetContent()
{
    return _content;
}

void NoteData::SetContent(wstring content)
{
    _content = content;
}

NoteData::ContentType NoteData::GetContentType()
{
    return _contentType;
}

void NoteData::SetContentType(ContentType contentType)
{
    _contentType = contentType;
}

time_t NoteData::GetDateCreated()
{
    return _dateCreated;
}

void NoteData::SetDateCreated(time_t dateCreated)
{
    _dateCreated = dateCreated;
}

time_t NoteData::GetDateModified()
{
    return _dateModified;
}

void NoteData::SetDateModified(time_t dateModified)
{
    _dateModified = dateModified;
}

void NoteData::SetStatus( DATA_STATUS s )
{
    time_t nowTime = 0;
    BaseData::SetStatus(s);
    if (BaseData::GetStatus() == UPDATE)
        _dateModified = ::time(&nowTime);
}

BaseData::DATA_STATUS NoteData::GetStatus() const
{
    return BaseData::GetStatus();
}

string NoteData::GetKey()
{
    return _key;
}

void NoteData::SetKey(string key)
{
    _key = key;
}

void NoteData::ConvertToDictInfos(map<string, void*>& infoss)
{
    if (GetStatus() == DEL)
        return ;

    infoss.clear();
    infoss.insert(pair<string, void*>("author", CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(_author))));
    infoss.insert(pair<string, void*>(DeviceSyncClient::RECORD_ENTITY_NAME_KEY, CCFConverter::StringtoCFString(NoteSynchronizer::NOTE_ENTITY_NAME)));
    infoss.insert(pair<string, void*>("content", CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(L"<div>"+_content+L"</div>"))));
    switch (_contentType)
    {
    case Html:
        infoss.insert(pair<string, void*>("contentType", CCFConverter::StringtoCFString("text/html")));
        break;
    case Plain:
    case Unknow:
    default:
        infoss.insert(pair<string, void*>("contentType", CCFConverter::StringtoCFString("text/plain")));
        break;
    }

    infoss.insert(pair<string, void*>("dateCreated", CCFConverter::TimeToCFDate(_dateCreated)));
    infoss.insert(pair<string, void*>("dateModified",CCFConverter::TimeToCFDate(_dateModified)));
    infoss.insert(pair<string, void*>("subject", CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(_subject))));
}

wstring NoteData::FormatContentStr(string content)
{
    wstring temp = L"";
    temp = CCodeOperation::GB2312ToUnicode(content);
    temp = CStrOperation::trimLeftW(temp, L"<div>");
    temp = CStrOperation::trimRightW(temp, L"</div>");
    temp = CStrOperation::trimRightW(temp, L"<br");
    return temp;
}

NoteData::ContentType NoteData::FormatContentType(string contentType)
{
    if (contentType == "text/html")
        return Html;
    else if (contentType == "text/plain")
        return Plain;

    return Unknow;
}


#endif