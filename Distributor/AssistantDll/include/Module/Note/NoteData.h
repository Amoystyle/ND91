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

        // ����¼���ݸ�ʽ
        enum ContentType
        {
            Plain,  // û�л���ɶ��,������ͨ��Plain
            Html,   // �л���
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
        wstring          _author;           // �豸ӵ����
        wstring          _subject;          // ����
        wstring          _content;          // ����
        ContentType      _contentType;      // ��������
        time_t           _dateCreated;      // ����ʱ��
        time_t           _dateModified;     // �޸�ʱ��
        string           _key;              // ����¼���豸�ϵ�ID

        static long     _idIncrementer;     // ����¼��PC�ϵ�ID
        string          _recordEntityName;
    };
}
