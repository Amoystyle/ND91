#pragma once

namespace ND91Assistant
{
    // 一个Apple ID对应的授权文件信息
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

        wstring     _accountID;         // 账户ID
        wstring     _IDs;               // 逗号分割的账户列表
        wstring     _softID;            // 应用ID;
        wstring     _iTunesVersion;     // 创建这个SCInfo对应iTunes版本
        bool        _isLocalCached;     // 本地是否有缓存
        wstring     _url;               // 授权文件所在地址
        wstring     _data;              // 本scinfo对应的相关信息
        wstring     _serverParams;      // 授权需要的两个参数
        time_t      _createDate;        // 本授权文件创建时间

        wstring     _folder;            // 本地缓存路径
    };
}