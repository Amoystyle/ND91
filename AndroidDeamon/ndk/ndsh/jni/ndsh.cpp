#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mount.h>
#include <pthread.h>

#include "common.h"
#include "log.h"
#include "ndstream.h"
#include "buffer.h"
#include "file.h"
#include "shell.h"
#include "screenshot.h"
#include "process.h"
#include "app_manager.h"
#include "sendevent.h"

#include "threadpool.h"


void Clear();
void KillNdsh();
void CreateFIFO();
void InitOutput();
void ExcuteBusiness(unsigned char bussinessId, struct nd_data* receiveData,int key);
void *ExcuteBusinessInThread(void *arg);
struct excutePara 
{ 
   unsigned char bussinessId;
   struct nd_data* receiveData;
   int key;
} ;
int CheckHead(char* head);
struct nd_data Upgrade(struct nd_data* data_receive);
struct nd_data GetProcessInfo();

int input, output;
char *processPath, *c2java, * java2c;
char g_name[NAME_MAX];
char g_fifo_folder[PATH_MAX];
char g_daemon_folder[PATH_MAX];


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;/*初始化互斥锁*/  
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;/*初始化条件变量*/  

static pthread_cond_t  uiConditionVariable  = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t uiConditionMutex     = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char* argv[])
{	
	Init_g_print_log();
	DebugNdsh("ndsh version: %s\n", VERSION);
	
	if (geteuid() == 0)
	{
		//提升ndsh进程权限，将实际用户id、有效用户id和保存的设置用户id都改为0（即改为root用户） 
		if(setuid(0) == -1)
		{
			DebugNdsh("setuid errno:%d\n", errno);
		}
	}
	DebugNdsh("uid: %d    eid: %d", getuid(), geteuid());

	if (argc == 3)
	{
		processPath = argv[0];
		c2java = argv[1];
		java2c = argv[2];

		DebugNdsh("processPath: %s\nc2java: %s\njava2c: %s", processPath, c2java, java2c);
		char* index = strrchr(processPath, '/') + 1;
		if (index != NULL)
		{
			strcpy(g_name, index);
			SetProcessName(g_name);
		}
		else
		{
			ErrorNdsh("argument processPath: \"%s\" is error !", processPath);
			CloseLog();
			exit(0);
		}
		index = strrchr(c2java, '/') + 1;
		if (index != NULL)
		{
			strncpy(g_fifo_folder, c2java, index - c2java);
			DebugNdsh("g_fifo_folder: %s", g_fifo_folder);
		}
		else
		{
			ErrorNdsh("argument c2java: \"%s\" is error !", c2java);
			CloseLog();
			exit(0);
		}
		
		int length = strlen(g_fifo_folder);
		char temp[PATH_MAX];
		strncpy(temp, g_fifo_folder, length - 1);
		temp[length] = '\0';
		index = strrchr(temp, '/') + 1;
		if (index != NULL)
		{
			strncpy(g_daemon_folder, temp, index - temp);
			DebugNdsh("g_daemon_folder: %s", g_daemon_folder);
		}
		else
		{
			ErrorNdsh("argument c2java: \"%s\" is error !", c2java);
			CloseLog();
			exit(0);
		}
		
	}
	else if (argc == 1 && strcmp(argv[0], "-v") == 0)
	{
		DebugNdsh("version: %s", VERSION);
		CloseLog();
		return 0;
	}
	else
	{
		ErrorNdsh("ndsh must have three argv !");
		CloseLog();
		return 0;
	}

	Clear();
	CreateFIFO();
	
	if ((input = open(java2c, O_RDONLY)) == -1)
	{
		ErrorNdsh("open \"%s\" fail !    error: %d\n", java2c, errno);

		unlink(java2c);
		CloseLog();
		exit(0);
	}
	
	DebugNdsh("open(\"%s\", O_RDONLY) success\n", java2c);

	struct threadpool *pool = threadpool_init(10, 10);	
	while (1)
	{ 
	    usleep(10);
        char* head = Read(input, NDPC_HEAD_LENGTH);
		if(head != NULL && CheckHead(head) == 1)
		{
			unsigned char businessId = head[4];
            if(businessId == INIT_C2JAVA)
			{
				InitOutput();
			}
			else
			{
				int dataLen = 0;
				dataLen += head[5] & 0x000000FF;
				dataLen += (head[6] << 8) & 0x0000FF00;
				dataLen += (head[7] << 16) & 0x00FF0000;
				dataLen += (head[8] << 24) & 0xFF000000;
				free(head);
		
				char* buffer = NULL;
				if(dataLen > 0)
				{
					buffer = Read(input, dataLen);
					if (!buffer)
					{
						dataLen = 0;
					}
				}
				struct nd_data dataReceive = {0, dataLen, buffer};
				if(businessId == UPGRADE){	
				    ExcuteBusiness(businessId, &dataReceive,-1);
				}else{
					int key = 0;
				    if(businessId == HEART_BEAT){
					    key = -1;
					}else{
					    key = ReadIntFromBuffer(&dataReceive);	
					}
			        struct excutePara pstru;
                    pstru.bussinessId = businessId;
                    pstru.receiveData = &dataReceive;
                    pstru.key = key;
					pthread_t id;  
				    threadpool_add_job(pool, ExcuteBusinessInThread, & (pstru));
				}

			 
			}
		}
	}
	threadpool_destroy(pool);
	close(input);
	close(output);
	CloseLog();
	pthread_mutex_destroy(&mutex);  
    pthread_cond_destroy(&cond);  
	exit(0);
}

