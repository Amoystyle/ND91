#pragma once
#include <WinInet.h>
#ifndef tstring
#define tstring wstring
#endif

//////////////////////////////////////////
// 设备激活类
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
	// @brief :激活设备
	// @return : 成功或失败
	bool Activate();

	///////////////////////////////////////////////////////////////
	// @brief :反激活设备
	// @return : 成功或失败
	bool Deactivate();
private:
	///////////////////////////////////////////////////////////////
	// @brief :从设备中取出激活信息，并转成XML字符串
	// @params :
	//		tstring &sInfo: 接收激活信息的字符串
	// @return : 成功或失败
	bool DeviceGetActivateInfoXML(tstring &sInfo);

	///////////////////////////////////////////////////////////////
	// @brief : 向字典中插入非空的key/value
	// @params :
	//		LPCTSTR sKey: 要插入的Key
	//		const tstring& sVal: 要插入的value
	//		std::map<tstring, tstring>& dict: 要插入的字典
	void AddNotNullKeyValue(LPCTSTR sKey, const tstring& sVal, std::map<tstring, tstring>& dict);

	///////////////////////////////////////////////////////////////
	// @brief : 向字典中插入key/value
	// @params :
	//		LPCTSTR sKey: 要插入的Key
	//		const tstring& sVal: 要插入的value
	//		std::map<tstring, tstring>& dict: 要插入的字典
	void AddKeyValue(LPCTSTR sKey, const tstring& sVal, std::map<tstring, tstring>& dict);

	///////////////////////////////////////////////////////////////
	// @brief : 生成 http post 界定字符串标识
	// @return : 返回生成的字符串
	tstring GenBoundary();

	///////////////////////////////////////////////////////////////
	// @brief : 生成 http post 界定字符串
	// @params :
	//		bool bEnd: 是否带有结束符
	// @return : 返回生成的字符串
	tstring GetBoundaryStr(bool bEnd);

	///////////////////////////////////////////////////////////////
	// @brief : 发送激活请求并获取服务器返回
	// @params :
	//		std::map<tstring, tstring>& params: 激活请求需要的参数
	// @return : 服务器返回的内容
	tstring PostActivationReq(std::map<tstring, tstring>& params);

	///////////////////////////////////////////////////////////////
	// @brief : 把字典转化成HTTP POST请求内容串
	// @params :
	//		const std::map<tstring, tstring>& dict: 源字典
	// @return : 返回的UTF8编码字符串
	std::string DictToHttpReqString(const std::map<tstring, tstring>& dict);

	///////////////////////////////////////////////////////////////
	// @brief : 获得字符串中指定XML节点及其内容
	// @params :
	//		const tstring& strOri: 源字符串
	//		const tstring& strNode: XML节点名
	// @return : 取得的串
	tstring GetXMLSubString(const tstring& strOri, const tstring& strNode);

	///////////////////////////////////////////////////////////////
	// @brief : 从设备获取时区偏移量，并转成字符串
	// @return : 取得的字符串
	tstring GetStringUTCOff();

	///////////////////////////////////////////////////////////////
	// @brief : 读取HTTP响应字符串
	// @params :
	//		HINTERNET hRequest: HTTP请求句柄
	// @return : 返回的字符串
	tstring ReadHttpResponseString(HINTERNET hRequest);


	///////////////////////////////////////////////////////////////
	// @brief : 处理apple服务器响应，找出需要的数据(iphone-activation/device-activation)
	// @params :
	//		tstring strRsp: apple服务器响应的字符串
	//		tstring& strErr: 如果服务器返回错误，接收错误文字描述
	// @return : ManagedType形式的激活数据，无用后需要delete
	void ParseAppleResponse(tstring strRsp, tstring& strErr,CFDictionaryMAP &DicMap);

	///////////////////////////////////////////////////////////////
	// @brief : 使用激活数据(activation-record)来激活设备
	// @params :
	//		CTypeBase* pActRecord: 激活数据(activation-record)
	// @return : 成功或失败
	bool DoActivation(void* pActRecord);
private:
	DeviceInfo* m_pDevice; // 设备指针
	long long m_llbound;  // http post boundary
};
}