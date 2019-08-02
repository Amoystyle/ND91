package daemon.protocol;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;

public class MemoryStream {

	protected static final int SENDBUFFERSIZE = 64 * 1024;
	protected int buffsize = 1024 * 16;
	protected byte[] buffer = null;
	protected int position = 0;
	protected int length = 0;

	public enum SeekOrigin {
		Begin, Current, End
	}

	/*****************************************************************************/
	// C.tor
	/*****************************************************************************/

	public MemoryStream() {
		buffer = new byte[buffsize];
	}

	public MemoryStream(int size) {
		buffer = new byte[size];
	}

	public MemoryStream(byte[] buff) {
		int len = 0;
		if ((null != buff) && ((len = buff.length) > 0)) {
			buffer = new byte[len];
			System.arraycopy(buff, 0, buffer, 0, len);
			length = buff.length;
		}
	} 

	public MemoryStream(ByteBuffer buff) {
		if (null != buff) {
			buffer = buff.array();
		}
	}

	/*****************************************************************************/
	// Property
	/*****************************************************************************/

	public int getOffset() {
		return position;
	}

	public void setOffset(int value) {
		if (value < 0)
			position = 0;
		else if (value > length)
			position = length;
		else
			position = value;
	}

	public int getLength() {
		return length;
	}

	public int getBufferSize() {
		return buffsize;
	}

	/*****************************************************************************/
	// Methods
	/*****************************************************************************/

	public int Append(ByteBuffer buff) {
		if (null != buff) {
			int len = buff.capacity();
			Resize(len);
			System.arraycopy(buff.array(), 0, buffer, position, len);
		}
		return 0;
	}

	public void Flush() {
		if (buffer == null)
			buffer = new byte[buffsize];
		position = 0;
	}

	public int Seek(int offset, SeekOrigin origin) {
		switch (origin) {
		case Begin:
			position = offset >= length ? length : offset;
			break;
		case Current:
			position = position + offset >= length ? length : position + offset;
			break;
		case End:
			position = length - offset > 0 ? length - offset : 0;
			break;
		}

		return position;
	}

	public int Read(byte[] buff, int offset, int count) {
		if (buff != null && buff.length >= count && position < length) {
			if (position + count > length)
				count = length - position - 1;
			System.arraycopy(buffer, position, buff, offset, count);
			position += count;
			return count;
		}
		return 0;
	}

	public byte ReadByte() {
		if (position < length) {
			return buffer[position++];
		}
		return -1;
	}

	public void Write(byte[] buff, int offset, int count) {
		if (buff != null && buff.length >= count && count > 0) {
			if (count > buff.length - offset)
				count = buff.length - offset - 1;
			Resize(count);
			System.arraycopy(buff, offset, buffer, position, count);
		
			position += count;
			if (position > length)
				length = position;
		}
	}

	public void WriteByte(byte value) {
		Resize(1);
		buffer[position++] = value;
		if (position > length)
			length = position;
	}

	public ByteBuffer GetBuffer() {
		return ByteBuffer.wrap(buffer, 0, length);
	}

	public ByteBuffer GetBuffer(int offset, int mlength) {
		return ByteBuffer.wrap(buffer, offset, mlength);
	}

	public int Send(SocketChannel channel) throws IOException {
		if (position < length) {
			int sendsize = SENDBUFFERSIZE;
			if (position + SENDBUFFERSIZE > length) {
				sendsize = length - position;
			}
            
			channel.write(ByteBuffer.wrap(buffer, position, sendsize));
			position += sendsize;
			return sendsize;
		} else {
			return 0;
		}
	}

	public int SendBuffer(IAsyncChannel channel) throws IOException {
		int totalSendSize = 0;
		ByteBuffer sendBuffer = null;
		
		if (position + SENDBUFFERSIZE > length)
			sendBuffer = ByteBuffer.allocate(length - position);
		else
			sendBuffer = ByteBuffer.allocate(SENDBUFFERSIZE);
		while (totalSendSize < length) {
			int buffersize = SENDBUFFERSIZE;
			if (position + SENDBUFFERSIZE > length) {
				buffersize = length - position;
			}

			sendBuffer = ByteBuffer.wrap(buffer, position, buffersize);
			int sendsize = channel.SendBuffer(sendBuffer);
			position += sendsize;
			totalSendSize += sendsize;
			sendBuffer.clear();
		}

		sendBuffer = null;
		return totalSendSize;
	}

	public byte[] ToArray() {
		if (position > 0) {
			byte[] bytes = new byte[position];
			System.arraycopy(buffer, 0, bytes, 0, position);
			return bytes;
		} else
			return null;
	}

	public byte[] GetArray() {
		return buffer;
	}

	public void MoveSize(int offset, int mlength, int movesize) {
		byte[] buff = new byte[mlength];
		Write(buff, 0, mlength);

		System.arraycopy(buffer, offset, buffer, offset + mlength, movesize);
	}

	public void SetLength(int length) {
		this.length = length;
	}

	protected void Resize(int len) {

		if (position + len > buffer.length) {
			int newsize = position + (len / buffsize) * buffsize
					+ ((len % buffsize) == 0 ? 0 : buffsize);
			byte[] buff = new byte[newsize];
			System.arraycopy(buffer, 0, buff, 0, length);
			buffer = buff;
		}
	}
}
