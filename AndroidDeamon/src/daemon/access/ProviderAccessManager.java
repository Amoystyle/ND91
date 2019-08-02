package daemon.access;

import java.util.ArrayList;

import android.content.Context;


import daemon.connection.ConnectionManager;
import daemon.event.ChangeConnectListener;
import daemon.event.ConnectionEvent;

public class ProviderAccessManager {

	private Context context;
	private ArrayList<ProviderAccess> accesses;

	private ProviderAccessManager(Context context) {
		this.context = context;
		accesses = new ArrayList<ProviderAccess>();
		ConnectionManager.getInstance().addChangeConnectListener(new ChangeConnectListener() {
			
			@Override
			public void disConnected(ConnectionEvent e) {
				ProviderAccess access;
				for (int i = accesses.size() - 1; i >= 0; i--) {
					access = accesses.get(i);
					if (access.getConnectionId() == e.getConnection().getConnectionId()) {
						access.close();
						accesses.remove(access);
					}
				}
			}
			
			@Override
			public void connected(ConnectionEvent e) {

				
			}
		});
	}

	private static ProviderAccessManager instance;

	public static ProviderAccessManager getInstance(Context context) {
		if (instance == null)
			instance = new ProviderAccessManager(context);
		return instance;
	}

	public ProviderAccess createAccess(int business, int connectionId) {

		ProviderAccess access = getAccess(business, connectionId);
		if (access == null) {
			access = new ProviderAccess(context, business, connectionId);
			accesses.add(access);
		}
		return access;
	}

	private ProviderAccess getAccess(int business, int connectionId) {
		ProviderAccess access;
		for (int i = 0; i < accesses.size(); i++) {
			access = accesses.get(i);

			if (access.getBusiness() == business && access.getConnectionId() == connectionId)
				return access;
		}
		return null;
	}
}
