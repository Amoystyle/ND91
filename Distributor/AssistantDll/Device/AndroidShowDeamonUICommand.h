#pragma once

#ifndef WIN32
#include "Common/winPort.h"
#endif
#include "AdbShellCommand.h"
#include "AdbPSCommand.h"
#include "Module/AndroidDefine.h"
 // 启动守护程序
 #define Start_Activity_Command  "am start -n com.nd.assistance/.ui.AssistanceActivity"

 namespace ND91Assistant
 {
	 // 启动手机守护进程
	 class AndroidShowDeamonUICommand : public AdbShellCommand
	 {
	 public:
		 AndroidShowDeamonUICommand(const std::wstring sn)
		 {
			 _sn = CCodeOperation::UnicodeToUTF_8(sn);
				 _strCommand = Start_Activity_Command;
		 };

	 protected:
		 // 解析返回数据
		 // 返回值：true 表示成功，false 表示失败
		 virtual bool ParseReturnData()
		 {         
		
			 _success = true;
			 return _success;
		 };
	 };

 }