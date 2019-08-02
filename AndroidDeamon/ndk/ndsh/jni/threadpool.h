#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mount.h>
#include <pthread.h>
struct job
{
    void* (*callback_function)(void *arg);    //�̻߳ص�����
    void *arg;                                //�ص���������
    struct job *next;
};

struct threadpool
{
    int thread_num;                   //�̳߳��п����̵߳ĸ���
    int queue_max_num;                //���������job�ĸ���
    struct job *head;                 //ָ��job��ͷָ��
    struct job *tail;                 //ָ��job��βָ��
    pthread_t *pthreads;              //�̳߳��������̵߳�pthread_t
    pthread_mutex_t mutex;            //�����ź���
    pthread_cond_t queue_empty;       //����Ϊ�յ���������
    pthread_cond_t queue_not_empty;   //���в�Ϊ�յ���������
    pthread_cond_t queue_not_full;    //���в�Ϊ������������
    int queue_cur_num;                //���е�ǰ��job����
    int queue_close;                  //�����Ƿ��Ѿ��ر�
    int pool_close;                   //�̳߳��Ƿ��Ѿ��ر�
};

//================================================================================================
//��������                   threadpool_init
//����������                 ��ʼ���̳߳�
//���룺                    [in] thread_num     �̳߳ؿ������̸߳���
//                         [in] queue_max_num  ���е����job���� 
//�����                    ��
//���أ�                    �ɹ����̳߳ص�ַ ʧ�ܣ�NULL
//================================================================================================
struct threadpool* threadpool_init(int thread_num, int queue_max_num);

//================================================================================================
//��������                    threadpool_add_job
//����������                  ���̳߳����������
//���룺                     [in] pool                  �̳߳ص�ַ
//                          [in] callback_function     �ص�����
//                          [in] arg                     �ص���������
//�����                     ��
//���أ�                     �ɹ���0 ʧ�ܣ�-1
//================================================================================================
int threadpool_add_job(struct threadpool *pool, void* (*callback_function)(void *arg), void *arg);

//================================================================================================
//��������                    threadpool_destroy
//����������                   �����̳߳�
//���룺                      [in] pool                  �̳߳ص�ַ
//�����                      ��
//���أ�                      �ɹ���0 ʧ�ܣ�-1
//================================================================================================
int threadpool_destroy(struct threadpool *pool);

//================================================================================================
//��������                    threadpool_function
//����������                  �̳߳����̺߳���
//���룺                     [in] arg                  �̳߳ص�ַ
//�����                     ��  
//���أ�                     ��
//================================================================================================
void* threadpool_function(void* arg);