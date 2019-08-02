package daemon.model.notepad;


import daemon.model.ModuleCategory;
import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;

/**
 * <p>
 * this is the Notepad Model Class for the assistance.
 * 
 * @author Yang Feng
 * @version1.0
 */

public class NotepadInfo extends ModuleCategory {
	private String note;
	private Long created;
	private Long modified;
	private NotepadHeadInfo head;

	public NotepadInfo()
	{
		head = new NotepadHeadInfo();
	}
	
	public NotepadHeadInfo getHead() {
		return head;
	}

	public void setHead(NotepadHeadInfo head) {
		this.head = head;
	}

	public String getNote() {
		return note;
	}

	public void setNote(String note) {
		this.note = note;
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

	@Override
	/*
	 * * �ӽ�������������򻯳�Ա
	 * 
	 * @param reader������
	 */
	public void Read(ByteReader reader) {
		// TODO Auto-generated method stub
		head.setId(reader.ReadInt32());
		head.setTitle(reader.ReadString());
		note = reader.ReadString();
		created = reader.ReadInt64();
		modified = reader.ReadInt64();
	}

	@Override
	/*
	 * * ͨ����������л���Ա����
	 * 
	 * @param writer������
	 */
	public void Write(ByteWriter writer) {
		// TODO Auto-generated method stub
		writer.Write(head.getId());
		writer.Write(head.getTitle());
		writer.Write(note);
		writer.Write(created);
		writer.Write(modified);
	}
}
