package daemon.provider.media;

import daemon.logger.LogCenter;

import android.content.Context;
import android.media.MediaScannerConnection;
import android.net.Uri;

public class MediaScanner {

	private MediaScannerConnection mediaScanConn = null;

	private MusicSannerClient client = null;

	private String mFilePath = null;
	private String mFileType = null;
	private Uri mUri = null;
	private int mTimeOut = 5000;
	private boolean mScanCompleted = false;

	private static MediaScanner mInstance = null;

	private MediaScanner(Context context) {
		// 创建MusicSannerClient
		if (client == null) {

			client = new MusicSannerClient();
		}

		if (mediaScanConn == null) {

			mediaScanConn = new MediaScannerConnection(context, client);
		}
	}

	public static MediaScanner getInstance(Context ctx) {
		if (mInstance == null) {
			mInstance = new MediaScanner(ctx);
		}

		return mInstance;
	}

	class MusicSannerClient implements
			MediaScannerConnection.MediaScannerConnectionClient {

		public void onMediaScannerConnected() {
			LogCenter.error("MediaScanner", "Connected");
			if (mFilePath != null) {
				mediaScanConn.scanFile(mFilePath, mFileType);
			}
		}

		@Override
		public void onScanCompleted(String path, Uri uri) {
			// TODO Auto-generated method stub
			mUri = uri;
			mediaScanConn.disconnect();
			mScanCompleted = true;
			LogCenter.error("MediaScanner",
					"ScanCompleted mUri:" + mUri.toString());
		}
	}

	public Uri scanFile(String filePath, String fileType) {
		LogCenter.error("MediaScanner", "Begin");
		synchronized (this) {
			this.mFilePath = filePath;
			this.mFileType = fileType;
			mScanCompleted = false;
			this.mediaScanConn.connect();

			int sleepTime = 100;
			int sleepCnt = mTimeOut / sleepTime;
			while (sleepCnt > 0) {
				if (mScanCompleted)
					break;

				try {
					Thread.sleep(sleepTime);
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				sleepCnt--;
			}

			LogCenter.error("MediaScanner", "End");
			return mUri;
		}
	}
}
