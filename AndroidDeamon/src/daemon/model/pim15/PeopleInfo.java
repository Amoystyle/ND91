package daemon.model.pim15;

import daemon.protocol.ByteReader;

public class PeopleInfo {

	private Integer id=0;	
	private String name = "";
	private boolean starred=false;
	private String custom_ringtone="";
	private String note = "";

	private ActionState actionState; 
	
	public PeopleInfo(){	
	}

	public Integer getId() {
		return id;
	}

	public void setId(Integer value) {
		id = value;
	}
	
	public String getCustomRingtone() {
		return custom_ringtone;
	}

	public void setCustomRingtone(String value) {
		if(value!=null)
		custom_ringtone = value;
	}

	public ActionState getActionState() {
		return actionState;
	}
	
	public String getName(){
		return name;
	}
	public void setName(String value){
		if(null != value){
			name = value;
		}
	}
	
	public String getNote(){
		return note;
	}
	public void setNote(String value){
		if(null != value){
			note = value;
		}
	}
	
	public boolean getStarred(){
		return starred;
	}
	public void setStarred(boolean value){	
		starred = value;
	}

	/**
	 * �ӽ�����������򻯳�Ա
	 * 
	 * @param reader
	 *            ������
	 */
	public void Read(ByteReader reader) {		
		id = reader.ReadInt32();
		name = reader.ReadString();		
		custom_ringtone = reader.ReadString();
		starred = reader.ReadBoolean();
		note = reader.ReadString();
	}	
}
