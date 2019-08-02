package com.nd.assistance.server;

import java.io.File;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.Environment;
import android.os.IBinder;
import android.os.RemoteException;
import daemon.common.Device;
import daemon.connection.ConnectionManager;
import daemon.event.ChangeConnectListener;
import daemon.event.ConnectionEvent;
import daemon.event.IConnectConfirmListener;
import daemon.event.PcMediaStatusChangeListener;
import daemon.logger.LogCenter;
import daemon.ndk.NdkManager;
import daemon.provider.pim.Sms44;
import daemon.server.DaemonServer;
import daemon.util.SendMessageHelper;

import com.nd.assistance.R;
import com.nd.assistance.util.ProductConstants;


public class ServerService extends Service {
	public static final String IS_START_BY_UI = "is_start_by_ui"; // 标记是否主界面启动Service

	public static byte[] lock = new byte[1];
	private String TAG = getClass().getName();

	Context context;
	private DaemonServer mDaemonServer;

	private boolean mIsServerRun = false;
	private boolean mIsDestroy = false;
	private boolean mIsStarting = false;

	private IServiceCallBack mServiceCallBack = null;
	
	private String PORT_FILE =Environment.getExternalStorageDirectory()+"/nd/connect/socketport/";

	private IConnectConfirmListener mConnectConfirm = new IConnectConfirmListener() {

		@Override
		public boolean isAllowConnect(String ip, String pcName, int timeout) {
			//ConnectConfirm confirm = new ConnectConfirm(ServerService.this, ip, pcName, timeout);
			//return confirm.isAllowConnect();
			return true;
		}
	};

	@Override
	public void onCreate() {
		LogCenter.debug(TAG, "ServerService onCreate()",null,true);
		super.onCreate();

		ConnectionManager.getInstance().setConfirmListener(mConnectConfirm);
		ConnectionManager.getInstance().addChangeConnectListener(new ChangeConnectListener() {
			
			@Override
			public void disConnected(ConnectionEvent event) {
				if (!ConnectionManager.getInstance().isConnecting()) {
					LogCenter.debug(TAG, "no connection, stop service",null,true);
					if(mServiceCallBack!=null)
						try {
							mServiceCallBack.connectionChanged(null);
						} catch (RemoteException e) {
							e.printStackTrace();
						}
					ServerService.this.stopSelf();
				}
			}
			@Override
			public void connected(ConnectionEvent event) {
				try {
					if(mServiceCallBack!=null)
					    mServiceCallBack.connectionChanged(event.getConnection().getIpAddress());
				} catch (RemoteException e) {
					e.printStackTrace();
				}

			}
		});
		ConnectionManager.getInstance().setPcMediaStatusChangeListener(new PcMediaStatusChangeListener() {
			
			@Override
			public void getStatus(int status) {
				try {
					if(mServiceCallBack!=null)
					    mServiceCallBack.pcMediaStatus(status);
				} catch (RemoteException e) {
					e.printStackTrace();
				}
			}
		});
		if(Device.getIntSdkVersion()>=5){
			ServerHelper.startForeground(this);
		}


		
	}

	/**
	 * <p>
	 * overide onStart(Intent intent, int startId)
	 * <p>
	 * get the port from the extra data from the intent.
	 */
	public void onStart(Intent intent, int startId) {
		LogCenter.debug(TAG, "ServerService onStart()",null,true);
		long lngTimeBeg = System.currentTimeMillis();
		if (mIsServerRun) {
			LogCenter.debug(TAG, "mIsServerRun = ture",null,true);
		}

		mIsServerRun = true;
		boolean isMainActivityStart = false;

		Bundle extras = intent.getExtras();
		if (extras != null) {
			isMainActivityStart = extras.getBoolean(IS_START_BY_UI);
		}

		if (mIsStarting) {
			LogCenter.debug(TAG, "mIsStarting = true",null,true);
			
			return;
		}
		
		LogCenter.debug(TAG, "mIsStarting is false, set mIsStarting = true",null,true);
		mIsStarting = true;

		super.onStart(intent, startId);
		startDaemonServer(isMainActivityStart);
		
		LogCenter.debug(TAG, "set mIsStarting = false",null,true);
		mIsStarting = false;
		long lngTimeEnd = System.currentTimeMillis();
		LogCenter.debug(TAG, "ServerService onStart used time: " + (lngTimeEnd - lngTimeBeg),null,true);
	}

