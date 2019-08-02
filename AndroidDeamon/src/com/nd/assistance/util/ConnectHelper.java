package com.nd.assistance.util;



import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;

import com.nd.assistance.controller.DaemonController;
import com.nd.assistance.server.IRemoteService;
import com.nd.assistance.server.IServiceCallBack;
import com.nd.assistance.server.ServerService;

import daemon.util.ConfigManager;
import daemon.util.LinkCodeMath;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;
import android.view.Window;

public class ConnectHelper {

	public static ConnectHelper sSConnectHelper;
	
	public static ConnectHelper getInstance(){
		if(sSConnectHelper==null){
			sSConnectHelper = new ConnectHelper();
		}
		return sSConnectHelper;
	}
	
	private Context mContext;
	
	public void init(Context context){
		mContext = context;
	}
	
	private  IRemoteService mService;
	
	
	
	private final IServiceCallBack.Stub mCallBack = new IServiceCallBack.Stub() {
		@Override
		public void connectionChanged(String connInfos) throws RemoteException {
			if (connInfos == null) {
				if (mDisConnectToPcLister != null) {
					mDisConnectToPcLister.disConnect();
				}
			} else {
				if (mConnectToPcLister != null) {
					mConnectToPcLister.connect();
				}
			}

		}

		@Override
		public void pcMediaStatus(int status) throws RemoteException {
			if (mediaStatusLister != null) {
				mediaStatusLister.pcMeidaStatus(status);
			}
		}
	};
	
	private  ServiceConnection mServiceConn = new ServiceConnection() {

		@Override
		public void onServiceDisconnected(ComponentName name) {
			mService = null;
		}

		@Override
		public void onServiceConnected(ComponentName name, IBinder service) {
			mService = IRemoteService.Stub.asInterface(service);
			try {
				mService.registerCallback(mCallBack);
			} catch (RemoteException e) {
				e.printStackTrace();
			}
		}
	}; 
	
	
	
	public  void setWifiStatus(boolean isWifiOn) {
		//ConfigManager.setIsWifiOn(mContext, isWifiOn);
		if (isWifiOn) {
			startService();
		}
		else {

			if (mService != null) {
				try {
					mService.setWifiOff();
					
				}
				catch (Exception e) {
					e.printStackTrace();
				}
			}
			if(mServiceConn!=null){
				try {
					mContext.unbindService(mServiceConn);
				}
				catch (Exception e) {
					e.printStackTrace();
				}
			}
			mService =null;
		}
	}
	
