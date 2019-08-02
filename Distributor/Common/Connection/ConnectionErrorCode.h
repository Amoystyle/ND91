#pragma once

enum DeviceConnectError
{
	/// <summary>
	/// 连接成功
	/// </summary>
	DeviceConnectError_Check_Success,
	/// <summary>
	/// 异常导致
	/// </summary>
	DeviceConnectError_FailToActive,
	/// <summary>
	/// 异常导致
	/// </summary>
	DeviceConnectError_Check_Exception,
	/// <summary>
	/// 手机断开
	/// </summary>
	DeviceConnectError_Check_offline,
	/// <summary>
	/// 驱动安装失败
	/// </summary>
	DeviceConnectError_Check_DriverError,
	/// <summary>
	/// 调试模式没开
	/// </summary>
	DeviceConnectError_Check_OffDebug,
	/// <summary>
	/// 错误的连接模式。
	/// </summary>
	DeviceConnectError_Check_ConnectTypeError,
	/// <summary>
	/// 设备已经认定正常，可能是ADBbug。或者驱动装错了。
	/// </summary>
	DeviceConnectError_Check_ADBError,
	/// <summary>
	/// 设备断开
	/// </summary>
	DeviceConnectError_Check_UnConnect,
	/// <summary>
	/// itunes未找到设备
	/// </summary>
	DeviceConnectError_Check_ITunesNoFind,
	/// <summary>
	/// itunes安装后需要重新连接手机
	/// </summary>
	DeviceConnectError_Check_ItunesNeedReconnect,
	/// <summary>
	/// itunes安装后需要重新连接手机，但不弹窗。
	/// </summary>
	DeviceConnectError_Check_ItunesNeedReconnect2,
	/// <summary>
	/// 驱动无法加载，请查看设备管理器，根据错误原因处理
	/// </summary>
	DeviceConnectError_Check_DriverUnload,
	/// <summary>
	/// 连接模式错误，需根据提示显示UI。
	/// </summary>
	DeviceConnectError_Check_ConnectModeError,
	/// <summary>
	/// 未知
	/// </summary>
	DeviceConnectError_UnKnow,
	/// <summary>
	/// 连接成功
	/// </summary>
	DeviceConnectError_ACR_Success, 
	/// <summary>
	///	设备offline，链接失败
	/// </summary>
	DeviceConnectError_ACR_Offline, 
	/// <summary>
	///	wifi下守护升级，需提醒用户到手机上点击确认，安装后再重连
	/// </summary>
	DeviceConnectError_ACR_UpdateDeamonConfirm, 
	/// <summary>
	///	上传守护失败,空间不足
	/// </summary>
	DeviceConnectError_ACR_OffSpace, 
	/// <summary>
	///	上传守护失败，没有sdcard（wifi模式）
	/// </summary>
	DeviceConnectError_ACR_NoSDCard, 
	/// <summary>
	///	安装守护失败
	/// </summary>
	DeviceConnectError_ACR_InstallDeamonFailed, 
	/// <summary>
	///	安装守护失败，内存不足
	/// </summary>
	DeviceConnectError_ACR_OffStorage, 
	/// <summary>
	///	启动守护失败
	/// </summary>
	DeviceConnectError_ACR_StartDeamonFailed, 
	/// <summary>
	///	创建长连接失败
	/// </summary>
	DeviceConnectError_ACR_LongConnFailed, 
	/// <summary>
	///	手机重启过程中，AppManager还没启动，连接失败，此时可提醒用户手机重启完成后再连接
	/// </summary>
	DeviceConnectError_ACR_AppMgrNotStart, 
	/// <summary>
	///	端口映射失败
	/// </summary>
	DeviceConnectError_ACR_ForwardPortFailed, 
	/// <summary>
	///	设备已连接，重复调用连接导致该结果
	/// </summary>
	DeviceConnectError_ACR_AlreadyConnect, 
	/// <summary>
	///	连接模式错误
	/// </summary>
	DeviceConnectError_ACR_ModeError, 
	/// <summary>
	///	守护版本太高，此时应提醒升级助手客户端
	/// </summary>
	DeviceConnectError_ACR_HigherVerDeamon,
	/// <summary>
	/// 守护版本太低，此时应提醒用户用usb线连接，自动升级守护
	/// </summary>
	DeviceConnectError_ACR_LowerVerDeamon, 
	/// <summary>
	///	找不到设备（usb）
	/// </summary>
	DeviceConnectError_ACR_DeviceNoFound, 
	/// <summary>
	/// 连接失败
	/// </summary>
	DeviceConnectError_ACR_Failed,		
	/// <summary>
	///	设备offline
	/// </summary>
	DeviceConnectError_ACS_Offline, 
	/// <summary>
	///	接收心跳包失败
	/// </summary>
	DeviceConnectError_ACS_DisConnect, 
	/// <summary>
	///	需要连接确认（wifi模式），此时应该引导用户到手机上允许连接
	/// </summary>
	DeviceConnectError_ACS_ConnectConfirm, 
	/// <summary>
	/// 连接失败
	/// </summary>
	DeviceConnectError_ICR_Success,
	/// <summary>
	///	参数无效
	/// </summary>
	DeviceConnectError_ICR_ParamError,
	/// <summary>
	///	找不到设备
	/// </summary>
	DeviceConnectError_ICR_NoPhone,
	/// <summary>
	///	wifi需要确认
	/// </summary>
	DeviceConnectError_ICR_ConnectConfirm,
	/// <summary>
	///	usb需要解锁
	/// </summary>
	DeviceConnectError_ICR_NeedUnLock
};