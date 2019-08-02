package daemon.connection;

import java.util.ArrayList;
import java.util.List;
import java.nio.ByteBuffer;


import daemon.logger.LogCenter;
import daemon.net.base.Peer;
import daemon.protocol.ByteWriter;
import daemon.util.IConstants;

//负责向PC端发送心跳包
class HeartbeatService {

	private final String TAG = getClass().getName();
	private ByteBuffer mHeartbeatPackage;
	private Thread mHeartbeatThread = null;

	// private final InetSocketAddress dummy_sock_addr = new InetSocketAddress("220.181.6.18", 44445);

	private HeartbeatService() {
		mHeartbeatPackage = ByteBuffer.allocate(IConstants.HEARTBEAT_DATA.length);
		mHeartbeatPackage.put(IConstants.HEARTBEAT_DATA);
	}

	// 启动心跳包服务
	public void start() {
		if (null != mHeartbeatThread) {
			return;
		}

		mTerminated = false;
		mHeartbeatThread = new Thread(new HeartbeatTask());
		mHeartbeatThread.start();
	}

	// 停止心跳包服务
	public void stop() {
		LogCenter.error(TAG, "stop heartbeat server.",null,true);
		mTerminated = true;
		if (null == mHeartbeatThread) {
			return;
		}

		
		mHeartbeatThread = null;
	}

	// 当前服务是否正在运行
	public boolean isStarted() {
		return !mTerminated;
	}

	// 向PC端发送心跳包
	public void doHeartbeat(Peer peer) {	
		ByteBuffer heartPack = ByteBuffer.allocate(IConstants.HEARTBEAT_DATA.length);
		heartPack.put(IConstants.HEARTBEAT_DATA);
		heartPack.rewind();
		peer.write(heartPack, 10000);
	}

	// 心跳包线程
	private class HeartbeatTask implements Runnable {
		public void run() {
			LogCenter.error(TAG, "start heartbeat server.",null,true);
			long time =System.currentTimeMillis();
			while (!mTerminated) {
				try {
					Thread.sleep(1000);
				}
				catch (InterruptedException e) {
					LogCenter.error(TAG, null, e);
				}
				
				if(System.currentTimeMillis()-time >= 1*60*1000){
					time = System.currentTimeMillis();
					LogCenter.error(TAG, "HeartbeatTask alive",null);
				}

				List<ConnectionInfo> lstConn = ConnectionManager.getInstance().getConnectionList();
				List<ConnectionInfo> lstConnTmp = new ArrayList<ConnectionInfo>();
				lstConnTmp.addAll(lstConn);
				
				for (ConnectionInfo conn : lstConnTmp) {		
					mHeartbeatPackage.rewind();
					conn.getPeer().write(mHeartbeatPackage, 10000);
				}
				List<SendMessageInfo> lstSendMessageTmp = new ArrayList<SendMessageInfo>();
				
				synchronized (mLock) {
					for (SendMessageInfo info :mSendMessageInfoList) {
						SendMessageInfo copyInfo =new SendMessageInfo();
						copyInfo.connectID =info.connectID;
						copyInfo.businessId =info.businessId;
						copyInfo.writer =info.writer;
						lstSendMessageTmp.add(copyInfo);
					}
					mSendMessageInfoList.clear();
				}
				
                for (SendMessageInfo info :lstSendMessageTmp) {
            		byte[] data = info.writer.getBaseStream().ToArray();

            		info.writer = new ByteWriter();
            		info.writer.Write(IConstants.TOKEN);
            		info.writer.Write(IConstants.PACKAGEFLAG_Server);
            		info.writer.Write(data.length + 4);
            		info.writer.Write(info.businessId);
            		info.writer.Write(data);
    				for (ConnectionInfo conn : lstConnTmp) {		
                        if(conn.getConnectionId()==info.connectID){
                    		info.writer.getBaseStream().setOffset(0);
                    		if(!conn.getPeer().write(info.writer.getBaseStream().GetBuffer(), 3000)){
                    			LogCenter.error(TAG, "sendMessage, send failed, businessId:" +info. businessId);
                    		}
                        }
    				}
				}               
                
			}
		}
	};

	private volatile boolean mTerminated = true;
	
	
	private static HeartbeatService mHeartbeatService;
	public static synchronized HeartbeatService getInstance(){
		if(mHeartbeatService==null){
			mHeartbeatService = new HeartbeatService();
		}
		return mHeartbeatService;
	}
	
	
	public void sendMessage(int connectID,int businessId,ByteWriter writer){
		SendMessageInfo info =new SendMessageInfo();
		info.connectID =connectID;
		info.businessId =businessId;
		info.writer =writer;
		synchronized (mLock) {
			mSendMessageInfoList.add(info);
		}
		
	}
	private Object mLock = new Object();
	private ArrayList<SendMessageInfo> mSendMessageInfoList =new ArrayList<SendMessageInfo>();
	
	public class SendMessageInfo{
		public int connectID;
		public int businessId;
		public ByteWriter writer;
		
	}
}
