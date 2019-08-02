package daemon.logger;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

public class FileAppender implements Appender {
	
	private File file_;
	private FileWriter writer_;
	private Formatter formatter_;
		
	FileAppender(File file)
	{
		file_ = file;
	}
	
	public synchronized void doLog(String name, long time, Level level, Object message, Throwable t)
	{
		if (null == writer_ || null == formatter_)
		{
			return;
		}
		
		String str = formatter_.format(name, time, level, message, t);
		if (null == str || 0 == str.length())
		{
			return;
		}
		
		try
		{
			writer_.append(str);
			writer_.flush();
		}
		catch (IOException e)
		{
		}catch (Exception e) {

		}
	}

	public synchronized void clear()
	{
		try
		{
			writer_ = new FileWriter(file_);
		}
		catch (IOException e)
		{
		}catch (Exception e) {

		}
	}

	public synchronized boolean close()
	{
		if (writer_ != null)
		{
			try
			{
				writer_.close();
				writer_ = null;
				return true;
			}
			catch (IOException e)
			{
			}catch (Exception e) {

			}
		}
		
		return false;
	}

	public synchronized boolean open()
	{
		if (null != writer_)
		{
			return false;
		}
		
		try
		{
			writer_ = new FileWriter(file_, true);
			return true;
		}
		catch (IOException e)
		{
		}catch (Exception e) {

		}
		
		return false;
	}

	public boolean isLogOpen()
	{
		return null != writer_;
	}

	public void setFormatter(Formatter formatter)
	{
		formatter_ = formatter;
	}

	public void setProperty(String name, String value)
		throws IllegalArgumentException
	{
	}
}
