package daemon.receiver;

import daemon.connection.ConnectionManager;
import daemon.protocol.ByteWriter;
import daemon.util.ConfigManager;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class BatteryReceiver extends BroadcastReceiver {
	private int power = 0;
	private int firstPower,lastPower;
	private long lastBatteryChangeTime;
	private boolean first=true,second=false;
	
	@Override
	public void onReceive(Context context, Intent intent) {
		try {
			if (intent.getAction().equals(Intent.ACTION_BATTERY_CHANGED)) {
				int level = intent.getIntExtra("level", 0);
				int scale = intent.getIntExtra("scale", 100);
				if (level > scale) {
					level /= 10;
				}

				power = level * 100 / scale;
				if(firstPower==0){
					firstPower=power;
				}
	            String info=ConfigManager.getBatterySpeed(context);
	            long oldspeed=0;
	            if(info!=null&&!info.equals("")){
	            	oldspeed=Long.parseLong(info);
				}
	            if(first){
					if(oldspeed!=0){
						sendMessage(oldspeed);
					}else{
						sendMessage(-1);
					}
	            }else{
	            	if(power>lastPower&&lastBatteryChangeTime!=0){
	                	long newSpeed=(System.currentTimeMillis()-lastBatteryChangeTime)/(power-lastPower);
	                	if(second){
	                		second=false;
	                		if(oldspeed!=0){
	                			newSpeed=(oldspeed+newSpeed)/2;	
	    						ConfigManager.setBatterySpeed(context, newSpeed+"");
	    						sendMessage(newSpeed);
	                		}
	                    }else{
	    					ConfigManager.setBatterySpeed(context, newSpeed+"");
	    					sendMessage(newSpeed);
	                    }
	            	}
	            }
				lastBatteryChangeTime=System.currentTimeMillis();
				lastPower=power;
				if(first){
					second=true;
				}
				first=false;
			}
		} catch (Exception e) {
			// TODO: handle exception
		}

	}
	
	private void sendMessage(long newSpeed) {
		//Log.e("速度", newSpeed+"");
		
		long time=0;
		if(newSpeed==-1){
			time=-1;
		}else{
			time=(100-power)*newSpeed/1000;
		}
		
		ByteWriter writer = new ByteWriter();
		writer.Write(SendMessageType.SYSTEM_BATTERY);
		writer.Write(power);
		writer.Write(time);
		ConnectionManager.getInstance().sendMessage(SendMessageType.SYSTEM_BUSINESS_ID, writer);
		//Log.e("剩余时间", time+"");
	}

	public int getPower() {
		return power;
	}



}
