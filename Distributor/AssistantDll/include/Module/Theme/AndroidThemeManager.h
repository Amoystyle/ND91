#pragma once

#include "ThemeManager.h"
#include "Monitor/DeamonMessageObserver.h"

namespace ND91Assistant
{
    // Android���������
    class CLASSINDLL_CLASS_DECL AndroidThemeManager : public ThemeManager, public DeamonMessageObserver
    {
    public:
        AndroidThemeManager(const DeviceData* pDevice);
        ~AndroidThemeManager(void);

    public: // �̳��� DeamonMessageSubject
        // �¶���֪ͨ
        void NewMessage(const MessageData* msg) {};
        // ���Ż�ִ֪ͨ
        void MessageReceipt(const MessageReceiptData* pMsgData) {};
        // ��è�������֪ͨ
        void PandaThemeAction(const PandaThemeActionReturn* pData);

    public: // �̳��� ThemeManager
        // Summary  : ��ʼ���ֻ���������Ҫ���ж�������Ҫ��װӦ��
        // Returns  : ND91Assistant::eThemeInitCode �����س�ʼ��������Ϣ
        virtual eThemeInitCode InitEnvironment();

        // Summary  : ���������б�
        // Returns  : void
        virtual bool Load(); //virtual void LoadThemes(vector<wstring>& vtThemeId) = 0; 

        // Summary  : ��ȡ������ϸ��Ϣ
        // Returns  : ThemeData* ������Ϣ�������������ͷ�
        // Parameter: wstring strThemeId ������ID
        virtual bool GetThemeDetailInfo(ThemeData* pThemeData); //virtual ThemeData* GetThemeDetailInfo(wstring strThemeId) = 0; 

        // Summary  : ��������Ԥ��ͼ
        // Returns  : bool ���ɹ�����TRUE
        // Parameter: wstring strFullNameOnPc ��Ԥ��ͼ�ڱ��ص����ƣ���·��)
        // Parameter: ThemeData * ��������Ϣ
        virtual bool LoadPreviewPhoto(ThemeData* pThemeData);

        // Summary  : ��ȡ��ǰ����
        // Returns  : ThemeData* �����ص�ǰ����
        virtual ThemeData* GetCurTheme();

        // Summary  : ������������
        // Returns  : bool �ɹ�����TRUE
        // Parameter: ThemeData * pThemeData ��������Ϣ
        // Parameter: wstring strNewName ����������
        virtual bool Rename(ThemeData* pThemeData, wstring strNewName);

        // Summary  : ���õ�ǰ����
        // Returns  : bool �ɹ�����TRUE
        // Parameter: ThemeData * pThemeData ��������Ϣ
        virtual bool DeleteTheme(ThemeData* pThemeData);

        // Summary  : �������
        // Returns  : bool �ɹ�����TRUE
        // Parameter: wstring strLocalFile ������·�����ļ�����IOSΪ�ļ��У�ANDROIDΪ�ļ���
        // Parameter: wstring strPreviewFile ��Ԥ��ͼ�ļ���
        // Parameter: ProgressObserver* pObserver ����������
        virtual ThemeData* AddLocalTheme(wstring strLocalFile, ProgressObserver* pObserver = NULL, wstring strPreviewFile = L"");

        // Summary  : ������������
        // Returns  : wstring ���ݺ�ı����ļ�����IOSΪ�ļ��У�ANDROIDΪ�ļ�����ʧ�ܷ���L""
        // Parameter: ThemeData * pThemeData ��������Ϣ
        // Parameter: wstring SavePath Ҫ�洢�����ص�Ŀ¼
        virtual wstring BackupTheme(ThemeData* pThemeData, wstring SavePath);

        // Summary  : ���õ�ǰ����
        // Returns  : bool �ɹ�����TRUE
        // Parameter: ThemeData * pThemeData ��������Ϣ
        virtual bool SetCurTheme(ThemeData* pThemeData);

        // Summary  : ������ǰ����
        // Returns  : bool �ɹ�����TRUE
        virtual bool RevocationTheme();

        // Summary  : �ж��Ƿ�Ϊ����Ŀ¼��PC�ˣ�
        // Returns  : bool ������Ŀ¼�򷵻�TRUE
        // Parameter: wstring strPath ��PC�ϵ�Ŀ¼
        virtual bool CheckIsThemePath(wstring strPath);

        // �ͷţ���������
        virtual void Release();
    private:
		enum ActionType
		{
			ENone = 0,
			EActionSetTheme = 1,
			EActionDelTheme = 2,
			EActionAddTheme = 3
		};
        PandaThemeActionReturn* _pActionReturnData;
        ActionType              _curActionType;//      _bActionReturnData;
        ThemeData*              _pCurrentTheme; //��ǰ����

        // �ȴ��ػ����ؽ��
        bool WaitForDeamonReturn(int nWaitTime = 15000);
        // ���������б�
        void LoadAllThemes();
        // Summary  : ����ȫ������id�б�2.0���ϰ汾ʹ��
        // Returns	: void
        ThemeBriefInfo LoadThemeBreifInfo();

		void LoadSmartThemes();

        // Summary  : ���apk����
        // Returns	: bool
        // Parameter: wstring localApkPath
        ThemeData* AddAPKTheme(wstring localApkPath);

        // Summary  : ���apt����
        // Returns	: bool
        // Parameter: wstring localAptPath
        ThemeData* AddAptTheme(wstring localAptPath,bool IsSmart=false);

		int AppInstallStatus(wstring pkgname, wstring minVersion);
    };
}