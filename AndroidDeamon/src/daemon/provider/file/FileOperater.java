package daemon.provider.file;

import java.io.File;

import daemon.ndk.NdkManager;
import daemon.util.FileHelper;
import android.content.Context;

public class FileOperater {
	public static byte[] getDirInfo(Context context, String strPath) {
		byte[] byteResult = null;
		if (NdkManager.isNdshActualRunning(context)) {
			byteResult = NdkManager.getDirInfo(context, strPath);
		}
		if (byteResult == null) {
			byteResult = FileJni.getDirInfo(strPath);
		}
	
		return byteResult;
	}
	
	public static byte[] getDirInfoSubWritable(Context context, String strPath) {
		byte[] byteResult = null;
		if (NdkManager.isNdshActualRunning(context)) {
			byteResult = NdkManager.getDirInfoSubWritable(context, strPath);
		}
		if (byteResult == null) {
			byteResult = FileJni.getDirInfoSubWritable(strPath);
		}
	
		return byteResult;
	}

	public static byte[] makeDir(Context context, String strPath) {
		byte[] byteResult = null;
		try {
			if (NdkManager.isNdshActualRunning(context)) {
				File file= new File(strPath);
				if(!FileHelper.isFileCanWriteByNdsh(context, file.getParent())){
					FileHelper.mount(context,new File(strPath));
				}			
				byteResult = NdkManager.makeDir(context, strPath);
			}
			if (byteResult == null) {
				byteResult = FileJni.makeDir(strPath);
			}
		} catch (Exception e) {

		}

		
		return byteResult;
	}

	public static byte[] remove(Context context, String strPath) {
		byte[] byteResult = null;
		if (NdkManager.isNdshActualRunning(context)) {
			byteResult = NdkManager.remove(context, strPath);
		}
		if (byteResult == null) {
			byteResult = FileJni.remove(strPath);
		}
		
		return byteResult;
	}

	public static byte[] move(Context context, String strPathOld, String strPathNew) {
		byte[] byteResult = null;
		if (NdkManager.isNdshActualRunning(context)) {
			byteResult = NdkManager.move(context, strPathOld, strPathNew);
		}
		if (byteResult == null) {
			byteResult = FileJni.move(strPathOld, strPathNew);
		}
		
		return byteResult;
	}

	public static byte[] copy(Context context, String strPathOld, String strPathNew) {
		byte[] byteResult = null;
		if (NdkManager.isNdshActualRunning(context)) {
			byteResult = NdkManager.copy(context, strPathOld, strPathNew);
		}
		if (byteResult == null) {
			byteResult = FileJni.copy(strPathOld, strPathNew);
		}
		
		return byteResult;
	}

	public static byte[] getFileAttr(Context context, String strPath) {
		byte[] byteResult = null;
		if (NdkManager.isNdshActualRunning(context)) {
			byteResult = NdkManager.getFileAttr(context, strPath);
		}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
		if (byteResult == null) {
			byteResult = FileJni.getFileAttr(strPath);
		}
		
		return byteResult;
	}
	
	public static byte[] getFileAttrWritable(Context context, String strPath) {
		byte[] byteResult = null;
		if (NdkManager.isNdshActualRunning(context)) {
			byteResult = NdkManager.getFileAttrWritable(context, strPath);
		}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
		if (byteResult == null) {
			byteResult = FileJni.getFileAttrWritable(strPath);
		}
		
		return byteResult;
	}

	public static byte[] chmod(Context context, String strPath, int mode) {
		byte[] byteResult = null;
		if (NdkManager.isNdshActualRunning(context)) {
			byteResult = NdkManager.chmod(context, strPath, mode);
		}
		if (byteResult == null) {
			byteResult = FileJni.chmod(strPath, mode);
		}
		
		return byteResult;
	}
}
