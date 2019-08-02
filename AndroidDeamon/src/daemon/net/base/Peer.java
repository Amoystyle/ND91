package daemon.net.base;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;

import android.content.Context;

import daemon.logger.LogCenter;
import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;
import daemon.protocol.MemoryStream;
import daemon.util.IConstants;

public abstract class Peer implements Runnable {

	private final String TAG = getClass().getName();
	protected Context mContext = null;
	protected int mPort;
	protected String mIpAddress;
	protected int mConnectId;
	protected SocketType mType = SocketType.NIO;
	protected boolean mIsClosed = false;

	private Object mObserverLock = new Object();
	private List<IPeerEventObserver> mEventObservers = new ArrayList<IPeerEventObserver>();

	public Peer(Context context) {
		mContext = context;
	}

	@Override
	public void run() {
		while (true) {

			beginHandle();

			PackageHead head = readPackageHead();

			if (head == null) {
				//LogCenter.error(TAG, "read head failed!");
				close();
				return;
			}

			if ((head.getFlag() & IConstants.PACKAGEFLAG_CloseCurrentConnection) == IConstants.PACKAGEFLAG_CloseCurrentConnection) {
				close();
				return;
			}

			PackageHandler handler = PackageHandler.createHandler(mContext, head, this);
			boolean result = handler.handle();

			if (result && handler.isLongConnection()) {
				return;
			}

			boolean exitLoop = endHandle();
			if (exitLoop)
				break;
		}
		
	}

	public void setEventObserver(IPeerEventObserver observer) {
		synchronized (mObserverLock) {
			mEventObservers.add(observer);
		}
	}

	protected void fireEvent(PeerEvent event) {
		synchronized (mObserverLock) {
			for (IPeerEventObserver observer : mEventObservers) {
				observer.onEvent(event, this);
			}
		}
	}

	/**
	 * 关闭当前Nio socket连接
	 * 
	 * @return
	 */
	public abstract boolean close();

	/**
	 * 往当前socket连接写入数据
	 * 
	 * @param bb
	 *            缓冲数据，从当前position到limit
	 * @param timeout
	 *            超时时间，为0则不处理超时
	 * @return
	 */
	public abstract boolean write(ByteBuffer bb, int timeout);

	/**
	 * 往当前socket连接写入数据
	 * 
	 * @param writer
	 *            将重新计算数据长度
	 * @param timeout
	 *            超时时间，为0则不处理超时
	 * @return
	 */
	public boolean write(ByteWriter writer, int timeout) {
		if (writer != null) {
			int datasize = writer.getBaseStream().getLength() - IConstants.HEADER_LENGTH + 4;
			writer.getBaseStream().setOffset(IConstants.TOKEN.length + 4);
			writer.Write(datasize);
			writer.getBaseStream().setOffset(0);

			ByteBuffer resultData = writer.getBaseStream().GetBuffer();
			return write(resultData, timeout);
		}

		return false;
	}

	/**
	 * 从socket中读取指定长度数据
	 * 
	 * @param bb
	 *            数据缓冲区，不会重定位position
	 * @param len
	 *            要读取的长度
	 * @param timeout
	 *            指定超时时间，为0则不处理超时
	 * @return 是否读取成功
	 */
	public  boolean read(ByteBuffer bb, int timeout){
		if(readEx(bb,timeout)<0){
			return false;
		}
		return true;
	}
	
	public abstract int readEx(ByteBuffer bb, int timeout);

	/**
	 * 读取协议包头
	 * 
	 * @return 如果读取异常，或者协议不正确，则返回null
	 */
	private PackageHead readPackageHead() {
		ByteBuffer headBuf = ByteBuffer.allocate(IConstants.HEADER_LENGTH);
		if (read(headBuf, 10000)) {
			byte[] headData = headBuf.array();
			//ByteReader reader = new ByteReader(new MemoryStream(headData));
			//LogCenter.error(TAG, "head: " + new String(reader.ReadBytes(4)) + ", flag: " + reader.ReadInt16() + ", longConn: " + reader.ReadInt16() + ", length: " + reader.ReadInt32() + ", businessID: " + reader.ReadInt32());
			if (headData[0] == IConstants.TOKEN[0] && headData[1] == IConstants.TOKEN[1] && headData[2] == IConstants.TOKEN[2] && headData[3] == IConstants.TOKEN[3]) {
				int flag = ByteReader.ReadInt16(headData, IConstants.TOKEN.length);
				mConnectId = ByteReader.ReadInt16(headData, IConstants.TOKEN.length + 2);
				int dataLen = ByteReader.ReadInt32(headData, IConstants.TOKEN.length + 4) - 4;
				int businessId = ByteReader.ReadInt32(headData, IConstants.TOKEN.length + 8);

				PackageHead head = new PackageHead(mConnectId, flag, businessId, dataLen);

				return head;
			}
			else{				
				LogCenter.error(TAG, "protocal head err:" + new String(headData));
			}
		}

		return null;
	}

	/**
	 * 开始处理业务
	 */
	protected abstract void beginHandle();

	/**
	 * 业务处理结束
	 * 
	 * @return 返回是否退出处理线程
	 */
	protected abstract boolean endHandle();

	/**
	 * 获取客户端ip地址
	 * 
	 * @return
	 */
	public String getIPAddress() {
		return mIpAddress;
	}

	/**
	 * 获取客户端端口号
	 * 
	 * @return
	 */
	public int getPort() {
		return mPort;
	}

	/**
	 * 获取连接id号
	 * 
	 * @return
	 */
	public int getConnectionId() {
		return mConnectId;
	}
	
	/**
	 * 是否可用
	 * @return
	 */
	public boolean isValid(){		
		return !mIsClosed;
	}
	
	public Context getContext(){
		return mContext;
	}
}
