package daemon.receiver;


import daemon.connection.ConnectionManager;
import daemon.protocol.ByteWriter;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;

public class SmartHomeReceiver extends BroadcastReceiver {
	private final String SMART_HOME_ACTION_RESULT = "com.nd.android.smarthome.manage_theme_result";
	private final String MOBOROBO_HOME_ACTION_RESULT = "com.nd.android.moborobo.home.manage_theme_result";
	private final int ACTION_SET_THEME = SendMessageType.SMARTHOME_ACTION_SET_THEME;
	private final int ACTION_DEL_THEME = SendMessageType.SMARTHOME_ACTION_DEL_THEME;
	private final int ACTION_ADD_THEME = SendMessageType.SMARTHOME_ACTION_ADD_THEME;
	private final int ACTION_GET_THEME = SendMessageType.SMARTHOME_ACTION_GET_THEME;
	private final int BUSINESS_ID = SendMessageType.SMARTHOME_BUSINESS_ID;

	@Override
	public void onReceive(Context context, Intent intent) {
		try {
			String action = intent.getAction();
			if (SMART_HOME_ACTION_RESULT.equals(action) || MOBOROBO_HOME_ACTION_RESULT.equals(action)) {
				onReceiveTheme(intent);
			}	
		} catch (Exception e) {
			// TODO: handle exception
		}

	
	}
	
	private void onReceiveTheme(final Intent intent) {			
		Bundle bundle = intent.getExtras();

		if (null != bundle) {
			int type = intent.getIntExtra("type", -1);				

			switch (type) {
			case ACTION_SET_THEME: {
				//LogCenter.error("smarthome", "receive set theme result: " + System.currentTimeMillis());
				ByteWriter writer = new ByteWriter();

				int result = intent.getIntExtra("result", 0);
				String themeId = intent.getStringExtra("themeId");
				writer.Write(ACTION_SET_THEME);
				writer.Write(result);
				writer.Write(themeId);
				ConnectionManager.getInstance().sendMessage(BUSINESS_ID,
						writer);
				//LogCenter.error("smarthome", "callback set theme result: " + System.currentTimeMillis());
				break;
			}

			case ACTION_DEL_THEME: {
				//LogCenter.error("smarthome", "receive del theme result: " + System.currentTimeMillis());
				ByteWriter writer = new ByteWriter();
				int result = intent.getIntExtra("result", 0);
				String themeId = intent.getStringExtra("themeId");
				writer.Write(ACTION_DEL_THEME);
				writer.Write(result);
				writer.Write(themeId);
				ConnectionManager.getInstance().sendMessage(BUSINESS_ID,
						writer);
				//LogCenter.error("smarthome", "callback del theme result: " + System.currentTimeMillis());
				break;
			}

			case ACTION_ADD_THEME: {
				//LogCenter.error("smarthome", "receive add theme result: " + System.currentTimeMillis());
				ByteWriter writer = new ByteWriter();
				int result = intent.getIntExtra("result", 0);
				String themeId = intent.getStringExtra("themeId");
				String name = intent.getStringExtra("name");
				String en_name = intent.getStringExtra("en_name");
				String oldThemeId = intent.getStringExtra("old_themeid");
				writer.Write(ACTION_ADD_THEME);
				writer.Write(result);
				writer.Write(themeId);
				writer.Write(name);
				writer.Write(en_name);
				writer.Write(oldThemeId);
				ConnectionManager.getInstance().sendMessage(BUSINESS_ID,
						writer);				
				//LogCenter.error("smarthome", "callback add theme result: " + System.currentTimeMillis());
				break;
			}
			case ACTION_GET_THEME: {
				//LogCenter.error("smarthome", "receive get theme result: " + System.currentTimeMillis());
				ByteWriter writer = new ByteWriter();
				int result = intent.getIntExtra("result", 0);
				String path = intent.getStringExtra("path");
				writer.Write(ACTION_GET_THEME);
				writer.Write(result);
				writer.Write(path);			
				ConnectionManager.getInstance().sendMessage(BUSINESS_ID,
						writer);
				//LogCenter.error("smarthome", "callback get theme result: " + System.currentTimeMillis());
				break;
			}					

			}
		}
	}		
}
