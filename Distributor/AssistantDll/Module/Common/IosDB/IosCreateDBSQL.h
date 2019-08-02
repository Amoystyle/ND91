#pragma once

/*********************************************************************
����IOS�豸�İ汾��ͬ��ȡ�������ݿ��SQL���
*********************************************************************/
#include <string>
using namespace std;

class IosCreateDBSQL
{
public:
    IosCreateDBSQL(void);
    ~IosCreateDBSQL(void);

public:
    // ����ͨ����¼���ݿ����
    static string CreateCallDataDBSQL();

	// ������ǩ���ݿ�sql
	static string CreateBookMarkDBSQL();

    // �����������ݿ����
    static string CreateMessageDBSQL(int iProductVersion);

    // �������������ݿ����
    static string CreateContactDBSQL(int iProductVersion);

    // ����������ͼƬ���ݿ����
    static string CreateContactImageDBSQL(int iProductVersion);

    // �����������ݿ����
    static string CreateCalenderDBSQL(int iProductVersion);

    // ɾ���������ݿⴥ�������
    static string DropSmsTriggersSQL();    

    // �����������ݿⴥ�������
    static string CreateSmsTriggersSQL();

    // ��װ���򲹶����(1X�汾)
    static string InstallPatchSQL_1X();

    // ��װ���򲹶����(2X��3X�汾)
    static string InstallPatchSQL_2X_3X();

    // ��װ���򲹶����(4X�汾)
    static string InstallPatchSQL_4X();

    // ж�����򲹶����(1X�汾)
    static string UnInstallPatchSQL_1X();

    // ж�����򲹶����(2X,3X,4X�汾)
    static string UnInstallPatchSQL_2X_3X_4X();
    
private:
    // �������������ݿ�1X�汾���
    static string CreateContact1XDBSQL();

    // �������������ݿ�2X�汾���
    static string CreateContact2XDBSQL();

    // �������������ݿ�3X�汾���
    static string CreateContact3XDBSQL();

    // �������������ݿ�4X�汾���
    static string CreateContact4XDBSQL();

    // �������������ݿ�4.3.X�汾���
    static string CreateContact43XDBSQL();

    // ����������ͼƬ���ݿ�1X�汾���
    static string CreateContactImage1XDBSQL();

    // ����������ͼƬ���ݿ�2X�汾���
    static string CreateContactImage2XDBSQL();

    // ����������ͼƬ���ݿ�3X�汾���
    static string CreateContactImage3XDBSQL();

    // ����������ͼƬ���ݿ�4X�汾���
    static string CreateContactImage4XDBSQL();

    // �����������ݿ�1X�汾���
    static string CreateCalender1XDBSQL();

    // �����������ݿ�2X�汾���
    static string CreateCalender2XDBSQL();

    // �����������ݿ�3X�汾���
    static string CreateCalender3XDBSQL();

};
