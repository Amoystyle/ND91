package daemon.receiver;


import daemon.connection.ConnectionManager;
import daemon.logger.LogCenter;
import daemon.protocol.ByteWriter;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;

public class ThemeManageReceiver extends BroadcastReceiver {

	private static final String BROADCAST_RECEIVER_PANDAHOMENAME = "com.nd.android.pandahome.manage_theme2_result";
	private final static int SET_THEME = 1;
	private final static int DELETE_THEME = 2;
	private final static int ADD_THEME = 3;
	private final static int ADD_THEME_NOW = 4;
//	private final static int GET_THEME_INFO = 5;
	private final static int INITUPDATE_THEME_LIST = 6;
	private final static int GET_THEME_BRIEF_INFO = 7;
	private final static int GET_THEME_DETAIL_INFO = 8;
	private final static int Get_THEME_EXPORT_PACKAGE = 9;
	private final static int GET_THEME_BRIEF_INFO_NEW = 10;
	private final static int GET_THEME_COUNT = 11;
	private String TAG = ThemeManageReceiver.class.getName();
	private final int BUSINESS_ID = SendMessageType.THEMEMANAGE_BUSINESS_ID;

	@Override
	public void onReceive(Context context, Intent intent) {
		try {
			String action = intent.getAction();
			if (BROADCAST_RECEIVER_PANDAHOMENAME.equals(action)) {
				onReceiveTheme(intent);
			}
		} catch (Exception e) {
			// TODO: handle exception
		}
	}
	private void onReceiveTheme(final Intent intent) {
		String action = intent.getAction();

		if (BROADCAST_RECEIVER_PANDAHOMENAME.equals(action)) {
			Bundle bundle = intent.getExtras();

			if (null != bundle) {
				int type = intent.getIntExtra("type", -1);
				LogCenter.error(TAG, "ThemeManageReceiver...type=" + type);

				switch (type) {
				case SET_THEME: {
					ByteWriter writer = new ByteWriter();

					int result = intent.getIntExtra("result", 0);
					String themeId = intent.getStringExtra("themeId");
					writer.Write(SendMessageType.THEMEMANAGE_SET_THEME);
					writer.Write(result);
					writer.Write(themeId);
					ConnectionManager.getInstance().sendMessage(BUSINESS_ID,
							writer);
					LogCenter.error(TAG,
							"ThemeManageReceiver set theme: result, themeId="
									+ themeId + "...result = " + result);
					break;
				}

				case DELETE_THEME: {

					ByteWriter writer = new ByteWriter();
					int result = intent.getIntExtra("result", 0);
					String themeId = intent.getStringExtra("themeId");
					writer.Write(SendMessageType.THEMEMANAGE_DELETE_THEME);
					writer.Write(result);
					writer.Write(themeId);
					ConnectionManager.getInstance().sendMessage(BUSINESS_ID,
							writer);
					LogCenter.error(TAG,
							"ThemeManageReceiver del Theme : result, themeId="
									+ themeId + "...result = " + result);
					break;
				}

				case ADD_THEME: {
					ByteWriter writer = new ByteWriter();
					int result = intent.getIntExtra("result", 0);
					String themeId = intent.getStringExtra("themeId");
					String oldThemeId = intent.getStringExtra("old_themeid");
					writer.Write(SendMessageType.THEMEMANAGE_ADD_THEME);
					writer.Write(result);
					writer.Write(themeId);
					writer.Write(oldThemeId);
					
					ConnectionManager.getInstance().sendMessage(BUSINESS_ID,
							writer);
					LogCenter.error(TAG,
							"ThemeManageReceiver add Theme : result, themeId="
									+ themeId + "...result = " + result);

					break;
				}
				case ADD_THEME_NOW: {

					ByteWriter writer = new ByteWriter();
					int result = intent.getIntExtra("result", 0);
					String themeId = intent.getStringExtra("themeId");
					writer.Write(SendMessageType.THEMEMANAGE_ADD_THEME_NOW);
					writer.Write(result);
					writer.Write(themeId);
					LogCenter.error(TAG,
							"ThemeManageReceiver add Theme and set theme : result, themeId="
									+ themeId + "...result = " + result);
					ConnectionManager.getInstance().sendMessage(BUSINESS_ID,
							writer);

					break;
				}

				case INITUPDATE_THEME_LIST: {

					ByteWriter writer = new ByteWriter();
					int result = intent.getIntExtra("result", 0);
					writer.Write(SendMessageType.THEMEMANAGE_INITUPDATE_THEME_LIST);
					writer.Write(result);
					ConnectionManager.getInstance().sendMessage(BUSINESS_ID,
							writer);
					LogCenter.error(TAG,
							"ThemeManageReceiver Init and update theme list : result = "
									+ result);

					break;
				}

				case GET_THEME_BRIEF_INFO: {

					ByteWriter writer = new ByteWriter();
					int result = intent.getIntExtra("result", 0);
					String path = intent.getStringExtra("path");
					writer.Write(SendMessageType.THEMEMANAGE_GET_THEME_BRIEF_INFO);
					writer.Write(result);
					writer.Write(path);
					ConnectionManager.getInstance().sendMessage(BUSINESS_ID,
							writer);
					LogCenter.error(TAG,
							"ThemeManageReceiver get Theme brief info : result, path="
									+ path + "...result = " + result);

					break;
				}

				case GET_THEME_DETAIL_INFO: {

					ByteWriter writer = new ByteWriter();
					int result = intent.getIntExtra("result", 0);
					String path = intent.getStringExtra("path");
					writer.Write(SendMessageType.THEMEMANAGE_GET_THEME_DETAIL_INFO);
					writer.Write(result);
					writer.Write(path);
					ConnectionManager.getInstance().sendMessage(BUSINESS_ID,
							writer);
					LogCenter.error(TAG,
							"ThemeManageReceiver get Theme detail info : result, path="
									+ path + "...result = " + result);

					break;
				}

				case GET_THEME_BRIEF_INFO_NEW: {
					ByteWriter writer = new ByteWriter();
					int result = intent.getIntExtra("result", 0);
					String path = intent.getStringExtra("path");
					writer.Write(SendMessageType.THEMEMANAGE_GET_THEME_BRIEF_INFO_NEW);
					writer.Write(result);
					writer.Write(path);
					ConnectionManager.getInstance().sendMessage(BUSINESS_ID,
							writer);
					LogCenter.error(TAG,
							"ThemeManageReceiver get theme package new  : result, path="
									+ path + "...result = " + result);
					break;
				}
				case GET_THEME_COUNT: {
					ByteWriter writer = new ByteWriter();
					int result = intent.getIntExtra("result", 0);
					int number = intent.getIntExtra("number", 0);
					writer.Write(SendMessageType.THEMEMANAGE_GET_COUNT);
					writer.Write(result);
					writer.Write(number);
					ConnectionManager.getInstance().sendMessage(BUSINESS_ID,
							writer);
					LogCenter.error(TAG,
							"ThemeManageReceiver GET_THEME_COUNT : count="
									+ number + "...result = " + result);
					break;
				}
				case Get_THEME_EXPORT_PACKAGE: {
					ByteWriter writer = new ByteWriter();
					int result = intent.getIntExtra("result", 0);
					String path = intent.getStringExtra("path");
					writer.Write(SendMessageType.THEMEMANAGE_Get_THEME_EXPORT_PACKAGE);
					writer.Write(result);
					writer.Write(path);
					ConnectionManager.getInstance().sendMessage(BUSINESS_ID,
							writer);
					LogCenter.error(TAG,
							"ThemeManageReceiver export theme package : result, path="
									+ path + "...result = " + result);
					break;
				}
				

				}
			}
		}
	}

}
