#pragma once
#include <string>
using namespace std;

#include "Module/BaseData.h"

namespace ND91Assistant
{
    class CLASSINDLL_CLASS_DECL NoteData : public BaseData
    {
    public:
        NoteData();
        NoteData(map<string,void*> mapEntities);
        ~NoteData();

        void ConvertToDictInfos(map<string, void*>& infoss);

        // 备忘录内容格式
        enum ContentType
        {
            Plain,  // 没有换行啥的,就这普通的Plain
            Html,   // 有换行
            Unknow
        };

        wstring     GetAuthor();
        void        SetAuthor(wstring author);
        wstring     GetSubject();
        void        SetSubject(wstring subject);
        wstring     GetContent();
        void        SetContent(wstring content);
        ContentType GetContentType();
        void        SetContentType(ContentType contentType);
        time_t      GetDateCreated();
        void        SetDateCreated(time_t dateCreated);
        time_t      GetDateModified();
        void        SetDateModified(time_t dateModified);
        string      GetKey();
        void        SetKey(string key);
        void        SetStatus(DATA_STATUS s);
        DATA_STATUS GetStatus() const;

    private:
        wstring FormatContentStr(string content);
        ContentType FormatContentType(string contentType);

    private:
        wstring          _author;           // 设备拥有者
        wstring          _subject;          // 主题
        wstring          _content;          // 内容
        ContentType      _contentType;      // 内容类型
        time_t           _dateCreated;      // 创建时间
        time_t           _dateModified;     // 修改时间
        string           _key;              // 备忘录在设备上的ID

        static long     _idIncrementer;     // 备忘录在PC上的ID
        string          _recordEntityName;
    };
}
