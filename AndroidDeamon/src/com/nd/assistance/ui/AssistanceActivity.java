package com.nd.assistance.ui;

import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.Socket;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.DialogInterface.OnCancelListener;
import android.graphics.Color;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.Vibrator;
import android.provider.Settings;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnTouchListener;
import android.view.Window;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.nd.assistance.R;
import com.nd.assistance.ui.RemoterView.ListerRemoterViewClick;
import com.nd.assistance.util.ConnectHelper;
import com.nd.assistance.util.ConnectHelper.ConnectToPcLister;
import com.nd.assistance.util.ConnectHelper.DisConnectToPcLister;
import com.nd.assistance.util.ConnectHelper.PcMediaStatusLister;

import daemon.receiver.SendMessageType;
import daemon.util.ConfigManager;
import daemon.util.LinkCodeMath;

public class AssistanceActivity extends Activity {
	private WifiManager mWifiManager;

	private TextView mLinkCode;

	private RelativeLayout mScan;
	// 断开连接
	private TextView mDiscanText;
	// 立即扫描
	private LinearLayout mScanLiner;
	// 说明图片
	private ImageView mImgScan;
	// 说明文字
	private TextView mInfoTxt;
	// 通知栏开关
	private ImageView mNotifychange;
	// 是否wifi连接中
	private boolean mIsWifiConnecting;

	// WIFI切换文字
	private TextView mTabWifi;
	// WIFI中间
	private RelativeLayout mWifiContext;
	// WIFI底部
	private RelativeLayout mWifiBottom;
	// 遥控器切换文字
	private TextView mTabControl;
	// 遥控器中间
	private RelativeLayout mContorlContext;
	// 遥控器底部
	private RelativeLayout mContorlBottom;
	private RemoterView mRemoterView;

	private SensorManager sensorManager;
	private Vibrator vibrator;
	private static final int SENSOR_SHAKE = 10;
	private boolean canUseControl;
	private ProgressDialog mLoadingDialog;

