package daemon.provider.media;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;

import android.content.ContentResolver;
import android.content.ContentUris;
import android.content.ContentValues;
import android.content.Context;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.graphics.Bitmap.CompressFormat;
import android.graphics.drawable.BitmapDrawable;
import android.net.Uri;
import android.os.Environment;
import android.provider.MediaStore.Images;
import android.provider.MediaStore.Audio.Media;
import android.provider.MediaStore.Images.Thumbnails;

import daemon.logger.LogCenter;
import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;
import daemon.provider.IProvider;
import daemon.provider.ProviderExecuteContext;
import daemon.provider.media.PhotoBucket;
import daemon.util.ConfigManager;

public class PhotoProvider_Old implements IProvider {

	private String TAG = this.getClass().getName();

	static Object syncObj = new Object();
	private Context context;

	public PhotoProvider_Old(Context context) {
		this.context = context;
	}

	@Override
	public int getBusiness() {
		return 3;
	}

	@Override
	public void execute(ProviderExecuteContext cxt) {
		ByteReader reader = cxt.getByteReader();
		byte action = reader.ReadByte();
		if (action == 99) { // query
			String tableName = reader.ReadString();
			if (tableName.equals("bucket")) {
				queryBucket(cxt);
			}
			else if (tableName.equals("images")) {
				queryPhotos(cxt);
			}
			else if (tableName.equals("thumbnails")) {
				queryThumbnails(cxt);
			}
		}
		else if (action == 0) { // change
			String tableName = reader.ReadString();
			if (tableName.equals("bucket")) {
				execBuckets(cxt);
			}
			else if (tableName.equals("images")) {
				execPhotos(cxt);
			}
		}
		else if (action == 1) { // set wallpaper
			setWallPaperByRow(cxt);
		}
		else if (action == 2) { // get wallpaper
			getWallPaper(cxt);
		}
		else if (action == 3) { // get screenshot havnt implemented
			// GetScreenShot(reader, writer);
		}
		else if (action == 4) { // set wallpaper by path
			setWallPaperByPath(cxt);
		}
		else if (action == 5) { // add contact photo
			addContactPhoto(cxt);
		}
		else if (action == 6) {
			setWallPaper(cxt);
		}
		else if (action == 7) {
			clearWallPaper(cxt);
		}
		else if (action == 8) {
			getTotalSize(cxt);
		}
		else if (action == 9) {
			int cnt = getPhotoCount();
			cxt.getByteWriter().Write(cnt);
		}
		else if (action == 10) {
			context.sendBroadcast(new Intent(Intent.ACTION_MEDIA_MOUNTED, Uri.parse("file://" + Environment.getExternalStorageDirectory())));
		}
	}

	private void queryBucket(ProviderExecuteContext cxt) {
		synchronized (syncObj) {
			List<PhotoBucket> buckets = ConfigManager.getPhotoBuckets(this.context);
			for (int i = buckets.size() - 1; i >= 0; i--) {
				// 判断一下分类是否还在
				if (!(new File(buckets.get(i).getPath())).exists()) {
					buckets.remove(i);
				}
			}

			cxt.getByteWriter().Write(buckets.size());
			for (PhotoBucket bucket : buckets) {
				cxt.getByteWriter().Write(bucket.getBucketId());
				cxt.getByteWriter().Write(bucket.getDisplayName());
				cxt.getByteWriter().Write(bucket.getPath());
				cxt.getByteWriter().Write(bucket.getVisible());
			}
		}
	}

