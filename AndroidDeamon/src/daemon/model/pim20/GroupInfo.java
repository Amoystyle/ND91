package daemon.model.pim20;

import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;

public class GroupInfo {
	private Integer id;
	private String  name;
	private String  notes;	
	private String  account;
	private String  accountType;
	
	public GroupInfo(){
		id = 0;
		name = "";
		notes = "";	
		account = "";
		accountType = "";
	}
	
	public Integer getId(){
		return id;
	}
	public void setId(Integer value){
		id = value;
	}
	
	public String getName(){
		return name;
	}
	public void setName(String value){			
		if(null != value)
			name = value;
	}
	
	public String getNotes(){
		return notes;
	}
	public void setNotes(String value){			
		if(null != value)
			notes = value;
	}	
	
	public String getAccount(){
		return account;
	}
	public void setAccount(String value){
		if(null != value)
			account = value;
	}
	
	public String getAccountType(){
		return accountType;
	}
	public void setAccountType(String value){
		if(null != value)
			accountType = value;
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
		notes = reader.ReadString();	
		account = reader.ReadString();
		accountType = reader.ReadString();
	}
}
