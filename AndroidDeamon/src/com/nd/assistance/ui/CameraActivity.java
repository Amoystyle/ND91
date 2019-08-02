package com.nd.assistance.ui;

import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;

import java.net.Socket;


import com.google.zxing.Result;
import com.google.zxing.client.android.camera.CameraManager;
import com.google.zxing.client.result.ParsedResult;
import com.google.zxing.client.result.ResultParser;
import com.nd.assistance.R;
import com.nd.assistance.qcode.CaptureActivityHandler;
import com.nd.assistance.qcode.ShowQrCodeView;
import com.nd.assistance.qcode.ViewfinderView;
import com.nd.assistance.qcode.ZxingCode;
import com.nd.assistance.util.ConnectHelper;
import com.nd.assistance.util.ConnectHelper.ConnectToPcLister;
import com.nd.assistance.util.ConnectHelper.QcodeInfo;



import android.app.Activity;
import android.app.AlertDialog;
import android.app.NotificationManager;
import android.app.ProgressDialog;
import android.app.AlertDialog.Builder;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnCancelListener;
import android.content.Intent;
import android.graphics.Bitmap;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.Vibrator;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.Window;
import android.widget.Button;

public class CameraActivity extends Activity implements SurfaceHolder.Callback {

	private static final String characterSet = "CHARACTER_SET";

	private final String TAG = "CameraTestActivity";

	private CameraManager cameraManager;

	private ViewfinderView viewfinderView;

	private ShowQrCodeView mShowQrCodeView;

	private CaptureActivityHandler handler;

	private boolean hasSurface;
	
	//private Dialog mLoadingDialog ;

	
	private ProgressDialog mLoadingDialog;
	
	public ViewfinderView getViewfinderView() {
		return viewfinderView;
	}

	public Handler getHandler() {
		return handler;
	}

