package daemon.model.pim20;

import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;

public class Raw_Contact {
	private int contactId;
	private String accountName;
	
	public Raw_Contact(){
		contactId = 0;
		accountName = "";
	}
	
	public int getContactId(){
		return contactId;
	}
	public void setContactId(int value){
		contactId = value;
	}
	
	public String getAccountName(){
		return accountName;
	}
	public void setAccountName(String value){
		if(null != value){
			accountName = value;
		}
	}
	
	
	public void Read(ByteReader reader) {
		contactId = reader.ReadInt32();
		accountName = reader.ReadString();
	}

	
	public void Write(ByteWriter writer) {
		writer.Write(MethodKind.RawContact.getValue());
		writer.Write(contactId);
		writer.Write(accountName);	
	}
}
