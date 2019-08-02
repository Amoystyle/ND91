#pragma once

enum DeviceConnectError
{
	/// <summary>
	/// ���ӳɹ�
	/// </summary>
	DeviceConnectError_Check_Success,
	/// <summary>
	/// �쳣����
	/// </summary>
	DeviceConnectError_FailToActive,
	/// <summary>
	/// �쳣����
	/// </summary>
	DeviceConnectError_Check_Exception,
	/// <summary>
	/// �ֻ��Ͽ�
	/// </summary>
	DeviceConnectError_Check_offline,
	/// <summary>
	/// ������װʧ��
	/// </summary>
	DeviceConnectError_Check_DriverError,
	/// <summary>
	/// ����ģʽû��
	/// </summary>
	DeviceConnectError_Check_OffDebug,
	/// <summary>
	/// ���������ģʽ��
	/// </summary>
	DeviceConnectError_Check_ConnectTypeError,
	/// <summary>
	/// �豸�Ѿ��϶�������������ADBbug����������װ���ˡ�
	/// </summary>
	DeviceConnectError_Check_ADBError,
	/// <summary>
	/// �豸�Ͽ�
	/// </summary>
	DeviceConnectError_Check_UnConnect,
	/// <summary>
	/// itunesδ�ҵ��豸
	/// </summary>
	DeviceConnectError_Check_ITunesNoFind,
	/// <summary>
	/// itunes��װ����Ҫ���������ֻ�
	/// </summary>
	DeviceConnectError_Check_ItunesNeedReconnect,
	/// <summary>
	/// itunes��װ����Ҫ���������ֻ�������������
	/// </summary>
	DeviceConnectError_Check_ItunesNeedReconnect2,
	/// <summary>
	/// �����޷����أ���鿴�豸�����������ݴ���ԭ����
	/// </summary>
	DeviceConnectError_Check_DriverUnload,
	/// <summary>
	/// ����ģʽ�����������ʾ��ʾUI��
	/// </summary>
	DeviceConnectError_Check_ConnectModeError,
	/// <summary>
	/// δ֪
	/// </summary>
	DeviceConnectError_UnKnow,
	/// <summary>
	/// ���ӳɹ�
	/// </summary>
	DeviceConnectError_ACR_Success, 
	/// <summary>
	///	�豸offline������ʧ��
	/// </summary>
	DeviceConnectError_ACR_Offline, 
	/// <summary>
	///	wifi���ػ��������������û����ֻ��ϵ��ȷ�ϣ���װ��������
	/// </summary>
	DeviceConnectError_ACR_UpdateDeamonConfirm, 
	/// <summary>
	///	�ϴ��ػ�ʧ��,�ռ䲻��
	/// </summary>
	DeviceConnectError_ACR_OffSpace, 
	/// <summary>
	///	�ϴ��ػ�ʧ�ܣ�û��sdcard��wifiģʽ��
	/// </summary>
	DeviceConnectError_ACR_NoSDCard, 
	/// <summary>
	///	��װ�ػ�ʧ��
	/// </summary>
	DeviceConnectError_ACR_InstallDeamonFailed, 
	/// <summary>
	///	��װ�ػ�ʧ�ܣ��ڴ治��
	/// </summary>
	DeviceConnectError_ACR_OffStorage, 
	/// <summary>
	///	�����ػ�ʧ��
	/// </summary>
	DeviceConnectError_ACR_StartDeamonFailed, 
	/// <summary>
	///	����������ʧ��
	/// </summary>
	DeviceConnectError_ACR_LongConnFailed, 
	/// <summary>
	///	�ֻ����������У�AppManager��û����������ʧ�ܣ���ʱ�������û��ֻ�������ɺ�������
	/// </summary>
	DeviceConnectError_ACR_AppMgrNotStart, 
	/// <summary>
	///	�˿�ӳ��ʧ��
	/// </summary>
	DeviceConnectError_ACR_ForwardPortFailed, 
	/// <summary>
	///	�豸�����ӣ��ظ��������ӵ��¸ý��
	/// </summary>
	DeviceConnectError_ACR_AlreadyConnect, 
	/// <summary>
	///	����ģʽ����
	/// </summary>
	DeviceConnectError_ACR_ModeError, 
	/// <summary>
	///	�ػ��汾̫�ߣ���ʱӦ�����������ֿͻ���
	/// </summary>
	DeviceConnectError_ACR_HigherVerDeamon,
	/// <summary>
	/// �ػ��汾̫�ͣ���ʱӦ�����û���usb�����ӣ��Զ������ػ�
	/// </summary>
	DeviceConnectError_ACR_LowerVerDeamon, 
	/// <summary>
	///	�Ҳ����豸��usb��
	/// </summary>
	DeviceConnectError_ACR_DeviceNoFound, 
	/// <summary>
	/// ����ʧ��
	/// </summary>
	DeviceConnectError_ACR_Failed,		
	/// <summary>
	///	�豸offline
	/// </summary>
	DeviceConnectError_ACS_Offline, 
	/// <summary>
	///	����������ʧ��
	/// </summary>
	DeviceConnectError_ACS_DisConnect, 
	/// <summary>
	///	��Ҫ����ȷ�ϣ�wifiģʽ������ʱӦ�������û����ֻ�����������
	/// </summary>
	DeviceConnectError_ACS_ConnectConfirm, 
	/// <summary>
	/// ����ʧ��
	/// </summary>
	DeviceConnectError_ICR_Success,
	/// <summary>
	///	������Ч
	/// </summary>
	DeviceConnectError_ICR_ParamError,
	/// <summary>
	///	�Ҳ����豸
	/// </summary>
	DeviceConnectError_ICR_NoPhone,
	/// <summary>
	///	wifi��Ҫȷ��
	/// </summary>
	DeviceConnectError_ICR_ConnectConfirm,
	/// <summary>
	///	usb��Ҫ����
	/// </summary>
	DeviceConnectError_ICR_NeedUnLock
};