	private void startDaemonServer(boolean isMainActivityStart) {
		LogCenter.debug(TAG, "startDaemonServer(" + isMainActivityStart + ")",null,true);
		initNdsh();

		// close no alive deamon server
		try {
			if (isMainActivityStart && mDaemonServer != null && !ConnectionManager.getInstance().isConnecting()) {
				LogCenter.error(TAG, "**daemonServer is not null but not alive!",null,true);
				mDaemonServer.StopServer();
				mDaemonServer = null;
			}
		}
		catch (Exception e) {
			e.printStackTrace();
			String strMsg = e.getMessage();
			LogCenter.error(TAG, "exception msg: " + strMsg,null,true);
		}

		// open logcenter
		try {
			LogCenter.open();
		}
		catch (Exception e) {
			e.printStackTrace();
			String strMsg = e.getMessage();
			LogCenter.error(TAG, "exception msg: " + strMsg,null,true);
		}

		// create deamon server
		try {
			if (mDaemonServer == null) {
				LogCenter.debug(TAG, "mDaemonServer == null",null,true);
				mDaemonServer = new DaemonServer(ServerService.this.getApplicationContext(), ProductConstants.SERVER_PORT);
				if (!mDaemonServer.InitServer()) {
					LogCenter.error(TAG, "**failed to start daemon server!",null,true);
					onDestroy();
					return;
				}

				mDaemonServer.StartServer();
			}
		}
		catch (Exception e) {
			e.printStackTrace();
			String strMsg = e.getMessage();
			LogCenter.error(TAG, "exception msg: " + strMsg,null,true);
		}
	}

	private void initNdsh() {
		// 初始化ndsh
		Thread threadNdsh = new Thread() {
			public void run() {
				NdkManager.init(ServerService.this, R.raw.ndsh, ProductConstants.NDSH, R.raw.ndmonitor, ProductConstants.NDMONITOR);
			}
		};
		threadNdsh.start();
	}

	/**
	 * overide onStop()
	 */
	public void onStop() {
		mIsServerRun = false;
		LogCenter.debug(TAG, "onStop ServerService.",null,true);
	}

	/**
	 * <p>
	 * kill its process before destoryed.
	 */
	public void onDestroy() {
		//如果是4.4,且默认短信为守护
		if(Device.getIntSdkVersion()>=19){
			if(getPackageName().equals(Sms44.getDefualPakName(this))){
				Sms44.restoreDefault(this);
			}
		}
		if(Device.getIntSdkVersion()>=5){
			ServerHelper.stopForeground(this);
		}
		try {
			if(Environment.MEDIA_MOUNTED.equals(Environment.getExternalStorageState())){
				File dir =new File(PORT_FILE);
			    if(dir.exists()){
				    File[] files = dir.listFiles();
				    for (int i = 0; i < files.length; i++) {
				    	files[i].delete();
					}	
			    }
			}
		} catch (Exception e) {

		}
		
		mIsServerRun = false;
		if (mIsDestroy) {
			LogCenter.debug(TAG, "onStart exit mIsServerRun = ture.",null,true);
			return;
		}
		

		mIsDestroy = true;
		LogCenter.debug(TAG, "** Enter ServerService onDestroy. **",null,true);
		if (mDaemonServer != null) {
			mDaemonServer.StopServer();
			mDaemonServer = null;
		}

		// Close LogCenter
		try {
			LogCenter.debug(TAG, "*Close LogCenter.",null,true);
			LogCenter.close_socket_backend();
			LogCenter.close();

		}
		catch (Exception e) {
			LogCenter.debug(TAG, "*Close LogCenter Exception.",null,true);
			e.printStackTrace();
		}

		mDaemonServer = null;

		LogCenter.debug(TAG, "** End ServerService onDestroy,kill Process. **",null,true);
		android.os.Process.killProcess(android.os.Process.myPid());
	}

	@Override
	public IBinder onBind(Intent intent) {
		return mBinder;
	}

	private IRemoteService.Stub mBinder = new IRemoteService.Stub() {

		@Override
		public void registerCallback(IServiceCallBack cb) throws RemoteException {
			mServiceCallBack = cb;
		}

		@Override
		public String getConnectionInfos() throws RemoteException {
			return null;
		}

		@Override
		public void setWifiOff() throws RemoteException {
			ConnectionManager.getInstance().closeWifiConnections();
			if (!ConnectionManager.getInstance().isConnecting())
				ServerService.this.stopSelf();  
		}
		

		@Override
		public boolean isWifiConnecting() throws RemoteException {
			return ConnectionManager.getInstance().isWifiConnecting();
		}

		@Override
		public void sendMessage(int action) throws RemoteException {
		     SendMessageHelper.sendMediaMessageToPc(action);
			
		}

		@Override
		public boolean isConnecting() throws RemoteException {
			return ConnectionManager.getInstance().isConnecting();
		}

		@Override
		public int getMediaStatus() throws RemoteException {
			return ConnectionManager.getInstance().getMediaStatus();
		}

		@Override
		public void initLogCenter() throws RemoteException {
            LogCenter.init();
		}

	};
}
