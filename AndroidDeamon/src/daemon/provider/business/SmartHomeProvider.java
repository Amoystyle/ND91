package daemon.provider.business;

import daemon.common.Common;
import daemon.common.Device;
import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;
import daemon.provider.IProvider;
import daemon.provider.ProviderExecuteContext;
import daemon.util.IConstants;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.os.Build;

public class SmartHomeProvider implements IProvider {
	private final static String SMARTHOME_PACKAGE = "com.nd.android.smarthome";
	private final static String SMARTHOME_LAUNCHER = "com.nd.android.smarthome.launcher.Launcher";

	// com.nd.android.moborobo.launcher
	private final static String MOBOROBO_SMARTHOME_PACKAGE = "com.nd.android.moborobo.launcher";
	private final static String MOBOROBO_SMARTHOME_LAUNCHE = "com.nd.android.smarthome.launcher.Launcher";

	private final String SMART_HOME_ACTION = "com.nd.android.smarthome.manage_theme";
	private final String MOBOROBO_HOME_ACTION = "com.nd.android.moborobo.home.manage_theme";

	private final int ACTION_SET_THEME = 1;
	private final int ACTION_DEL_THEME = 2;
	private final int ACTION_ADD_THEME = 3;
	private final int ACTION_GET_THEME = 4;

	private final int MOBOROBO_ACTION_SET_THEME = 11;
	private final int MOBOROBO_ACTION_DEL_THEME = 12;
	private final int MOBOROBO_ACTION_ADD_THEME = 13;
	private final int MOBOROBO_ACTION_GET_THEME = 14;

	private Context mContext = null;                    

	public SmartHomeProvider(Context context) {
		mContext = context;
	}

	@Override
	public void execute(ProviderExecuteContext ctx) {
		ByteReader reader = ctx.getByteReader();
		ByteWriter writer = ctx.getByteWriter();
		byte action = reader.ReadByte();

		//startHome(action);

		switch (action) {
			case ACTION_SET_THEME:
				setTheme(false, reader, writer);
				break;
			case ACTION_DEL_THEME:
				deleteTheme(false, reader, writer);
				break;
			case ACTION_ADD_THEME:
				addTheme(false, reader, writer);
				break;
			case ACTION_GET_THEME:
				getTheme(false, reader, writer);
				break;
			case MOBOROBO_ACTION_SET_THEME:
				setTheme(true, reader, writer);
				break;
			case MOBOROBO_ACTION_DEL_THEME:
				deleteTheme(true, reader, writer);
				break;
			case MOBOROBO_ACTION_ADD_THEME:
				addTheme(true, reader, writer);
				break;
			case MOBOROBO_ACTION_GET_THEME:
				getTheme(true, reader, writer);
				break;

		}
	}

	private void startHome(int action) {
		// 启动桌面
		try {
			int sdkVersion = Integer.parseInt(Build.VERSION.SDK);
			if (sdkVersion > IConstants.ICECREAM_SANDWICH) {
				ComponentName component = null;
				if (action < 10) {
					if (!Common.isProcessRunning(SMARTHOME_PACKAGE, mContext)) {
						component = new ComponentName(SMARTHOME_PACKAGE, SMARTHOME_LAUNCHER);
						Common.startComponentActivity(component, mContext);
					}
				}
				else {
					if (!Common.isProcessRunning(MOBOROBO_SMARTHOME_PACKAGE, mContext)) {
						component = new ComponentName(MOBOROBO_SMARTHOME_PACKAGE, MOBOROBO_SMARTHOME_LAUNCHE);
						Common.startComponentActivity(component, mContext);
					}
				}
			}
		}
		catch (Exception e) {
			e.printStackTrace();
		}
	}

	private void setTheme(boolean moboroboHome, ByteReader reader, ByteWriter writer) {
		String themeId = reader.ReadString();
		sendBroadcast(moboroboHome, ACTION_SET_THEME, "themeId", themeId);
		writer.Write(1);
	}

	private void deleteTheme(boolean moboroboHome, ByteReader reader, ByteWriter writer) {
		String themeId = reader.ReadString();
		sendBroadcast(moboroboHome, ACTION_DEL_THEME, "themeId", themeId);
		writer.Write(1);
	}

	private void addTheme(boolean moboroboHome, ByteReader reader, ByteWriter writer) {
		String path = reader.ReadString();
		sendBroadcast(moboroboHome, ACTION_ADD_THEME, "path", path);
		writer.Write(1);
	}

	private void getTheme(boolean moboroboHome, ByteReader reader, ByteWriter writer) {
		String path = reader.ReadString();
		sendBroadcast(moboroboHome, ACTION_GET_THEME, "path", path);
		writer.Write(1);
	}

	private void sendBroadcast(boolean moboroboHome, int action, String key, String value) {
		Intent intentTemp = new Intent(moboroboHome ? MOBOROBO_HOME_ACTION : SMART_HOME_ACTION);
		int sdkVersion = Device.getIntSdkVersion();
		if(sdkVersion >= 12){//3.1以上需加FLAG_INCLUDE_STOPPED_PACKAGES才能使停止状态的程序接收到广播
			intentTemp.setFlags(32); //Intent.FLAG_INCLUDE_STOPPED_PACKAGES(api level 12)
		}
		
		
		intentTemp.putExtra(key, value);
		intentTemp.putExtra("type", action);
		mContext.sendBroadcast(intentTemp);
	}

	@Override
	public int getBusiness() {
		return 21;
	}
}
