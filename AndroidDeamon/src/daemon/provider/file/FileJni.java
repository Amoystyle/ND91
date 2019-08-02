package daemon.provider.file;

import daemon.logger.LogCenter;

public class FileJni {
	private static final String LIB_ND_NAME = "file_jni";
	private static FileJni mInstance = null;
	private static String className = FileJni.class.getSimpleName();
	
	static {
		LogCenter.debug(className, "load " + LIB_ND_NAME + " shared library");
		System.loadLibrary(LIB_ND_NAME);
		initLog();
	}
	
	public static native void initLog();
	
	public static native byte[] getDirInfo(String path);
	
	//新增接口，获取目录信息的时候，获取目录子目录是否可写属性
	public static native byte[] getDirInfoSubWritable(String path);
	
	public static native byte[] makeDir(String path);
	
	public static native byte[] remove(String path);
	
	public static native byte[] move(String oldPath, String newPath);
	
	public static native byte[] copy(String oldPath, String newPath);
	
	public static native byte[] getFileAttr(String path);
	
	//新增接口，获取文件属性中增加文件是否可写属性
	public static native byte[] getFileAttrWritable(String path);
	
	public static native byte[] chmod(String path, int mode);
}
