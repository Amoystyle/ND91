#pragma once
#include <vector>
#include <map>
using namespace std;

namespace ND91Assistant
{
    enum IosVersion              // �豸�汾    
    {
        VERSION_OTHER,
        VERSION_1,
        VERSION_2,
        VERSION_3,
        VERSION_4,
        VERSION_43,
    };

    class IosContactDB;
    class IosAutoSort
    {
    public:
        IosAutoSort(IosContactDB *contactDB);
        ~IosAutoSort();

    public:
        // Summary  : ��ȡƴ����汾
        // Returns  : std::wstring �汾��
        wstring GetPinyinVersion();

        // Summary  : ��װһ������
        // Returns  : bool true:�ɹ� false:ʧ��
        // Parameter: wstring pyVersion ƴ���汾
        bool Patch_Install(wstring pyVersion);

        // Summary  : ж��һ������
        // Returns  : bool true:�ɹ� false:ʧ��
        bool Patch_UnInstall();

    private:

        // ��ȡIOS�豸�汾
        IosVersion GetIosVersion();

        // Summary  : ��װһ������(1X�汾)
        // Returns  : bool true:�ɹ� false:ʧ��
        // Parameter: wstring pyVersion ƴ���汾
        bool SortPathInstall_For_1X(wstring pyVersion);

        // Summary  : ��װһ������(2X,3X�汾)
        // Returns  : bool true:�ɹ� false:ʧ��
        // Parameter: wstring pyVersion ƴ���汾
        // Parameter: bool is2Xor3X true:2x false:3x
        bool SortPathInstall_For_2X_3X(wstring pyVersion,bool is2Xor3X);

        // Summary  : ��װһ������(4X,43X�汾)
        // Returns  : bool true:�ɹ� false:ʧ��
        // Parameter: bool is4Xor43X true:4X false:43X
        bool SortPatchInstall_For_4X_43X(bool is4Xor43X);

        // Summary  : ж��һ������(1X�汾)
        // Returns  : bool true:�ɹ� false:ʧ��
        bool SortPatchUnInstall_For_1X();

        // Summary  : ж��һ������(2X,3X,4X�汾)
        // Returns  : bool true:�ɹ� false:ʧ��
        bool SortPatchUnInstall_For_2X_3X_4X();

        // Summary  : ��װ���(1X�汾)
        // Returns  : void 
        // Parameter: wstring filePath ����ļ�·��
        void InstallCodeTable_1X(wstring filePath);

        // Summary  : ��װ���(2X,3X,4X�汾)
        // Returns  : void
        // Parameter: wstring filePath ����ļ�·��
        // Parameter: map<char, int> iPhone30SortDic ����������ֵ�
        // Parameter: bool is2Xor3X true:2x false:3x true:2x false:3x
        void InstallCodeTable_2X_3X_4X(wstring filePath,map<char, int> iPhone30SortDic,bool is2Xor3X);

        // Summary  : ��ȡƴ�����phoneticizeֵ
        // Returns  : string 
        // Parameter: string pystr py.dat�ļ��е�ƴ���ַ���
        // Parameter: map<char, int> iPhone30SortDic  ����������ֵ�
        // Parameter: int * languageIndex ƴ�����languageIndexֵ
        // Parameter: bool is2Xor3X true:2x false:3x
        string MakeSortSection(string pystr, map<char, int> iPhone30SortDic, int *languageIndex, bool is2Xor3X);

        // Summary  : ��ȡƴ�����phoneticizeֵ(2X�汾)
        // Returns  : string 
        // Parameter: string pystr py.dat�ļ��е�ƴ���ַ���
        // Parameter: map<char, int> iPhone30SortDic ����������ֵ�
        // Parameter: int * languageIndex ƴ�����languageIndexֵ
        string MakeSortSection_For_2X(string pystr, map<char, int> iPhone30SortDic, int *languageIndex);

        // Summary  : ��ȡƴ�����phoneticizeֵ(3X�汾)
        // Returns  : string
        // Parameter: string pystr py.dat�ļ��е�ƴ���ַ���
        // Parameter: map<char, int> iPhone30SortDic ����������ֵ�
        // Parameter: int * languageIndex ƴ�����languageIndexֵ
        string MakeSortSection_For_3X(string pystr, map<char, int> iPhone30SortDic, int *languageIndex);

        // Summary  : ����Ƿ��������
        // Returns  : bool true:���� false: ������
        // Parameter: const char * sql ����SQL���
        bool CheckHasData(const char* sql);

        // Summary  : ��ȡƴ���汾
        // Returns  : std::wstring ƴ���汾
        // Parameter: const char * sql SQL���
        wstring SelectPinyinVersion(const char* sql);

        // Summary  : ����Ƿ�װ��һ�����򲹶�
        // Returns  : bool true:�Ѱ�װ false:δ��װ
        bool IsPatched();

        // Summary  : ���ļ����ݸ�ʽתΪָ���ĸ�ʽ�� "��", "A " -> "'��','A '" 
        // Returns  : std::wstring  ת��ǰ���ַ���
        // Parameter: wstring fileContext ת������ַ���
        wstring PaddingPinyin(wstring fileContext);

    private:
        IosContactDB *      _contactDB;             // ���������ݿ�
        wstring             _currWorkingDir;        // ��ǰ����Ŀ¼
    };
}