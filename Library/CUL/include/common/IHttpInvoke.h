#pragma once
#include "cul.h"

class CUL_CLASS_DECL IHttpInvoke
{
public:
	IHttpInvoke(void);
	~IHttpInvoke(void);
	/**
 *	接收到数据时的回调函数
 *
 *
 *	@param [in] buffer 接收到的数据所在缓冲区
 *	@param [in] size 数据长度
 *	@param [in] nmemb 数据片数量
 *	 获取的数据长度=size*nmemb
 */
 virtual void Callback(void *contents, size_t size, size_t nmemb) = 0;


 //************************************
 // Method:    Progress
 // FullName:  IHttpInvoke::Progress
 // Access:    virtual public 
 // Returns:   bool (返回ture为退出，返回false继续）
 // Qualifier:
 // Parameter: double dltotal 需要下载的数据的总大小
 // Parameter: double dlnow 已经下载的数据的大小
 // Parameter: double ultotal 需要上传的数据总大小
 // Parameter: double ulnow 已经上传的数据的大小
 //************************************
 virtual bool Progress(double dltotal, double dlnow,double ultotal, double ulnow) = 0;
};

