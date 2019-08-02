package daemon.model.pim15;

import daemon.protocol.ByteReader;

public class OrganizationInfo extends MethodBase {

	private int type;
	private String company;
	private String position;
	private String label;

	public OrganizationInfo() {
		setKind(MethodKind.Organization);
		type = 1;
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

	/**
	 * �ӽ�����������򻯳�Ա
	 * 
	 * @param reader
	 *            ������
	 */
	public void Read(ByteReader reader) {
		BaseFromStream(reader);

		type = reader.ReadInt32();
		company = reader.ReadString();
		position = reader.ReadString();
		label = reader.ReadString();
	}
}
