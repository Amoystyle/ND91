package daemon.server;

import static daemon.util.IConstants.ACTION_SEND_SMS_RESULT_LONGCONNECTION;

import java.util.ArrayList;
import java.util.concurrent.TimeUnit;

import android.content.BroadcastReceiver;
import android.content.ContentResolver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.database.ContentObserver;
import android.net.Uri;
import android.os.Handler;
import android.os.HandlerThread;
import android.provider.Contacts.People;
import android.telephony.PhoneStateListener;
import android.telephony.TelephonyManager;


import daemon.common.Device;
import daemon.connection.ConnectionManager;
import daemon.logger.LogCenter;
import daemon.ndk.NdkManager;
import daemon.net.base.ServerBase;
import daemon.net.nio.NioServerImp;
import daemon.net.original.OriginalServerImp;
import daemon.protocol.ByteWriter;
import daemon.provider.pim.Contact20Helper;
import daemon.provider.pim.NewMessageHelper;
import daemon.provider.pim.SmsProvider_V2;
import daemon.receiver.DaemonPhoneStateListener;
import daemon.receiver.SendMessageType;
import daemon.util.ContentObserverListerHelper;
import daemon.util.IConstants;

public class DaemonServerImp {


	private String TAG = getClass().getName();
	private boolean mIsNio = true;
	// private FTPServer mFtpServer;
	private Context mContext;
	private ServerBase mServerBase;
	private int mTcpPort;
	private DaemonPhoneStateListener mPhoneStateListener;
	private SmsReceiver mSmsReceiver;
	private SendSmsResultReceiver mSendStatusReceiver;

	public DaemonServerImp(Context context, int port) {
		LogCenter.error(TAG, "new DaemonServerImp(" + context.toString() + ", " + port + ")");
	
		mContext = context;
		mTcpPort = port;

		if (android.os.Build.MODEL.equals("U8812D")) {
			//华为机子，使用NIO 文件传输失败，特殊处理
			mIsNio = false;
		}

		init();
		CreateServer(mIsNio);
	}

	private void CreateServer(boolean isNio) {
		if (isNio) {
			mServerBase = new NioServerImp(mContext, mTcpPort);
		}
		else {
			mServerBase = new OriginalServerImp(mContext, mTcpPort);
		}
	}

	// 初始化
	public boolean init() {
		LogCenter.error(TAG, "init()");

		//registReceiver
		try {
			NewMessageHelper.getInstance(mContext);//初始化短信接收线程
			registReceiver();
		}
		catch (Exception e) {
			e.printStackTrace();
			String strMsg = e.getMessage();
			LogCenter.debug(TAG, "exception msg: " + strMsg);
		}

		return true;
	}

	// 启动服务
	public void start() {
		try {
			if (!mServerBase.startServer()) {
				if (mIsNio) {
					LogCenter.debug("DaemonServer", "Switch to Socket.");
					mIsNio = false;
					CreateServer(mIsNio);
					mServerBase.startServer();
				}
			}
		}
		catch (Exception ex) {
			ex.printStackTrace();
		}
	}

	// 停止服务
	public void stop() {
		// unRegistReceiver
		try {
			unRegistReceiver();
			NdkManager.quit(mContext);
		}
		catch (Exception e) {
			LogCenter.debug(TAG, "*unRegistReceiver Exception.");
			e.printStackTrace();
		}

		mServerBase.stopServer();
	}

