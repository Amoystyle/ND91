package daemon.model.pim20;

import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;

public class PhoneInfo extends MethodBase {

	private String type;
	private String number;
	private String label;

	public PhoneInfo() {
		setKind(MethodKind.Phone);
		number = "";	
		label = "";
	}

	public String getType() {
		return type;
	}

	public void setType(String value) {
		if (value != null)
			type = value;
	}

	public String getNumber() {
		return number;
	}

	public void setNumber(String value) {
		if (value == null) {
			number = "";
		} else {
			number = value;
		}
	}

	public String getLabel() {
		return label;
	}

	public void setLabel(String value) {
		if (null == value) {
			label = "";
		} else {
			label = value;
		}
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
		number = reader.ReadString();
		label = reader.ReadString();
	}
}
