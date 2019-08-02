#include "common.h"

char* BusinessTypeToStr(unsigned char type)
{
	char* result = (char*) malloc(32);
	memset(result, '\0', 32);

	switch (type)
	{
	case INIT_C2JAVA:
		result = strcpy(result, "INIT_C2JAVA");
		break;

	case GET_PROCESS_INFO:
		result = strcpy(result, "GET_PROCESS_INFO");
		break;
	case UPGRADE:
		result = strcpy(result, "UPGRADE");
		break;

	case GET_DIR:
		result = strcpy(result, "GET_DIR");
		break;
	case GET_DIR_SUB_WRITABLE:
		result = strcpy(result, "GET_DIR_SUB_WRITABLE");
		break;
	case MKDIR:
		result = strcpy(result, "MKDIR");
		break;
	case REMOVE:
		result = strcpy(result, "REMOVE");
		break;
	case MOVE:
		result = strcpy(result, "MOVE");
		break;
	case COPY:
		result = strcpy(result, "COPY");
		break;
	case READ_FILE:
		result = strcpy(result, "READ_FILE");
		break;
	case GET_FILE_ATTR:
		result = strcpy(result, "GET_FILE_ATTR");
		break;
	case GET_FILE_ATTR_WRITABLE:
		result = strcpy(result, "GET_FILE_ATTR_WRITABLE");
		break;
	case MAKE_FILE:
		result = strcpy(result, "MAKE_FILE");
		break;
	case WRITE_FILE:
		result = strcpy(result, "WRITE_FILE");
		break;
	case RENAME_FILE:
		result = strcpy(result, "RENAME_FILE");
		break;
	case READDATA_FILE:
		result = strcpy(result, "READDATA_FILE");
		break;
	case SCREENSHOT:
		result = strcpy(result, "SCREENSHOT");
		break;
	case SCREENSHOTJPEG:
		result = strcpy(result, "SCREENSHOTJPEG");
		break;
	case SENDEVENT:
		result = strcpy(result, "SENDEVENT");
        break;
	case EXECUTESHELL:
		result = strcpy(result, "EXECUTESHELL");
    break;
	case EXECUTESHELLFORSTR:
		result = strcpy(result, "EXECUTESHELLFORSTR");
    break;
	case INSTALL:
		result = strcpy(result, "INSTALL");
		break;
	case UNINSTALL:
		result = strcpy(result, "UNINSTALL");
		break;
	case UNINSTALL_SYSTEM_APP:
		result = strcpy(result, "UNINSTALL_SYSTEM_APP");
		break;
	case CHMOD:
		result = strcpy(result, "CHMOD");
        break;
	case CHOWN_AND_GRP:
		result = strcpy(result, "CHOWN_AND_GRP");
		break;
	case HEART_BEAT:
		result = strcpy(result, "HEART_BEAT");
		break;
	case QUIT:
		result = strcpy(result, "QUIT");
		break;
	default:
		result = strcpy(result, "UNKNOW");
		break;
	}
	
	return result;
}

void ToLower(char* src, char* des, int length)
{
	int i;
	for (i = 0; i < length; i ++)
	{
		*(des + i) = tolower(*(src + i));
	}
}

void ToUpper(char* src, char* des, int length)
{
	int i;
	for (i = 0; i < length; i ++)
	{
		*(des + i) = toupper(*(src + i));
	}
}