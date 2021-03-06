/*
 *  InfoLog.h
 *
 *  Created by lindaojiang on 2012-6-8
 *  Copyright 2012 NetDragon Websoft Inc. All rights reserved.
 *
 */

#ifndef _TEST_INFO_LOG_H_
#define _TEST_INFO_LOG_H_

#include <stdarg.h>

#if _DEBUG
#define INFOLOG(...)			infoLogStderr(__VA_ARGS__)
#else
#define INFOLOG(...)
#endif

#define TEST_LOG_MAX_SIZE			1048576

#ifdef __cplusplus
extern "C" {
#endif
	
	void infoLogFile(PTCHAR format, ...);			
	//头部为时间,pid;format中间部分;写入文件

	void infoLogStderr(PTCHAR format, ...);
	//写到标准错误输出

#ifdef __cplusplus
}
#endif

extern PTCHAR iphoneinterface_logFile;

#endif 

