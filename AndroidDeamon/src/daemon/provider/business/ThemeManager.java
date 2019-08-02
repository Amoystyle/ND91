package daemon.provider.business;


import java.lang.reflect.Method;
import android.annotation.SuppressLint;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.os.Build;
import daemon.common.Common;
import daemon.common.Device;
import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;
import daemon.provider.IProvider;
import daemon.provider.ProviderExecuteContext;
import daemon.util.IConstants;

public class ThemeManager implements IProvider {
	private final static String PANDAHOME_PACKAGE = "com.nd.android.pandahome2";
	private final static String PANDAHOME_LAUNCHER = "com.nd.android.launcher.Launcher";

	public final static String BROADCAST_NAME = "com.nd.android.pandahome.manage_theme2";

	private final static byte SET_THEME = 1;
	private final static byte DELETE_THEME = 2;
	private final static byte ADD_THEME = 3;
	private final static byte ADD_THEME_NOW = 4;
	private final static byte GET_THEME_INFO = 5;
	private final static byte INITUPDATE_THEME_LIST = 6;
	private final static byte GET_THEME_BRIEF_INFO = 7;
	private final static byte GET_THEME_DETAIL_INFO = 8;
	private final static byte SEND_BROADCAST_THEME = 9;
	private final static byte GET_THEME_BRIEF_INFO_NEW = 10;
	private final static byte GET_THEME_COUNT = 11;
	

	private Context mContext;
	private String mPackageName;

	public ThemeManager(Context context) {
		this.mContext = context;
		mPackageName = context.getPackageName();
	}

	@Override
	public void execute(ProviderExecuteContext context) {

		//startHome();

		ByteReader reader = context.getByteReader();
		ByteWriter writer = context.getByteWriter();

		byte action = reader.ReadByte();
		switch (action) {
			case SET_THEME:
				// set theme
				SetTheme(reader, writer);
				break;
			case DELETE_THEME:
				// delete theme
				DeleteTheme(reader, writer);
				break;
			case ADD_THEME:
				// add theme
				AddTheme(reader, writer);
				break;
			case ADD_THEME_NOW:
				// add theme and set theme
				AddAndSetTheme(reader, writer);
				break;
			case GET_THEME_INFO:
				// get theme info
				GetThemeInfo(reader, writer);
				break;
			case INITUPDATE_THEME_LIST:
				// init pandaTheme and update Theme List
				InitAndUpdateThemes(reader, writer);
				break;
			case GET_THEME_BRIEF_INFO:
				GetThemeBriefInfo(reader, writer);
				break;
			case GET_THEME_DETAIL_INFO:
				GetThemeDetailInfo(reader, writer);
				break;
			case SEND_BROADCAST_THEME:
				ExportThemePackage(reader, writer);
				break;
			case GET_THEME_BRIEF_INFO_NEW:
				GetThemeBriefInfoNew(reader, writer);
				break;
			case GET_THEME_COUNT:
				GetThemeCount(writer);
				break;
			default:
				break;
		}
	}


	
	private void GetThemeCount(ByteWriter writer) {
		sendBroadcast(11,null, null);
		writer.Write(1);
	}

	private void GetThemeBriefInfoNew(ByteReader reader, ByteWriter writer) {
		String path = reader.ReadString();
		sendBroadcast(10,"path", path);
		writer.Write(1);
		
	}

	@Override
	public int getBusiness() {
		return 15;
	}

	private void startHome() {
		// ������
		try {
			int sdkVersion = Integer.parseInt(Build.VERSION.SDK);
			if (sdkVersion > IConstants.ICECREAM_SANDWICH && !Common.isProcessRunning(PANDAHOME_PACKAGE, mContext)) {
				ComponentName component = new ComponentName(PANDAHOME_PACKAGE, PANDAHOME_LAUNCHER);
				Common.startComponentActivity(component, mContext);
			}
		}
		catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	@SuppressLint("NewApi")
	private void sendBroadcast(int action, String key, String value) {
		Intent intentTemp = new Intent(BROADCAST_NAME);
		int sdkVersion = Device.getIntSdkVersion();

		// Intent.setPackage方法要求1.6以上API把嫩，此处用反射处理
		if (mPackageName.equals("com.macrobile.daemon")) {
			Common.setIntentPackage(intentTemp, "com.nd.android.freemobiledesktop");			
		}
        if(key!=null&&value!=null){
        	intentTemp.putExtra(key, value);
        }
		intentTemp.putExtra("type", action);
		if(sdkVersion >= 12){//3.1以上需加FLAG_INCLUDE_STOPPED_PACKAGES才能使停止状态的程序接收到广播
			intentTemp.setFlags(32); //Intent.FLAG_INCLUDE_STOPPED_PACKAGES(api level 12)
		}
		if("com.nd.assistance".equals(mContext.getPackageName())){
			intentTemp.setPackage(PANDAHOME_PACKAGE);
		}
		mContext.sendBroadcast(intentTemp);
	}

	private void SetTheme(ByteReader reader, ByteWriter writer) {
		String themeID = reader.ReadString();	
		sendBroadcast(1, "themeId", themeID);
		writer.Write(1);
	}

	private void DeleteTheme(ByteReader reader, ByteWriter writer) {
		String themeID = reader.ReadString();
		sendBroadcast(2, "themeId", themeID);
		writer.Write(1);
	}

	private void AddTheme(ByteReader reader, ByteWriter writer) {
		String path = reader.ReadString();
		sendBroadcast(3, "path", path);
		writer.Write(1);
	}

	private void AddAndSetTheme(ByteReader reader, ByteWriter writer) {
		String path = reader.ReadString();
		sendBroadcast(4, "path", path);
		writer.Write(1);
	}

	private void GetThemeInfo(ByteReader reader, ByteWriter writer) {
		String path = reader.ReadString();
		sendBroadcast(5, "path", path);
		writer.Write(1);
	}

	private void InitAndUpdateThemes(ByteReader reader, ByteWriter writer) {
		Intent intentTemp = new Intent(BROADCAST_NAME);
		intentTemp.putExtra("type", 6);
		mContext.sendBroadcast(intentTemp);
		writer.Write(1);
	}

	private void GetThemeBriefInfo(ByteReader reader, ByteWriter writer) {
		String path = reader.ReadString();
		sendBroadcast(7, "path", path);
		writer.Write(1);
	}

	private void GetThemeDetailInfo(ByteReader reader, ByteWriter writer) {
		String themeID = reader.ReadString();
		sendBroadcast(8, "themeId", themeID);
		writer.Write(1);
	}

	private void ExportThemePackage(ByteReader reader, ByteWriter writer) {
		String themeID = reader.ReadString();
		sendBroadcast(9, "themeId", themeID);
		writer.Write(1);
	}
}
