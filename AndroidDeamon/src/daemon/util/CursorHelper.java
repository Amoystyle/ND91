package daemon.util;

import android.content.ContentResolver;
import android.database.Cursor;
import android.net.Uri;
public class CursorHelper {

	//部分机器刚重启时连接，sd卡相关的数据获取不到，据测试，7、8秒后就能获取到，此处cursor为null时，延时20秒
	public static Cursor query(ContentResolver resolver,Uri uri, String[] projection,
			String selection, String[] selectionArgs, String sortOrder){
		Cursor cursor = null;
		try {
			int count =0;
			while(count<80){
				cursor =resolver.query(uri, projection, selection, selectionArgs, sortOrder);
				if(cursor!=null){
					return cursor;
				}
				Thread.sleep(250);
				count++;
			}
			
		} catch (Exception e) {

		}
		
		return cursor;
	}

}
