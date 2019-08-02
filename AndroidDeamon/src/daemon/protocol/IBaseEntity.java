package daemon.protocol;



/**
 * <p>
 * Title: IBaseEntity
 * </p>
 * <p>
 * Description: IBaseEntity
 * </p>
 * <p>
 * Copyright: Copyright (c) 2009
 * </p>
 * <p>
 * Company: nd
 * </p>
 * 
 * @author: Jason.Lee
 * @version: 1.0
 */
public interface IBaseEntity {

	/**
	 * ȡ��ִ�ж�����ʶ
	 * @return
	 */
	Integer GetAction();

	/**
	 * �ӽ�������������򻯳�Ա
	 * @param reader ������
	 */
	void Read(ByteReader reader);
   
	/**
	 * ͨ����������л���Ա����
	 * @param writer������
	 */
	void Write(ByteWriter writer);

}
