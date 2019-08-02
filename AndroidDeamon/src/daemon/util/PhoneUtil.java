package daemon.util;

import java.lang.reflect.Field;
import java.lang.reflect.Method;

import com.android.internal.telephony.ITelephony;

import daemon.logger.LogCenter;

import android.content.Context;
import android.media.AudioManager;
import android.telephony.TelephonyManager;

public class PhoneUtil {
	/**
	 * 运用反射技术获得ITelephony
	 * 
	 * @param telMgr
	 * @return
	 * @throws Exception
	 */
	public static ITelephony getITelephony(TelephonyManager telMgr) throws Exception {
		Method method = telMgr.getClass().getDeclaredMethod("getITelephony");
		method.setAccessible(true);

		return (ITelephony) method.invoke(telMgr);// 发回ITelephony
	}

	public static void printAllInform(Class clsShow) {
		try {
			// 取得所有方法
			Method[] hideMethod = clsShow.getDeclaredMethods();
			int i = 0;
			for (; i < hideMethod.length; i++) {
				LogCenter.debug("method name", hideMethod[i].getName());
			}
			// 取得所有常量
			Field[] allFields = clsShow.getFields();
			for (i = 0; i < allFields.length; i++) {
				LogCenter.debug("Field name", allFields[i].getName());
			}
		}
		catch (SecurityException e) {
			e.printStackTrace();
		}
		catch (IllegalArgumentException e) {
			e.printStackTrace();
		}
		catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	/**
	 * 打开扬声器，成功打开就将音量设为最大
	 * @param context
	 * @return 要是成功打开扬声器，就返回打开前的音量
	 */
	public static int openSpeaker(Context context) {
		int result = -1;
		AudioManager audioManager = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
		boolean isSpeakerphoneOn = false;
		try {
			if (audioManager != null) {
				isSpeakerphoneOn = audioManager.isSpeakerphoneOn();
				if (!isSpeakerphoneOn) {
					int currMode = audioManager.getMode();
					audioManager.setMode(AudioManager.MODE_IN_CALL);
					audioManager.setSpeakerphoneOn(true);
					audioManager.setMode(currMode);
					result = audioManager.getStreamVolume(AudioManager.STREAM_VOICE_CALL);
					audioManager.setStreamVolume(AudioManager.STREAM_VOICE_CALL, audioManager.getStreamMaxVolume(AudioManager.STREAM_VOICE_CALL), AudioManager.STREAM_VOICE_CALL);
				}
				isSpeakerphoneOn = audioManager.isSpeakerphoneOn();
				LogCenter.debug("", "isSpeakerphoneOn: " + isSpeakerphoneOn);
			}
		}
		catch (Exception e) {
			e.printStackTrace();
			result = -1;
		}


		
		return result;
	}

	/**
	 * 关闭扬声器
	 * @param context
	 * @return
	 */
	public static boolean closeSpeaker(Context context) {
		return closeSpeaker(context, -1);
	}
	
	/**
	 * 关闭扬声器，将音量设为volume
	 * @param context 
	 * @param volume 关闭扬声器后要设置的音量大小，如果值为-1，就不设置
	 */
	public static boolean closeSpeaker(Context context, int volume) {
		boolean result = false;
		try {
			AudioManager audioManager = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
			if (audioManager != null) {
				boolean isSpeakerphoneOn =  audioManager.isSpeakerphoneOn();
				if (isSpeakerphoneOn) {
					int currMode = audioManager.getMode();
					audioManager.setMode(AudioManager.MODE_IN_CALL);
					audioManager.setSpeakerphoneOn(false);
					audioManager.setMode(currMode);
					if (volume != -1) {
						audioManager.setStreamVolume(AudioManager.STREAM_VOICE_CALL, volume, AudioManager.STREAM_VOICE_CALL);
					}
				}
				
				result = true;
			}
		}
		catch (Exception e) {
			e.printStackTrace();
			result = false;
		}
		
		return result;
	}
}
