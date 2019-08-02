#include "pch_module.h"

#if defined( PLATFORM_IOS ) && defined( MODULE_APP )

#include "Module/Application/IosApp/IosAppOperationFactory.h"
#include "Module/Application/IosApp/IosPXLAppOperation.h"
#include "Module/Application/IosApp/IosIPAAppOperation.h"
#include "Module/Application/IosApp/IosDebAppOperation.h"
#include "Module/Application/IosApp/IosPandaAppOperation.h"
#include "Module/Application/IosApp/IosInstallerAppOperation.h"

IosAppOperationFactory::IosAppOperationFactory(void)
{
}

IosAppOperationFactory::~IosAppOperationFactory(void)
{
}

IIosAppOperation* IosAppOperationFactory::GetAppOperation(DeviceData* pDevice, APP_TYPE appType, ProgressSubject& subject)
{
	IIosAppOperation* pOperation = NULL;

	switch(appType)
	{
	case APP_TYPE_PXL:
		pOperation = new IosPXLAppOperation(pDevice, subject);			
		break;
	case APP_TYPE_IPA:
		pOperation = new IosIPAAppOperation(pDevice, subject);
		break;
	case APP_TYPE_DEB:
		pOperation = new IosDebAppOperation(pDevice, subject);
		break;
	case APP_TYPE_INSTALLER:
		pOperation = new IosInstallerAppOperation(pDevice, subject);
		break;
	case APP_TYPE_PANDA:
		pOperation = new IosPandaAppOperation(pDevice, subject);
		break;
	default:
		break;
	}

	return pOperation;
}
#endif