int CheckHead(char* head)
{
	if(head[0] == NDPC[0] && head[1] == NDPC[1] && head[2] == NDPC[2] && head[3] == NDPC[3])
		return 1;
	else
		return 0;
}

void Clear()
{
	KillNdsh();

	if (access(c2java, F_OK) == 0)
	{
		unlink(c2java);
	}
	if (access(java2c, F_OK) == 0)
	{
		unlink(java2c);
	}
}

void CreateFIFO()
{
	struct stat statDaemon;
	if (lstat(g_daemon_folder, &statDaemon) != 0)
	{
		ErrorNdsh("lstat(\"%s\", &statDaemon) error! errno: %d\n", g_daemon_folder, errno);

		return;
	}
	struct nd_data dataRet = MakeDir(g_fifo_folder);
	dataRet.pos = 0;
	if (ReadIntFromBuffer(&dataRet) != SUCCESS)
	{
		ErrorNdsh("MakeDir(\"%s\") fail !   error: %d\n", g_fifo_folder, errno);

		exit(0);
	}
	
	int daemonUid = statDaemon.st_uid;
	int daemonGid = statDaemon.st_gid;
	ErrorNdsh("daemonUid: %d    daemonGid: %d\n", daemonUid, daemonGid);
	int folderMod = chmod(g_fifo_folder, 0777);
	int folderOwn = chown(g_fifo_folder, daemonUid, daemonGid);

	if (mkfifo(c2java, 0777) == -1 && errno != EEXIST)
	{
		ErrorNdsh("mkfifo \"%s\" fail !    error: %d\n", c2java, errno);

		unlink(c2java);
		exit(0);
	}
	
	if (mkfifo(java2c, 0777) == -1 && errno != EEXIST)
	{
		ErrorNdsh("mkfifo \"%s\" fail !    error: %d\n", java2c, errno);

		unlink(java2c);
		exit(0);
	}

	int c2javaMod = chmod(c2java, 0777);
	int java2cMod = chmod(java2c, 0777);
	int c2javaOwn = chown(c2java, daemonUid, daemonGid);
	int java2cOwn = chown(java2c, daemonUid, daemonGid);

	DebugNdsh("mkfifo success\n");
}

