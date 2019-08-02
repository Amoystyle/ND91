package com.nd.assistance.qcode;

import java.util.ArrayList;

import com.google.zxing.ResultPoint;
import com.google.zxing.client.android.camera.CameraManager;
import com.nd.assistance.R;


import android.content.Context;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.RectF;
import android.graphics.drawable.BitmapDrawable;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;
import android.widget.RelativeLayout;

public class ShowQrCodeView extends View {

	private int maskColor;
	private int resultColor;
	private int frameColor;
	private int laserColor;
	private int resultPointColor;
	private Paint paint;
	// 上下扫描
	private int linePositon;
	// 上下扫描的图片
	private Bitmap drawLine;
	// 扫描框所在
	public Rect scanRect;
	// 是否上下扫描
	private boolean isScaning;
	
	private  CameraManager cameraManager;

	public ShowQrCodeView(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		init(context);
	}

	public ShowQrCodeView(Context context, AttributeSet attrs) {
		super(context, attrs);
		init(context);
	}

	public ShowQrCodeView(Context context) {
		super(context);
		init(context);
	}

	
	 public void setCameraManager(CameraManager cameraManager) {
		    this.cameraManager = cameraManager;
     }

	
	private void init(Context context) {
		paint = new Paint(Paint.ANTI_ALIAS_FLAG);
		Resources resources = getResources();
		maskColor = 0x60000000;
		resultColor = 0xb0000000;
		frameColor = 0xff8dc500;
		laserColor = 0xffff0000;
		resultPointColor = 0xc0ffff00;
		BitmapDrawable bd = (BitmapDrawable) (context.getResources()
				.getDrawable(R.drawable.zxingdrawline));
		drawLine = bd.getBitmap();
		isScaning = true;
		invalidate();
	}

	@Override
	public void onDraw(Canvas canvas) {
	    if (cameraManager == null) {
		      return;
		 }
	    Rect carmeframe = cameraManager.getFramingRect();
	    if (carmeframe == null) {
	      return;
	    }
		int width = canvas.getWidth();
		int height = canvas.getHeight();
		int framRectWidth = width / 8;
		Rect frame = new Rect();
//		frame.top = height / 4;
//		frame.left = width / 4;
//		frame.right = width * 3 / 4;
//		frame.bottom = frame.top + (frame.right - frame.left);
		int rectWidth = carmeframe.right - carmeframe.left;
		int rectHeight = carmeframe.bottom - carmeframe.top;
		int diff =rectHeight-rectWidth;
		if(diff>0){
			carmeframe.top += diff/2;
			carmeframe.bottom -= diff/2;
		}else{
			carmeframe.left -= diff/2;
			carmeframe.right += diff/2;
		}
		
		frame = carmeframe;
		scanRect = frame;
		// Draw the exterior (i.e. outside the framing rect) darkened
		// 画外围背景矩形
		paint.setColor(maskColor);
		canvas.drawRect(0, 0, width, scanRect.top, paint);
		canvas.drawRect(0, scanRect.top, scanRect.left, scanRect.bottom, paint);
		canvas.drawRect(scanRect.right, scanRect.top, width, scanRect.bottom,
				paint);
		canvas.drawRect(0, scanRect.bottom, width, height, paint);

		// 画扫描行
		if (linePositon < scanRect.top) {
			linePositon = scanRect.top;
		}
		if (linePositon > scanRect.bottom - 18) {
			linePositon = scanRect.top;
		}
		RectF lineRect = new RectF();
		lineRect.left = scanRect.left + 6;
		lineRect.top = linePositon;
		lineRect.right = scanRect.right - 6;
		lineRect.bottom = linePositon + 6;
		if(isScaning){
			canvas.drawBitmap(drawLine, null, lineRect, null);
		}
		paint.setColor(frameColor);
		canvas.drawRect(scanRect.left, scanRect.top, scanRect.left
				+ framRectWidth, scanRect.top + 6, paint);
		canvas.drawRect(scanRect.left, scanRect.top, scanRect.left + 6,
				scanRect.top + framRectWidth, paint);
		canvas.drawRect(scanRect.right - framRectWidth, scanRect.top,
				scanRect.right, scanRect.top + 6, paint);
		canvas.drawRect(scanRect.right - 6, scanRect.top, scanRect.right,
				scanRect.top + framRectWidth, paint);
		canvas.drawRect(scanRect.left, scanRect.bottom - 6, scanRect.left
				+ framRectWidth, scanRect.bottom, paint);
		canvas.drawRect(scanRect.left, scanRect.bottom - framRectWidth,
				scanRect.left + 6, scanRect.bottom, paint);
		canvas.drawRect(scanRect.right - framRectWidth, scanRect.bottom - 6,
				scanRect.right, scanRect.bottom, paint);
		canvas.drawRect(scanRect.right - 6, scanRect.bottom - framRectWidth,
				scanRect.right, scanRect.bottom, paint);
		linePositon += 6;
		invalidate();
	}

	public void startScan() {
		isScaning = true;
		
	}

	public void stopScan() {
		isScaning = false;
		
	}
}
