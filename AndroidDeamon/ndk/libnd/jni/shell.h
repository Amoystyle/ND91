#pragma once

#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>     
#include <stdarg.h>    


#include "log.h"
#include "buffer.h"
#include "ndstream.h"

#define OPEN_MAX_GUESS 256
#define SHELL   "/bin/sh"   

struct nd_data ExecuteShellCallPopen(char* cmd);
int ExecuteShellCallSystem(char* cmd);
int ExecuteShellCallPeopnForStr(char* cmd, char** result, int timeout);
struct nd_data ExecuteShellForStr(struct nd_data* data_receive);
struct nd_data ExecuteShell(struct nd_data* data_receive);
int execcmd(const char *cmd);
FILE *c_popen(const char *cmdstring, const char *type)  ;
int c_pclose(FILE *fp) ;