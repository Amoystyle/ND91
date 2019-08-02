package daemon.logger;

import java.util.Vector;
import java.io.IOException;

public class LoggerImpl {
	public static final Level DEFAULT_LOG_LEVEL = Level.DEBUG;
	private String name;
	private Level level;
	private final static Vector<Appender> appenderList = new Vector<Appender>(2);
	
	LoggerImpl() 
	{
		level = DEFAULT_LOG_LEVEL;
	}
	
	/**
	 * Get the log level.
	 * 
	 * @return the log <code>Level</code>.
	 */
	public Level getLevel() {
		return level;
	}

	/**
	 * Set the log level.
	 * 
	 * @param level
	 *            The logLevel to set.
	 * @throws IllegalArgumentException
	 *             if the <code>level</code> is null.
	 */
	public void setLevel(Level level) throws IllegalArgumentException {
		if (level == null) {
			throw new IllegalArgumentException("The level must not be null.");
		}
		this.level = level;
	}
	
	public void setName(String name)
	{
		this.name = name;
	}
	
	/**
	 * Get the name of the <code>Logger</code>.
	 * 
	 * @return the name of the <code>Logger</code>.
	 */
	public String getName() {
		return name;
	}

	/**
	 * Add the specified appender to the output appenders.
	 * 
	 * @param appender
	 *            the <code>Appender</code> to add.
	 * @throws IllegalArgumentException
	 *             if the <code>appender</code> is <code>null</code>.
	 */
	public void addAppender(Appender appender) throws IllegalArgumentException {
		if (appender == null) {
			throw new IllegalArgumentException(
					"Appender not allowed to be null");
		}

		if (!appenderList.contains(appender)) {
			appenderList.addElement(appender);
		}
	}

	/**
	 * Remove the specified appender from the appender list.
	 * 
	 * @param appender
	 *            the <code>Appender</code> to remove.
	 */
	public void removeAppender(Appender appender)
			throws IllegalArgumentException {
		if (appender == null) {
			throw new IllegalArgumentException("The appender must not be null.");
		}

		if (appender.isLogOpen()) {
			appender.close();
		}
		appenderList.removeElement(appender);
	}

	/**
	 * Log the message at the specified level.
	 * 
	 * @param level
	 *            the <code>Level</code> to log at.
	 * @param message
	 *            the message to log.
	 * @throws IllegalArgumentException
	 *             if the <code>level</code> is <code>null</code>.
	 */
	public void log(Level level, Object message)
			throws IllegalArgumentException {
		this.log(level, message, null);
	}

	/**
	 * Log the message and the Throwable object at the specified level.
	 * 
	 * @param level
	 *            the log level
	 * @param message
	 *            the message to log.
	 * @param t
	 *            the <code>Throwable</code> object.
	 * @throws IllegalArgumentException
	 *             if the <code>level</code> is <code>null</code>.
	 */
	public void log(Level level, Object message, Throwable t)
			throws IllegalArgumentException {
		if (level == null) {
			throw new IllegalArgumentException("The level must not be null.");
		}

		if (this.level.levelValue <= level.levelValue) 
		{
			int nofAppenders = appenderList.size();

			for (int index = 0; index < nofAppenders; index++) 
			{
				Appender appender = (Appender) appenderList.elementAt(index);
				appender.doLog(name, System.currentTimeMillis(), level, message, t);
			}
		}
	}

	/**
	 * Is this <code>Logger</code> enabled for TRACE level?
	 * 
	 * @return true if logging is enabled.
	 */
	public boolean isTraceEnabled() {
		return level.levelValue <= Level.TRACE_INT;
	}

	/**
	 * Log the message at <code>Level.TRACE</code> level.
	 * 
	 * @param message
	 *            the message to log.
	 */
	public void trace(Object message) {
		log(Level.TRACE, message, null);
	}

	/**
	 * Log the message and the <code>Throwable</code> object at
	 * <code>Level.TRACE</code>.
	 * 
	 * @param message
	 *            the message to log.
	 * @param t
	 *            the Throwable object to log.
	 */
	public void trace(Object message, Throwable t) {
		log(Level.TRACE, message, t);
	}

	/**
	 * Is this <code>Logger</code> enabled for DEBUG level?
	 * 
	 * @return true if logging is enabled.
	 */
	public boolean isDebugEnabled() {
		return level.levelValue <= Level.DEBUG_INT;
	}

	/**
	 * Log the message at <code>Level.DEBUG</code> level.
	 * 
	 * @param message
	 *            the message to log.
	 */
	public void debug(Object message) {
		log(Level.DEBUG, message, null);
	}

