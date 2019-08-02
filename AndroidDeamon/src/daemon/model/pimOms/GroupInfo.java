package daemon.model.pimOms;

import daemon.protocol.ByteReader;

public class GroupInfo {
	private Integer id;
	private String  name;
	private String  notes;	
	
	public GroupInfo(){
		id = 0;
		name = "";
		notes = "";		
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
	}
}
