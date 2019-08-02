package daemon.net.original;

import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;


import daemon.connection.ConnectionManager;
import daemon.logger.LogCenter;
import daemon.net.base.DispatcherThreadPool;
import daemon.net.base.ServerBase;

import android.content.Context;

public class OriginalServerImp extends ServerBase {

	private final String TAG = getClass().getName();

	private ServerSocket mListenSocket;
	private boolean mIsClosed = false;
	private Runnable mSocketServerRunnable = null;

	public OriginalServerImp(Context context, int port) {
		super(context, port);
	}

	// 启动socket服务
	@Override
	public boolean startServer() {
		LogCenter.debug(TAG, "start orginal server");
		
		if (!init())
			return false;

		DispatcherThreadPool.getInstance().accept(mSocketServerRunnable);
		return true;
	}

	// 停止socket服务
	@Override
	public void stopServer() {
		try {
			mListenSocket.close();
			mIsClosed = true;
		}
		catch (Exception e) {
			e.printStackTrace();
		}
	}

	private boolean init() {
		for (int i = 0; i < TRY_COUNT; i++) {
			try {
				mListenSocket = new ServerSocket();
				mListenSocket.setReuseAddress(true);
				mListenSocket.bind(new InetSocketAddress(super.mPort));
			}
			catch (Exception e) {
				mPort++;
				LogCenter.error(TAG, "init original socket server error", e);
				continue;
			}
			break;
		}
        writePortToFile(mPort);

		createRunnable();

		return true;
	}

	private void createRunnable() {
		mSocketServerRunnable = new Runnable() {
			@Override
			public void run() {
				try {
					while (true) {
						if (mIsClosed)
							break;

						Socket clientSocket = mListenSocket.accept();
						OriginalPeer peer = ConnectionManager.getInstance().createOriginalPeer(mContext, clientSocket);
						DispatcherThreadPool.getInstance().accept(peer);
					}
				}
				catch (Exception e) {
					LogCenter.error(TAG, "original socket server accept error", e);
				}
			}
		};
	}
}
