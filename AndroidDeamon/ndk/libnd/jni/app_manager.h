#pragma once

#include <sys/mount.h>
#include <errno.h>

#include "log.h"
#include "file.h"
#include "buffer.h"
#include "shell.h"

int Install(char* path, char isLockApp, char isRecover, int installLocation);
struct nd_data Install_nd_data(struct nd_data* dataReceive);
int Uninstall(char* packageName, char isKeepData);
struct nd_data Uninstall_nd_data(struct nd_data* dataReceive);
int UninstallSystemApp(char* packageName, char* apkPath, char* type, char* source, char* target);
struct nd_data UninstallSystemApp_nd_data(struct nd_data* dataReceive);