#pragma once

#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <assert.h>

/// data type
#undef NULL
#define NULL 0

#define LPVOID void*
#define LPSTR  char*
#define BOOL   int
#define UINT   unsigned int
#define DWORD  unsigned int
#define HANDLE pthread_t

#define TCHAR char

/// critical section
#include <pthread.h>

#ifndef CRITICAL_SECION
class ThreadLock
{
public:
	ThreadLock() { pthread_mutex_init(&_localLock, NULL); _lockThreadId = 0; _repeatCount = 0; }
	~ThreadLock() { pthread_mutex_destroy(&_localLock); }

	void Init() 
	{

		if ( ! _lockThreadId )
		{
			pthread_mutex_init(&_lock, NULL);
            
            pthread_mutex_lock(&_localLock);
			_lockThreadId = NULL;
			_repeatCount = 0;
            pthread_mutex_unlock(&_localLock);
		}

	}

	void Delete()
	{

		pthread_mutex_destroy(&_lock);
		pthread_mutex_lock(&_localLock);
	 	_lockThreadId = NULL;
        _repeatCount = 0;
		pthread_mutex_unlock(&_localLock);
	}

	void Lock()
	{

		pthread_t currentThreadId = pthread_self();

		if ( _lockThreadId == NULL ) // first time by current thread
		{
            pthread_mutex_lock(&_localLock);
			_lockThreadId = currentThreadId;
			_repeatCount = 1;
            pthread_mutex_unlock(&_localLock);
            return;
		}
		else if ( _lockThreadId == currentThreadId ) // already locked by current thread
		{
            pthread_mutex_lock(&_localLock);
			_repeatCount++;
            pthread_mutex_unlock(&_localLock);
            return;
		}
		else    // locked by other thread
		{
			pthread_mutex_lock(&_lock); // wait for lock
            
            pthread_mutex_lock(&_localLock);
			_lockThreadId = currentThreadId;
			_repeatCount = 1;
            pthread_mutex_unlock(&_localLock);
            return;
		}

	}

	void Unlock()
	{

		pthread_t currentThreadId = pthread_self();

		if ( _lockThreadId == currentThreadId ) // locked by current thread
		{
            pthread_mutex_lock(&_localLock);
			_repeatCount--;
            
			if ( 0 ==  _repeatCount ) // should release lock
			{
                _lockThreadId = NULL;
				pthread_mutex_unlock(&_lock);
			}
            pthread_mutex_unlock(&_localLock);
		}
		else
		{
			// If a thread calls Unlock() when it does not have ownership, error occurs 
			assert(0); 
		}

		return;
	}

private:
	pthread_mutex_t  _localLock;
	pthread_mutex_t  _lock;
	pthread_t        _lockThreadId;
	unsigned int     _repeatCount;
};
#endif

#define CRITICAL_SECTION              ThreadLock
#define LPCRITICAL_SECTION            ThreadLock*
#define InitializeCriticalSection(p)  (p)->Init()
#define EnterCriticalSection(p)       (p)->Lock()
#define LeaveCriticalSection(p)       (p)->Unlock()
#define DeleteCriticalSection(p)      (p)->Delete()

/// dynamic library 
#include <dlfcn.h>
#define HINSTANCE            void*
#define LoadLibrary(x)       dlopen(x, RTLD_LAZY)
#define FreeLibrary          dlclose
#define GetProcAddress       dlsym

/// Win socket 
#include <sys/socket.h>
#include <sys/errno.h>
#include <netdb.h>
#include <arpa/inet.h>

#define WSAGetLastError() errno
#define closesocket	::close
#define SOCKET int
#define PROTOCOL PF_INET
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

typedef struct sockaddr SOCKADDR;
typedef struct sockaddr SOCKADDR_IN;
typedef struct sockaddr *LPSOCKADDR;

/// path
void GetTempPath(int bufferSize, wchar_t* pBuffer);

/// misc function

#define Sleep(x)                      usleep(x*1000);

unsigned int GetTickCount();

// 注意：仅用于设定时限等待线程结束，timeout为毫秒（minisecond）
//       若timeout为INFINITE, 请直接使用pthread_join方法
void WaitForSingleObject(pthread_t t, int timeout);

// 拷贝文件函数
bool CopyFile(const char* pSourceFile, const char* pDestFile);

// 设置环境变量
BOOL SetEnvironmentVariable(const wchar_t* key, const wchar_t* value);


typedef char gchar;
typedef unsigned char guchar;
typedef unsigned int gsize;
typedef void* gpointer;

/* Provide macros to feature the GCC function attribute.
 */
#if    __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 96)
	#define G_GNUC_MALLOC    		__attribute__((__malloc__))
#else
	#define G_GNUC_MALLOC
#endif

gchar* g_base64_encode (const guchar *data, gsize len) G_GNUC_MALLOC;
void	 g_free	         (gpointer	 mem);

