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
	INIT_C2JAVA = 1,	//��ʼ�������ܵ�
	GET_PROCESS_INFO = 2,	//��ȡndsh������Ϣ
	UPGRADE = 3,	//����/system/bin/ndsh

	GET_DIR = 10,	//��ȡ�ļ�������
	MKDIR = 11,		//�����ļ���
	REMOVE = 12,		//ɾ���ļ�
	MOVE = 13,		//�ƶ��ļ������С���������
	COPY = 14,		//�����ļ�
	READ_FILE = 15,	//��ȡ�ļ�����
	GET_FILE_ATTR = 16,		//��ȡ�ļ���������
	CHMOD = 17,     //�����ļ�Ȩ��
	GET_FILE_ATTR_WRITABLE = 18,	//��ȡ�ļ��Ƿ��д
	GET_DIR_SUB_WRITABLE = 19,	//��ȡ�ļ������ݲ���ȡ�ļ��������ļ���дȨ������
	CHOWN_AND_GRP = 20,		//�ݹ��޸��ļ������ߺ���
	MAKE_FILE = 21,		//����һ���ļ�
	WRITE_FILE = 22 ,	//��һ���ļ�д�ļ�
	RENAME_FILE = 23 ,	//�������ļ�
	READDATA_FILE = 24 , //��ȡ�ļ���������
	
	SCREENSHOT = 30,	//����
		SCREENSHOTJPEG = 31,	//������ѹ����jpeg��ʽ
	SCREENSIZE = 32,	//������ѹ����jpeg��ʽ

	INSTALL = 40,	//��װ
	UNINSTALL = 41,	//ж��
	UNINSTALL_SYSTEM_APP = 42,	//ж��ϵͳ����
	
	SENDEVENT = 50,   //������Ļ����
	
	EXECUTESHELL = 60, //SHELL����
	EXECUTESHELLFORSTR = 61, //SHELL����

	HEART_BEAT = 254, //������
	QUIT = 255		//�˳�
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