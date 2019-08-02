package daemon.logger;

public interface Formatter {
	/**
	 * Format the given message and the Throwable object.
	 * @param name
	 *            the name of the logger.
	 * @param time
	 *            the time since the first logging has done (in milliseconds).
	 * @param level
	 *            the logging level
	 * @param message
	 *            the message
	 * @param t
	 *            the exception.
	 * 
	 * @return a String that is not null.
	 */
	String format(String name, long time, Level level, Object message, Throwable t);
}
