#include "ndstream.h"
#include <sys/time.h>

const int NDPC_HEAD_LENGTH = 9;
const char NDPC[4] = {'N', 'D', 'P', 'C'};
char STD_OUTPUT_BUFFER[4096];

char* Read(int input, int len)
{
	if(len <= 0)
		return NULL;

	char* data =  (char*) malloc(len);
	int readCnt = 0;
	char* p = NULL;	

	struct timeval start, stop;
	gettimeofday(&start, 0);

	while (readCnt < len)
	{		
		p = data + readCnt;
		int onceRead = read(input, p, len - readCnt);		

		if(onceRead > 0)
		{				
			readCnt += onceRead;
		}
		else if (onceRead == 0)
		{		
            gettimeofday(&stop, 0);
			if(stop.tv_sec - start.tv_sec > 10)
			{
			    ErrorNdsh("heart beat timeout");
				exit(0);
			}

			continue;
		}	
		else if (onceRead == -1)
		{
			ErrorNdsh("FIFO_JAVA2C read fail!    error: %d !\n", errno);

			free(data);
			data = NULL;
			break;
		}		
	}

	p = NULL;
	return data;
}

void Write(int output, unsigned char businessId, char* data, int len)
{
    char* head = GetHead(businessId, len);
	WriteData(output, head, NDPC_HEAD_LENGTH);
	WriteData(output, data, len);
}

void WriteData(int output, char* data, int length)
{	
    if(data == NULL || length <= 0)
		return;
	int surplus = length;
	int needOnceWrite = surplus > PIPE_BUF ? PIPE_BUF : surplus;
	int totalWrite = 0;
	char* ptr = data;
	
	while(totalWrite < length)
	{
		int onceWrite = write(output, ptr, needOnceWrite);
	    if(onceWrite > 0)
		{
			totalWrite += onceWrite;
			surplus = length - totalWrite;
			needOnceWrite = surplus > PIPE_BUF ? PIPE_BUF : surplus;
			ptr += onceWrite;
		}
		else if(onceWrite == 0)
		{
			continue;
		}
		else if(onceWrite < 0 && errno == 11)
		{
			continue;
			//usleep(10 * 1000);
		}
		else
		{
			ErrorNdsh("WriteData(int output, char* data, int length) fail!    errno: %d\n", errno);

			break;
		}
		
	}
	ptr = NULL;
	free(data);
}

char* GetHead(unsigned char businessId, int len)
{
	char* head = (char*) malloc(NDPC_HEAD_LENGTH);
	memcpy(head, NDPC, 4);

	head[4] = businessId;

	head[5] =  (char) (len & 0x000000FF);
	head[6] =  (char) ((len & 0x0000FF00) >> 8);
	head[7] =  (char) ((len & 0x00FF0000) >> 16);
	head[8] =  (char) ((len & 0xFF000000) >> 24);

	return head;
}

int ReadFromStream(FILE* stream, char** content, int length)
{
	if (length < 0)
	{
		ErrorNdsh("illegal argument.ReadFromStreamFixed(FILE* stream, char** content, int length) argument 'length' = %d\n", length);

		return -1;
	}
	char* buff = (char*) malloc(length);
	if (buff == NULL)
	{
		ErrorNdsh("malloc(%d * sizeof(char)) fail\n", length);

		return -1;
	}
	memset(buff, '\0', length);
	int onceRead = 0;
	int total = 0;
	char* ptr = buff;

	while ((onceRead = fread(ptr, 1, length - total, stream)) != -1)
	{
		total += onceRead;
		if (total == length)
		{
			break;
		}
		ptr = buff + total;
	}
	if (feof(stream) || total == length)
	{
		*content = buff;
		return total;
	}

	free(buff);

	ErrorNdsh("ReadFromStreamFixed(FILE* stream, char** content, int length) fail\n");

	return -1;
}

int ReadFromStreamEnd(FILE* stream, char** content, int timeout)
{
	DebugNdsh("ReadFromStreamEnd(FILE* stream, char** content)\n");

	char* buff = buff = (char*) malloc(BUFSIZ * sizeof(char));
	if (buff ==	NULL)
	{
		ErrorNdsh("malloc(%d * sizeof(char)) fail\n", BUFSIZ);

		return -1;
	}
	memset(buff, '\0', BUFSIZ);
	
	int totalRead = 0;
	int onceRead = 0;
	int mul = 1;
	int totalSleep = 0;
	char* ptr = buff;
	while (1)
	{
		onceRead = fread(ptr, sizeof(char), BUFSIZ, stream);
		if (onceRead < 0)
		{
			ErrorNdsh("fread faile ! errno: %d\n", errno);

			return -1;
		}
		if (onceRead == 0)
		{
			DebugNdsh("onceRead = 0\n");

			usleep(50 * 1000);
			totalSleep += 50 * 1000;
			if (totalSleep > timeout)
			{
				ErrorNdsh("fread timeout !\n");
				free(buff);

				return -1;
			}
			if (feof(stream))
			{
				*content = buff;
				break;
			}
			continue;
		}

		totalRead += onceRead;
		
		if (totalRead == (mul * BUFSIZ))
		{
			mul *= 2;
			char* tmp = (char*) malloc(mul * BUFSIZ);
			if (tmp == NULL)
			{
				ErrorNdsh("malloc(%d * sizeof(char)) fail\n", BUFSIZ);

				free(buff);
				return -1;
			}
			memset(tmp, '\0', mul * BUFSIZ);
			memcpy(tmp, buff, totalRead);
			free(buff);
			buff = tmp;
		}
		ptr = buff + totalRead;
		if (feof(stream))
		{
			*content = buff;
			break;
		}
	}

	return totalRead;
}
