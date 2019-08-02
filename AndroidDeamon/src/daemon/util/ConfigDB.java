package daemon.util;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.database.Cursor;
import android.content.ContentValues;

public class ConfigDB {
	private static final String DATABASE_NAME = "assistance.db";
	private static final int DATABASE_VERSION = 3;
	
	public static final String TABLE_SETTINGS = "settings";
	public static final String SETTINGS_COLUMN_KEY = "key";
	public static final String SETTINGS_COLUMN_VALUE = "value";
	
	public static final String TABLE_PHOTO_BUCKETS = "photo_buckets";
	public static final String BUCKETS_COLUMN_ID = "id";
	public static final String BUCKETS_COLUMN_DISPLAY_NAME = "display_name";
	public static final String BUCKETS_COLUMN_PATH = "path";
	public static final String BUCKETS_COLUMN_VISIBLE = "visible";
	
    private static ConfigDB mInstance;
	
	public static ConfigDB createInstance(Context ctx){
		if(mInstance == null){
			synchronized (ConfigDB.class) { 
				if(mInstance == null){
					mInstance = new ConfigDB(ctx);
				}
			}
		}
		
		return mInstance;
	}
		
	private ConfigDB(Context context) {
		DatabaseHelper helper = new DatabaseHelper(context);
		readable_db_ = helper.getReadableDatabase();
		writable_db_ = helper.getWritableDatabase();
	}

	private class DatabaseHelper extends SQLiteOpenHelper {
		DatabaseHelper(Context context) {
			super(context, DATABASE_NAME, null, DATABASE_VERSION);
		}

		@Override
		public void onCreate(SQLiteDatabase db) {
			db.execSQL("CREATE TABLE " + TABLE_SETTINGS + "("
					+ SETTINGS_COLUMN_KEY + " TEXT PRIMARY KEY," 
					+ SETTINGS_COLUMN_VALUE + " TEXT);");
			
			db.execSQL("CREATE TABLE " + TABLE_PHOTO_BUCKETS + "("
					+ BUCKETS_COLUMN_ID + " TEXT PRIMARY KEY,"
					+ BUCKETS_COLUMN_DISPLAY_NAME + " TEXT,"
					+ BUCKETS_COLUMN_PATH + " TEXT,"
					+ BUCKETS_COLUMN_VISIBLE + " INTEGER);");
		}

		@Override
		public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
			db.execSQL("DROP TABLE IF EXISTS " + TABLE_SETTINGS);
			db.execSQL("DROP TABLE IF EXISTS " + TABLE_PHOTO_BUCKETS);
			onCreate(db);			
		}
		
		@Override
		public void onDowngrade(SQLiteDatabase db, int oldVersion, int newVersion){
			db.execSQL("DROP TABLE IF EXISTS " + TABLE_SETTINGS);
			db.execSQL("DROP TABLE IF EXISTS " + TABLE_PHOTO_BUCKETS);
			onCreate(db);			
		}
	}
	
	public SQLiteDatabase getReadableDB() {
		return readable_db_;
	}

	public SQLiteDatabase getWritableDB() {
		return writable_db_;
	}

	private SQLiteDatabase readable_db_;
	private SQLiteDatabase writable_db_;
}
