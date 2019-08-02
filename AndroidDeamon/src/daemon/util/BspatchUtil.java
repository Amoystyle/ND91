package daemon.util;

import android.content.Context;

public class BspatchUtil {
	static {
		System.loadLibrary("bspatch");
	}
	
	public static String getLibDir(Context ctx){	
		String cache_dir, lib_dir;
		try {
			cache_dir = ctx.getCacheDir().getCanonicalPath();				
			lib_dir = cache_dir.replace("cache", "lib");
		} catch (Exception e) {
			e.printStackTrace();
			lib_dir = "/data/data/"+ctx.getPackageName()+"/lib";
		}
		return lib_dir;
	}
	
	
	public static boolean patch(Context ctx,String oldFile,String newFile,String patchFile){
		return patch(getLibDir(ctx),oldFile, newFile, patchFile);
	}
	private native static boolean patch(String libPath,String oldFile,String newFile,String patchFile);
}