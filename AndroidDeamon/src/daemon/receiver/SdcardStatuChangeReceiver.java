package daemon.receiver;

import daemon.connection.ConnectionManager;
import daemon.protocol.ByteWriter;
import daemon.util.ConfigManager;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class SdcardStatuChangeReceiver extends BroadcastReceiver {

	
	@Override
	public void onReceive(Context context, Intent intent) {
        try {
    		String action = intent.getAction();
    		if (action.equals(Intent.ACTION_MEDIA_MOUNTED)) 
    		{
    			// SD卡已经成功挂载
    			sendMessage(0);
    		} else if (action.equals(Intent.ACTION_MEDIA_REMOVED)|| action.equals(Intent.ACTION_MEDIA_UNMOUNTED)
    				|| action.equals(Intent.ACTION_MEDIA_BAD_REMOVAL)) {
    			// 各种未挂载状态
    			String state = android.os.Environment.getExternalStorageState();
    			if(!state.equals(android.os.Environment.MEDIA_MOUNTED)){
    				sendMessage(1);
    			}
    		} else if (action.equals(Intent.ACTION_MEDIA_SHARED)) {
    			// 扩展介质的挂载被解除 (unmount)。因为它已经作为/ USB 大容量存储被共享
    			sendMessage(2);
    		}
		} catch (Exception e) {
			// TODO: handle exception
		}

		
	}
	
	private void sendMessage(int status) {
        //0成功挂载
		//1未挂载
		//2作为 USB 大容量存储被共享
		ByteWriter writer = new ByteWriter();
		writer.Write(SendMessageType.SYSTEM_SDCARDSTATUS);
		writer.Write(status);
		ConnectionManager.getInstance().sendMessage(SendMessageType.SYSTEM_BUSINESS_ID, writer);
	}





}
