package daemon.model.calendar;

import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;
import daemon.protocol.IBaseEntity;


public class Reminders implements IBaseEntity {
	private int id;
	private int eventId;
	private int minutes;
	private int method;

	public void setId(int id) {
		this.id = id;
	}

	public int getId() {
		return id;
	}

	public void setEventId(int eventId) {
		this.eventId = eventId;
	}

	public int getEventId() {
		return eventId;
	}

	public void setMinutes(int minutes) {
		this.minutes = minutes;
	}

	public int getMinutes() {
		return minutes;
	}

	public void setMethod(int method) {
		this.method = method;
	}

	public int getMethod() {
		return method;
	}

	@Override
	public Integer GetAction() {
		return 0;
	}

	@Override
	public void Read(ByteReader reader) {
		id = reader.ReadInt32();
		eventId = reader.ReadInt32();
		minutes = reader.ReadInt32();
		method = reader.ReadInt32();
	}

	@Override
	public void Write(ByteWriter writer) {
		writer.Write(id);
		writer.Write(eventId);
		writer.Write(minutes);
		writer.Write(method);
	}
}
