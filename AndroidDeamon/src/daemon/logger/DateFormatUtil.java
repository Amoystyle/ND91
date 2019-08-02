package daemon.logger;

import java.util.Calendar;
import java.util.TimeZone;
import java.util.Date;

public class DateFormatUtil {
	final static String[] MONTH_ARRAY = { "JAN", "FEB", "MAR", "APR",
			"MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC" };

	private static Calendar calendar = Calendar.getInstance(TimeZone.getDefault());

	private long time_;
	
	public DateFormatUtil(long time)
	{
		time_ = time;
	}
	
	/**
	 * Format as an absolute date time format, that is
	 * 
	 * @param time
	 *            the time to format.
	 * @return the formatted <code>String</code>.
	 */
	String toAbsoluteFormat(long time) {

		calendar.setTime(new Date(time));
		long hours = calendar.get(Calendar.HOUR_OF_DAY);
		StringBuffer buffer = new StringBuffer();

		if (hours < 10) {
			buffer.append('0');
		}
		buffer.append(hours);

		buffer.append(':');

		long minutes = calendar.get(Calendar.MINUTE);
		if (minutes < 10) {
			buffer.append('0');
		}
		buffer.append(minutes);

		buffer.append(':');

		long seconds = calendar.get(Calendar.SECOND);
		if (seconds < 10) {
			buffer.append('0');
		}
		buffer.append(seconds);

		buffer.append(',');

		long milliseconds = calendar.get(Calendar.MILLISECOND);
		if (milliseconds < 10) {
			buffer.append('0');
		}
		buffer.append(milliseconds);

		return buffer.toString();
	}

	String toDateFormat(long time) {

		calendar.setTime(new Date(time));
		StringBuffer buffer = new StringBuffer();

		int year = calendar.get(Calendar.YEAR);
		buffer.append(year);
		buffer.append(' ');
		
		int month = calendar.get(Calendar.MONTH);
		buffer.append(MONTH_ARRAY[month]);
		buffer.append(' ');
		
		int day = calendar.get(Calendar.DAY_OF_MONTH);
		if (day < 10) {
			buffer.append('0');
		}
		buffer.append(day);
		buffer.append(' ');		

		String absoluteTimeString = toAbsoluteFormat(time);
		buffer.append(absoluteTimeString);

		return buffer.toString();
	}

	public String toString()
	{
		return toDateFormat(time_);
	}
	
	public int hashCode() {
		int hash = 8;
		hash = 31 * hash + (int)time_;
		return hash;
	}
}
