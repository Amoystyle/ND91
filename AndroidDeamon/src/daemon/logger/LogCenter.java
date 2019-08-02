package daemon.logger;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

import android.os.Environment;
import android.util.Log;

public class LogCenter {
	private static final String LOG_PARENT = Environment.getExternalStorageDirectory().getPath() + "/nd";
	private static final String LOG_FILE = LOG_PARENT + "/log.txt";
	private static final String LOG_DIR = LOG_PARENT+ "/log/";
	
	private static boolean sLogOut = false;
	
	public static void init() {
		File log_file = new File(LOG_FILE);
		File log_dir = new File(LOG_DIR);
		File log_parent = new File(LOG_PARENT);
		if(!log_parent.exists()){
			log_parent.mkdirs();
		}
		
		if (log_dir.exists() && log_dir.isDirectory()) {
			sLogOut =true;
		}
		boolean needCreateNew = false;
		if (log_file.exists() && log_file.isFile()) {
			// 检查log文件大小，如果大于1Mb，就重新创建
			if (log_file.length() > 1024 * 1024) {
				if (log_file.delete()) {
					needCreateNew = true;
				}
			}
		} else if (!log_file.exists()) {
			needCreateNew = true;
		}
		if (needCreateNew) {
			boolean createSuccess = false;
			try {
				createSuccess = log_file.createNewFile();
			} catch (Exception e) {
				if(!log_file.exists()){
					FileOutputStream fos = null;
					try {
						fos = new FileOutputStream(log_file, false);
						fos.write("".getBytes());
					} catch (Exception ex) {
						ex.printStackTrace();
					} finally {
						try {
							if(fos!=null){
								fos.close();
							}
						} catch (Exception ex) {
							ex.printStackTrace();
						}
					}
				}
			}

			if (!createSuccess)
				return;
		}
		if (log_file.exists() && log_file.isFile()) {
			logger_impl_ = new LoggerImpl();

			Log.e("LogCenter", "logger_impl_ init");

			FileAppender fa = new FileAppender(log_file);
			fa.setFormatter(new SimpleFormatter());
			if (fa.open()) {
				logger_impl_.addAppender(fa);
			} else {
				fa = null;
			}

			AndroidLogAppender ala = new AndroidLogAppender();
			ala.open();
			logger_impl_.addAppender(ala);
		}
		
	}

	public static void open() {
		if (logger_impl_ == null)
			return;

		try {
			logger_impl_.open();
		}
		catch (IOException e) {
		}
	}

	public static void close() {
		if (logger_impl_ == null)
			return;

		try {
			logger_impl_.close();
		}
		catch (IOException e) {
		}
	}

	public static synchronized boolean open_socket_backend() {
		if (logger_impl_ == null)
			return false;

		if (null != sa_) {
			return true;
		}

		sa_ = new SocketAppender();

		if (!sa_.open()) {
			sa_ = null;
			return false;
		}

		sa_.setFormatter(new SimpleFormatter());
		logger_impl_.addAppender(sa_);

		return true;
	}

	public static synchronized boolean close_socket_backend() {
		if (null != sa_) {
			sa_.close();
			sa_ = null;
			return true;
		}

		return false;
	}

	public static void debug(String tag, String msg) {
		debug(tag, msg, null);
	}

	public static void debug(String tag, String msg, Throwable t) {
       debug(tag, msg, t, false);
	}
	
	public static void debug(String tag, String msg, Throwable t,boolean special) {
		if (logger_impl_ == null)
			return;
		if(!special&&!sLogOut){
			return;
		}

		logger_impl_.setName(tag);
		logger_impl_.debug(msg, t);
	}

	public static void info(String tag, String msg) {
		if (logger_impl_ == null)
			return;

		info(tag, msg, null);
	}

	public static void info(String tag, String msg, Throwable t) {
		info(tag, msg, t, false);
	}
	
	public static void info(String tag, String msg, Throwable t,boolean special) {
		if (logger_impl_ == null)
			return;
		if(!special&&!sLogOut){
			return;
		}
		logger_impl_.setName(tag);
		logger_impl_.info(msg, t);
	}

	public static void warn(String tag, String msg) {
		error(tag, msg, null);
	}

	public static void warn(String tag, String msg, Throwable t) {
		warn(tag, msg, t, false);
	}
	
	public static void warn(String tag, String msg, Throwable t,boolean special) {
		if (logger_impl_ == null)
			return;
		if(!special&&!sLogOut){
			return;
		}
		logger_impl_.setName(tag);
		logger_impl_.warn(msg, t);
	}

	public static void error(String tag, String msg) {
		error(tag, msg, null);
	}

	public static void error(String tag, String msg, Throwable t) {
		error(tag, msg,t,false);
	}
	
	public static void error(String tag, String msg, Throwable t,boolean special) {
		if (logger_impl_ == null)
			return;
		if(!special&&!sLogOut){
			return;
		}
		logger_impl_.setName(tag);
		logger_impl_.error(msg, t);
	}


	static {
		init();
	}

	private static SocketAppender sa_;
	private static LoggerImpl logger_impl_;
}
