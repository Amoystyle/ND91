package daemon.provider.system;

import java.lang.reflect.Constructor;
import java.lang.reflect.Method;
import java.util.ArrayList;


import daemon.common.Device;
import daemon.logger.LogCenter;
import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;
import daemon.provider.IProvider;
import daemon.provider.ProviderExecuteContext;

import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.content.ContentResolver;
import android.content.Context;
import android.database.Cursor;
import android.media.AudioManager;
import android.os.Build;
import android.provider.Settings;
import android.provider.Settings.SettingNotFoundException;
import android.net.Uri;
import android.content.ContentValues;

public class SettingProvider implements IProvider {

	private Context context;
	private IBaseSetting basicSetProvider;
	
	public SettingProvider(Context context) {
		this.context = context;
		
	}
	
	public SettingProvider(Context context, int sdkVersion) {
		this.context = context;
		if(sdkVersion < 3)
			basicSetProvider = new BasicSetProvider_11(this.context);
		else 
			basicSetProvider = new BasicSetProvider_15(this.context);
			
	}

	@Override
	public int getBusiness() {
		return 11;
	}
	
	@Override
	public void execute(ProviderExecuteContext cxt) {
		int settingType = cxt.getByteReader().ReadInt32();
		if(settingType == 1)
		{
			cxt.getByteWriter().Write(1);
			int action =  cxt.getByteReader().ReadInt32();
			cxt.getByteWriter().Write(action);
			switch (action) {
			case 1:   // Query All Basic Settings
				queryAllBasicSettings(cxt.getByteWriter());
				break;
			case 2:   // Query Part Basic Settings 
				queryPartBasicSettings(cxt.getByteReader(), cxt.getByteWriter());
				break;
			case 3:   // Update
				updateBasicSettings(cxt.getByteReader(), cxt.getByteWriter());
				break;
			case 4: // query INSTALL_NON_MARKET_APPS
				queryInstallNonMarketApps(cxt.getByteWriter());
				break;
			default:
				break;	
			}
		}
	}
	
	public void queryInstallNonMarketApps(ByteWriter writer) {
		int value = basicSetProvider.queryInstallNonMarketApps();		
		writer.Write(value);
	}
	
	/**
	 * Query All System Settings
	 * 
	 * @param writer
	 */
	public void queryAllBasicSettings(ByteWriter writer) {
		ArrayList<BasicSetInfo> sysList = basicSetProvider.queryAllBasicSettings();
		
		int sysSize = sysList.size();
		writer.Write(sysSize);
		
		for(BasicSetInfo sysInfo : sysList) {
			writer.Write(sysInfo.getName());
			writer.Write(sysInfo.getValue());
		}
	}
	
	/**
	 * Query Part System Settings 
	 * 
	 * @param reader
	 * @param writer
	 */
	public void queryPartBasicSettings(ByteReader reader, ByteWriter writer) {
		ArrayList<String> nameList = new ArrayList<String>();
		
		int nameSize = reader.ReadInt32();
		for(int i = 0; i < nameSize; i++) {
			nameList.add(reader.ReadString());
		}
		
		ArrayList<BasicSetInfo> syssetList = basicSetProvider.queryPartBasicSettings(nameList);
		
		int sysSize = syssetList.size();
		writer.Write(sysSize);
		for(BasicSetInfo sysInfo : syssetList) {
			writer.Write(sysInfo.getName());
			writer.Write(sysInfo.getValue());
		}
	}
	
	/**
	 * Update
	 * 
	 * @param reader
	 * @param writer
	 */
	public void updateBasicSettings(ByteReader reader, ByteWriter writer) {
		ArrayList<BasicSetInfo> syssetList = new ArrayList<BasicSetInfo>();
		
		int updateSize = reader.ReadInt32();
		for(int i = 0; i < updateSize; i++) {
			BasicSetInfo sysInfo = new BasicSetInfo();
			sysInfo.Read(reader);			
			syssetList.add(sysInfo);
		}
		
		boolean updateSuccess = basicSetProvider.updateBasicSettings(syssetList);
		writer.Write(updateSuccess);
	}
	
	public class BasicSetInfo {
		// Settings下的system表的字段信息
		private String name;      // 某设置名称
		private String value;     // 某设置的值

		public String getName() {
			return name;
		}

