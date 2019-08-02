package daemon.connection;


import daemon.logger.LogCenter;
import daemon.net.base.Peer;
import daemon.protocol.ByteWriter;
import daemon.util.IConstants;

//对长连接的封装
public class ConnectionInfo {
	private final String TAG = getClass().getName();

	private Peer mPeer = null;
	private String mPcName = "";
	private int mediaStatus = -1;
	private boolean isPlayMediaIng;

	public ConnectionInfo(Peer peer, String pcName) {
		mPeer = peer;
		mPcName = pcName;
		isPlayMediaIng = false;
	}

	/**
	 * 发送即时消息到客户端
	 * @param businessId 相关业务id号
	 * @param writer 发送数据
	 */
	public void sendMessage(int businessId, ByteWriter writer) {	
		if (null == mPeer) {
			LogCenter.error(TAG, "sendMessage, peer is null.");
			return;
		}

		byte[] data = writer.getBaseStream().ToArray();

		writer = new ByteWriter();
		writer.Write(IConstants.TOKEN);
		writer.Write(IConstants.PACKAGEFLAG_Server);
		writer.Write(data.length + 4);
		writer.Write(businessId);
		writer.Write(data);
		
		writer.getBaseStream().setOffset(0);
		if(!mPeer.write(writer.getBaseStream().GetBuffer(), 3000)){
			LogCenter.error(TAG, "sendMessage, send failed, businessId:" + businessId);
		}
	}
	
	/**
	 * 获取当前连接
	 * @return
	 */
	public Peer getPeer(){
		return mPeer;
	}

	/**
	 * 获取客户端名称
	 * @return
	 */
	public String getPcName(){
		return mPcName;
	}

	/**
	 * 获取连接id号
	 * @return
	 */
	public int getConnectionId(){
		return mPeer.getConnectionId();
	}
	
	/**
	 * 获取客户端ip地址
	 * @return
	 */
	public String getIpAddress(){
		return mPeer.getIPAddress();
	}
	
	/**
	 * 获取客户端端口号
	 * @return
	 */
	public int getPort(){
		return mPeer.getPort();
	}
	
	/**
	 * 是否wifi连接
	 * @return
	 */
	public boolean isWifi(){
		String ip = getIpAddress();
		return !ip.equals("127.0.0.1") && !ip.equals("0.0.0.0");
	}
	
	public int getPcMediaStatus(){
		return mediaStatus;
	}
	
	public void setPcMediaStatus(int value){
		mediaStatus = value;
	}
	
	public boolean isPlayMediaStatus(){
		return isPlayMediaIng;
	}
	
	public void setsPlayMediaStatus(boolean value){
		isPlayMediaIng = value;
	}
}
