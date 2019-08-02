#pragma once
App_CC_BEGIN
// 进入时，bool值会置为true, 退出时置为false, 与LPCRITICAL_SECTION不同的地方在于，可以让其他的操作马上退出，不再进入后续的操作
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