		public void setName(String name) {
			this.name = name;
		}

		public String getValue() {
			return value;
		}

		public void setValue(String value) {
			this.value = value;
		}
		
		/*
		 * * 从解码器里读流反序化成员
		 * 
		 * @param reader解码器
		 */
		public void Read(ByteReader reader) {
			name = reader.ReadString();
			value = reader.ReadString();
		}

		/*
		 * * 通过编码器序列化成员到流
		 * 
		 * @param writer编码器
		 */
		public void Write(ByteWriter writer) {
			writer.Write(name);
			writer.Write(value);
		}
	}
	
	public interface IBaseSetting {

		public ArrayList<BasicSetInfo> queryAllBasicSettings();

		public  ArrayList<BasicSetInfo> queryPartBasicSettings(ArrayList<String> namelist) ;

		public boolean updateBasicSettings(ArrayList<BasicSetInfo> syssetList);
		
		public int queryInstallNonMarketApps();
	}
	
	public class BasicSetProvider_11 implements IBaseSetting {
		// Settings下的system表的字段信息
		private static final String NAME = "name";        // 某设置名称
		private static final String VALUE = "value";      // 某设置的值

		private Uri SYSTEM_URI;
		
		private Context mContext;

		private AudioManager  mAudioManager; 
		
		/**
		 * constructor
		 * 
		 * @param context
		 */
		public BasicSetProvider_11(Context context) {
			SYSTEM_URI = Settings.System.CONTENT_URI;
			mContext = context;
			mAudioManager  = (AudioManager)mContext.getSystemService(Context.AUDIO_SERVICE);
		}
		
		public int queryInstallNonMarketApps() {
			int value = 0;

			try {
				value = Settings.System.getInt(context.getContentResolver(), Settings.System.INSTALL_NON_MARKET_APPS);
			}
			catch (SettingNotFoundException e) {
				e.printStackTrace();
			}

			return value;
		}

		
		/**
		 * Query All System Settings
		 * 
		 * @return ArrayList<SystemSetInfo>
		 */
		public ArrayList<BasicSetInfo> queryAllBasicSettings() {
			ArrayList<BasicSetInfo> result = new ArrayList<BasicSetInfo>();
			//Cursor sys_cursor = Settings.System.getSystemList(mContext
			//		.getContentResolver());
			
			LogCenter.debug("BaseSettingProvider", "queryAllBasicSettings.");
			Cursor sys_cursor = mContext.getContentResolver().query(SYSTEM_URI, null,
					null, null, null);

			if ((null != sys_cursor) && (sys_cursor.moveToFirst())) {
				try {
					do {
						BasicSetInfo sysset = new BasicSetInfo();

						sysset.setName(sys_cursor.getString(sys_cursor
								.getColumnIndexOrThrow(NAME)));
						sysset.setValue(sys_cursor.getString(sys_cursor
								.getColumnIndexOrThrow(VALUE)));

						result.add(sysset);
					} while (sys_cursor.moveToNext());
				} finally {
					sys_cursor.close();
				}
			}
			
			LogCenter.debug("BaseSettingProvider", "queryAllBasicSettings count:" + result.size());
			return result;
		}

		/**
		 * Query a Part Of System Settings By Name
		 * 
		 * @param namelist
		 * @return ArrayList<SystemSetInfo>
		 */
		public ArrayList<BasicSetInfo> queryPartBasicSettings(
				ArrayList<String> namelist) {
			ArrayList<BasicSetInfo> result = new ArrayList<BasicSetInfo>();

			int nameSize = namelist.size();
			for (int i = 0; i < nameSize; i++) {
				String where = NAME + "='" + namelist.get(i) + "'";
				Cursor cs = mContext.getContentResolver().query(SYSTEM_URI, null,
						where, null, null);
				if ((null != cs) && (cs.moveToFirst())) {
					try {
						do {
							BasicSetInfo sysset = new BasicSetInfo();
							sysset.setName(namelist.get(i));
							sysset.setValue(cs.getString(cs
									.getColumnIndexOrThrow(VALUE)));

							result.add(sysset);
						} while (cs.moveToNext());
					} finally {
						cs.close();
					}
				}
			} // end for

			return result;
		}