	/**
	 * 注册广播Receiver
	 */
	private void registReceiver() {
		mPhoneStateListener = DaemonPhoneStateListener.getInstance();
		TelephonyManager telephonyManager = (TelephonyManager) mContext.getSystemService(Context.TELEPHONY_SERVICE);
		telephonyManager.listen(mPhoneStateListener, PhoneStateListener.LISTEN_CALL_STATE);

		IntentFilter filter;	
	
		//数据库监听新短信
		HandlerThread thread = createAliveHandlerThread();
		if (thread != null) {
			ContentResolver resolver = mContext.getContentResolver();
			Uri uriSms = Uri.parse("content://sms/");
			// 传入创建并已启用的线程Looper,这是因为ContentObserver的onChange()方法是
			// 运行在Handler的Looper所在线程当中，如果不传就跑在主线程
			Handler handler = new Handler(thread.getLooper());
			SmsContentObserver observer = new SmsContentObserver(handler, mContext);
			resolver.registerContentObserver (uriSms, true, observer);
			
			if(Device.getIntSdkVersion() >= 5){
				uriSms = Contact20Helper.url;
			}else{
				uriSms = People.CONTENT_URI;
			}
			
			Handler conactHandler = new Handler(thread.getLooper());
			ConactContentObserver conactObserver = new ConactContentObserver(conactHandler, mContext);
			resolver.registerContentObserver(uriSms, true, conactObserver);
			
			uriSms = android.provider.CallLog.Calls.CONTENT_URI;
			Handler callHandler = new Handler(thread.getLooper());
			CallContentObserver callobserver = new CallContentObserver(callHandler, mContext);
			resolver.registerContentObserver(uriSms, true, callobserver);
			
			uriSms = Uri.parse("content://mms-sms/conversations");
			Handler threadsHandler = new Handler(thread.getLooper());
			ThreadsContentObserver threadsobserver = new ThreadsContentObserver(threadsHandler, mContext);
			resolver.registerContentObserver(uriSms, true, threadsobserver);
			
			

		}
	
		//广播监听新短信
		LogCenter.error(TAG, "register SmsReceiver");
		mSmsReceiver = new SmsReceiver();
		filter = new IntentFilter(IConstants.ACTION_SMS_RECEIVED);
		filter.setPriority(0x7fffffff);
		mContext.registerReceiver(mSmsReceiver, filter);
		
		
		//监听短信发送结果
		mSendStatusReceiver = new SendSmsResultReceiver();
		filter = new IntentFilter(ACTION_SEND_SMS_RESULT_LONGCONNECTION);
		mContext.registerReceiver(mSendStatusReceiver, filter);
	}

	/**
	 * 根据手机的IMEI判断出机型， 如果机型在给定列表中，表示该机型短信广播无法正常工作需要使用监听短信数据库的形式，返回true; 如果机型不在给定列表中，表示该机型短信广播可正常使用，返回false
	 * 
	 * @param strIMEI
	 *            手机的IMEI
	 * @return true或者false，true表示要使用监听短信数据库形式；false表示使用短信广播形式
	 */
	/*
	 * private boolean useDBListener(String strIMEI) { boolean blnReturn = true; return blnReturn; }
	 */

