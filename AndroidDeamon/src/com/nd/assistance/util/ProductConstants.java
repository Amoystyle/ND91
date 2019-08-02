package com.nd.assistance.util;

public interface ProductConstants {
	//启动控制
	String ACTION_START_SERVICE = "android.intent.action.START_SOCKET_SERVER";
	String ACTION_RELEASE_SERVICE = "android.intent.action.RELEASE_SERVER_SERVICE";
	String ACTION_START_MAIN_ACTIVITY = "android.intent.action.LAUNCH_CONFIG";	//启动主界面	
	int SERVER_PORT = 5900;
	String NDSH = "ndsh";
	String NDMONITOR = "ndmonitor";
	
	//连接确认
	String ACTION_CONNECT_CONFIRM = "91PCSuite.ConnectionConfirm";	
}
