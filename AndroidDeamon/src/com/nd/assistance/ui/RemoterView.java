package com.nd.assistance.ui;


import java.util.ArrayList;
import java.util.List;

import com.nd.assistance.R;

import daemon.receiver.SendMessageType;


import android.content.Context;
import android.graphics.*;
import android.util.AttributeSet;
import android.view.*;


public class RemoterView extends View {

	// 中间按钮
	private final float centerCircle = 57f/354f;
	private final float centerCircle_2 = 48f/354f;
	// 按键距离
	private final float menuDistance = 84f/230f;
	// 按钮
	private final float menuWidth = 55f/177f;
	
	//中间的大圆图片
	private Bitmap mCircleBmp;
	
	//背景图片图片
	private Bitmap mBgBmp;
	
	
	private int mWidth;
	

	private final int NORMAL =0;
	private final int CENTER =1;
	private final int UP =2;
	private final int DOWN =3;
	private final int LEFT =4;
	private final int RIGHT =5;
	/**
	 * 0 正常态
	 * 1 中间被点击
	 * 2 上被点击
	 * 3 下被被点击
	 * 4左被点击
	 * 5右被点击
	 */
	private int status =NORMAL;
	//中间按钮是否在PLAY状态
	private boolean isPlaying;
	
	
	private Context mContext; 
	
	private Paint objPaint;
	
	private  int DEVIATION = 20;
	private int horizontalDeviation =0;
	private int verticalDeviation =0;
	
	
	//加载的背景圆的长度
	private int circleWidth;
	private Rect centerRect =new Rect();
	private Rect upRect =new Rect();
	private Rect downRect =new Rect();
	private Rect leftRect =new Rect();
	private Rect rightRect =new Rect();
	
	interface ListerRemoterViewClick{
		public void OnClick(int action);
	}
	
	private ListerRemoterViewClick mListerRemoterViewClick;
	
	public void SetListerRemoterViewClick(ListerRemoterViewClick o){
		mListerRemoterViewClick = o;
	}
	
	public RemoterView(Context context) {
		super(context);
		init(context);
	}
	public RemoterView(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		init(context);
	}


	public RemoterView(Context context, AttributeSet attrs) {
		super(context, attrs);
		init(context);
	}
	
	private void init(Context context) {
		mContext = context;
		mCircleBmp = BitmapFactory.decodeResource(context.getResources(), R.drawable.circlebg);
		circleWidth = mCircleBmp.getWidth();
		isPlaying = true;
		objPaint = new Paint();
		objPaint.setAntiAlias(true);
		//objPaint.setAntiAlias(false); 
	//	objPaint.setFilterBitmap(false); 
		DEVIATION = dip2px(context, DEVIATION);
	}



