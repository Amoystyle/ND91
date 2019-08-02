#pragma once
#include "../cul.h"
#ifdef WIN32
    #include <windows.h>
    #define pthread_mutex_t HANDLE
#else
    #include <common/winPort.h>
#endif
#include <stdio.h>
#include <string>

using namespace std;
#pragma warning(disable : 4251)

#define LOG Log::GetInstance()

#define LOG_DEBUG(x)    if (Log::GetInstance()->GetLevel() <= Log::LOG_DEBUG) Log::GetInstance()->WriteDebug(x);
#define LOG_ERROR(x)    if (Log::GetInstance()->GetLevel() <= Log::LOG_ERROR) Log::GetInstance()->WriteError(x);
#define LOG_ALERT(x)    if (Log::GetInstance()->GetLevel() <= Log::LOG_ALERT) Log::GetInstance()->WriteAlert(x);

// �������������
#define LOG_BINARY(x, y)    if (Log::GetInstance()->GetLevel() <= x) Log::GetInstance()->WriteBinary(x, y);

#define LOG_FUNTCION_BEGIN  {wstringstream woss;woss << "Begin Function " << __FUNCTION__;LOG_DEBUG(woss.str());}
#define LOG_FUNTCION_END  {wstringstream woss;woss << "End Function " << __FUNCTION__;LOG_DEBUG(woss.str());}

// LOG������
// ����������ʹ��LOG��
class CUL_CLASS_DECL Log
{
public:
    enum LOG_LEVEL { LOG_DEBUG, LOG_ERROR, LOG_ALERT };

    // �Զ�����LOG����
    // ����ļ�·�������� STR_LOG_FILE_PATH ѡ��
    // ����ļ���С������ INT_MAX_LOG_FILE_SIZE ѡ��
    // ��δ�������Զ��ڳ�������Ŀ¼����assistant.log�ļ�
    static Log* GetInstance();
    static void Release();

#pragma region ���Ʒ���

    // ��������Log���ı�����ļ�λ�û��Сʱʹ�ã�
    void Restart();

    // ֹͣLog
    void Stop();

    // ��������ȼ�: Ŀǰ֧�ֵȼ��ɵ͵���Ϊ: DEBUG, ERROR, ALERT
    // Ĭ��Ϊ��ͼ���DEBUG, ������Ϊ�ߵȼ�����͵ȼ������������
    void SetLevel(LOG_LEVEL level);
    LOG_LEVEL GetLevel() { return _level; }

#pragma endregion

#pragma region ���LOG�ķ���


    // ���������Ϣ��������ȼ���
    void WriteDebug(wstring msg);

    // ���������Ϣ
    // ����Exceptionʱ�Զ�����˼�����Ϣ
    void WriteError(wstring msg);

    // �����ʾ��Ϣ��������ȼ���
    void WriteAlert(wstring msg);

    // ��������ͬprintf�Ŀɱ��������
    void Write(LOG_LEVEL level, const wchar_t* fmt, ... );

    // �������������������
    void WriteBinary(LOG_LEVEL level, string buffer);

#pragma endregion

private:
    Log(void);
    virtual ~Log(void);

    // ����Log����
    // ����path���ļ�������������·����
    void Start();

    void Write(wstring msg, LOG_LEVEL level);

    void GetOption();

    void CreateFile();

    void CheckFileSize();

private:
    static Log* _pInstance; // ����ʵ��
    LOG_LEVEL   _level;     // ����ȼ�
    wstring     _path;      // log�ļ���������ʱ�����չ����
    wstring     _filename;  // log�ļ�������ʱ�����չ����
    int         _maxSize;   // log�ļ���󳤶�
    FILE*       _pFile;     // log�ļ�ָ��

    pthread_mutex_t      _mutex;     // ��
};
