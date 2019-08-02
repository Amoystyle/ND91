package daemon.util;

import java.io.File;
import java.nio.ByteBuffer;

import android.content.Context;
import android.os.Environment;
import android.text.TextUtils;
import daemon.common.MountPartitionParam;
import daemon.ndk.NdkManager;
import daemon.protocol.ByteReader;
import daemon.protocol.MemoryStream;

public class FileHelper {
	 
	
	public static boolean isFileCanWriteByNdsh(Context context,String path){
		if(!NdkManager.isNdshActualRunning(context)){
			return false;
		}
		FileInfo info  = getFileInfo(context, path);
		return info.isCanWrite();
	}
	
	public static FileInfo getFileInfo(Context context,String path){
		if(!NdkManager.isNdshActualRunning(context)){
			return null;
		}
		FileInfo fileInfo =new FileInfo();
		byte[] info =NdkManager.getFileAttrWritable(context, path);
		if(info!=null){
			ByteReader reader = new ByteReader((new MemoryStream(info)));
			int ret = reader.ReadInt32();
			if(ret==1){
				fileInfo.setExist(true);
				fileInfo.setPath(reader.ReadString());
				fileInfo.setSize(reader.ReadInt64());
				fileInfo.setTime(reader.ReadInt64());
				fileInfo.setMode(reader.ReadInt32());
				fileInfo.setHasChild(reader.ReadBoolean());
				fileInfo.setCanWrite(reader.ReadBoolean());
			}
		}
		return fileInfo;
	}
	
	public static boolean isFileExitByNdsh(Context context,String path){
		if(!NdkManager.isNdshActualRunning(context)){
			return false;
		}
		FileInfo info  = getFileInfo(context, path);
		return info.isExist();
	}
	
	public static boolean removeFileByNdsh(Context context,String path){
		if(!NdkManager.isNdshActualRunning(context)){
			return false;
		}
		byte[] info =NdkManager.remove(context, path);
		boolean result = false;
		if(info!=null){
			ByteReader reader = new ByteReader((new MemoryStream(info)));
			int ret = reader.ReadInt32();
			if(ret==1){
				result = true;
			}
		}
		return result;
	}
	
	public static boolean isFileCanWriteByJava(String path){
		if(!path.endsWith("/")){
			path+="/";
		}
		String testpath = path + System.currentTimeMillis();;
		File sysFile = new File(testpath);
		if(sysFile.exists()){
			return sysFile.delete();
		}else{
			boolean result = sysFile.mkdirs();
			if(result)
			   sysFile.delete();
			return result;
		}
	}


	
	public static byte[] readFromFileByNdsh(Context context,String path,int num,int readsize){
		if(!NdkManager.isNdshActualRunning(context)){
			return null;
		}
		byte[] info =NdkManager.readFileData(context, path, num,readsize);
		if(info!=null){
			ByteReader reader = new ByteReader((new MemoryStream(info)));
			int ret = reader.ReadInt32();
			if(ret==1){
				int readcount= reader.ReadInt32();
				if(readcount==readsize){
					return reader.ReadBytes(readsize);
				}
			}
		}
		return null;
	}

	public static void mount(Context c,File file) {
	    String   dir = file.getPath().substring(1);
		int index = dir.indexOf("/");
		if(index ==-1){
		    dir= "/";
		}else{
		    dir = "/" + dir.substring(0, index+1);
	    }	
		MountPartitionParam param = MountPartitionParam.createPartitionParam(dir);
		if (param != null) {
			String partition = param.getSource();
			String type = param.getFilesystemType();
			String format = "mount -o remount -w -t %1$s %2$s "+dir;
			if (!TextUtils.isEmpty(partition)
					&& !TextUtils.isEmpty(type)) {
				String cmd = String.format(format, type, partition);
				NdkManager.execShell(c, cmd);
				//NdkManager.execShell(mContext, "chmod 0777 "+file.getParent());
			}
			
		}
		
	}

	public static boolean isSdcardFile(String mFilePath) {
		return(mFilePath.startsWith(Environment.getExternalStorageDirectory().toString())
				||mFilePath.startsWith("/sdcard"));
	}
	
	
}
