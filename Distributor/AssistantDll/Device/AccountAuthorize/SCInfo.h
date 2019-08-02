#pragma once

namespace ND91Assistant
{
    // һ��Apple ID��Ӧ����Ȩ�ļ���Ϣ
    class  Authorization
    {
    public:
        Authorization();
        Authorization(const char* data[7]);
        ~Authorization();

        wstring ToString();

        wstring GetAccountID() { return _accountID; }
        void SetAccountID(wstring accountID) { _accountID = accountID; }

        wstring GetIDS() { return _IDs; }
        void SetIDS(wstring IDs) { _IDs = IDs; }

        wstring GetSoftID() { return _softID; }
        void SetSoftID(wstring softID) { _softID = softID; }

        wstring GetITunesVersion() { return _iTunesVersion; }
        void SetITunesVersion(wstring iTunesVersion) { _iTunesVersion = iTunesVersion; }

        bool GetIsLocalCached() { return _isLocalCached; }
        void SetIsLocalCached(bool isLocalCached) { _isLocalCached = isLocalCached; }

        wstring GetUrl() { return _url; }
        void SetUrl(wstring url) { _url = url; }

        wstring GetData() { return _data; }
        void SetData(wstring data) { _data = data; }

        wstring GetServerParams() { return _serverParams; }
        void SetServerParams(wstring serverParams) { _serverParams = serverParams; }

        time_t GetCreateDate() { return _createDate; }
        void SetCreateDate(time_t createDate) { _createDate = createDate; }

        wstring GetFolder();
        void SetFolder(wstring folder);

    private:

        wstring     _accountID;         // �˻�ID
        wstring     _IDs;               // ���ŷָ���˻��б�
        wstring     _softID;            // Ӧ��ID;
        wstring     _iTunesVersion;     // �������SCInfo��ӦiTunes�汾
        bool        _isLocalCached;     // �����Ƿ��л���
        wstring     _url;               // ��Ȩ�ļ����ڵ�ַ
        wstring     _data;              // ��scinfo��Ӧ�������Ϣ
        wstring     _serverParams;      // ��Ȩ��Ҫ����������
        time_t      _createDate;        // ����Ȩ�ļ�����ʱ��

        wstring     _folder;            // ���ػ���·��
    };
}