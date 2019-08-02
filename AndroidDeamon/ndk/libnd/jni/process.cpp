#include "process.h"

int Find_pids_by_name(char* pidName, long* pidList, int length)
{
	DIR *dir;
	struct dirent *next;
	int runningCount = 0;

	//proc中包括当前的进程信息,读取该目录
	dir = opendir("/proc");
	if (dir == NULL)
	{
		ErrorNdsh("opendir(\"/proc\") fail !    errno: %d\n", errno);

		return 0;
	}

	char daemonShName[30];
	char* index = strrchr(pidName, '/') + 1;
	strcpy(daemonShName, index);

	//遍历
	while ((next = readdir(dir)) != NULL) 
	{
		FILE *status;
		char filename[PATH_MAX];
		char buffer[BUFSIZ];
		char name[BUFSIZ];

		/* Must skip ".." since that is outside /proc */
		if (strcmp(next->d_name, "..") == 0)
			continue;

		/* If it isn't a number, we don't want it */
		if (!isdigit(*next->d_name))
			continue;

		//设置进程
		sprintf(filename, "/proc/%s/status", next->d_name);
		if (!(status = fopen(filename, "r")))
		{
			ErrorNdsh("fopen(\"%s\", \"r\") fail !    errno: %d\n", filename, errno);

			continue;
		}
		
		if (fgets(buffer, BUFSIZ - 1, status) == NULL)
		{
			fclose(status);
			continue;
		}

		fclose(status);

		//得到进程id
		/* Buffer should contain a string like "Name:   binary_name" */
		sscanf(buffer, "%*s %s", name);

		if (strcmp(name, pidName) == 0 || strcmp(name, daemonShName) == 0)
		{
			pidList[runningCount++] = strtol(next->d_name, NULL, 0);
			if (runningCount == length)
			{
				DebugNdsh("there may be more\n");
				break;
			}
		}
	}

	return runningCount;
}
