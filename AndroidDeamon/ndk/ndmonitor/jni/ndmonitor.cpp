#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mount.h>
#include <pthread.h>
#include <dirent.h>
#include <ctype.h>
#include <android/log.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/system_properties.h>

#define LOG_SWITCH 0
#define READ_BUF_SIZE 1024

#define DATA_LOCAL_TMP "/data/local/tmp"

#define VERSION			"1.2"
#define NDMONITOR_PATH 	"/system/bin/ndmonitor"
#define NDMONITOR_PORT  "/data/data/com.nd.assistance/files/ndmonitor_port"
#define SUCCESS 		0

void initDaemon(void);
// 判断monitor是否已经运行
bool isMonitorRunning();
// 升级
bool UpGrade(char* fileName);
// 追加启动项
bool Write2InstallRecovery();
void sigroutine(int dunno);
// 拷贝
int CopyFile(const char* srcPath, const char* desPath);
char* GetDirPath(const char* filePath);
int MakeDir(const char* path);
int findPidsByName(char* pidName, long* pidList, int length);
void WriteFile(const char* fileName, char* data);
int RemountSystem(const char *mntpoint, unsigned long mountflags);
int run_proc(const char *progname, char *const argv[]);
void Printf(const char* formatter, ...);
// sdk版本
int GetSdkVer();

#define PORT 	8891
#define BACKLOG 10

