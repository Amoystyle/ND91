#include "StdAfx.h"
#include "CCallJSHelper.h"

CCCallJSHelper::CCCallJSHelper(void)
{
}

CCCallJSHelper::~CCCallJSHelper(void)
{
}

HRESULT CCCallJSHelper::CallJSMethod( IDispatch* pDispatch,LPOLESTR pMethod, VARIANT *pVarResult, VARIANT *ps, int cArgs )
{
	return CActiveXUI::InvokeMethod(pDispatch, pMethod,pVarResult, ps, cArgs);

}
