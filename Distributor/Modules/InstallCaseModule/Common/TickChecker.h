#pragma once

INSTALLCASE_BEGIN

#define INITTICKCHECKER TickChecker tick;
#define TICKCHECKER tick.InsertTick();
#define TICKSTRING   tick.ToString()
class TickChecker
{
public:
    TickChecker() {
        _lastTick = ::GetTickCount();;
    };
    ~TickChecker() {};

    void InsertTick()
    {
        int nTick = ::GetTickCount();
        _tickList.push_back(nTick - _lastTick);
        _lastTick = nTick;
    };
    wstring ToString()
    {
        wstring str = L"";
        for (vector<int>::iterator it = _tickList.begin(); it != _tickList.end(); it++)
        {
            str += CStrOperation::IntToWString(*it) + L" - ";
        }
        return str;
    }

private:
    vector<int> _tickList;
    int _lastTick;
};

INSTALLCASE_END