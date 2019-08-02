package daemon.model.pim20;

import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;

public class Account {
	private String name;
	private String type;
	
	public Account(){
		name = "";
		type = "";
	}
	
	public String getName(){
		return name;
	}
	public void setName(String value){
		if(null != value){
			name = value;
		}
	}
	
	public String getType(){
		return type;
	}
	public void setType(String value){
		if(null != value){
			type = value;
		}
	}
	
	/**
	 * �ӽ�����������򻯳�Ա
	 * 
	 * @param reader
	 *            ������
	 */
	public void Read(ByteReader reader) {		
		name = reader.ReadString();
		type = reader.ReadString();
	}

	/**
	 * ͨ����������л���Ա����
	 * 
	 * @param writer������
	 */
	public void Write(ByteWriter writer) {	
		writer.Write(MethodKind.Account.getValue());
		writer.Write(name);
		writer.Write(type);		
	}
}
