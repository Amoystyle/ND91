package daemon.provider.media;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;

import daemon.common.Common;
import daemon.logger.LogCenter;
import daemon.ndk.NdkManager;
import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;
import daemon.provider.IProvider;
import daemon.provider.ProviderExecuteContext;
import daemon.provider.file.FileOperater;
import daemon.util.ConfigManager;

import android.content.ContentResolver;
import android.content.ContentUris;
import android.content.ContentValues;
import android.content.Context;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Bitmap.CompressFormat;
import android.graphics.Matrix;
import android.graphics.drawable.BitmapDrawable;
import android.net.Uri;
import android.os.Environment;
import android.provider.MediaStore.Images;
import android.provider.MediaStore.Audio.Media;
import android.provider.MediaStore.Images.Thumbnails;
import android.text.TextUtils;

public class PhotoProvider implements IProvider {

	private String TAG = this.getClass().getName();	

	static Object syncObj = new Object();
	private Context context;

	public PhotoProvider(Context context) {
		this.context = context;
	}
	
	private enum PhotoType{
	    WallPaper,
	    UnWallPaper,
	    AllPhoto
	};

	@Override
	public int getBusiness() {
		return 211;
	}

	@Override
	public void execute(ProviderExecuteContext cxt) {
		ByteReader reader = cxt.getByteReader();
		int action = reader.ReadInt32();
		switch (action) {
			case 1:
				queryBucket(cxt);
				break;
			case 2:
				queryPhotos(cxt, false);
				break;
			case 3:
				queryThumbnails(cxt);
				break;
			case 4:
				addBuckets(cxt);
				break;
			case 5:
				addPhotos(cxt);
				break;
			case 6:
				addContactPhoto(cxt);
				break;
			case 7:
				modifyBuckets(cxt);
				break;
			case 8:
				movePhotos(cxt);
				break;
			case 9:
				deleteBuckets(cxt);
				break;
			case 10:
				deletePhotos(cxt);
				break;
			case 11:
				queryPhotos(cxt, true);
				break;
			case 12:
				addPhotosExif(cxt);
				break;
			case 13:
				queryPhotosEx(cxt, PhotoType.AllPhoto);
				break;
			case 14:
				//addPhotosEx(cxt);  //ExitInterface要求2.0固件，该接口暂时没用，先屏蔽
				break;
			case 16:
				getWallPaper(cxt);
				break;
			case 17:
				setWallPaperByRow(cxt);
				break;
			case 18:
				setWallPaperByPath(cxt);
				break;
			case 19:
				setWallPaper(cxt);
				break;
			case 20:
				clearWallPaper(cxt);
				break;
			case 21:
			    queryPhotosEx(cxt, PhotoType.WallPaper);// 壁纸
			    break;
			case 22:
                queryPhotosEx(cxt, PhotoType.UnWallPaper);// 非壁纸
                break;
			case 26:
				getTotalSize(cxt, PhotoType.AllPhoto);
				break;
			case 27:
				int cnt = getPhotoCount(true);
				cxt.getByteWriter().Write(cnt);
				break;
			case 28:
				context.sendBroadcast(new Intent(Intent.ACTION_MEDIA_MOUNTED, Uri.parse("file://" + Environment.getExternalStorageDirectory())));
				break;
			case 29:
				createThumbnail(cxt.getByteReader(), cxt.getByteWriter());
				break;
			case 30:
				getTotalSize(cxt, PhotoType.UnWallPaper);
				break;
			case 31:
				int cntWidthoutWallpaper = getPhotoCount(false);
				cxt.getByteWriter().Write(cntWidthoutWallpaper);
				break;
			case 32:
			    getTotalSize(cxt, PhotoType.WallPaper); //获取壁纸的size和count
			    break;
			case 33:
				addPhotosEx(cxt);
				break;
		}
	}

//ExitInterface要求2.0固件，该接口暂时没用，先屏蔽
//	private void addPhotosEx(ProviderExecuteContext cxt) {
//		boolean isSuccess = false;
//		int id = 0;
//		long dateModified = 0;
//		long dateCreate = 0;
//
//		try {
//			String data = cxt.getByteReader().ReadString();
//			String mime = cxt.getByteReader().ReadString();
//			String bucketId = cxt.getByteReader().ReadString();
//			String bucketName = cxt.getByteReader().ReadString();
//			double latitude = cxt.getByteReader().ReadDouble();
//			double longitude = cxt.getByteReader().ReadDouble();
//
//			File f = new File(data);
//			if (f.exists()) {
//				ExifInterface exif = new ExifInterface(data);
//				String datetime=exif.getAttribute(ExifInterface.TAG_DATETIME);
//				dateCreate = getTime(datetime);
//				dateModified = f.lastModified() / 1000;
//				long time = dateCreate;
//				if(time==0){
//					time = System.currentTimeMillis() / 1000;
//					dateCreate = dateModified;
//				}
//				Uri uri = addImage(data, mime, bucketId, bucketName, latitude, longitude, f.length(), dateModified,time);
//				if (uri != null) {
//					isSuccess = true;
//					id = (int) ContentUris.parseId(uri);
//				}
//			}
//		}
//		catch (Exception e) {
//			e.printStackTrace();
//		}
//
//		if (isSuccess) {
//			cxt.getByteWriter().Write(1);
//			cxt.getByteWriter().Write(id);
//			cxt.getByteWriter().Write(dateCreate);
//			cxt.getByteWriter().Write(dateModified);
//		}
//		else {
//			cxt.getByteWriter().Write(0);
//		}
//		
//	}
	
//	private  long getTime(String user_time) {
//		if(user_time!=null&&!"".equals(user_time)){
//			SimpleDateFormat sdf = new SimpleDateFormat("yyyy:MM:dd HH:mm:ss");
//			Date d;
//			try {
//				d = sdf.parse(user_time);
//				return  d.getTime()/1000;
//			} catch (ParseException e) {
//
//			}
//		}
//		return 0;
//	}

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

