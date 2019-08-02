package daemon.model;

import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;

public class TodoInfo extends ModuleCategory {
	private Integer id;
	private String title;
	private Integer state; // �Ƿ����
	private Integer alarm;
	private Long begin;
	private Long over;
	private Integer priority; // ���ȼ�
	private String notes;
	private Integer reminder;
	private Long created; // ����ʱ��
	private Long modified; // ����޸�ʱ��

	public Integer getId() {
		return id;
	}

	public void setId(Integer id) {
		this.id = id;
	}

	public String getTitle() {
		return title;
	}

	public void setTitle(String title) {
		this.title = title;
	}

	public Integer getState() {
		return state;
	}

	public void setState(Integer state) {
		this.state = state;
	}

	public Integer getAlarm() {
		return alarm;
	}

	public void setAlarm(Integer alarm) {
		this.alarm = alarm;
	}

	public Long getBegin() {
		return begin;
	}

	public void setBegin(Long begin) {
		this.begin = begin;
	}

	public Long getOver() {
		return over;

	}

	public void setOver(Long over) {
		this.over = over;
	}

	public Integer getPriority() {
		return priority;
	}

	public void setPriority(Integer priority) {
		this.priority = priority;
	}

	public String getNotes() {
		return notes;
	}

	public void setNotes(String notes) {
		this.notes = notes;
	}

	public Integer getReminder() {
		return reminder;
	}

	public void setReminder(Integer reminder) {
		this.reminder = reminder;
	}

	public Long getCreated() {
		return created;
	}

	public void setCreated(Long created) {
		this.created = created;
	}

	public Long getModified() {
		return modified;
	}

	public void setModified(Long modified) {
		this.modified = modified;
	}

	/**
	 * �ӽ�������������򻯳�Ա
	 * 
	 * @param reader
	 *            ������
	 */
	public void Read(ByteReader reader) {
		id = reader.ReadInt32();
		title = reader.ReadString();
		state = reader.ReadInt32();
		alarm = reader.ReadInt32();
		begin = reader.ReadInt64();
		over = reader.ReadInt64();
		priority = reader.ReadInt32();
		notes = reader.ReadString();
		reminder = reader.ReadInt32();
		created = reader.ReadInt64();
		modified = reader.ReadInt64();
	}

	/**
	 * ͨ����������л���Ա����
	 * 
	 * @param writer������
	 */
	public void Write(ByteWriter writer) {
		writer.Write(id);
		writer.Write(title);
		writer.Write(state);
		writer.Write(alarm);
		writer.Write(begin);
		writer.Write(over);
		writer.Write(priority);
		writer.Write(notes);
		writer.Write(reminder);
		writer.Write(created);
		writer.Write(modified);
	}

}
