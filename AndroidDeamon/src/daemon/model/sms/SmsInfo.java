package daemon.model.sms;


import daemon.model.ModuleCategory;
import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;

public class SmsInfo extends ModuleCategory {

	public SmsInfo()
	{
		
	}
	
	public SmsInfo(int action)
	{
	  super(action);	
	}
	
	private int id;
	private long threadid=0;
	private String number = "";
	private String content = "";
	private long dataTime = 0;
	// Receive = 1,Sended = 2, Draft = 3, SendError = 6,
	private Integer type = 1;
	private int read;

	public Integer getType() {
		return type;
	}

	public void setType(Integer type) {
		if (type != null)
			this.type = type;
	}

	public String getNumber() {
		return number;
	}

	public void SetNumber(String number) {
		if (number != null)
			this.number = number;
	}

	public String getContent() {
		return content;
	}

	public void setContent(String content) {
		if (content != null)
			this.content = content;
	}

	public long getDataTime() {
		return dataTime;
	}

	public void setDataTime(long dataTime) {
		this.dataTime = dataTime;
	}

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	/**
	 * �ӽ�������������򻯳�Ԅ1�7
	 * 
	 * @param reader
	 *            ������
	 */
	public void Read(ByteReader reader) 
	{
		threadid=reader.ReadInt64();
		int type = reader.ReadInt32();
		String number = reader.ReadString();
		String text = reader.ReadString();
		setType(type);
		SetNumber(number);
		setContent(text);
		setDataTime(reader.ReadInt64());
		SetRead(reader.ReadInt32());
	}

	public void SetRead(int value) 
	{
		read=value;
	}
	
	public int GetRead() 
	{
		 return  read ;
	}

	/**
	 * ͨ����������л���Ա���ￄ1�7
	 * 
	 * @param writer������
	 */
	public void Write(ByteWriter writer)
	{
		writer.Write(threadid);
		writer.Write(id);
		writer.Write(this.type);
		writer.Write(this.number);
		writer.Write(this.content);
		writer.Write(this.dataTime);
	}

	public void setThreadId(long threadId)
	{
		this.threadid=threadId;
		
	}
}