	private void queryPhotos(ProviderExecuteContext cxt, boolean withSize) {
		synchronized (syncObj) {
			Uri uri = android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI;
			// 列不在从PC端获取，保留获取列数代码只是为了与以前代码兼容
			String[] columns = null;
			/* 以上读取列的代码实际上不使用 */
			columns = new String[] {"_id", "_data", "bucket_id", "bucket_display_name", Media.DATE_ADDED, Media.DATE_MODIFIED, "latitude", "longitude", Media.SIZE};
			// String where = cxt.getByteReader().ReadString();
			String order = cxt.getByteReader().ReadString();

			ContentResolver resolver = context.getContentResolver();
			Cursor cursor = resolver.query(uri, columns, null, null, order);
			if (cursor != null) {
				List<ImageInfo> exitPathImage = new ArrayList<ImageInfo>();
				if (cursor.moveToFirst()) {
					do {
						if (cxt.isCancel())
							break;
						String path = cursor.getString(1);
						File f = new File(path);
						if (f.exists()) {
							ImageInfo imageInfo = new ImageInfo();
							imageInfo._id = cursor.getInt(0);
							imageInfo._data = cursor.getBlob(1);
							imageInfo.bucket_id = cursor.getBlob(2);
							imageInfo.bucket_display_name = cursor.getBlob(3);
							imageInfo.latitude = cursor.getDouble(6);
							imageInfo.longitude = cursor.getDouble(7);
							imageInfo.size = cursor.getLong(8);

							long add = cursor.getLong(4);
							long modifed = cursor.getLong(5);
							if (modifed != 0) {
								imageInfo.modifyDate = modifed;
							}
							else if (add != 0) {
								imageInfo.modifyDate = add;
							}
							else {
								imageInfo.modifyDate = f.lastModified() / 1000;
							}

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
					cxt.getByteWriter().Write(imageInfo.modifyDate);
					cxt.getByteWriter().Write(imageInfo.latitude);
					cxt.getByteWriter().Write(imageInfo.longitude);

					if (withSize)
						cxt.getByteWriter().Write(imageInfo.size);
				}
			}
		}
	}

	//0 搜索全部，1搜索壁纸，2搜索照相机，3搜索图库
	private void queryPhotosEx(ProviderExecuteContext cxt, PhotoType type) {
		synchronized (syncObj) {
			Uri uri = android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI;
			// 列不在从PC端获取，保留获取列数代码只是为了与以前代码兼容
			String[] columns = null;
			/* 以上读取列的代码实际上不使用 */
			File storageDirectory = Environment.getExternalStorageDirectory();
			String strSdcard = "";
			String where = "1 = 1 ";
			if (storageDirectory != null) { 
			    
			    strSdcard = storageDirectory.getPath().toLowerCase();
			    String exStrSdcard = Common.getExternalSDCardPath(context); //外置sd卡
			    String strWallPaperEx = "";
			    String strUnWallPaperEx = "";
			    if(!TextUtils.isEmpty(exStrSdcard)){
			        strWallPaperEx =  " or LOWER(_data) LIKE '" + exStrSdcard + "/wallpaper/%'";
			        strUnWallPaperEx = " and LOWER(_data) not LIKE '" + exStrSdcard + "/wallpaper/%'";
			    }
			    if(type == PhotoType.WallPaper){//壁纸
			        where = "LOWER(_data) LIKE '" + strSdcard + "/wallpaper/%' or LOWER(_data) LIKE '/sdcard/wallpper/%'" + strWallPaperEx;
			    }else if(type == PhotoType.UnWallPaper){//非壁纸
			        where = "LOWER(_data) not LIKE '" + strSdcard + "/wallpaper/%' and LOWER(_data) not LIKE '/sdcard/wallpper/%'" + strUnWallPaperEx;
			    }
            }
			
			columns = new String[] {"_id", "_data", "bucket_id", "bucket_display_name", Media.DATE_ADDED, Media.DATE_MODIFIED, "latitude", "longitude", Media.SIZE, "orientation"};
			// String where = cxt.getByteReader().ReadString();
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
						File f = new File(path);
						if (f.exists()) {
							ImageInfo imageInfo = new ImageInfo();
							imageInfo._id = cursor.getInt(0);
							imageInfo._data = cursor.getBlob(1);
							imageInfo.bucket_id = cursor.getBlob(2);
							imageInfo.bucket_display_name = cursor.getBlob(3);
							imageInfo.addDate = cursor.getLong(4);
							imageInfo.modifyDate = cursor.getLong(5);
							imageInfo.latitude = cursor.getDouble(6);
							imageInfo.longitude = cursor.getDouble(7);
							imageInfo.size = cursor.getLong(8);
							imageInfo.orientation = cursor.getInt(9);

							if (imageInfo.modifyDate == 0) {
								imageInfo.modifyDate = f.lastModified() / 1000;
							}
							//String filePath = new String(imageInfo._data, 0, imageInfo._data.length - 1);
							
							/*if(filePath.contains("/wallpaper"))
							{
							    exitPathImage.add(imageInfo);
							}*/
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
					cxt.getByteWriter().Write(imageInfo.modifyDate);
					cxt.getByteWriter().Write(imageInfo.latitude);
					cxt.getByteWriter().Write(imageInfo.longitude);
					cxt.getByteWriter().Write(imageInfo.size);
					cxt.getByteWriter().Write(imageInfo.orientation);
					cxt.getByteWriter().Write(imageInfo.addDate);
				}
			}
		}
	}

	class ImageInfo {
		int _id;
		double latitude, longitude;
		long modifyDate, addDate;
		long size;
		int orientation;
		byte[] _data;
		byte[] bucket_id;
		byte[] bucket_display_name;
	}

	private void queryThumbnails(ProviderExecuteContext cxt) {
		synchronized (syncObj) {
			Uri uri = Thumbnails.EXTERNAL_CONTENT_URI;
			ContentResolver resolver = context.getContentResolver();
			Cursor cursor = resolver.query(uri, null, null, null, null);
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

	// 添加
	private void addBuckets(ProviderExecuteContext cxt) {
		int count = cxt.getByteReader().ReadInt32();
		if (count > 0) {
			for (int i = 0; i < count; i++) {
				PhotoBucket bucket = new PhotoBucket();
				bucket.Read(cxt.getByteReader());
				if (new File(bucket.getPath()).exists()) {
					ConfigManager.putPhotoBucket(this.context, bucket);
				}
			}
		}

		cxt.getByteWriter().Write(1);
	}

	private void deleteBuckets(ProviderExecuteContext cxt) {
		int count = cxt.getByteReader().ReadInt32();
		if (count > 0) {
			for (int i = 0; i < count; i++) {
				PhotoBucket bucket = new PhotoBucket();
				bucket.Read(cxt.getByteReader());
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
		}

		cxt.getByteWriter().Write(1);
	}

	private void modifyBuckets(ProviderExecuteContext cxt) {
		int count = cxt.getByteReader().ReadInt32();
		if (count > 0) {
			for (int i = 0; i < count; i++) {
				PhotoBucket bucket = new PhotoBucket();
				bucket.Read(cxt.getByteReader());
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

		cxt.getByteWriter().Write(1);
	}

	private void addPhotos(ProviderExecuteContext cxt) {
		int count = cxt.getByteReader().ReadInt32();
		if (count > 0) {
			for (int i = 0; i < count; i++) {
				String data = cxt.getByteReader().ReadString();
				String mime = cxt.getByteReader().ReadString();
				String bucketId = cxt.getByteReader().ReadString();
				String bucketName = cxt.getByteReader().ReadString();
				Uri uri = addImage(data, mime, bucketId, bucketName);

				if (uri != null) {
					int photoId = (int) ContentUris.parseId(uri);
					cxt.getByteWriter().Write(photoId);
					cxt.getByteWriter().Write(data);
					cxt.getByteWriter().Write(bucketId);
					cxt.getByteWriter().Write(bucketName);
				}
			}
		}
	}
	
	// 还原图片时使用，从上层传下来时间
	private void addPhotosEx(ProviderExecuteContext cxt) {
		boolean isSuccess = false;
		int id = 0;
		long dateModified = 0;

		try {
			String data = cxt.getByteReader().ReadString();
			String mime = cxt.getByteReader().ReadString();
			String bucketId = cxt.getByteReader().ReadString();
			String bucketName = cxt.getByteReader().ReadString();
			double latitude = cxt.getByteReader().ReadDouble();
			double longitude = cxt.getByteReader().ReadDouble();
			long dateAdd = cxt.getByteReader().ReadInt64(); 
			dateModified = cxt.getByteReader().ReadInt64(); 

			File f = new File(data);
			if (f.exists()) {
				Uri uri = addImage(data, mime, bucketId, bucketName, latitude, longitude, f.length(), dateModified,dateAdd);
				if (uri != null) {
					isSuccess = true;
					id = (int) ContentUris.parseId(uri);
				}
			}
		}
		catch (Exception e) {
			e.printStackTrace();
		}

		if (isSuccess) {
			cxt.getByteWriter().Write(1);
			cxt.getByteWriter().Write(id);
			cxt.getByteWriter().Write(dateModified);
		}
		else {
			cxt.getByteWriter().Write(0);
		}
	}

	// 添加图片，包含经纬度，时间等扩展信息
	private void addPhotosExif(ProviderExecuteContext cxt) {
		boolean isSuccess = false;
		int id = 0;
		long dateModified = 0;

		try {
			String data = cxt.getByteReader().ReadString();
			String mime = cxt.getByteReader().ReadString();
			String bucketId = cxt.getByteReader().ReadString();
			String bucketName = cxt.getByteReader().ReadString();
			double latitude = cxt.getByteReader().ReadDouble();
			double longitude = cxt.getByteReader().ReadDouble();

			File f = new File(data);
			if (f.exists()) {
				dateModified = f.lastModified() / 1000;
				Uri uri = addImage(data, mime, bucketId, bucketName, latitude, longitude, f.length(), dateModified);
				if (uri != null) {
					isSuccess = true;
					id = (int) ContentUris.parseId(uri);
				}
			}
		}
		catch (Exception e) {
			e.printStackTrace();
		}

		if (isSuccess) {
			cxt.getByteWriter().Write(1);
			cxt.getByteWriter().Write(id);
			cxt.getByteWriter().Write(dateModified);
		}
		else {
			cxt.getByteWriter().Write(0);
		}
	}

	private void deletePhotos(ProviderExecuteContext cxt) {
		int count = cxt.getByteReader().ReadInt32();
		if (count > 0) {
			ContentResolver resolver = context.getContentResolver();
			for (int i = 0; i < count; i++) {
				int photoId = cxt.getByteReader().ReadInt32();
				// 删除图片文件
				boolean result = DeletePhotoFile(photoId);

				// 删除数据库记录
				Uri uri = ContentUris.withAppendedId(android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI, photoId);
				resolver.delete(uri, null, null);
				if(result)
				    cxt.getByteWriter().Write(1);
				else
				    cxt.getByteWriter().Write(0);
			}
		}
	}

	private boolean DeletePhotoFile(int id) {
		Uri uri = android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI;
		String[] columns = new String[] {"_data"};
		ContentResolver resolver = context.getContentResolver();
		String where = "_id=" + id;
		Cursor cur = null;
		boolean result = false;
		
		try {
			cur = resolver.query(uri, columns, where, null, null);
			if (cur != null && cur.moveToFirst()) {
				String filePath = cur.getString(0);
				File f = new File(filePath);
				if (f.exists())
				    result =f.delete();
			}
		}
		catch (Exception e) {
			e.printStackTrace();
		}
		finally {
			if (cur != null)
				cur.close();
		}
		return result;
	}

	private void movePhotos(ProviderExecuteContext cxt) {
		int count = cxt.getByteReader().ReadInt32();
		boolean isSuccess = false;

		if (count > 0) {
			for (int i = 0; i < count; i++) {
				int photoId = cxt.getByteReader().ReadInt32();
				String bucketId = cxt.getByteReader().ReadString();
				String bucketName = cxt.getByteReader().ReadString();
				String oldPath = cxt.getByteReader().ReadString();
				String newPath = cxt.getByteReader().ReadString();

				byte[] rtnData = null;
				if (IsSameRoot(oldPath, newPath))
					rtnData = FileOperater.move(this.context, oldPath, newPath);
				else
					rtnData = FileOperater.copy(this.context, oldPath, newPath);

				if (isMoveFileSuccess(rtnData)) {
					isSuccess = true;
					editImage(photoId, bucketId, bucketName, newPath);
					//NdkManager.remove(this.context, oldPath);
					File oldFile = new File(oldPath);
					oldFile.delete();
				}
				else {
					isSuccess = false;
					break;
				}
			}
		}

		if (isSuccess)
			cxt.getByteWriter().Write(1);
		else
			cxt.getByteWriter().Write(0);
	}

	private boolean IsSameRoot(String path1, String path2) {
		String root1 = Common.getPathMount(path1);
		String root2 = Common.getPathMount(path2);

		if (root1 != null && root2 != null && root1.equals(root2))
			return true;

		return false;
	}

	private boolean isMoveFileSuccess(byte[] rtnData) {
		if (rtnData != null && rtnData.length == 4 && rtnData[0] == 1 && rtnData[1] == 0 && rtnData[2] == 0 && rtnData[3] == 0)
			return true;

		return false;
	}

	private Uri addImage(String data, String mime, String bucketId, String bucketName) {
		try {
			File f = new File(data);
			if (f.exists()) {
				return addImage(data, mime, bucketId, bucketName, 0, 0, f.length(), f.lastModified() / 1000);
			}
			else {
				return null;
			}
		}
		catch (Exception e) {
			e.printStackTrace();
		}

		return null;
	}

	private Uri addImage(String data, String mime, String bucketId, String bucketName, double latitude, double longitude, long fileSize, long dateModified) {
		return addImage( data,  mime,  bucketId,  bucketName,  latitude,  longitude,  fileSize,  dateModified,System.currentTimeMillis() / 1000);
	}
	
	private Uri addImage(String data, String mime, String bucketId, String bucketName, double latitude, double longitude, long fileSize, long dateModified,long dateAdd) {
		boolean update = false;
		Uri uri = android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI;
		String[] columns = new String[] {"_id", "bucket_id", "bucket_display_name", "latitude", "longitude", android.provider.MediaStore.Images.Media.SIZE, Media.DATE_MODIFIED};
		ContentResolver resolver = context.getContentResolver();
		Cursor cursor = resolver.query(uri, columns, " _data = \"" + data + "\"", null, null);
		if (cursor != null) {
			if (cursor.moveToFirst()) {
				int id = cursor.getInt(0);
				uri = ContentUris.withAppendedId(uri, id);
				String bucket_id = cursor.getString(1);
				String bucket_name = cursor.getString(2);
				double old_latitude = cursor.getDouble(3);
				double old_longitude = cursor.getDouble(4);
				long old_fileSize = cursor.getLong(5);
				long old_dateModified = cursor.getLong(6);
				if ((bucket_id.equals(bucketId) && bucket_name.equals(bucketName)) && old_latitude == latitude && old_longitude == longitude && old_fileSize == fileSize && old_dateModified == dateModified) {
					return uri;
				}
				update = true;
			}
			cursor.close();
		}
		resolver = context.getContentResolver();
		ContentValues values = new ContentValues();
		values.put(android.provider.MediaStore.Images.Media.DATA, data);
		values.put(android.provider.MediaStore.Images.Media.MIME_TYPE, mime);
		values.put("bucket_id", bucketId);
		values.put("bucket_display_name", bucketName);
		values.put("latitude", latitude);
		values.put("longitude", longitude);
		values.put(android.provider.MediaStore.Images.Media.SIZE, fileSize);
		values.put(Media.DATE_MODIFIED, dateModified);
		values.put(Media.DATE_ADDED, dateAdd);

		if (update) {
			resolver.update(uri, values, null, null);
		}
		else {
			try {
				uri = resolver.insert(android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI, values);
			}
			catch (Exception e) {
				uri = null;
				e.printStackTrace();
			}
		}

		// 生成缩略图
		if (uri != null)
			StoreThumbnail(ContentUris.parseId(uri), data);

		return uri;
	}

	private void editImage(int photoId, String bucketId, String bucketName, String newPath) {
		Uri uri = ContentUris.withAppendedId(android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI, photoId);
		ContentValues values = new ContentValues();
		values.put("BUCKET_id", bucketId);
		values.put("BUCKET_display_name", bucketName);
		values.put(android.provider.MediaStore.Images.Media.DATA, newPath);
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
			cxt.getByteWriter().Write(1);
			return;
		}
		catch (FileNotFoundException e) {
			LogCenter.error(e.getMessage(), "");
		}
		catch (IOException e) {
			LogCenter.error(e.getMessage(), "");
		}
		cxt.getByteWriter().Write(0);
	}

	private void getWallPaper(ProviderExecuteContext cxt) {
		BitmapDrawable drawable = (BitmapDrawable) context.getWallpaper();
		Bitmap image = drawable.getBitmap();
		// if (android.os.Environment.getExternalStorageState().equals(android.os.Environment.MEDIA_MOUNTED)) {
		// writeBitmap1(image, cxt.getByteWriter());
		// }
		// else {
		writeBitmap2(image, cxt.getByteWriter());
		// }
	}

	private void writeBitmap2(Bitmap image, ByteWriter writer) {
		try {
			ByteArrayOutputStream stream = new ByteArrayOutputStream();
			image.compress(CompressFormat.PNG, 0, stream);
			writer.Write(1);
			writer.Write(stream.size());
			writer.Write(stream.toByteArray());
			stream.close();
		}
		catch (Exception e) {
			LogCenter.error("WriteBitmap2 error:", e.getMessage());
			writer.Write(0);
		}
	}

	private void setWallPaper(ProviderExecuteContext cxt) {
		int lenth = cxt.getByteReader().ReadInt32();
		byte[] buffer = cxt.getByteReader().ReadBytes(lenth);

		boolean blnSuccess = applyWallpaper(buffer);;
		if (blnSuccess) {
			cxt.getByteWriter().Write(1);
		}
		else {
			cxt.getByteWriter().Write(0);
		}
	}

	private void setWallPaperByPath(ProviderExecuteContext cxt) {
		String path = cxt.getByteReader().ReadString();

		boolean blnSuccess = applyWallpaper(path);
		if (blnSuccess) {
			cxt.getByteWriter().Write(1);
		}
		else {
			cxt.getByteWriter().Write(0);
		}
	}

	private boolean applyWallpaper(byte[] buffer) {
		try {
			Bitmap bitmap = BitmapFactory.decodeByteArray(buffer, 0, buffer.length/* , options */);
			if (bitmap != null) {
				context.setWallpaper(bitmap);
				return true;
			}
		}
		catch (IOException e) {
			e.printStackTrace();
		}

		return false;
	}

	public boolean applyWallpaper(final String wallpaperFileFullName) {
		try {
			File f = new File(wallpaperFileFullName);
			if (!f.exists())
				return false;

			Bitmap bitmap = BitmapFactory.decodeFile(wallpaperFileFullName/* , options */);
			if (bitmap != null) {
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
			cxt.getByteWriter().Write(1);
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
	
	//壁纸比较少。 
	private void getTotalSize(ProviderExecuteContext cxt, PhotoType photoType) {
		Cursor cur = null;
		long size = 0L;
		int  count = 0; //用来计算壁纸的个数
		String where = null; 
		try {
			String[] columns = new String[] {Images.Media.DATA};
			if (photoType == PhotoType.WallPaper) {
			    File storageDirectory = Environment.getExternalStorageDirectory();
	            String strSdcard = "";
	            
	            String exStrSdcard = Common.getExternalSDCardPath(context); //外置sd卡
                String strWallPaperEx = "";
                if(!TextUtils.isEmpty(exStrSdcard)){
                    strWallPaperEx =  " or LOWER(_data) LIKE '" + exStrSdcard + "/wallpaper/%'";
                    //strUnWallPaperEx = " and LOWER(_data) not LIKE '" + exStrSdcard + "/wallpaper/%'";
                }
                
	            if (storageDirectory != null) { 
	                strSdcard = storageDirectory.getPath().toLowerCase();
	                where = "LOWER(_data) LIKE '" + strSdcard + "/wallpaper/%' or LOWER(_data) LIKE '/sdcard/wallpper/%'" + strWallPaperEx;
	            }
            }
			cur = context.getContentResolver().query(Images.Media.EXTERNAL_CONTENT_URI, columns, where, null, null);

			if (cur != null && cur.moveToFirst()) {
				while (!cur.isAfterLast()) {
					String path = cur.getString(0);

					if (Common.isSdcardPath(path)) {
						File f = new File(path);
						if (f.exists()) {
						    if(photoType == PhotoType.WallPaper || photoType == PhotoType.AllPhoto){
						        size += f.length();
						        count ++;
						    }
						    else if(photoType == PhotoType.UnWallPaper && !isWallPaper(path)){ //壁纸比较少在外面判断比较快
						        size += f.length();
						        count ++;
						    }
						}
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
				
		cxt.getByteWriter().Write(size);
		cxt.getByteWriter().Write(count);
	}

	private int getPhotoCount(boolean includeWallPaper) {
		Cursor cursor = context.getContentResolver().query(android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI, new String[] {Media.DATA}, null, null, null);

		if (cursor != null) {
			int cnt = 0;
			if (cursor.moveToFirst()) {
				do {
					String path = cursor.getString(0);
					if ((new File(path)).exists()) {
						if (includeWallPaper || !isWallPaper(path)) {
							cnt++;
						}
					}
				}
				while (cursor.moveToNext());
			}
			cursor.close();
			return cnt;
		}
		return 0;
	}

	private boolean isWallPaper(String path) {
	    String exStrSdcard = Common.getExternalSDCardPath(context) + "/wallpaper"; //外置sd卡
		String wallPaperFolder = Environment.getExternalStorageDirectory() + "/wallpaper/";
		boolean result = false;
		if(path.toLowerCase(Locale.getDefault()).startsWith(wallPaperFolder) || path.toLowerCase().startsWith("/sdcard/wallpaper/") || path.toLowerCase().startsWith(exStrSdcard))
		    result = true;
		return result;
	}

	private long StoreThumbnail(long id, String path) {
		long size = 0;
		File f = new File(path);
		if (!f.exists()) {
			return -1;
		}

		Uri thumbUri = null;
		Bitmap bm = null;
		Cursor cursor = null;
		String[] columns = new String[] {Images.Thumbnails._ID};
		try {
			cursor = context.getContentResolver().query(Images.Thumbnails.EXTERNAL_CONTENT_URI, columns, Images.Thumbnails.IMAGE_ID + "=" + id, null, null);
			if (cursor != null && cursor.moveToFirst()) {
				int thumbID = cursor.getInt(0);
				thumbUri = ContentUris.withAppendedId(Images.Thumbnails.EXTERNAL_CONTENT_URI, thumbID);
				cursor.close();
			}

			size = f.length();
			if (size < 30 * 1024 * 1024) {
				final int maxside = 200;
				BitmapFactory.Options option = new BitmapFactory.Options();
				option.inJustDecodeBounds = true;
				bm = BitmapFactory.decodeFile(path, option);
				if (option.outHeight > maxside && option.outWidth > maxside) {
					int scaleHeight = option.outHeight / maxside;
					int scaleWidth = option.outWidth / maxside;
					option.inSampleSize = Math.min(scaleHeight, scaleWidth);
				}
				option.inJustDecodeBounds = false;
				bm = BitmapFactory.decodeFile(path, option);
				ContentValues values = new ContentValues(4);

				values.put(Images.Thumbnails.KIND, Images.Thumbnails.MICRO_KIND);
				values.put(Images.Thumbnails.IMAGE_ID, (int) id);
				values.put(Images.Thumbnails.HEIGHT, bm.getHeight());
				values.put(Images.Thumbnails.WIDTH, bm.getWidth());

				if (thumbUri == null) {
					thumbUri = context.getContentResolver().insert(Images.Thumbnails.EXTERNAL_CONTENT_URI, values);
				}else{
					context.getContentResolver().update(thumbUri, values, null, null);				
				}

				OutputStream thumbOut = context.getContentResolver().openOutputStream(thumbUri);
				bm.compress(Bitmap.CompressFormat.JPEG, 85, thumbOut);
				thumbOut.close();
				return id;
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
		return -1;
	}

	private void createThumbnail(ByteReader reader, ByteWriter writer) {
		long id = reader.ReadInt64();
		String path = reader.ReadString();
		id = StoreThumbnail(id, path);

		if (id != -1) {
			String where = Images.Thumbnails.IMAGE_ID + "=" + id;
			Cursor cursor = null;
			try {
				cursor = context.getContentResolver().query(Images.Thumbnails.EXTERNAL_CONTENT_URI, null, where, null, null);
				if (cursor.moveToFirst()) {
					writer.Write(1);
					writer.Write(cursor.getInt(0));
					writer.WriteBlob(cursor.getBlob(1));
					writer.Write(cursor.getInt(2));
					writer.Write(cursor.getInt(3));
					writer.Write(cursor.getInt(4));
					writer.Write(cursor.getInt(5));
					return;
				}
			}
			catch (Exception e) {
				e.printStackTrace();
			}
			finally {
				if (cursor != null) {
					cursor.close();
				}
			}
		}

		writer.Write(0);
	}
}
