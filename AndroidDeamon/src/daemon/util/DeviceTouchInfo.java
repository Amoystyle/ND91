package daemon.util;

import android.content.Context;
import android.util.Log;
import daemon.ndk.NdkManager;
import daemon.shell.Su;

public class DeviceTouchInfo {
    private int width,height;
    private boolean success;
    private boolean hasTracking;
    private int minX,minY,maxX,maxY;
    private String event;
    //是否有003a代替0030
    private boolean has3A;
    //是否有1 14a按键事件
    private boolean hasKeydownEvent;
    //是否为synaptics屏幕
    private boolean isSynaptics;
   
    
	public int getWidth() {
		return width;
	}
	public void setWidth(int width) {
		this.width = width;
	}
	public int getHeight() {
		return height;
	}
	public void setHeight(int height) {
		this.height = height;
	}
	public boolean isSuccess() {
		return success;
	}
	public void setSuccess(boolean success) {
		this.success = success;
	}
	public boolean isHasTracking() {
		return hasTracking;
	}
	public void setHasTracking(boolean hasTracking) {
		this.hasTracking = hasTracking;
	}
	public int getMinX() {
		return minX;
	}
	public void setMinX(int minX) {
		this.minX = minX;
	}
	public int getMinY() {
		return minY;
	}
	public void setMinY(int minY) {
		this.minY = minY;
	}
	public int getMaxX() {
		return maxX;
	}
	public void setMaxX(int maxX) {
		this.maxX = maxX;
	}
	public int getMaxY() {
		return maxY;
	}
	public void setMaxY(int maxY) {
		this.maxY = maxY;
	}
	public String getEvent() {
		return event;
	}
	public void setEvent(String event) {
		this.event = event;
	}
	public boolean has3A() {
		return has3A;
	}
	public void setHas3A(boolean has3a) {
		has3A = has3a;
	}
	public boolean hasKeydownEvent() {
		return hasKeydownEvent;
	}
	public void setKeydownEvent(boolean hasKeydownEvent) {
		this.hasKeydownEvent = hasKeydownEvent;
	}
	public boolean isSynaptics() {
		return isSynaptics;
	}
	public void setSynaptics(boolean isSynaptics) {
		this.isSynaptics = isSynaptics;
	}
	
	
	
    public boolean init(Context context){
        //String result=Su.getInstance().exec_with_str_result("getevent -p", 30000);
    	String result=NdkManager.execShellforStr(context, "getevent -p");
        Log.e("getTouchDeviceInfo.result", result);
        String[] events=result.split("/dev/input/event");
        int length=events.length;
        int index=-1;
        boolean isSynaptics=false;
        for (int i = 0; i <length ; i++) {
			if((events[i].contains("touch")||events[i].contains("synaptics")) &&(events[i].contains("0035")&&events[i].contains("0036")) ){
				if(events[i].contains("synaptics")&&events[i].contains("0039")){
					isSynaptics=true;
				}
				index=i;
				break;
			}
		}
        if(index==-1){
            for (int i = 0; i <length ; i++) {
    			if(events[i].contains("0035")&&events[i].contains("0036")){
    				index=i;
    				break;
    			}
    		}
        }
        if(index==-1){
        	return false;
        }
        setSynaptics(isSynaptics);
        result=events[index];
		ScreenSizeInfo screenSizeInfo = NdkManager.getScreenSizeInfo(context);
		setWidth(screenSizeInfo.getWidth());//;wm.getDefaultDisplay().getWidth());
		setHeight(screenSizeInfo.getHeight());//wm.getDefaultDisplay().getHeight());
		Log.e("screenSizeInfo.getWidth()", screenSizeInfo.getWidth()+"");
		Log.e("screenSizeInfo.getHeight()", screenSizeInfo.getHeight()+"");
        String[] info=result.split("\n");
        setEvent("/dev/input/event"+Integer.parseInt(info[0]));
        length=info.length;
        for (int i = 1; i < length; i++) {
			if(info[i].contains("0039")){
				setHasTracking(true);
			}
			if(info[i].contains("003a")){
				setHas3A(true);
			}
			if(info[i].contains("KEY")&&info[i].contains("014a")){
				setKeydownEvent(true);
			}
			if(info[i].contains("0035")){
				String[] values=info[i].split(",");
				for (int j = 0; j < values.length; j++) {
					if(values[j].contains("min")){
						String min=values[j].replace("min", "");
						min=min.replace(" ", "");
						setMinX(Integer.parseInt(min));
					}else if (values[j].contains("max")){
						String max=values[j].replace("max", "");
						max=max.replace(" ", "");
						setMaxX(Integer.parseInt(max));
					}
				}
			}else if(info[i].contains("0036")){
				String[] values=info[i].split(",");
				for (int j = 0; j < values.length; j++) {
					if(values[j].contains("min")){
						String min=values[j].replace("min", "");
						min=min.replace(" ", "");
						setMinY(Integer.parseInt(min));
					}else if (values[j].contains("max")){
						String max=values[j].replace("max", "");
						max=max.replace(" ", "");
						setMaxY(Integer.parseInt(max));
					}
				}
			}
		}       
        setSuccess(true);
        return true;
    }
    
    public int getRealX(int x){
    	return x*(getMaxX()-getMinX())/getWidth()+getMinX();
    }
    
    public int getRealY(int y){
    	return y*(getMaxY()-getMinY())/getHeight()+getMinY();
    }
    

    
    
}
