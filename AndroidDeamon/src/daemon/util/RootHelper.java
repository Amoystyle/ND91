package daemon.util;

import daemon.logger.LogCenter;
import daemon.shell.Shell;

public class RootHelper {

	
	public static boolean isMiuiNoRoot(){
		//如果是miui稳定版，则直接判断为非ROOT
		String cmdResult = Shell.getInstance().execForString("getprop ro.miui.ui.version.name");
		if(cmdResult!=null&&!"".equals(cmdResult)){
			cmdResult = Shell.getInstance().execForString("getprop ro.build.version.incremental");
			if(cmdResult!=null&&!"".equals(cmdResult)){
				if(!cmdResult.matches("[.|0-9]+")){
					LogCenter.debug("root result ", ""+false);
					return true;
				}
			}
		}
		return false;
	}
}
