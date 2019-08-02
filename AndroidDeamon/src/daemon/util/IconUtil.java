
package daemon.util;

import java.io.File;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.Method;

import daemon.common.Device;
import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.res.AssetManager;
import android.content.res.Resources;
import android.graphics.drawable.Drawable;
import android.util.DisplayMetrics;



public class IconUtil {

	  static final String TAG="IconUtil";
	  
	  public static  Drawable getAppIcon2_2(Context context,String apkPath) {
		    Drawable drawable = null;
	        String PATH_PackageParser = "android.content.pm.PackageParser";
	        try {
	            // apk包的文件路径
	            // 这是一个Package 解释器, 是隐藏的
	            // 构造函数的参数只有一个, apk文件的路径
	            // PackageParser packageParser = new PackageParser(apkPath);
	            Class pkgParserCls = Class.forName(PATH_PackageParser);
	            Class[] typeArgs = new Class[1];
	            typeArgs[0] = String.class;
	            Constructor pkgParserCt = pkgParserCls.getConstructor(typeArgs);
	            Object[] valueArgs = new Object[1];
	            valueArgs[0] = apkPath;
	            Object pkgParser = pkgParserCt.newInstance(valueArgs);
	            DisplayMetrics metrics = new DisplayMetrics();
	            metrics.setToDefaults();
	            typeArgs = new Class[4];
	            typeArgs[0] = File.class;
	            typeArgs[1] = String.class;
	            typeArgs[2] = DisplayMetrics.class;
	            typeArgs[3] = Integer.TYPE;
	            Method pkgParser_parsePackageMtd = pkgParserCls.getDeclaredMethod("parsePackage",
	                    typeArgs);
	            valueArgs = new Object[4];
	            valueArgs[0] = new File(apkPath);
	            valueArgs[1] = apkPath;
	            valueArgs[2] = metrics;
	            valueArgs[3] = 0;
	            Object retPackage = pkgParser_parsePackageMtd.invoke(pkgParser, valueArgs);
	        	if(retPackage != null){           
	        		Class pkgCls = Class.forName("android.content.pm.PackageParser$Package");
	        		Field f = pkgCls.getDeclaredField("applicationInfo");  
	        		f.setAccessible(true);  
					ApplicationInfo info = (ApplicationInfo)f.get(retPackage);  
					Resources pRes = context.getResources();   
		            Class<?> localClass = Class.forName("android.content.res.AssetManager");
	                Constructor localConstructor = localClass.getConstructor();
	                Object assmgr = localConstructor.newInstance();
	                Class[] array = new Class[1];
	                array[0] = String.class;
	                Method localMethod = localClass.getDeclaredMethod("addAssetPath", array);
	                localMethod.invoke(assmgr, apkPath);
	                
	                Resources res = new Resources((AssetManager)assmgr, pRes.getDisplayMetrics(), pRes.getConfiguration());
	                if(info.icon != 0)
	                	drawable = res.getDrawable(info.icon);
					return drawable;
				}

	        } catch (Exception e) {
	            e.printStackTrace();
	        }
	        return null;
	    } 

	  public static Drawable getAppIcon(final Context context,final String filePath){
		  Drawable d = null;
			try{
		        PackageManager pm = context.getPackageManager();     
		        PackageInfo info = pm.getPackageArchiveInfo(filePath, PackageManager.GET_ACTIVITIES); 
				if (Device.getIntSdkVersion() < 8)
					d = context.getPackageManager().getApplicationIcon(info.applicationInfo);
				else
					d = IconUtil.getAppIcon2_2(context, filePath);
				return d;
			}
			catch(Exception e){
				e.printStackTrace();
			}
			return d;
	  }
}