// 服务端socket
int gSockFd = -1;
// 退出标示
bool gExitFlag = false;
int main(void)
{
	int conn_fd;
    struct sockaddr_in my_addr;
    struct sockaddr_in their_addr;
    int sin_size;
    int port;
    bool bUpGrade = false;
	time_t now;
	
    time(&now);

    // 版本号
    char temp_buf[256];
    sprintf(temp_buf, "NdMonitor Ver:%s\n", VERSION);
    __android_log_write(ANDROID_LOG_DEBUG, "ndmonitor", temp_buf);

	if (geteuid() == 0)
  	{
    	//提升ndsh进程权限，将实际用户id、有效用户id和保存的设置用户id都改为0（即改为root用户）
    	if(setuid(0) == -1)
		{
			Printf("setuid errno:%d\n", errno);
		}
	}

  	// 升级可能还没退出
    for(int i = 0; i < 5; i++)
    {
		if(isMonitorRunning())
		{
			Printf("ndmonitor is already running, try times = %d...\n", i);
			sleep(1);
		}
		else
		{
			break;
		}
    }

    if(isMonitorRunning())
    {
    	Printf("ndmonitor is already running...\n");
    	return 1;
    }
	
	if(GetSdkVer() < 21)
	{
		// 这里5.0不需要开机启动，需要重新授权，把SEAndroid关闭
		// 判断是否已经添加到启动sh里了
		Write2InstallRecovery();
	}

	initDaemon();

	// 注册信号
	signal(SIGUSR1,sigroutine);

    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(my_addr.sin_zero), 8);

    port = PORT;
    // 查看端口是否给占用
    for(int i = 0; i < 200; i++)
    {
		if ((gSockFd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
			Printf("socket error, errno:%d!\n", errno);
			return 0;
		}
		my_addr.sin_port = htons(port);
		if (bind(gSockFd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
		{
			Printf("bind errer, errno:%d!\n", errno);
			port++;
			continue;
		}
		else
		{
			// 写入文件
			char data[256];
			sprintf(data, "%d", port);
			WriteFile(NDMONITOR_PORT, data);
			Printf("ndmonitor open port:%s success\n", data);

			break;
		}
    }

    if (listen(gSockFd, BACKLOG) == -1)
    {
    	Printf("listen error!\n");
        return 0;
    }

    while (1)
    {
		if(gExitFlag)
		{
			break;
		}
		
        sin_size = sizeof(struct sockaddr_in);
        if ((conn_fd = accept(gSockFd, (struct sockaddr *)&their_addr, &sin_size)) == -1)
        {
        	Printf("accept error!, errno:%d\n", errno);
            continue;
        }
        Printf("server: got connection from %s\n", inet_ntoa(their_addr.sin_addr));

        if (fork() == 0)
        {
        	int numbytes = 0;
			int bytes = 0;
        	char buf[2048];
			char temp[1024];
			char* pStr = NULL;
        	memset(buf, 0, 2048);

			while((bytes = read(conn_fd, temp, 1024)) > 0)
			{
				memcpy(&buf[numbytes], temp, bytes);
				numbytes += bytes;
			}
			
        	if (numbytes > 0)
        	{
        		int in = 0;
        		char* param[10];

        		Printf("buf:%s\n", buf);

        		pStr = strtok(buf, "|");

        		while((param[in] = strtok(NULL, "|")) != NULL){
        			Printf("param[%d] = %s\n", in, param[in]);
        			in++;
        		}

				if(pStr != NULL)
				{
					if(strstr(pStr, "restart") != NULL)
					{
						// 参数三个
						if(in >= 3)
						{
							// 运行
							param[in] = NULL;
							run_proc(param[0], param);
						}
					}
					else if(strstr(pStr, "upgrade") != NULL)
					{
						// 升级
						if(in >= 1)
						{
							if(UpGrade(param[0]))
							{
								// 通知父进程重启啦
								kill(getppid(),SIGUSR1);
							}
						}
					}
				}
        	}
			close(gSockFd);
        	close(conn_fd);
        	gSockFd = -1;
        	return 0;
        }

        // 不接收内容，默认关闭
        close(conn_fd);

        while(waitpid(-1, NULL, WNOHANG) > 0);
    }
	
    close(gSockFd);
    gSockFd = -1;

	// 这里要重启,这里必须是孙子进程，子进程有问题
    int pid;

    pid = fork();
    if(pid == 0)
    {
    	char* param[10];

    	// 建议将可执行程序名(比如ls)也作为execv()函数的参数.实际测试，如果没有该参数，会启动失败
    	param[0] = (char*)NDMONITOR_PATH;
    	param[1] = NULL;
    	run_proc(NDMONITOR_PATH, param);
    	return 0;
    }
    else if(pid < 0)
    {
    	Printf("pid < 0, errno : %d", errno);
    }
    sleep(1);
    exit(0);
	return 0;
}

void sigroutine(int dunno) {
	// 信号处理
	Printf("rev signal: %d...\n", dunno);
	gExitFlag = true;
	if(gSockFd != -1)
	{
		close(gSockFd);
	}
	return;
}

// 写文件
void WriteFile(const char* fileName, char* data)
{
	FILE* fileWrite = NULL;
	if((fileWrite = fopen(fileName, "w")) == NULL)
	{
		return;
	}

	fwrite(data, sizeof(unsigned char), strlen(data), fileWrite);
	fclose(fileWrite);
}

// 升级
bool UpGrade(char* fileName)
{
	const char* pMnonitor = NDMONITOR_PATH;
	bool res = false;
	 
	RemountSystem("/system", MS_REMOUNT);
	// 判断文件是否存在
	if(access(fileName, F_OK) != 0)
	{
		Printf("file isn't exist    errno: %d\n", errno);
		goto _end;
	}
	
	if (unlink(pMnonitor) != 0)
	{ 
		Printf("unlink!    errno: %d\n", errno);
		goto _end;
	}
	
	CopyFile(fileName, pMnonitor);
	
	if (chown(pMnonitor, 0, 0) != 0)
	{
		Printf("chown(\"%s\", 0, 0) fail !    errno: %d\n", pMnonitor, errno); 
		goto _end;
	}
	if (chmod(pMnonitor, 04755) != 0)
	{
		Printf("chmod(\"%s\", 04755) fail !    errno: %d\n", pMnonitor, errno);
		goto _end;
	}
	
	res = true;
	
	_end:
	RemountSystem("/system", MS_REMOUNT | MS_RDONLY);
	return res;
}

// 判断monitor是否已经运行
bool isMonitorRunning()
{
	int pidSelf = getpid();
	long pidList[100];
	char processPath[256];
	strcpy(processPath, NDMONITOR_PATH);

	int length = findPidsByName(processPath, pidList, 100);
	int i;
	for (i = 0; i < length; i ++)
	{
		if (pidList[i] == pidSelf)
		{
			continue;
		}
		return true;
	}
	return false;
}

int run_proc(const char *progname, char *const argv[])
{
	int status;
	pid_t   pid;

	Printf("start %s...\n", progname);

	pid = fork();
	if(pid < 0 )
	{
		return false;
	}
    else if (pid == 0)
    {
		// 如果应用程序正常执行完毕，那么execv是永远不会返回的；当execv在调用进程中返回时，
		// 那么这个应用程序应该出错了（可能是程序本身没找到，权限不够...), 此时它的返回值
		// 应该是-1,具体的错误代码可以通过全局变量errno查看，还可以通过stderr得到具体的错误描述字符串
    	if(execv(progname, argv)<0)
		{
			Printf("execv fail, errno:%d\n", errno);
			exit(0);
			return false;
		}
		exit(0);
    }
    else
    {
    	Printf("fork father \n");
    }
	return true;
}
 
void Printf(const char* formatter, ...)
{ 
#if LOG_SWITCH
	char log_msg[1024];
	va_list ap;
		
	va_start(ap, formatter);
	memset(log_msg, '\0', 1024); 
	int cnt = vsnprintf(log_msg, 1024, formatter, ap);
	va_end(ap);
	
	printf(log_msg);
	__android_log_write(ANDROID_LOG_DEBUG, "ndmonitor", log_msg);
#endif
}

void initDaemon(void)
{
	int pid;
	int i;
	if((pid=fork()) > 0)
	{
		// 是父进程，结束父进程
		exit(0);
	}
	else if(pid< 0)
	{
		// fork失败，退出
		exit(1);
	}

	// 是第一子进程，后台继续执行
	// 第一子进程成为新的会话组长和进程组长
	if (setsid() == -1)
	{
		exit(1); // 开启一个新会话
	}

	// 并与控制终端分离
	if((pid=fork()) > 0)
	{
		// 是第一子进程，结束第一子进程
		exit(0);
	}
	else if(pid< 0)
	{
		// fork失败，退出
		exit(1);
	}

	// 是第二子进程，继续
	// 第二子进程不再是会话组长

	// 6.重设文件掩码，防止某些属性被父进程屏蔽
	if (umask(0) == -1) exit(1);

	// 8.本守护进程的子进程若不需要返回信息，那么交给init进程回收，避免产生僵尸进程
	if (signal(SIGCHLD, SIG_IGN) == SIG_ERR)
	{
		exit(1);
	}
	return;
}

int findPidsByName(char* pidName, long* pidList, int length)
{
	DIR *dir;
	struct dirent *next;
	int runningCount = 0;

	//proc中包括当前的进程信息,读取该目录
	dir = opendir("/proc");
	if (dir == NULL)
	{
		return 0;
	}

	if(pidName == NULL || length <= 0)
	{
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
				break;
			}
		}
	}

	return runningCount;
}
int RemountSystem(const char *mntpoint, unsigned long mountflags)
{
	FILE *f = NULL;
	int found = 0;
	char buf[1024], *dev = NULL, *fstype = NULL;

	if ((f = fopen("/proc/mounts", "r")) == NULL)
		return -1;

	memset(buf, 0, sizeof(buf));
	for (;!feof(f);) {
		if (fgets(buf, sizeof(buf), f) == NULL)
			break;
		if (strstr(buf, mntpoint)) {
			found = 1;
			break;
		}
	}
	fclose(f);
	if (!found)
		return -1;
	if ((dev = strtok(buf, " \t")) == NULL)
		return -1;
	if (strtok(NULL, " \t") == NULL)
		return -1;
	if ((fstype = strtok(NULL, " \t")) == NULL)
		return -1;
	return mount(dev, mntpoint, fstype, mountflags/*MS_REMOUNT*/, 0);
}
//复制源路径和目录路径必须为最终路径。例如复制文件,srcPath:/sdcard/a.txt, desPath:/sdcard/nd/a.txt;复制目录,srcPath: /sdcard/dir_a, desPath: /sdcard/nd/dir_a
int CopyFile(const char* srcPath, const char* desPath)
{
	Printf("copyfile, src: \"%s\", des: \"%s\"\n", srcPath, desPath);

	int from_fd = 0;
	int to_fd = 0;

	int err = SUCCESS;

	if((from_fd = open(srcPath, O_RDONLY)) == -1)
	{
		Printf("open(\"%s\", O_RDONLY) fail !    errno: %d\n", srcPath, errno);
		err = errno;
	}

	//创建文件夹
	char* desDir = GetDirPath(desPath);
	if(desDir != NULL)
	{
		MakeDir(desDir);
		if (access(desDir, F_OK) != 0)
		{
			err = errno;
		}

		free(desDir);
	}
	
	if(err == SUCCESS && (to_fd = open(desPath, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO)) == -1)
	{
		Printf("open(\"%s\", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR) fail !    errno: %d\n", desPath, errno);
		err = errno;
	}
 
	if(err == SUCCESS)
	{
		//三星s3创建文件时给定777无效，创建完后改权限
		chmod(desPath, 0777);

		int bytes_read = 0;
		int bytes_write = 0;
		char buffer[16384];
		char* ptr;
		while(err == SUCCESS && (bytes_read = read(from_fd, buffer, 16384)))
		{
			if(bytes_read == -1)
			{
				Printf("read fail! path:%s,  errno: %d\n", srcPath, errno);
				err = errno;
				break;
			}

			else if(bytes_read > 0)
			{
				ptr = buffer;

				while(bytes_write = write(to_fd, ptr, bytes_read))
				{
					if(bytes_write == -1)
					{
						Printf("write fail !    errno: %d\n", errno);
						err = errno;
						break;
					}
					else if(bytes_write == bytes_read)
					{
						break;
					}
					else if(bytes_write > 0)
					{
						ptr += bytes_write;
						bytes_read -= bytes_write;
					}
				}
			}
		}
	}

	if(from_fd > 0)
	{
		close(from_fd);
	}

	if(to_fd > 0)
	{
		close(to_fd);
	}

	return err;	
}

