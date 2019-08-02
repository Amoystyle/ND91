package daemon.util;

import java.io.BufferedInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.zip.CRC32;
import java.util.zip.CheckedOutputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;
import java.util.zip.ZipOutputStream;

import android.util.Log;

public class ZipUtil {
	static {
		System.loadLibrary("zip-jni");
	}
	
	private static final int BUFFER = 8192;
	private static final String TAG = "ZipUtil";
	private static final String SEPARATOR = ":";

	public native static void mergerZip(String baseFile, String updateFile, String deleteFileList);
	public native static String readZipComment(String fileName);
	
	public static void mergerZip(String baseFile, String updateFile, List<String> deleteFile){
		if(deleteFile == null || deleteFile.isEmpty()){
			mergerZip(baseFile, updateFile, "");
		}else{
			mergerZip(baseFile, updateFile, join(deleteFile,SEPARATOR));
		}
	}

	public static void mergerZip(String baseFile, String updateFile, String outFile, List<String> deleteFile) throws Exception{
		byte[] buf = new byte[BUFFER];
		
		List<String> list = new ArrayList<String>();
		
		ZipInputStream zin = new ZipInputStream(new FileInputStream(baseFile));
		ZipInputStream uin = new ZipInputStream(new FileInputStream(updateFile));
	    CheckedOutputStream cos = new CheckedOutputStream(new FileOutputStream(outFile), new CRC32());
	    
		ZipOutputStream out = new ZipOutputStream(cos);
		
	    ZipEntry zEntry = null;
	    ZipEntry uEntry = null;

	    while((uEntry = uin.getNextEntry()) != null){
	    	if(uEntry.isDirectory()){
	    		continue;
	    	}
	    	Log.d(TAG,uEntry.getName());
	    	out.putNextEntry(uEntry);
	    	list.add(uEntry.getName());
	    	int len;
            while ((len = uin.read(buf)) > 0) {
                out.write(buf, 0, len);
            }
	    }
	    
	    if(deleteFile == null || deleteFile.isEmpty()){
	    	while((zEntry = zin.getNextEntry()) != null){
		    	if(zEntry.isDirectory()){
		    		continue;
		    	}

		    	if(!list.contains(zEntry.getName())){
		    		Log.d(TAG,zEntry.getName());
		    		out.putNextEntry(zEntry);
		    		int len;
		    		while ((len = zin.read(buf)) > 0) {
		                out.write(buf, 0, len);
		            }
		    	}
		    }
	    }else{
	    	while((zEntry = zin.getNextEntry()) != null){
		    	if(zEntry.isDirectory()){
		    		continue;
		    	}
	    		if(deleteFile.contains(zEntry.getName())){
	    			continue;
	    		}
		    	if(!list.contains(zEntry.getName())){
		    		Log.d(TAG,zEntry.getName());
		    		out.putNextEntry(zEntry);
		    		int len;
		    		while ((len = zin.read(buf)) > 0) {
		                out.write(buf, 0, len);
		            }
		    	}
		    }
	    }
	    zin.close();
	    uin.close();
	    out.closeEntry();
	    out.close();
	}
	
