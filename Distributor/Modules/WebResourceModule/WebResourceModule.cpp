#include "stdafx.h"
#include "WebResourceModule.h"
#include "Logic/WebResourceController.h"
#include "CheckCpaValid.h"
WEBRESOURCE_API SourceModule* CreateWebResourceModule()
{
	return WebResourceModule::GetInstance();
}

WebResourceModule* WebResourceModule::_pInstance = NULL;
WebResourceModule::WebResourceModule( void )
{
	_pController = new WebResourceController();
	_pCheckCpaValid = new CCheckLocalCpaValid();
}

WebResourceModule::~WebResourceModule()
{

}

WebResourceModule* WebResourceModule::GetInstance()
{
	if(_pInstance == NULL)
		_pInstance = new WebResourceModule();

	return _pInstance;
}

tstring WebResourceModule::Id()
{
	return L"WebResourceModule";
}

tstring WebResourceModule::Name()
{
	return LoadString(L"WebResource_Text_ModuleText");
}

tstring WebResourceModule::Icon()
{
	return L"#WebResourceUI_Image_ModuleIcon";
}

int WebResourceModule::LayoutIndex()
{
	return 0;
}

void WebResourceModule::Show()
{
	//_pController->Show();
}

void WebResourceModule::Hide()
{
	_pController->Hide();
}

bool WebResourceModule::Init(CContainerUI* pLeft, CContainerUI* pContainer )
{
	if(!_pController->Init(pLeft, pContainer))
		return false;

	return true;
}