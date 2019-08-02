/*	CFWindowsMessageQueue.h
	Copyright (c) 1999-2008, Apple Inc. All rights reserved.
*/

#if !defined(__COREFOUNDATION_CFWINDOWSMESSAGEQUEUE__)
#define __COREFOUNDATION_CFWINDOWSMESSAGEQUEUE__ 1

#if DEPLOYMENT_TARGET_WINDOWS

#include <CoreFoundation/CFString.h>
#include <CoreFoundation/CFRunLoop.h>


CF_EXTERN_C_BEGIN

typedef struct __CFWindowsMessageQueue * CFWindowsMessageQueueRef;

CF_EXPORT CFTypeID	CFWindowsMessageQueueGetTypeID(void);

CF_EXPORT CFWindowsMessageQueueRef	CFWindowsMessageQueueCreate(CFAllocatorRef allocator, uint32_t /* DWORD */ mask);

CF_EXPORT uint32_t	CFWindowsMessageQueueGetMask(CFWindowsMessageQueueRef wmq);
CF_EXPORT void		CFWindowsMessageQueueInvalidate(CFWindowsMessageQueueRef wmq);
CF_EXPORT Boolean	CFWindowsMessageQueueIsValid(CFWindowsMessageQueueRef wmq);

CF_EXPORT CFRunLoopSourceRef	CFWindowsMessageQueueCreateRunLoopSource(CFAllocatorRef allocator, CFWindowsMessageQueueRef wmq, CFIndex order);

CF_EXTERN_C_END

#endif

#endif /* ! __COREFOUNDATION_CFWINDOWSMESSAGEQUEUE__ */

