package daemon.model.pimOms;

import daemon.protocol.ByteReader;

public class PostalAddress extends MethodBase {
	private int type;
	private String displayAddress;
	private String label;

	public PostalAddress() {
		setKind(MethodKind.PostalAddress);
		type = 1;
		displayAddress = "";		
		label = "";
	}

	public int getType() {
		return type;
	}

	public void setType(int value) {		
			type = value;
	}

	public String getDisplayAddress() {
		return displayAddress;
	}

	public void setDisplayAddress(String value) {
		displayAddress = value;

		if (null == value)
			displayAddress = "";
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

		type = reader.ReadInt32();
		label = reader.ReadString();
		displayAddress = reader.ReadString();		
	}
}