	private void queryPhotos(ProviderExecuteContext cxt) {
		synchronized (syncObj) {
			Uri uri = android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI;
			// 列不在从PC端获取，保留获取列数代码只是为了与以前代码兼容
			int columnCount = cxt.getByteReader().ReadInt32();
			String[] columns = null;
			if (columnCount > 0) {
				columns = new String[columnCount + 1];
				for (int i = 0; i < columnCount; i++) {
					columns[i] = cxt.getByteReader().ReadString();
				}
			}
			/* 以上读取列的代码实际上不使用 */
			columns = new String[] {"_id", "_data", "bucket_id", "bucket_display_name"};
			String where = cxt.getByteReader().ReadString();
			String order = cxt.getByteReader().ReadString();

			ContentResolver resolver = context.getContentResolver();
			Cursor cursor = resolver.query(uri, columns, where, null, order);
			if (cursor != null) {
				List<ImageInfo> exitPathImage = new ArrayList<ImageInfo>();
				if (cursor.moveToFirst()) {
					do {
						if (cxt.isCancel())
							break;
						String path = cursor.getString(1);
						if ((new File(path)).exists()) {
							ImageInfo imageInfo = new ImageInfo();
							imageInfo._id = cursor.getInt(0);
							imageInfo._data = cursor.getBlob(1);
							imageInfo.bucket_id = cursor.getBlob(2);
							imageInfo.bucket_display_name = cursor.getBlob(3);
							exitPathImage.add(imageInfo);
						}
					}
					while (cursor.moveToNext());
				}
				cursor.close();
				cxt.getByteWriter().Write(exitPathImage.size());
				for (ImageInfo imageInfo : exitPathImage) {
					cxt.getByteWriter().Write(imageInfo._id);
					cxt.getByteWriter().WriteBlob(imageInfo._data);
					cxt.getByteWriter().WriteBlob(imageInfo.bucket_id);
					cxt.getByteWriter().WriteBlob(imageInfo.bucket_display_name);
				}
			}
		}
	}

	class ImageInfo {
		int _id;
		byte[] _data;
		byte[] bucket_id;
		byte[] bucket_display_name;
	}

	private void queryThumbnails(ProviderExecuteContext cxt) {
		synchronized (syncObj) {
			Uri uri = Thumbnails.EXTERNAL_CONTENT_URI;
			int columnCount = cxt.getByteReader().ReadInt32();
			String[] columns = null;
			if (columnCount > 0) {
				columns = new String[columnCount];
				for (int i = 0; i < columnCount; i++) {
					columns[i] = cxt.getByteReader().ReadString();
				}
			}
			String where = cxt.getByteReader().ReadString();
			String order = cxt.getByteReader().ReadString();

			ContentResolver resolver = context.getContentResolver();
			Cursor cursor = resolver.query(uri, columns, where, null, order);
			if (cursor != null) {
				cxt.getByteWriter().Write(cursor.getCount());
				if (cursor.moveToFirst()) {
					do {
						if (cxt.isCancel())
							break;
						cxt.getByteWriter().Write(cursor.getInt(0));
						cxt.getByteWriter().WriteBlob(cursor.getBlob(1));
						cxt.getByteWriter().Write(cursor.getInt(2));
						cxt.getByteWriter().Write(cursor.getInt(3));
						cxt.getByteWriter().Write(cursor.getInt(4));
						cxt.getByteWriter().Write(cursor.getInt(5));
					}
					while (cursor.moveToNext());
				}
				cursor.close();
			}
		}
	}

