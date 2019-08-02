package daemon.model.pimOms;

import daemon.protocol.ByteReader;

public class IM extends MethodBase {

	private String type;
	private String imAddress;
	private String label;

	public IM() {
		setKind(MethodKind.IM);
		type = "";
		imAddress = "";
		label = "";
	}

	public String getType() {
		return type;
	}

	public void setType(String value) {
		if (value != null)
			type = value;
	}

	public String getImAddress() {
		return imAddress;
	}

	public void setImAddress(String value) {
		imAddress = value;

		if (null == value)
			imAddress = "";
	}

	public String getLabel() {
		return label;
	}

	public void setLabel(String value) {
		label = value;

		if (null == value)
			label = "";
	}

	@Override
	public void Read(ByteReader reader) {
		BaseFromStream(reader);

		type = reader.ReadString();
		imAddress = reader.ReadString();
		label = reader.ReadString();
	}
}
