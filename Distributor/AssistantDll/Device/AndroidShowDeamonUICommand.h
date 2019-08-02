#pragma once

#ifndef WIN32
#include "Common/winPort.h"
#endif
#include "AdbShellCommand.h"
#include "AdbPSCommand.h"
#include "Module/AndroidDefine.h"
 // �����ػ�����
 #define Start_Activity_Command  "am start -n com.nd.assistance/.ui.AssistanceActivity"

 namespace ND91Assistant
 {
	 // �����ֻ��ػ�����
	 class AndroidShowDeamonUICommand : public AdbShellCommand
	 {
	 public:
		 AndroidShowDeamonUICommand(const std::wstring sn)
		 {
			 _sn = CCodeOperation::UnicodeToUTF_8(sn);
				 _strCommand = Start_Activity_Command;
		 };

	 protected:
		 // ������������
		 // ����ֵ��true ��ʾ�ɹ���false ��ʾʧ��
		 virtual bool ParseReturnData()
		 {         
		
			 _success = true;
			 return _success;
		 };
	 };

 }