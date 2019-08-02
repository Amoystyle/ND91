#pragma once
#include "cul.h"

class CUL_CLASS_DECL IHttpInvoke
{
public:
	IHttpInvoke(void);
	~IHttpInvoke(void);
	/**
 *	���յ�����ʱ�Ļص�����
 *
 *
 *	@param [in] buffer ���յ����������ڻ�����
 *	@param [in] size ���ݳ���
 *	@param [in] nmemb ����Ƭ����
 *	 ��ȡ�����ݳ���=size*nmemb
 */
 virtual void Callback(void *contents, size_t size, size_t nmemb) = 0;


 //************************************
 // Method:    Progress
 // FullName:  IHttpInvoke::Progress
 // Access:    virtual public 
 // Returns:   bool (����tureΪ�˳�������false������
 // Qualifier:
 // Parameter: double dltotal ��Ҫ���ص����ݵ��ܴ�С
 // Parameter: double dlnow �Ѿ����ص����ݵĴ�С
 // Parameter: double ultotal ��Ҫ�ϴ��������ܴ�С
 // Parameter: double ulnow �Ѿ��ϴ������ݵĴ�С
 //************************************
 virtual bool Progress(double dltotal, double dlnow,double ultotal, double ulnow) = 0;
};

