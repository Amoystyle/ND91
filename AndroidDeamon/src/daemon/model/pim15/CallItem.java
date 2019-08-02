package daemon.model.pim15;

import daemon.protocol.ByteReader;

public class CallItem {
    private int id =0;
    private String number = "";
    private long date = 0;
    private int type = 0;
    private long duration =0;
    
    public int getId(){
    	return id;
    }
    public void setId(int value){
    	id = value;
    }
    
    public String getNumber(){
    	return number;
    }
    
    public long getDate(){
    	return date;
    }
    
    public int getType(){
    	return type;
    }
    
    public long getDuration(){
    	return duration;
    }
    
    public void Read(ByteReader reader){
    	id = reader.ReadInt32();
    	number = reader.ReadString();
    	date = reader.ReadInt64();
    	type = reader.ReadInt32();
    	duration = reader.ReadInt64();
    }
}
