package daemon.provider.media;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.lang.reflect.Method;
import java.util.LinkedList;
import java.util.List;

import daemon.common.Common;
import daemon.common.Device;
import daemon.connection.ConnectionManager;
import daemon.logger.LogCenter;
import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;
import daemon.provider.IProvider;
import daemon.provider.ProviderExecuteContext;
import daemon.util.CursorHelper;
import daemon.util.GetVideoThumbnail;

import android.content.ContentResolver;
import android.content.ContentUris;
import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.Matrix;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.RingtoneManager;
import android.media.MediaPlayer.OnCompletionListener;
import android.media.MediaScannerConnection.MediaScannerConnectionClient;
import android.net.Uri;
import android.provider.MediaStore.Audio.AudioColumns;
import android.provider.MediaStore.Audio;
import android.provider.MediaStore.Video;
import android.provider.MediaStore.Audio.Media;
import android.text.TextUtils;

public class MediaProvider implements IProvider {

	static Object syncObj = new Object();
	private Context context;
	private MediaPlayer player;

	public MediaProvider(Context context) {
		this.context = context;
	}

	@Override
	public int getBusiness() {
		return 212;
	}

	@Override
	public void execute(ProviderExecuteContext cxt) {
		int action = cxt.getByteReader().ReadInt32();
		switch (action) {
			case 1:
				queryAudios(cxt, true); // 查找系统内的音频
				break;
			case 2:
				queryAudios(cxt, false);// 查找SD卡的音频
				break;
			case 3:
				queryMusicCount(cxt);// 查找音乐数目 不包含铃声
				break;
			case 4:
				getTotalMusicSize(cxt);// 得到所有音乐的大小
				break;
			case 5:
				queryAlbums(cxt); // 得到专辑列表
				break;
			case 6:
				queryArtists(cxt);// 得到歌手列表
				break;
			case 7:
				addAudio(cxt);// 添加音频 无论音乐还是铃声
				break;
			case 8:
				renameAudio(cxt);// 重命名音乐
				break;
			case 9:
				modifyAudio(cxt);// 修改音频信息
				break;
			case 10:
				changeAudioType(cxt);// 修改音乐类型为是否是铃声，是否是音乐
				break;
			case 11:
				removeMusic(cxt);// 删除音频文件
				break;
			case 12:
				removeRingtone(cxt);// 删除铃声、这个和删除音频的区别是当这个铃声既是音乐又是铃声时，仅删除铃声标志不删除音乐？
				break;
			case 13:
				setRing(cxt);// 设置为铃声
				break;
			case 14:
				playAudio(cxt);// 播放音乐
				break;
			case 15:
				stopPlay(cxt);// 停止播放
				break;
			case 16:
				setVolume(cxt);// 设置音量
				break;
			case 17:
				seekPlayerProgress(cxt);// 让播放器从指定的位置开始播放？预留接口
				break;
			case 18:
				readAudioTag(cxt);// 将音频头信息读入缓存 主要用于乱码修复
				break;
			case 19:
				pausePlayer(cxt);
				break;
			case 20:
				playPausePlayer(cxt);
				break;
			case 21:
				queryAlbumArt(cxt);// 查找专辑图片
				break;
			case 22:
				addAlbumArt(cxt);// 添加专辑图片
				break;
			case 23:
				removeAlbumArt(cxt);// 删除专辑图片
				break;
			case 24:
				queryPlaylist(cxt);// 得到播放列表
				break;
			case 25:
				queryPlaylistItems(cxt);// 得到播放列表
				break;
			case 26:
				addPlaylist(cxt);// 添加播放列表
				break;
			case 27:
				addTrackToPlaylist(cxt);// 添加音乐到播放列表内
				break;
			case 28:
				changePlaylistItemOrder(cxt);// 把音乐转移到另外一个播放列表
				break;
			case 29:
				renamePlaylist(cxt);// 重命名播放列表
				break;
			case 30:
				removePlaylist(cxt);// 删除播放列表
				break;
			case 31:
				removeFromPlaylist(cxt);// 把音乐从播放列表内删除
				break;
			case 32:
				removeNotify(cxt); // 删除通知铃声
				break;
			case 33: //获取自定义铃声格式
			    queryDefRingCount(cxt); 
			    break;
			case 34: //获取系统铃声
			    querySysRingCount(cxt);
			    break;
			case 41:
				queryVideos(cxt,false);// 得到视频
				break;
			case 42:
				getVideoThumbnail(cxt);// 得到视频缩略图
				break;
			case 43:
				queryVideoCount(cxt);// 得到视频个数
				break;
			case 44:
				getTotalVideoSize(cxt);// 得到视频个数的总大小
				break;
			case 45:
				addVideo(cxt);// 添加视频
				break;
			case 46:
				renameVideo(cxt);// 给视频重新命名
				break;
			case 47:
				removeVideo(cxt);// 删除
				break;
			case 48:
				queryVideos(cxt,true);// 得到视频
				break;
			case 51:
				getMediaVolume(cxt); // 获取媒体音量
				break;		
			case 52:
				setMediaVolume(cxt); // 设置媒体音量
				break;
			case 53:
				getMediaStatus(cxt); //pc端通知的多媒体状态
				break;
		}
	}

	private void getMediaStatus(ProviderExecuteContext cxt) {
		int status = cxt.getByteReader().ReadInt32();
		//0 暂停态  -1播放态
		ConnectionManager.getInstance().pcMediaStatus(cxt.getAccess().getConnectionId(), status);		
	}

	private void getMediaVolume(ProviderExecuteContext cxt) {
		AudioManager am = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
		if (am != null) {
			int maxVolume = am.getStreamMaxVolume(AudioManager.STREAM_MUSIC);
			int currVolume = am.getStreamVolume(AudioManager.STREAM_MUSIC);
			cxt.getByteWriter().Write(maxVolume);
			cxt.getByteWriter().Write(currVolume);
		}
	}
	
	private void setMediaVolume(ProviderExecuteContext cxt){
		int volume = cxt.getByteReader().ReadInt32();
		AudioManager am = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
		if (am != null) {
			am.setStreamVolume(AudioManager.STREAM_MUSIC, volume, 0);
		}
	}

