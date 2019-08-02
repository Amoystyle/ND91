#pragma once

class CCCallJSHelper
{
public:
	HRESULT static CallJSMethod(IDispatch* pDispatch,LPOLESTR pMethod, VARIANT *pVarResult, VARIANT *ps, int cArgs);
private:
	CCCallJSHelper(void);
	~CCCallJSHelper(void);
};
