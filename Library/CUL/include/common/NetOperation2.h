#pragma once
#include "IHttpInvoke.h"
#include "curl/curl.h"
#include "PostParam.h"

class CUL_CLASS_DECL NetOperation2
{
public:
	NetOperation2(void);
	~NetOperation2(void);

	/*
	*@param cUrl url 
	*@param iTimeout ��ʱʱ�䣨��λ������)
	*@param invoke �ص�����
	*/
	CURLcode static Get( const char* cUrl, int iTimeOut = 0 , IHttpInvoke* invoke = NULL);

	/*
	*@param cUrl url 
	*@param iTimeout ��ʱʱ�䣨��λ������)
	*@param invoke �ص�����
	*@param head ���ͷ��Ϣ
	*/
	CURLcode static Post(const char* cUrl, PostParam* param, int iTimeOut = 0, IHttpInvoke* invoke = NULL );
};
