#pragma once

#define CHECK_EXIT  if (CheckExit()) return 0;
#define WAIT_EXIT(t)   if (WaitExit(t)) return 0;

// �̻߳��࣬ͨ��ThraedManager���Ը���ID���ָ��
class SINGLETON_API BaseThread
{
    friend class ThreadManager;
public:

    BaseThread(const string id=""); // id���߳�id�����ظ��ķǿ��ַ���
    virtual ~BaseThread();

    // �趨�Ƿ����߳̽���ʱɾ��������
    void SetAutoDelete(bool value);

    // �����̣߳����سɹ���ʧ��
    bool Run();

    // ֹͣ�߳�
    void Exit();

	HANDLE          _hThread;       // �߳̾��

    int GetGUID() { return _nGUID; };
protected:
    wstring         _deviceID;      // ��Ӧ���豸id,Ϊ�ձ�ʾ�޶�Ӧ�豸���豸�Ͽ�ʱ������ֹ��Ӧ���߳�
    int             _nGroup;        // �̷߳���
    int             _nGUID;     // �̱߳�־

    
    string          _id;            // �߳�ID

    
    // �������˳����ƣ��������ʹ�ö�����������ֱ��ʹ�ñ���

    // ����˳���־
    virtual bool CheckExit();

    // �ȴ�ϵͳ�˳����û����Լ�����Wait����
    // return : bool �Ƿ�Ҫ�˳�
    virtual bool WaitExit(DWORD dwMilliseconds=INFINITE);

    HANDLE      _hExitEvent;        // �߳��˳��¼�  
    bool        _exit;              // �߳��˳���־����

    HANDLE      _hSystemExitEvent;  // ϵͳ�˳��¼�  
    const bool* _pSystemExit;       // ϵͳ�˳���־����

    bool        _autoDelete;        // �߳̽���ʱɾ��������
private:

    // �߳�ִ�к���    
    static DWORD WINAPI ThreadMain( LPVOID lpParam ) ;

	void InitPrintThreadID();

	string strPrintThreadID;
    // ʵ���߳�ִ�к���������ʵ��
    virtual unsigned long Main() = 0;

};