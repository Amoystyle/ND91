package daemon.logger;

import java.io.IOException;

import android.util.Log;

public class AndroidLogAppender implements Appender {

	public void doLog(String name, long time, Level level, Object message, Throwable t)
	{
		if (closed_)
		{
			return;
		}
		
		if (level == Level.DEBUG)
		{
			if (null == t)
			{
				Log.d(name, (String)message);
			}
			else
			{
				Log.d(name, (String)message, t);
			}
		}
		else if (level == Level.ERROR)
		{
			if (null == t)
			{
				Log.e(name, (String)message);
			}
			else
			{
				Log.e(name, (String)message, t);
			}
		}
		else
		{
			if (null == t)
			{
				Log.i(name, (String)message);
			}
			else
			{
				Log.e(name, (String)message, t);
			}
		}
	}

	public void clear()
	{
	}

	/**
	 * Close the log. The consequence is that the logging is disabled until the
	 * log is opened. The logging could be enabled by calling
	 * <code>open()</code>.
	 * 
	 * @throws IOException
	 *             if the close failed.
	 */
	public boolean close()
	{
		closed_ = true;
		return true;
	}

	/**
	 * Open the log. The consequence is that the logging is enabled.
	 * 
	 * @throws IOException
	 *             if the open failed.
	 * 
	 */
	public boolean open()
	{
		closed_ = false;
		return true;
	}

	/**
	 * Check if the log is open.
	 * 
	 * @return true if the log is open, false otherwise.
	 */
	public boolean isLogOpen()
	{
		return true;
	}

	/**
	 * Set the formatter to use.
	 * 
	 * @param formatter
	 *            The formatter to set.
	 */
	public void setFormatter(Formatter formatter)
	{
	}
	
	/**
	 * Set the specified property to the supplied value.
	 * 
	 * @param name
	 *            the name of the property to set.
	 * @param value
	 *            the value to set.
	 * @throws IllegalArgumentException
	 *             if the name or value has an invalid value, such as
	 *             <code>null</code>.
	 */
	public void setProperty(String name, String value)
			throws IllegalArgumentException
	{
	}
	
	private boolean closed_ = true;
}
