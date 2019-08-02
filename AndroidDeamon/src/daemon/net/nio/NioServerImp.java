package daemon.net.nio;

import android.content.Context;


import daemon.logger.LogCenter;
import daemon.net.base.DispatcherThreadPool;
import daemon.net.base.ServerBase;

public class NioServerImp extends ServerBase {

	private final String TAG = getClass().getName();
	private SelectReactor mReactor;

	public NioServerImp(Context context, int port) {
		super(context, port);
		mReactor = new SelectReactor(context);
	}

	// 启动socket服务
	@Override
	public boolean startServer() {
		LogCenter.debug(TAG, "start nio server",null,true);
		for (int i = 0; i < TRY_COUNT; i++) {
			try {
				if (!mReactor.open(mPort)){
					mPort++;
					continue;
				}
				writePortToFile(mPort);	
				DispatcherThreadPool.getInstance().accept(mReactor);
				return true;
			}
			catch (Exception e) {
				e.printStackTrace();
				mPort++;
			}
		}
		

		
		return false;
	}


	// 停止socket服务
	@Override
	public void stopServer() {
		mReactor.close();
	}
}
