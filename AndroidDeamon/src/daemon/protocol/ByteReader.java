package daemon.protocol;

import java.io.UnsupportedEncodingException;

public class ByteReader {
	
//	private static final String TAG = "ByteReader";
	private static final String UTF_8 = "UTF-8";
	protected MemoryStream baseStream = null;
	
	public ByteReader(MemoryStream stream){
		if(stream!=null)
			baseStream = stream;
	}
	
	public int available(){
		return baseStream.getLength() - baseStream.getOffset();
	}
	
	public MemoryStream getBaseStream(){
		return baseStream;
	}
	
	public int Read (){
		return baseStream.ReadByte();
	}
		
	public int Read(byte[] buffer, int index, int count){
		baseStream.Read(buffer, index, count);
		return buffer.length;
	}

	public byte ReadByte(){
		return baseStream.ReadByte();
	}
	
	public byte[] ReadBytes(int count){
		byte[] buf = new byte[count];
		baseStream.Read(buf, 0, count);
		return buf;
	}
	
	public boolean ReadBoolean (){
		return baseStream.ReadByte()!=0;
	}

	public char ReadChar(){
		return (char)baseStream.ReadByte();
	}

	public char[] ReadChars(int count){
		byte[] bytes = new byte[count];
		baseStream.Read(bytes, 0, count);
		char chars[]= null;
		try{
			String s = new String(bytes,0,count,UTF_8);
			chars = s.toCharArray(); 
		} catch (UnsupportedEncodingException e) {
//            LogUtil.log(TAG, LogCenter.errorRROR, e.getMessage());
        }		  
		return chars; 
	}
	
	public double ReadDouble (){
		byte[] bytes = new byte[8];
		baseStream.Read(bytes, 0, 8);		
		long l;
	    l=bytes[0];
	    l&=0xff;
	    l|=((long)bytes[1]<<8);
	    l&=0xffff;
	    l|=((long)bytes[2]<<16);
	    l&=0xffffff;
	    l|=((long)bytes[3]<<24);
	    l&=0xffffffffl;
	    l|=((long)bytes[4]<<32);
	    l&=0xffffffffffl;
	    l|=((long)bytes[5]<<40);
	    l&=0xffffffffffffl;
	    l|=((long)bytes[6]<<48);
	    l&=0xffffffffffffffl;
	    l|=((long)bytes[7]<<56);
	    return Double.longBitsToDouble(l);
	}
	
	public float ReadSingle(){
		byte[] bytes = new byte[4];
		baseStream.Read(bytes, 0, 4);		
		int accum = 0; 
        for ( int shiftBy = 0; shiftBy < 4; shiftBy++ ) { 
                accum |= (bytes[shiftBy] & 0xff) << shiftBy * 8; 
        } 
        return Float.intBitsToFloat(accum); 
	}

	public short ReadInt16(){
		byte[] bytes = new byte[2];
		baseStream.Read(bytes, 0, 2);	
		return  (short)(((bytes[0] << 8) | bytes[1] & 0xff));
	}
	
	public int ReadInt32(){
		byte[] bytes = new byte[4];
		baseStream.Read(bytes, 0, 4);	
		int result = 0;
		result |= ((int)bytes[0]) & 0x000000FF;
		result |= (((int)bytes[1]) & 0x000000FF) << 8;
		result |= (((int)bytes[2]) & 0x000000FF) << 16;
		result |= (((int)bytes[3]) & 0x000000FF) << 24;
		return result;
	}
	
	public static short ReadInt16(byte[] bytes, int offset){	
		return  (short)(((bytes[offset] & 0xff) | bytes[offset + 1] << 8));
	}
	
	public static int ReadInt32(byte[] bytes, int offset){
		int result = 0;
		result |= ((int)bytes[offset]) & 0x000000FF;
		result |= (((int)bytes[offset + 1]) & 0x000000FF) << 8;
		result |= (((int)bytes[offset + 2]) & 0x000000FF) << 16;
		result |= (((int)bytes[offset + 3]) & 0x000000FF) << 24;
		return result;
	}

	public long ReadInt64(){
		byte[] bytes = new byte[8];
		baseStream.Read(bytes, 0, 8);	
		long result = 0;
		for (int index = 0; index < 8; index++) {
            result |= (bytes[index] & 0xFFL) << (((index)) * 8);
        }
		return result;
	}

	public String ReadString(){
		String s = "";
		int len = ReadInt32();
		if(len>0){
			byte[] bytes = new byte[len];
			baseStream.Read(bytes, 0, len);
			try{
				s = new String(bytes,0,len,UTF_8);
			} catch (UnsupportedEncodingException e) {
//	            LogUtil.log(TAG, LogCenter.errorRROR, e.getMessage());
	        }
		}
		return s;
	}
}
