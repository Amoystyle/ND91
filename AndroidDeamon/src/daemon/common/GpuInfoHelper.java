package daemon.common;

import java.util.Random;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.os.Handler;
import android.widget.Toast;

public class GpuInfoHelper {
	private static GpuInfoHelper mInstance = null;
	private Object mLock = new Object();
	private GpuInfo mInfo = null;
	
	public synchronized static GpuInfoHelper getInstance(){
		if(mInstance == null)
			mInstance = new GpuInfoHelper();
		
		return mInstance;
	}
	
	private GpuInfoHelper(){
		
	}
	
	public synchronized GpuInfo getGpuInfo(final Context c){
		if(mInfo != null)
			return mInfo;
				
		try {
			Handler handler = new Handler(c.getApplicationContext().getMainLooper());
			handler.post(new Runnable() {
				@Override
				public void run() {
					try{
						GLSurfaceView glView = new GpuGLSurfaceView(c); 
						glView.getHolder().setFormat(PixelFormat.TRANSLUCENT);
						Toast  toast = new Toast(c);
						toast.setDuration(1);
					    toast.setView(glView);
					    toast.show();
					}catch (Exception e) {
						
					}

				}				
				
			});	
			
		}
		catch (Exception e) {

		}
		
		synchronized (mLock) {
			try {
				mLock.wait(3000);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}

		return mInfo;
	}

	
	/*
	 * GLSurfaceView用于获取设备GPU
	 * */
	//private static GLSurfaceView mGLView;
	class GlRenderer implements GLSurfaceView.Renderer{
	    Random aleatorio = new Random();
        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {          
            gl.glClearColor(0, 0, 0, 0);        
            
            mInfo = new GpuInfo(gl.glGetString(GL10.GL_VENDOR), gl.glGetString(GL10.GL_RENDERER));
            
            synchronized (mLock) {            	
				mLock.notify();
			}
        }
        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {
        }
        @Override
        public void onDrawFrame(GL10 gl) {
            gl.glClear(GL10.GL_COLOR_BUFFER_BIT);
        }
	}
	
	public  class GpuGLSurfaceView extends GLSurfaceView {

		GlRenderer mRenderer;
		public GpuGLSurfaceView(Context context) {
			super(context);
			setEGLConfigChooser(8, 8, 8, 8, 16, 0);
			mRenderer = new GlRenderer();
			setRenderer(mRenderer);
		}
	}
	
	public class GpuInfo
	{
		private String mVendor;
		private String mRenderer;
		
		public GpuInfo(String vendor, String renderer){
			mVendor = vendor;
			mRenderer = renderer;
		}
		
		public String getVendor(){
			return mVendor;
		}
		
		public String getRenderer(){
			return mRenderer;
		}
	}
}
