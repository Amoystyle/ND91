package daemon.model;

import java.util.ArrayList;

import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;
import daemon.protocol.IBaseEntity;

public class EntityColloction<T> extends ArrayList<T> implements IBaseEntity {

	private Integer action;

	public EntityColloction(Integer action) {
		this.action = action;
	}

	/**
	 * ȡ��ִ�ж�����ʶ
	 * 
	 * @return
	 */
	public Integer GetAction() {
		return action;
	}

	/**
	 * �ӽ�������������򻯳�Ա
	 * 
	 * @param reader
	 *            ������
	 */
	public void Read(ByteReader reader) {

	}

	/**
	 * ͨ����������л���Ա����
	 * 
	 * @param writer������
	 */
	public void Write(ByteWriter writer) {

	}

}
