#pragma once
#include "struct.h"

enum MESSAGE_TYPE
{
    USER_KEY              , // �û�����
    PROMPT_BOX            , // ��ʾ�Ի���OK��ť
    CONFIRM_BOX           , // ȷ�϶Ի���OK/Cancel��ť
	CONFIRMWARN_BOX       , // ȷ�϶Ի���OK/Cancel��ť�������ͣ�
    INPUT_BOX             , // ����Ի��������ַ�����OK/Cancel��ť
    NOTICE_BOX            , // �Զ��ر���ʾ�� 
    PROGRESS_BOX          , // ���ȶԻ��� ȡ��/������ť
    EXIST_ACTION_BOX      , // �ļ��Ѵ��ڶԻ���ѯ���û���β���
	Refresh_APP			  ,	// �ļ�����ˢ��Ӧ���������
	Refresh_Node		  ,	// �ļ�����ˢ�µ�ǰ�ڵ�
	BackUpAndRestorePro   ,	// ���ݻ�ԭ����������
	BackUpMsg   ,			// ����ȡ��������ʱ������Ϣ
	RestoreMsg,				// ��ԭȡ��������ʱ������Ϣ
    BackupAndRestoreState,  // ���ݻ�ԭ����״̬�仯
	BackUpAndRestoreProVisible   ,	// ���ݻ�ԭ��������ʾ������
	AotoSelectOtherRestore   ,	// ��һ�豸���˳��������豸������ʾ�˱���
	Update_DeviceUI,		//�豸Խ����֮���Զ����½��������Ϣ
	Vitrue_Progress,		//���������
	FileManager_InitRoot,
	StopBackUpAndRestore,	//��ֹ���ݻ�ԭ
};
