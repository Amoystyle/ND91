/*
    CFURLEnumerator.h
    Copyright (c) 2008, Apple Inc. All rights reserved.
 */

#if !defined(__COREFOUNDATION_CFURLENUMERATOR__)
#define __COREFOUNDATION_CFURLENUMERATOR__ 1

#include <CoreFoundation/CFBase.h>
#include <CoreFoundation/CFArray.h>
#include <CoreFoundation/CFError.h>
#include <CoreFoundation/CFURL.h>

CF_EXTERN_C_BEGIN


typedef const struct __CFURLEnumerator *CFURLEnumeratorRef;

/*
    CFURLEnumeratorGetTypeID - Returns the CFURLEnumerator CFTypeID.
*/
CF_EXPORT
CFTypeID CFURLEnumeratorGetTypeID( void );


/*
    CFURLEnumeratorOptions - Options for controlling enumerator behavior.
*/
enum {
    kCFURLEnumeratorDescendRecursively = 1, /* Deep recursion ("depth-first") into each subdirectory enumerated */
    kCFURLEnumeratorSkipInvisibles     = 2  /* Skip resources marked "invisible" */
};
typedef CFOptionFlags CFURLEnumeratorOptions;

/*
    CFURLEnumeratorCreateForDirectoryURL - Creates a directory enumerator, flat 
    or recursive. Client specifies the directory URL to enumerate, a bit array 
    of options, and an optional array of property keys to pre-fetch for the found 
    URLs. Specifying pre-fetch properties allows the implementation to optimize 
    device access by using bulk operations when available. Pre-fetching more properties 
    than are actually needed may degrade performance. The enumerator generates URLs with 
    the same scheme as the directory URL being enumerated.
*/
CF_EXPORT
CFURLEnumeratorRef CFURLEnumeratorCreateForDirectoryURL( CFAllocatorRef alloc, CFURLRef directoryURL, CFURLEnumeratorOptions option, CFArrayRef propertyKeys );


/*
    CFURLEnumeratorCreateForMountedVolumes - Creates an enumerator for  
    mounted filesystem volumes. Client specifies an allocator, a bit 
    array of options, and an optional array of property keys to pre-fetch for 
    the volume URLs. The enumerator generates 'file:' URLs. 
    The kCFURLEnumeratorDescendRecursively option is ignored.
*/
CF_EXPORT
CFURLEnumeratorRef CFURLEnumeratorCreateForMountedVolumes( CFAllocatorRef alloc, CFURLEnumeratorOptions option, CFArrayRef propertyKeys );


/*
    CFURLEnumeratorResult - Results for CFURLEnumeratorGetNextURL
*/
enum {
    kCFURLEnumeratorSuccess = 1,    /* The enumeration was successful. The url output parameter is valid. */
    kCFURLEnumeratorEnd = 2,	    /* The enumeration is complete. */
    kCFURLEnumeratorError = 3	    /* An error occured during enumeration. The retained error output parameter describes the error. */
};
typedef CFIndex CFURLEnumeratorResult;

/*
    CFURLEnumeratorGetNextURL - Advances the enumerator. If kCFURLEnumeratorSuccess
    is returned, the url output parameter returns the next URL found.
    If kCFURLEnumeratorError is returned, an error has occured and the error output
    parameter describes the error. If kCFURLEnumeratorEnd, the enumeration is finished.
    The url output parameter, if returned, is not retained. The error output
    parameter, if returned, is retained and must be released.
*/
CF_EXPORT
CFURLEnumeratorResult CFURLEnumeratorGetNextURL( CFURLEnumeratorRef enumerator, CFURLRef *url, CFErrorRef *error );


/*
    CFURLEnumeratorSkipDescendents - Tells a recursive CFURLEnumerator not to
    descend into the directory of the last CFURLRef returned by
    CFURLEnumeratorGetNextURL.
    
    Calls to CFURLEnumeratorSkipDescendents are ignored if:
	*   CFURLEnumeratorGetNextURL has never been called with the CFURLEnumerator.
	*   The last CFURL returned by CFURLEnumeratorGetNextURL is not a directory.
	*   The CFURLEnumerator was not created with CFURLEnumeratorCreateForDirectoryURL
	    using the kCFURLEnumeratorDescendRecursively option.
*/
CF_EXPORT
void CFURLEnumeratorSkipDescendents( CFURLEnumeratorRef enumerator );


/*
    CFURLEnumeratorGetDescendentLevel - Returns the number of levels the
    enumerator has descended down into the directory hierarchy from the
    starting directory. The children of the starting directory are at
    level 1. Each time a recursive enumerator descends into a
    subdirectory, it adds one to the descendent level. It then subtracts
    one from the level when it finishes a subdirectory and continues
    enumerating the parent directory.
*/
CF_EXPORT
CFIndex CFURLEnumeratorGetDescendentLevel( CFURLEnumeratorRef enumerator );


/*
    CFURLEnumeratorGetSourceDidChange - Returns true if the data source changed
    while being enumerated. If the directory contents (or the volume list) changes 
    during enumeration, the enumerator's results may be stale. Best practice is to 
    check the result CFURLEnumeratorGetSourceDidChange() after completing enumeration, 
    and repeat if necessary by creating a new enumerator. Be prepared to handle rapidly 
    changing directories, for which clean enumeration may not be possible.
*/
CF_EXPORT
Boolean CFURLEnumeratorGetSourceDidChange( CFURLEnumeratorRef enumerator );


CF_EXTERN_C_END

#endif /* ! __COREFOUNDATION_CFURLENUMERATOR__ */