		@SuppressWarnings("unused")
		/**
		 * Update System Settings 
		 * 
		 * @param syssetList
		 * @return
		 */
		public boolean updateBasicSettings(ArrayList<BasicSetInfo> syssetList) {
			boolean result = false;

			int sysSize = syssetList.size();
			for (int i = 0; i < sysSize; i++) {
				
				String name = syssetList.get(i).getName();
				String value = syssetList.get(i).getValue();		
			
				if(name.equals("mode_ringer")){ //静音
					 mAudioManager.setRingerMode(value.equals("0") ? AudioManager.RINGER_MODE_SILENT
			                    : AudioManager.RINGER_MODE_NORMAL);
					 
					result = true;
					continue;
				}
				else if(name.equals("vibrate_on")){ //振动	
			            mAudioManager.setVibrateSetting(AudioManager.VIBRATE_TYPE_RINGER,
			                    value.equals("1") ? 5 : 4);  
			        result = true;
			        continue;
				}
				else if(name.equals("auto_time")){ //网络时间
				    Settings.System.putInt(mContext.getContentResolver(), 
		                    Settings.System.AUTO_TIME, value.equals("1")?1:0);
				    result = true;
				    continue;
				}
				else if(name.equals("date_format")){ //时间格式
					Settings.System.putString(mContext.getContentResolver(), 
		                    Settings.System.DATE_FORMAT, value);
					
					result = true;
					continue;
				}else if (name.equals("volume_ring")) { //铃音
					mAudioManager.setStreamVolume(AudioManager.STREAM_RING, Integer.parseInt(value), 0);

					result = true;
					continue;
				}else if (name.equals("volume_music")) { //媒体音量
					mAudioManager.setStreamVolume(AudioManager.STREAM_MUSIC, Integer.parseInt(value), 0);

					result = true;
					continue;
				}else if (name.equals("screen_off_timeout")) { //屏幕待机时间
					 Settings.System.putInt(mContext.getContentResolver(), 
							 Settings.System.SCREEN_OFF_TIMEOUT, Integer.parseInt(value));
					  
					result = true;
					continue;
				}else if (name.equals("screen_brightness")) { //屏幕亮度
					Settings.System.putInt(mContext.getContentResolver(), 
		                    Settings.System.SCREEN_BRIGHTNESS, Integer.parseInt(value));
					  
					result = true;
					continue;
				}else if (name.equals("airplane_mode_on")) { //飞行模式
					 Settings.System.putInt(mContext.getContentResolver(), Settings.System.AIRPLANE_MODE_ON, 
							 value.equals("1") ? 1 : 0);
					  
					result = true;
					continue;
				}
				
				
				String where = NAME + "='" + syssetList.get(i).getName() + "'";
				ContentValues values = new ContentValues();
				values.put(VALUE, syssetList.get(i).getValue());
				Integer id = 0;
				Cursor cs = mContext.getContentResolver().query(SYSTEM_URI, null, where, null, null);
				if((null != cs) && (cs.moveToFirst())) {
					try {
						do {
							id = cs.getInt(cs.getColumnIndexOrThrow("_id"));
						}while(cs.moveToNext());
					} finally {
						cs.close();
				}
				
					/*
				if(id != 0){
					Uri uri = ContentUris.withAppendedId(SYSTEM_URI, id);
					int tmp = this.mContext.getContentResolver().delete(uri, null, null);
					LogUtil.log("BaseSettingProvider", "delete tmp:" + tmp);
				}*/
				
				ContentValues addValue = new ContentValues();
				addValue.put(NAME, syssetList.get(i).getName());
				addValue.put(VALUE, syssetList.get(i).getValue());
				Uri addUri = this.mContext.getContentResolver().insert(SYSTEM_URI, addValue);
				if(addUri.getPath() != "")
					result = true;
				}

				if(!result)
				{
					break;
				}
			}
			
			return result;
		}

	}

	
	@TargetApi(Build.VERSION_CODES.CUPCAKE)
	public class BasicSetProvider_15 implements IBaseSetting {
		// Settings下的system表的字段信息
		private static final String NAME = "name"; // 某设置名称
		private static final String VALUE = "value"; // 某设置的值

		private Uri SYSTEM_URI;
		private Uri SECURE_URI;

		private Context mContext;

		private AudioManager mAudioManager;

