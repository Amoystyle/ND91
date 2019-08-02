// NDBase64.h: interface for the CNDBase64 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NDBASE64_H__1CEA0845_E85A_4AD9_BF3E_4D1A18702598__INCLUDED_)
#define AFX_NDBASE64_H__1CEA0845_E85A_4AD9_BF3E_4D1A18702598__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../cul.h"

class CUL_CLASS_DECL CNDBase64  
{
public:
	CNDBase64();
	virtual ~CNDBase64();

	static int Base64encode_len(int len);
	static int Base64encode(char * coded_dst, const char *plain_src,int len_plain_src);
	
	static int Base64decode_len(const char * coded_src);
	static int Base64decode(char * plain_dst, const char *coded_src);

};

#endif // !defined(AFX_NDBASE64_H__1CEA0845_E85A_4AD9_BF3E_4D1A18702598__INCLUDED_)
