package daemon.provider.system;

import java.util.UUID;

import daemon.common.Common;
import daemon.common.Device;
import daemon.logger.LogCenter;
import daemon.protocol.ByteWriter;
import daemon.provider.IProvider;
import daemon.provider.ProviderExecuteContext;
import daemon.util.ConfigManager;
import android.content.Context;

public class DeamonProvider implements IProvider {
	private String TAG = getClass().getName();
	private static UUID sUuid = null;
	
	private Context mContext;
	private final String mDaemonVersion;

	public DeamonProvider(Context ctx) {
		mContext = ctx;
		mDaemonVersion = Common.getAppVersionName(mContext, "");
	}

	@Override
	public void execute(ProviderExecuteContext context) {
		int action = context.getByteReader().ReadInt32();	
		switch (action) {
			case 1:
				getDeamonInfo(context.getByteWriter());
				break;
			case 2: // pass word
				String pwd = get_wifi_pwd();
				context.getByteWriter().Write(pwd);
				break;
			case 3:
				context.getByteWriter().Write(mDaemonVersion);
				break;
			case 4:
				getUuid(context.getByteWriter());
				break;
			default:
				break;
		}

	}
	
	private void getUuid(ByteWriter writer) {
		UUID uuid = getUuid();
		String strUuid = uuid.toString();
		LogCenter.debug(TAG, "UUID: " + strUuid);
		writer.Write(strUuid);
	}
	
	synchronized private UUID getUuid() {
		if (sUuid == null) {
			sUuid = UUID.randomUUID();
		}
		
		return sUuid;
	}

	private void getDeamonInfo(ByteWriter writer) {
		writer.Write(6);

		writer.Write("version");
		writer.Write(mDaemonVersion);

		writer.Write("imei");
		writer.Write(Device.getIMEI(mContext));

		writer.Write("mac");
		writer.Write(Device.getMacAddress(mContext));

		writer.Write("password");
		writer.Write(get_wifi_pwd());

		writer.Write("is_wifi_on");
		writer.Write(ConfigManager.getIsWifiOn(mContext) ? "1" : "0");

		writer.Write("model");
		writer.Write(Device.getModel());
	}

	private String get_wifi_pwd() {
		return "";
	}

	@Override
	public int getBusiness() {
		return 16;
	}

}
