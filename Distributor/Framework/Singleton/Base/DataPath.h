#pragma once

// ȡ��Ŀ¼�Ĺ�������
class SINGLETON_API DataPath
{
public:
    // ���û�����Ŀ¼�����ҵ��ĵ�\CompanyName\ProductName\ 
    static wstring GetDataDir();

    // ����ʱ����Ŀ¼����ϵͳ��ʱĿ¼\ProductName\ 
    static wstring GetTempDir();

    // ���Ψһ����ʱ����Ŀ¼�������߸���ɾ����
    static wstring GetUniqueTempDir();

private:
    static wstring _dataPath;   // �û�����Ŀ¼
    static wstring _tempPath;   // ��ʱ����Ŀ¼
};