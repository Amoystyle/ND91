package daemon.net.nio;

import java.net.InetSocketAddress;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.nio.channels.spi.SelectorProvider;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import android.content.Context;

import daemon.connection.ConnectionManager;
import daemon.logger.LogCenter;
import daemon.net.base.DispatcherThreadPool;
import daemon.util.IConstants;

public class SelectReactor implements Runnable {
	private String TAG = getClass().getName();
	private DispatcherThreadPool mThreadPool;
	private Selector mSelector;
	private boolean mExit;
	private Context mContext = null;
	private List<SocketChannel> mListNeedClosedChannel = new ArrayList<SocketChannel>();
	private List<SocketChannel> mRegisterWriteOptChannels = new ArrayList<SocketChannel>();

	public SelectReactor(Context context) {
		mThreadPool = DispatcherThreadPool.getInstance();
		mContext = context;
	}

	// 创建选择器[Selector]
	public boolean open(int port) {
		try {
			mSelector = SelectorProvider.provider().openSelector();

			ServerSocketChannel serverChannel = ServerSocketChannel.open();
			serverChannel.socket().setReuseAddress(true);
			serverChannel.configureBlocking(false);
			serverChannel.socket().bind(new InetSocketAddress(port));
			serverChannel.register(mSelector, SelectionKey.OP_ACCEPT);

			return true;
		}
		catch (Exception e) {
			LogCenter.error(TAG, "start nio socket server fail.  " +e.toString(),null,true);
		}

		return false;
	}

	// 关闭选择器[Selector]
	public void close() {
		if (null == mSelector) {
			return;
		}

		mExit = true;
		mSelector.wakeup();
	}

	// 轮询选择器中的连接，并触发相应的时间
	@Override
	public void run() {
		LogCenter.debug(TAG, "SelectReactor run()",null,true);
		long time =System.currentTimeMillis();
		try {
			while (true) {				
				registerWriteOptInternal();
				closeChannelInternal();
				mSelector.select(1000);

				if (mExit) {
					break;
				}
				if(System.currentTimeMillis()-time >= 1*60*1000){
					time = System.currentTimeMillis();
					LogCenter.error(TAG, "SelectReactor running",null);
				}
				
				for (Iterator<SelectionKey> i = mSelector.selectedKeys().iterator(); i.hasNext();) {
					SelectionKey sk = i.next();
					i.remove();

					if (sk.isValid() == false) {
						sk.cancel();
						continue;
					}

					if (sk.isAcceptable()) {
						try {
							ServerSocketChannel server = (ServerSocketChannel) sk.channel();
							SocketChannel channel = server.accept();
							channel.configureBlocking(false);
							channel.socket().setTcpNoDelay(true);
							channel.socket().setReceiveBufferSize(IConstants.SOCKET_BUFFER_SIZE);
							channel.socket().setSendBufferSize(IConstants.SOCKET_BUFFER_SIZE);

							NioPeer peer = ConnectionManager.getInstance().createNioPeer(mContext, channel, this);
							channel.register(mSelector, SelectionKey.OP_READ | SelectionKey.OP_WRITE, peer);
						}
						catch (Exception e) {
							e.printStackTrace();
							sk.cancel();
						}
					}
					else {
						if (sk.isReadable()) {							
							NioPeer peer = (NioPeer) sk.attachment();
							peer.notifyReadable();
							if (!peer.isProcessing()) {
								peer.setProcessing(true);
								mThreadPool.accept(peer);
							}
						}
						if (sk.isWritable()) {
							sk.interestOps(sk.interestOps() & ~SelectionKey.OP_WRITE);
							NioPeer peer = (NioPeer) sk.attachment();
							peer.notifyWritable();
						}
					}
				}
			}
		}
		catch (Exception e) {
			e.printStackTrace();
			String strMsg = e.getMessage();
			LogCenter.error(TAG, "exception msg: " + strMsg,null,true);
		}

		for (Iterator<SelectionKey> i = mSelector.keys().iterator(); i.hasNext();) {
			SelectionKey k = i.next();
			try {
				k.channel().close();
			}
			catch (Exception e) {
			}
		}

		try {
			mSelector.close();
		}
		catch (Exception e) {
			LogCenter.error("SelectReactor", null, e,true);
		}
	}

	public void closeChannel(SocketChannel channel) {
		synchronized (mListNeedClosedChannel) {
			mListNeedClosedChannel.add(channel);
		}
	}

	private void closeChannelInternal() {
		try {
			synchronized (mListNeedClosedChannel) {
				for (SocketChannel channel : mListNeedClosedChannel) {
					channel.register(mSelector, 0);
					channel.close();
				}

				mListNeedClosedChannel.clear();
			}
		}
		catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void registerWriteOpt(SocketChannel channel) {
		synchronized (mRegisterWriteOptChannels) {
			mRegisterWriteOptChannels.add(channel);
			mSelector.wakeup();
		}
	}

	private void registerWriteOptInternal() {
		try {
			synchronized (mRegisterWriteOptChannels) {
				for (SocketChannel channel : mRegisterWriteOptChannels) {
					SelectionKey key = channel.keyFor(mSelector);
					key.interestOps(key.interestOps() | SelectionKey.OP_WRITE);
				}

				mRegisterWriteOptChannels.clear();
			}
		}
		catch (Exception e) {
			e.printStackTrace();
		}
	}	
}
