#pragma once
#include "../cul.h"
#include <map>
#include <string>

// Ϊ����ʹ���ṩOPTION�궨��
#define OPTION GlobalOption::GetInstance()

// ѡ���ȡ�࣬������֧��������������
// ע�⣺��ָ�����ͣ�ֻ�ܱ���ָ�뱾��
class CUL_CLASS_DECL GlobalOption
{
public:

    // ѡ�����ͣ�ÿ��ѡ��������������һ��ֵ
    // Ϊ�����������ʹ���ÿ�������������������Ϳ�ͷ����INT_
    enum OPTION_TYPE {
        STR_LOG_FILE_PATH,              // log�ļ�·��			
        INT_MAX_LOG_FILE_SIZE,          // log�ļ������ֵ
        //IOS
        INT_IOS_CHECK_DEAMON_RUNNING,   // IOS����ػ�ʱ�����е�ʱ����
        STR_IOS_WIFI_CONNECT_PASSWORD,  // WIFI���ӵ�Ĭ������
        STR_IOS_PHOTO_DATABASE_PATH,	// IOS�豸ͼ��ͼƬ��ʼ��photo database�ļ�·��
        //Driver
        STR_DRIVER_INI_PATH,			// ������Ϣ�����ļ�·��
        //Android
        STR_ANDROID_DEAMON_PCPATH,      // Android�ػ���PC·��
        INT_DEAMON_SILENCE_LIMIT,       // �ػ����������ʱ��(ms)����������Ϊ�豸�Ͽ�
        INT_DEAMON_PORT,                // �ػ�ʹ�õĶ˿�
        STR_DEAMON_NAME,                // �ػ���PC����ģ�飩���ֻ��е�package����
        STR_DEAMON_PATH,                // �ػ���PC����ģ�飩���ļ�λ�� ������·����
        STR_PANDASPACE_NAME,            // �ػ�������-ԭ��è�ռ䣩���ֻ��е�package����
        STR_PANDASPACE_PATH,            // �ػ�������-ԭ��è�ռ䣩���ļ�λ�� ������·���� 
        STR_ADB_FILE_APTH,              // ADB.EXE�ļ�λ�ã�����·����
        STR_DLLPATH,					// Assistant.dll ndiPhoneNet.dll��·��
        STR_ANDROID_NPK_TEMP_PATH,      // NPK��װ���̻���Ŀ¼
        INT_ANDROID_IMPORT_CONTACT_COUNT, // ��׿����������ϵ�������������������
        INT_ANDROID_IMPORT_CALLDATA_COUNT,// ��׿��������ͨ����¼�����������������
        INT_ANDROID_IMPORT_MESSAGE_COUNT, // ��׿�����������Ϣ�����������������
        INT_ANDROID_DEAMON_MODE,		// �ػ�ģʽ 0��ʾ�������ػ�ģʽ 1��ʾ�����ػ�ģʽ
        INT_IOS_DEAMON_MODE,		// �ػ�ģʽ 0��ʾ�������ػ�ģʽ 1��ʾ�����ػ�ģʽ
        INT_FILETASK_COUNTMODE,    // �����ļ����ϴ�/���ص�������ģʽ 0��ʾ�������ļ��� 1��ʾ�����ļ���
        STR_PCNAME,                // ���������
        STR_IOS_CONNECT_SHOWTEXT,  // Ios����ȷ������
        INT_IOS_CONNECT_TIMEOUT,   // Ios����ȷ�ϳ�ʱʱ��
        STR_91HELPERDEAMON_LOG,     //NoDeamon�ػ���ַ
        INT_LOG_SWITCH,				//1��0��
    };

    static GlobalOption* GetInstance();
    static void Release();

    // string ����
    void Set(OPTION_TYPE key, const char* value);
    void Set(OPTION_TYPE key, std::string value);
    bool Get(OPTION_TYPE key, std::string& value) const;

    // wstring ����
    void Set(OPTION_TYPE key, const wchar_t* value);
    void Set(OPTION_TYPE key, std::wstring value);
    bool Get(OPTION_TYPE key, std::wstring& value) const;

    // ����ѡ��
    template<class T>
    void Set(OPTION_TYPE key, T value)
    {
        int size = sizeof(T);

        char* pBuffer = new char[size];
        //memcpy_s(pBuffer, size, &value, size);
        memcpy(pBuffer, &value, size);

        _datas[key] = std::string(pBuffer, size);
        delete[] pBuffer;
    }

    // ��ȡѡ��
    // ����ѡ����ڣ�δ���ã����򷵻�false
    template<class T>
    bool Get(OPTION_TYPE key, T& value) const
    {
        std::string s;

        if ( ! Get(key, s) )
            return false;

        //int size = sizeof(T);
        //memcpy_s(&value, size, s.c_str(), s.length());
        memcpy(&value, s.c_str(), s.length());

        return true;
    }

private:
    GlobalOption();
    virtual ~GlobalOption();

    // ��ŵ�������
    static GlobalOption* _pInstance;

#pragma warning(disable : 4251)

    // �洢ÿ��ѡ����Ӧ��ֵ
    std::map<OPTION_TYPE, std::wstring> _wstrings;  // �洢wstring��������
    std::map<OPTION_TYPE, std::string> _datas;      // �洢�����������ݣ�����string����
};