	/**
	 * 创建并返回已启动的HandlerThread。在方法中等待线程启动，如果等待时间超过5秒则立即返回null
	 * 
	 * @return
	 */
	private HandlerThread createAliveHandlerThread() {
		HandlerThread threadReturn = new HandlerThread("HandlerThread");
		threadReturn.start();
		int count = 5 * 1000 / 10; // 5秒线程不启动就放弃
		int i = 0;
		while (!threadReturn.isAlive() && i++ < count) {
			try {
				TimeUnit.MICROSECONDS.sleep(10);
			}
			catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		if (i == count) {
			threadReturn = null;
		}
		return threadReturn;
	}

	/**
	 * 取消注册和界面广播Receiver
	 */
	private void unRegistReceiver() {
		if (mSmsReceiver != null) {
			mContext.unregisterReceiver(mSmsReceiver);
			mSmsReceiver = null;
		}

		if (mSendStatusReceiver != null) {
			mContext.unregisterReceiver(mSendStatusReceiver);
			mSendStatusReceiver = null;
		}
	}	

	class SmsReceiver extends BroadcastReceiver {
		@Override
		public void onReceive(final Context ctx, final Intent intent) {		
			String action = intent.getAction();

			if (IConstants.ACTION_SMS_RECEIVED.equals(action)) {
				NewMessageHelper.getInstance(ctx).notifyNewMessage(true);
			}
		}	
	};

	class SendSmsResultReceiver extends BroadcastReceiver {	
		
		@Override
		public void onReceive(Context ctx, Intent intent) {
			String action = intent.getAction();

			if (action.equals(ACTION_SEND_SMS_RESULT_LONGCONNECTION)) {

				int id = intent.getIntExtra("sms_id", 0);
				LogCenter.error("sms_id", "" + id);
				int successId = (int) intent.getIntExtra("successId", -1);
				if (id > 0 || successId>0) {
					int connectionId = (int) intent.getIntExtra("connectionId", -1);
					boolean sendSuccess = intent.getBooleanExtra("success", false);

					//新接口
					ByteWriter writer = new ByteWriter();
					if(successId != -1){
						writer.Write(SendMessageType.SMS_ACTION_SEND_SMS_REUSLT_4_4);
					}else{
						writer.Write(SendMessageType.SMS_ACTION_SEND_RESULT);
					}
					writer.Write(id);
					writer.Write(sendSuccess);
					if(successId != -1){
						writer.Write(successId);
					}
					
					//旧接口
					ByteWriter writerOld = new ByteWriter();
					writerOld.Write(SendMessageType.SMS_ACTION_SEND_RESULT_OLD);
					writerOld.Write(id);
					writerOld.Write(sendSuccess);
					if(successId != -1){
						writer.Write(successId);
					}

					if (connectionId != -1) {
						ConnectionManager.getInstance().sendMessage(SendMessageType.PIM_BUSINESS_ID, connectionId,  writer);
						ConnectionManager.getInstance().sendMessage(SendMessageType.SMS_BUSINESS_ID_OLD, connectionId,  writerOld);
					}
					else {
						ConnectionManager.getInstance().sendMessage(SendMessageType.PIM_BUSINESS_ID, writer);
						ConnectionManager.getInstance().sendMessage(SendMessageType.SMS_BUSINESS_ID_OLD, writerOld);
					}

					int type = sendSuccess ? 2 : 5;

					SmsProvider_V2.SetSmsType(ctx, id, type);
				}
			}
		}
	};

	class SmsContentObserver extends ContentObserver {
		private Context mContext;

		// private SmsFilter mFilter;

		public SmsContentObserver(Handler handler, Context context) {
			super(handler);
			this.mContext = context;
			ContentObserverListerHelper.setSmsLastTime(System.currentTimeMillis());
			// mFilter = SmsFilter.getInstance();
		}

		@Override
		public void onChange(boolean selfChange) {
			super.onChange(selfChange);
			ContentObserverListerHelper.setSmsLastTime(System.currentTimeMillis());
			NewMessageHelper.getInstance(mContext).notifyNewMessage(false);
		}
	}

	
	class CallContentObserver extends ContentObserver {

		public CallContentObserver(Handler handler, Context context) {
			super(handler);
			ContentObserverListerHelper.setCallLastTime(System.currentTimeMillis());
		}

		@Override
		public void onChange(boolean selfChange) {
			super.onChange(selfChange);
			ContentObserverListerHelper.setCallLastTime(System.currentTimeMillis());

		}
	}
	
	
	class ThreadsContentObserver extends ContentObserver {

		public ThreadsContentObserver(Handler handler, Context context) {
			super(handler);
			ContentObserverListerHelper.setCallLastTime(System.currentTimeMillis());
		}

		@Override
		public void onChange(boolean selfChange) {
			super.onChange(selfChange);
			ContentObserverListerHelper.setSmsLastTime(System.currentTimeMillis());

		}
	}
	
	class ConactContentObserver extends ContentObserver {

		public ConactContentObserver(Handler handler, Context context) {
			super(handler);
			ContentObserverListerHelper.setConactLastTime(System.currentTimeMillis());
		}

		@Override
		public void onChange(boolean selfChange) {
			super.onChange(selfChange);
			ContentObserverListerHelper.setConactLastTime(System.currentTimeMillis());

		}
	}
	/*
	 * //启动文件ftp服务 public boolean runFtpServer() { return mFtpServer.run_server(); }
	 * 
	 * //停止文件ftp服务 public boolean stopFtpServer() { return mFtpServer.stop_server(); }
	 */
}