void ExcuteBusiness(unsigned char businessId, struct nd_data* dataReceive,int key)
{
	struct nd_data dataSend = {0, 0, NULL};
	struct nd_data returnDataSend = {0, 0, NULL};
	enum BUSINESS_TYPE business = (enum BUSINESS_TYPE) businessId;
	if(businessId != HEART_BEAT)
	{
		char* businessTypeStr = BusinessTypeToStr(businessId);
		DebugNdsh("businessType: %s %d\n", businessTypeStr,key);
		free(businessTypeStr);
	}
	switch (business)
	{	
	case GET_PROCESS_INFO:
		dataSend = GetProcessInfo();
		break;
	case UPGRADE:
		dataSend = Upgrade(dataReceive);
		break;

	case GET_DIR:
		dataSend = GetDirInfo_nd_data(dataReceive, false);
		break;
	case GET_DIR_SUB_WRITABLE:
		dataSend = GetDirInfo_nd_data(dataReceive, true);
		break;
	case MKDIR:
		dataSend = MakeDir_nd_data(dataReceive);
		break;
	case REMOVE:
		dataSend = RemoveFile_nd_data(dataReceive);
		break;
	case MOVE:
		dataSend = MoveFile_nd_data(dataReceive);
		break;
	case COPY:
		dataSend = CopyFile_nd_data(dataReceive);
		break;
	case READ_FILE:
		dataSend = ReadFile(dataReceive);
		break;
	case GET_FILE_ATTR:
		dataSend = GetFileAttr_nd_data(dataReceive, false);
		break;
	case GET_FILE_ATTR_WRITABLE:
		dataSend = GetFileAttr_nd_data(dataReceive, true);
		break;
	case MAKE_FILE:
		dataSend = MakeFile_nd_data(dataReceive);
		break;
	case WRITE_FILE:
		dataSend = WriteFile_nd_data(dataReceive);
		break;
	case RENAME_FILE:
		dataSend = RenameFile_nd_data(dataReceive);
		break;
	case READDATA_FILE:
		dataSend = ReaddataFile_nd_data(dataReceive);
		break;	
	case SCREENSHOT:				
		dataSend = Screenshot(dataReceive);
		break;	
	case SCREENSHOTJPEG:				
		dataSend = ScreenshotJpeg(dataReceive);
		break;	
	case SCREENSIZE:				
		dataSend = ScreenSize(dataReceive);
		break;
	case SENDEVENT:				
		dataSend = Sendevent(dataReceive);
		break;	
	case EXECUTESHELLFORSTR:	
		dataSend = ExecuteShellForStr(dataReceive);
	    break;	
	case EXECUTESHELL:	
		dataSend = ExecuteShell(dataReceive);
	    break;	
	case INSTALL:
		dataSend = Install_nd_data(dataReceive);
		break;
	case UNINSTALL:
		dataSend = Uninstall_nd_data(dataReceive);
		break;
	case CHMOD:
		dataSend = Chmod_nd_data(dataReceive);
        break;
	case CHOWN_AND_GRP:
		dataSend = ChownAndGrp_nd_data(dataReceive);
		break;
	case UNINSTALL_SYSTEM_APP:
		dataSend = UninstallSystemApp_nd_data(dataReceive);
		break;
	case HEART_BEAT: //心跳包，不做回答
		break;
	case QUIT:
		{
			Clear();
			exit(0);
		}
		break;
	default:
		{
			dataSend.pos = 0;
			dataSend.size = 0;
			dataSend.buffer = NULL;
			if(key!=-1)
			    WriteIntToBuffer(&dataSend, key);
			WriteIntToBuffer(&dataSend, UNKNOW_ERROR);
			char* errBusinessType = (char*) "business is err !\n";
			WriteStringToBuffer(&dataSend, errBusinessType, strlen(errBusinessType));
		}
		break;
	}
	if(key!=-1){
	    WriteIntToBuffer(&returnDataSend, key);
        WriteStringToBuffer(&returnDataSend, dataSend.buffer, dataSend.pos);
		pthread_mutex_lock(&mutex);
	    Write(output, businessId, returnDataSend.buffer, returnDataSend.pos);	
		pthread_mutex_unlock(&mutex);
	}else{
	    pthread_mutex_lock(&mutex);
	    Write(output, businessId, dataSend.buffer, dataSend.pos);	
		pthread_mutex_unlock(&mutex);
	}
}
void *ExcuteBusinessInThread(void *arg)
{    
    
    excutePara *pstru; 
    pstru = (struct excutePara *) arg; 
    unsigned char businessId = pstru->bussinessId;
    struct nd_data* dataReceive = pstru->receiveData;
	int key = pstru->key;
    ExcuteBusiness(businessId,dataReceive,key);
    

}
void InitOutput()
{
	int cnt = 100;
	while (cnt-- > 0)
	{
		if ((output = open(c2java, O_WRONLY | O_NONBLOCK)) == -1)
		{
			if (errno == ENXIO)
			{
				DebugNdsh("fifo has not read open\n");

				usleep(50 * 1000);
				continue;
			}
			else
			{
				ErrorNdsh("open \"%s\" fail !    error: %d\n",c2java, errno);

				exit(0);
			}
		}
		else
		{
			break;
		}
	}
	if (cnt < 0)
	{
		ErrorNdsh("open(\"%s\", O_WRONLY | O_NONBLOCK)) timeout\n", c2java);

		exit(0);
	}
	
	DebugNdsh("open \"%s\" SUCCESS !\n", c2java);
}

