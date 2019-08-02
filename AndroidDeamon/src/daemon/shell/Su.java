package daemon.shell;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import android.content.Context;


import daemon.common.Device;
import daemon.logger.LogCenter;
import daemon.util.KeyguardHelper;
import daemon.util.RootHelper;

public class Su {
	private Boolean mHasSuAuth = null;
	private String mSuPath = "";
	private static Su mInstance = null;

	public static Su getInstance() {
		if (mInstance == null)
			mInstance = new Su();

		return mInstance;
	}

	private Su() {

	}

	protected void finalize() {
		try {
			if (null != proc_) {
				proc_.destroy();
				proc_ = null;
			}
		} catch (Exception e) {
		}
	}

	public boolean hasSU() {
		if(RootHelper.isMiuiNoRoot()){
			return false;
		}
		String[] suArray = new String[] { "/sbin/su", "/vendor/bin/su",
				"/system/sbin/su", "/system/bin/su", "/system/xbin/su" };
		for (String suPath : suArray) {
			File suFile = new File(suPath);
			if (suFile.exists()) {
				if (checkSuRoot(suFile.getPath())) {
					mSuPath = suFile.getAbsolutePath();
					return true;
				}
			}
		}
		return false;
	}

	private boolean checkSuRoot(String suFile) {
		String cmd = "ls -l " + suFile;
		String result = Shell.getInstance().execForString(cmd);
		if (result != null && result.length() >= 4) {
			String suid = result.substring(3, 4);
			if (suid.toLowerCase().equals("s")){
				return true;
			}else{
				//4.3后su授权
				if(Device.getIntSdkVersion()>=18){
					String daemonsu = suFile.substring(0,suFile.length()-2)+"daemonsu";
                    File f= new File(daemonsu);
                    if(f.exists())
                    	return true;
				}
			}
				
		}

		return false;
	}

	public boolean hasSuAuthorize() {
		if(mHasSuAuth != null)
			return mHasSuAuth;
		else
			return false;
	}

	public synchronized  boolean getSuAuthorize(Context context) {
		if (!hasSU())
			return false;	
        KeyguardHelper.getInstance().lightScreen(context);
		if (mHasSuAuth == null) {
			try {
				if (runSu()) {
					for (int i = 0; i < 3; i++) {
						String rtn = exec_with_str_result("id", 30000, true);

						LogCenter.error("Su", " id:" + rtn);

						if (null == rtn || rtn.length() == 0) {
							LogCenter.error("Su", "null == rtn || rtn.length() == 0:");
						}else if (rtn.contains("root")
									|| (rtn.contains("uid=0") && rtn.contains("gid=0"))) {
							    LogCenter.error("Su", "result:    true");	
								mHasSuAuth = true;
								break;
						}
						LogCenter.error("Su", "result:"+mHasSuAuth);				
						if(mHasSuAuth == null){
							Thread.sleep(300);
							continue;
						}
					}

				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}

		return hasSuAuthorize();
	}

	private boolean runSu() {
		try {
			proc_ = Runtime.getRuntime().exec(mSuPath);

			stderr_ = proc_.getErrorStream();
			Thread.sleep(50);

			if (0 != stderr_.available()) {
				byte[] buf = new byte[stderr_.available()];
				int read = stderr_.read(buf);
				String errMsg = new String(buf, 0, read);
				LogCenter.error("Su", "runSu err:" + errMsg);
				//错误信息显示此提示时，其实是有弹窗，可以进行授权
				if(!errMsg.toLowerCase().equals("WARNING: generic atexit() called from legacy shared library\n".toLowerCase())){
					stderr_ = null;
					proc_.destroy();
					return false;
				}
			}

			stdin_ = proc_.getOutputStream();
			stdout_ = proc_.getInputStream();

			return true;
		} catch (Exception e) {
			e.printStackTrace();
		}

		return false;
	}

	public boolean exec(String cmd) {
		return exec(cmd, false);
	}

	private boolean exec(String cmd, boolean forceSu) {
		if ((mHasSuAuth != null && mHasSuAuth.booleanValue()) || forceSu) {
			try {
				stderr_.reset();
			} catch (IOException e) {
			}

			try {
				StringBuffer sb = new StringBuffer(cmd);
				if (!cmd.endsWith("\n")) {
					sb.append("\n");
				}

				stdin_.write(sb.toString().getBytes());

				return true;
			} catch (IOException e) {
				e.printStackTrace();
			}

		} else {
			return Shell.getInstance().exec(cmd);
		}

		return false;
	}

	public String exec_with_str_result(String cmd, int timeout) {
		return exec_with_str_result(cmd, timeout, false);
	}

	private String exec_with_str_result(String cmd, int timeout, boolean forceSu) {

		if ((mHasSuAuth != null && mHasSuAuth.booleanValue()) || forceSu) {
			if (false == exec(cmd, forceSu)) {
				return null;
			}

			// LogCenter.error("shell cmd", cmd);

			int try_count = 0;
			byte[] buf = new byte[4096];
			StringBuffer sb = new StringBuffer();
			InputStream input = stdout_;
			for (;;) {
				try {
					int ava = input.available();
					if (0 == ava) {
						if (0 != sb.length()) {
							break;
						} else {
							ava = stderr_.available();
							if (ava > 0) {
								int read = stderr_.read(buf);
								sb.append(new String(buf, 0, read));
								input = stderr_;
							}

							try_count += 1;

							Thread.sleep(200);

							if (try_count > 1000 || try_count * 200 > timeout) {
								break;
							}

							continue;
						}
					} else {
						int read = stdout_.read(buf);
						sb.append(new String(buf, 0, read));
					}
				} catch (Exception e) {
					e.printStackTrace();
				}
			}

			if (0 == sb.length()) {
				LogCenter.error("Su", "cmd:" + cmd + ", result:null");
				return null;
			} else {
				LogCenter.error("Su", "cmd:" + cmd + ", result:" + sb.toString());
				return sb.toString();
			}
		} else {			
			return Shell.getInstance().execForString(cmd);
		}
	}

	private OutputStream stdin_; // ��Ӧshell��׼����
	private InputStream stderr_; // ��Ӧshell��׼�������
	private InputStream stdout_; // ��Ӧshell��׼���
	private Process proc_;
	private boolean is_root_mode_ = false;
}