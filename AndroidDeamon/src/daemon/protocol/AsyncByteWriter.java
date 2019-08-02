package daemon.protocol;

public class AsyncByteWriter extends ByteWriter {
	
//	private String TAG = getClass().getName();
	
	private static final byte[] SYNC_TOKEN = new byte[] { (byte) 'S', (byte) 'Y',
		(byte) 'N', (byte) 'C' };
	
	private static final byte[] SYNC_END = new byte[] { (byte) 'S', (byte) 'E',
		(byte) 'N', (byte) 'D' };
	
	private IAsyncChannel _asyncChannel;
	private boolean _isAsyncSend = false;
	private boolean _isFirst = true;
	private static final int HEADER_LENGTH = 16;
	private static final int SYNC_HEADER_LENGTH = 8;
	
	public  boolean IsAsyncSend()
	{
		return _isAsyncSend;
	}
	
	public AsyncByteWriter() {
		super();
	}
	
	public AsyncByteWriter(IAsyncChannel channel) {
		super();
		this._asyncChannel = channel;
	}

	public AsyncByteWriter(IAsyncChannel channel, int bufferSize) {
		super(bufferSize);
		this._asyncChannel = channel;
	}
	
	public AsyncByteWriter(MemoryStream stream) {
		super(stream);
	}
	
	public AsyncByteWriter(MemoryStream stream,IAsyncChannel channel) {
		super(stream);
		this._asyncChannel = channel;
	}
	
	public void AsyncSendEnd()
	{
		int tmpOffset = super.baseStream.getOffset();
		Write(SYNC_END);
		_asyncChannel.AsyncSend(super.baseStream, tmpOffset, SYNC_END.length);
		
	}
	public int AsyncSend()
	{
		if(super.baseStream == null)
			return 0;
		
		if(_asyncChannel == null)
			return 0;
		
		if(super.baseStream.getOffset() == 0)
			return 0;
		
		_isAsyncSend = true;
		int AsyncPos = 0;
		int tmpPos = super.baseStream.getOffset();
		int length = tmpPos;
		int syncLength = tmpPos;
		if(_isFirst)
		{		
			super.baseStream.setOffset(tmpPos);
			super.baseStream.MoveSize(HEADER_LENGTH, SYNC_HEADER_LENGTH, length - HEADER_LENGTH);
			length += SYNC_HEADER_LENGTH;
			syncLength = tmpPos + SYNC_HEADER_LENGTH - HEADER_LENGTH; 
			_isFirst = false;
			AsyncPos = HEADER_LENGTH;
			tmpPos = length;
		}	
		
		super.baseStream.setOffset(AsyncPos);
		Write(SYNC_TOKEN);
		Write(syncLength);
		
		super.baseStream.setOffset(tmpPos);
		int totalSend = _asyncChannel.AsyncSend(super.baseStream, 0, length);
		super.baseStream.setOffset(SYNC_HEADER_LENGTH);
		super.baseStream.SetLength(SYNC_HEADER_LENGTH);
		//LogUtil.log(TAG, "Do AsyncSend.AsyncPos:"+AsyncPos+",length:"+length);
		
		return totalSend;
	}
}

