package com.nd.assistance.util;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import daemon.shell.Shell;


import android.content.Context;




public class SuEx {
	private Boolean mHasSuAuth = null;
	private String mSuPath = "";
	private static SuEx mInstance = null;

	public static SuEx getInstance() {
		if (mInstance == null)
			mInstance = new SuEx();

		return mInstance;
	}

	private SuEx() {

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
			if (suid.toLowerCase().equals("s"))
				return true;
		}

		return false;
	}

	public boolean hasSuAuthorize() {
		if(mHasSuAuth != null)
			return mHasSuAuth;
		else
			return false;
	}

	public boolean getSuAuthorize(Context context) {
		if (!hasSU())
			return false;	

		if (mHasSuAuth == null) {
			try {
				if (runSu()) {
					String rtn = exec_with_str_result("id", 30000, true);



					if (null == rtn || rtn.length() == 0) {
						return false;
					}

					if (rtn.contains("root")
							|| (rtn.contains("uid=0") && rtn.contains("gid=0"))) {
						mHasSuAuth = true;
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

				
				stderr_ = null;
				proc_.destroy();
				return false;
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

	
	public interface SuResultLister{
		public void OnSuResult(String result);
	}
	SuResultLister mShellResultLister;
	
	public boolean execing=false;
	public boolean exec_with_str_result_ex(String cmd, int timeout,SuResultLister o) {
		mShellResultLister = o;
		execing=true;
		if ((mHasSuAuth != null && mHasSuAuth.booleanValue())) {
			if (false == exec(cmd, false)) {
				return false;
			}
			while(execing){
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

								Thread.sleep(2000);

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

					return false;
				} else {
					mShellResultLister.OnSuResult(sb.toString());
					try {
						Thread.sleep(200);
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
			}
			return true;

		} else {			
			return false;
		}
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
				return null;
			} else {
				return sb.toString();
			}
		} else {			
			return Shell.getInstance().execForString(cmd);
		}
	}

	private OutputStream stdin_; // �1�7�1�7�0�8shell�1�7�1�7�0�6�1�7�1�7�1�7�1�7
	private InputStream stderr_; // �1�7�1�7�0�8shell�1�7�1�7�0�6�1�7�1�7�1�7�1�7�1�7�1�7�1�7
	private InputStream stdout_; // �1�7�1�7�0�8shell�1�7�1�7�0�6�1�7�1�7�1�7
	private Process proc_;
	private boolean is_root_mode_ = false;
}