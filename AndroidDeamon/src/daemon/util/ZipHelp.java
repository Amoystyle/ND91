package daemon.util;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.zip.CRC32;
import java.util.zip.CheckedOutputStream;

import daemon.util.zip.ZipEntry;
import daemon.util.zip.ZipOutputStream;


public class ZipHelp {
	private static final int BUFFER = 8192;
	
	/** 压缩一个文件或文件夹 */
	public static void doZip(String srcPathName, String zipFilePath) {
		File file = new File(srcPathName);
		File zipFile = new File(zipFilePath);
		if (!file.exists())
			throw new RuntimeException(srcPathName + "�����ڣ�");
		ZipOutputStream out = null;
		try {
			FileOutputStream fileOutputStream = new FileOutputStream(zipFile);
			CheckedOutputStream cos = new CheckedOutputStream(fileOutputStream,
					new CRC32());
			out = new ZipOutputStream(cos);
			out.setEncoding("GBK");
			String basedir = "";
			if(file.isDirectory()){
				compress(file, out, basedir);
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
	
	/** 压缩一个文件夹下所有的子文件�*/
	public static void doZipDirectory(String srcPathName, String zipFilePath) {
		File file = new File(srcPathName);
		File zipFile = new File(zipFilePath);
		if (!file.exists())
			throw new RuntimeException(srcPathName + "�����ڣ�");
		ZipOutputStream out = null;
		try {
			FileOutputStream fileOutputStream = new FileOutputStream(zipFile);
			CheckedOutputStream cos = new CheckedOutputStream(fileOutputStream,
					new CRC32());
			out = new ZipOutputStream(cos);
			out.setEncoding("GBK");
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
		/* 判断是目录还是文仄1�7 */
		if (file.isDirectory()) {
			System.out.println("压缩＄1�7" + basedir + file.getName());
			compressDirectory(file, out, basedir);
		} else {
			System.out.println("压缩＄1�7" + basedir + file.getName());
			compressFile(file, out, basedir);
		}
	}

	/** 压缩丄1�7个目彄1�7 */
	private static void compressDirectory(File dir, ZipOutputStream out, String basedir) {
		if (!dir.exists())
			return;

		File[] files = dir.listFiles();
		for (int i = 0; i < files.length; i++) {
			/* 递归 */
			compress(files[i], out, basedir + dir.getName() + "/");
		}
	}

	/** 压缩丄1�7个文仄1�7 */
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
}
