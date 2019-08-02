package daemon.logger;

public class SimpleFormatter implements Formatter {
	
	public static final String DEFAULT_DELIMITER = "\n\r";
	private static final int INITIAL_BUFFER_SIZE = 256;
	StringBuffer buffer = new StringBuffer(INITIAL_BUFFER_SIZE);
	private String delimiter = DEFAULT_DELIMITER;

	public SimpleFormatter () {}
	
	public String getDelimiter() {
		return delimiter;
	}

	/**
	 * Set the delimiter that is used between the different fields when logging.
	 * 
	 * @param delimiter
	 *            the delimiter to set
	 */
	public void setDelimeter(String delimiter) {
		this.delimiter = delimiter;
	}

	/**
	 * Format the given message and the Throwable object. The format is
	 * <code>{Level}{-message.toString()}{-t}</code>
	 * @param level
	 *            the logging level. If null, it is not appended to the String.
	 * @param message
	 *            the message. If null, it is not appended to the String.
	 * @param t
	 *            the exception. If null, it is not appended to the String.
	 * @return a String that is not null.
	 */
	public String format(String name, long time, Level level, Object message, Throwable t) {
		if (buffer.length() > 0) {
			buffer.delete(0, buffer.length());
		}
		
		buffer.append(new DateFormatUtil(time));
		buffer.append(':');
		
		buffer.append("[thread: " + Thread.currentThread().getId() + "] ");

		if (level != null) {
			buffer.append('[');
			buffer.append(level);
			buffer.append(']');
		}
		
		if (message != null) {
			//buffer.append(delimiter);
			buffer.append(' ');
			
			if (null != name)
			{
				buffer.append('[');
				buffer.append(name);
				buffer.append(']');
			}
			
			buffer.append(message);
			if (buffer.lastIndexOf(DEFAULT_DELIMITER) != buffer.length() - 2)
			{
				buffer.append(DEFAULT_DELIMITER);
			}
		}

		if (t != null) {
			buffer.append(delimiter);
			buffer.append(t);
		}

		return buffer.toString();
	}
}
