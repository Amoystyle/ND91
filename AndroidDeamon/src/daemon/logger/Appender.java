package daemon.logger;

import java.io.IOException;

public interface Appender {
	/**
	 * Size returned if log size cannot be determined.
	 */
	int SIZE_UNDEFINED = -1;

	/**
	 * Do the logging.
	 * 
	 * @param clientID
	 *            the id of the client.
	 * @param name
	 *            the name of the logger.
	 * @param time
	 *            the time since the first logging has done (in milliseconds).
	 * @param level
	 *            the logging level
	 * @param message
	 *            the message to log.
	 * @param t
	 *            the exception to log.
	 */
	void doLog(String name, long time, Level level, Object message, Throwable t);

	/**
	 * Clear the log.
	 */
	void clear();

	/**
	 * Close the log. The consequence is that the logging is disabled until the
	 * log is opened. The logging could be enabled by calling
	 * <code>open()</code>.
	 * 
	 * @throws IOException
	 *             if the close failed.
	 */
	boolean close();

	/**
	 * Open the log. The consequence is that the logging is enabled.
	 * 
	 * @throws IOException
	 *             if the open failed.
	 * 
	 */
	boolean open();

	/**
	 * Check if the log is open.
	 * 
	 * @return true if the log is open, false otherwise.
	 */
	boolean isLogOpen();

	/**
	 * Set the formatter to use.
	 * 
	 * @param formatter
	 *            The formatter to set.
	 */
	void setFormatter(Formatter formatter);
	
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
	void setProperty(String name, String value)
			throws IllegalArgumentException;
}
