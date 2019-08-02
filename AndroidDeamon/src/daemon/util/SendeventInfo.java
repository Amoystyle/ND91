package daemon.util;

import java.util.ArrayList;

import android.content.Context;
import android.util.Log;
import daemon.ndk.NdkManager;
import daemon.shell.Su;

public class SendeventInfo {
    //动作
	 private int  action;
    //坐标
	 public int x;
	 public int y;
	 
	public int getAction() {
		return action;
	}
	public void setAction(int action) {
		this.action = action;
	}
	public int getX() {
		return x;
	}
	public void setX(int x) {
		this.x = x;
	}
	public int getY() {
		return y;
	}
	public void setY(int y) {
		this.y = y;
	} 
	
	
    
}