	@Override
	public boolean onTouchEvent(MotionEvent e) {
		int state = e.getAction();
		int eventX = (int) e.getX();
		int eventY = (int) e.getY();
		if (state == MotionEvent.ACTION_DOWN) {
			
			if(centerRect.contains(eventX, eventY)){
				status = CENTER;
			}else if(upRect.contains(eventX, eventY)){
				status = UP;
			}else if(downRect.contains(eventX, eventY)){
				status = DOWN;
			}else if(leftRect.contains(eventX, eventY)){
				status = LEFT;
			}else if(rightRect.contains(eventX, eventY)){
				status = RIGHT;
			}

		} else if (state == MotionEvent.ACTION_UP) {
			if(mListerRemoterViewClick!=null){
				switch (status) {
				case CENTER:
					if(isPlaying){
						mListerRemoterViewClick.OnClick(SendMessageType.SYSTEM_PC_PLAY);
					}else{
						mListerRemoterViewClick.OnClick(SendMessageType.SYSTEM_PC_STOP);
					}
					isPlaying =!isPlaying;
					break;
				case LEFT:
					mListerRemoterViewClick.OnClick(SendMessageType.SYSTEM_PC_LAST);
					break;
				case RIGHT:
					mListerRemoterViewClick.OnClick(SendMessageType.SYSTEM_PC_NEXT);
					break;
				case UP:
					mListerRemoterViewClick.OnClick(SendMessageType.SYSTEM_PC_VOLADD);
					break;
				case DOWN:
					mListerRemoterViewClick.OnClick(SendMessageType.SYSTEM_PC_VOLREDUCE);
					break;
				default:
					break;
				}
			}
			status = NORMAL;
		}
		invalidate();
		return true;
	}

	
	public void setPlaying(boolean playing){
		isPlaying = playing;
		status = CENTER;
		postInvalidate();
		status = NORMAL;
		postInvalidate();
	}
	
	
	@Override
    protected void onDraw(Canvas c) {
		super.onDraw(c);
		int width = getWidth();
		final int orginWidth = width;
		int height = getHeight();
		if(width!=0&&height!=0){
			
			if(width>height){
				width = height;
			}
			mWidth = width/2;
			
			horizontalDeviation = (orginWidth - width)/2 + DEVIATION;
			verticalDeviation = (height -width)/2 + DEVIATION;
			width = width - 2*DEVIATION;
				
			Rect cricleRect = new Rect();
			cricleRect.left = horizontalDeviation;
			cricleRect.right = cricleRect.left+ width;
			cricleRect.top = verticalDeviation;
			cricleRect.bottom = cricleRect.top + width;
			//画背景圆
			if(mCircleBmp!=null){
				c.drawBitmap(mCircleBmp, null, cricleRect, objPaint);
			}
			//画背景状态
			Bitmap bmp = null;
			//画中间圆
			if(status==CENTER){
				if(isPlaying){
					bmp = BitmapFactory.decodeResource(mContext.getResources(), R.drawable.play_press);
				}else{
					bmp = BitmapFactory.decodeResource(mContext.getResources(), R.drawable.stop_press);
				}
				
			}else{
				if(isPlaying){
					bmp = BitmapFactory.decodeResource(mContext.getResources(), R.drawable.play_normal);
				}else{
					bmp = BitmapFactory.decodeResource(mContext.getResources(), R.drawable.stop_normal);
				}
			}
			if(bmp!=null){
				float center = centerCircle;
				if(isPlaying){
					center = centerCircle_2;
				}
				
				centerRect.left = horizontalDeviation + (int) ((1f-center)*width/2);
				centerRect.right = centerRect.left + (int)((centerCircle)*width);
				centerRect.top = verticalDeviation + (int) ((1f-centerCircle)*width/2);
				centerRect.bottom = centerRect.top + (int)((centerCircle)*width);
				c.drawBitmap(bmp, null, centerRect, objPaint);
				bmp.recycle();
				bmp=null;
			}
			//画上边
			if(status==UP){
				bmp = BitmapFactory.decodeResource(mContext.getResources(), R.drawable.voladd_press);
			}else{
				bmp = BitmapFactory.decodeResource(mContext.getResources(), R.drawable.voladd_normal);
			}
			if(bmp!=null){
				upRect.left =horizontalDeviation+ (int) (width/2 - menuWidth*width/2/2);
				upRect.right = upRect.left + (int)((menuWidth)*width/2);
				upRect.top =  verticalDeviation+(int) ((1-centerCircle - menuDistance - menuWidth)*width/2);
				upRect.bottom = verticalDeviation+(int) ((1-centerCircle - menuDistance )*width/2);
				c.drawBitmap(bmp, null, upRect, objPaint);
				bmp.recycle();
				bmp=null;
			}
			//画下边
			if(status==DOWN){
				bmp = BitmapFactory.decodeResource(mContext.getResources(), R.drawable.volreduce_press);
			}else{
				bmp = BitmapFactory.decodeResource(mContext.getResources(), R.drawable.volreduce_normal);
			}
			if(bmp!=null){
				downRect.left = horizontalDeviation +(int) (width/2 - menuWidth*width/2/2);
				downRect.right = downRect.left + (int)((menuWidth)*width/2);
				downRect.top =  verticalDeviation+width/2 +  (int) ((centerCircle + menuDistance )*width/2);
				downRect.bottom = downRect.top + (int) (menuWidth*width/2);
				c.drawBitmap(bmp, null, downRect, objPaint);
				bmp.recycle();
				bmp=null;
			}
			//画左边
			if(status==LEFT){
				bmp = BitmapFactory.decodeResource(mContext.getResources(), R.drawable.last_press);
			}else{
				bmp = BitmapFactory.decodeResource(mContext.getResources(), R.drawable.last_normal);
			}
			if(bmp!=null){
				leftRect.left =horizontalDeviation+ (int) ((1-centerCircle - menuDistance - menuWidth)*width/2);
				leftRect.right =horizontalDeviation+ (int) ((1-centerCircle - menuDistance )*width/2);
				leftRect.top = verticalDeviation+(int) (width/2 - menuWidth*width/2/2);
				leftRect.bottom = leftRect.top + (int)((menuWidth)*width/2);
				c.drawBitmap(bmp, null, leftRect, objPaint);
				bmp.recycle();
				bmp=null;
			}
			//画右边
			if(status==RIGHT){
				bmp = BitmapFactory.decodeResource(mContext.getResources(), R.drawable.next_press);
			}else{
				bmp = BitmapFactory.decodeResource(mContext.getResources(), R.drawable.next_normal);
			}
			if(bmp!=null){
				rightRect.left =  horizontalDeviation + width/2 +  (int) ((centerCircle + menuDistance )*width/2);
				rightRect.right = rightRect.left + (int) (menuWidth*width/2);
				rightRect.top = verticalDeviation + (int) (width/2 - menuWidth*width/2/2);
				rightRect.bottom = rightRect.top + (int)((menuWidth)*width/2);
				c.drawBitmap(bmp, null, rightRect, objPaint);
				bmp.recycle();
				bmp=null;
			}
			
		}
	}

	public int dip2px(Context context, float dipValue){   
        final float scale = context.getResources().getDisplayMetrics().density;   
        return (int)(dipValue * scale + 0.5f);   
    } 
	  
}
