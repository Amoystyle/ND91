#pragma once

#ifdef SINGLETON_EXPORTS
#define SINGLETON_API __declspec(dllexport)
#else
#define SINGLETON_API __declspec(dllimport)
#endif

// 发给主线程的消息：所有继承BaseMessage的消息都用这个类型
#define WM_SEND_TO_MAIN				(WM_USER + 100)  // 用户自定义消息
#define WM_GROUP_BEGIN				(WM_USER + 99)  // 用户自定义消息：组消息开始
#define WM_GROUP_END				(WM_USER + 98)  // 用户自定义消息：组消息结束

