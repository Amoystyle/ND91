#pragma once

#ifdef SINGLETON_EXPORTS
#define SINGLETON_API __declspec(dllexport)
#else
#define SINGLETON_API __declspec(dllimport)
#endif

// �������̵߳���Ϣ�����м̳�BaseMessage����Ϣ�����������
#define WM_SEND_TO_MAIN				(WM_USER + 100)  // �û��Զ�����Ϣ
#define WM_GROUP_BEGIN				(WM_USER + 99)  // �û��Զ�����Ϣ������Ϣ��ʼ
#define WM_GROUP_END				(WM_USER + 98)  // �û��Զ�����Ϣ������Ϣ����

