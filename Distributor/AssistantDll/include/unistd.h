/* CoreGraphics - unistd.h
 * Copyright (c) 2006-2007 Apple Inc.
 * All rights reserved. */

/* Provide functions used by ImageIO from `<unistd.h>'. */

#ifndef IMAGE_IO_UNISTD_H_
#define IMAGE_IO_UNISTD_H_

#pragma once

#include <sys/types.h>

/* Used in `ColorSync/Dispatcher/Sources/PrefsFile.c'. */

__inline char *
realpath(const char *pathname, char resolvedPath[MAX_PATH])
{
    return strncpy_s(resolvedPath, MAX_PATH, pathname, _TRUNCATE) == 0 ?
	resolvedPath : NULL;
}

#endif /* IMAGE_IO_UNISTD_H_ */
