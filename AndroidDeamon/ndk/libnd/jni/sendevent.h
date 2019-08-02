#pragma once

#include <sys/stat.h>
#include <sys/mman.h>
#include <buffer.h>
#include <fcntl.h>
#include <linux/input.h>
#include <errno.h>

#include "log.h"


struct nd_data Sendevent(struct nd_data* data_receive);