	public CameraManager getCameraManager() {
		return cameraManager;
	}

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);// 隐藏标题
		setContentView(R.layout.zxing);
		cameraManager = new CameraManager(getApplication());
		viewfinderView = (ViewfinderView) findViewById(R.id.viewfinder_view);
		viewfinderView.setCameraManager(cameraManager);
		hasSurface = false;
		mLoadingDialog = new ProgressDialog(this);
		mShowQrCodeView = (ShowQrCodeView) findViewById(R.id.zxing_showqr);
		mShowQrCodeView.setCameraManager(cameraManager);
		Button back = (Button) findViewById(R.id.zxing_back);
		back.setOnClickListener(new View.OnClickListener() {

			@Override
			public void onClick(View v) {
				setResult(RESULT_CANCELED);
				finish();
			}
		});
		ConnectHelper.getInstance().setConnectToPcLister(new ConnectToPcLister() {
			
			@Override
			public void connect() {
				Bundle bundle = new Bundle();
				bundle.putBoolean("result", true);
				Intent mIntent = new Intent();
				mIntent.putExtras(bundle);
				CameraActivity.this.setResult(RESULT_OK, mIntent);
				finish();		
			}
		});

	}

	@Override
	protected void onResume() {
		super.onResume();
		SurfaceView surfaceView = (SurfaceView) findViewById(R.id.preview_view);
		SurfaceHolder surfaceHolder = surfaceView.getHolder();
		// inactivityTimer.onResume();
		if (hasSurface) {
			// The activity was paused but not stopped, so the surface still
			// exists. Therefore
			// surfaceCreated() won't be called, so init the camera here.
			initCamera(surfaceHolder);
		} else {
			// Install the callback and wait for surfaceCreated() to init the
			// camera.
			surfaceHolder.addCallback(this);
			surfaceHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
		}
	}

	@Override
	protected void onPause() {
		if (handler != null) {
			handler.quitSynchronously();
			handler = null;
		}
		// inactivityTimer.onPause();
		cameraManager.closeDriver();
		if (!hasSurface) {
			SurfaceView surfaceView = (SurfaceView) findViewById(R.id.preview_view);
			SurfaceHolder surfaceHolder = surfaceView.getHolder();
			surfaceHolder.removeCallback(this);
		}
		super.onPause();
	}

	@Override
	protected void onDestroy() {
		// InactivityTimer.shutdown();
		super.onDestroy();
	}

	@Override
	public void surfaceCreated(SurfaceHolder holder) {
		if (holder == null) {
			Log.e(TAG,
					"*** WARNING *** surfaceCreated() gave us a null surface!");
		}
		if (!hasSurface) {
			hasSurface = true;
			initCamera(holder);
		}

	}

	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int width,
			int height) {

	}

	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		hasSurface = false;

	}

	private void initCamera(SurfaceHolder surfaceHolder) {
		if (surfaceHolder == null) {
			throw new IllegalStateException("No SurfaceHolder provided");
		}
		if (cameraManager.isOpen()) {
			Log.w(TAG,
					"initCamera() while already open -- late SurfaceView callback?");
			return;
		}
		try {

			cameraManager.openDriver(surfaceHolder);
			// Creating the handler starts the preview, which can also throw a
			// RuntimeException.
			if (handler == null) {
				handler = new CaptureActivityHandler(this, null,null, characterSet,
						cameraManager);
			}
			// decodeOrStoreSavedBitmap(null, null);
		} catch (IOException ioe) {
			Log.w(TAG, ioe);
			// displayFrameworkBugMessageAndExit();
		} catch (RuntimeException e) {
			// Barcode Scanner has seen crashes in the wild of this variety:
			// java.?lang.?RuntimeException: Fail to connect to camera service
			Log.w(TAG, "Unexpected error initializing camera", e);
			// displayFrameworkBugMessageAndExit();
		}
	}

	public void drawViewfinder() {
		viewfinderView.drawViewfinder();
	}
	String ip;
	String port;
	/**
	 * 处理解析结果
	 */
	/**
	 * @param rawResult
	 * @param barcode
	 */
	public void handleDecode(Result rawResult, Bitmap barcode) {

		mLoadingDialog.show();
		mLoadingDialog.setContentView(R.layout.processdialog);
		mLoadingDialog.setCancelable(true);
		mLoadingDialog.setOnCancelListener(new OnCancelListener() {

			@Override
			public void onCancel(DialogInterface dialog) {
				restartPreviewAfterDelay(100);
			}
		});
		mShowQrCodeView.stopScan();
		// restartPreviewAfterDelay(100);
		Vibrator vibrator = (Vibrator) getSystemService(Context.VIBRATOR_SERVICE);
		vibrator.vibrate(1000);
		ParsedResult result = ResultParser.parseResult(rawResult);
		final String info = result.getDisplayResult();
		int type = ConnectHelper.getInstance().getCodeType(info);
		QcodeInfo qcodeInfo =ConnectHelper.getInstance().getQcodeInfo(info,type);
		if (qcodeInfo.errorCode != 0) {
			showError(qcodeInfo.errorCode);
			return;
		}
		ConnectHelper.getInstance().startTcpConnect(qcodeInfo.ip, qcodeInfo.port, mHandler);

	}
	
	private void showError(int errorCode) {
		mLoadingDialog.dismiss();
		AlertDialog.Builder builder = new Builder(CameraActivity.this);
		String error=String.format(this.getString(R.string.wifi_errorinfo),errorCode);
		builder.setMessage(error);
		builder.setTitle("提示");
		builder.setPositiveButton("重试", new
		DialogInterface.OnClickListener() {
 
			@Override
			public void onClick(DialogInterface dialog, int which) {
				restartPreviewAfterDelay(100);
			}
		});
		builder.setNegativeButton("关闭", new
		DialogInterface.OnClickListener() {

			@Override
			public void onClick(DialogInterface dialog, int which) {
                 CameraActivity.this.finish();
			}
		});
		builder.create().show();
		
	}
	Handler mHandler = new Handler(){
        public void handleMessage(Message msg) {   
            super.handleMessage(msg);   
    		if (msg.what==0) {
    			mLoadingDialog.dismiss();
    			Bundle bundle = new Bundle();
    			bundle.putBoolean("result", false);
    			Intent mIntent = new Intent();
    			mIntent.putExtras(bundle);
    			setResult(RESULT_OK, mIntent);
    			finish();
    			// restartPreviewAfterDelay(100);
    		}else if(msg.what==1){
    			ConnectHelper.getInstance().setConnectToPcLister(new ConnectToPcLister() {
					
					@Override
					public void connect() {
		    			mLoadingDialog.dismiss();
		    			Bundle bundle = new Bundle();
		    			bundle.putBoolean("result", true);
		    			Intent mIntent = new Intent();
		    			mIntent.putExtras(bundle);
		    			setResult(RESULT_OK, mIntent);
		    			finish();
						
					}
				});

    		}
       }   
	};

	public void restartPreviewAfterDelay(long delayMS) {
		mShowQrCodeView.startScan();
		if (handler != null) {
			handler.sendEmptyMessageDelayed(ZxingCode.restart_preview, delayMS);
		}
	}

	


}