	public static String readZipTextFile(String zipFile, String fileName){
		return readZipTextFile(zipFile,fileName,false);
	}
	public static String readZipTextFile(String zipFile, String fileName, boolean like){
		ZipInputStream inZip = null;
		ByteArrayOutputStream os = null;
		try{
			inZip = new ZipInputStream(new FileInputStream(zipFile));
			ZipEntry zipEntry;
			
			while ((zipEntry = inZip.getNextEntry()) != null) {
				if(zipEntry.getName().equals(fileName) || (like && zipEntry.getName().contains(fileName))){
					os = new ByteArrayOutputStream();
					int len;
					byte[] buffer = new byte[1024];
					while ((len = inZip.read(buffer)) != -1) {
						os.write(buffer, 0, len);
					}
					os.flush();
					return new String(os.toByteArray());
				}
			}
			return null;
		}catch(Exception e){
			e.printStackTrace();
			return null;
		}finally{
			try {
				if(inZip != null)
					inZip.close();
				if(os != null)
					os.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		
	}

	public static void unZip(String zipFileString, String outPathString)throws Exception {
		ZipInputStream inZip = new ZipInputStream(new FileInputStream(zipFileString));
		ZipEntry zipEntry;
		String szName = "";
		
		while ((zipEntry = inZip.getNextEntry()) != null) {
			szName = zipEntry.getName();
		
			if (zipEntry.isDirectory()) {
				// get the folder name of the widget
				szName = szName.substring(0, szName.length() - 1);
				File folder = new File(outPathString + File.separator + szName);
				folder.mkdirs();
		
			} else {
		
				File file = new File(outPathString + File.separator + szName);
				File parent = file.getParentFile();
				if(!parent.exists()){
					parent.mkdirs();
				}
				file.createNewFile();
				// get the output stream of the file
				FileOutputStream out = new FileOutputStream(file);
				int len;
				byte[] buffer = new byte[1024];
				// read (len) bytes into buffer
				while ((len = inZip.read(buffer)) != -1) {
					// write (len) byte from buffer at the position 0
					out.write(buffer, 0, len);
					out.flush();
				}
				out.close();
			}
		}//end of while
		inZip.close();
	}//end of func

	public static void doZip(String srcPathName, String zipFilePath) {
		File file = new File(srcPathName);
		File zipFile = new File(zipFilePath);
		if (!file.exists())
			throw new RuntimeException(srcPathName + "不存在！");
		ZipOutputStream out = null;
		try {
			FileOutputStream fileOutputStream = new FileOutputStream(zipFile);
			CheckedOutputStream cos = new CheckedOutputStream(fileOutputStream,
					new CRC32());
			out = new ZipOutputStream(cos);
			String basedir = "";
			if(file.isDirectory()){
				File[] fLists = file.listFiles();
				for(File f : fLists){
					compress(f, out, basedir);
				}
			}else
				compress(file, out, basedir);
		} catch (Exception e) {
			throw new RuntimeException(e);
		} finally {
			try {
				if(out != null)
					out.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}

	private static void compress(File file, ZipOutputStream out, String basedir) {
		/* 判断是目录还是文件 */
		if (file.isDirectory()) {
			System.out.println("压缩：" + basedir + file.getName());
			compressDirectory(file, out, basedir);
		} else {
			System.out.println("压缩：" + basedir + file.getName());
			compressFile(file, out, basedir);
		}
	}

	/** 压缩一个目录 */
	private static void compressDirectory(File dir, ZipOutputStream out, String basedir) {
		if (!dir.exists())
			return;

		File[] files = dir.listFiles();
		for (int i = 0; i < files.length; i++) {
			/* 递归 */
			compress(files[i], out, basedir + dir.getName() + "/");
		}
	}

	/** 压缩一个文件 */
	private static void compressFile(File file, ZipOutputStream out, String basedir) {
		if (!file.exists()) {
			return;
		}
		BufferedInputStream bis = null;
		try {
			bis = new BufferedInputStream(
					new FileInputStream(file));
			ZipEntry entry = new ZipEntry(basedir + file.getName());
			out.putNextEntry(entry);
			int count;
			byte data[] = new byte[BUFFER];
			while ((count = bis.read(data, 0, BUFFER)) != -1) {
				out.write(data, 0, count);
			}
		} catch (Exception e) {
			throw new RuntimeException(e);
		} finally {
			try {
				if(bis != null)
					bis.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	
	private static String join(List<String> strs, String separator){ 
        StringBuffer sb = new StringBuffer(); 
        for(int i = 0; i < strs.size(); i++) { 
                sb.append(strs.get(i)).append(separator); 
        } 
        return sb.substring(0, sb.length() - separator.length()); 
	}
}