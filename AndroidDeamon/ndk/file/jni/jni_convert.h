#include <jni.h>

#ifndef _Included_jni_convert
#define _Included_jni_convert

char* jstringToCstr(JNIEnv* env, jstring jstr);

jstring cstrtoJstring(JNIEnv* env, const char* pat);

#endif