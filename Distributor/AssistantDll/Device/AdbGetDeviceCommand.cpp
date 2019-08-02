#include "stdafx.h"

#include "AdbGetDeviceCommand.h"

AdbGetDeviceCommand::AdbGetDeviceCommand(string cmd)
:AdbRemoteCommand(L"")
{
    _isShellCommand = false;
	_strCommand = cmd;
}

void AdbGetDeviceCommand::InternalExecute()
{
	CommonExecute(_strCommand);//"host:nddevices");
  //  CommonExecute(ADB_CMD_LIST_DEVICE);
}

const vector<wstring>* AdbGetDeviceCommand::GetSnList()
{
    return &_snList;
}

bool AdbGetDeviceCommand::ParseReturnData()
{
	if(_returnData.empty())
		return false;

    _snList.clear();

    // ����
    vector<string> lines;
    stringstream ss(_returnData);
    string line;
    while(std::getline(ss, line)) {
        lines.push_back(line);
    }

    if ( lines.empty() )
        return false;

    // ÿ�з�2��
    vector<string>::iterator it;
    for(it = lines.begin(); it != lines.end(); ++it)
    {
        line = *it;
        int offset = line.find('\t');
        if ( offset == string::npos )
        {
            continue;   // ����û��tab�ַ�������
        }


        string sn;
        string state;

		//����nddevice
		if(_strCommand == "host:nddevices")
		{
			string nextstringkey(line, offset+1);
			int offsetDeviceKey = nextstringkey.find('\t');
			sn = string(nextstringkey, offsetDeviceKey+1);
			state = string(nextstringkey, 0,offsetDeviceKey);
		}
		else
		{
			sn = string(line, 0, offset);
			state = string(line, offset+1);
		}

		wstring stateTmp = _snMap[CCodeOperation::UTF_8ToUnicode(sn)];
		if(stateTmp == L"offline")
			continue;

		//��offline�豸Ҳ�ӽ���
        if ( state == "device" || state == "offline")
        {
			_snList.push_back(CCodeOperation::UTF_8ToUnicode(sn));
        }
        _snMap[CCodeOperation::UTF_8ToUnicode(sn)] = CCodeOperation::UTF_8ToUnicode(state);
    }
    return ! _snList.empty();
}
string AdbGetDeviceCommand::GetReturnData()
{
	return _returnData;
}