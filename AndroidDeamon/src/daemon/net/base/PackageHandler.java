package daemon.net.base;

import java.nio.ByteBuffer;

import android.content.Context;


import daemon.logger.LogCenter;
import daemon.protocol.ByteReader;
import daemon.protocol.MemoryStream;
import daemon.util.IConstants;

public abstract class PackageHandler {

	protected PackageHead mHead = null;
	protected Peer mPeer = null;
	protected Context mContext = null;
	protected boolean misLongConnection = false;

	public PackageHandler(Context context, PackageHead head, Peer peer) {
		mContext = context;
		mHead = head;
		mPeer = peer;
	}

	public static PackageHandler createHandler(Context context, PackageHead head, Peer peer) {
		if ((head.getFlag() & IConstants.PACKAGEFLAG_CreateLongConnection) == IConstants.PACKAGEFLAG_CreateLongConnection) {
			LogCenter.debug("PackageHandler", "Long connection");
			return new ConnectionHandler(context, head, peer);
		}
		else if (CheckConfirmHandler.isConnectionConfirm(head)) {			
			return new CheckConfirmHandler(context, head, peer);
		}
		else if (FileTransferHandler.isFileTransfer(head)) {		
			return new FileTransferHandler(context, head, peer);
		}else if(FileTransferHandler_Old.isFileTransfer_Old(head)){
			return new FileTransferHandler_Old(context, head, peer);
		}else if (FileTransferHandler_New.isFileTransfer(head)) {		
			return new FileTransferHandler_New(context, head, peer);
		}
		return new BusinessHandler(context, head, peer);
	}

	protected ByteReader getReader() {
		ByteBuffer data = ByteBuffer.allocate(mHead.getDataLength());		
		if (mPeer.read(data, 60 * 1000)) {
			MemoryStream ms = new MemoryStream(data);
			ms.SetLength(mHead.getDataLength());
			ByteReader reader = new ByteReader(ms);
			reader.getBaseStream().setOffset(0);

			return reader;
		}

		return null;
	}

	/**
	 * 处理数据包
	 * 
	 * @return 是否处理成功
	 */
	public abstract boolean handle();

	/**
	 * 是否长连接
	 * 
	 * @return
	 */
	public boolean isLongConnection() {
		return misLongConnection;
	}
}
