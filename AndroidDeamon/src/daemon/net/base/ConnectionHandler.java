package daemon.net.base;
import android.content.Context;

import daemon.connection.ConnectionManager;
import daemon.logger.LogCenter;
import daemon.net.nio.NioPeer;
import daemon.protocol.ByteReader;
import daemon.util.IConstants;

public class ConnectionHandler extends PackageHandler {
	private String TAG = getClass().getName();

	public static boolean isConnection(PackageHead head) {
		if ((head.getFlag() & IConstants.PACKAGEFLAG_CreateLongConnection) == IConstants.PACKAGEFLAG_CreateLongConnection)
			return true;
		else if ((head.getFlag() & IConstants.PACKAGEFLAG_CloseCurrentConnection) == IConstants.PACKAGEFLAG_CloseCurrentConnection)
			return true;

		return false;
	}

	public ConnectionHandler(Context context, PackageHead head, Peer peer) {
		super(context, head, peer);
		misLongConnection = true;
	}

	@Override
	public boolean handle() {
		if ((mHead.getFlag() & IConstants.PACKAGEFLAG_CreateLongConnection) == IConstants.PACKAGEFLAG_CreateLongConnection) {
			if (mHead.getDataLength() > 0) {			
				ByteReader reader = getReader();
				if (reader != null) {
					String pcName = "";
					boolean doConnectConfirm = true;
					int timeout = 30;

					int version = reader.ReadInt32();
					if (version == 1) {
						pcName = reader.ReadString();
						timeout = reader.ReadInt32();
					}
					else if (version == 2) {
						pcName = reader.ReadString();
						timeout = reader.ReadInt32();
						doConnectConfirm = reader.ReadBoolean();
					}
				
					boolean blnResult = ConnectionManager.getInstance().createConnection(mContext, mPeer, pcName, timeout, doConnectConfirm);
					//长连接加入列表管理后，立即设置为未在处理，这是为了解决PC端因超时收不到心跳包而断开长连接时,
					//长连接的peer会在SelectReactor中得到一个可读事件从而通知守护关闭长连接
					if (mPeer instanceof NioPeer) {
						((NioPeer) mPeer).setProcessing(false);
					}
					
					return blnResult;
				}
			}

			return false;
		}
		else if ((mHead.getFlag() & IConstants.PACKAGEFLAG_CloseCurrentConnection) == IConstants.PACKAGEFLAG_CloseCurrentConnection) {		
			mPeer.close();
			return true;
		}

		return false;
	}
}