		/**
		 * constructor
		 * 
		 * @param context
		 */
		public BasicSetProvider_15(Context context) {
			SYSTEM_URI = Settings.System.CONTENT_URI;
			SECURE_URI = Settings.Secure.CONTENT_URI;

			mContext = context;
			mAudioManager = (AudioManager) mContext
					.getSystemService(Context.AUDIO_SERVICE);
		}
		
		public int queryInstallNonMarketApps() {
			int value = 0;
			try {
				int sdkVer = Device.getIntSdkVersion();
				if (sdkVer >= 17) {

					Class<?> classType = Class.forName("android.provider.Settings$Global");
					Constructor con = classType.getDeclaredConstructor(null);
					con.setAccessible(true);
					Object classGlobal = con.newInstance();

					Method methodGetInt = classType.getMethod("getInt", new Class[] {ContentResolver.class, String.class});

					Object objID = methodGetInt.invoke(classGlobal, new Object[] {context.getContentResolver(), Settings.Secure.INSTALL_NON_MARKET_APPS});
					value = Integer.parseInt(objID.toString());
				}
				else {
					value = Settings.Secure.getInt(context.getContentResolver(), Settings.Secure.INSTALL_NON_MARKET_APPS);
				}
			}
			catch (Exception e) {
				e.printStackTrace();
			}
			
			return value;
		}


		/**
		 * Query All System Settings
		 * 
		 * @return ArrayList<SystemSetInfo>
		 */
		public ArrayList<BasicSetInfo> queryAllBasicSettings() {
			ArrayList<BasicSetInfo> result = new ArrayList<BasicSetInfo>();
			// Cursor sys_cursor = Settings.System.getSystemList(mContext
			// .getContentResolver());

			LogCenter.debug("BaseSettingProvider", "queryAllBasicSettings.");

			QuerySettingInfos(SYSTEM_URI, result);
			LogCenter.debug("BaseSettingProvider", "query SYSTEM_URI count:"
					+ result.size());
			QuerySettingInfos(SECURE_URI, result);
			LogCenter.debug("BaseSettingProvider", "query SECURE_URI count:"
					+ result.size());
			return result;
		}

		/**
		 * Query a Part Of System Settings By Name
		 * 
		 * @param namelist
		 * @return ArrayList<SystemSetInfo>
		 */
		public ArrayList<BasicSetInfo> queryPartBasicSettings(
				ArrayList<String> namelist) {
			ArrayList<BasicSetInfo> result = new ArrayList<BasicSetInfo>();

			int nameSize = namelist.size();
			for (int i = 0; i < nameSize; i++) {
				String where = NAME + "='" + namelist.get(i) + "'";
				Cursor cs = mContext.getContentResolver().query(SYSTEM_URI, null,
						where, null, null);

				try {
					if ((null != cs) && (cs.moveToFirst())) {

						do {
							BasicSetInfo sysset = new BasicSetInfo();
							sysset.setName(namelist.get(i));
							sysset.setValue(cs.getString(cs
									.getColumnIndexOrThrow(VALUE)));

							result.add(sysset);
						} while (cs.moveToNext());
					}
				} finally {
					if (null != cs) {
						cs.close();
					}
				}
			} // end for

			return result;
		}

		/**
		 * Update System Settings
		 * 
		 * @param syssetList
		 * @return
		 */
		public boolean updateBasicSettings(ArrayList<BasicSetInfo> syssetList) {

			ArrayList<BasicSetInfo> systemList = new ArrayList<BasicSetInfo>();
			ArrayList<BasicSetInfo> secureList = new ArrayList<BasicSetInfo>();

			for (int i = 0; i < syssetList.size(); i++) {
				String fieldName = syssetList.get(i).getName();
				if (fieldName.equals("bluetooth_on")
						|| fieldName.equals("data_roaming")
						|| fieldName.equals("install_non_market_apps")
						|| fieldName.equals("network_preference")
						|| fieldName.equals("usb_mass_storage_enabled")
						|| fieldName
								.equals("wifi_networks_available_notification_on")
						|| fieldName.equals("wifi_watchdog_watch_list")
						|| fieldName.equals("mock_location")
						|| fieldName.equals("location_providers_allowed")
						|| fieldName.equals("device_provisioned")
						|| fieldName.equals("adb_enabled")
						|| fieldName.equals("logging_id2")
						|| fieldName.equals("logging_id")
						|| fieldName.equals("enabled_input_methods")
						|| fieldName.equals("default_input_method")
						|| fieldName.equals("wifi_on")) {
					secureList.add(syssetList.get(i));
				} else {
					systemList.add(syssetList.get(i));
				}
			}

			LogCenter.debug("BaseSettingProvider", "update SYSTEM_URI count:"
					+ systemList.size());
			boolean result = true;
			if (systemList.size() > 0) {
				result = UpdateSettingInfos(SYSTEM_URI, systemList);
				if (!result)
					return result;
			}

			LogCenter.debug("BaseSettingProvider", "update SECURE_URI count:"
					+ secureList.size());
			if (secureList.size() > 0) {
				result = UpdateSettingInfos(SECURE_URI, secureList);
				if (!result)
					return result;
			}

			return result;
		}

