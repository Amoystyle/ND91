#pragma once

#include "Module/Theme/ThemeData.h"

namespace ND91Assistant
{
	typedef enum eThemeInitCode
	{
		eThemeInitCode_Success, //�Ѱ�װ���ɹ�
		eThemeInitCode_Ios_UnJailBreaked, //δԽ��
		eThemeInitCode_Ios_UnInstall_SummerBoard, //δװSummerBoard
		eThemeInitCode_Ios_UnInstall_WinterBoard, //δװWinterBoard
		eThemeInitCode_Ios_UnInstall_PandaHome, // δװ��è����
		eThemeInitCode_Error, //��������
		eThemeInitCode_Android_NotSupport,
		eThemeInitCode_Android_NotSupport_SmartHome,
		eThemeInitCode_Android_NeedInstall_SmartHome,
		eThemeInitCode_Android_NeedInstall_PandaHome_Two,
		eThemeInitCode_Android_NeedInstall_BothHome,
	};

	class CLASSINDLL_CLASS_DECL ThemeManager
	{
	public:
		ThemeManager(const DeviceData* pDevice);
		~ThemeManager(void);

		// Summary  : ��ʼ���ֻ���������Ҫ���ж�������Ҫ��װӦ��
		// Returns  : ND91Assistant::eThemeInitCode �����س�ʼ��������Ϣ
		virtual eThemeInitCode InitEnvironment() = 0;

		// Summary  : ���������б�
		// Returns  : void
		virtual bool Load() = 0; //virtual void LoadThemes(vector<wstring>& vtThemeId) = 0; 

		// Summary  : ��ȡ������ϸ��Ϣ
		// Returns  : ThemeData* ������Ϣ�������������ͷ�
		// Parameter: wstring strThemeId ������ID
		virtual bool GetThemeDetailInfo(ThemeData* pThemeData) = 0; //virtual ThemeData* GetThemeDetailInfo(wstring strThemeId) = 0; 

		// Summary  : ��������Ԥ��ͼ
		// Returns  : bool ���ɹ�����TRUE
		// Parameter: wstring strFullNameOnPc ��Ԥ��ͼ�ڱ��ص����ƣ���·��)
		// Parameter: ThemeData * ��������Ϣ
		virtual bool LoadPreviewPhoto(ThemeData* pThemeData) = 0;

		// Summary  : ��ȡ��ǰ����
		// Returns  : ThemeData* �����ص�ǰ����
		virtual ThemeData* GetCurTheme() = 0;

		// Summary  : ������������
		// Returns  : bool �ɹ�����TRUE
		// Parameter: ThemeData * pThemeData ��������Ϣ
		// Parameter: wstring strNewName ����������
		virtual bool Rename(ThemeData* pThemeData, wstring strNewName) = 0;

		// Summary  : ���õ�ǰ����
		// Returns  : bool �ɹ�����TRUE
		// Parameter: ThemeData * pThemeData ��������Ϣ
		virtual bool DeleteTheme(ThemeData* pThemeData) = 0;

		// Summary  : �������
		// Returns  : bool �ɹ�����TRUE
		// Parameter: wstring strLocalFile ������·�����ļ�����IOSΪ�ļ��У�ANDROIDΪ�ļ���
        // Parameter: ProgressObserver* pObserver ����������
		// Parameter: wstring strPreviewFile ��Ԥ��ͼ�ļ���
		virtual ThemeData* AddLocalTheme(wstring strLocalFile, ProgressObserver* pObserver = NULL, wstring strPreviewFile = L"") = 0;

		// Summary  : ������������
		// Returns  : wstring ���ݺ�ı����ļ�����IOSΪ�ļ��У�ANDROIDΪ�ļ�����ʧ�ܷ���L""
		// Parameter: ThemeData * pThemeData ��������Ϣ
		// Parameter: wstring SavePath Ҫ�洢�����ص�Ŀ¼
		virtual wstring BackupTheme(ThemeData* pThemeData, wstring SavePath) = 0;

		// Summary  : ���õ�ǰ����
		// Returns  : bool �ɹ�����TRUE
		// Parameter: ThemeData * pThemeData ��������Ϣ
		virtual bool SetCurTheme(ThemeData* pThemeData) = 0;

		// Summary  : ������ǰ����
		// Returns  : bool �ɹ�����TRUE
		virtual bool RevocationTheme() = 0;

		// Summary  : �ж��Ƿ�Ϊ����Ŀ¼��PC�ˣ�
		// Returns  : bool ������Ŀ¼�򷵻�TRUE
		// Parameter: wstring strPath ��PC�ϵ�Ŀ¼
		virtual bool CheckIsThemePath(wstring strPath) = 0;

		// Summary  : ��ȡ�����б�
		// Returns  : vector<ThemeData*>
		virtual deque<ThemeData*> GetThemeList();

        // �ͷţ���������
        virtual void Release() = 0;

	protected:
		void ClearThemeList();
		void AddTheme(wstring id, ThemeData* pThemeData);
        void AddTheme(ThemeData* pThemeData);
        void RemoveTheme(ThemeData* pThemeData);
		ThemeData* FindTheme(wstring id);
		ThemeData* FindTheme(ThemeType eType, wstring strThemeName);

	protected:
		const DeviceData* _pDevice;
		map<wstring, ThemeData*> _themeList;
		wstring _strPreviewImageFolder;
	};
}