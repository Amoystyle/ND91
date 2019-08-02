package daemon.net.base;

import java.io.File;

import android.content.Context;
import android.os.Environment;

public abstract class ServerBase {

	protected Context mContext = null;
	protected int mPort;
	
	protected final int TRY_COUNT =20;
	private String PORT_FILE =Environment.getExternalStorageDirectory()+"/nd/connect/socketport/";

	public ServerBase(Context context, int port) {
		mContext = context;	
		mPort = port;
	}

	// 启动socket服务
	public abstract boolean startServer();

	// 停止socket服务
	public abstract void stopServer();
	
	protected void writePortToFile(int mPort) {
		try {
			if(Environment.MEDIA_MOUNTED.equals(Environment.getExternalStorageState())){
				File dir =new File(PORT_FILE);
			    if(!dir.exists())
			    	dir.mkdirs();
			    File[] files = dir.listFiles();
			    for (int i = 0; i < files.length; i++) {
			    	files[i].delete();
				}
			    File f =new File(PORT_FILE+mPort);
			    f.createNewFile();
			}
		} catch (Exception e) {
			// TODO: handle exception
		}

	}
}
