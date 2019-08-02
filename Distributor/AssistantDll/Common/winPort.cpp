#include "winPort.h"
#include "unistd.h"
#include "stdlib.h"
#include <wchar.h>
#include "Common/CodeOperation.h"

void GetTempPath(int bufferSize, wchar_t* pBuffer)
{
	wcscpy(pBuffer, L"/tmp");
}

unsigned int GetTickCount()
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return (int)(tv.tv_sec*1000 + (tv.tv_usec / 1000));
}

void WaitForSingleObject(pthread_t t, int timeout)
{
	const int SLEEP_INTERVAL = 100000; // check thread every 0.1 seconds

	unsigned int begin = GetTickCount();
	unsigned int end = GetTickCount();
	do
	{
		if ( pthread_join(t, NULL) )
		break;

		usleep(SLEEP_INTERVAL);
		end = GetTickCount();
	}
	while( end - begin > timeout );

	return;
}

bool CopyFile(const char* pSourceFile, const char* pDestFile)
{
    int fd_to = 0, fd_from = 0;
    char buf[4096];
    ssize_t nread;

    fd_from = open(pSourceFile, O_RDONLY);
    if (fd_from < 0)
        return false;

    fd_to = open(pDestFile, O_WRONLY | O_CREAT | O_EXCL, 0666);
    if (fd_to < 0)
    {
        close(fd_from);
        return false;
    }

    while (nread = read(fd_from, buf, sizeof buf), nread > 0)
    {
        char *out_ptr = buf;
        ssize_t nwritten;

        do {
            nwritten = write(fd_to, out_ptr, nread);

            if (nwritten >= 0)
            {
                nread -= nwritten;
                out_ptr += nwritten;
            }
            else if (errno != EINTR)
            {
                close(fd_from);
                close(fd_to);
                return false;
            }
        } while (nread > 0);
    }

    close(fd_from);
    return ( ! close(fd_to) && nread == 0);
}

    // 环境变量
BOOL SetEnvironmentVariable(const wchar_t* key, const wchar_t* value)
{
    return setenv( CCodeOperation::UnicodeToGB2312(key).c_str(), 
			CCodeOperation::UnicodeToGB2312(value).c_str(), 1)
		+1;
	// SetEnvironmentVariable return: 0 fail, otherwise success
	// setenv return: 0 success, -1 fail, so plus one
}

gchar* g_base64_encode(const guchar *in_str, gsize in_len)
{
		static gchar base64[] ="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";  
		int curr_out_len = 0;  
		int i = 0;  
		guchar a, b, c;
		gchar *out_str = (gchar*) malloc (in_len * 4 / 3 + 1);
		out_str[0] = '\0';
		
		if (in_len > 0)  
		{  
   		while (i < in_len)  
   		{  
    		a = in_str[i];  
    		b = (i + 1 >= in_len) ? 0 : in_str[i + 1];  
    		c = (i + 2 >= in_len) ? 0 : in_str[i + 2];  
    		if (i + 2 < in_len)  
    		{  
     			out_str[curr_out_len++] = (base64[(a >> 2) & 0x3F]);  
     			out_str[curr_out_len++] = (base64[((a << 4) & 0x30) + ((b >> 4) & 0xf)]);  
     			out_str[curr_out_len++] = (base64[((b << 2) & 0x3c) + ((c >> 6) & 0x3)]);  
     			out_str[curr_out_len++] = (base64[c & 0x3F]);  
    		}  
    		else if (i + 1 < in_len)  
    		{  
     			out_str[curr_out_len++] = (base64[(a >> 2) & 0x3F]);  
     			out_str[curr_out_len++] = (base64[((a << 4) & 0x30) + ((b >> 4) & 0xf)]);  
     			out_str[curr_out_len++] = (base64[((b << 2) & 0x3c) + ((c >> 6) & 0x3)]);  
     			out_str[curr_out_len++] = '=';  
    		}  
    		else  
    		{  
     			out_str[curr_out_len++] = (base64[(a >> 2) & 0x3F]);  
     			out_str[curr_out_len++] = (base64[((a << 4) & 0x30) + ((b >> 4) & 0xf)]);  
     			out_str[curr_out_len++] = '=';  
     			out_str[curr_out_len++] = '=';  
    		}  
    		i += 3;  
   		}  
   		out_str[curr_out_len] = '\0';  
		}
		
		return out_str;  
}

void	g_free(gpointer	mem)
{
		free(mem);
}
