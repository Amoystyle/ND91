package daemon.model.pim20;

import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;

public class PeopleInfo {

	private Integer id=0;	
	private String accountName="";
	private String accountType = "";
	private boolean starred=false;
	private String custom_ringtone="";

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
	
	public String getAccountName(){
		return accountName;
	}
	public void setAccountName(String value){
		if(null != value){
			accountName = value;
		}
	}
	
	public String getAccountType(){
		return accountType;
	}
	public void setAccountType(String value){
		if(null != value){
			accountType = value;
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
		accountName = reader.ReadString();
		accountType = reader.ReadString();
		custom_ringtone = reader.ReadString();
		starred = reader.ReadBoolean();
	}	
}
