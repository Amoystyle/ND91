package daemon.model;

import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;

public class EmailInfo extends ModuleCategory{
	
	private Integer id;
	private String  uidl;
	private Integer type;
	private String  from;
	private String  fromDisplay;
	private String  to;
	private String  toDisplay;
	private String  replyTo;
	private Long    date;
	private String  subject;
	private Integer flags;
	private Integer readSize;
	private Integer totalSize;
	private Integer accountID;
	
	public Integer getId(){
		return id;
	}
	public void setId(Integer value){
		id = value;
	}
	
	public String getUidl(){
		return uidl;
	}
	public void setUidl(String value){
		uidl = value;
	}
	
	public Integer getType(){
		return type;
	}
	public void setType(Integer value){
		type = value;
	}
	
	public String getFrom(){
		return from;
	}
	public void setFrom(String value){
		from = value;
	}
	
	public String getFromDisplay(){
		return fromDisplay;
	}
	public void setFromDisplay(String value){
		fromDisplay = value;
	}
	
	public String getTo(){
		return to;
	}
	public void setTo(String value){
		to = value;
	}
	
	public String getToDisplay(){
		return toDisplay;
	}
	public void setToDisplay(String value){
		toDisplay = value;
	}
	
	public String getReplyTo(){
		return replyTo;
	}
	public void setReplyTo(String value){
		replyTo = value;
	}
	
	public Long getDate(){
		return date;
	}
	public void setDate(Long value){
		date = value;
	}
	
	public String getSubject(){
		return subject;
	}
	public void setSubject(String value){
		subject = value;
	}
	
	public Integer getFlags(){
		return flags;
	}
	public void setFlags(Integer value){
		flags = value;
	}
	
	public Integer getReadSize(){
		return readSize;
	}
	public void setReadSize(Integer value){
		readSize = value;
	}
	
	public Integer getTotalSize(){
		return totalSize;
	}
	public void setTotalSize(Integer value){
		totalSize = value;
	}
	
	public Integer getAccountID(){
		return accountID;
	}
	public void setAccountID(Integer value){
		accountID = value;
	}
		
	/**
	 * �ӽ�������������򻯳�Ա
	 * 
	 * @param reader
	 *            ������
	 */
	public void Read(ByteReader reader) {
		id = reader.ReadInt32();
		uidl = reader.ReadString();
		type = reader.ReadInt32();
		from = reader.ReadString();
		fromDisplay = reader.ReadString();
		to = reader.ReadString();
		toDisplay = reader.ReadString();
		replyTo = reader.ReadString();
		date = reader.ReadInt64();
		subject = reader.ReadString();
		flags = reader.ReadInt32();
		readSize = reader.ReadInt32();
		totalSize = reader.ReadInt32();
		accountID = reader.ReadInt32();
	}

	/**
	 * ͨ����������л���Ա����
	 * 
	 * @param writer������
	 */
	public void Write(ByteWriter writer) {
		writer.Write(id);
		writer.Write(uidl);
		writer.Write(type);
		writer.Write(from);
		writer.Write(fromDisplay);
		writer.Write(to);
		writer.Write(toDisplay);
		writer.Write(replyTo);
		writer.Write(date);
		writer.Write(subject);
		writer.Write(flags);
		writer.Write(readSize);
		writer.Write(totalSize);
		writer.Write(accountID);
	}
}
