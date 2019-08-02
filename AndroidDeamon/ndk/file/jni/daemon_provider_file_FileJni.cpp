#include "daemon_provider_file_FileJni.h"
#include <common.h>
#include <file.h>
#include <log.h>

#include "jni_convert.h"
	/*env->SetByteArrayRegion(byteArr, 0, NDPC_HEAD_LENGTH, (jbyte*) pszHead); \*/
#define GetReturnByteArray(env, businessID, dataRet, byteArr) \
	int length = dataRet.pos; \
	byteArr = env->NewByteArray(length); \
	env->SetByteArrayRegion(byteArr, 0, dataRet.pos, (jbyte*) dataRet.buffer); \

JNIEXPORT jbyteArray JNICALL Java_daemon_provider_file_FileJni_getDirInfo(JNIEnv * env, jclass cls, jstring path)
{
	char* pszPath = jstringToCstr(env, path);
	struct nd_data dataRet = {0, 0, NULL};
	dataRet = GetDirInfo(pszPath, false);
	jbyteArray byteArr;
	GetReturnByteArray(env, GET_DIR, dataRet, byteArr);
	
	free(pszPath);
	if(dataRet.buffer!=NULL)
	{
	  free(dataRet.buffer);
	}
	return byteArr;
}

JNIEXPORT jbyteArray JNICALL Java_daemon_provider_file_FileJni_getDirInfoSubWritable(JNIEnv * env, jclass cls, jstring path)
{
	char* pszPath = jstringToCstr(env, path);
	struct nd_data dataRet = {0, 0, NULL};
	dataRet = GetDirInfo(pszPath, true);
	jbyteArray byteArr;
	GetReturnByteArray(env, GET_DIR_SUB_WRITABLE, dataRet, byteArr);

	free(pszPath);
	if(dataRet.buffer!=NULL)
	{
	  free(dataRet.buffer);
	}
	return byteArr;
}

JNIEXPORT jbyteArray JNICALL Java_daemon_provider_file_FileJni_makeDir(JNIEnv * env, jclass cls, jstring path)
{
	char* pszPath = jstringToCstr(env, path);
	struct nd_data dataRet = {0, 0, NULL};
	dataRet = MakeDir(pszPath);
	jbyteArray byteArr;
	GetReturnByteArray(env, MKDIR, dataRet, byteArr);
	free(pszPath);
	if(dataRet.buffer!=NULL)
	{
	  free(dataRet.buffer);
	}
	return byteArr;
}

JNIEXPORT jbyteArray JNICALL Java_daemon_provider_file_FileJni_remove(JNIEnv * env, jclass cls, jstring path)
{
	char* pszPath = jstringToCstr(env, path);
	struct nd_data dataRet = {0, 0, NULL};
	dataRet = RemoveFile(pszPath);
	jbyteArray byteArr;
	GetReturnByteArray(env, REMOVE, dataRet, byteArr);
	free(pszPath);
	if(dataRet.buffer!=NULL)
	{
	  free(dataRet.buffer);
	}
	return byteArr;
}

JNIEXPORT jbyteArray JNICALL Java_daemon_provider_file_FileJni_move(JNIEnv * env, jclass cls, jstring pathOld, jstring pathNew)
{
	char* pszPathOld = jstringToCstr(env, pathOld);
	char* pszPathNew = jstringToCstr(env, pathNew);
	struct nd_data dataRet = {0, 0, NULL};
	dataRet = MoveFile(pszPathOld, pszPathNew);
	jbyteArray byteArr;
	GetReturnByteArray(env, MOVE, dataRet, byteArr);
	free(pszPathOld);
	free(pszPathNew);
	if(dataRet.buffer!=NULL)
	{
	  free(dataRet.buffer);
	}
	return byteArr;
}

JNIEXPORT jbyteArray JNICALL Java_daemon_provider_file_FileJni_copy(JNIEnv * env, jclass cls, jstring pathOld, jstring pathNew)
{
	char* pszPathSrc = jstringToCstr(env, pathOld);
	char* pszPathDes = jstringToCstr(env, pathNew);
	struct nd_data dataRet = {0, 0, NULL};
	dataRet = CopyFile(pszPathSrc, pszPathDes);
	jbyteArray byteArr;
	GetReturnByteArray(env, COPY, dataRet, byteArr);
	free(pszPathSrc);
	free(pszPathDes);
	if(dataRet.buffer!=NULL)
	{
	  free(dataRet.buffer);
	}
	return byteArr;
}

JNIEXPORT jbyteArray JNICALL Java_daemon_provider_file_FileJni_getFileAttr(JNIEnv * env, jclass cls, jstring path)
{
	char* pszPath = jstringToCstr(env, path);
	struct nd_data dataRet = {0, 0, NULL};
	dataRet = GetFileAttr(pszPath, false);
	jbyteArray byteArr;
	GetReturnByteArray(env, GET_FILE_ATTR, dataRet, byteArr);
	free(pszPath);
	if(dataRet.buffer!=NULL)
	{
	  free(dataRet.buffer);
	}
	return byteArr;
}

JNIEXPORT jbyteArray JNICALL Java_daemon_provider_file_FileJni_getFileAttrWritable(JNIEnv * env, jclass cls, jstring path)
{
	char* pszPath = jstringToCstr(env, path);
	struct nd_data dataRet = {0, 0, NULL};
	dataRet = GetFileAttr(pszPath, true);
	jbyteArray byteArr;
	GetReturnByteArray(env, GET_FILE_ATTR_WRITABLE, dataRet, byteArr);
	free(pszPath);
	if(dataRet.buffer!=NULL)
	{
	  free(dataRet.buffer);
	}
	return byteArr;
}

JNIEXPORT jbyteArray JNICALL Java_daemon_provider_file_FileJni_chmod(JNIEnv * env, jclass cls, jstring path)
{
	char* pszPath = jstringToCstr(env, path);
	struct nd_data dataRet = {0, 0, NULL};
	dataRet = RemoveFile(pszPath);
	jbyteArray byteArr;
	GetReturnByteArray(env, CHMOD, dataRet, byteArr);
	free(pszPath);
	if(dataRet.buffer!=NULL)
	{
	  free(dataRet.buffer);
	}
	return byteArr;
}

JNIEXPORT void JNICALL Java_daemon_provider_file_FileJni_initLog(JNIEnv* env, jclass cls)
{
	Init_g_print_log();
}
