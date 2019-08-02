#pragma once

#include "GetWebInfoBase.h"

namespace ND91Assistant
{
	//�����ȡiphone�豸��Ϣ�Ľ�����
	class CLASSINDLL_CLASS_DECL CGetIPhoneSellInfo : public CGetWebInfoBase
	{
	public:
		CGetIPhoneSellInfo(DeviceData* pDevice);
		~CGetIPhoneSellInfo();

		//bool GetDeviceSellInfo();
        //bool LoadFrom91();

        // ��ȡ�豸���������Ϣ
        bool GetWarrantInfo();

        wstring GetSn() { return _strSn; }                              // ��ȡ�豸���к�
        wstring GetPurchCountry() { return _strPurchCountry; }          // ��ȡ���۹���
        wstring GetLastUnbrick() { return _strLastUnbrick; }            // ��ȡ�ϴν���
        wstring GetPHEndDate() { return _strPHEndDate; }                // ��ȡ�绰�������ʱ��
        wstring GetHash() { return _strHash; }                          // ��ȡ����˴洢apple��ȡ��Ϣhashֵ
        wstring GetColor() { return _strColor; }                        // ��ȡ�豸��ɫ
        wstring GetPurchaseDate() { return _strPurchaseDate; }          // ��ȡ��������
        wstring GetCovEndDate() { return _strCovEndDate; }              // ��ȡ��������

        int GetIsRegistered() { return _nIsRegistered; }                // ��ȡ�Ƿ�ע��        -1=δ֪ 0=No 1=Yes
        int GetActivationStatus() { return _nActivationStatus; }        // ��ȡ�Ƿ񼤻�        -1=δ֪ 0=No 1=Yes
        int GetPHHasCoverage() { return _nPHHasCoverage; }              // ��ȡ�Ƿ��е绰����  -1=δ֪ 0=No 1=Yes
        int GetHWHasCoverage() { return _nHWHasCoverage; }              // ��ȡ�Ƿ�Ӳ������    -1=δ֪ 0=No 1=Yes

        bool GetSuccess() const { return _bSuccess; }

    private:
        // ��91��������ȡ���������Ϣ
        int RequestWarrantyFrom91Server(string& retWarrantyStr);

        // ��ƻ���������ϻ�ȡ������Ϣ
        int RequestWarrantyFromAppleServer(string& retWarrantyStr);

        // ����91��������������Ϣ
        void Analytic91Html(string htmlStr);

        // ����ƻ����������������Ϣ
        void AnalyticAppleHtml(string htmlStr);

        wstring _strSn;                 // �豸���к�
        wstring _strPurchCountry;       // ���۹���
        wstring _strLastUnbrick;        // �ϴν���
        wstring _strPHEndDate;          // �绰�������ʱ��
        wstring _strHash;               // ����˴洢apple��ȡ��Ϣhashֵ
        wstring _strColor;              // �豸��ɫ
        wstring _strPurchaseDate;       // ��������
        wstring _strCovEndDate;         // ��������

        int _nIsRegistered;             // �Ƿ�ע��        -1=δ֪ 0=No 1=Yes
        int _nActivationStatus;         // �Ƿ񼤻�        -1=δ֪ 0=No 1=Yes
        int _nPHHasCoverage;            // �Ƿ��е绰����  -1=δ֪ 0=No 1=Yes
        int _nHWHasCoverage;            // �Ƿ�Ӳ������    -1=δ֪ 0=No 1=Yes
        
        bool _bSuccess;				    // �Ƿ��ȡ���ݳɹ�
    };
}

