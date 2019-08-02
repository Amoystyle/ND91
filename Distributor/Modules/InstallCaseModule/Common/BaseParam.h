#pragma once

INSTALLCASE_BEGIN
class VirtualObServer;

// 参数类封装
class BaseParam
{
public:
    BaseParam();
    BaseParam(void* pParent, int nAction, void* pProgress, BaseParam* pInitParam=NULL);    
    virtual ~BaseParam();

    // 添加参数
    void AddVoidParam(void* par);
    void AddStrParam(wstring par);
    void AddIntParam(int par);
    void SetParent(void* pParent);
    void SetProgress(void* pProgress);
    void SetAction(int nAction);
    void SetStr(wstring str) { _strParam = str; };
    void SetVoid(void* p) { _voidParam = p;};
    void SetInt(int i) { _intParam = i;};
    void SetBool(bool b) {_boolParam = b;};

    // 取得参数
    const vector<void*>* GetVoidParam() const;
    const vector<wstring>* GetStrParam() const;
    const vector<int>* GetIntParam() const;
    void* GetParent() const;
    void* GetProgress() const;
    int GetAction() const;
    wstring GetStr() const { return _strParam; };
    void* GetVoid() const { return _voidParam;};
    int GetInt() const { return _intParam;};
    bool GetBool() const { return _boolParam;};

	VirtualObServer* GetObserver() {return _pObserver;}
	void SetObserver(VirtualObServer* pob){ _pObserver = pob;}
    
protected:
    void* _pParent;     // 所有着
    void* _pProgress;   // 进度窗口
    int   _nAction;     // 行为

    vector<void*>   _voidList;
    vector<wstring> _strList;
    vector<int>     _intList;
    int _voidIndex;
    int _strIndex;
    int _intIndex;

    wstring _strParam;
    void* _voidParam;
    int _intParam;
    bool _boolParam;

	VirtualObServer* _pObserver;
};
INSTALLCASE_END