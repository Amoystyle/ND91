#pragma once

#include "Module/Application/IosIcon/IosIconSet.h"

namespace ND91Assistant
{
    // itunes���ٵ���֧�ּ��� __UIKitDesktopImageChangedNotification ����¼�
	// IOS�豸����Ļ������, ʵ����com.apple.springboardservices����ͨѶ
	// ��Ҫ����ͼ��(λ��),��ֽ����Ϣ��ȡ���� (ֻ֧��3.X����)
	class IosIconSet;
	class CLASSINDLL_CLASS_DECL IosScreenOperation
	{
	public:
		//************************************
		// Method:    IosScreenOperation ���캯��
		// FullName:  IosScreenOperation::IosScreenOperation
		// Access:    public 
		// Returns:   
		// Qualifier:
		// Parameter: void * pDevPtr ������_ios_devptr
		// Parameter: wstring strProductVersion ������_deviceProductVersion
		// Parameter: bool biPad ������IsPad()
		//************************************
		IosScreenOperation(void* pDevPtr, wstring strProductVersion, bool biPad);
		~IosScreenOperation(void);

		// ��ȡͼ��״̬��Ϣ
		IosIconSet* GetIconState();

		// �����µ�ͼ��״̬
		bool SetIconState();	

		// �ͷ�ͼ��״̬��Ϣ
		void FreeIconSet();

        // ���Ӷ�δ�����豸��֧��
        bool AllowUnactivated() { return false; }

	private:	
		// ��ȡ�豸��Ļ����
		eInterfaceOrientation GetInterfaceOrientation();

		// ��ȡ����Ļ��ֽ(�̼�4.0��ʼ�����������,3GS������ʼ,3G��Ҫ�ֶ�����)
		bool GetHomeScreenWallpaper(wstring strSavePath);

		bool SendMessageToService(void* pSocket, map<string, string> mapMsg);
		IosIconSet* ReadMssageFromService(void* pSocket);

		void InitIconImageFile(IosIcon* pIcon);
		wstring GetIconImageData(string strBundleID);

		void StartSpringBoardService();
		void CloseSpringBoardService();

	private:
		void*                   _pSpringBoardServiceHandle; // ��Ļ�����ķ�����
		void*                   _pDevPtr;                   // �豸���
		wstring                 _strProductVersion;         // �豸�İ汾��
		int	                    _nVer;                      // �豸�汾�ŵ����ֱ�ʾ
		bool                    _biPad;                     // �Ƿ�ΪIPAD
		eInterfaceOrientation   _eInterfaceOrientationType; // ��������ʶ
		IosIconSet*             _pIosIconSet;               // �豸��Ļ��������ָ��
        CRITICAL_SECTION        _cs;                        // �����ٽ�ֵ
	};

}
