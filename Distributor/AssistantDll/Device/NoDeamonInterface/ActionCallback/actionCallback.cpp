#include "stdafx.h"

#include "actionCallback.h"
#include "itunesInterface.h"
#include "Device/NoDeamonInterface/iphoneInterface.h"
#include "InfoLog.h"
#include <windows.h>
#include <stdio.h>


int chmodChownCallback(CFDictionaryRef dict)
{
	CFStringRef actionCFString;
	PTCHAR action;
	actionCFString = (CFStringRef)CFDictionaryGetValue(dict, CFSTR("callback"));
	action = iphoneinterface_TCHARCreateFromCFStringRef(actionCFString); 

	CFStringRef pathCFString;
	PTCHAR path;
	pathCFString = (CFStringRef)CFDictionaryGetValue(dict, CFSTR("path"));
	path = iphoneinterface_TCHARCreateFromCFStringRef(pathCFString);

	//执行错误会收到下边具体错误原因
	CFStringRef errornumCFString;
	PTCHAR errornum;
	CFStringRef errorstringCFString;
	PTCHAR errorstring;

	errornumCFString = (CFStringRef)CFDictionaryGetValue(dict, CFSTR("errornum"));
	errorstringCFString = (CFStringRef)CFDictionaryGetValue(dict, CFSTR("errorstring"));
	errornum = iphoneinterface_TCHARCreateFromCFStringRef(errornumCFString);	//如果无执行错误这个返回NULL,errornumCFString值为NULL
	errorstring = iphoneinterface_TCHARCreateFromCFStringRef(errorstringCFString); //如果无执行错误这个返回NULL,errorstringCFString值为NULL

	//进度显示
	INFOLOG(_T("%s action=%s path=%s errornum=%s errorstring=%s"), _T(__FUNCTION__), action, path, errornum, errorstring);

// 	CString string;
// 	if(errornum != NULL) //执行失败
// 		string.Format(_T("%s path=%s errornum=%s errorstring=%s"), action, path, errornum, errorstring);
// 	else //无错
// 		string.Format(_T("%s path=%s"), action, path);
// 	dlg->addEditString(string);

	iphoneinterface_delete(action);
	iphoneinterface_delete(path);
	iphoneinterface_delete(errornum);
	iphoneinterface_delete(errorstring);

	return 0;
}

int pxlCallback(CFDictionaryRef dict)
{
	CFStringRef callbackCFString;
	PTCHAR callback;
	callbackCFString = (CFStringRef)CFDictionaryGetValue(dict, CFSTR("callback"));
	callback = iphoneinterface_TCHARCreateFromCFStringRef(callbackCFString); 

	CFStringRef currentActionCFString;
	PTCHAR currentAction;
	currentActionCFString = (CFStringRef)CFDictionaryGetValue(dict, CFSTR("action"));
	currentAction = iphoneinterface_TCHARCreateFromCFStringRef(currentActionCFString);

	CFStringRef scheduleCFString;
	PTCHAR schedule;
	scheduleCFString = (CFStringRef)CFDictionaryGetValue(dict, CFSTR("schedule"));
	schedule = iphoneinterface_TCHARCreateFromCFStringRef(scheduleCFString);

	CFStringRef stepCFString;
	PTCHAR step;
	stepCFString = (CFStringRef)CFDictionaryGetValue(dict, CFSTR("step"));
	step = iphoneinterface_TCHARCreateFromCFStringRef(stepCFString);

	//执行错误会收到下边具体错误原因
	CFStringRef errorstringCFString;
	PTCHAR errorstring;
	errorstringCFString = (CFStringRef)CFDictionaryGetValue(dict, CFSTR("errorstring"));
	errorstring = iphoneinterface_TCHARCreateFromCFStringRef(errorstringCFString); //如果无执行错误这个返回NULL,errorstringCFString值为NULL

	//进度显示
	INFOLOG(_T("%s callback=%s currentAction=%s schedule=%s step=%s errorstring=%s"), _T(__FUNCTION__), callback, currentAction, schedule, step, errorstring);

// 	CString string;
// 	if(errorstring == NULL) //无错
// 		string.Format(_T("callback=%s currentAction=%s schedule=%s step=%s"), callback, currentAction, schedule, step);
// 	else //执行失败
// 		string.Format(_T("callback=%s errorstring=%s"), callback, errorstring);
// 	dlg->addEditString(string);

	iphoneinterface_delete(callback);
	iphoneinterface_delete(currentAction);
	iphoneinterface_delete(schedule);
	iphoneinterface_delete(step);
	iphoneinterface_delete(errorstring);

	return 0;
}