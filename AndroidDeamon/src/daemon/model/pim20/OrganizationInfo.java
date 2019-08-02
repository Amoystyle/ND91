package daemon.model.pim20;

import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;

public class OrganizationInfo extends MethodBase {

	private String type;
	private String company;
	private String position;
	private String label;

	public OrganizationInfo() {
		setKind(MethodKind.Organization);
		type = "";
		company = "";
		position = "";
		label = "";
	}

	public String getCompany() {
		return company;
	}

	public void setCompany(String value) {
		company = value;

		if (null == value)
			company = "";
	}

	public String getPosition() {
		return position;
	}

	public void setPosition(String value) {
		position = value;

		if (null == value)
			position = "";
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

	/**
	 * �ӽ�����������򻯳�Ա
	 * 
	 * @param reader
	 *            ������
	 */
	public void Read(ByteReader reader) {
		BaseFromStream(reader);

		type = reader.ReadString();
		company = reader.ReadString();
		position = reader.ReadString();
		label = reader.ReadString();
	}
}
