#pragma once

#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>

#define DATA_LOCAL_TMP "/data/local/tmp"
#define NDSH "ndsh"

#define VERSION "V0.6.12"

#define TRUE ((char) 1)
#define FALSE ((char) 0)

extern char g_process_name[];
typedef char BOOLEAN;

enum NDSH_ERROR_CODE
{
	SUCCESS = 1,
	UNKNOW_ERROR = -1,
};

enum BUSINESS_TYPE
{
	INIT_C2JAVA = 1,	//初始化命名管道
	GET_PROCESS_INFO = 2,	//获取ndsh进程信息
	UPGRADE = 3,	//升级/system/bin/ndsh

	GET_DIR = 10,	//获取文件夹数据
	MKDIR = 11,		//创建文件夹
	REMOVE = 12,		//删除文件
	MOVE = 13,		//移动文件（剪切、重命名）
	COPY = 14,		//复制文件
	READ_FILE = 15,	//读取文件数据
	GET_FILE_ATTR = 16,		//获取文件本身属性
	CHMOD = 17,     //更改文件权限
	GET_FILE_ATTR_WRITABLE = 18,	//获取文件是否可写
	GET_DIR_SUB_WRITABLE = 19,	//获取文件夹数据并获取文件夹下子文件夹写权限数据
	CHOWN_AND_GRP = 20,		//递归修改文件所有者和组
	MAKE_FILE = 21,		//创建一个文件
	WRITE_FILE = 22 ,	//往一个文件写文件
	RENAME_FILE = 23 ,	//重命名文件
	READDATA_FILE = 24 , //读取文件具体数据
	
	SCREENSHOT = 30,	//截屏
		SCREENSHOTJPEG = 31,	//截屏，压缩成jpeg格式
	SCREENSIZE = 32,	//截屏，压缩成jpeg格式

	INSTALL = 40,	//安装
	UNINSTALL = 41,	//卸载
	UNINSTALL_SYSTEM_APP = 42,	//卸载系统程序
	
	SENDEVENT = 50,   //发送屏幕命令
	
	EXECUTESHELL = 60, //SHELL命令
	EXECUTESHELLFORSTR = 61, //SHELL命令

	HEART_BEAT = 254, //心跳包
	QUIT = 255		//退出
};

struct nd_data
{
	int pos;
	int size;
	char* buffer;
};

char* BusinessTypeToStr(unsigned char type);

void ToLower(char* src, char* des, int length);
void ToUpper(char* src, char* des, int length);