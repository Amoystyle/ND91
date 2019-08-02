package daemon.net.base;

import android.content.Context;

import daemon.access.ProviderAccess;
import daemon.access.ProviderAccessManager;
import daemon.logger.LogCenter;
import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;
import daemon.util.IConstants;

public class BusinessHandler extends PackageHandler {

	private String TAG = getClass().getName();

	public BusinessHandler(Context context, PackageHead head, Peer peer) {
		super(context, head, peer);
	}

	@Override
	public boolean handle() {

		ByteWriter writer = new ByteWriter();

		try {
			ByteReader reader = getReader();

			if (reader != null) {
				int datasize = 4;
				writer.Write(IConstants.TOKEN);
				writer.Write(IConstants.PACKAGEFLAG_Server);
				writer.Write(datasize);
				writer.Write(mHead.getBusinessId());

				ProviderAccess pa = ProviderAccessManager.getInstance(mContext).createAccess(mHead.getBusinessId(), mHead.getConnectionId());
				pa.execute(reader, writer);

				if ((mHead.getFlag() & IConstants.PACKAGEFLAG_NeedPostBack) == IConstants.PACKAGEFLAG_NeedPostBack)
					return mPeer.write(writer, 60 * 1000);
			}
		}
		catch (Exception ex) {
			ex.printStackTrace();
			LogCenter.error(TAG, "异常：", ex);
			LogCenter.error(TAG, "businessID: " + mHead.getBusinessId() + "异常");
			writer = new ByteWriter();
			int datasize = 4;
			writer.Write(IConstants.TOKEN);
			writer.Write(IConstants.PACKAGEFLAG_Server);
			writer.Write(datasize);
			writer.Write(mHead.getBusinessId());
			writer.WriteString("FAIL");
			writer.Write(99);
			writer.Write(ex.getMessage());

			if ((mHead.getFlag() & IConstants.PACKAGEFLAG_NeedPostBack) == IConstants.PACKAGEFLAG_NeedPostBack)
				mPeer.write(writer, 60 * 1000);
		}

		return false;
	}
}
