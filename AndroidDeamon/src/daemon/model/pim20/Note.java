package daemon.model.pim20;

import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;

public class Note extends MethodBase{

	private String data;
	
	public Note(){
		setKind(MethodKind.Note);
		data = "";
	}
	
	public String getData(){
		return data;
	}
	public void setData(String value){
		data = value;
		
		if(null == value)
			data = "";
	}
	
	@Override
	public void Read(ByteReader reader) {
		BaseFromStream(reader);
		
		data = reader.ReadString();
	}
}
