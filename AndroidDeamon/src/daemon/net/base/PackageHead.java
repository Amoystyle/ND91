package daemon.net.base;

/**
 * 协议包头
 * @author nd
 *
 */
public class PackageHead {
	
	private int mConnectionId = 0;
	private int mFlag = 0;
	private int mBusinessId = 0;
	private int mDataLength = 0;
	
	public PackageHead(int connectionId, int flag, int businessId, int dataLength){
		mConnectionId = connectionId;
		mFlag = flag;
		mBusinessId = businessId;
		mDataLength = dataLength;
	}
	
	/**
	 * 获取长连接id
	 * @return
	 */
	public int getConnectionId(){
		return mConnectionId;
	}
	
	/**
	 * 获取业务标志
	 * @return
	 */
	public int getFlag(){
		return mFlag;
	}
	
	/**
	 * 获取业务id
	 * @return
	 */
	public int getBusinessId(){
		return mBusinessId;
	}
	
	/**
	 * 获取数据长度
	 * @return
	 */
	public int getDataLength(){
		return mDataLength;
	}
}
