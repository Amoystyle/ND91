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
	*@param iTimeout 超时时间（单位：毫秒)
	*@param invoke 回调函数
	*/
	CURLcode static Get( const char* cUrl, int iTimeOut = 0 , IHttpInvoke* invoke = NULL);

	/*
	*@param cUrl url 
	*@param iTimeout 超时时间（单位：毫秒)
	*@param invoke 回调函数
	*@param head 添加头信息
	*/
	CURLcode static Post(const char* cUrl, PostParam* param, int iTimeOut = 0, IHttpInvoke* invoke = NULL );
};
