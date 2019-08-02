package daemon.model;

import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;

public class EmailAttachment extends ModuleCategory {
	private Integer id;
	private String  fileName;
	private Integer flags;
	private boolean ishtml;
	private byte[]  data;
	
	public Integer getID(){
		return id;
	}
	public void setID(Integer value){
		id = value;
	}
	
	public String getFileName(){
		return fileName;
	}
	public void setFileName(String value){
		fileName = value;
	}
	
	public Integer getFlags(){
		return flags;
	}
	public void setFlags(Integer value){
		flags = value;
	}
	
	public boolean getIsHtml(){
		return ishtml;
	}
	public void setIsHtml(boolean value){
		ishtml = value;
	}
	
	public byte[] getData(){
		return data;
	}
	public void setData(byte[] value){
		data = value;
	}
		
	
	/**
	 * �ӽ�������������򻯳�Ա
	 * 
	 * @param reader
	 *            ������
	 */
	public void Read(ByteReader reader) {
		id = reader.ReadInt32();
		fileName = reader.ReadString();
		flags = reader.ReadInt32();
		ishtml = reader.ReadBoolean();
		Integer dataLen = reader.ReadInt32();
		data = reader.ReadBytes(dataLen);
	}

	/**
	 * ͨ����������л���Ա����
	 * 
	 * @param writer������
	 */
	public void Write(ByteWriter writer) {
		writer.Write(id);
		writer.Write(fileName);
		writer.Write(flags);
		writer.Write(ishtml);
		writer.Write(data.length);
		writer.Write(data);
	}
}
