package daemon.model.pim15;

import daemon.protocol.ByteReader;

public class Email extends MethodBase {
	private int type;
	private String label;
	private String emailAddress;

	public Email() {
		setKind(MethodKind.Email);
		type = 1;
		label = "";
		emailAddress = "";
	}

	public int getType() {
		return type;
	}

	public void setType(int value) {		
			type = value;
	}

	public String getLabel() {
		return label;
	}

	public void setLabel(String value) {
		label = value;

		if (null == value)
			label = "";
	}

	public String getEmailAddress() {
		return emailAddress;
	}

	public void setEmailAddress(String value) {
		emailAddress = value;
	}

	@Override
	public void Read(ByteReader reader) {
		BaseFromStream(reader);
		type = reader.ReadInt32();
		label = reader.ReadString();
		emailAddress = reader.ReadString();
	}
}
