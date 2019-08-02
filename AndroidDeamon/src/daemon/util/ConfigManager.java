package daemon.util;

import java.util.ArrayList;
import java.util.List;

import daemon.provider.media.PhotoBucket;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;

public class ConfigManager {
	private static final String KEY_PHONE_NAME = "phone_name";
	private static final String KEY_IS_WIFI_ON = "is_wifi_on";
	private static final String KEY_FIFO_PRIVATE = "fifo_private_path";
	private static final String KEY_WIFI_SHOW_NOTIFICATION = "wifi_show_notification";
	private static final String KEY_NEED_CONNECT_CONFIRM = "wifi_connect_confirm";
	private static final String KEY_BATTERY_SPEED = "battery_speed";
	private static final String KEY_WIFI_NAME = "wifi_name";
	
	private static Object mLock = new Object();

	private ConfigManager() {

	}
	
	public static String getWifiName(Context ctx) {
		return getSettingValue(ctx, KEY_WIFI_NAME);
	}

	public static void setWifiName(Context ctx, String value) {
		putSettingValue(ctx, KEY_WIFI_NAME, value);
	}
	
	public static String getBatterySpeed(Context ctx){
		return getSettingValue(ctx, KEY_BATTERY_SPEED);
	}
	
	public static void setBatterySpeed(Context ctx, String value){
		putSettingValue(ctx, KEY_BATTERY_SPEED, value);
	}

	public static String getPhoneName(Context ctx) {
		return getSettingValue(ctx, KEY_PHONE_NAME);
	}

	public static void setPhoneName(Context ctx, String value) {
		putSettingValue(ctx, KEY_PHONE_NAME, value);
	}

	public static boolean getIsWifiOn(Context ctx) {
	    return getBoolean(ctx, KEY_IS_WIFI_ON, false);
	}

	public static void setIsWifiOn(Context ctx, boolean value) {	
		setBoolean(ctx, KEY_IS_WIFI_ON, value);
	}
	
	public static boolean getIsWifiShowNotify(Context ctx){
		return getBoolean(ctx, KEY_WIFI_SHOW_NOTIFICATION, true); //默认通知提醒
	}
	
	public static void setIsWifiShowNotify(Context ctx, boolean value){
		setBoolean(ctx, KEY_WIFI_SHOW_NOTIFICATION, value);
	}
	
	public static boolean getIsNeedConnectConfirm(Context ctx){
		return getBoolean(ctx, KEY_NEED_CONNECT_CONFIRM, false);
	}
	
	public static void setIsNeedConnectConfirm(Context ctx, boolean value){
		setBoolean(ctx, KEY_NEED_CONNECT_CONFIRM, value);
	}
	
	private static boolean getBoolean(Context ctx, String key, boolean defaultValue){
		String value = getSettingValue(ctx, key);
		if(value == null || value.equals(""))
			return defaultValue;
		
		return value.toLowerCase().equals("true");
	}
	
	private static void setBoolean(Context ctx, String key, boolean value){
		String strValue = value ? "true" : "false";
		putSettingValue(ctx, key, strValue);
	}

	public static String getFifoPrivate(Context ctx) {
		return getSettingValue(ctx, KEY_FIFO_PRIVATE);
	}

	public static void setFifoPrivate(Context ctx, String value) {
		putSettingValue(ctx, KEY_FIFO_PRIVATE, value);
	}

	private static  String getSettingValue(Context ctx, String key) {
		synchronized (mLock) {
			Cursor cur = null;

			try {
				ConfigDB configDB = ConfigDB.createInstance(ctx);
				SQLiteDatabase db = configDB.getReadableDB();
				
				String[] columns = new String[] {ConfigDB.SETTINGS_COLUMN_KEY, ConfigDB.SETTINGS_COLUMN_VALUE};
				cur = db.query(ConfigDB.TABLE_SETTINGS, columns, null, null, null, null, null);
				if (cur != null && cur.moveToFirst()) {
					while (!cur.isAfterLast()) {
						String tmpKey = cur.getString(0);
						if (tmpKey.equals(key)) {
							String value = cur.getString(1);
							return value;
						}

						cur.moveToNext();
					}
				}
			}
			catch (Exception e) {
				e.printStackTrace();
			}
			finally {
				if (cur != null)
					cur.close();
			}
		}
		return "";
	}

	private static  void putSettingValue(Context ctx, String key, String value) {
		synchronized (mLock) {
			try{
				ConfigDB configDB = ConfigDB.createInstance(ctx);
				SQLiteDatabase db = configDB.getWritableDB();

				ContentValues values = new ContentValues();

				String existValue = getSettingValue(ctx, key);
				if (existValue.equals("")) {
					values.put(ConfigDB.SETTINGS_COLUMN_KEY, key);
					values.put(ConfigDB.SETTINGS_COLUMN_VALUE, value);
					db.insert(ConfigDB.TABLE_SETTINGS, null, values);
				}
				else {
					values.put(ConfigDB.SETTINGS_COLUMN_VALUE, value);
					String where = ConfigDB.SETTINGS_COLUMN_KEY + "='" + key + "'";
					db.update(ConfigDB.TABLE_SETTINGS, values, where, null);
				}
			}catch (Exception e) {

			}
		}
	}

