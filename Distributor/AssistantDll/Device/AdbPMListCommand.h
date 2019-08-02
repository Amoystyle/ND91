#pragma once

#include "AdbShellCommand.h"
#include "Module/Application/AppData.h"
#include "Module/System/ProcessData.h"
#include "Common/Stroperation.h"
#include "Common/CodeOperation.h"

#define ADB_CMD_PM_LIST      "pm list packages -f"
using namespace std;
namespace ND91Assistant
{
	//��ȡ��ǰ�����б������ڼ���ػ������Ƿ���ڣ������ػ��Ƿ�ɹ�
	class AdbPMListCommand : public AdbShellCommand
	{
	public:
		AdbPMListCommand(const wstring sn)
		{
			_sn = CCodeOperation::UnicodeToUTF_8(sn);
			_strCommand = ADB_CMD_PM_LIST;
		};
		AdbPMListCommand(const string sn)
		{
			_sn = sn;
			_strCommand = ADB_CMD_PM_LIST;
		};
		virtual ~AdbPMListCommand()
		{
			for (vector<AppData*>::iterator it = _AppList.begin(); it != _AppList.end(); it++)
			{
				AppData* pData = *it;
				delete pData;
			}
			_AppList.clear();
		};

		//��ȡ�����б�
		vector<AppData*> getDataList()
		{
			return _AppList;
		}; 

	protected:
		// ������������
		// ����ֵ��true ��ʾ�ɹ���false ��ʾʧ��
		virtual bool ParseReturnData()
		{
			_AppList.clear();
			vector<string> strLines = CStrOperation::parseLines(_returnData.c_str(), _returnData.length());
			for (vector<string>::iterator it = strLines.begin(); it != strLines.end(); it++)
			{
				AppData* pData = new AppData();
				if (pData->ParseAndroidAdbData(*it))
					_AppList.push_back(pData);
				else
					delete pData;
			}
			return _AppList.size() > 0;
		};

	private:
		vector<AppData*>  _AppList;
	};
}