#pragma once
App_CC_BEGIN
// ����ʱ��boolֵ����Ϊtrue, �˳�ʱ��Ϊfalse, ��LPCRITICAL_SECTION��ͬ�ĵط����ڣ������������Ĳ��������˳������ٽ�������Ĳ���
#define CheckBLock(lock, value) if (lock) return value;
#define SetBLock(lock) BoolLock boolLock(&lock);
#define SetBLockReturn(lock, value) CheckBLock(lock, value); BoolLock boolLock(&lock);
#define WaitBLock(lock,t) BoolLock::Wait(&lock,t);
class BoolLock
{
public:
    BoolLock(bool* pLock)
    {
        _pLock = pLock;
        *_pLock = true;
    };
    ~BoolLock() 
    {
		*_pLock = false;
	};

    static void Wait(bool* pLock, int t=INFINITE)
    {
        int nTick = ::GetTickCount();
        while (*pLock && abs(::GetTickCount() - nTick) < INFINITE)
        {
            Sleep(100);
        }
    }
private:
    bool	*_pLock;
};
App_CC_END