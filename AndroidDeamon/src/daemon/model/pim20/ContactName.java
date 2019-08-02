package daemon.model.pim20;

import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;

public class ContactName extends MethodBase {
	private String displayName;
	private String givenName;
	private String familyName;
	private String middleName;

	public ContactName() {
		setKind(MethodKind.Name);
		displayName = "";
		givenName = "";
		familyName = "";
		middleName = "";
	}

	public String getDisplayName() {
		return displayName;
	}

	public void setDisplayName(String value) {
		displayName = value;

		if (null == value)
			displayName = "";
	}

	public String getGivenName() {
		return givenName;
	}

	public void setGivenName(String value) {
		givenName = value;

		if (null == value)
			givenName = "";
	}

	public String getFamilyName() {
		return familyName;
	}

	public void setFamilyName(String value) {
		familyName = value;

		if (null == value)
			familyName = "";
	}

	public String getMiddleName() {
		return middleName;
	}

	public void setMiddleName(String value) {
		middleName = value;

		if (middleName == null)
			middleName = "";
	}

	@Override
	public void Read(ByteReader reader) {
		BaseFromStream(reader);
		displayName = reader.ReadString();
		givenName = reader.ReadString();
		familyName = reader.ReadString();
		middleName = reader.ReadString();
	}
}
