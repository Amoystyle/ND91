package daemon.util;

/**
 * <p>
 * Title: Constants
 * </p>
 * <p>
 * Description: Constants
 * </p>
 * <p>
 * Copyright: Copyright (c) 2009
 * </p>
 * <p>
 * Company: nd
 * </p>
 * 
 * @author: Jason.Lee
 * @version: 1.0
 */

public interface IConstants {		
	// wifi
	String BROADCAST_RECEIVER = "broadcast_receiver";
	String UI_BROADCAST = "ui_broadcast";
	String DB_WIFI_PWD_KEY = "wifi_pwd";
	String DB_IS_WIFI_ON = "is_wifi_on";
	String PWD_ENCRYPT_KEY = "x!x~~119";
	String IS_MAIN_ACTIVITY_START = "is_main_activity_start"; //标记是否主界面启动Service
	
	// wifi
	String START_UDP_RECEIVER = "start_udp_receiver";
	String Action_SetWifiOff = "91PCSuite.SetWifiOff";
	String Action_WifiConnectionChanged = "91PCSuite.WifiConnectionChanged";
	String Action_CutWifiConnection = "91PCSuite.CutWifiConnection";
	String Action_RequestWifiConnection = "91PCSuite.RequestWifiConnection";
	
	//Sms
	String ACTION_SMS_RECEIVED = "android.provider.Telephony.SMS_RECEIVED";
	String ACTION_SEND_SMS_RESULT = "91PCSuite.SEND_SMS_RESULT";
	String ACTION_SEND_SMS_RESULT_LONGCONNECTION = "91PCSuite.SEND_SMS_RESULT_LONGCONNECTION";

	
	//Socket
	String UTF_8 = "UTF-8";
	byte[] TOKEN = new byte[] { (byte) 'N', (byte) 'D',(byte) 'P', (byte) 'C' };
	int HEADER_LENGTH = 16;
	int SLEEPTIME = 20;
	long TIMEOUT = 600000 / SLEEPTIME; 
	int BRFFER_LENGTH_TEST = 1024 * 128;
	int BUFFER_LENGTH = 64 * 1024;
	int SOCKET_BUFFER_SIZE = 64 * 1024;
	int PACKAGEFLAG_Unknow = 0;
	int PACKAGEFLAG_Client = 2;
	int PACKAGEFLAG_Server = 2 << 1;
	int PACKAGEFLAG_NeedPostBack = 2 << 2;
	int PACKAGEFLAG_CreateLongConnection = 2 << 3;
	int PACKAGEFLAG_CloseCurrentConnection = 2 << 4;
	int PACKAGEFLAG_CancelPushFile = 2 << 5;
	byte[] HEARTBEAT_DATA = {(byte)'N', (byte)'D', (byte)'P', (byte)'C', 4, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0};
	
	int ICECREAM_SANDWICH = 14;
}
