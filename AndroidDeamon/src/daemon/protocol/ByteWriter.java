package daemon.protocol;

import java.io.UnsupportedEncodingException;

public class ByteWriter {

	// private static final String TAG = "ByteWriter";
	private static final String UTF_8 = "UTF-8";
	protected MemoryStream baseStream = null;

	public ByteWriter() {
		baseStream = new MemoryStream();
	}

	public ByteWriter(int size) {
		baseStream = new MemoryStream(size);
	}

	public ByteWriter(MemoryStream stream) {
		if (stream != null)
			stream = new MemoryStream();
		baseStream = stream;
	}

	public void Flush() {
		baseStream.Flush();
	}

	public MemoryStream getBaseStream() {
		return baseStream;
	}

	public void Write(byte value) {
		baseStream.WriteByte(value);
	}

	public void Write(byte[] buffer) {
		if (buffer != null)
			baseStream.Write(buffer, 0, buffer.length);
	}

	public void Write(byte[] buffer, int offset, int count) {
		if (buffer != null)
			baseStream.Write(buffer, offset, count);
	}

	public void Write(Boolean value) {
		baseStream.WriteByte((byte) (value ? 1 : 0));
	}

	public void Write(char ch) {
		baseStream.WriteByte((byte) ch);
	}

	public void Write(char[] chars) {
		try {
			byte[] buf = new String(chars).getBytes(UTF_8);
			baseStream.Write(buf, 0, buf.length);
		} catch (UnsupportedEncodingException e) {
			// LogUtil.log(TAG, LogCenter.errorRROR, e.getMessage());
		}
	}

	public void Write(double value) {
		byte[] buf = new byte[8];
		long l = Double.doubleToLongBits(value);
		for (int i = 0; i < 8; i++) {
			buf[i] = new Long(l).byteValue();
			l = l >> 8;
		}
		baseStream.Write(buf, 0, buf.length);
	}

	public void Write(float value) {
		byte[] buf = new byte[4];
		int j = Float.floatToIntBits(value);
		for (int i = 0; i < 4; i++) {
			buf[i] = new Integer(j).byteValue();
			j = j >> 8;
		}
		baseStream.Write(buf, 0, buf.length);
	}

	public void Write(short value) {
		byte[] buf = new byte[2];
		if (value != 0) {
			buf[0] = (byte) (value >> 8);
			buf[1] = (byte) (value >> 0);
		}
		baseStream.Write(buf, 0, buf.length);
	}

	public void Write(int value) {
		byte[] buf = new byte[4];
		buf[0] = (byte) (value & 0x000000FF);
		buf[1] = (byte) ((value & 0x0000FF00) >> 8);
		buf[2] = (byte) ((value & 0x00FF0000) >> 16);
		buf[3] = (byte) ((value & 0xFF000000) >> 24);
		baseStream.Write(buf, 0, buf.length);
	}

	public void Write(long value) {
		byte[] buf = new byte[8];
		if (value != 0) {
			for (int index = 0; index < 8; index++) {
				buf[index] = (byte) (value >>> (8 * index) & 0xFF);
			}
		}
		baseStream.Write(buf, 0, buf.length);
	}

	public void Write(String value) {
		if (value != null) {
			try {
				byte[] buf = value.getBytes(UTF_8);
				int len = buf.length;
				Write(len);
				if (len > 0) {
					baseStream.Write(buf, 0, buf.length);
				}
			} catch (UnsupportedEncodingException e) {
				// LogUtil.log(TAG, LogCenter.errorRROR, e.getMessage());
			}
		} else {
			Write(0);
		}
	}

	public void WriteString(String value) {
		if (value != null) {
			try {
				byte[] buf = value.getBytes(UTF_8);
				if (buf.length > 0) {
					baseStream.Write(buf, 0, buf.length);
				}
			} catch (UnsupportedEncodingException e) {
				// LogUtil.log(TAG, LogCenter.errorRROR, e.getMessage());
			}
		}
	}

	public void WriteBlob(byte[] value) {
		if (value != null && value.length > 1) {
			Write(value.length - 1);
			Write(value, 0, value.length - 1);
		} else {
			Write(0);
		}
	}
}
