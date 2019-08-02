package daemon.provider.business;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.lang.reflect.Method;
import java.util.ArrayList;


import daemon.connection.ConnectionManager;
import daemon.logger.LogCenter;
import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;
import daemon.provider.IProvider;
import daemon.provider.ProviderExecuteContext;

import android.content.ContentResolver;
import android.content.ContentUris;
import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.Matrix;
import android.media.MediaPlayer;
import android.media.MediaScannerConnection;
import android.media.RingtoneManager;
import android.media.MediaPlayer.OnCompletionListener;
import android.media.MediaScannerConnection.MediaScannerConnectionClient;
import android.net.Uri;
import android.provider.Settings;
import android.provider.MediaStore.Audio;
import android.provider.MediaStore.Video;
import android.provider.MediaStore.Audio.Media;

public class MediaProvider implements IProvider {

	static Object syncObj = new Object();
	private Context context;
	private MediaPlayer player;
	private ArrayList<AudioInfo> audios;
	private ArrayList<AlbumInfo> albums;
	private ArrayList<String> artists;
	private ArrayList<Playlist> playlists;
	private ArrayList<VideoInfo> videos;

	public MediaProvider(Context context) {
		this.context = context;
	}

	@Override
	public int getBusiness() {
		return 7;
	}

	public void ClearCache() {
		synchronized (syncObj) {
			if (audios != null) {
				audios.clear();
				audios = null;
			}
			if (playlists != null) {
				playlists.clear();
				playlists = null;
			}
			if (videos != null) {
				videos.clear();
				videos = null;
			}
		}
	}

	@Override
	public void execute(ProviderExecuteContext cxt) {
		byte action = cxt.getByteReader().ReadByte();
		switch (action) {
		case 99:
			queryAudios(cxt);
			break;
		case 98:
			queryAlbums(cxt);
			break;
		case 97:
			queryArtists(cxt);
			break;
		case 96:
			queryPlaylist(cxt);
			break;
		case 95:
			queryPlaylistItems(cxt);
			break;
		case 94:
			queryVideos(cxt);
			break;
		case 93:
			queryAlbumArt(cxt);
			break;
		case 0:
			addAudio(cxt);
			break;
		case 1:
			removeAudio(cxt);
			break;
		case 2:
			setRing(cxt);
			break;
		case 3:
			renameAudio(cxt);
			break;
		case 4:
			modifyAudio(cxt);
			break;
		case 5:
			addPlaylist(cxt);
			break;
		case 6:
			renamePlaylist(cxt);
			break;
		case 7:
			removePlaylist(cxt);
			break;
		case 8:
			addTrackToPlaylist(cxt);
			break;
		case 9:
			changePlaylistItemOrder(cxt);
			break;
		case 10:
			removeFromPlaylist(cxt);
			break;
		case 11:
			addVideo(cxt);
			break;
		case 12:
			removeVideo(cxt);
			break;
		case 13:
			renameVideo(cxt);
			break;
		case 14:
			addAlbumArt(cxt);
			break;
		case 15:
			removeAlbumArt(cxt);
			break;
		case 16:
			changeAudioType(cxt);
			break;
		case 17:
			playAudio(cxt);
			break;
		case 19:
			stopPlay(cxt);
			break;
		case 20:
			readAudioTag(cxt);
			break;
		case 21:
			queryAudioCount(cxt);
			break;
		case 22:
			queryVideoCount(cxt);
			break;
		case 23:
			getTotalAudioSize(cxt);
			break;
		case 24:
			getTotalVideoSize(cxt);
			break;
		case 101:
			getVideoThumbnail(cxt);
			break;
		}
	}

	public String getAlbumArt(String albumName) {
		AlbumInfo info = getCachedAlbumInfo(albumName);
		if (info != null)
			return info.AlbumArt;
		else {
			String art = getDBAlbumArt(albumName);
			return art;
		}
	}

	public AudioInfo addAudio(String file) {
		AudioInfo info = null;
		Uri uri = scanFile(file, null);
		if (uri != null) {
			Cursor cursor = context.getContentResolver().query(
					uri,
					new String[] { Media._ID, Media.DATA, Media.TITLE,
							Media.DURATION, Media.ARTIST, Media.ALBUM,
							Media.IS_RINGTONE, Media.IS_MUSIC, Media.IS_ALARM,
							Media.IS_NOTIFICATION, Media.SIZE }, null, null,
					null);
			if (cursor != null) {
				if (cursor.moveToFirst())
					info = readAudioInfo(cursor);
				cursor.close();
			}
		}
		return info;
	}

