package daemon.model.calendar;


import daemon.model.EntityCollection;
import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;
import daemon.protocol.IBaseEntity;


public class CalendarInfo implements IBaseEntity {
	private Integer id;
	private String sync_count;
	private String name;
	private String displayName;
	private Integer hidden;
	private Integer color;
	private Integer selected;
	private String timezone;

	private EntityCollection<CalendarEvents> events = new EntityCollection<CalendarEvents>(
			"CalendarEvents");

	public EntityCollection<CalendarEvents> getEvents() {
		return events;
	}

	public void setEvents(EntityCollection<CalendarEvents> eventList) {
		this.events = eventList;
	}
	
	public Integer getId() {
		return id;
	}

	public void setId(Integer id) {
		this.id = id;
	}

	public String getSync_count() {
		return sync_count;
	}

	public void setSync_count(String sync_count) {
		this.sync_count = sync_count;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getDisplayName() {
		return displayName;
	}

	public void setDisplayName(String displayName) {
		this.displayName = displayName;
	}

	public Integer getHidden() {
		return hidden;
	}

	public void setHidden(Integer hidden) {
		this.hidden = hidden;
	}

	public Integer getColor() {
		return color;
	}

	public void setColor(Integer color) {
		this.color = color;
	}

	public Integer getSelected() {
		return selected;
	}

	public void setSelected(Integer selected) {
		this.selected = selected;
	}

	public String getTimezone() {
		return timezone;
	}

	public void setTimezone(String timezone) {
		this.timezone = timezone;
	}

	@Override
	public Integer GetAction() {
		return 0;
	}

	@Override
	public void Read(ByteReader reader) {

		EntityCollection<CalendarInfo> infos = new EntityCollection<CalendarInfo>(
				"CalendarInfo");
		infos.Read(reader);
	}

	@Override
	public void Write(ByteWriter writer) {

		if (id == null) {
			writer.Write((byte) 0);
		} else {
			writer.Write((byte) 1);
			writer.Write(id);
		}

		if (sync_count == null) {
			writer.Write((byte) 0);
		} else {
			writer.Write((byte) 1);
			writer.Write(sync_count);
		}

		if (name == null) {
			writer.Write((byte) 0);
		} else {
			writer.Write((byte) 1);
			writer.Write(name);
		}

		if (displayName == null) {
			writer.Write((byte) 0);
		} else {
			writer.Write((byte) 1);
			writer.Write(displayName);
		}

		if (hidden == null) {
			writer.Write((byte) 0);
		} else {
			writer.Write((byte) 1);
			writer.Write(hidden);
		}

		if (color == null) {
			writer.Write((byte) 0);
		} else {
			writer.Write((byte) 1);
			writer.Write(color);
		}

		if (selected == null) {
			writer.Write((byte) 0);
		} else {
			writer.Write((byte) 1);
			writer.Write(selected);
		}

		if (timezone == null) {
			writer.Write((byte) 0);
		} else {
			writer.Write((byte) 1);
			writer.Write(timezone);
		}

		if (events == null || events.size() < 1) {
			writer.Write((byte) 0);
		} else {
			writer.Write((byte) 1);
			events.Write(writer);
		}
	}
	}