	private void turntoTab(boolean isWifi) {
		if (isWifi) {
			mTabWifi.setTextColor(0xffffd800);
			mTabWifi.setBackgroundResource(R.drawable.second_title_bg_select);
			mWifiContext.setVisibility(View.VISIBLE);
			mWifiBottom.setVisibility(View.VISIBLE);
			mTabControl.setTextColor(0xff969696);
			mTabControl.setBackgroundResource(R.drawable.second_title_bg);
			mContorlContext.setVisibility(View.GONE);
			mContorlBottom.setVisibility(View.GONE);
		} else {
			mTabWifi.setTextColor(0xff969696);
			mTabWifi.setBackgroundResource(R.drawable.second_title_bg);
			mWifiContext.setVisibility(View.GONE);
			mWifiBottom.setVisibility(View.GONE);
			mTabControl.setTextColor(0xffffd800);
			mTabControl.setBackgroundResource(R.drawable.second_title_bg_select);
			mContorlContext.setVisibility(View.VISIBLE);
			mContorlBottom.setVisibility(View.VISIBLE);
		}
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		setContentView(R.layout.main);
		mTabWifi = (TextView) findViewById(R.id.main_tab_wifi);
		mTabWifi.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				turntoTab(true);
			}
		});
		mWifiContext = (RelativeLayout) findViewById(R.id.main_content);
		mWifiBottom = (RelativeLayout) findViewById(R.id.main_tool);
		mTabControl = (TextView) findViewById(R.id.main_tab_control);
		mTabControl.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				turntoTab(false);
			}
		});
		mContorlContext = (RelativeLayout) findViewById(R.id.control_content);
		mContorlBottom = (RelativeLayout) findViewById(R.id.control_tool);
		mNotifychange = (ImageView) findViewById(R.id.main_notifychange);
		mLinkCode = (TextView) findViewById(R.id.content_code);
		mScanLiner = (LinearLayout) findViewById(R.id.contecnt_scan_start);
		mImgScan = (ImageView) findViewById(R.id.content_img);
		mInfoTxt = (TextView) findViewById(R.id.content_txt);
		turntoTab(true);
		mWifiManager = (WifiManager) getSystemService(Context.WIFI_SERVICE);

		mScan = (RelativeLayout) findViewById(R.id.content_scan);
		mScanLiner = (LinearLayout) findViewById(R.id.contecnt_scan_start);
		mDiscanText = (TextView) findViewById(R.id.content_conttect_txt);
		changeToScanInit(true);
		ConfigManager.setIsWifiOn(AssistanceActivity.this, true);

		boolean showNotify = ConfigManager
				.getIsWifiShowNotify(AssistanceActivity.this);
		if (showNotify) {
			mNotifychange.setImageResource(R.drawable.notify_on);
		} else {
			mNotifychange.setImageResource(R.drawable.notify_off);
		}
		mNotifychange.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				boolean showNotify = ConfigManager.getIsWifiShowNotify

				(AssistanceActivity.this);
				if (showNotify) {
					mNotifychange.setImageResource(R.drawable.notify_off);
					NotificationManager nm = (NotificationManager) getSystemService(Context.NOTIFICATION_SERVICE);
					nm.cancelAll();
				} else {
					mNotifychange.setImageResource

					(R.drawable.notify_on);
					if (mIsWifiConnecting) {
						showWifiNotify();
					}
				}
				ConfigManager.setIsWifiShowNotify(AssistanceActivity.this,
						!showNotify);
			}
		});

		ImageButton backBtn = (ImageButton) findViewById(R.id.buttonBack);
		backBtn.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				finish();
			}
		});
		ImageButton helpBtn = (ImageButton) findViewById(R.id.buttonHelp);
		helpBtn.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				AssistanceActivity.this.startActivity(new Intent

				(AssistanceActivity.this, HelpActivity.class));

			}
		});
		ConnectHelper.getInstance().init(this);
		ConnectHelper.getInstance().setDisConnectToPcLister(new

		DisConnectToPcLister() {

			@Override
			public void disConnect() {
				NotificationManager nm = (NotificationManager)

				getSystemService(Context.NOTIFICATION_SERVICE);
				nm.cancelAll();
				mHandler.sendEmptyMessage(0);
			}
		});
		ConnectHelper.getInstance().setConnectToPcLister(
				new ConnectToPcLister() {

					@Override
					public void connect() {
						mHandler.sendEmptyMessage(1);
					}
				});
		sensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
		vibrator = (Vibrator) getSystemService(VIBRATOR_SERVICE);
		
		mRemoterView =(RemoterView)findViewById(R.id.control_remoteer);
		mRemoterView.SetListerRemoterViewClick(new ListerRemoterViewClick() {
			
			@Override
			public void OnClick(int action) {
				sendRemoterMessage(action);
			}
		});
		ImageButton close = (ImageButton)findViewById(R.id.control_close);
		close.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				
				closePc();
			}
		});
		ConnectHelper.getInstance().setPcMediaStatusLister(new PcMediaStatusLister() {
			
			@Override
			public void pcMeidaStatus(int status) {
				updateStatus(status);
			}
		});
		showControl(true);
		canUseControl =false;
		
		mLoadingDialog = new ProgressDialog(this);
		if(!ConnectHelper.getInstance().isWifiConnecting()){
			Intent  i = getIntent();
			final String ip = i.getStringExtra("ip");
			final String port = i.getStringExtra("port");
			//旧版本
			if(ip!=null&&port!=null){
				//Toast.makeText(this, ip+":"+port, 3000).show();
				mLoadingDialog.show();
				mLoadingDialog.setContentView(R.layout.processdialog);
				mLoadingDialog.setCancelable(true);
				ConnectHelper.getInstance().startTcpConnect(ip, port,mConnectHandler);
			}else{
				//新版本
				final String info = i.getStringExtra("info");
				if(info!=null){
					mLoadingDialog.show();
					mLoadingDialog.setContentView(R.layout.processdialog);
					mLoadingDialog.setCancelable(true);
					ConnectHelper.getInstance().startTcpConnectByInfo(info,mConnectHandler);
				}

			}
		}
		
		
		
	}
	
	Handler mConnectHandler = new Handler(){
        public void handleMessage(Message msg) {   
            super.handleMessage(msg);   
    		if (msg.what==0) {
                //失败
    			mLoadingDialog.dismiss();
    			showConnetFail();
    		}else if(msg.what==1){
    			//成功
    			ConnectHelper.getInstance().setConnectToPcLister(new ConnectToPcLister() {
					@Override
					public void connect() {
						mConnectHandler.sendEmptyMessage(2);
					}
				});
    		}else if(msg.what==2){
				changeToScanInit(false);
				mLoadingDialog.dismiss();
    		}
       }   
	};
	
	private void updateStatus(int status){
		if(status==10){
            //关闭
			//mShowControlHandler.sendEmptyMessage(0);
		}else if(status==11){
			//打开
			//mShowControlHandler.sendEmptyMessage(1);
		}else if(status==0){
			mRemoterView.setPlaying(true);
			//mShowControlHandler.sendEmptyMessage(1);
		}else{
			mRemoterView.setPlaying(false);
			//mShowControlHandler.sendEmptyMessage(1);
		}
	}
	
	