int MakeDir(const char* path)
{
	int i = 1;
	int len = strlen(path);
	int result = 0;

	char* subPath = (char*) malloc(len * sizeof(char));


	//创建父目录
	while(i < len && result == 0 )
	{
		if(path[i] == '/')
		{
			memset(subPath, '\0', len);
			strncpy(subPath, path, i);

			if(access(subPath, F_OK) != 0)
			{
				result = mkdir(subPath, S_IRWXU | S_IRWXG | S_IRWXO);
				if(result == 0)
				{
					//三星s3创建文件时给定777无效，创建完后改权限
					chmod(path, 0777);
				}
			}
		}

		i++;
	}

	//如果给定路径目录不存在，就创建
	if(result == 0 && access(path, F_OK) != 0)
	{
		result = mkdir(path, 0777);
	}

	if(result == 0 && access(path, F_OK) == 0)
	{
		//三星s3创建文件时给定777无效，创建完后改权限
		chmod(path, 0777);
	}

	return SUCCESS;
}

char* GetDirPath(const char* filePath)
{
	int len = strlen(filePath);
	int index = -1;

	for(int i = 0; i < len; ++i)
	{
		if(*(filePath + i) == '/')
		{
			index = i;
		}
	}

	char* dir = NULL;
	if(index != -1)
	{
		dir = new char[index + 2];
		memset(dir, 0, index + 2);
		strncpy(dir, filePath, index+1);
	}

	return dir;
}

