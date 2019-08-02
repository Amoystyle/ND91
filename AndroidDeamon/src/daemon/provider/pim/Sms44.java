package daemon.provider.pim;

import daemon.logger.LogCenter;
import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.provider.Telephony;
import android.provider.Telephony.Sms;

public class Sms44 {
	
	private static String sDefualtPackName="";

	public static void sendChangeDefault(Context context, String packname){
		try {
			Intent intent =new Intent(Sms.Intents.ACTION_CHANGE_DEFAULT);
			intent.putExtra(Sms.Intents.EXTRA_PACKAGE_NAME, packname);  
			intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
			context.startActivity(intent); 

		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public static void restoreDefault(Context context){
		if(sDefualtPackName==null||sDefualtPackName.equals("")){
			return;
		}
		try {
			Intent intent =new Intent(Sms.Intents.ACTION_CHANGE_DEFAULT);
			intent.putExtra(Sms.Intents.EXTRA_PACKAGE_NAME, sDefualtPackName);  
			intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
			context.startActivity(intent);
			sDefualtPackName="";
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	
	
	
	@SuppressLint("NewApi")
	public static String getDefualPakName(Context context){
		String packname ="";
		try {
			packname = Telephony.Sms.getDefaultSmsPackage(context);
			if(!context.getPackageName().equals(packname)){
				sDefualtPackName=packname;
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return packname;  
	}
	
	
}
