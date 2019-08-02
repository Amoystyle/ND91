#include "log.h"

char g_process_name[NAME_MAX] = {'\0'};
char g_log_msg[BUFSIZ];
int g_print_log = -9;
FILE* g_fp = NULL;

void SetProcessName(const char* pszName)
{
	int length = strlen(pszName);
	length = length > NAME_MAX ? NAME_MAX : length;
	strncpy(g_process_name, pszName, length);
	g_process_name[length] = '\0';
}

void Init_g_print_log()
{
	g_print_log = access(LOG_FILE_PATH, F_OK);
	if (g_print_log == 0)
	{
		struct stat statLog;
		if (stat(LOG_FILE_PATH, &statLog) != -1)
		{
			if (statLog.st_size >= 1024 * 1024 /*1MµÄ´óÐ¡*/)
			{
				g_fp = fopen(LOG_FILE_PATH, "w");
			}
			else
			{
				g_fp = fopen(LOG_FILE_PATH, "a");
			}
		}
		else
		{
			g_print_log == -9;
		}
	}

	SetProcessName("daemon");
}

void VerboseNdsh(const char* formatter, ...)
{
	va_list ap;
	va_start(ap, formatter);
	NdshPrintf(ANDROID_LOG_VERBOSE, g_process_name, formatter, ap);
	va_end(ap);
}

void DebugNdsh(const char* formatter, ...)
{
	va_list ap;
	va_start(ap, formatter);
	NdshPrintf(ANDROID_LOG_DEBUG, g_process_name, formatter, ap);
	va_end(ap);
}

void InfoNdsh(const char* formatter, ...)
{
	va_list ap;
	va_start(ap, formatter);
	NdshPrintf(ANDROID_LOG_INFO, g_process_name, formatter, ap);
	va_end(ap);
}

void WarnNdsh(const char* formatter, ...)
{
	va_list ap;
	va_start(ap, formatter);
	NdshPrintf(ANDROID_LOG_WARN, g_process_name, formatter, ap);
	va_end(ap);
}

void ErrorNdsh(const char* formatter, ...)
{
	va_list ap;
	va_start(ap, formatter);
	NdshPrintf(ANDROID_LOG_ERROR, g_process_name, formatter, ap);
	va_end(ap);
}

void Verbose(const char* tag, const char* formatter, ...)
{
	va_list ap;
	va_start(ap, formatter);
	NdshPrintf(ANDROID_LOG_VERBOSE, tag, formatter, ap);
	va_end(ap);
}

void Debug(const char* tag, const char* formatter, ...)
{
	va_list ap;
	va_start(ap, formatter);
	NdshPrintf(ANDROID_LOG_DEBUG, tag, formatter, ap);
	va_end(ap);
}

void Info(const char* tag, const char* formatter, ...)
{
	va_list ap;
	va_start(ap, formatter);
	NdshPrintf(ANDROID_LOG_INFO, tag, formatter, ap);
	va_end(ap);
}

void Warn(const char* tag, const char* formatter, ...)
{
	va_list ap;
	va_start(ap, formatter);
	NdshPrintf(ANDROID_LOG_WARN, tag, formatter, ap);
	va_end(ap);
}

void Error(const char* tag, const char* formatter, ...)
{
	va_list ap;
	va_start(ap, formatter);
	NdshPrintf(ANDROID_LOG_ERROR, tag, formatter, ap);
	va_end(ap);
}

void Log(int level, const char* tag, const char* formatter, ...)
{
	va_list ap;
	va_start(ap, formatter);
	NdshPrintf(level, tag, formatter, ap);
	va_end(ap);
}

void NdshPrintf(int level, const char* tag, const char* formatter, va_list ap)
{
	if (g_print_log != 0)
	{
		return;
	}

	memset(g_log_msg, '\0', BUFSIZ);
	char versionPrefix[BUFSIZ + 10];
	strcpy(versionPrefix, VERSION);
	strcat(versionPrefix, " $ ");
	strcat(versionPrefix, formatter);
	int cnt = vsnprintf(g_log_msg, BUFSIZ, versionPrefix, ap);

	LogInFile(level, tag, formatter, ap);

#ifdef _DEBUG_LOG
	__android_log_write(level, tag, g_log_msg);
#endif
}

void LogInFile(int level, const char* tag, const char* formatter, va_list ap)
{
	memset(g_log_msg, '\0', BUFSIZ);
	char msg[BUFSIZ + 500];
	vsnprintf(g_log_msg, BUFSIZ, formatter, ap);

	char* pszLevel = GetLevelStr(level);
	char* pszTime = GetCurrentTime();
	sprintf(msg, LOG_FORMATTER, pszTime, pszLevel, tag, g_log_msg);
	
	free(pszLevel);
	free(pszTime);

	if (g_fp != NULL)
	{
		fputs(msg, g_fp);
	}
	
}

char* GetLevelStr(int level)
{
	char* szLevel = (char*) malloc(20 * sizeof(char));
	memset(szLevel, '\0', 20);
	switch (level)
	{
	case ANDROID_LOG_VERBOSE:
		strcpy(szLevel, VERBOSE);
		break;
	case ANDROID_LOG_DEBUG:
		strcpy(szLevel, DEBUG);
		break;
	case ANDROID_LOG_INFO:
		strcpy(szLevel, INFO);
		break;
	case ANDROID_LOG_WARN:
		strcpy(szLevel, WARN);
		break;
	case ANDROID_LOG_ERROR:
		strcpy(szLevel, ERROR);
		break;
	}

	return szLevel;
}

char* GetCurrentTime()
{
	char* pszTime = (char*) malloc(500 * sizeof(char));
	struct timeval timeCur;
	gettimeofday(&timeCur, NULL);
	int millisecond = timeCur.tv_usec / 1000;

	time_t timeSeconds = time(NULL);
	//struct tm* tmDay = gmtime(timeSeconds);
	struct tm* tmDay = localtime(&timeSeconds);
	sprintf(pszTime, TIME_FORMATTER, tmDay->tm_year + 1900, tmDay->tm_mon + 1, tmDay->tm_mday, tmDay->tm_hour, tmDay->tm_min, tmDay->tm_sec, millisecond);

	return pszTime;
}

void CloseLog()
{
	if (g_print_log == 0)
	{
		g_print_log = -9;
		if (g_fp != NULL)
		{
			fclose(g_fp);
			g_fp = NULL;
		}
	}
}