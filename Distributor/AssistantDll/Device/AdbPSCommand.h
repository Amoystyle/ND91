#pragma once

#include "AdbShellCommand.h"
#include "Module/system/ProcessData.h"
#include "Common/Stroperation.h"
#include "Common/CodeOperation.h"

#define ADB_CMD_PS      "ps"
using namespace std;
namespace ND91Assistant
{
    //��ȡ��ǰ�����б������ڼ���ػ������Ƿ���ڣ������ػ��Ƿ�ɹ�
    class AdbPSCommand : public AdbShellCommand
    {
    public:
        AdbPSCommand(const wstring sn)
        {
			_sn = CCodeOperation::UnicodeToUTF_8(sn);
            _strCommand = ADB_CMD_PS;
        };
		AdbPSCommand(const string sn)
		{
			_sn = sn;
			_strCommand = ADB_CMD_PS;
		};
        virtual ~AdbPSCommand()
        {
            for (vector<ProcessData*>::iterator it = _ProcessList.begin(); it != _ProcessList.end(); it++)
            {
                ProcessData* pData = *it;
                delete pData;
            }
            _ProcessList.clear();
        };

        //��ȡ�����б�
        vector<ProcessData*> getProcessList()
        {
            return _ProcessList;
        }; 
        
        //����strProgress�Ƿ����
		bool isProcessExist(wstring strProcess)
		{
			return isProcessExist(CCodeOperation::UnicodeToGB2312(strProcess));
		}
        bool isProcessExist(string strProcess)
        {
            for (vector<ProcessData*>::iterator it = _ProcessList.begin(); it != _ProcessList.end(); it++)
            {
                ProcessData* pData = *it;
                if (pData->_strName == strProcess)
                    return true;
            }
            return false;
        };
        int GetProgressPID(string strName)
        {
            for (vector<ProcessData*>::iterator it = _ProcessList.begin(); it != _ProcessList.end(); it++)
            {
                ProcessData* pData = *it;
                if (pData->_strName == strName)
                    return pData->_nPID;
            }
            return 0;
        };

    protected:
        // ������������
        // ����ֵ��true ��ʾ�ɹ���false ��ʾʧ��
        virtual bool ParseReturnData()
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
        };

    private:
        vector<ProcessData*>  _ProcessList;
    };
}