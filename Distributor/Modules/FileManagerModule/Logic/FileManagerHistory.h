#pragma once

// ����¼���ٸ������ʷ
#define  MAX_HISTORY_COUNT 128

// �ļ�����ģ�� �����ʷ������
class FileManagerHistory
{
public:
    FileManagerHistory(unsigned int maxCount);

    // �Ƿ���ͣ�����µ���λ�ã�ǰ��������ʱʹ�ã�
    void EnableGoto(bool val) ;

    void    Goto(wstring path); // ��ת����λ��
    wstring Prev();             // ����
    wstring Next();             // ǰ��
    wstring GetCurrent();       // ��ǰ
    bool    IsHead();           // �Ƿ��Ѿ�����һ��
    bool    IsTail();           // �Ƿ��Ѿ������һ��
    void    DeleteCurrent();    // ɾ����ǰ·��

private:
    bool            _enableGoto;    // ����
    unsigned int    _maxCount;  // ��ౣ����ٸ���ʷ��¼
    list<wstring>   _history;   // �����ʷ

    list<wstring>::const_iterator     _current;    // ��ǰλ��
};
