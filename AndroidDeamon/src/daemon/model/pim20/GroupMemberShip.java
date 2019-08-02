package daemon.model.pim20;

import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;

public class GroupMemberShip extends MethodBase{
	
	private Integer id;
	private Integer personId;
	private Integer groupId;
	
	public GroupMemberShip(){
		setKind(MethodKind.GroupMembership);
		id = 0;
		personId = 0;
		groupId = 0;
	}
	
	public Integer getId(){
		return id;
	}
	public void setId(Integer value){
		id = value;
	}
	
	public Integer getPersonId(){
		return personId;
	}
	public void setPersonId(Integer value){
		personId = value;
	}
	
	public Integer getGroupId(){
		return groupId;
	}
	public void setGroupId(Integer value){
		groupId = value;
	}
	
	/**
	 * �ӽ�����������򻯳�Ա
	 * 
	 * @param reader
	 *            ������
	 */
	public void Read(ByteReader reader) {
		BaseFromStream(reader);	
	
		groupId = reader.ReadInt32();
	}	
}
