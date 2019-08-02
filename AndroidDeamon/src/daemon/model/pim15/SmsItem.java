package daemon.model.pim15;

import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;

public class SmsItem {
    int id = 0;
    String number = "";
    long date = 0;
    int type = 1;
    String body = "";
    boolean read = true;
    int threadId = 0;
    
    public int getId(){
    	return id;
    }
    public void setId(int value){
    	id = value;
    }
    
    public String getNumber(){
    	return number;
    }
    public void setNumber(String value){
    	if(value != null)
    		number = value;
    	else
    		number = "";
    }
    
    public long getDate(){
    	return date;
    }
    public void setDate(long value){
    	date = value;
    }
    
    public int getType(){
    	return type;
    }
    public void setType(int value){
    	type = value;
    }
    
    public String getBody(){
    	return body;
    }
    public void setBody(String value){
    	if(value != null)
    		body = value;
    	else
    		body = "";
    }
    
    public boolean getRead(){
    	return read;
    }
    public void setRead(boolean value){
    	read = value;
    }
    
    public int getThreadId(){
    	return threadId;
    }
    public void setThreadId(int id){
    	threadId = id;
    }
    
    public void Read(ByteReader reader){
    	id = reader.ReadInt32();
    	number = reader.ReadString();
    	date = reader.ReadInt64();
    	type = reader.ReadInt32();
    	body = reader.ReadString();
    	read = reader.ReadBoolean();
    }
    
    public void Write(ByteWriter writer){
    	writer.Write(id);
    	writer.Write(threadId);
    	writer.Write(number);
    	writer.Write(date);
    	writer.Write(type);
    	writer.Write(body);
    	writer.Write(read);
    }
}