	public String getAlbumArt(String albumName) {
		String art = "";
		int albumId = getAlbumId(albumName);
		if (albumId >= 0) {
			Uri uri = Uri.parse("content://media/external/audio/albumart");
			uri = ContentUris.withAppendedId(uri, albumId);
			Cursor cursor = context.getContentResolver().query(uri, new String[] {"_data"}, null, null, null);
			if (cursor != null) {
				if (cursor.moveToFirst())
					art = cursor.getString(0);
				cursor.close();
			}
		}
		return art;
	}

	public boolean putAlbumArt(int albumId, String artPath) {
		Uri artUri = Uri.parse("content://media/external/audio/albumart");
		Uri albumArtUri = ContentUris.withAppendedId(artUri, albumId);
		Cursor c = context.getContentResolver().query(albumArtUri, new String[] {"_data"}, null, null, null);
		String data = "";
		if (c != null) {
			if (c.getCount() > 0) {
				c.moveToFirst();
				data = c.getString(0);
			}
			c.close();
		}

		if (!data.equals(artPath)) {
			context.getContentResolver().delete(albumArtUri, null, null);
			ContentValues artValues = new ContentValues();
			artValues.put("album_id", albumId);
			artValues.put("_data", artPath);
			Uri uri = context.getContentResolver().insert(artUri, artValues);
			return uri != null;
		}
		return false;
	}

	public Uri addTrackToPlaylist(int playlistId, int audioId, int order) {
		ContentResolver cr = context.getContentResolver();
		Uri uri = getPlaylistUri(playlistId);
		ContentValues values = new ContentValues();
		values.put(Audio.Playlists.Members.AUDIO_ID, audioId);
		values.put(Audio.Playlists.Members.PLAY_ORDER, order);
		return cr.insert(uri, values);
	}

	public boolean removeFromPlaylist(int playlistId, int itemId) {
		ContentResolver cr = context.getContentResolver();
		Uri uri = getPlaylistUri(playlistId);
		int exeted = cr.delete(uri, "_id=" + String.valueOf(itemId), null);
		return exeted >= 0;
	}

	public AudioTagInfo ReadAudioTagInfo(String file) {
		AudioTagInfo info = new AudioTagInfo();
		RandomAccessFile stream = null;
		try {
			stream = new RandomAccessFile(file, "r");
			info.TotalLength = stream.length();
			byte[] buffer = new byte[3];
			stream.read(buffer);
			String header = new String(buffer);
			byte[] tagHeader = null;
			if (header.equals("ID3")) {
				stream.read();
				stream.read();
				stream.read();
				buffer = new byte[4];
				stream.read(buffer);
				int tagSize = buffer[0] * 0x200000 + buffer[1] * 0x4000 + buffer[2] * 0x80 + buffer[3];
				tagHeader = new byte[tagSize];
				stream.seek(0);
				stream.read(tagHeader);
				info.Header = tagHeader;
			}
			byte[] tagBottom = new byte[128];
			stream.seek(stream.length() - 128);
			stream.read(tagBottom);
			info.Bottom = tagBottom;
			info.Success = true;
		}
		catch (Exception e) {
			LogCenter.error("Can't Read tag", e.getMessage());
		}
		finally {
			if (stream != null)
				try {
					stream.close();
				}
				catch (IOException e) {

				}
		}
		return info;
	}

