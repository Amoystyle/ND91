package daemon.net.original;

import java.io.BufferedOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.nio.ByteBuffer;

import android.content.Context;


import daemon.logger.LogCenter;
import daemon.net.base.Peer;
import daemon.net.base.PeerEvent;
import daemon.net.base.SocketType;
import daemon.util.IConstants;

public class OriginalPeer extends Peer implements Runnable {

	protected Socket mSocket;
	protected BufferedOutputStream mBufferOut;

	private Object mWriteLocker = new Object();
	private final String TAG = getClass().getName();	

	public OriginalPeer(Context context, Socket socket) {
		super(context);
		
		 mType = SocketType.ORIGINAL;
		mSocket = socket;		
		mIpAddress = mSocket.getInetAddress().getHostAddress();
		mPort = mSocket.getPort();
	}
	
	@Override
	public boolean close() {		
		try {
			if (mBufferOut != null)
				mBufferOut.close();
		}
		catch (Exception e) {
			e.printStackTrace();
		}

		try {
			if (mSocket != null)
				mSocket.close();
		}
		catch (Exception e) {
			e.printStackTrace();
		}

		fireEvent(PeerEvent.CLOSED);
		mIsClosed = true;

		return true;
	}
	
	@Override
	public boolean write(ByteBuffer bb, int timeout) {

		synchronized (mWriteLocker) {
			try {
				int len = bb.remaining();
				byte[] data = new byte[len];
				bb.get(data);
				OutputStream os = mSocket.getOutputStream();
				int sendCnt = 0;
				
				long start = System.currentTimeMillis();
				long duration = 0;
				
				while (sendCnt < len) {
					int bufSize = Math.min(IConstants.SOCKET_BUFFER_SIZE, len - sendCnt);
					os.write(data, sendCnt, bufSize);
					sendCnt += bufSize;
					
					if (timeout > 0) {
						duration = System.currentTimeMillis() - start;
						if (duration > timeout) {
							LogCenter.error(TAG, "write timeout:" + timeout);
							close();
							return false;
						}
					}
				}
				
				return true;
			}			
			catch (Exception e) {		
				close();
				LogCenter.error(TAG, null, e);
			}
		}
		
		close();
		return false;
	}


	@Override
	protected void beginHandle() {
		// TODO Auto-generated method stub

	}

	@Override
	protected boolean endHandle() {
		return mIsClosed;
	}

	@Override
	public int readEx(ByteBuffer bb, int timeout) {
		if (!mSocket.isConnected()) {
			close();
			LogCenter.debug(TAG, "Can't receive from unconnected socket");
			return -1;
		}

		try {
			int len = bb.remaining();
			int bytesRead = 0;
			InputStream in = mSocket.getInputStream();
			byte[] data = new byte[len];
			

			while (bytesRead < len) {
				
				int onceRead = in.read(data, bytesRead, len - bytesRead);
				if(onceRead < 0){
					close();
					return -1;
				}
				
				bytesRead += onceRead;
			}

			bb.put(data);
			return bytesRead;
		}
		catch (Exception e) {
			close();
			e.printStackTrace();
		}

		return -1;
	}
}
