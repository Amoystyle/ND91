#pragma once
#ifndef GetDeviceCommandWare_h__
#define GetDeviceCommandWare_h__

#ifdef _ASSISTANT_DLL 
#define CLASSINDLL_CLASS_DECL     __declspec(dllexport) 
#else 
#define CLASSINDLL_CLASS_DECL     __declspec(dllimport) 
#endif 

//文件说明，提供给驱动扫描dll使用的获得设备接口，需要界面对本动态库完成初始化后才能正确返回。
#ifdef __cplusplus
#define  C_EXTERN  extern "C"
#else
#define  C_EXTERN 
#endif
	
// 获得ios设备驱动列表
C_EXTERN CLASSINDLL_CLASS_DECL const char* GetIosDevices();

// 获得android设备驱动列表
C_EXTERN CLASSINDLL_CLASS_DECL const char* GetAdbDevices();

// 释放前面命令返回的dll内部分配的内存块
C_EXTERN CLASSINDLL_CLASS_DECL void ReleaseCharPtr(char* p);

// 关闭ADB
C_EXTERN CLASSINDLL_CLASS_DECL void KillAdb();

// 初始化ios加载库 0-失败 1-成功
C_EXTERN CLASSINDLL_CLASS_DECL int IosInit();

// Summary  : 当adb_usb.ini中无参数带的vid时，修改adb_usb.ini
// Returns  : true：修改成功，false，修改失败。不进行修改，但是已在ini中返回true。
// Parameter: int vid 厂家ID，十进制数
C_EXTERN CLASSINDLL_CLASS_DECL bool ModADBIni(int vid);


#endif // GetDeviceCommandWare_h__