	public int getAudioCount(Boolean isMusic, Boolean isSystem) {
		String where = "";
		if(isMusic)
		    where = Media.IS_MUSIC + " = 1";
		else
		    where = Media.IS_RINGTONE + " = 1 or " + Media.IS_NOTIFICATION + " = 1";
		
		 Uri uri = isSystem ? Audio.Media.INTERNAL_CONTENT_URI : Audio.Media.EXTERNAL_CONTENT_URI;
				
        Cursor cursor = CursorHelper.query(context.getContentResolver(),uri, new String[] {Media.DATA}, where, null, null);
		if (cursor != null) {
			int cnt = 0;
			if (cursor.moveToFirst()) {
				do {
					MediaInfo info = new MediaInfo();
					info.data = cursor.getBlob(0);
					String filePath = new String(info.data, 0, info.data.length - 1);
					if (new File(filePath).exists()) {
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

	private int GetVideoCount() {
		Uri uri = Video.Media.EXTERNAL_CONTENT_URI;
		//Cursor cursor = context.getContentResolver().query(uri, new String[] {Video.Media._ID}, null, null, null);
		Cursor cursor = CursorHelper.query(context.getContentResolver(), uri, new String[] {Video.Media._ID}, null, null, null);
		if (cursor != null) {
			int cnt = cursor.getCount();
			cursor.close();
			return cnt;
		}
		return 0;
	}

	private Uri scanFile(String filePath, String fileType) {
		// Object locker = new Object();
		// ProviderMediaScannerConnectionClient client = new ProviderMediaScannerConnectionClient(
		// locker);
		// MediaScannerConnection conn = new MediaScannerConnection(context,
		// client);
		//
		// conn.connect();
		//
		// conn.scanFile(path, mimeType);
		//
		// try {
		// locker.wait(5000);
		// } catch (Exception e) {
		// //LogCenter.error("On Provider scan file", e.getMessage());
		// e.printStackTrace();
		// }
		//
		//
		// return client.Uri;

		return MediaScanner.getInstance(this.context).scanFile(filePath, fileType);
	}

	private void queryAudios(ProviderExecuteContext cxt, boolean isSystem) {
		synchronized (syncObj) {
			Uri uri = isSystem ? Audio.Media.INTERNAL_CONTENT_URI : Audio.Media.EXTERNAL_CONTENT_URI;
			// Media.ALBUM_ID,
			String[] columns = new String[] {Audio.Media._ID, Audio.Media.DATA, Audio.Media.TITLE, Audio.Media.DURATION, Audio.Media.ARTIST, Audio.Media.ALBUM, Audio.Media.IS_RINGTONE, Audio.Media.IS_MUSIC, Audio.Media.IS_ALARM,
					Audio.Media.IS_NOTIFICATION, Audio.Media.SIZE};
			Cursor cursor = context.getContentResolver().query(uri, columns, null, null, null);
			if (cursor != null) {
				List<MediaInfo> listMedia = new LinkedList<MediaProvider.MediaInfo>();
				if (cursor.moveToFirst()) {
					do {
						if (cxt.isCancel())
							break;
						MediaInfo info = new MediaInfo();
						info._id = cursor.getInt(0);
						info.data = cursor.getBlob(1);
						String filePath = new String(info.data, 0, info.data.length - 1);
						if (!new File(filePath).exists()) {
							continue;
						}
						info.title = cursor.getBlob(2);
						info.duration = cursor.getInt(3);
						info.artist = cursor.getBlob(4);
						info.album = cursor.getBlob(5);
						int flag = cursor.getInt(6);
						info.is_ringtone = flag == 1;
						flag = cursor.getInt(7);
						info.is_music = flag == 1;
						flag = cursor.getInt(8);
						info.is_alarm = flag == 1;
						flag = cursor.getInt(9);
						info.is_notification = flag == 1;
						info.size = cursor.getLong(10);
						listMedia.add(info);
					}
					while (cursor.moveToNext());
				}
				cursor.close();
				ByteWriter writer = cxt.getByteWriter();
				writer.Write(listMedia.size());
				for (MediaInfo info : listMedia) {
					writer.Write(info._id);
					writer.WriteBlob(info.data);
					writer.WriteBlob(info.title);
					writer.Write(info.duration);
					writer.WriteBlob(info.artist);
					writer.WriteBlob(info.album);
					writer.Write(info.is_ringtone);
					writer.Write(info.is_music);
					writer.Write(info.is_alarm);
					writer.Write(info.is_notification);
					writer.Write(info.size);
				}
			}
			else {
				cxt.getByteWriter().Write(0);
			}
		}
	}

	private void queryAlbums(ProviderExecuteContext cxt) {
		synchronized (syncObj) {
			String[] columns = new String[] {Audio.Albums.ALBUM, Audio.Albums.ALBUM_ART};
			Cursor cursor = context.getContentResolver().query(Audio.Albums.EXTERNAL_CONTENT_URI, columns, null, null, null);
			if (cursor != null) {
				cxt.getByteWriter().Write(cursor.getCount());
				if (cursor.moveToFirst()) {
					do {
						if (cxt.isCancel())
							break;
						cxt.getByteWriter().WriteBlob(cursor.getBlob(0));
						cxt.getByteWriter().WriteBlob(cursor.getBlob(1));
					}
					while (cursor.moveToNext());
				}
				cursor.close();
			}
			else {
				cxt.getByteWriter().Write(0);
			}
		}
	}

	private void queryArtists(ProviderExecuteContext cxt) {
		String[] columns = new String[] {Audio.Artists.ARTIST};
		Cursor cursor = context.getContentResolver().query(Audio.Artists.EXTERNAL_CONTENT_URI, columns, null, null, null);
		if (cursor != null) {
			cxt.getByteWriter().Write(cursor.getCount());
			if (cursor.moveToFirst()) {
				do {
					if (cxt.isCancel())
						break;
					cxt.getByteWriter().WriteBlob(cursor.getBlob(0));
				}
				while (cursor.moveToNext());
			}
			cursor.close();
		}
		else {
			cxt.getByteWriter().Write(0);
		}
	}

	private void queryPlaylist(ProviderExecuteContext cxt) {
		synchronized (syncObj) {
			String[] columns = new String[] {Audio.Playlists._ID, Audio.Playlists.NAME};
			Cursor cursor = context.getContentResolver().query(Audio.Playlists.EXTERNAL_CONTENT_URI, columns, null, null, null);
			if (cursor != null) {
				cxt.getByteWriter().Write(cursor.getCount());
				if (cursor.moveToFirst()) {
					do {
						if (cxt.isCancel())
							break;
						cxt.getByteWriter().Write(cursor.getInt(0));
						cxt.getByteWriter().WriteBlob(cursor.getBlob(1));
					}
					while (cursor.moveToNext());
				}
				cursor.close();
			}
			else {
				cxt.getByteWriter().Write(0);
			}
		}
	}

	private void queryPlaylistItems(ProviderExecuteContext cxt) {
		int playlistId = cxt.getByteReader().ReadInt32();
		Uri uri = getPlaylistUri(playlistId);
		String[] columns = new String[] {Audio.Playlists.Members._ID, Audio.Playlists.Members.AUDIO_ID, Audio.Playlists.Members.PLAY_ORDER};
		Cursor cursor = context.getContentResolver().query(uri, columns, null, null, Audio.Playlists.Members.PLAY_ORDER);
		if (cursor != null) {
			cxt.getByteWriter().Write(cursor.getCount());
			if (cursor.moveToFirst()) {
				do {
					if (cxt.isCancel())
						break;
					cxt.getByteWriter().Write(cursor.getInt(0));
					cxt.getByteWriter().Write(cursor.getInt(1));
					cxt.getByteWriter().Write(cursor.getInt(2));
				}
				while (cursor.moveToNext());
			}
			cursor.close();
		}
		else {
			cxt.getByteWriter().Write(0);
		}
	}

	private void queryVideos(ProviderExecuteContext cxt,boolean getTime) {
		synchronized (syncObj) {
			String[] columns = new String[] {Video.Media._ID, Video.Media.DATA, Video.Media.TITLE, Video.Media.DURATION, Video.Media.ARTIST, Video.Media.ALBUM,Video.Media.DATE_ADDED,Video.Media.DATE_MODIFIED};
			Cursor cursor = context.getContentResolver().query(Video.Media.EXTERNAL_CONTENT_URI, columns, null, null, null);
			if (cursor != null) {
			    //先判断文件是否存在 
			    List<MediaInfo> listMedia = new LinkedList<MediaProvider.MediaInfo>();
                if (cursor.moveToFirst()) {
                    do {
                        if (cxt.isCancel())
                            break;
                        MediaInfo info = new MediaInfo();
                        info._id = cursor.getInt(0);
                        info.data = cursor.getBlob(1);
                        String filePath = new String(info.data, 0, info.data.length - 1);
                        File f = new File(filePath);
                        if (!f.exists()) {
                            continue;
                        }
                        info.title = cursor.getBlob(2);
                        info.duration = cursor.getInt(3);
                        info.artist = cursor.getBlob(4);
                        info.album = cursor.getBlob(5);
                        info.addtime = cursor.getLong(6);
                        info.modifiedtime = cursor.getLong(7);
                        info.size = f.length();
                        listMedia.add(info);
                    }
                    while (cursor.moveToNext());
                }
                cursor.close();
                //再写入消息内
                ByteWriter writer = cxt.getByteWriter();
                writer.Write(listMedia.size());
                for (MediaInfo info : listMedia) {
                    writer.Write(info._id);
                    writer.WriteBlob(info.data);
                    writer.WriteBlob(info.title);
                    writer.Write(info.duration);
                    writer.WriteBlob(info.artist);
                    writer.WriteBlob(info.album);
                    if(getTime){
                    	writer.Write(info.addtime);
                    	writer.Write(info.modifiedtime);
                    	writer.Write(info.size);
                    }
                }
			}
			else {
				cxt.getByteWriter().Write(0);
			}
		}
	}

	private void queryAlbumArt(ProviderExecuteContext cxt) {
		synchronized (syncObj) {
			String albumName = cxt.getByteReader().ReadString();
			String art = getAlbumArt(albumName);
			cxt.getByteWriter().Write(art);
		}
	}

	private int getAlbumId(String album) {
		int albumId = -1;
		String[] columns = new String[] {Audio.Albums._ID, Audio.Albums.ALBUM};
		Cursor cursor = context.getContentResolver().query(Audio.Albums.EXTERNAL_CONTENT_URI, columns, null, null, null);
		if (cursor != null) {
			if (cursor.moveToFirst()) {
				while (!cursor.isAfterLast()) {
					String name = cursor.getString(1);
					if (name.equals(album)) {
						albumId = cursor.getInt(0);
						break;
					}
					cursor.moveToNext();
				}
			}
			cursor.close();
		}
		return albumId;
	}

	private void addAudio(ProviderExecuteContext cxt) {
		String data = cxt.getByteReader().ReadString();
		String title = cxt.getByteReader().ReadString();
		String artist = cxt.getByteReader().ReadString();
		String album = cxt.getByteReader().ReadString();
		String artPath = cxt.getByteReader().ReadString();
		long size = cxt.getByteReader().ReadInt64();
		String format = cxt.getByteReader().ReadString();
		int modified = (int) (getLastModified(data) / 1000);
		int duration = cxt.getByteReader().ReadInt32();
		byte track = cxt.getByteReader().ReadByte();
		String year = cxt.getByteReader().ReadString();
		int type = cxt.getByteReader().ReadInt32();
		int isRingtone = ((type & RingtoneManager.TYPE_RINGTONE) > 0) ? 1 : 0;
		int isMusic = ((type & 128) > 0) ? 1 : 0;
		int isAlarm = ((type & RingtoneManager.TYPE_ALARM) > 0) ? 1 : 0;
		int isNotification = ((type & RingtoneManager.TYPE_NOTIFICATION) > 0) ? 1 : 0;
		
		if (artist == null || artist.equals(""))
			artist = "<unknown>";
		if (album == null || album.equals(""))
			album = "<unknown>";
		ContentValues values = new ContentValues();
		values.put(Audio.Media.DATA, data);
		values.put(Audio.Media.TITLE, title);
		values.put(Audio.Media.ARTIST, artist);
		values.put(Audio.Media.ALBUM, album);
		values.put(Audio.Media.SIZE, size);
		values.put(Audio.Media.MIME_TYPE, format);
		values.put(Audio.Media.DATE_MODIFIED, modified);
		values.put(Audio.Media.DURATION, duration);
		values.put(Audio.Media.TRACK, track);
		values.put(Audio.Media.IS_RINGTONE, isRingtone);
		values.put(Audio.Media.IS_MUSIC, isMusic);
		values.put(Audio.Media.IS_ALARM, isAlarm);
		values.put(Audio.Media.IS_NOTIFICATION, isNotification);
		
		
		int id = -1;
		if(!isExternalAudioExist(data)){
			id = addAudio(cxt.getByteWriter(), values);
			if(id == -1){
				id = scanAndUpdateAudio(cxt.getByteWriter(),data,values);
			}
		}else{
			id = scanAndUpdateAudio(cxt.getByteWriter(),data, values);
		}
		//添加专辑
        if(id != -1){
        	File f = new File(artPath);
			if(f.exists()&&f.isFile()){
				if (album == null || album.equals(""))
					album = "<unknown>";
				int albumId = getAlbumId(album);
				if (albumId >= 0)
					putAlbumArt(albumId, artPath);
			}
        }
		if(id != -1){
			cxt.getByteWriter().Write(1);
			cxt.getByteWriter().Write(id);
		}else{
			cxt.getByteWriter().Write(0);
		}	
	}
	
	public int addAudio(ByteWriter writer, ContentValues values) {
		try {
			Uri insertUri = context.getContentResolver().insert(Audio.Media.EXTERNAL_CONTENT_URI, values);
			if (insertUri != null) {
				int id = Integer.parseInt(insertUri.getLastPathSegment());
				return id;
			}
		}
		catch (Exception ex) {
			LogCenter.error("Can't insert audio normally.", ex.getMessage());
		}
		
		return -1;
	}

	public int scanAndUpdateAudio(ByteWriter writer, String file,ContentValues values) {	
		Uri uri = scanFile(file, null);
		
		if (uri != null) {
			Cursor cursor = context.getContentResolver().query(uri,
					new String[] {Media._ID}, null, null, null);
			if (cursor != null) {			
				if (cursor.moveToFirst()) {
					int id = cursor.getInt(0);
					updateRecordValues(id, values);
					return id;
				}
				cursor.close();
			}
		}
		
		return -1;
	}
	
	//判断音频是否已存在
	private boolean isExternalAudioExist(String path){
		String[] columns = new String[] {Audio.Media._ID};
		String where = Audio.Media.DATA + "='" + path + "'";
		Cursor cur = null;
		try
		{
			cur = context.getContentResolver().query(Audio.Media.EXTERNAL_CONTENT_URI, columns, where, null, null);
			if(cur != null && cur.moveToFirst()){
				return true;
			}
		}catch(Exception e){
			e.printStackTrace();
		}finally{
			if(cur != null){
				cur.close();
			}
		}
		
		return false;
	}

	private void removeRingtone(ProviderExecuteContext cxt) {
		int id = cxt.getByteReader().ReadInt32();
		if(updateRecordFlags(id, -1, 0, -1, -1)){
			deleteAudioWidthCheck(id);
			cxt.getByteWriter().Write(1);
		}else{
			cxt.getByteWriter().Write(0);
		}		
	}

	private void removeMusic(ProviderExecuteContext cxt) {
		int id = cxt.getByteReader().ReadInt32();
		if(updateRecordFlags(id, 0, -1, -1, -1)){
			deleteAudioWidthCheck(id);
			cxt.getByteWriter().Write(1);
		}else{
			cxt.getByteWriter().Write(0);
		}		
	}
	
	private void removeNotify(ProviderExecuteContext cxt){
		int id = cxt.getByteReader().ReadInt32();
		if(updateRecordFlags(id, -1, -1, 0, -1)){
			deleteAudioWidthCheck(id);
			cxt.getByteWriter().Write(1);
		}else{
			cxt.getByteWriter().Write(0);
		}		
	}
	
	//检查音频，如果不是任意类型的铃声，才删除
	private void deleteAudioWidthCheck(int id){	
		Uri uri = ContentUris.withAppendedId(Media.EXTERNAL_CONTENT_URI, id);
		Cursor cursor = context.getContentResolver().query(uri, new String[] {AudioColumns.IS_MUSIC, AudioColumns.IS_RINGTONE, AudioColumns.IS_NOTIFICATION, AudioColumns.IS_ALARM, AudioColumns.IS_PODCAST}, null, null, null);
		boolean canDelete = false;
		if (cursor != null) {
			if (cursor.moveToFirst()) {
				int isMusic = cursor.getInt(0);
				int isRingtone = cursor.getInt(1);
				int isNotification = cursor.getInt(2);
				int isAlarm = cursor.getInt(3);
				int isPodcast = cursor.getInt(4);
				if (isMusic == 0 && isRingtone == 0 && isNotification == 0 && isAlarm == 0 && isPodcast == 0) {
					canDelete = true;				
				}
			}
			cursor.close();
		}
		
		if(canDelete){
			context.getContentResolver().delete(uri, null, null);
		}
	}


	private boolean updateRecordValues(int id, ContentValues values) {
		try{
			Uri uri = ContentUris.withAppendedId(Media.EXTERNAL_CONTENT_URI, id);
			return context.getContentResolver().update(uri, values, null, null) > 0;
		}catch(Exception e){
			e.printStackTrace();
		}
		return false;
	}
	/*
	 * Update a record's flags. -1 don't change this flag; 1 true; 0 false
	 */
	private boolean updateRecordFlags(int id, int isMusic, int isRingtone, int isNotification, int isAlarm) {
		try{
			Uri uri = ContentUris.withAppendedId(Media.EXTERNAL_CONTENT_URI, id);
			ContentValues values = new ContentValues();
			if (isMusic > -1)
				values.put(AudioColumns.IS_MUSIC, isMusic);
			if (isRingtone > -1)
				values.put(AudioColumns.IS_RINGTONE, isRingtone);
			if (isNotification > -1)
				values.put(AudioColumns.IS_NOTIFICATION, isNotification);
			if (isAlarm > -1) {
				values.put(AudioColumns.IS_ALARM, isAlarm);
			}
			return context.getContentResolver().update(uri, values, null, null) > 0;
		}catch(Exception e){
			e.printStackTrace();
		}
	
		return false;
	}

	private void renameAudio(ProviderExecuteContext cxt) {
		int id = cxt.getByteReader().ReadInt32();
		String title = cxt.getByteReader().ReadString();
		Uri uri = ContentUris.withAppendedId(Media.EXTERNAL_CONTENT_URI, id);
		ContentValues values = new ContentValues();
		values.put(Media.TITLE, title);
		context.getContentResolver().update(uri, values, null, null);
		cxt.getByteWriter().Write(1);
	}

	private void modifyAudio(ProviderExecuteContext cxt) {
		int trackId = cxt.getByteReader().ReadInt32();
		String album = cxt.getByteReader().ReadString();
		String artist = cxt.getByteReader().ReadString();

		Uri trackUri = ContentUris.withAppendedId(Audio.Media.EXTERNAL_CONTENT_URI, trackId);

		ContentValues values = new ContentValues();
		if (!album.equals(""))
			values.put(Audio.Media.ALBUM, album);
		if (!artist.equals(""))
			values.put(Audio.Media.ARTIST, artist);
		if (values.size() > 0)
			context.getContentResolver().update(trackUri, values, null, null);
		cxt.getByteWriter().Write(1);
	}

	private void addPlaylist(ProviderExecuteContext cxt) {
		String name = cxt.getByteReader().ReadString();

		ContentValues values = new ContentValues();
		values.put(Audio.Playlists.NAME, name);

		Uri uri = context.getContentResolver().insert(Audio.Playlists.EXTERNAL_CONTENT_URI, values);
		if (uri != null) {
			Cursor cursor = context.getContentResolver().query(uri, new String[] {Audio.Playlists._ID, Audio.Playlists.NAME}, null, null, null);
			if (cursor != null) {
				if (cursor.moveToFirst()) {
					cxt.getByteWriter().Write(cursor.getInt(0));
					cxt.getByteWriter().WriteBlob(cursor.getBlob(1));
				}
				cursor.close();
			}
			else {
				cxt.getByteWriter().Write(0);
			}
		}
	}

	private void renamePlaylist(ProviderExecuteContext cxt) {
		int id = cxt.getByteReader().ReadInt32();
		String name = cxt.getByteReader().ReadString();

		Uri uri = ContentUris.withAppendedId(Audio.Playlists.EXTERNAL_CONTENT_URI, id);
		ContentValues values = new ContentValues();
		values.put(Audio.Playlists.NAME, name);

		ContentResolver cr = context.getContentResolver();
		cr.update(uri, values, null, null);
		cxt.getByteWriter().Write(1);
	}

	private void removePlaylist(ProviderExecuteContext cxt) {
		int id = cxt.getByteReader().ReadInt32();
		Uri uri = ContentUris.withAppendedId(Audio.Playlists.EXTERNAL_CONTENT_URI, id);
		ContentResolver cr = context.getContentResolver();
		cr.delete(uri, null, null);
		cxt.getByteWriter().Write(1);
	}

	private void addTrackToPlaylist(ProviderExecuteContext cxt) {
		int playlistId = cxt.getByteReader().ReadInt32();
		int trackId = cxt.getByteReader().ReadInt32();
		int order = cxt.getByteReader().ReadInt32();

		Uri mapUri = addTrackToPlaylist(playlistId, trackId, order);
		Uri uri = getPlaylistUri(playlistId);
		if (mapUri != null) {
			long mapId = ContentUris.parseId(mapUri);
			Cursor cursor = context.getContentResolver().query(uri, new String[] {Audio.Playlists.Members._ID, Audio.Playlists.Members.PLAY_ORDER}, null, null, Audio.Playlists.Members.PLAY_ORDER);
			if (cursor != null) {
				if (cursor.moveToFirst()) {
					do {
						if (cursor.getLong(0) == mapId) {
							cxt.getByteWriter().Write((int) mapId);
							cxt.getByteWriter().Write(cursor.getInt(1));
							break;
						}
					}
					while (cursor.moveToNext());
				}
				cursor.close();
			}
		}
	}

	private void changePlaylistItemOrder(ProviderExecuteContext cxt) {
		int playlistId = cxt.getByteReader().ReadInt32();
		int itemId = cxt.getByteReader().ReadInt32();
		int audioId = cxt.getByteReader().ReadInt32();
		int order = cxt.getByteReader().ReadInt32();

		if (removeFromPlaylist(playlistId, itemId)) {
			Uri uri = addTrackToPlaylist(playlistId, audioId, order);
			if (uri != null) {
				cxt.getByteWriter().Write((int) ContentUris.parseId(uri));
				return;
			}
		}
		cxt.getByteWriter().WriteString("FAIL");
	}

	private void removeFromPlaylist(ProviderExecuteContext cxt) {
		int playlistId = cxt.getByteReader().ReadInt32();
		int itemId = cxt.getByteReader().ReadInt32();

		if (removeFromPlaylist(playlistId, itemId)) {
			cxt.getByteWriter().Write(1);
		}
		else {
			cxt.getByteWriter().Write(0);
		}
	}

	private void setRing(ProviderExecuteContext cxt) {
		int id = cxt.getByteReader().ReadInt32();
		boolean isSystem = cxt.getByteReader().ReadBoolean();
		int kind = cxt.getByteReader().ReadInt32();

		Uri root = isSystem ? Media.INTERNAL_CONTENT_URI : Media.EXTERNAL_CONTENT_URI;
		Uri mediaUri = ContentUris.withAppendedId(root, id);
		if (!isSystem)
			changeAudioType(mediaUri, kind);
		
		RingtoneManager.setActualDefaultRingtoneUri(context, kind, mediaUri);
		cxt.getByteWriter().Write(1);
	}

	private void addVideo(ProviderExecuteContext cxt) {
		ByteReader reader = cxt.getByteReader();
		String data = reader.ReadString();
		String title = reader.ReadString();
		String artist = reader.ReadString();
		String album = reader.ReadString();
		long size = reader.ReadInt64();
		String format = reader.ReadString();
		int duration = reader.ReadInt32();

		boolean r = addVideo(cxt.getByteWriter(), data, title, artist, album, size, format, duration);
		if (!r) {
			r = addVideo(cxt.getByteWriter(), data);
			if (!r)
				cxt.getByteWriter().Write(0);
		}
	}

	public boolean addVideo(ByteWriter writer, String data, String title, String artist, String album, long size, String format, int duration) {
		if (artist == null || artist.equals(""))
			artist = "<unknown>";
		if (album == null || album.equals(""))
			album = "<unknown>";
		long modified = getLastModified(data);
		ContentValues values = new ContentValues();
		values.put(Video.Media.DATA, data);
		values.put(Video.Media.TITLE, title);
		values.put(Video.Media.ARTIST, artist);
		values.put(Video.Media.ALBUM, album);
		values.put(Video.Media.SIZE, size);
		values.put(Video.Media.MIME_TYPE, format);
		values.put(Video.Media.DATE_MODIFIED, modified);
		values.put(Video.Media.DURATION, duration);

		ContentResolver resolver = context.getContentResolver();
		try {
			Uri uri = resolver.insert(Video.Media.EXTERNAL_CONTENT_URI, values);
			Cursor cursor = resolver.query(uri, new String[] {Video.Media._ID}, null, null, null);
			if (cursor != null) {
				if (cursor.moveToFirst()) {
					writer.Write(cursor.getInt(0));					
				}
				cursor.close();
				return true;
			}
		}
		catch (Exception ex) {
			LogCenter.error("MediaProvide On addVideo", ex.getMessage());
		}
		return false;
	}

	public boolean addVideo(ByteWriter writer, String file) {
		Uri uri = scanFile(file, null);
		if (uri != null) {
			Cursor cursor = context.getContentResolver().query(uri, new String[] {Video.Media._ID}, null, null, null);
			if (cursor != null) {
				if (cursor.moveToFirst()) {
					writer.Write(cursor.getInt(0));					
				}
				cursor.close();
				return true;
			}
		}
		return false;
	}

	private void removeVideo(ProviderExecuteContext cxt) {
		int id = cxt.getByteReader().ReadInt32();
		Uri uri = ContentUris.withAppendedId(Video.Media.EXTERNAL_CONTENT_URI, id);
		context.getContentResolver().delete(uri, null, null);
		cxt.getByteWriter().Write(1);
	}

	private void renameVideo(ProviderExecuteContext cxt) {
		ByteReader reader = cxt.getByteReader();
		int id = reader.ReadInt32();
		String title = reader.ReadString();
		Uri uri = ContentUris.withAppendedId(Video.Media.EXTERNAL_CONTENT_URI, id);
		ContentValues values = new ContentValues();
		values.put(Video.Media.TITLE, title);
		int executed = context.getContentResolver().update(uri, values, null, null);
		if (executed > 0)
			cxt.getByteWriter().Write(1);
		else
			cxt.getByteWriter().Write(0);
	}

	private void addAlbumArt(ProviderExecuteContext cxt) {
		String albumTitle = cxt.getByteReader().ReadString();
		String art = cxt.getByteReader().ReadString();

		int albumId = getAlbumId(albumTitle);
		boolean result = false;
		if (albumId >= 0)
			result = putAlbumArt(albumId, art);
		if (result)
			cxt.getByteWriter().Write(1);
		else
			cxt.getByteWriter().Write(0);
	}

	private void removeAlbumArt(ProviderExecuteContext cxt) {
		String album = cxt.getByteReader().ReadString();
		int albumId = getAlbumId(album);

		Uri artUri = Uri.parse("content://media/external/audio/albumart");
		Uri albumArtUri = ContentUris.withAppendedId(artUri, albumId);
		context.getContentResolver().delete(albumArtUri, null, null);
		cxt.getByteWriter().Write(1);
	}

	private void changeAudioType(ProviderExecuteContext cxt) {
		ByteReader reader = cxt.getByteReader();
		int id = reader.ReadInt32();
		int type = reader.ReadInt32();
		Uri uri = ContentUris.withAppendedId(Audio.Media.EXTERNAL_CONTENT_URI, id);
		if (changeAudioType(uri, type))
			cxt.getByteWriter().Write(1);
		else
			cxt.getByteWriter().Write(0);
	}

	private void playAudio(final ProviderExecuteContext cxt) {
		boolean result = false;
		String ringPath = cxt.getByteReader().ReadString();
		float volume = cxt.getByteReader().ReadSingle();

		if (ringPath != null && !ringPath.equals("")) {
			try {
				stopPlayer();
				player = MediaPlayer.create(this.context, Uri.parse(ringPath));
				if (player != null) {
					player.setVolume(volume, volume);
					final int connectId = cxt.getAccess().getConnectionId();
					player.setOnCompletionListener(new OnCompletionListener() {

						@Override
						public void onCompletion(MediaPlayer player) {
							stopPlayer();
							ByteWriter writer = new ByteWriter();
							writer.Write(getBusiness());
							writer.Write(true);
							writer.Write(0);
							ConnectionManager.getInstance().setConnectInfoPlayMediaStatus(connectId,false);
							ConnectionManager.getInstance().sendMessage(getBusiness(), cxt.getAccess().getConnectionId(), writer);
						}
					});

					player.start();
					ConnectionManager.getInstance().setConnectInfoPlayMediaStatus(connectId,true);
					
					result = true;
				}
			}
			catch (Exception e) {
				LogCenter.error("Media Provide on Play audio", e.getMessage());
			}
		}
		if (result)
			cxt.getByteWriter().Write(1);
		else
			cxt.getByteWriter().Write(0);
	}

	private void stopPlay(ProviderExecuteContext cxt) {
		stopPlayer();
		// cxt.getByteWriter().Write(19);
		cxt.getByteWriter().Write(true);
	}

	private void stopPlayer() {
		if (player != null) {
			player.stop();
			player.release();
			player = null;
		}
	}

	private void pausePlayer(ProviderExecuteContext cxt) {
		if (player != null && player.isPlaying())
			player.pause();
		cxt.getByteWriter().Write(1);
	}

	private void playPausePlayer(ProviderExecuteContext cxt) {
		if (player != null)
			player.start();
		cxt.getByteWriter().Write(1);
	}

	private void setVolume(ProviderExecuteContext cxt) {
		float volume = cxt.getByteReader().ReadSingle();
		setPlayerVolume(volume);
	}

	private void setPlayerVolume(float volume) {
		if (player != null) {
			player.setVolume(volume, volume);
		}	
	}

	private void seekPlayerProgress(ProviderExecuteContext cxt) {
		int ms = cxt.getByteReader().ReadInt32();
		if (player != null) {
			player.seekTo(ms);
		}
	}

	private void readAudioTag(ProviderExecuteContext cxt) {
		String path = cxt.getByteReader().ReadString();
		AudioTagInfo info = ReadAudioTagInfo(path);
		if (info.Success) {
			cxt.getByteWriter().Write(1);
			cxt.getByteWriter().Write(info.TotalLength);
			if (info.Header != null) {
				cxt.getByteWriter().Write(info.Header.length);
				cxt.getByteWriter().Write(info.Header);
			}
			else {
				cxt.getByteWriter().Write(0);
			}
			if (info.Bottom != null) {
				cxt.getByteWriter().Write(info.Bottom.length);
				cxt.getByteWriter().Write(info.Bottom);
			}
			else {
				cxt.getByteWriter().Write(0);
			}
		}
		else {
			cxt.getByteWriter().Write(0);
		}
	}

	private void queryMusicCount(ProviderExecuteContext cxt) {
		int cnt = getAudioCount(true, false);
		cxt.getByteWriter().Write(cnt);
	}
	
	private void queryDefRingCount(ProviderExecuteContext cxt)
	{
	    int cnt = getAudioCount(false, false);
	    cxt.getByteWriter().Write(cnt);
	}
	
	private void querySysRingCount(ProviderExecuteContext cxt)
    {
	    int cnt = getAudioCount(false, true);
        cxt.getByteWriter().Write(cnt);
    }

	private void queryVideoCount(ProviderExecuteContext cxt) {
		int cnt = GetVideoCount();
		cxt.getByteWriter().Write(cnt);
	}

	private void getTotalMusicSize(ProviderExecuteContext cxt) {
		String where = Media.IS_MUSIC + " = 1";
		Cursor cur = null;
		long size = 0L;

		try {
			cur = context.getContentResolver().query(Media.EXTERNAL_CONTENT_URI, new String[] {Media.DATA}, where, null, null);

			if (cur != null && cur.moveToFirst()) {
				while (!cur.isAfterLast()) {
					String path = cur.getString(0);

					if (Common.isSdcardPath(path)) {
						File f = new File(path);
						if (f.exists())
							size += f.length();
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
	}

	private void getTotalVideoSize(ProviderExecuteContext cxt) {
		Cursor cur = null;
		long size = 0L;

		try {
			cur = context.getContentResolver().query(Video.Media.EXTERNAL_CONTENT_URI, new String[] {Video.Media.DATA}, null, null, null);

			if (cur != null && cur.moveToFirst()) {
				while (!cur.isAfterLast()) {
					String path = cur.getString(0);

					if (Common.isSdcardPath(path)) {
						File f = new File(path);
						if (f.exists())
							size += f.length();
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
	}

	private void getVideoThumbnail(ProviderExecuteContext cxt) {
		ByteReader reader = cxt.getByteReader();
		ByteWriter writer = cxt.getByteWriter();
		int cnt = reader.ReadInt32();
		String[] filePaths = new String[cnt];
		for (int i = 0; i < cnt; i++) {
			filePaths[i] = reader.ReadString();
		}

		for (String filePath : filePaths) {
			Bitmap bitmap = null;
			Object retriever = null;
			try {
                if (Device.getIntSdkVersion() < 10) {
                    Class<?> classType = Class.forName("android.media.MediaMetadataRetriever");
                    retriever = classType.newInstance();
                    int mode_capture_frame_only = classType.getField("MODE_CAPTURE_FRAME_ONLY")
                            .getInt(null);

                    Method setMode = classType.getMethod("setMode", new Class[] {
                        int.class
                    });
                    setMode.invoke(retriever, new Object[] {
                        mode_capture_frame_only
                    });

                    Method setDataSource = classType.getMethod("setDataSource", new Class[] {
                        String.class
                    });
                    setDataSource.invoke(retriever, new Object[] {
                        filePath
                    });

                    Method captureFrame = classType.getMethod("captureFrame", new Class[] {});
                    bitmap = (Bitmap)captureFrame.invoke(retriever, new Object[] {});
                }
                else
                {                    
                    bitmap = GetVideoThumbnail.CreateVideoThumbnail(filePath);
                }
			}
			catch (Exception e) {
				LogCenter.error("Provider getVideoThumbnail error", e.getMessage());
			}
			finally {
				try {
					if (retriever != null) {
						Method release = retriever.getClass().getMethod("release", new Class[] {});
						release.invoke(retriever, new Object[] {});
					}
				}
				catch (Exception e) {
					LogCenter.error("Provider getVideoThumbnail error", e.getMessage());
				}
			}

			byte[] bytes = getBitmapBytes(bitmap);
			if (bytes != null && bytes.length > 0) {
				writer.Write(bytes.length);
				writer.Write(bytes);
			}
			else {
				writer.Write(0);
			}
		}
	}

	private Uri getPlaylistUri(int id) {
		String uriString = new StringBuilder().append("content://media/external/audio/playlists/").append(id).append("/members").toString();
		return Uri.parse(uriString);
	}

	private long getLastModified(String path) {
		File file = new File(path);
		return file.lastModified();
	}

	private byte[] getBitmapBytes(Bitmap bitmap) {
		if (bitmap != null) {
			int width = bitmap.getWidth();
			int height = bitmap.getHeight();

			int maxSide = 150;
			float scale = 0;

			if (width >= height && width > maxSide)
				scale = ((float) maxSide) / width;
			else if (height > width && height > maxSide)
				scale = ((float) maxSide) / height;
			else
				scale = 1;

			Matrix matrix = new Matrix();
			matrix.postScale(scale, scale);

			Bitmap resizedBitmap = Bitmap.createBitmap(bitmap, 0, 0, width, height, matrix, true);

			ByteArrayOutputStream baos = new ByteArrayOutputStream();
			resizedBitmap.compress(Bitmap.CompressFormat.PNG, 100, baos);

			byte[] bytes = baos.toByteArray();
			return bytes;
		}
		else {
			return null;
		}
	}

	private boolean changeAudioType(Uri audio, int kind) {
		ContentValues values = new ContentValues();
		switch (kind) {
			case RingtoneManager.TYPE_RINGTONE:
				values.put(Media.IS_RINGTONE, 1);
				break;
			case RingtoneManager.TYPE_NOTIFICATION:
				values.put(Media.IS_NOTIFICATION, 1);
				break;
			case RingtoneManager.TYPE_ALARM:
				values.put(Media.IS_ALARM, 1);
				break;
		}
		int executed = context.getContentResolver().update(audio, values, null, null);
		return executed > 0;
	}

	public class AudioTagInfo {
		public boolean Success = false;
		public long TotalLength;
		public byte[] Header;
		public byte[] Bottom;
	}

	public class ProviderMediaScannerConnectionClient implements MediaScannerConnectionClient {
		private Object locker;

		public ProviderMediaScannerConnectionClient(Object locker) {
			this.locker = locker;
		}

		public Uri Uri;

		@Override
		public void onMediaScannerConnected() {

		}

		@Override
		public void onScanCompleted(String arg0, Uri arg1) {
			Uri = arg1;
			locker.notify();
		}
	}

	class MediaInfo {
		int _id;
		byte[] data;
		byte[] title;
		int duration;
		byte[] artist;
		byte[] album;
		boolean is_ringtone;
		boolean is_music;
		boolean is_alarm;
		boolean is_notification;
		long size;
		long addtime;
		long modifiedtime;
	}
	
}
