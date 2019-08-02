package daemon.access;

import java.util.ArrayList;

import android.content.Context;


import daemon.connection.ConnectionInfo;
import daemon.logger.LogCenter;
import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;
import daemon.provider.IProvider;
import daemon.provider.ProviderExecuteContext;
import daemon.provider.ProviderManager;

public class ProviderAccess {

	private final String TAG = getClass().getName();
	private Context mContext;
	private int mBusinessId;
	private int mConnectionId;
	private ArrayList<ProviderExecuteContext> contexts;

	public ProviderAccess(Context context, int business, int connectionId) {
		mContext = context;
		mBusinessId = business;
		mConnectionId = connectionId;
		contexts = new ArrayList<ProviderExecuteContext>();
	}

	public int getBusiness() {
		return mBusinessId;
	}

	public int getConnectionId() {
		return mConnectionId;
	}

	public IProvider getProvider() {
		return ProviderManager.getInstance(mContext).createProvider(mBusinessId);
	}

	public void execute(ByteReader reader, ByteWriter writer) {
		ProviderExecuteContext context = new ProviderExecuteContext(reader, writer, this);

		IProvider provider = getProvider();
		if (provider != null) {
			contexts.add(context);
			getProvider().execute(context);
			contexts.remove(context);
		}
		else {
			LogCenter.error(TAG, "provider is null, businessId:" + mBusinessId);
		}
	}

	public void close() {
		ProviderExecuteContext context;
		for (int i = 0; i < contexts.size(); i++) {
			context = (ProviderExecuteContext) contexts.get(i);
			context.cancel();
		}
	}
}