	public boolean isWifiConnecting(){
		if(mService!=null){
			try {
				return mService.isWifiConnecting();
			} catch (RemoteException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		return false;
	}
	
	public boolean isConnecting(){
		if(mService!=null){
			try {
				return mService.isConnecting();
			} catch (RemoteException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		return false;
	}
	
	public void initLogCenter(){
		if(mService!=null){
			try {
				 mService.initLogCenter();
			} catch (RemoteException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	
	public boolean canUseControl(){
		if(getMediaControlStatus()==10){
			return false;
		}
		return true;
	}
	
	public int getMediaControlStatus(){
		if(mService!=null){
			try {
				return mService.getMediaStatus();
			} catch (RemoteException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		
		return 10;
	}
	
	public void sendMessage(int action){
		if(mService!=null){
			try {
				mService.sendMessage(action);
			} catch (RemoteException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}

	public  void startService() {
		DaemonController.StartServer(mContext, true);
		Intent intent = new Intent(mContext, ServerService.class);
		mContext.bindService(intent, mServiceConn, Context.BIND_AUTO_CREATE);
	}
	
	public interface ConnectToPcLister{
		public void connect();
	}
	ConnectToPcLister mConnectToPcLister;
	public void setConnectToPcLister(ConnectToPcLister o){
		mConnectToPcLister = o;
	}
	
	public interface DisConnectToPcLister{
		public void disConnect();
	}
	DisConnectToPcLister mDisConnectToPcLister;
	public void setDisConnectToPcLister(DisConnectToPcLister o){
		mDisConnectToPcLister = o;
	}
	
	public interface PcMediaStatusLister{
		public void  pcMeidaStatus(int status);
	}
	PcMediaStatusLister mediaStatusLister;
	public void setPcMediaStatusLister(PcMediaStatusLister o){
		mediaStatusLister = o;
	}
	
	public void startTcpConnect(final String ips ,final String port,final Handler mHandler){
		//尝试连接
		final String[] ipList =ips.split("\\|");
		new Thread(){
			public void run(){
				boolean connetResult = false;
				int count = ipList.length;
				int timeout =10*1000;
				if(count>1){
					timeout = 5*1000;
				}
				for (int i = 0; i < count; i++) {
					try {
						String ip = ipList[i];
						if(!(isIp(ip))){
							ip = LinkCodeMath.AnalyzeCode(ip);
							if(!(isIp(ip))){
								continue;
							}
						}
						Log.d("TCP", "C: Connecting...");
						Socket socket = new Socket();
						
						SocketAddress remoteAddr = new InetSocketAddress(ip, Integer.parseInt(port)) ;
						socket.connect(remoteAddr,timeout);
						String message = "NDPC;22;" + Build.MODEL;
						try {
							Log.d("TCP", "C: Sending: '" + message + "'");
							PrintWriter out = new PrintWriter(new BufferedWriter(
									new OutputStreamWriter(socket.getOutputStream())), true);
							out.println(message);
							connetResult = true;
							break;
						} catch (Exception e) {
							Log.e("CameraActivty", "S: Error", e);
							connetResult = false;
						} finally {
							socket.close();
						}
					} catch (IOException e) {
						Log.e("try tcp connecting", e.toString());
					}
				}
				if(connetResult){
					mHandler.sendEmptyMessage(1);
				}else{
					mHandler.sendEmptyMessage(0);
				}
				
			}
		}.start();
	}
	
	public boolean isIpInfo(String ipInfo){
		final String[] ipList =ipInfo.split("\\|");
		for (int i = 0; i < ipList.length; i++) {
			String ip = ipList[i];
			if(!isIp(ip)){
				ip = LinkCodeMath.AnalyzeCode(ip);
				if(ip!=null){
					return isIp(ip);
				}
			}else{
				return true;
			}
		}
		return false;
	}
	
	public  boolean isIp(String IP){
		if(IP==null){
			return false;
		}
        boolean b = false; 
        if(IP.matches("^((\\d|(\\d\\d)|(1\\d\\d)|(2[0-4]\\d)|(25[0-5]))\\.){3}(\\d|(\\d\\d)|(1\\d\\d)|(2[0-4]\\d)|(25[0-5]))$")){ 
             b = true; 
        }
        return b; 
    } 
	public final static int UNKNOWN =-1;
	public final static int OLD = 1;
	public final static int NEW =2;
	public final static int HTTP = 3;
	
	public  int getCodeType(String info){
		info =info.toLowerCase();
        int type = UNKNOWN;
        if(info.matches("pcip:.*:(\\d+):(\\d+):(2|4)")){ 
        	type = OLD;
        }else if(info.matches("pcip:.*:(\\d+):(2|4)")){
        	type = NEW;
        }else if(info.matches("http://.*qr\\?.*,(\\d+),(2|4)")){
        	type = HTTP;
        }
        return type; 
    }
	

	public void startTcpConnectByInfo(String info, Handler mConnectHandler) {
		int type = getCodeType(info);
		QcodeInfo qcodeInfo =getQcodeInfo(info,type);
		startTcpConnect(qcodeInfo.ip, qcodeInfo.port, mConnectHandler);
		
	} 
	
	public QcodeInfo getQcodeInfo(String info,int type){
		QcodeInfo qcodeInfo =new QcodeInfo();
		// 1 二维码信息协议不对
		// 2 IP信息不对
		switch (type) {
		case ConnectHelper.HTTP: {
			int index = info.indexOf("qr?");
			String qrcodeinfo = info.substring(index + "qr?".length());
			int nextIndex = qrcodeinfo.indexOf(",");
			qcodeInfo.ip = qrcodeinfo.substring(0, nextIndex);
			if (!ConnectHelper.getInstance().isIpInfo(qcodeInfo.ip)) {
				qcodeInfo.errorCode = 2;
				return qcodeInfo;
			}
			qrcodeinfo = qrcodeinfo.substring(nextIndex + ",".length());
			nextIndex = qrcodeinfo.indexOf(",");
			qcodeInfo.port = qrcodeinfo.substring(0, nextIndex);
		}
			break;
		case ConnectHelper.OLD:
		case ConnectHelper.NEW: {
			final String[] infos = info.split(":");
			qcodeInfo.ip = infos[1];
			qcodeInfo.port = infos[2];
			if (!ConnectHelper.getInstance().isIpInfo(qcodeInfo.ip)) {
				qcodeInfo.errorCode = 2;
				return qcodeInfo;
			}
		}
			break;
		default:
			qcodeInfo.errorCode = 1;
			break;
		}
		return qcodeInfo;
	}
	
	
	public class QcodeInfo{
		public String ip;
		public String port;
		public int errorCode=0;
	}
}
