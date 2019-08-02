#pragma once
#include "struct.h"

enum MESSAGE_TYPE
{
    USER_KEY              , // 用户按键
    PROMPT_BOX            , // 提示对话框：OK按钮
    CONFIRM_BOX           , // 确认对话框：OK/Cancel按钮
	CONFIRMWARN_BOX       , // 确认对话框：OK/Cancel按钮（警告型）
    INPUT_BOX             , // 输入对话框：输入字符串，OK/Cancel按钮
    NOTICE_BOX            , // 自动关闭提示框 
    PROGRESS_BOX          , // 进度对话框： 取消/跳过按钮
    EXIST_ACTION_BOX      , // 文件已存在对话框：询问用户如何操作
	Refresh_APP			  ,	// 文件管理，刷新应用软件名称
	Refresh_Node		  ,	// 文件管理，刷新当前节点
	BackUpAndRestorePro   ,	// 备份还原进度条数据
	BackUpMsg   ,			// 备份取消或跳过时发送消息
	RestoreMsg,				// 还原取消或跳过时发送消息
    BackupAndRestoreState,  // 备份还原过程状态变化
	BackUpAndRestoreProVisible   ,	// 备份还原进度条显示与隐藏
	AotoSelectOtherRestore   ,	// 任一设备份了程序其他设备均需显示此备份
	Update_DeviceUI,		//设备越狱完之后将自动更新界面相关信息
	Vitrue_Progress,		//虚拟进度条
	FileManager_InitRoot,
	StopBackUpAndRestore,	//终止备份还原
};
