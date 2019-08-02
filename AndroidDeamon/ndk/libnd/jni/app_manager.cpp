#include "app_manager.h"

int Install(char* path, char isLockApp, char isRecover, int installLocation)
{
	if (access(path, F_OK) != 0)
	{
		ErrorNdsh("\"%s\" not found\n");

		return UNKNOW_ERROR;
	}
	chmod(path, 0777);

	char cmd[128];
	strcpy(cmd, "LD_LIBRARY_PATH=/vendor/lib:/system/lib pm install ");
	if (isLockApp)
		strcat(cmd, "-l ");
	if (isRecover)
		strcat(cmd, "-r ");
	if (installLocation == 1)
		strcat(cmd, "-s ");
	else if (installLocation == 2)
		strcat(cmd, "-f ");
	strcat(cmd, path);
	
	int status = UNKNOW_ERROR;
	char* shellResult;
	int shellResultLength = ExecuteShellCallPeopnForStr(cmd, &shellResult, 50 * 1000 * 1000);
	if (shellResultLength > 0)
	{
		char* lower = (char*) malloc(shellResultLength * sizeof(char));
		if (lower != NULL)
		{
			ToLower(shellResult, lower, shellResultLength);
			
			if (strstr(lower, "success"))
			{
				status = SUCCESS;
			}
			free(lower);
		}
	}

	free(shellResult);

	return status;
}

struct nd_data Install_nd_data(struct nd_data* dataReceive)
{
	char* packagePath = ReadStringFromBuffer(dataReceive);
	char isLockApp = ReadCharFromBuffer(dataReceive);
	char isRecover = ReadCharFromBuffer(dataReceive);
	int installLocation = ReadIntFromBuffer(dataReceive);

	//DebugNdsh("packagePath: %s", packagePath);

	struct nd_data dataResult = {0, 0, NULL};
	WriteIntToBuffer(&dataResult, UNKNOW_ERROR);
	time_t now;
	time(&now);
	char tmpPath[128];
	sprintf(tmpPath, "%s/%ld.apk", DATA_LOCAL_TMP, now);
	struct nd_data dataCopy;
	dataCopy = CopyFile(packagePath, tmpPath);
	dataCopy.pos = 0;
	int copyStatus = ReadIntFromBuffer(&dataCopy);
	char* path = packagePath;
	if (copyStatus)
		path = tmpPath;

	int status = Install(path, isLockApp, isRecover, installLocation);
	if (status == SUCCESS)
		WriteIntToBufferInPos(&dataResult, 0, SUCCESS);
	
	if (copyStatus)
		unlink(tmpPath);

	free(packagePath);

	return dataResult;
}

int Uninstall(char* packageName, char isKeepData)
{
	char cmd[BUFSIZ];
	strcpy(cmd, "LD_LIBRARY_PATH=/vendor/lib:/system/lib pm uninstall ");
	if (isKeepData)
		strcat(cmd, "-k ");
	strcat(cmd, packageName);

	int status = UNKNOW_ERROR;
	char* shellResult;
	int shellResultLength = ExecuteShellCallPeopnForStr(cmd, &shellResult, 50 * 1000 * 1000);
	if (shellResultLength > 0)
	{
		char* lower = (char*) malloc(shellResultLength * sizeof(char));
		if (lower != NULL)
		{
			ToLower(shellResult, lower, shellResultLength);
			char* indexPtr = strstr(lower, "success");
			if (indexPtr != NULL)
			{
				status = SUCCESS;
			}
			free(lower);
		}
	}

	free(shellResult);

	return status;
}

struct nd_data Uninstall_nd_data(struct nd_data* dataReceive)
{
	char* packageName = ReadStringFromBuffer(dataReceive);
	char isKeepData = ReadCharFromBuffer(dataReceive);

	struct nd_data dataResult = {0, 0, NULL};
	int status = Uninstall(packageName, isKeepData);
	if (status == SUCCESS)
	{
		//DebugNdsh("uninstall \"%s\" success\n", packageName);
		WriteIntToBuffer(&dataResult, SUCCESS);
	}
	else
	{
		ErrorNdsh("uninstall \"%s\" fail\n", packageName);

		WriteIntToBuffer(&dataResult, UNKNOW_ERROR);
	}
	free(packageName);

	return dataResult;
}

int UninstallSystemApp(char* packageName, char* apkPath, char* type, char* source, char* target)
{
	int result = UNKNOW_ERROR;

	if (mount(source, target, type, MS_REMOUNT, NULL) != 0)
	{
		ErrorNdsh("mount(\"%s\", \"%s\", MS_REMOUNT, NULL) fail !    errno: %d\n", source, target);

		return result;
	}

	if (unlink(apkPath) != 0)
	{
		ErrorNdsh("unlink(\"%s\") fail !    errno: %d\n", apkPath, errno);

		return result;
	}

	char odexPath[BUFSIZ];
	char* indexPtr = strstr(apkPath, ".apk");
	if (indexPtr != NULL)
	{
		int cnt = indexPtr - apkPath;
		strncpy(odexPath, apkPath, cnt);
		strcat(odexPath, ".odex");
		DebugNdsh(odexPath);
		if (access(odexPath, F_OK) != 0)
		{
			DebugNdsh("\"%s\" not found !\n", odexPath);
		}
		else
		{
			if (unlink(odexPath) != 0)
			{
				ErrorNdsh("unlink(\"%s\") fail !    errno: %d\n", odexPath, errno);
			}
		}
	}

	Uninstall(packageName, FALSE);

	if (mount(source, target, type, MS_REMOUNT | MS_RDONLY, NULL) != 0)
	{
		ErrorNdsh("mount(\"%s\", \"%s\", MS_REMOUNT | MS_RDONLY, NULL) fail !    errno: %d\n", source, target, errno);
	}

	result = SUCCESS;

	return result;
}

struct nd_data UninstallSystemApp_nd_data(struct nd_data* dataReceive)
{
	char* packageName = ReadStringFromBuffer(dataReceive);
	char* apkPath = ReadStringFromBuffer(dataReceive);
	char* type = ReadStringFromBuffer(dataReceive);
	char* source = ReadStringFromBuffer(dataReceive);
	char* target = ReadStringFromBuffer(dataReceive);

	/*DebugNdsh(type);
	DebugNdsh(source);
	DebugNdsh(target);
	DebugNdsh(apkPath);*/

	struct nd_data dataResult = {0, 0, NULL};
	int status = UninstallSystemApp(packageName, apkPath, type, source, target);
	WriteIntToBuffer(&dataResult, status);

	free(packageName);
	free(apkPath);
	free(type);
	free(source);
	free(target);

	return dataResult;
}