	/**
	 * Log the message and the <code>Throwable</code> object at
	 * <code>Level.DEBUG</code> level.
	 * 
	 * @param message
	 *            the message to log.
	 * @param t
	 *            the <code>Throwable</code> object to log.
	 */
	public void debug(Object message, Throwable t) {
		log(Level.DEBUG, message, t);
	}

	/**
	 * Is this <code>Logger</code> enabled for INFO level?
	 * 
	 * @return true if the <code>Level.INFO</code> level is enabled.
	 */
	public boolean isInfoEnabled() {
		return level.levelValue <= Level.INFO_INT;
	}

	/**
	 * Log the specified message at <code>Level.INFO</code> level.
	 * 
	 * @param message
	 *            the message to log.
	 */
	public void info(Object message) {
		log(Level.INFO, message, null);
	}

	/**
	 * Log the specified message and the <code>Throwable</code> at
	 * <code>Level.INFO</code> level.
	 * 
	 * @param message
	 *            the message to log.
	 * @param t
	 *            the <code>Throwable</code> to log.
	 */
	public void info(Object message, Throwable t) {
		log(Level.INFO, message, t);
	}

	/**
	 * Is this <code>Logger</code> enabled for <code>Level.WARN</code> level?
	 * 
	 * @return true if WARN level is enabled.
	 */
	public boolean isWarnEnabled() {
		return level.levelValue <= Level.WARN_INT;
	}

	/**
	 * Log the specified message at <code>Level.WARN</code> level.
	 * 
	 * @param message
	 *            the message to log.
	 */
	public void warn(Object message) {
		log(Level.WARN, message, null);
	}

	/**
	 * Log the specified message and <code>Throwable</code> object at
	 * <code>Level.WARN</code> level.
	 * 
	 * @param message
	 *            the object to log.
	 * @param t
	 *            the <code>Throwable</code> to log.
	 */
	public void warn(Object message, Throwable t) {
		log(Level.WARN, message, t);
	}

	/**
	 * Is this LOGGER enabled for ERROR level?
	 * 
	 * @return true if the ERROR level is enabled.
	 */
	public boolean isErrorEnabled() {
		return level.levelValue <= Level.ERROR_INT;
	}

	/**
	 * Log the specified message at ERROR level.
	 * 
	 * @param message
	 *            the object to log.
	 */
	public void error(Object message) {
		log(Level.ERROR, message, null);
	}

	/**
	 * Log the specified message and Throwable object at ERROR level.
	 * 
	 * @param message
	 *            the object to log.
	 * @param t
	 *            the <code>Throwable</code> to log.
	 */
	public void error(Object message, Throwable t) {
		log(Level.ERROR, message, t);
	}

	/**
	 * Is this LOGGER enabled for FATAL level?
	 * 
	 * @return true if the FATAL level is enabled.
	 */
	public boolean isFatalEnabled() {
		return level.levelValue <= Level.FATAL_INT;
	}

	/**
	 * Log the specified message at FATAL level.
	 * 
	 * @param message
	 *            the object to log.
	 */
	public void fatal(Object message) {
		log(Level.FATAL, message, null);
	}

	/**
	 * Log the specified message and Throwable object at FATAL level.
	 * 
	 * @param message
	 *            the object to log.
	 * @param t
	 *            the <code>Throwable</code> to log.
	 */
	public void fatal(Object message, Throwable t) {
		log(Level.FATAL, message, t);
	}

	/**
	 * @see java.lang.Object#toString()
	 */
	public String toString() {
		StringBuffer stringBuffer = new StringBuffer();
		stringBuffer.append(super.toString());
		stringBuffer.append('[');

		int nofAppenders = appenderList.size();
		for (int index = 0; index < nofAppenders; index++) {
			Appender appender = (Appender) appenderList.elementAt(index);
			stringBuffer.append(appender);
			stringBuffer.append(';');
		}
		stringBuffer.append(']');
		return stringBuffer.toString();
	}

	/**
	 * Open the log. The logging is now turned on.
	 */
	public void open() throws IOException {

		int nofAppenders = appenderList.size();
		for (int index = 0; index < nofAppenders; index++) {
			Appender appender = (Appender) appenderList.elementAt(index);
			appender.open();
		}
	}

	/**
	 * Close the log. From this point on, no logging is done.
	 * 
	 * @throws IOException
	 *             if the <code>Logger</code> failed to close.
	 */
	public void close() throws IOException {
		int nofAppenders = appenderList.size();
		for (int index = 0; index < nofAppenders; index++) {
			Appender appender = (Appender) appenderList.elementAt(index);
			appender.close();
		}
	}
	
	public Object get_appender(Class<?> t)
	{
		for (int i = appenderList.size() - 1; i >= 0; --i)
		{
			if (appenderList.elementAt(i).getClass() == t)
			{
				return appenderList.elementAt(i);
			}
		}
		
		return null;
	}
}
