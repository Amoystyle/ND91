package daemon.provider;


import daemon.access.ProviderAccess;
import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;

public class ProviderExecuteContext {

	private ByteReader reader;
	private ByteWriter writer;
	private ProviderAccess access;
	private boolean cancel = false;

	public ProviderExecuteContext(ByteReader reader, ByteWriter writer,
			ProviderAccess access) {
		this.reader = reader;
		this.writer = writer;
		this.access = access;
	}

	public ByteReader getByteReader() {
		return reader;
	}

	public ByteWriter getByteWriter() {
		return writer;
	}

	public ProviderAccess getAccess() {
		return access;
	}

	public boolean isCancel() {
		return cancel;
	}

	public void cancel() {
		cancel = true;
	}
}