		private boolean UpdateSettingInfos(Uri updateUri,
				ArrayList<BasicSetInfo> syssetList) {

			boolean result = false;

			int sysSize = syssetList.size();
			for (int i = 0; i < sysSize; i++) {
				String name = syssetList.get(i).getName();
				String value = syssetList.get(i).getValue();

				if (name.equals("mode_ringer")) { //是否静音
					mAudioManager
							.setRingerMode(value.equals("0") ? AudioManager.RINGER_MODE_SILENT
									: AudioManager.RINGER_MODE_NORMAL);

					result = true;
					continue;
				} else if (name.equals("vibrate_on")) { //振动
					mAudioManager.setVibrateSetting(
							AudioManager.VIBRATE_TYPE_RINGER,
							value.equals("1") ? AudioManager.VIBRATE_SETTING_ON
									: AudioManager.VIBRATE_SETTING_OFF);

					result = true;
					continue;
				} else if (name.equals("auto_time")) { //网络时间
					Settings.System.putInt(mContext.getContentResolver(),
							Settings.System.AUTO_TIME, value.equals("1") ? 1 : 0);
					result = true;
					continue;
				} else if (name.equals("date_format")) { //时间格式
					Settings.System.putString(mContext.getContentResolver(),
							Settings.System.DATE_FORMAT, value);

					result = true;
					continue;
				} else if (name.equals("volume_ring")) { //铃音
					mAudioManager.setStreamVolume(AudioManager.STREAM_RING, Integer.parseInt(value), 0);

					result = true;
					continue;
				}else if (name.equals("volume_music")) { //媒体音量
					mAudioManager.setStreamVolume(AudioManager.STREAM_MUSIC, Integer.parseInt(value), 0);

					result = true;
					continue;
				}else if (name.equals("screen_off_timeout")) { //屏幕待机时间
					 Settings.System.putInt(mContext.getContentResolver(), 
							 Settings.System.SCREEN_OFF_TIMEOUT, Integer.parseInt(value));
					  
					result = true;
					continue;
				}else if (name.equals("screen_brightness")) { //屏幕亮度
					Settings.System.putInt(mContext.getContentResolver(), 
		                    Settings.System.SCREEN_BRIGHTNESS, Integer.parseInt(value));
					  
					result = true;
					continue;
				}

				ContentValues addValue = new ContentValues();
				addValue.put(NAME, name);
				addValue.put(VALUE, value);
				Uri addUri = this.mContext.getContentResolver().insert(updateUri,
						addValue);
				if (addUri.getPath() != "")
					result = true;
				// }

				if (!result) {
					break;
				}
			}

			return result;
		}

		private void QuerySettingInfos(Uri QueryUri, ArrayList<BasicSetInfo> result) {
			if (result == null)
				return;

			Cursor sys_cursor = mContext.getContentResolver().query(QueryUri, null,
					null, null, null);

			try {
				if ((null != sys_cursor) && (sys_cursor.moveToFirst())) {

					do {
						BasicSetInfo sysset = new BasicSetInfo();

						sysset.setName(sys_cursor.getString(sys_cursor
								.getColumnIndexOrThrow(NAME)));
						sysset.setValue(sys_cursor.getString(sys_cursor
								.getColumnIndexOrThrow(VALUE)));

						result.add(sysset);
					} while (sys_cursor.moveToNext());
				}
			} finally {
				if (null != sys_cursor) {
					sys_cursor.close();
				}
			}
		}
	}
}