//	Handler mShowControlHandler = new Handler() {
//		public void handleMessage(Message msg) {
//			super.handleMessage(msg);
//			if (msg.what == 0) {
//				showControl(false);
//			} else {
//				showControl(true);
//			}
//
//		}
//	};
	
	private void showControl(boolean show){
//		if(show){
			mTabWifi.setVisibility(View.VISIBLE);
			mTabControl.setVisibility(View.VISIBLE);
//		}else{
//			mTabWifi.setVisibility(View.GONE);
//			mTabControl.setVisibility(View.GONE);
//			turntoTab(true);
//		}
	//	showControl = show;
	}

	protected void sendRemoterMessage(int action) {
		if(ConnectHelper.getInstance().isConnecting()){
			if(!ConnectHelper.getInstance().canUseControl()){
				//Toast.makeText(AssistanceActivity.this, "您所连接的助手版本，暂不支持遥控器功能", 2000).show();
				AlertDialog.Builder builder = new Builder(AssistanceActivity.this);
				builder.setMessage("您所连接的助手版本，暂不支持遥控器功能");
				builder.setTitle("提示");
				builder.setPositiveButton(android.R.string.ok, null);
				builder.create().show();
				return;
			}
			ConnectHelper.getInstance().sendMessage(action);
			return;
		}
		AlertDialog.Builder builder = new Builder(AssistanceActivity.this);
		builder.setMessage("设备未连接上PC，立即连接？");
		builder.setTitle("提示");
		builder.setPositiveButton(android.R.string.ok, new
		DialogInterface.OnClickListener() {

			@Override
			public void onClick(DialogInterface dialog, int which) {
				turntoTab(true);
			}
		});

		builder.setNegativeButton(android.R.string.cancel, null);
		builder.create().show();
	}

	protected void closePc() {
		if(!ConnectHelper.getInstance().isConnecting()){
			sendRemoterMessage(0);
			return;
		}
		if(!ConnectHelper.getInstance().canUseControl()){
			AlertDialog.Builder builder = new Builder(AssistanceActivity.this);
			builder.setMessage("您所连接的助手版本，暂不支持遥控器功能");
			builder.setTitle("提示");
			builder.setPositiveButton(android.R.string.ok, null);
			builder.create().show();
			return;
		}
		AlertDialog.Builder builder = new Builder(AssistanceActivity.this);
		builder.setMessage("该操作将关闭您的电脑，是否继续？");
		builder.setTitle("提示");
		builder.setPositiveButton(android.R.string.ok, new
		DialogInterface.OnClickListener() {

			@Override
			public void onClick(DialogInterface dialog, int which) {
				ConnectHelper.getInstance().sendMessage(SendMessageType.SYSTEM_PC_CLOSE);
			}
		});

		builder.setNegativeButton(android.R.string.cancel, null);
		builder.create().show();
		
	}

	

	@Override
	protected void onStop() {
		super.onStop();
		if (sensorManager != null) {
			sensorManager.unregisterListener(sensorEventListener);
		}

	}
	
	


	@Override
	protected void onDestroy() {
		if(!ConnectHelper.getInstance()
				.isConnecting()){
			ConnectHelper.getInstance().setWifiStatus(false);
		}
		super.onDestroy();
	}





	/**
	 * 重力感应监听
	 */
	private SensorEventListener sensorEventListener = new SensorEventListener() {

		@Override
		public void onSensorChanged(SensorEvent event) {
			if(!ConnectHelper.getInstance().canUseControl()){
				return;
			}
			float[] values = event.values;
			float x = values[0];
			float y = values[1]; 
			float z = values[2]; 
			int medumValue = 17;
			if (Math.abs(x) > medumValue || Math.abs(y) > medumValue
					|| Math.abs(z) > medumValue) {
				vibrator.vibrate(200);
				Message msg = new Message();
				msg.what = SENSOR_SHAKE;
				mSensorHandler.sendMessage(msg);
			}
		}

		@Override
		public void onAccuracyChanged(Sensor sensor, int accuracy) {

		}
	};

	/**
	 * 动作执行
	 */
	Handler mSensorHandler = new Handler() {

		@Override
		public void handleMessage(Message msg) {
			super.handleMessage(msg);
			switch (msg.what) {
			case SENSOR_SHAKE:
				ConnectHelper.getInstance().sendMessage(SendMessageType.SYSTEM_PC_ROCK);
				break;
			}
		}

	};
	Handler mHandler = new Handler() {
		public void handleMessage(Message msg) {
			super.handleMessage(msg);
			if (msg.what == 0) {
				changeToScanInit(true);
			} else {
				changeToScanInit(false);
			}

		}
	};

	private void changeToScanInit(boolean init) {
		mIsWifiConnecting = !init;
		if (init) {
			mScan.setBackgroundResource(R.drawable.scan_normal);
			mScan.setOnTouchListener(new OnTouchListener() {

				@Override
				public boolean onTouch(View v, MotionEvent event) {
					if (event.getAction() == MotionEvent.ACTION_DOWN) {
						mScan.setBackgroundResource

						(R.drawable.scan_select);
						return true;
					} else if (event.getAction() ==

					MotionEvent.ACTION_UP) {
						mScan.setBackgroundResource

						(R.drawable.scan_normal);
						startScan();
					}
					return false;
				}
			});
			mScanLiner.setVisibility(View.VISIBLE);
			mDiscanText.setVisibility(View.INVISIBLE);
			mLinkCode.setVisibility(View.VISIBLE);
			mImgScan.setImageResource(R.drawable.qcode_img);
			mInfoTxt.setText(R.string.scan_info);
			mInfoTxt.setTextColor(Color.argb(0xff, 0xff, 0xff, 0xff));
		} else {
			if (ConfigManager.getIsWifiShowNotify(AssistanceActivity.this)) {
				showWifiNotify();
			}
			mScan.setBackgroundResource(R.drawable.disconect_normal);
			mScan.setOnTouchListener(new OnTouchListener() {

				@Override
				public boolean onTouch(View v, MotionEvent event) {
					if (event.getAction() == MotionEvent.ACTION_DOWN) {
						mScan.setBackgroundResource

						(R.drawable.disconect_select);
						return true;
					} else if (event.getAction() ==

					MotionEvent.ACTION_UP) {
						dialog();
					}
					return false;
				}
			});
			mScanLiner.setVisibility(View.INVISIBLE);
			mDiscanText.setVisibility(View.VISIBLE);
			mLinkCode.setVisibility(View.INVISIBLE);
			mImgScan.setImageResource(R.drawable.connect_img);
			mInfoTxt.setText(R.string.qcode_success);
			mInfoTxt.setTextColor(Color.argb(0xff, 0x81, 0xc3, 0x04));

		}
	}

	private void dialog() {
		AlertDialog.Builder builder = new Builder(AssistanceActivity.this);
		builder.setMessage("该操作将断开手机与电脑的连接，是否继续？");
		builder.setTitle("提示");

		builder.setPositiveButton(android.R.string.ok, new

		DialogInterface.OnClickListener() {

			@Override
			public void onClick(DialogInterface dialog, int which) {
				mScan.setBackgroundResource(R.drawable.disconect_normal);
				ConnectHelper.getInstance().setWifiStatus(false);
				NotificationManager nm = (NotificationManager) getSystemService(Context.NOTIFICATION_SERVICE);
				nm.cancelAll();
				// stopService();
				finish();
			}
		});

		builder.setNegativeButton(android.R.string.cancel, new

		DialogInterface.OnClickListener() {

			@Override
			public void onClick(DialogInterface dialog, int which) {

			}
		});
		builder.create().show();
	}

	protected void startScan() {
		if (isWiFiActive(AssistanceActivity.this)) {
			boolean isConnection = ConnectHelper.getInstance().isConnecting();
			if (!isConnection) {
				ConnectHelper.getInstance().setWifiStatus(true);
			}
			Intent i = new Intent();
			i.setClass(AssistanceActivity.this, CameraActivity.class);
			startActivityForResult(i, 1);
		} else {
			try {
				AssistanceActivity.this.startActivity(new Intent(
						Settings.ACTION_WIFI_SETTINGS));
			} catch (Exception e) {
				e.printStackTrace();
			}
		}

	}
	
	private void showConnetFail(){
		Toast.makeText(AssistanceActivity.this,
				"连接失败,请检查网络后重新尝试连接", 3000).show();
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		if (requestCode == 1) {
			if (resultCode == RESULT_OK) {
				Bundle bundle = data.getExtras();
				if (bundle.getBoolean("result")) {
					changeToScanInit(false);
					return;
				} else {
					showConnetFail();
				}
			}
			changeToScanInit(true);
		}
	}

	@Override
	protected void onResume() {
		super.onResume();
		// startService();
		String linkCode = LinkCodeMath.getLinkCode(getLocalIpAddress());
		if (linkCode != null && !linkCode.equals("")) {
			mLinkCode.setText(getString(R.string.qcode_1) + linkCode);
		} else {
			mLinkCode.setText(getString(R.string.qcode));
		}
		boolean isWifiConnecting = ConnectHelper.getInstance()
				.isWifiConnecting();
		boolean isConnection = ConnectHelper.getInstance()
						.isConnecting();
		if(!isConnection){
			ConnectHelper.getInstance().setWifiStatus(true);
		}
		changeToScanInit(!isWifiConnecting);
		if (sensorManager != null) {
			sensorManager.registerListener(sensorEventListener,
					sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER),
					SensorManager.SENSOR_DELAY_NORMAL);
		}
		
		//int status = ConnectHelper.getInstance().getMediaControlStatus();
	    //updateStatus(status);		
	}

	private String getLocalIpAddress() {
		if (mWifiManager.isWifiEnabled()) {
			try {
				WifiInfo wifiInfo = mWifiManager.getConnectionInfo();
				int ipAddress = wifiInfo.getIpAddress();
				if (ipAddress != 0) {
					String ip = intToIp(ipAddress);
					return ip;
				}
			} catch (Exception e) {

			}
		}

		return null;
	}

	private String intToIp(int i) {

		return (i & 0xFF) + "." + ((i >> 8) & 0xFF) + "." + ((i >> 16) & 0xFF)
				+ "." + (i >> 24 & 0xFF);
	}

	private void showWifiNotify() {
		NotificationManager nm = (NotificationManager) getSystemService

		(Context.NOTIFICATION_SERVICE);
		nm.cancelAll();
		Intent intent = new Intent(AssistanceActivity.this,

		AssistanceActivity.class);
		intent.setFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP);
		PendingIntent pi = PendingIntent.getActivity(AssistanceActivity.this,
				1,

				intent, 0);
		Notification notification = new Notification

		(R.drawable.icon_notification_wifi, getString(R.string.notify_text), 0);
		String title = getString(R.string.notify_title);
		String text = getString(R.string.notify_text);
		notification.setLatestEventInfo(AssistanceActivity.this, title, text,
				pi);
		nm.notify(1, notification);
	}
	
	private boolean isWiFiActive(Context inContext) {
		Context context = inContext.getApplicationContext();
		ConnectivityManager connectivity = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
		if (connectivity != null) {
			NetworkInfo[] info = connectivity.getAllNetworkInfo();
			if (info != null) {
				for (int i = 0; i < info.length; i++) {
					if (info[i].getTypeName().equals("WIFI") && info[i].isConnected()) {
						return true;
					}
				}
			}
		}

		return false;
	}
}