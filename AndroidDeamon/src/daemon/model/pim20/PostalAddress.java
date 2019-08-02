package daemon.model.pim20;

import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;

public class PostalAddress extends MethodBase {
	private String type;
	private String displayAddress;
	private String street;
	private String city;
	private String state;
	private String zipCode;
	private String label;

	public PostalAddress() {
		setKind(MethodKind.PostalAddress);
		type = "";
		displayAddress = "";
		street = "";
		city = "";
		state = "";
		zipCode = "";
		label = "";
	}

	public String getType() {
		return type;
	}

	public void setType(String value) {
		if (value != null)
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

	public String getStreet() {
		return street;
	}

	public void setStreet(String value) {
		street = value;

		if (null == value)
			street = "";
	}

	public String getCity() {
		return city;
	}

	public void setCity(String value) {
		city = value;

		if (null == value)
			city = "";
	}

	public String getState() {
		return state;
	}

	public void setState(String value) {
		state = value;

		if (null == value)
			state = "";
	}

	public String getZipCode() {
		return zipCode;
	}

	public void setZipCode(String value) {
		zipCode = value;

		if (null == value)
			zipCode = "";
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
		label = reader.ReadString();
		displayAddress = reader.ReadString();
		street = reader.ReadString();
		city = reader.ReadString();
		state = reader.ReadString();
		zipCode = reader.ReadString();
	}
}