//杀死除了自身外所有名称为"ndsh"的进程
void KillNdsh()
{
	//ErrorNdsh("KillNdsh");
	int uid = getuid();
	if (uid == 0)
	{
		char cmd[32];
		int pidSelf = getpid();
		long pidList[100];
		char cmdKill[32];
		memset(cmdKill, '\0', 32);
		int length = Find_pids_by_name(processPath, pidList, 100);
		int i;
		for (i = 0; i < length; i ++)
		{
			if (pidList[i] == pidSelf)
			{
				continue;
			}
			sprintf(cmdKill, "kill -9 %d", pidList[i]);
			ErrorNdsh(cmdKill);
			ExecuteShellCallPopen(cmdKill);

			memset(cmdKill, '\0', 32);
		}
	}
}

//升级
struct nd_data Upgrade(struct nd_data* data_receive)
{
	DebugNdsh("Upgrade(struct nd_data* data_receive)\n");

	struct nd_data dataResult = {0, 0, NULL};

	char* type = ReadStringFromBuffer(data_receive);
	char* source = ReadStringFromBuffer(data_receive);
	char* privateNdshPath = ReadStringFromBuffer(data_receive);

	char* target = (char*) "/system";
	char* suNdshPath = processPath;

	if (mount(source, target, type, MS_REMOUNT, NULL) != 0)
	{
		ErrorNdsh("mount(\"%s\", \"%s\", MS_REMOUNT, NULL) fail !    errno: %d\n", source, target);
		WriteIntToBuffer(&dataResult, UNKNOW_ERROR);

		return dataResult;
	}
	if (unlink(suNdshPath) != 0)
	{
		ErrorNdsh("unlink(\"%s\") fail !    errno: %d\n", suNdshPath, errno);
		WriteIntToBuffer(&dataResult, UNKNOW_ERROR);

		return dataResult;
	}
	CopyFile(privateNdshPath, suNdshPath);
	if (chown(suNdshPath, 0, 0) != 0)
	{
		ErrorNdsh("chown(\"%s\", 0, 0) fail !    errno: %d\n", suNdshPath, errno);
		WriteIntToBuffer(&dataResult, UNKNOW_ERROR);

		return dataResult;
	}
	if (chmod(suNdshPath, 04755) != 0)
	{
		ErrorNdsh("chmod(\"%s\", 04755) fail !    errno: %d\n", suNdshPath, errno);
		WriteIntToBuffer(&dataResult, UNKNOW_ERROR);

		return dataResult;
	}
	if (mount(source, target, type, MS_REMOUNT | MS_RDONLY, NULL) != 0)
	{
		ErrorNdsh("mount(\"%s\", \"%s\", MS_REMOUNT | MS_RDONLY, NULL) fail !    errno: %d\n", source, target, errno);
	}

	WriteIntToBuffer(&dataResult, SUCCESS);

	DebugNdsh("upgrade success\n");

	return dataResult;
}

struct nd_data GetProcessInfo()
{
	//DebugNdsh("GetProcessInfo()\n");
	
	struct nd_data dataResult = {0, 0, NULL};

	int pid = getpid();
	int ppid = getppid();
	int ruid = getuid();
	int euid = geteuid();

	WriteIntToBuffer(&dataResult, 1);
	WriteIntToBuffer(&dataResult, pid);
	WriteIntToBuffer(&dataResult, ppid);
	WriteIntToBuffer(&dataResult, ruid);
	WriteIntToBuffer(&dataResult, euid);

	return dataResult;
}

