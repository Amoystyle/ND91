#pragma once

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

#include "log.h"

int Find_pids_by_name(char* pidName, long* pidList, int length);