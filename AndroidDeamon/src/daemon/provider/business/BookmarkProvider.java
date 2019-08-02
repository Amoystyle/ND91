package daemon.provider.business;

import android.content.ContentResolver;
import android.content.ContentUris;
import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.net.Uri;
import android.os.Build;
import android.provider.Browser;
import android.provider.Browser.BookmarkColumns;



import daemon.common.Device;
import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;
import daemon.provider.IProvider;
import daemon.provider.ProviderExecuteContext;

public class BookmarkProvider implements IProvider {
	private Uri BOOKMARK_URI = Browser.BOOKMARKS_URI;
	private Context mContext = null;
	private boolean mHasFolderColumn = false;
	private final String COLUMN_FOLDER = "folder";

	public BookmarkProvider(Context context) {
		this.mContext = context;
		CheckFolderColumn();
	}

	private void CheckFolderColumn() {
		ContentResolver resolver = this.mContext.getContentResolver();
		Cursor cursor = resolver.query(BOOKMARK_URI, null,
				BookmarkColumns.BOOKMARK + "=1", null, null);
		try {
			if (cursor != null) {
				int i = cursor.getColumnIndexOrThrow(COLUMN_FOLDER);
				if (i >= 0)
					mHasFolderColumn = true;
			}
		} catch (Exception e) {

		}finally{
			if(cursor != null)
				cursor.close();
		}
	}

	@Override
	public void execute(ProviderExecuteContext context) {
		ByteReader reader = context.getByteReader();
		ByteWriter writer = context.getByteWriter();
		int action = reader.ReadInt32();
		writer.Write(action);

		switch (action) {
		case 1:// ��ѯ
			Query(writer);
			break;
		case 2:// ����
			AddMark(reader, writer);
			break;
		case 3:// ɾ��
			DeleteMark(reader, writer);
			break;
		case 4:// �޸�
			UpdateMark(reader, writer);
			break;
		}
	}

	@Override
	public int getBusiness() {
		// TODO Auto-generated method stub
		return 5;
	}

	/**
	 * ��ѯ
	 * 
	 * @param writer
	 */
	private void Query(ByteWriter writer) {
		ContentResolver resolver = this.mContext.getContentResolver();
		String[]  selects=null;
		if(Device.getIntSdkVersion()>=14){
			selects=new String[]{BookmarkColumns._ID,BookmarkColumns.BOOKMARK,
					BookmarkColumns.TITLE,BookmarkColumns.URL,
					BookmarkColumns.VISITS,BookmarkColumns.DATE,BookmarkColumns.CREATED};
		}
		Cursor cursor = resolver.query(BOOKMARK_URI,selects ,
				BookmarkColumns.BOOKMARK + "=1", null, null);

		if (cursor != null && cursor.moveToFirst()) {
			try {
				Integer count = cursor.getCount();
				writer.Write(count);
				while (!cursor.isAfterLast()) {
					int id = cursor.getInt(cursor
							.getColumnIndexOrThrow(BookmarkColumns._ID));
					String title = cursor.getString(cursor
							.getColumnIndexOrThrow(BookmarkColumns.TITLE));
					String url = cursor.getString(cursor
							.getColumnIndexOrThrow(BookmarkColumns.URL));
					int visits = cursor.getInt(cursor
							.getColumnIndexOrThrow(BookmarkColumns.VISITS));
					long date = cursor.getLong(cursor
							.getColumnIndexOrThrow(BookmarkColumns.DATE));
					long created = cursor.getLong(cursor
							.getColumnIndexOrThrow(BookmarkColumns.CREATED));
					String description;
					if(Device.getIntSdkVersion()>=14){
						 description = "";
					}else{
						 description = cursor.getString(cursor.getColumnIndexOrThrow("description"));
					}
					
					int bookmark = cursor.getInt(cursor
							.getColumnIndexOrThrow(BookmarkColumns.BOOKMARK));
					writer.Write(id);
					writer.Write(title);
					writer.Write(url);
					writer.Write(visits);
					writer.Write(date);
					writer.Write(created);
					writer.Write(description);
					writer.Write(bookmark);
					cursor.moveToNext();
				}
			} finally {
				cursor.close();
			}
		} else {
			writer.Write(0);
		}
	}

	/**
	 * ��ￄ1�7
	 * 
	 * @param reader
	 * @param writer
	 */
	private void AddMark(ByteReader reader, ByteWriter writer) {
		Integer id = -1;
		try {
			ContentValues values = new ContentValues();
			reader.ReadInt32();
			values.put(BookmarkColumns.TITLE, reader.ReadString());
			values.put(BookmarkColumns.URL, reader.ReadString());
			values.put(BookmarkColumns.VISITS, reader.ReadInt32());
			values.put(BookmarkColumns.DATE, reader.ReadInt64());
			values.put(BookmarkColumns.CREATED,reader.ReadInt64());
			String description=reader.ReadString();
			if(Device.getIntSdkVersion()<14){	
			    values.put("description", description);   
			}
			values.put(BookmarkColumns.BOOKMARK, reader.ReadInt32());
			if(mHasFolderColumn)
				values.put(COLUMN_FOLDER, 0);

			ContentResolver resolver = this.mContext.getContentResolver();

			Uri insert = resolver.insert(BOOKMARK_URI, values);
			id = Integer.parseInt(insert.getLastPathSegment());
		} finally {
			// �Ƿ�ɹￄ1�7
			writer.Write(id > 0);
			// id
			writer.Write(id);
		}
	}

	/**
	 * ɾ��
	 * 
	 * @param reader
	 * @param writer
	 */
	private void DeleteMark(ByteReader reader, ByteWriter writer) {
		boolean success = false;
		try {
			Integer id = reader.ReadInt32();
			ContentResolver resolver = this.mContext.getContentResolver();
			Uri delUri = ContentUris.withAppendedId(BOOKMARK_URI, id);
			success = resolver.delete(delUri, null, null) >= 0; 
		} finally {
			// �Ƿ�ɹￄ1�7
			writer.Write(success);
		}
	}

	/**
	 * �޸�
	 * 
	 * @param reader
	 * @param writer
	 */
	private void UpdateMark(ByteReader reader, ByteWriter writer) {
		boolean success = false;
		try {
			ContentValues values = new ContentValues();
			Integer id = reader.ReadInt32();
			values.put(BookmarkColumns.TITLE, reader.ReadString());
			values.put(BookmarkColumns.URL, reader.ReadString());
			values.put(BookmarkColumns.VISITS, reader.ReadInt32());
			values.put(BookmarkColumns.DATE, reader.ReadInt64());
			values.put(BookmarkColumns.CREATED, reader.ReadInt64());
			String description=reader.ReadString();
			if(Device.getIntSdkVersion()<14){	
			    values.put("description", description);   
			}
			values.put(BookmarkColumns.BOOKMARK, reader.ReadInt32());
			ContentResolver resolver = this.mContext.getContentResolver();

			Uri insUri = ContentUris.withAppendedId(BOOKMARK_URI, id);
			success = resolver.update(insUri, values, null, null) > 0;
		} finally {
			// �Ƿ�ɹￄ1�7
			writer.Write(success);
		}
	}

}