	public AudioInfo addAudio(String data, String title, String artist,
			String album, String artPath, long size, String format,
			int modified, int duration, byte track, String year, int type,
			int isRingtone, int isMusic, int isAlarm, int isNotification) {
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
		values.put(Audio.Media.YEAR, year);
		values.put(Audio.Media.IS_RINGTONE, isRingtone);
		values.put(Audio.Media.IS_MUSIC, isMusic);
		values.put(Audio.Media.IS_ALARM, isAlarm);
		values.put(Audio.Media.IS_NOTIFICATION, isNotification);
		try {
			Uri uri = context.getContentResolver().insert(
					Audio.Media.EXTERNAL_CONTENT_URI, values);
			if (uri != null) {
				Cursor cursor = context.getContentResolver().query(
						uri,
						new String[] { Media._ID, Media.DATA, Media.TITLE,
								Media.DURATION, Media.ARTIST, Media.ALBUM,
								Media.ALBUM_ID, Media.IS_RINGTONE,
								Media.IS_MUSIC, Media.IS_ALARM,
								Media.IS_NOTIFICATION, Media.SIZE }, null,
						null, null);
				if (cursor != null) {
					AudioInfo info = null;
					if (cursor.moveToFirst()) {
						File file = new File(artPath);
						if (file.exists()) {
							int albumId = cursor.getInt(6);
							if (putAlbumArt(albumId, artPath)) {
								if (albums != null) {
									AlbumInfo albumInfo = new AlbumInfo();
									albumInfo.Album = cursor.getString(5);
									albumInfo.AlbumArt = artPath;
									putAlbumCache(albumInfo);
								}
							}
						}
						info = readAudioInfo(cursor);
					}
					cursor.close();
					return info;
				}
			}
		} catch (Exception ex) {
			LogCenter.error("Can't insert audio normally.", ex.getMessage());
		}
		return null;
	}

	public VideoInfo addVideo(String file) {
		VideoInfo info = null;
		Uri uri = scanFile(file, null);
		if (uri != null) {
			Cursor cursor = context.getContentResolver().query(
					uri,
					new String[] { Video.Media._ID, Video.Media.DATA,
							Video.Media.TITLE, Video.Media.DURATION,
							Video.Media.ARTIST, Video.Media.ALBUM }, null,
					null, null);
			if (cursor != null) {
				if (cursor.moveToFirst())
					info = readVideoInfo(cursor);
				cursor.close();
			}
		}
		return info;
	}

