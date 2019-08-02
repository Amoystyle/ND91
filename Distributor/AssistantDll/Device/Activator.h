#pragma once
#include <WinInet.h>
#ifndef tstring
#define tstring wstring
#endif

//////////////////////////////////////////
// �豸������
//
#define CFDictionaryMAP  map<std::string, void*>
namespace ND91Assistant
{
	class DeviceInfo;
class CActivator
{
public:
	CActivator(DeviceInfo* pDevice);
	~CActivator(void);

	///////////////////////////////////////////////////////////////
	// @brief :�����豸
	// @return : �ɹ���ʧ��
	bool Activate();

	///////////////////////////////////////////////////////////////
	// @brief :�������豸
	// @return : �ɹ���ʧ��
	bool Deactivate();
private:
	///////////////////////////////////////////////////////////////
	// @brief :���豸��ȡ��������Ϣ����ת��XML�ַ���
	// @params :
	//		tstring &sInfo: ���ռ�����Ϣ���ַ���
	// @return : �ɹ���ʧ��
	bool DeviceGetActivateInfoXML(tstring &sInfo);

	///////////////////////////////////////////////////////////////
	// @brief : ���ֵ��в���ǿյ�key/value
	// @params :
	//		LPCTSTR sKey: Ҫ�����Key
	//		const tstring& sVal: Ҫ�����value
	//		std::map<tstring, tstring>& dict: Ҫ������ֵ�
	void AddNotNullKeyValue(LPCTSTR sKey, const tstring& sVal, std::map<tstring, tstring>& dict);

	///////////////////////////////////////////////////////////////
	// @brief : ���ֵ��в���key/value
	// @params :
	//		LPCTSTR sKey: Ҫ�����Key
	//		const tstring& sVal: Ҫ�����value
	//		std::map<tstring, tstring>& dict: Ҫ������ֵ�
	void AddKeyValue(LPCTSTR sKey, const tstring& sVal, std::map<tstring, tstring>& dict);

	///////////////////////////////////////////////////////////////
	// @brief : ���� http post �綨�ַ�����ʶ
	// @return : �������ɵ��ַ���
	tstring GenBoundary();

	///////////////////////////////////////////////////////////////
	// @brief : ���� http post �綨�ַ���
	// @params :
	//		bool bEnd: �Ƿ���н�����
	// @return : �������ɵ��ַ���
	tstring GetBoundaryStr(bool bEnd);

	///////////////////////////////////////////////////////////////
	// @brief : ���ͼ������󲢻�ȡ����������
	// @params :
	//		std::map<tstring, tstring>& params: ����������Ҫ�Ĳ���
	// @return : ���������ص�����
	tstring PostActivationReq(std::map<tstring, tstring>& params);

	///////////////////////////////////////////////////////////////
	// @brief : ���ֵ�ת����HTTP POST�������ݴ�
	// @params :
	//		const std::map<tstring, tstring>& dict: Դ�ֵ�
	// @return : ���ص�UTF8�����ַ���
	std::string DictToHttpReqString(const std::map<tstring, tstring>& dict);

	///////////////////////////////////////////////////////////////
	// @brief : ����ַ�����ָ��XML�ڵ㼰������
	// @params :
	//		const tstring& strOri: Դ�ַ���
	//		const tstring& strNode: XML�ڵ���
	// @return : ȡ�õĴ�
	tstring GetXMLSubString(const tstring& strOri, const tstring& strNode);

	///////////////////////////////////////////////////////////////
	// @brief : ���豸��ȡʱ��ƫ��������ת���ַ���
	// @return : ȡ�õ��ַ���
	tstring GetStringUTCOff();

	///////////////////////////////////////////////////////////////
	// @brief : ��ȡHTTP��Ӧ�ַ���
	// @params :
	//		HINTERNET hRequest: HTTP������
	// @return : ���ص��ַ���
	tstring ReadHttpResponseString(HINTERNET hRequest);


	///////////////////////////////////////////////////////////////
	// @brief : ����apple��������Ӧ���ҳ���Ҫ������(iphone-activation/device-activation)
	// @params :
	//		tstring strRsp: apple��������Ӧ���ַ���
	//		tstring& strErr: ������������ش��󣬽��մ�����������
	// @return : ManagedType��ʽ�ļ������ݣ����ú���Ҫdelete
	void ParseAppleResponse(tstring strRsp, tstring& strErr,CFDictionaryMAP &DicMap);

	///////////////////////////////////////////////////////////////
	// @brief : ʹ�ü�������(activation-record)�������豸
	// @params :
	//		CTypeBase* pActRecord: ��������(activation-record)
	// @return : �ɹ���ʧ��
	bool DoActivation(void* pActRecord);
private:
	DeviceInfo* m_pDevice; // �豸ָ��
	long long m_llbound;  // http post boundary
};
}