// /system/etc/install-recovery.sh
// 追加启动项
bool Write2InstallRecovery()
{
	char buff[512];
	const char* pSrcInstallRecovery = "/system/etc/install-recovery.sh";
	char pTempInstallRecovery[256];
	const char* pRunCmd = NDMONITOR_PATH;
	bool res = false;

	strcpy(pTempInstallRecovery, DATA_LOCAL_TMP);
	strcat(pTempInstallRecovery, "/install-recovery.sh");

	RemountSystem("/system", MS_REMOUNT);

	CopyFile(pSrcInstallRecovery, pTempInstallRecovery);

	FILE* fileRead = NULL;
	if((fileRead = fopen(pTempInstallRecovery, "ab+")) == NULL)
	{
		Printf("Write2InstallRecovery:fopen(\"%s\", \"r\") fail !    errno: %d\n", pTempInstallRecovery, errno);
		goto _end;
	}
	fseek(fileRead, 0L, SEEK_SET);
	while(true)
	{
		memset(buff, 0, 512);
		if(fgets(buff, 512, fileRead) != NULL)
		{
			if(strstr(buff, pRunCmd) != NULL)
			{
				fclose(fileRead);
				Printf("Write2InstallRecovery: Find install-recovery");
				res = true;
				goto _end;
			}
		}
		else
		{
			Printf("error:%d\n",errno);
			break;
		}
	}

	// 追加
	fseek(fileRead, 0L, SEEK_END);

	fwrite("\n", sizeof(unsigned char), 1, fileRead);
	fwrite(pRunCmd, sizeof(unsigned char), strlen(pRunCmd), fileRead);
	fwrite("\n", sizeof(unsigned char), 1, fileRead);

	fclose(fileRead);
	CopyFile(pTempInstallRecovery, pSrcInstallRecovery);

	_end:
	RemountSystem("/system", MS_REMOUNT | MS_RDONLY);
	return true;
}

// sdk版本
int GetSdkVer()
{
	char m_szSdkVer[256];
	memset(m_szSdkVer, 0, 256);

	__system_property_get("ro.build.version.sdk", m_szSdkVer);
	Printf("SDK Ver:%s\n", m_szSdkVer);
	return atoi(m_szSdkVer);
}