	private void execBuckets(ProviderExecuteContext cxt) {
		boolean result = true;

		int count = cxt.getByteReader().ReadInt32();
		if (count > 0) {
			for (int i = 0; i < count; i++) {
				byte state = cxt.getByteReader().ReadByte();
				PhotoBucket bucket = new PhotoBucket();
				bucket.Read(cxt.getByteReader());

				if (state == 4 && ((new File(bucket.getPath()).exists()))) {
					if (new File(bucket.getPath()).exists()) {
						ConfigManager.putPhotoBucket(this.context, bucket);
					}
					else {
						result = false;
						break;
					}
				}
				else if (state == 8) {
					// 删除图片分类
					ContentResolver resolver = context.getContentResolver();
					String where = new StringBuilder().append("bucket_id='").append(bucket.getBucketId()).append("'").toString();
					String[] columns = new String[] {android.provider.MediaStore.Images.Media._ID, android.provider.MediaStore.Images.Media.DATA};
					Cursor cursor = resolver.query(android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI, columns, where, null, null);
					if (cursor != null) {
						if (cursor.moveToFirst()) {
							do {
								Uri uri = ContentUris.withAppendedId(android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI, cursor.getInt(0));
								resolver.delete(uri, null, null);

								/*
								 * 一般情况下上一语句删除数据库中数据分自动删除图片文件 但汉王pad删除数据库数据时，不删除文件 所以做如下操作
								 */
								try {
									String imagePath = cursor.getString(1);
									File file = new File(imagePath);
									if (file.exists()) {
										file.delete();
									}
								}
								catch (Exception e) {
									e.printStackTrace();
								}
							}
							while (cursor.moveToNext());
						}
						cursor.close();
					}

					ConfigManager.deletePhotoBucket(this.context, bucket.getBucketId());
				}
				else if (state == 16) {
					// 修改图片分类
					ContentResolver resolver = context.getContentResolver();
					ContentValues values = new ContentValues();
					values.put("BUCKET_display_name", bucket.getDisplayName());
					String where = new StringBuilder().append("bucket_id='").append(bucket.getBucketId()).append("'").toString();
					Cursor cursor = resolver.query(android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI, new String[] {android.provider.MediaStore.Images.Media._ID}, where, null, null);
					if (cursor != null) {
						if (cursor.moveToFirst()) {
							do {
								Uri uri = ContentUris.withAppendedId(android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI, cursor.getInt(0));
								resolver.update(uri, values, null, null);
							}
							while (cursor.moveToNext());
						}
						cursor.close();
					}

					ConfigManager.putPhotoBucket(this.context, bucket);
				}
			}
		}

		if (result)
			cxt.getByteWriter().WriteString("OKAY");
		else
			cxt.getByteWriter().WriteString("FAIL");
	}

	private void execPhotos(ProviderExecuteContext cxt) {
		int count = cxt.getByteReader().ReadInt32();
		if (count > 0) {
			ContentResolver resolver = context.getContentResolver();
			for (int i = 0; i < count; i++) {
				int state = cxt.getByteReader().ReadByte();
				if (state == 4) {
					String data = cxt.getByteReader().ReadString();
					String mime = cxt.getByteReader().ReadString();
					String bucketId = cxt.getByteReader().ReadString();
					String bucketName = cxt.getByteReader().ReadString();
					Uri uri = addImage(data, mime, bucketId, bucketName);
					Cursor cursor = resolver.query(uri, new String[] {"_id", "_data", "bucket_id", "bucket_display_name"}, null, null, null);
					if (cursor != null) {
						if (cursor.moveToFirst()) {
							do {
								int photoId = cursor.getInt(0);
								String newBucketId = cursor.getString(2);
								String newBucketDisplay = cursor.getString(3);
								if (!newBucketId.equals(bucketId)) {
									editImage(photoId, bucketId, bucketName);
									newBucketId = bucketId;
									newBucketDisplay = bucketName;
								}
								cxt.getByteWriter().Write(photoId);
								cxt.getByteWriter().WriteBlob(cursor.getBlob(1));
								cxt.getByteWriter().Write(newBucketId);
								cxt.getByteWriter().Write(newBucketDisplay);
							}
							while (cursor.moveToNext());
						}
						cursor.close();
					}
				}
				else if (state == 8) {
					int photoId = cxt.getByteReader().ReadInt32();
					DeletePhotoFile(photoId);
					
					Uri uri = ContentUris.withAppendedId(android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI, photoId);
					resolver.delete(uri, null, null);
					cxt.getByteWriter().WriteString("OKAY");
				}
				else if (state == 16) {
					int photoId = cxt.getByteReader().ReadInt32();
					String bucketId = cxt.getByteReader().ReadString();
					String bucketName = cxt.getByteReader().ReadString();
					editImage(photoId, bucketId, bucketName);
					cxt.getByteWriter().WriteString("OKAY");
				}
			}
		}
	}
	
