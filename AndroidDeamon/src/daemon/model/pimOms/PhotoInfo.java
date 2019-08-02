package daemon.model.pimOms;

import daemon.protocol.ByteReader;

public class PhotoInfo extends MethodBase {
	private byte[] data;

	public PhotoInfo() {
		setKind(MethodKind.Photo);
		data = null;
	}

	public byte[] getData() {
		return data;
	}

	public void setData(byte[] value) {
		data = value;
	}

	@Override
	public void Read(ByteReader reader) {
		BaseFromStream(reader);

		int len = reader.ReadInt32();

		if (len > 0)
			data = reader.ReadBytes(len);
		else
			data = null;
	}
}
