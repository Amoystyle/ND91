package daemon.net.base;

import java.nio.ByteBuffer;
import android.content.Context;


import daemon.connection.ConnectionManager;
import daemon.logger.LogCenter;
import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;
import daemon.protocol.MemoryStream;
import daemon.util.ConfigManager;
import daemon.util.IConstants;

public class CheckConfirmHandler extends PackageHandler {
	private String TAG = getClass().getName();

	public CheckConfirmHandler(Context context,PackageHead head, Peer peer) {
		super(context, head, peer);
	}

	public static final int CONFIRM_BUSINESS_ID = 200;

	public static boolean isConnectionConfirm(PackageHead head) {
		if (head.getBusinessId() == CONFIRM_BUSINESS_ID)
			return true;

		return false;
	}

	@Override
	public boolean handle() {
		ByteBuffer data = ByteBuffer.allocate(mHead.getDataLength());
		if (mPeer.read(data, 1000)) {
			data.flip();
			MemoryStream ms = new MemoryStream(data);
			ms.SetLength(mHead.getDataLength());
			ByteReader reader = new ByteReader(ms);
						
			ByteWriter writer = new ByteWriter();
			int action = reader.ReadInt32();
			
			if (action == 1) {
				int needConfirm = 0;
				
				if (ConnectionManager.isWifiConnect(mPeer.getIPAddress())) {
					if (ConfigManager.getIsWifiOn(mContext)) {
						if (ConfigManager.getIsNeedConnectConfirm(mContext)) 
							needConfirm = 1;
						else
							needConfirm = 0;
					}
					else {// 如果wifi没开则不允许连接，返回-1
						needConfirm = -1;
					}
				}
				
				writer = new ByteWriter();
				writer.Write(IConstants.TOKEN);
				writer.Write(IConstants.PACKAGEFLAG_Server);
				writer.Write(4 + 4); //整型长度4
				writer.Write(CONFIRM_BUSINESS_ID);
				writer.Write(needConfirm);
			}

			writer.getBaseStream().setOffset(0);
			ByteBuffer resultData = writer.getBaseStream().GetBuffer();
			resultData.rewind();
			return mPeer.write(resultData, 1000);
		}

		return false;
	}
}