	private void DeletePhotoFile(int id) {
		Uri uri = android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI;
		String[] columns = new String[] {"_data"};
		ContentResolver resolver = context.getContentResolver();
		String where = "_id=" + id;
		Cursor cur = null;

		try {
			cur = resolver.query(uri, columns, where, null, null);
			if (cur != null && cur.moveToFirst()) {
				String filePath = cur.getString(0);
				File f = new File(filePath);
				if (f.exists())
					f.delete();
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

	private Uri addImage(String data, String mime, String bucketId, String bucketName) {
		boolean update=false;
		Uri uri = android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI;
		String[] columns  = new String[] {"_id","bucket_id", "bucket_display_name"};
		ContentResolver resolver = context.getContentResolver();
		Cursor cursor = resolver.query(uri, columns, " _data = \"" +data+"\"", null, null);
		if (cursor != null) {
			if (cursor.moveToFirst()) {
				int id=cursor.getInt(0);
				uri=ContentUris.withAppendedId(uri, id);
				String bucket_id = cursor.getString(1);
				String bucket_name = cursor.getString(2);
				if((bucket_id.equals(bucketId)&&bucket_name.equals(bucketName))){
					return uri;
				}
				update=true; 
			}
			cursor.close();
		}
		resolver = context.getContentResolver();
		ContentValues values = new ContentValues();
		values.put(android.provider.MediaStore.Images.Media.DATA, data);
		values.put(android.provider.MediaStore.Images.Media.MIME_TYPE, mime);
		values.put("BUCKET_id", bucketId);
		values.put("BUCKET_display_name", bucketName);
		
		try {
			File f = new File(data);
			if (f.exists())
				values.put(android.provider.MediaStore.Images.Media.SIZE, f.length());
		}
		catch (Exception e) {

		}
		if(update){
		    resolver.update(uri, values,null,null);
		}else{
		    uri = resolver.insert(android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI, values);
			if (uri != null) {
				StoreThumbnail(ContentUris.parseId(uri), data);
			}
		}
		return uri;
	}

	private void editImage(int photoId, String bucketId, String bucketName) {
		Uri uri = ContentUris.withAppendedId(android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI, photoId);
		ContentValues values = new ContentValues();
		values.put("BUCKET_id", bucketId);
		values.put("BUCKET_display_name", bucketName);
		context.getContentResolver().update(uri, values, null, null);
	}

	private void setWallPaperByRow(ProviderExecuteContext cxt) {
		ContentResolver resolver = context.getContentResolver();
		int photoId = cxt.getByteReader().ReadInt32();
		Uri uri = ContentUris.withAppendedId(android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI, photoId);
		try {
			InputStream stream = resolver.openInputStream(uri);
			context.setWallpaper(stream);
			stream.close();
			cxt.getByteWriter().WriteString("OKAY");
			return;
		}
		catch (FileNotFoundException e) {
			LogCenter.error(e.getMessage(), "");
		}
		catch (IOException e) {
			LogCenter.error(e.getMessage(), "");
		}
		cxt.getByteWriter().WriteString("FAIL");
	}

	private void getWallPaper(ProviderExecuteContext cxt) {
		BitmapDrawable drawable = (BitmapDrawable) context.getWallpaper();
		Bitmap image = drawable.getBitmap();
		if (android.os.Environment.getExternalStorageState().equals(android.os.Environment.MEDIA_MOUNTED)) {
			writeBitmap1(image, cxt.getByteWriter());
		}
		else {
			writeBitmap2(image, cxt.getByteWriter());
		}
	}

	private void writeBitmap1(Bitmap image, ByteWriter writer) {
		try {
			String temp = "/sdcard/tmp~wallpaper~";
			FileOutputStream outStream = new FileOutputStream(temp);
			image.compress(CompressFormat.JPEG, 100, outStream);
			writer.WriteString("OKAY");
			writer.Write(temp);
		}
		catch (FileNotFoundException e) {
			LogCenter.error("WriteBitmap1 error:", e.getMessage());
			writer.WriteString("FAIL");
		}
	}

	private void writeBitmap2(Bitmap image, ByteWriter writer) {
		try {
			ByteArrayOutputStream stream = new ByteArrayOutputStream();
			image.compress(CompressFormat.PNG, 0, stream);
			writer.WriteString("OKAY");
			writer.Write(stream.size());
			writer.Write(stream.toByteArray());
			stream.close();
		}
		catch (FileNotFoundException e) {
			LogCenter.error("WriteBitmap2 error:", e.getMessage());
			writer.WriteString("FAIL");
		}
		catch (IOException e) {
			LogCenter.error("WriteBitmap2 error:", e.getMessage());
			writer.WriteString("FAIL");
		}
	}

	private void setWallPaper(ProviderExecuteContext cxt) {
		int lenth = cxt.getByteReader().ReadInt32();
		byte[] buffer = cxt.getByteReader().ReadBytes(lenth);

		boolean blnSuccess = applyWallpaper(context, buffer);

		if (blnSuccess) {
			cxt.getByteWriter().WriteString("OKAY");
		}
		else {
			cxt.getByteWriter().WriteString("FAIL");
		}
	}

	private void setWallPaperByPath(ProviderExecuteContext cxt) {
		String path = cxt.getByteReader().ReadString();

		boolean blnSuccess = applyWallpaper(context, path);
		if (blnSuccess) {
			cxt.getByteWriter().WriteString("OKAY");
		}
		else {
			cxt.getByteWriter().WriteString("FAIL");
		}
	}

	private boolean applyWallpaper(Context ctx, byte[] buffer) {
		try {
			Bitmap bitmap = BitmapFactory.decodeByteArray(buffer, 0, buffer.length/* , options */);
			if (null != bitmap) {
				context.setWallpaper(bitmap);
				return true;
			}
		}
		catch (IOException e) {
			e.printStackTrace();
		}

		return false;
	}

	public boolean applyWallpaper(final Context ctx, final String wallpaperFileFullName) {
		try {
			Bitmap bitmap = BitmapFactory.decodeFile(wallpaperFileFullName/* , options */);
			if (null != bitmap) {
				context.setWallpaper(bitmap);
				return true;
			}
		}
		catch (Exception e) {
			e.printStackTrace();
		}

		return false;
	}

	/**
	 * 根据传入的图像路径返回图像的宽高，如果path == null 或者 路径不存在 返回宽高 为-1
	 * 
	 * @param path
	 *            图像的路径
	 * @return 图像的宽高数组
	 */
	public static int[] getImageWH(String path) {
		int[] wh = {-1, -1};
		if (path == null) {
			return wh;
		}
		File file = new File(path);
		if (file.exists() && !file.isDirectory()) {
			try {
				BitmapFactory.Options options = new BitmapFactory.Options();
				options.inJustDecodeBounds = true;
				InputStream is = new FileInputStream(path);
				BitmapFactory.decodeStream(is, null, options);
				wh[0] = options.outWidth;
				wh[1] = options.outHeight;
			}
			catch (Throwable e) {
			}
		}
		return wh;
	}

	/**
	 * 将已有的图像缩放为给定的newWidth宽和newHeight高， 如果原图像的宽高比例和期望的宽高比例不一致， 则切取符合期望比例的图像，切取的图像为原图像中心部分。 切取后将图像缩放为给定宽高，并返回。
	 * 
	 * @param bitmapOrg
	 *            原图像
	 * @param newWidth
	 *            目标图像的宽
	 * @param newHeight
	 *            目标图像的高
	 * @return 返回为宽为newWidth，高为newHeight的图像，如果原图像为null,则返回null
	 */
	public static Bitmap toSizeBitmap(Bitmap bitmapOrg, int newWidth, int newHeight) {
		if (null == bitmapOrg) {
			return null;
		}

		// 获取这个图片的宽和高
		int w = bitmapOrg.getWidth();
		int h = bitmapOrg.getHeight();

		int x, y = 0;

		int wTemp = newWidth * h / newHeight;
		if (wTemp > w) {
			// 以宽度
			h = newHeight * w / newWidth;
			x = 0;
			y = (bitmapOrg.getHeight() - h) / 2;
		}
		else {
			w = wTemp;
			y = 0;
			x = (bitmapOrg.getWidth() - wTemp) / 2;
		}

		float scaleWidth, scaleHeight = 0;

		// 创建操作图片用的matrix对象
		Matrix matrix = new Matrix();
		Bitmap resizedBitmap;

		// 将整个头像按比例缩放绘制到屏幕中
		// 计算缩放率，新尺寸除原始尺寸
		scaleWidth = ((float) newWidth) / w;
		scaleHeight = ((float) newHeight) / h;

		// 缩放图片动作
		matrix.postScale(scaleWidth, scaleHeight);

		// 创建新的图片
		resizedBitmap = Bitmap.createBitmap(bitmapOrg, x, y, w, h, matrix, true);
		return resizedBitmap;
	}

	private void clearWallPaper(ProviderExecuteContext cxt) {
		try {
			context.clearWallpaper();
			cxt.getByteWriter().WriteString("OKAY");
		}
		catch (IOException e) {
			LogCenter.error("Clear wallpaper error: ", e.getMessage());
		}
	}

	private void addContactPhoto(ProviderExecuteContext cxt) {
		String data = cxt.getByteReader().ReadString();
		String mime = cxt.getByteReader().ReadString();
		String bucketId = cxt.getByteReader().ReadString();
		String bucketName = cxt.getByteReader().ReadString();

		Uri uri = addImage(data, mime, bucketId, bucketName);
		if (uri != null) {
			cxt.getByteWriter().Write(uri.toString());
		}
	}

	private void getTotalSize(ProviderExecuteContext cxt) {
		Cursor cursor = context.getContentResolver().query(android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI, new String[] {"sum(_size)"}, null, null, null);
		if (cursor != null) {
			if (cursor.moveToFirst()) {
				long size = cursor.getLong(0);
				cxt.getByteWriter().Write(size);
			}
			else {
				cxt.getByteWriter().Write(0L);
			}
			cursor.close();
		}
		else {
			cxt.getByteWriter().Write(0L);
		}
	}

	private int getPhotoCount() {
		Cursor cursor = context.getContentResolver().query(android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI, new String[] {Media.DATA}, null, null, null);

		if (cursor != null) {
			int cnt = 0;
			if (cursor.moveToFirst()) {
				do {
					String path = cursor.getString(0);
					if ((new File(path)).exists()) {
						cnt++;
					}
				}
				while (cursor.moveToNext());
			}
			cursor.close();
			return cnt;
		}
		return 0;
	}

	private void StoreThumbnail(long id, String path) {
		long size = 0;
		File f = new File(path);
		if (!f.exists()) {
			return;
		}

		Cursor cursor = null;
		Bitmap bm = null;
		try {
			cursor = context.getContentResolver().query(Images.Thumbnails.EXTERNAL_CONTENT_URI, null, Images.Thumbnails.IMAGE_ID + "=" + id, null, null);
			if (cursor.getCount() > 0) {
				cursor.close();
				return;
			}
			size = f.length();
			if (size < 30 * 1024 * 1024) {
				BitmapFactory.Options option = new BitmapFactory.Options();
				option.inJustDecodeBounds = true;
				bm = BitmapFactory.decodeFile(path, option);
				if (option.outHeight > 100 && option.outWidth > 100) {
					int scaleHeight = option.outHeight / 100;
					int scaleWidth = option.outWidth / 100;
					option.inSampleSize = (scaleHeight < scaleWidth ? scaleHeight : scaleWidth);
				}
				option.inJustDecodeBounds = false;
				bm = BitmapFactory.decodeFile(path, option);
				ContentValues values = new ContentValues(4);
				values.put(Images.Thumbnails.KIND, Images.Thumbnails.MICRO_KIND);
				values.put(Images.Thumbnails.IMAGE_ID, (int) id);
				values.put(Images.Thumbnails.HEIGHT, bm.getHeight());
				values.put(Images.Thumbnails.WIDTH, bm.getWidth());
				Uri url = context.getContentResolver().insert(Images.Thumbnails.EXTERNAL_CONTENT_URI, values);

				OutputStream thumbOut = context.getContentResolver().openOutputStream(url);
				bm.compress(Bitmap.CompressFormat.JPEG, 100, thumbOut);
				thumbOut.close();
			}
		}
		catch (Exception ex) {
			ex.printStackTrace();
		}
		finally {
			if (bm != null) {
				bm.recycle();
			}
			if (cursor != null) {
				cursor.close();
			}

		}
	}

}
