package daemon.net.nio;

import java.nio.ByteBuffer;
import java.nio.channels.NotYetConnectedException;
import java.nio.channels.SocketChannel;
import android.content.Context;

import daemon.logger.LogCenter;
import daemon.net.base.Peer;
import daemon.net.base.PeerEvent;
import daemon.net.base.SocketType;

//Nio socket连接的基类
public class NioPeer extends Peer {
	private final String TAG = getClass().getName();

	private SelectReactor mReactor = null;
	private SocketChannel mChannel = null;

	private Object mLockReadable = new Object();
	private Object mLockWritable = new Object();

	private boolean mIsProcessing = false;
	
	private final int WAIT_TIME	= 1000;//读写返回0时，一次等待1s

	public NioPeer(Context context, SocketChannel channel, SelectReactor reactor) {
		super(context);

		mType = SocketType.NIO;
		mReactor = reactor;
		mChannel = channel;
		mIpAddress = mChannel.socket().getInetAddress().getHostAddress();
		mPort = mChannel.socket().getPort();
	}

	// 关闭当前Nio socket连接
	@Override
	public boolean close() {
		try {
			if (!mIsClosed) {
				mIsClosed = true;
				fireEvent(PeerEvent.CLOSED);
				mReactor.closeChannel(mChannel);
			}
		}
		catch (Exception e) {
			e.printStackTrace();
		}

		return true;
	}

	@Override
	public synchronized boolean write(ByteBuffer bb, int timeout) {
		if (mIsClosed)
			return false;

		try {
			int waitTime = 0;

			while (bb.hasRemaining()) {
				int writeCnt = mChannel.write(bb);

				if (writeCnt > 0) {
					waitTime = 0;
				}
				else if (writeCnt == 0) {
					if (waitTime > timeout) {
						LogCenter.error(TAG, "write timeout:" + timeout);
						close();
						return false;
					}
								
					try {						
						synchronized (mLockWritable) {
							mReactor.registerWriteOpt(mChannel);
							mLockWritable.wait(WAIT_TIME);
							waitTime += WAIT_TIME;
						}
					}
					catch (Exception e) {
						LogCenter.error(TAG, "write wait", e);
					}
				}
				else if (writeCnt < 0) {
					LogCenter.error(TAG, "write failed");
					close();
					return false;
				}
			}
			
			return true;
		}
		catch (Exception e) {
			LogCenter.error(TAG, null, e);
			close();
		}

		return false;
	}

	@Override
	public int readEx(ByteBuffer bb, int timeout) {
		if (mIsClosed) {
			return 0;
		}

		try {
			int waitTime = 0;
			int allCnt = 0;
			while (bb.hasRemaining()) {
				int readCnt = mChannel.read(bb);

				if (readCnt > 0) {
					allCnt+=readCnt;
					waitTime = 0;
				}
				else if (readCnt == 0) {
					if (waitTime > timeout) {
						LogCenter.error(TAG, "read timeout:" + timeout);
						close();
						return -1;
					}
									
					try {
						synchronized (mLockReadable) {	
							mLockReadable.wait(timeout);
							waitTime += WAIT_TIME;
						}
					}
					catch (Exception e) {
						LogCenter.error(TAG, "read wait", e);
					}					
				}
				else if (readCnt < 0) {					
					close();
					LogCenter.error(TAG, "client socket has closed, readCnt: " + readCnt + ", " + this);

					return -1;
				}
			}
			
			return allCnt;
		}
		catch (Exception e) {
			close();
			LogCenter.error(TAG, null, e);
		}

		return -1;
	}
	
	
	@Override
	protected void beginHandle() {

	}

	@Override
	protected boolean endHandle() {		
		mIsProcessing = false;
	
		return true;
	}

	public boolean isProcessing() {
		return mIsProcessing;
	}

	public void setProcessing(boolean isProcessing) {
		mIsProcessing = isProcessing;
	}

	public void notifyReadable() {		
		synchronized (mLockReadable) {
			try {
				mLockReadable.notify();
			}
			catch (Exception e) {
				LogCenter.error(TAG, null, e);
			}
		}
	}

	public void notifyWritable() {		
		synchronized (mLockWritable) {
			try {
				mLockWritable.notify();
			}
			catch (Exception e) {
				LogCenter.error(TAG, null, e);
			}
		}
	}


}
