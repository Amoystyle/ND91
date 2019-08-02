package daemon.model.pim20;

import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;

public class Email extends MethodBase {
	private String type;
	private String label;
	private String emailAddress;

	public Email() {
		setKind(MethodKind.Email);
		type = "";
		label = "";
		emailAddress = "";
	}

	public String getType() {
		return type;
	}

	public void setType(String value) {
		if (value != null)
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
		type = reader.ReadString();
		label = reader.ReadString();
		emailAddress = reader.ReadString();
	}
}
