#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <buffer.h>
#include <android/log.h>
#include <time.h>
#include <sys/stat.h>

#include "common.h"

#define LOG_FILE_PATH "/sdcard/nd/ndsh.log"
#define LOG_FORMATTER "%s : [%s] [%s] %s\n"
#define TIME_FORMATTER "%d-%d-%d %d:%d:%d,%d"
//#define _DEBUG_LOG

#define VERBOSE "VERBOSE"
#define DEBUG "DEBUG"
#define INFO "INFO"
#define WARN "WARN"
#define ERROR "ERROR"
#define ERR_LEVEL "ERR_LEVEL"

void SetProcessName(const char* pszName);
void Init_g_print_log();

void VerboseNdsh(const char* formatter, ...);
void DebugNdsh(const char* formatter, ...);
void InfoNdsh(const char* formatter, ...);
void WarnNdsh(const char* formatter, ...);
void ErrorNdsh(const char* formatter, ...);

void Verbose(const char* tag, const char* formatter, ...);
void Debug(const char* tag, const char* formatter, ...);
void Info(const char* tag, const char* formatter, ...);
void Warn(const char* tag, const char* formatter, ...);
void Error(const char* tag, const char* formatter, ...);

void Log(int level, const char* tag, const char* formatter, ...);

void NdshPrintf(int level, const char* tag, const char* formatter, va_list ap);

void LogInFile(int level, const char* tag, const char* formatter, va_list ap);

char* GetLevelStr(int level);

char* GetCurrentTime();

void CloseLog();