#pragma once
/*********************************************************************
����IOS�豸�İ汾��ͬ��ȡ���ݿ���IOS�豸��·��
*********************************************************************/
#include <string>
#include "Core/DeviceData.h"
using namespace std;

namespace ND91Assistant
{
    typedef enum ISDBTYPE
    {
        ISCALLDATADB,       // ��ͨ����¼���ݿ�
        ISMESSAGEDB,        // �Ƕ������ݿ�
        ISCONTACTDB,        // ����ϵ�����ݿ�
        ISCONTACTIMAGEDB,   // ��������ͼƬ���ݿ�
		ISNOTESDB,			// �Ǳ���¼���ݿ�
        ISCALENDERDB,       // ���������ݿ�
		ISBOOKMARKDB        // ��ǩ
    };

    class IosDBPath
    {
    public:
        IosDBPath(const DeviceData* pDevice, ISDBTYPE type);
        ~IosDBPath(void);
 
        wstring GetTempFolder();

        // ��ȡ�豸�汾��
        int GetProductVersion();

        // �豸�Ƿ�Խ��
        bool GetJailBreaked();

        // ��ȡ�豸���к�
        wstring GetDeviceSerial(); 

        // ��ȡϵͳ ����iPad1,1
        wstring GetDeviceProductType();

        // ��ȡ���ݿ�����
        ISDBTYPE GetDBType();
        const DeviceData* GetDeviceData();

        // ��ȡ���ݿ���IOS�豸�ϵ�·��
        wstring GetDBPath();

        // ��ȡ���ݿ���IOS�豸�ϵ�Ŀ¼
        wstring GetDBFolder();

    private:
    #pragma region ͨ����¼���ݿ�

        // ��ȡͨ����¼���ݿ���IOS�豸�ϵ�·��
        wstring GetCallDataDBPath();

        // ��ȡͨ����¼���ݿ���IOS�豸�ϵ�Ŀ¼
        wstring GetCallDataDBFolder();

    #pragma endregion

    #pragma region �������ݿ�

        // ��ȡ�������ݿ���IOS�豸�ϵ�·��
        wstring GetMessageDBPath();

        // ��ȡ�������ݿ���IOS�豸�ϵ�Ŀ¼
        wstring GetMessageDBFolder();

    #pragma endregion

    #pragma region �����˾ݿ�

        // ��ȡ���������ݿ���IOS�豸�ϵ�·��
        wstring GetContactDBPath();

        // ��ȡ���������ݿ���IOS�豸�ϵ�Ŀ¼
        wstring GetContactDBFolder();

		// ��ȡ����¼���ݿ���IOS�豸�ϵ�Ŀ¼
		wstring GetNotesDBFolder();

		// ��ȡ����¼���ݿ���IOS�豸�ϵ�·��
		wstring GetNotesDBPath();

    #pragma endregion

    #pragma region ������ͼƬ���ݿ�

        // ��ȡ������ͼƬ���ݿ���IOS�豸�ϵ�·��
        wstring GetContactImageDBPath();

        // ��ȡ������ͼƬ���ݿ���IOS�豸�ϵ�Ŀ¼
        wstring GetContactImageDBFolder();

    #pragma endregion

#pragma region �������ݿ�

        // ��ȡ�������ݿ���IOS�豸�ϵ�·��
        wstring GetCalenderDBPath();

        // ��ȡ�������ݿ���IOS�豸�ϵ�Ŀ¼
        wstring GetCalenderDBFolder();

#pragma endregion

		wstring GetBookMarkDBPath();

    private:
        int _iProductVersion;           // �汾��
        bool _bJailBreaked;             // �Ƿ�Խ�� 
        wstring _deviceSerial;          // �豸���к�
        wstring _deviceProductType;     // ϵͳ ����iPad1,1

        ISDBTYPE _type;                   // ���ݿ�����
        const DeviceData* _pDevice;      
    };
}
