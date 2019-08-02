package daemon.provider.pim;

import static daemon.util.IConstants.ACTION_SEND_SMS_RESULT_LONGCONNECTION;

import java.util.ArrayList;
import java.util.List;

import daemon.common.Device;
import daemon.connection.ConnectionManager;
import daemon.model.pim20.SmsItem;
import daemon.model.sms.SmsInfo;
import daemon.protocol.ByteWriter;
import daemon.receiver.SendMessageType;

import android.annotation.SuppressLint;
import android.content.ContentResolver;
import android.content.Context;
import android.content.Intent;
import android.database.Cursor;
import android.net.Uri;
import android.os.Build;
import android.util.Log;

@SuppressLint("NewApi")
public class NewMessageHelper extends Thread {
	private static NewMessageHelper mInstance = null;
	private Context mContext = null;
	private int mTryTimes = 0;
	private Object mLock = new Object();
	private List<Long> mListSmsDate = new ArrayList<Long>();
	private final Uri URI_INBOX = Uri.parse("content://sms/inbox/");
	private final Uri URI_SENT = Uri.parse("content://sms/sent/");
	private final Uri URI_FAIL = Uri.parse("content://sms/failed/");
	private int mMaxSendSmsId;
	private int mMaxFailSmsId;
	private static final int SEND_RESULT_44_TIME_OUT = 30*1000;
	
	//发送短信时，保留下短信内容,4.4以上手机用来处理短信成功通知，4.4以下用来处理短信失败通知
	private ArrayList<SmsItem_v4> mSmsItemList = new ArrayList<SmsItem_v4>();
	//已向PC端发送失败通知的短信id、
	private ArrayList<Integer> mHasSendFailList = new ArrayList<Integer>();
	//主动插入数据库的接收短信中，如果是未读短信，则放入这个lsit
	private ArrayList<SmsItem> mNotReadItemList = new ArrayList<SmsItem>();

	private NewMessageHelper(Context context) {
		mContext = context;
	}

	public static synchronized NewMessageHelper getInstance(Context context) {
		if (mInstance == null) {
			mInstance = new NewMessageHelper(context);
			mInstance.start();
		}

		return mInstance;
	}
	
	
	public void addSendSmsInfo(SmsItem_v4 sms){
		synchronized (mSmsItemList) {
			mSmsItemList.add(sms);
		}
	}
	
	
	public void addNotReadSmsInfo(SmsItem sms){
		synchronized (mNotReadItemList) {
			mNotReadItemList.add(sms);
		}
	}
	
	
	public void addHasSendFailList(int id){
		synchronized (mHasSendFailList) {
			mHasSendFailList.add(id);
		}
	}
	
	public boolean hasSendFail(int id){
		synchronized (mHasSendFailList) {
             if(mHasSendFailList.contains(id)){
            	 mHasSendFailList.remove(Integer.valueOf(id));
            	 return true;
             }
             mHasSendFailList.add(id);
             return false;
		}
	}
	
	
	

	/*
	 * 通知有新短信 isBroadcast：是广播通知(true)还是数据库变化(false)
	 */
	public void notifyNewMessage(boolean isBroadcast) {
		synchronized (mLock) {
			try {
				// 接收到广播后，可能要较长时间后才会写入数据库，需尝试10次读取（lenovo K860手机，在锁屏情况下接收到新短信发出广播到短信写入数据库要8秒的时间）
				if (isBroadcast) {					
					mTryTimes = 10;
				}
				else if (mTryTimes <= 0) { // 数据库通知只记录一次，防止频繁查找。				
					mTryTimes = 1;
				}

				mLock.notify();
			}
			catch (Exception e) {
				e.printStackTrace();
			}
		}
	}
	
