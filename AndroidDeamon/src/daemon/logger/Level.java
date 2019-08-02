package daemon.logger;

public class Level {
	public static final int FATAL_INT = 16;
	public static final int ERROR_INT = 8;
	public static final int WARN_INT = 4;
	public static final int INFO_INT = 2;
	public static final int DEBUG_INT = 1;
	public static final int TRACE_INT = 0;

	public static final String FATAL_STRING = "FATAL";
	public static final String ERROR_STRING = "ERROR";
	public static final String WARN_STRING = "WARN";
	public static final String INFO_STRING = "INFO";
	public static final String DEBUG_STRING = "DEBUG";
	public static final String TRACE_STRING = "TRACE";
	public static final Level FATAL = new Level(FATAL_INT, FATAL_STRING);
	public static final Level ERROR = new Level(ERROR_INT, ERROR_STRING);
	public static final Level WARN = new Level(WARN_INT, WARN_STRING);
	public static final Level INFO = new Level(INFO_INT, INFO_STRING);
	public static final Level DEBUG = new Level(DEBUG_INT, DEBUG_STRING);
	public static final Level TRACE = new Level(TRACE_INT, TRACE_STRING);

	int levelValue;

	private String levelString = "";

	/**
	 * Create a <code>Level</code> object.
	 * 
	 * @param level
	 *            the level to create. This should be set using one of the
	 *            constants defined in the class.
	 * @param levelString
	 *            the <code>String</code> that shall represent the level. This
	 *            should be set using one of the defined constants defined in
	 *            the class.
	 */
	private Level(int level, String levelString) {
		this.levelValue = level;
		this.levelString = levelString;
	}

	/**
	 * Return the integer level for this <code>Level</code>.
	 * 
	 * @return the integer level.
	 */
	public int toInt() {
		return levelValue;
	}

	/**
	 * Return a <code>String</code> representation for this <code>Level</code>.
	 * 
	 * @return a <code>String</code> representation for the <code>Level</code>.
	 */
	public String toString() {
		return levelString;
	}	
	
	/**
	 * @see java.lang.Object#equals(java.lang.Object)
	 */
	public boolean equals(Object obj) {	
		boolean equals = false;
		
		if (obj instanceof Level) {
			Level compareLevel = (Level) obj;
			
			if(levelValue == compareLevel.levelValue){
				equals = true;
			}
		}
	
		return equals;
	}

	/**
	 * @see java.lang.Object#hashCode()
	 */
	public int hashCode() {
		int hash = 7;
		hash = 31 * hash + levelValue;
		return hash;
	}
}
