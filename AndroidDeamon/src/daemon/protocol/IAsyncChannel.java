package daemon.protocol;

import java.nio.ByteBuffer;

public interface IAsyncChannel {
	int AsyncSend(MemoryStream stream, int offset, int length);
	int AsyncRead(ByteBuffer packageTemp, int offset, int length);
	int SendBuffer(ByteBuffer packageTemp);
}