	public VideoInfo addVideo(String data, String title, String artist,
			String album, long size, String format, int duration) {
		VideoInfo info = null;
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
			Cursor cursor = resolver.query(uri, new String[] { Video.Media._ID,
					Video.Media.DATA, Video.Media.TITLE, Video.Media.DURATION,
					Video.Media.ARTIST, Video.Media.ALBUM }, null, null, null);
			if (cursor != null) {
				if (cursor.moveToFirst())
					info = readVideoInfo(cursor);
				cursor.close();
			}
		} catch (Exception ex) {
			LogCenter.error("MediaProvide On addVideo", ex.getMessage());
		}
		return info;
	}

	public boolean putAlbumArt(int albumId, String artPath) {
		Uri artUri = Uri.parse("content://media/external/audio/albumart");
		Uri albumArtUri = ContentUris.withAppendedId(artUri, albumId);
		Cursor c = context.getContentResolver().query(albumArtUri,
				new String[] { "_data" }, null, null, null);
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
				int tagSize = buffer[0] * 0x200000 + buffer[1] * 0x4000
						+ buffer[2] * 0x80 + buffer[3];
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
		} catch (Exception e) {
			LogCenter.error("Can't Read tag", e.getMessage());
		} finally {
			if (stream != null)
				try {
					stream.close();
				} catch (IOException e) {

				}
		}
		return info;
	}

	public int getAudioCount() {
		String where = Media.IS_MUSIC + " = 1";
		Cursor cursor = context.getContentResolver().query(
				Media.EXTERNAL_CONTENT_URI, new String[] { Media._ID }, where,
				null, null);

		if (cursor != null) {
			int cnt = cursor.getCount();
			cursor.close();
			return cnt;
		}
		return 0;
	}

	private int GetVideoCount() {
		Uri uri = Video.Media.EXTERNAL_CONTENT_URI;
		Cursor cursor = context.getContentResolver().query(uri,
				new String[] { Video.Media._ID }, null, null, null);

		if (cursor != null) {
			int cnt = cursor.getCount();
			cursor.close();
			return cnt;
		}
		return 0;
	}

	private Uri scanFile(String path, String mimeType) {
		Object locker = new Object();
		ProviderMediaScannerConnectionClient client = new ProviderMediaScannerConnectionClient(
				locker);
		MediaScannerConnection conn = new MediaScannerConnection(context,
				client);
		conn.scanFile(path, mimeType);
		try {
			locker.wait(5000);
		} catch (InterruptedException e) {
			LogCenter.error("On Provider scan file", e.getMessage());
		}
		return client.Uri;
	}

	private void queryAudios(ProviderExecuteContext cxt) {
		synchronized (syncObj) {
			if (audios != null) {
				cxt.getByteWriter().Write(audios.size());
				for (int i = 0; i < audios.size(); i++) {
					writeAudioInfo(audios.get(i), cxt.getByteWriter());
				}
			} else {
				audios = new ArrayList<AudioInfo>();

				boolean isSystem = cxt.getByteReader().ReadBoolean();
				Uri uri = isSystem ? Audio.Media.INTERNAL_CONTENT_URI
						: Audio.Media.EXTERNAL_CONTENT_URI;
				String[] columns = new String[] { Audio.Media._ID,
						Audio.Media.DATA, Audio.Media.TITLE,
						Audio.Media.DURATION, Audio.Media.ARTIST,
						Audio.Media.ALBUM, Audio.Media.IS_RINGTONE,
						Audio.Media.IS_MUSIC, Audio.Media.IS_ALARM,
						Audio.Media.IS_NOTIFICATION, Audio.Media.SIZE };
				Cursor cursor = context.getContentResolver().query(uri,
						columns, null, null, null);
				if (cursor != null) {
					cxt.getByteWriter().Write(cursor.getCount());
					if (cursor.moveToFirst()) {
						do {
							if (cxt.isCancel()) {
								if (audios != null) {
									audios.clear();
									audios = null;
								}
								break;
							}
							AudioInfo audio = readAudioInfo(cursor);
							audios.add(audio);
							writeAudioInfo(audio, cxt.getByteWriter());
						} while (cursor.moveToNext());
					}
					cursor.close();
				} else {
					cxt.getByteWriter().Write(0);
				}
			}
		}
	}

	private void queryAlbums(ProviderExecuteContext cxt) {
		synchronized (syncObj) {
			if (albums != null) {
				cxt.getByteWriter().Write(albums.size());
				for (int i = 0; i < albums.size(); i++)
					writeAlbumInfo(albums.get(i).Album, albums.get(i).AlbumArt,
							cxt.getByteWriter());
			} else {
				albums = new ArrayList<AlbumInfo>();

				String[] columns = new String[] { Audio.Albums.ALBUM,
						Audio.Albums.ALBUM_ART };
				Cursor cursor = context.getContentResolver().query(
						Audio.Albums.EXTERNAL_CONTENT_URI, columns, null, null,
						null);
				if (cursor != null) {
					cxt.getByteWriter().Write(cursor.getCount());
					if (cursor.moveToFirst()) {
						do {
							if (cxt.isCancel()) {
								if (albums != null) {
									albums.clear();
									albums = null;
								}
							}
							AlbumInfo info = new AlbumInfo();
							info.Album = cursor.getString(0);
							info.AlbumArt = cursor.getString(1);
							putAlbumCache(info);
							writeAlbumInfo(info.Album, info.AlbumArt,
									cxt.getByteWriter());
						} while (cursor.moveToNext());
					}
					cursor.close();
				} else {
					cxt.getByteWriter().Write(0);
				}
			}
		}
	}

	private void queryArtists(ProviderExecuteContext cxt) {
		synchronized (syncObj) {
			if (artists != null) {
				cxt.getByteWriter().Write(artists.size());
				for (int i = 0; i < artists.size(); i++)
					cxt.getByteWriter().Write(artists.get(i));
			} else {
				artists = new ArrayList<String>();

				String[] columns = new String[] { Audio.Artists.ARTIST };
				Cursor cursor = context.getContentResolver().query(
						Audio.Artists.EXTERNAL_CONTENT_URI, columns, null,
						null, null);
				if (cursor != null) {
					cxt.getByteWriter().Write(cursor.getCount());
					if (cursor.moveToFirst()) {
						do {
							if (cxt.isCancel()) {
								artists.clear();
								artists = null;
								break;
							}
							String artist = cursor.getString(0);
							artists.add(artist);
							cxt.getByteWriter().Write(artist);
						} while (cursor.moveToNext());
					}
					cursor.close();
				} else {
					cxt.getByteWriter().Write(0);
				}
			}
		}
	}

	private void queryPlaylist(ProviderExecuteContext cxt) {
		synchronized (syncObj) {
			if (playlists != null) {
				cxt.getByteWriter().Write(playlists.size());
				for (int i = 0; i < playlists.size(); i++)
					writePlaylistInfo(playlists.get(i), cxt.getByteWriter());
			} else {
				playlists = new ArrayList<Playlist>();

				String[] columns = new String[] { Audio.Playlists._ID,
						Audio.Playlists.NAME };
				Cursor cursor = context.getContentResolver().query(
						Audio.Playlists.EXTERNAL_CONTENT_URI, columns, null,
						null, null);
				if (cursor != null) {
					cxt.getByteWriter().Write(cursor.getCount());
					if (cursor.moveToFirst()) {
						do {
							if (cxt.isCancel()) {
								playlists.clear();
								playlists = null;
								break;
							}
							Playlist playlist = new Playlist();
							playlist.Id = cursor.getInt(0);
							playlist.Name = cursor.getString(1);
							playlists.add(playlist);
							writePlaylistInfo(playlist, cxt.getByteWriter());
						} while (cursor.moveToNext());
					}
					cursor.close();
				} else {
					cxt.getByteWriter().Write(0);
				}
			}
		}
	}

	private void queryPlaylistItems(ProviderExecuteContext cxt) {
		int playlistId = cxt.getByteReader().ReadInt32();
		Uri uri = getPlaylistUri(playlistId);
		String[] columns = new String[] { Audio.Playlists.Members._ID,
				Audio.Playlists.Members.AUDIO_ID,
				Audio.Playlists.Members.PLAY_ORDER };
		Cursor cursor = context.getContentResolver().query(uri, columns, null,
				null, Audio.Playlists.Members.PLAY_ORDER);
		if (cursor != null) {
			cxt.getByteWriter().Write(cursor.getCount());
			if (cursor.moveToFirst()) {
				do {
					if (cxt.isCancel())
						break;
					cxt.getByteWriter().Write(cursor.getInt(0));
					cxt.getByteWriter().Write(cursor.getInt(1));
					cxt.getByteWriter().Write(cursor.getInt(2));
				} while (cursor.moveToNext());
			}
			cursor.close();
		} else {
			cxt.getByteWriter().Write(0);
		}
	}

	private void queryVideos(ProviderExecuteContext cxt) {
		synchronized (syncObj) {
			if (videos != null) {
				cxt.getByteWriter().Write(videos.size());
				for (int i = 0; i < videos.size(); i++)
					writeVideoInfo(videos.get(i), cxt.getByteWriter());
			} else {
				videos = new ArrayList<VideoInfo>();

				String[] columns = new String[] { Video.Media._ID,
						Video.Media.DATA, Video.Media.TITLE,
						Video.Media.DURATION, Video.Media.ARTIST,
						Video.Media.ALBUM };
				Cursor cursor = context.getContentResolver().query(
						Video.Media.EXTERNAL_CONTENT_URI, columns, null, null,
						null);
				if (cursor != null) {
					cxt.getByteWriter().Write(cursor.getCount());
					if (cursor.moveToFirst()) {
						do {
							if (cxt.isCancel()) {
								videos.clear();
								videos = null;
								break;
							}
							VideoInfo video = readVideoInfo(cursor);
							videos.add(video);
							writeVideoInfo(video, cxt.getByteWriter());
						} while (cursor.moveToNext());
					}
					cursor.close();
				} else {
					cxt.getByteWriter().Write(0);
				}
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

	private String getDBAlbumArt(String albumName) {
		String art = "";
		int albumId = getAlbumId(albumName);
		if (albumId >= 0) {
			Uri uri = Uri.parse("content://media/external/audio/albumart");
			uri = ContentUris.withAppendedId(uri, albumId);
			Cursor cursor = context.getContentResolver().query(uri,
					new String[] { "_data" }, null, null, null);
			if (cursor != null) {
				if (cursor.moveToFirst())
					art = cursor.getString(0);
				cursor.close();
			}
		}
		return art;
	}

	private int getAlbumId(String album) {
		int albumId = -1;
		String[] columns = new String[] { Audio.Albums._ID, Audio.Albums.ALBUM };
		Cursor cursor = context.getContentResolver().query(
				Audio.Albums.EXTERNAL_CONTENT_URI, columns, null, null, null);
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
		int isNotification = ((type & RingtoneManager.TYPE_NOTIFICATION) > 0) ? 1
				: 0;
		AudioInfo info = addAudio(data, title, artist, album, artPath, size,
				format, modified, duration, track, year, type, isRingtone,
				isMusic, isAlarm, isNotification);
		if (info == null) {
			info = addAudio(data);
		}
		if (info != null) {
			writeAudioInfo(info, cxt.getByteWriter());
		}
	}

	private void removeAudio(ProviderExecuteContext cxt) {
		int id = cxt.getByteReader().ReadInt32();
		Uri uri = ContentUris.withAppendedId(Media.EXTERNAL_CONTENT_URI, id);
		context.getContentResolver().delete(uri, null, null);
		cxt.getByteWriter().WriteString("OKAY");
	}

	private void renameAudio(ProviderExecuteContext cxt) {
		int id = cxt.getByteReader().ReadInt32();
		String title = cxt.getByteReader().ReadString();
		Uri uri = ContentUris.withAppendedId(Media.EXTERNAL_CONTENT_URI, id);
		ContentValues values = new ContentValues();
		values.put(Media.TITLE, title);
		context.getContentResolver().update(uri, values, null, null);
		cxt.getByteWriter().WriteString("OKAY");
	}

	private void modifyAudio(ProviderExecuteContext cxt) {
		int trackId = cxt.getByteReader().ReadInt32();
		String album = cxt.getByteReader().ReadString();
		String artist = cxt.getByteReader().ReadString();

		Uri trackUri = ContentUris.withAppendedId(
				Audio.Media.EXTERNAL_CONTENT_URI, trackId);

		ContentValues values = new ContentValues();
		if (!album.equals(""))
			values.put(Audio.Media.ALBUM, album);
		if (!artist.equals(""))
			values.put(Audio.Media.ARTIST, artist);
		if (values.size() > 0)
			context.getContentResolver().update(trackUri, values, null, null);
		cxt.getByteWriter().WriteString("OKAY");
	}

	private void addPlaylist(ProviderExecuteContext cxt) {
		String name = cxt.getByteReader().ReadString();

		ContentValues values = new ContentValues();
		values.put(Audio.Playlists.NAME, name);

		Uri uri = context.getContentResolver().insert(
				Audio.Playlists.EXTERNAL_CONTENT_URI, values);
		if (uri != null) {
			Cursor cursor = context.getContentResolver().query(uri,
					new String[] { Audio.Playlists._ID, Audio.Playlists.NAME },
					null, null, null);
			if (cursor != null) {
				if (cursor.moveToFirst()) {
					cxt.getByteWriter().Write(0);
					cxt.getByteWriter().Write(1);
				}
				cursor.close();
			} else {
				cxt.getByteWriter().WriteString("FAIL");
			}
		}
	}

	private void renamePlaylist(ProviderExecuteContext cxt) {
		int id = cxt.getByteReader().ReadInt32();
		String name = cxt.getByteReader().ReadString();

		Uri uri = ContentUris.withAppendedId(
				Audio.Playlists.EXTERNAL_CONTENT_URI, id);
		ContentValues values = new ContentValues();
		values.put(Audio.Playlists.NAME, name);

		ContentResolver cr = context.getContentResolver();
		cr.update(uri, values, null, null);
		cxt.getByteWriter().WriteString("OKAY");
	}

	private void removePlaylist(ProviderExecuteContext cxt) {
		int id = cxt.getByteReader().ReadInt32();
		Uri uri = ContentUris.withAppendedId(
				Audio.Playlists.EXTERNAL_CONTENT_URI, id);
		ContentResolver cr = context.getContentResolver();
		cr.delete(uri, null, null);
		cxt.getByteWriter().WriteString("OKAY");
	}

	private void addTrackToPlaylist(ProviderExecuteContext cxt) {
		int playlistId = cxt.getByteReader().ReadInt32();
		int trackId = cxt.getByteReader().ReadInt32();
		int order = cxt.getByteReader().ReadInt32();

		Uri mapUri = addTrackToPlaylist(playlistId, trackId, order);
		Uri uri = getPlaylistUri(playlistId);
		if (mapUri != null) {
			long mapId = ContentUris.parseId(mapUri);
			Cursor cursor = context.getContentResolver().query(
					uri,
					new String[] { Audio.Playlists.Members._ID,
							Audio.Playlists.Members.AUDIO_ID,
							Audio.Playlists.Members.PLAY_ORDER }, null, null,
					Audio.Playlists.Members.PLAY_ORDER);
			if (cursor != null) {
				if (cursor.moveToFirst()) {
					do {
						if (cursor.getLong(0) == mapId) {
							cxt.getByteWriter().Write((int) mapId);
							cxt.getByteWriter().Write(cursor.getInt(1));
							cxt.getByteWriter().Write(cursor.getInt(2));
							break;
						}
					} while (cursor.moveToNext());
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
			cxt.getByteWriter().WriteString("OKAY");
		} else {
			cxt.getByteWriter().WriteString("FAIL");
		}
	}

	private void setRing(ProviderExecuteContext cxt) {
		int id = cxt.getByteReader().ReadInt32();
		boolean isSystem = cxt.getByteReader().ReadBoolean();
		int kind = cxt.getByteReader().ReadInt32();

		Uri root = isSystem ? Media.INTERNAL_CONTENT_URI
				: Media.EXTERNAL_CONTENT_URI;
		Uri mediaUri = ContentUris.withAppendedId(root, id);
		if (!isSystem)
			changeAudioType(mediaUri, kind);
		switch (kind) {
		case RingtoneManager.TYPE_RINGTONE:
			Settings.System.putString(context.getContentResolver(),
					Settings.System.RINGTONE, mediaUri.toString());
			break;
		case RingtoneManager.TYPE_NOTIFICATION:
			Settings.System.putString(context.getContentResolver(),
					Settings.System.NOTIFICATION_SOUND, mediaUri.toString());
			break;
		}
		cxt.getByteWriter().WriteString("OKAY");
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

		VideoInfo info = addVideo(data, title, artist, album, size, format,
				duration);
		if (info == null)
			info = addVideo(data);
		if (info != null) {
			writeVideoInfo(info, cxt.getByteWriter());
		} else {
			LogCenter.error("Media provider on add video", "Add video failed.");
			cxt.getByteWriter().WriteString("FAIL");
		}
	}

	private void removeVideo(ProviderExecuteContext cxt) {
		int id = cxt.getByteReader().ReadInt32();
		Uri uri = ContentUris.withAppendedId(Video.Media.EXTERNAL_CONTENT_URI,
				id);
		context.getContentResolver().delete(uri, null, null);
		cxt.getByteWriter().WriteString("OKAY");
	}

	private void renameVideo(ProviderExecuteContext cxt) {
		ByteReader reader = cxt.getByteReader();
		int id = reader.ReadInt32();
		String title = reader.ReadString();
		Uri uri = ContentUris.withAppendedId(Video.Media.EXTERNAL_CONTENT_URI,
				id);
		ContentValues values = new ContentValues();
		values.put(Video.Media.TITLE, title);
		int executed = context.getContentResolver().update(uri, values, null,
				null);
		if (executed > 0)
			cxt.getByteWriter().WriteString("OKAY");
		else
			cxt.getByteWriter().WriteString("FAIL");
	}

	private void addAlbumArt(ProviderExecuteContext cxt) {
		String albumTitle = cxt.getByteReader().ReadString();
		String art = cxt.getByteReader().ReadString();

		int albumId = getAlbumId(albumTitle);
		boolean result = false;
		if (albumId >= 0) {
			result = putAlbumArt(albumId, art);
			AlbumInfo info = new AlbumInfo();
			info.Album = albumTitle;
			info.AlbumArt = art;
			putAlbumCache(info);
		}
		if (result)
			cxt.getByteWriter().WriteString("OKAY");
		else
			cxt.getByteWriter().WriteString("FAIL");
	}

	private void removeAlbumArt(ProviderExecuteContext cxt) {
		String album = cxt.getByteReader().ReadString();
		int albumId = getAlbumId(album);

		Uri artUri = Uri.parse("content://media/external/audio/albumart");
		Uri albumArtUri = ContentUris.withAppendedId(artUri, albumId);
		context.getContentResolver().delete(albumArtUri, null, null);
		cxt.getByteWriter().WriteString("OKAY");
	}

	private void changeAudioType(ProviderExecuteContext cxt) {
		ByteReader reader = cxt.getByteReader();
		int id = reader.ReadInt32();
		int type = reader.ReadInt32();
		Uri uri = ContentUris.withAppendedId(Audio.Media.EXTERNAL_CONTENT_URI,
				id);
		if (changeAudioType(uri, type))
			cxt.getByteWriter().WriteString("OKAY");
		else
			cxt.getByteWriter().Write("FAIL");
	}

	private void playAudio(final ProviderExecuteContext cxt) {
		boolean result = false;
		String ringPath = cxt.getByteReader().ReadString();
		if (ringPath != null && !ringPath.equals("")) {
			try {
				if (player != null && player.isPlaying()) {
					player.stop();
					player.reset();
				}
				player = MediaPlayer.create(this.context, Uri.parse(ringPath));
				if (player != null) {
					player.setOnCompletionListener(new OnCompletionListener() {

						@Override
						public void onCompletion(MediaPlayer player) {
							ByteWriter writer = new ByteWriter();
							writer.Write(true);
							writer.Write(0);
							ConnectionManager.getInstance().sendMessage(getBusiness(), cxt.getAccess().getConnectionId(), writer);						
						}
					});
					player.start();
					result = true;
				}
			} catch (Exception e) {
				LogCenter.error("Media Provide on Play audio", e.getMessage());
			}
		}
		cxt.getByteWriter().Write(17);
		cxt.getByteWriter().Write(result);
	}

	private void stopPlay(ProviderExecuteContext cxt) {
		if (player != null && player.isPlaying()) {
			player.stop();
			player.reset();
		}
		cxt.getByteWriter().Write(19);
		cxt.getByteWriter().Write(true);
	}

	private void readAudioTag(ProviderExecuteContext cxt) {
		String path = cxt.getByteReader().ReadString();
		AudioTagInfo info = ReadAudioTagInfo(path);
		if (info.Success) {
			cxt.getByteWriter().WriteString("OKAY");
			if (info.Header != null) {
				cxt.getByteWriter().Write(info.Header.length);
				cxt.getByteWriter().Write(info.Header);
			} else {
				cxt.getByteWriter().Write(0);
			}
			if (info.Bottom != null) {
				cxt.getByteWriter().Write(info.Bottom.length);
				cxt.getByteWriter().Write(info.Bottom);
			} else {
				cxt.getByteWriter().Write(0);
			}
		} else {
			cxt.getByteWriter().WriteString("FAIL");
		}
	}

	private void queryAudioCount(ProviderExecuteContext cxt) {
		int cnt = getAudioCount();
		cxt.getByteWriter().Write(cnt);
	}

	private void queryVideoCount(ProviderExecuteContext cxt) {
		int cnt = GetVideoCount();
		cxt.getByteWriter().Write(cnt);
	}

	private void getTotalAudioSize(ProviderExecuteContext cxt) {
		Cursor cursor = context.getContentResolver().query(
				Media.EXTERNAL_CONTENT_URI, new String[] { "sum(_size)" },
				null, null, null);
		long size = 0L;
		if (cursor != null) {
			if (cursor.moveToFirst())
				size = cursor.getLong(0);
			cursor.close();
		}
		cxt.getByteWriter().Write(size);
	}

	private void getTotalVideoSize(ProviderExecuteContext cxt) {
		Cursor cursor = context.getContentResolver().query(
				Video.Media.EXTERNAL_CONTENT_URI,
				new String[] { "sum(_size)" }, null, null, null);
		long size = 0L;
		if (cursor != null) {
			if (cursor.moveToFirst())
				size = cursor.getLong(0);
			cursor.close();
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
				Class<?> classType = Class
						.forName("android.media.MediaMetadataRetriever");
				retriever = classType.newInstance();
				int mode_capture_frame_only = classType.getField(
						"MODE_CAPTURE_FRAME_ONLY").getInt(null);

				Method setMode = classType.getMethod("setMode",
						new Class[] { int.class });
				setMode.invoke(retriever,
						new Object[] { mode_capture_frame_only });

				Method setDataSource = classType.getMethod("setDataSource",
						new Class[] { String.class });
				setDataSource.invoke(retriever, new Object[] { filePath });

				Method captureFrame = classType.getMethod("captureFrame",
						new Class[] {});
				bitmap = (Bitmap) captureFrame.invoke(retriever,
						new Object[] {});
			} catch (Exception e) {
				LogCenter.error("Provider getVideoThumbnail error",
						e.getMessage());
			} finally {
				try {
					if (retriever != null) {
						Method release = retriever.getClass().getMethod(
								"release", new Class[] {});
						release.invoke(retriever, new Object[] {});
					}
				} catch (Exception e) {
					LogCenter.error("Provider getVideoThumbnail error",
							e.getMessage());
				}
			}

			byte[] bytes = getBitmapBytes(bitmap);
			if (bytes != null && bytes.length > 0) {
				writer.Write(bytes.length);
				writer.Write(bytes);
			} else {
				writer.Write(0);
			}
		}
	}

	private AudioInfo readAudioInfo(Cursor cursor) {
		AudioInfo audio = new AudioInfo();
		audio.Id = cursor.getInt(cursor.getColumnIndex(Audio.Media._ID));
		audio.Data = cursor.getString(cursor.getColumnIndex(Audio.Media.DATA));
		audio.Title = cursor
				.getString(cursor.getColumnIndex(Audio.Media.TITLE));
		audio.Duration = cursor.getInt(cursor
				.getColumnIndex(Audio.Media.DURATION));
		audio.Artist = cursor.getString(cursor
				.getColumnIndex(Audio.Media.ARTIST));
		audio.Album = cursor
				.getString(cursor.getColumnIndex(Audio.Media.ALBUM));
		audio.IsRingtone = cursor.getInt(cursor
				.getColumnIndex(Audio.Media.IS_RINGTONE)) == 1;
		audio.IsMusic = cursor.getInt(cursor
				.getColumnIndex(Audio.Media.IS_MUSIC)) == 1;
		audio.IsAlarm = cursor.getInt(cursor
				.getColumnIndex(Audio.Media.IS_ALARM)) == 1;
		audio.IsNotification = cursor.getInt(cursor
				.getColumnIndex(Audio.Media.IS_NOTIFICATION)) == 1;
		audio.Size = cursor.getLong(cursor.getColumnIndex(Audio.Media.SIZE));
		return audio;
	}

	private VideoInfo readVideoInfo(Cursor cursor) {
		VideoInfo info = new VideoInfo();
		info.Id = cursor.getInt(0);
		info.Data = cursor.getString(1);
		info.Title = cursor.getString(2);
		info.Duration = cursor.getInt(3);
		info.Artist = cursor.getString(4);
		info.Album = cursor.getString(5);
		return info;
	}

	private void writeAudioInfo(AudioInfo audio, ByteWriter writer) {
		writer.Write(audio.Id);
		writer.Write(audio.Data);
		writer.Write(audio.Title);
		writer.Write(audio.Duration);
		writer.Write(audio.Artist);
		writer.Write(audio.Album);
		writer.Write(audio.IsRingtone);
		writer.Write(audio.IsMusic);
		writer.Write(audio.IsAlarm);
		writer.Write(audio.IsNotification);
		writer.Write(audio.Size);
	}

	private void writeAlbumInfo(String album, String albumArt, ByteWriter writer) {
		writer.Write(album);
		writer.Write(albumArt);
	}

	private void writePlaylistInfo(Playlist playlist, ByteWriter writer) {
		writer.Write(playlist.Id);
		writer.Write(playlist.Name);
	}

	private void writeVideoInfo(VideoInfo video, ByteWriter writer) {
		writer.Write(video.Id);
		writer.Write(video.Data);
		writer.Write(video.Title);
		writer.Write(video.Duration);
		writer.Write(video.Artist);
		writer.Write(video.Album);
	}

	private Uri getPlaylistUri(int id) {
		String uriString = new StringBuilder()
				.append("content://media/external/audio/playlists/").append(id)
				.append("/members").toString();
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

			int maxSide = 95;
			float scale = 0;

			if (width >= height && width > maxSide)
				scale = ((float) maxSide) / width;
			else if (height > width && height > maxSide)
				scale = ((float) maxSide) / height;
			else
				scale = 1;

			Matrix matrix = new Matrix();
			matrix.postScale(scale, scale);

			Bitmap resizedBitmap = Bitmap.createBitmap(bitmap, 0, 0, width,
					height, matrix, true);

			ByteArrayOutputStream baos = new ByteArrayOutputStream();
			resizedBitmap.compress(Bitmap.CompressFormat.PNG, 100, baos);

			byte[] bytes = baos.toByteArray();
			return bytes;
		} else {
			return null;
		}
	}

	private void putAlbumCache(AlbumInfo info) {
		AlbumInfo ai = getCachedAlbumInfo(info.Album);
		if (ai != null)
			ai.AlbumArt = info.AlbumArt;
		else {
			if (albums != null)
				albums.add(info);
		}
	}

	private AlbumInfo getCachedAlbumInfo(String album) {
		if (albums != null) {
			for (int i = 0; i < albums.size(); i++) {
				AlbumInfo info = albums.get(i);
				if (info.Album.equals(album))
					return info;
			}
		}
		return null;
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
		int executed = context.getContentResolver().update(audio, values, null,
				null);
		return executed > 0;
	}

	protected class AudioInfo {
		public int Id;
		public String Data;
		public String Title;
		public int Duration;
		public String Artist;
		public String Album;
		public boolean IsRingtone;
		public boolean IsMusic;
		public boolean IsAlarm;
		public boolean IsNotification;
		public long Size;
	}

	protected class Playlist {
		public int Id;
		public String Name;
	}

	protected class AlbumInfo {
		public String Album;
		public String AlbumArt;
	}

	protected class VideoInfo {
		public int Id;
		public String Data;
		public String Title;
		public int Duration;
		public String Artist;
		public String Album;
	}

	public class AudioTagInfo {
		public boolean Success = false;
		public long TotalLength;
		public byte[] Header;
		public byte[] Bottom;
	}

	public class ProviderMediaScannerConnectionClient implements
			MediaScannerConnectionClient {
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
}
