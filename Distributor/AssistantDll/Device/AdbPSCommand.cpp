
#include "AdbPSCommand.h"
#include "Common/Stroperation.h"

using namespace ND91Assistant;

AdbPSCommand::AdbPSCommand(const std::string sn)
: _sn(sn)
{
    _isShellCommand = true;
    _bSizeInBody = false;
}

AdbPSCommand::~AdbPSCommand()
{
    for (vector<ProcessData*>::iterator it = _ProcessList.begin(); it != _ProcessList.end(); it++)
    {
        ProcessData* pData = *it;
        delete pData;
    }
    _ProcessList.clear();
}

void AdbPSCommand::Execute()
{
    ConnectToDevice(_sn);
    CommonExecute(ADB_CMD_PS);
}

bool AdbPSCommand::ParseReturnData()
{
    _ProcessList.clear();
    vector<string> strLines = CStrOperation::parseLines(_returnData.c_str(), _returnData.length());
    for (vector<string>::iterator it = strLines.begin(); it != strLines.end(); it++)
    {
        ProcessData* pData = new ProcessData();
        if (pData->ParseAndroidData(*it))
            _ProcessList.push_back(pData);
        else
            delete pData;
    }
    return _ProcessList.size() > 0; 
}

vector<ProcessData*> AdbPSCommand::getProcessList()
{
    return _ProcessList;
}

bool AdbPSCommand::isProcessExist(string strProcess)
{
    for (vector<ProcessData*>::iterator it = _ProcessList.begin(); it != _ProcessList.end(); it++)
    {
        ProcessData* pData = *it;
        if (pData->_strName == strProcess)
            return true;
    }
    return false;
}