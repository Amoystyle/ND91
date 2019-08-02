package com.nd.assistance.controller;

import com.nd.assistance.server.ServerService;
import com.nd.assistance.ui.AssistanceActivity;
import com.nd.assistance.util.ProductConstants;
import daemon.logger.LogCenter;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;

public class DaemonController extends BroadcastReceiver {
	public void onReceive(Context context, Intent intent) {
		String action = intent.getAction();
		
		boolean server_in_action = ProductConstants.ACTION_START_SERVICE.equals(action);
        LogCenter.debug("DaemonController", "Receiver action : "+action);
		if (server_in_action) {	
			StartServer(context, false);
		}else if (ProductConstants.ACTION_RELEASE_SERVICE.equals(action)) {			
			Intent serverIntent = new Intent(context, ServerService.class);
			context.stopService(serverIntent);
		}else if (ProductConstants.ACTION_START_MAIN_ACTIVITY.equals(action)){			
			Intent i = new Intent(context, AssistanceActivity.class);
			i.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
			context.startActivity(i);
		}
	}

	public static void StartServer(Context context, boolean isStartByUI){		
		Intent serverIntent = new Intent(context, ServerService.class);
		Bundle serverBundle = new Bundle();
		serverBundle.putBoolean(ServerService.IS_START_BY_UI, isStartByUI);
		serverIntent.putExtras(serverBundle);
		context.startService(serverIntent);
	}
}