	private void sendSmsSendResult(int id ,int connectId,boolean success,int successId){
		Intent longConnectionIntent = new Intent(ACTION_SEND_SMS_RESULT_LONGCONNECTION);
		longConnectionIntent.putExtra("sms_id", id);
		longConnectionIntent.putExtra("success", success);
		longConnectionIntent.putExtra("connectionId", connectId);
		longConnectionIntent.putExtra("successId", successId);
		mContext.sendBroadcast(longConnectionIntent);
	}


	
	@Override
	public void run() {
		getCurrSmsInfos();

		
		while (true) {
			synchronized (mLock) {
				if (mTryTimes <= 0) {
					try {						

						// 如果没有通知，则每10秒检查一次数据库，即使数据库变化没通知，也能读到新短信
						mLock.wait(10000);	

					}
					catch (Exception e) {
						e.printStackTrace();
					}
				}
				
				--mTryTimes;
			}

			ContentResolver resolver = mContext.getContentResolver();
			String strSelection;
			Cursor cursor = null;		
			try {
				if(mSmsItemList.size()>0){
					if(Device.getIntSdkVersion()>=19){
						checkSendSuccess44();
					}else{
						//4.4以上手机，发送短信失败后，不会写入手机数据库
						checkSendFail();
					}
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
			
			strSelection = "read=0";
			boolean hasNewSms = false;			
			
			try {
				cursor = resolver.query(URI_INBOX, null, strSelection, null, "_id DESC");
				if (cursor != null) {
					while (cursor.moveToNext()) {
						int lngID = cursor.getInt(cursor.getColumnIndex("_id"));
						long lngDate = cursor.getLong(cursor.getColumnIndex("date"));
						if (!mListSmsDate.contains(lngDate)) {
							mListSmsDate.add(lngDate);
							String strContent = cursor.getString(cursor.getColumnIndex("body"));
							String strAddress = cursor.getString(cursor.getColumnIndex("address"));
							int count = mNotReadItemList.size();
							int index =-1;
							for (int i = 0; i < count; i++) {
								SmsItem item = mNotReadItemList.get(i);
								if(item.getBody().equals(strContent)&&item.getNumber().equals(strAddress)
									&&item.getDate()==lngDate){
									index = i;
									break;
								}
							}
							if(index!=-1){
								synchronized (mNotReadItemList) {
									mNotReadItemList.remove(index);
								}
								continue;
							}
							//
							
							SmsInfo info = new SmsInfo();
							info.setType(1);
							info.setId(lngID);
							info.setDataTime(lngDate);
							info.setContent(strContent);
							info.SetNumber(strAddress);
							long lngThreadID = cursor.getLong(cursor.getColumnIndex("thread_id"));
							info.setThreadId(lngThreadID);

							// 新接口
							ByteWriter writer = new ByteWriter();
							writer.Write(SendMessageType.SMS_ACTION_NEW_SMS);
							info.Write(writer);
							ConnectionManager.getInstance().sendMessage(SendMessageType.PIM_BUSINESS_ID, writer);

							// 旧接口
							ByteWriter writerOld = new ByteWriter();
							writerOld.Write(SendMessageType.SMS_ACTION_NEW_SMS_OLD);
							info.Write(writerOld);
							ConnectionManager.getInstance().sendMessage(SendMessageType.SMS_BUSINESS_ID_OLD, writerOld);

							hasNewSms = true;
						}
					}				
				

					// 已找到新短信的情况下，将mTryTimes次数置为1，只做再一次查询
					if (hasNewSms) {
						synchronized (mLock) {
							if (mTryTimes > 1) {
								mTryTimes = 1;
							}
						}					
					}
				}
			}
			catch (Exception e) {
				e.printStackTrace();
			}
			finally {
				if (cursor != null) {
					cursor.close();
				}
			}

			// 最多每500毫秒查询一次，防止导入短信时数据库改变通知太频繁，cpu占用太高
			// 时间太长的话，如果开着短信界面，新短信可能会很快被改成已读
			// 如果mTryTimes<=0则不休眠，进入下个循环wait
			if (mTryTimes > 0) {
				try {
					Thread.sleep(500);
				}
				catch (Exception e) {
					e.printStackTrace();
				}
			}
		}
	}

	private void checkSendFail() {
		ContentResolver resolver = mContext.getContentResolver();
		String strSelection;
		Cursor cursor = null;
		strSelection = "  _id > " + mMaxFailSmsId ;
		cursor = resolver.query(URI_FAIL, null, strSelection, null,null);
		if(cursor!=null){
			while (cursor.moveToNext()) {
				int id = cursor.getInt(cursor.getColumnIndex("_id"));
				if(id>mMaxFailSmsId){
					mMaxFailSmsId = id;
				}
				for (int i = 0; i < mSmsItemList.size(); i++) {
					SmsItem_v4 info =mSmsItemList.get(i);
					SmsItem item = info.getItem();
					int failId =item.getId();
					if(failId==id){
						sendSmsSendResult(id,info.getConnectId(),false,id);
						addHasSendFailList(failId);
                        break;
					}
				}
			}
		}
		if(cursor!=null){
			cursor.close();
		}
		
	}

	private void checkSendSuccess44() {
		ContentResolver resolver = mContext.getContentResolver();
		String strSelection;
		Cursor cursor = null;
		strSelection = " type = 2 and _id > " + mMaxSendSmsId ;
		ArrayList<SmsItem_v4> removedSmsList =new ArrayList<SmsItem_v4>();
		cursor = resolver.query(URI_SENT, null, strSelection, null,null);
		if(cursor!=null){
			while (cursor.moveToNext()) {
				int id = cursor.getInt(cursor.getColumnIndex("_id"));
				if(id>mMaxSendSmsId){
					mMaxSendSmsId = id;
				}
				mMaxSendSmsId = id;
				String body = cursor.getString(cursor.getColumnIndex("body"));
				String address = cursor.getString(cursor.getColumnIndex("address"));
				for (int i = 0; i < mSmsItemList.size(); i++) {
					SmsItem_v4 info =mSmsItemList.get(i);
					SmsItem item = info.getItem();
					if(item.getBody().equals(body)&&item.getNumber().equals(address)){
						sendSmsSendResult(item.getId(),info.getConnectId(),true,id);
						removedSmsList.add(info);
                        break;
					}
				}
			}
		}
		synchronized (mSmsItemList) {
			mSmsItemList.removeAll(removedSmsList);
		}
		removedSmsList.clear();
		for (int i = 0; i < mSmsItemList.size(); i++) {
			long time =System.currentTimeMillis() - mSmsItemList.get(i).getTime();
			if(time>=SEND_RESULT_44_TIME_OUT){
				sendSmsSendResult(mSmsItemList.get(i).getItem().getId(),mSmsItemList.get(i).getConnectId(),false,mSmsItemList.get(i).getItem().getId());
				removedSmsList.add(mSmsItemList.get(i));
			}
		}
		synchronized (mSmsItemList) {
			mSmsItemList.removeAll(removedSmsList);
		}
		if(cursor!=null){
			cursor.close();
		}
	}

	private void getCurrSmsInfos() {
		Cursor cursor = null;
		mMaxSendSmsId = 0;
		mMaxFailSmsId = 0;
		String strSelection = "read=0";
		try {
			ContentResolver resolver = mContext.getContentResolver();
			cursor = resolver.query(URI_INBOX, null, strSelection, null, null);
			if (cursor != null) {
				while (cursor.moveToNext()) {
					long lngDate = cursor.getLong(cursor.getColumnIndex("date"));
					mListSmsDate.add(lngDate);
				}
			}
			if(Device.getIntSdkVersion() >=19){
				cursor = resolver.query(URI_SENT, new String[]{"_id"} , "type = 2", null, "_id DESC");
				if (cursor != null) {
					if(cursor.moveToFirst()){
						mMaxSendSmsId = cursor.getInt(cursor.getColumnIndex("_id"));
					}
				}	
			}
			cursor = resolver.query(URI_FAIL, new String[]{"_id"} , null, null, "_id DESC");
			if (cursor != null) {
				if(cursor.moveToFirst()){
					mMaxFailSmsId = cursor.getInt(cursor.getColumnIndex("_id"));
				}
			}	
			
			
		}
		catch (Exception e) {
			e.printStackTrace();
		}
		finally {
			if (cursor != null) {
				cursor.close();
			}
		}
	}
}
