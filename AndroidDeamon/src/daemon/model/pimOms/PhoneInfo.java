package daemon.model.pimOms;

import daemon.protocol.ByteReader;

public class PhoneInfo extends MethodBase {

	private int type;
	private String number;
	private String label;

	public PhoneInfo() {
		setKind(MethodKind.Phone);
		number = "";	
		label = "";
	}

	public int getType() {
		return type;
	}

	public void setType(int value) {		
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

		type = reader.ReadInt32();
		number = reader.ReadString();
		label = reader.ReadString();
	}
}
