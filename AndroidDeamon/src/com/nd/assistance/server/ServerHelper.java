package com.nd.assistance.server;

import android.annotation.SuppressLint;
import android.app.Notification;
import android.app.Service;

public class ServerHelper {

	@SuppressLint("NewApi")
	public static void startForeground(Service server){
		try {
			Notification notification  =new Notification();
			server.startForeground(0x1982, notification);
		} catch (Exception e) {
			// TODO: handle exception
		}
	}
	
	@SuppressLint("NewApi")
	public static void stopForeground(Service server){
		try {
			server.stopForeground(true);
		} catch (Exception e) {
			// TODO: handle exception
		}
	}
}
