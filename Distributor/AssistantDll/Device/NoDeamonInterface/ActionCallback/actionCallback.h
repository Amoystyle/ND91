#ifndef _91HELPER_DAEMON_ACTION_CALLBACK_H_
#define _91HELPER_DAEMON_ACTION_CALLBACK_H_

#include "itunesInterface.h"

#ifdef __cplusplus
extern "C" {
#endif
	int chmodChownCallback(CFDictionaryRef dict);
	int pxlCallback(CFDictionaryRef dict);

#ifdef __cplusplus
}
#endif

#endif