	public static List<PhotoBucket> getPhotoBuckets(Context ctx) {
		List<PhotoBucket> buckets = new ArrayList<PhotoBucket>();
		synchronized (mLock) {
			ConfigDB configDB = ConfigDB.createInstance(ctx);
			SQLiteDatabase db = configDB.getReadableDB();
			Cursor cur = null;
			String[] columns = new String[] {ConfigDB.BUCKETS_COLUMN_ID, ConfigDB.BUCKETS_COLUMN_DISPLAY_NAME, ConfigDB.BUCKETS_COLUMN_PATH, ConfigDB.BUCKETS_COLUMN_VISIBLE};

			try {
				cur = db.query(ConfigDB.TABLE_PHOTO_BUCKETS, columns, null, null, null, null, null);
				if (cur != null && cur.moveToFirst()) {
					while (!cur.isAfterLast()) {
						PhotoBucket bucket = new PhotoBucket();
						bucket.setBucketId(cur.getString(0));
						bucket.setDisplayName(cur.getString(1));
						bucket.setPath(cur.getString(2));
						bucket.setVisible(cur.getInt(3));
						buckets.add(bucket);

						cur.moveToNext();
					}
				}
			}
			catch (Exception e) {
				e.printStackTrace();
			}
			finally {
				if (cur != null)
					cur.close();
			}
		}
		return buckets;
	}

	public static PhotoBucket getPhotoBucket(Context ctx, String bucketId) {
		synchronized (mLock) {
			ConfigDB configDB = ConfigDB.createInstance(ctx);
			SQLiteDatabase db = configDB.getReadableDB();
			Cursor cur = null;
			String[] columns = new String[] {ConfigDB.BUCKETS_COLUMN_ID, ConfigDB.BUCKETS_COLUMN_DISPLAY_NAME, ConfigDB.BUCKETS_COLUMN_PATH, ConfigDB.BUCKETS_COLUMN_VISIBLE};

			try {
				cur = db.query(ConfigDB.TABLE_PHOTO_BUCKETS, columns, null, null, null, null, null);
				if (cur != null && cur.moveToFirst()) {
					while (!cur.isAfterLast()) {
						String tmpBucketId = cur.getString(0);
						if (tmpBucketId.equals(bucketId)) {
							PhotoBucket bucket = new PhotoBucket();
							bucket.setBucketId(bucketId);
							bucket.setDisplayName(cur.getString(1));
							bucket.setPath(cur.getString(2));
							bucket.setVisible(cur.getInt(3));
							return bucket;
						}

						cur.moveToNext();
					}
				}
			}
			catch (Exception e) {
				e.printStackTrace();
			}
			finally {
				if (cur != null)
					cur.close();
			}
		}
		return null;
	}
	
	public static void putPhotoBucket(Context ctx, PhotoBucket bucket){
		if(bucket == null)
			return;
		synchronized (mLock) {
			ConfigDB configDB = ConfigDB.createInstance(ctx);
			SQLiteDatabase db = configDB.getWritableDB();
			
			ContentValues values = new ContentValues();
			values.put(ConfigDB.BUCKETS_COLUMN_DISPLAY_NAME, bucket.getDisplayName());
			values.put(ConfigDB.BUCKETS_COLUMN_PATH, bucket.getPath());
			values.put(ConfigDB.BUCKETS_COLUMN_VISIBLE, bucket.getVisible());
			
			PhotoBucket existBucket = getPhotoBucket(ctx, bucket.getBucketId());
			if(existBucket != null){
				String where = ConfigDB.BUCKETS_COLUMN_ID + "='"  + bucket.getBucketId() + "'";
				db.update(ConfigDB.TABLE_PHOTO_BUCKETS, values, where, null);
			}else{
				values.put(ConfigDB.BUCKETS_COLUMN_ID, bucket.getBucketId());
				db.insert(ConfigDB.TABLE_PHOTO_BUCKETS, null, values);
			}
		}

	}
	
	public static void deletePhotoBucket(Context ctx, String bucketId){
		synchronized (mLock) {
			ConfigDB configDB = ConfigDB.createInstance(ctx);
			SQLiteDatabase db = configDB.getWritableDB();
			String where = ConfigDB.BUCKETS_COLUMN_ID + "='" + bucketId + "'";
			
			try{
				db.delete(ConfigDB.TABLE_PHOTO_BUCKETS, where, null);
			}catch(Exception e){
				e.printStackTrace();
			}
		}
	}
}
