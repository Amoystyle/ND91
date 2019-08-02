#include "pch_module.h"

#if defined( MODULE_APP )

#include "Module/Application/ArchiveAppCommand.h"
#include "Module/Application/AppData.h"

#include "IosApp/IosAppOperationFactory.h"

ArchiveAppCommand::ArchiveAppCommand(const DeviceData* pDevice, const AppData* appData, const wstring strFileOnPC)
: BaseCommand(pDevice)
{
	_appData = appData;
	_strFileOnPC = strFileOnPC;
}

ArchiveAppCommand::~ArchiveAppCommand(void)
{
}

void ND91Assistant::ArchiveAppCommand::Execute()
{
	if (!_pDevice || !_pDevice->GetDevInfo())
		return ;

	if (_pDevice->GetDevInfo()->_deviceType == Android)
	{
        DownloadFileCommand downcmd(_pDevice);
        ProgressObserver* pObserver = NULL;
        if (!_observers.empty())
            pObserver = dynamic_cast<ProgressObserver*>(_observers[0]);
        downcmd.Attach(pObserver);
        downcmd.SendPrompt(MESSAGE_ARCHIVE_DOWNLOAD);
        downcmd.SetTaskNumberSum(1);
        downcmd.SetAllTaskSum(CFileOperation::GetFileSizeW(_appData->_strAppPhonePath));
        downcmd.SendProgress();
        if (!downcmd.Download(_strFileOnPC, _appData->_strAppPhonePath))
        {
            _success = false;
            _result = COMMAND_EXECUTE_FAIL;
        }
        downcmd.IncreaseTaskNumberComplete();
        downcmd.SendProgress();
	}
	else if (_pDevice->GetDevInfo()->_deviceType == Ios)
	{
		IIosAppOperation* pOperation = IosAppOperationFactory::GetAppOperation(_pDevice, _appData->_eAppType, *this);
		if (pOperation)
		{
			_result = pOperation->ArchiveApp(_appData->_packageName, _appData->_strAppPhonePath, 
				_appData->_AppName, _appData->_strVersion, eArchiveMode_All, _strFileOnPC);
            _success = _result == COMMAND_EXECUTE_SUCCESS;

			SAFE_DELETE(pOperation);
		}
		pOperation = NULL;
	}
	Complete();
}
#endif