/***************************************************************************//**
 *  @file 
 *  @brief [�߳̿���������ؽṹ�����������ͷ�ļ�]
 * 
 *  <b>�ļ���</b>      : ThreadHeader.h
 *  @n@n<b>��Ȩ����</b>: �����������Ӧ�����������
 *  @n@n<b>��  ��</b>  : alzq
 *  @n@n<b>����ʱ��</b>: 09-2-6 9:56:00
 *  @n@n<b>�ļ�����</b>: 
 *  @version  �汾        �޸���        ʱ��        ����@n
 *  @n        [�汾��]    alzq          2009-02-06  [����]
 * 
*******************************************************************************/
#ifndef __THREAD_HEADER__
#define __THREAD_HEADER__

#include <list>

using namespace std;

//�ź�����----------�����ź���
#define SINGLE_EVENT						0x01
#define SINGLE_MUTEX						0x02

//�̵߳ȴ�״̬
#define WAIT_INFINTE						0x01
#define WAIT_TIME_LIMIT						0x02

//�ͷ������ͷŽ��
#define RELEASE_MUTEX_ERROR					0x01
#define RELEASE_MUTEX_FAIL					0x02
#define RELEASE_MUTEX_SUC					0x04

//�����ź������ͷŽ��
#define SET_EVENT_ERROR						0x01
#define SET_EVENT_FAIL						0x02
#define SET_EVENT_SUC						0x04

//�ȴ��źŻ���ʱ�ķ��ؽ������
#define WAIT_SINGLE_ERROR					0x01		//�ȴ����ź�ָ�����
#define WAIT_SINGLE_TIMEOUT					0x02		//�ȴ�����ʱ������
#define WAIT_SINGLE_SUC						0x04		//�ȴ��ɹ�
#define WAIT_SINGLE_SUC_THREAD_EXIT			0x08		//�ȴ��ɹ������ǻ�ȡ��Ȩ�޵��߳��˳���δ�ͷ������˽��ֻ���ڵȴ��������ĵ��ú�������

//�߳�״̬
#define THREAD_STATE_ERROR					0x01		//�ȴ�״̬
#define THREAD_STATE_WAIT					0x02		//�ȴ�״̬
#define THREAD_STATE_TIMEOUT				0x04		//���һ���ȴ����Ϊ��ʱ
#define THREAD_STATE_SUC					0x08		//���һ���ȴ����Ϊ��ȡ�ɹ�
#define THREAD_STATE_SUC_THREAD_EXIT		0x10		//���һ���ȴ����Ϊ��ȡ�ɹ������ǻ�ȡ��Ȩ�޵��߳��˳���δ�ͷ������˽��ֻ���ڵȴ��������ĵ��ú�������
#define THREAD_STATE_CLOSE					0x20		//�ر��߳�

//�ȴ��������������״̬
#define WAIT_STATE_WAIT						0x01		//���ڵȴ�
#define WAIT_STATE_SUC						0x02		//�Ѿ���ȡ�ɹ�

//��ȫ�ͷ�ָ���
#ifndef SAFE_DELETE
#define SAFE_DELETE(p) {if (NULL != p) {delete p;p = NULL;}}
#endif
#define SAFE_DELETE_CHARS(p) {if (NULL != p) {delete [] p;p = NULL;}}

//�߳̿��ƶ�����Ϣ�洢�ṹ��
struct THREADTARGET
{
	BOOL isMainThread;			//�Ƿ����߳�
	HANDLE controlEvent;		//���������̵߳Ķ���
	BOOL isWinThread;			//�Ƿ񴰿��߳�
	HANDLE handle;				//�߳̾��
	LPVOID winThread;			//�����̵߳�ָ��
	DWORD threadId;				//�߳�ID���������̵߳�ʶ��

	THREADTARGET()
	{
		isMainThread = FALSE;			//�Ƿ����߳�
		isWinThread = FALSE;
		handle = NULL;
		winThread = NULL;
		threadId = NULL;

		controlEvent = CreateEvent(NULL, TRUE, FALSE, NULL);		//���������̵߳Ķ���
	}
	~THREADTARGET()
	{
		if(NULL != controlEvent)
		{
			CloseHandle(controlEvent);
			controlEvent = NULL;
		}
	}
};

//�û�ͨ������������߳�ʱ��������Ҫ��ѯ����Ϣ�洢�ṹ��
struct THREADTOUSERINFO
{
	BOOL threadEnable;			//�̵߳�ǰ�Ƿ���Ч

	THREADTOUSERINFO()
	{
		threadEnable = FALSE;
	}
};

//���ͽṹ�д洢�����ź����Ľڵ���
struct SINGLE
{
public:
protected:
	LPVOID lpSingle;			//�洢��ؽڵ�ĵ�ַ����
	DWORD singleID;				//�����ͽṹ�е�λ�ñ�ʾ����
	UINT singleType;			//��ʾ���������ź����ı���
	bool logEnable;				//�Ƿ���Ҫ����LOG���

public:
	SINGLE()
	{
		singleType = NULL;
		lpSingle = NULL;
		singleID = NULL;
		logEnable = false;
	}
	SINGLE(UINT type, LPVOID lp, DWORD id)
	{
		singleType = type;
		lpSingle = lp;
		singleID = id;
	}
	UINT GetType()
	{
		return singleType;
	}
	UINT GetId()
	{
		return singleID;
	}
	LPVOID GetSingle()
	{
		return lpSingle;
	}
	void SetLogEnable(bool enable)
	{
		logEnable = enable;
	}
	bool GetLogEnable()
	{
		return logEnable;
	}
};

//�û��ȴ��������ź�����ʾ�������Լ���ȡ״̬
struct WAITSTATE
{
	SINGLE * singleTarget;			//�ȴ��������ź����ı�־����
	UINT state;						//��ȡ״̬���������ȴ�״̬���ź�����

	WAITSTATE()
	{
		singleTarget = NULL;
		state = NULL;
	}
	WAITSTATE(SINGLE * single, UINT st)
	{
		singleTarget = single;
		state = st;
	}
};

typedef		TCHAR								CTCHAR;

typedef		list<SINGLE *>						SINGLELIST,EventList,MutexList;
typedef		list<WAITSTATE *>					SINGLEWAITLIST;
typedef		list<LPVOID>						THREADLIST;

#endif
