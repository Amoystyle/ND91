package daemon.model;

import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;

/**
 * <p>
 * java bean object for the BookMark table.
 * 
 * @version 1.0
 * 
 */
public class AlarmInfo extends ModuleCategory {
	private Integer id;
	private Integer hour;
	private Integer minutes;
	private Integer daysofweek;
	private long alarmtime;
	private Integer enabled;
	private Integer vibrate;
	private String message;
	private String alert;
	
	public AlarmInfo(){
		id = 0;
		hour = 0;
		minutes = 0;
		daysofweek = 0;
		alarmtime = 0;
		enabled = 0;
		vibrate = 0;
		message = "";
		alert = "";
	}


	public Integer getId() {
		return id;
	}

	public void setId(Integer value) {
		id = value;
	}

	public Integer getHour() {
		return hour;
	}

	public void setHour(Integer value) {
		hour = value;
	}

	public Integer getMinutes() {
		return minutes;
	}

	public void setMinutes(Integer value) {
		minutes = value;
	}

	public Integer getDaysOfWeek() {
		return daysofweek;
	}

	public void setDaysOfWeek(Integer value) {
		daysofweek = value;
	}

	public long getAlarmTime() {
		return alarmtime;
	}

	public void setAlarmTime(long value) {
		alarmtime = value;
	}

	public Integer getEnabled() {
		return enabled;
	}

	public void setEnabled(Integer value) {
		enabled = value;
	}

	public Integer getVibrate() {
		return vibrate;
	}

	public void setVibrate(Integer value) {
		vibrate = value;
	}

	public String getMessage() {
		return message;
	}

	public void setMessage(String value) {
		if (value != null)
			message = value;
		else
			message = "";
	}

	public String getAlert() {
		return alert;
	}

	public void setAlert(String value) {
		if (value != null)
			alert = value;
		else 
			alert = "";
	}

	/**
	 * �ӽ�����������򻯳�Ա
	 * 
	 * @param reader
	 *            ������
	 */
	public void Read(ByteReader reader) {
		id = reader.ReadInt32();
		hour = reader.ReadInt32();
		minutes = reader.ReadInt32();
		daysofweek = reader.ReadInt32();
		alarmtime = reader.ReadInt64();
		enabled = reader.ReadInt32();
		vibrate = reader.ReadInt32();
		message = reader.ReadString();
		alert = reader.ReadString();
	}

	/**
	 * ͨ����������л���Ա����
	 * 
	 * @param writer
	 *            ������
	 */
	public void Write(ByteWriter writer) {
		writer.Write(id);
		writer.Write(hour);
		writer.Write(minutes);
		writer.Write(daysofweek);
		writer.Write(alarmtime);
		writer.Write(enabled);
		writer.Write(vibrate);
		writer.Write(message);
		writer.Write(alert);
	}
}
